/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ADXL345.h"
#include <stdio.h>
#include <math.h>
#include "ds18b20.h"
#include <string.h>
#include "arm_math.h"
#ifdef AI_MODEL_DEPLOYED
#include "ae_model.h"
#include "ai_platform.h"
#include "ae_model_data.h"
#endif

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SAMPLES         2048
#define RATED_RPM       1500
#define PULSES_PER_REV  1
#define MAX_RPM         3000
#define RPM_STOP_TIMEOUT_US 3000000
#define MIN_PULSE_US    (60000000 / (MAX_RPM * PULSES_PER_REV))
#define VIB_WINDOW      128
#define WARMUP_WINDOWS  6
#define ACS_SENSITIVITY 0.185f
#define KNOWN_CURRENT   0.444f
#define FFT_SIZE        VIB_WINDOW
/* ========= MACHINE CONFIGURATION ========= */



/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* CMSIS FFT Buffers */
arm_rfft_fast_instance_f32 fft_handler;
float fft_input[FFT_SIZE];
float fft_output[FFT_SIZE];
/* ADC / Voltage / Current */
uint16_t adcBuffer[SAMPLES];
float calibrationFactor = 1.0f;
volatile uint8_t streamEnable = 0;
volatile uint8_t adcHalfReady = 0;
volatile uint8_t adcFullReady = 0;
float realVoltage = 0;
float current = 0;
float baseline_A = 0.0f;
float scaleFactor = 1.0f;
uint16_t dbg_max = 0, dbg_min = 0, dbg_peak = 0;
volatile uint8_t uartPrintReady = 0;
typedef enum {
    MODE_STREAM = 0,
    MODE_AI,
    MODE_CALIBRATE
} SystemMode_t;

volatile SystemMode_t currentMode = MODE_STREAM;
uint8_t rxByte;
/* Accelerometer */
float ax, ay, az;
uint8_t current_range = 0x01;

/* Temperature */
ds18b20_t ds18;
float temperature = 0.0f;

/* RPM */
volatile float rpm = 0.0f;
volatile uint32_t lastPulseUs = 0;

/* Vibration buffers */
float axBuffer[VIB_WINDOW];
float ayBuffer[VIB_WINDOW];
float azBuffer[VIB_WINDOW];
uint8_t magIndex = 0;
uint8_t magBufferReady = 0;
float prevTemp = 0.0f;
uint8_t prevTempValid = 0;
uint8_t warmupCount = 0;

/* Feature struct */

	typedef struct {

	    /* Time domain */
	    float vib_rms;
	    float vib_peak;
	    float vib_crest;
	    float vib_mean;
	    float vib_variance;
	    float vib_kurtosis;

	    /* Frequency domain */
	    float dominant_freq;
	    float harmonic_score;
	    float peak_count;

	    /* Sensors */
	    float temperature;
	    float temp_rise;
	    float voltage;
	    float current;
	    float rpm;

	    /* Fusion */
	    float vib_power_corr;

	} FeaturePacket_t;

FeaturePacket_t features = {0};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void runCalibration(void);
void computeFFT(float *input, int N)
{
    for (int i = 0; i < N; i++)
        fft_input[i] = input[i];
    arm_rfft_fast_f32(&fft_handler, fft_input, fft_output, 0);
}
void sendToESP32(FeaturePacket_t *f,
                 float mse, float health, float confidence,
                 float anomaly_score,
                 float trend, float RUL,
                 int fault, int bearing, int thermal, int electrical, int mech);
void calibrateVoltage(float knownVoltage);
float readCurrentRMS(void);
void extractVibrationFeatures(float *xb, float *yb, float *zb, int N, FeaturePacket_t *f);
void sendFeaturePacket(FeaturePacket_t *f);
#ifdef AI_MODEL_DEPLOYED
void runInference(FeaturePacket_t *f);
#endif
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* DMA callbacks */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1) adcHalfReady = 1;
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1) adcFullReady = 1;
}

/* Process ADC samples — voltage (even index) + current (odd index) */
void processSamples(uint16_t *buffer, int length)
{
    /* --- Voltage --- */
    float mean = 0; float sum = 0; int samples = 0;
    for(int i = 0; i < length; i += 2) {
        if(buffer[i] < 100 || buffer[i] > 4000) continue;
        float v = (buffer[i] * 3.3f) / 4095.0f;
        mean += v; samples++;
    }
    mean /= samples;
    for(int i = 0; i < length; i += 2) {
        float v = (buffer[i] * 3.3f) / 4095.0f;
        float c = v - mean; sum += c * c;
    }
    realVoltage = sqrtf(sum / samples) * calibrationFactor;
    dbg_max = 0; dbg_min = 4095;
    for(int i = 0; i < length; i += 2) {
        if(buffer[i] > dbg_max) dbg_max = buffer[i];
        if(buffer[i] < dbg_min) dbg_min = buffer[i];
    }
    dbg_peak = (dbg_max - dbg_min) / 2;
    uartPrintReady = 1;

    /* --- Current --- */
    float cMean = 0; float cSum = 0; int cSamples = 0;
    for(int i = 1; i < length; i += 2) {
        float v = buffer[i] * 0.000805f; cMean += v; cSamples++;
    }
    cMean /= cSamples;
    for(int i = 1; i < length; i += 2) {
        float v = buffer[i] * 0.000805f;
        float c = v - cMean; cSum += c * c;
    }
    float rawCurrent = sqrtf(cSum / cSamples) / ACS_SENSITIVITY;
    current = (rawCurrent - baseline_A) * scaleFactor;
}

void calibrateVoltage(float knownVoltage)
{
    float sum = 0, mean = 0; int samples = 0;
    for(int i = 0; i < SAMPLES; i += 2) {
        float v = (adcBuffer[i] * 3.3f) / 4095.0f;
        mean += v; samples++;
    }
    mean /= samples;
    for(int i = 0; i < SAMPLES; i += 2) {
        float v = (adcBuffer[i] * 3.3f) / 4095.0f;
        float c = v - mean; sum += c * c;
    }
    calibrationFactor = knownVoltage / sqrtf(sum / samples);
}

float readCurrentRMS(void)
{
    float mean = 0, sum = 0; int samples = 0;
    for(int i = 1; i < SAMPLES; i += 2) {
        float v = (adcBuffer[i] * 3.3f) / 4095.0f;
        mean += v; samples++;
    }
    mean /= samples;
    for(int i = 1; i < SAMPLES; i += 2) {
        float v = (adcBuffer[i] * 3.3f) / 4095.0f;
        float c = v - mean; sum += c * c;
    }
    return sqrtf(sum / samples) / ACS_SENSITIVITY;
}

int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, 1000);
    return len;
}

void OW_TimerCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM4) ow_callback(&ds18.ow);
}

/* Vibration feature extraction */
void extractVibrationFeatures(float *xb, float *yb, float *zb, int N, FeaturePacket_t *f)
{
	float magArr[VIB_WINDOW];
	float magRaw[VIB_WINDOW];
	float magMean = 0.0f;
	for(int i = 0; i < N; i++) {
	    magArr[i] = sqrtf(xb[i]*xb[i] + yb[i]*yb[i] + zb[i]*zb[i]);
	    magRaw[i] = magArr[i];   // save raw before any modification
	    magMean += magArr[i];
	}
    magMean /= N;
    f->vib_mean = magMean;

    float sumSq = 0.0f;
    for(int i = 0; i < N; i++) sumSq += magArr[i] * magArr[i];
    f->vib_rms = sqrtf(sumSq / N);

    float peak = 0.0f;
    for(int i = 0; i < N; i++) if(magArr[i] > peak) peak = magArr[i];
    f->vib_peak = peak;

    f->vib_crest = (f->vib_rms > 0.0f) ? (f->vib_peak / f->vib_rms) : 0.0f;

    float variance = 0.0f;
    for(int i = 0; i < N; i++) { float d = magArr[i]-magMean; variance += d*d; }
    f->vib_variance = variance / (N - 1);

    /* Kurtosis — gravity removed */
    float xMean=0, yMean=0, zMean=0;
    for(int i=0;i<N;i++){xMean+=xb[i];yMean+=yb[i];zMean+=zb[i];}
    xMean/=N; yMean/=N; zMean/=N;

    float acMag[VIB_WINDOW]; float acMean=0.0f;
    for(int i=0;i<N;i++){
        float dx=xb[i]-xMean, dy=yb[i]-yMean, dz=zb[i]-zMean;
        acMag[i]=sqrtf(dx*dx+dy*dy+dz*dz); acMean+=acMag[i];
    }
    acMean/=N;

    float acVar=0.0f;
    for(int i=0;i<N;i++){float d=acMag[i]-acMean; acVar+=d*d;}
    acVar/=(N-1);

    float sigma4=acVar*acVar; float kurt=0.0f;
    if(sigma4>1e-10f){
        for(int i=0;i<N;i++){float d=acMag[i]-acMean; kurt+=d*d*d*d;}
        kurt/=(N*sigma4);
    }
    f->vib_kurtosis = kurt;
    /* ===== FFT ===== */
    /* Remove DC component */
    for(int i=0;i<N;i++)
    {
        magArr[i] -= f->vib_mean;
    }
    /* Hanning window */
    for(int i=0;i<N;i++)
    {
        float w = 0.5f * (1.0f - cosf(2 * 3.14159f * i / (N-1)));
        magArr[i] *= w;
    }
    computeFFT(magArr, N);

    /* ===== Dominant Frequency ===== */
    float maxMag = 0;
    int maxIndex = 0;
    float noise_floor = f->vib_rms * 0.1f;
    for(int i = 1; i < N/2; i++)
    {
        float real = fft_output[2*i];
        float imag = fft_output[2*i + 1];
        float mag = sqrtf(real*real + imag*imag);

        if(mag > maxMag && mag > noise_floor)
        {
            maxMag = mag;
            maxIndex = i;
        }
    }



    float Fs = 250.0f;
    f->dominant_freq = (maxIndex * Fs) / N;


    /* ===== Peak Count (IMPROVED) ===== */
    int peak_count = 0;

    /* Adaptive threshold (more stable than variance-based) */
    float threshold = f->vib_rms * 1.2f;
    for(int i = 1; i < N-1; i++)
    {
        if(magRaw[i] > magRaw[i-1] &&
           magRaw[i] > magRaw[i+1] &&
           magRaw[i] > threshold)
        {
            peak_count++;
        }
    }

    f->peak_count = peak_count;


    /* ===== Harmonic Score (SAFE & STABLE VERSION) ===== */
    float baseFreq = f->dominant_freq;
    float harmonicEnergy = 0.0f;
    float totalEnergy = 0.0f;

    if(baseFreq < 1.0f)
    {
        f->harmonic_score = 0.0f;
    }
    else
    {
    	float tol = baseFreq * 0.05f;
        for(int i = 1; i < N/2; i++)
        {
            float freq = (i * Fs) / N;

            float real = fft_output[2*i];
            float imag = fft_output[2*i + 1];
            float mag = sqrtf(real*real + imag*imag);

            totalEnergy += mag;

            float ratio = freq / baseFreq;
            int harmonic = (int)(ratio + 0.5f);
            if(fabsf(freq - harmonic * baseFreq) < tol)
            {
                harmonicEnergy += mag;
            }
        }

        if(totalEnergy > 0.0001f)
            f->harmonic_score = harmonicEnergy / totalEnergy;
        else
            f->harmonic_score = 0.0f;
    }
    /* ===== vib_power_corr (MUST BE OUTSIDE) ===== */

    float energy = 0.0f;

    for(int i = 0; i < N; i++)
    {
        energy += magRaw[i] * magRaw[i];
    }

    energy = energy / N;

    f->vib_power_corr = energy;
}


/* ============================================================
   sendFeaturePacket

   BEFORE deploy  (#ifndef AI_MODEL_DEPLOYED):
     → Pure CSV, 10 values, no prefix, no debug
     → Used by Python collect.py

   AFTER deploy   (#ifdef AI_MODEL_DEPLOYED):
     → Run autoencoder inference
     → Print "Machine: HEALTHY" or "Machine: FAULT DETECTED"
   ============================================================ */
void sendFeaturePacket(FeaturePacket_t *f)
{
    if(currentMode == MODE_STREAM)
    {
        printf("DATA,%.4f,%.4f,%.4f,%.4f,%.4f,%.2f,%.4f,%.2f,%.4f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",
            f->vib_rms,
            f->vib_peak,
            f->vib_crest,
            f->vib_variance,
            f->vib_kurtosis,
            f->temperature,
            f->temp_rise,
            f->voltage,
            f->current,
            f->rpm,
            f->dominant_freq,
            f->harmonic_score,
            f->peak_count,
            f->vib_power_corr
        );
    }
#ifdef AI_MODEL_DEPLOYED
    else if(currentMode == MODE_AI)
    {
        runInference(f);
    }
#endif
}

#ifdef AI_MODEL_DEPLOYED
/* ── Fill these after running train.py ── */
static const float SCALER_MEAN[14] = {
1.065390f, 1.301660f, 1.167326f, 0.057079f, 2.156149f,
48.700072f, 0.071920f, 229.730196f, 0.453986f, 1433.094716f,
79.831001f, 0.231053f, 3.383901f, 1.173829f
};

static const float SCALER_STD[14] = {
0.197051f, 2.291226f, 0.302844f, 1.519685f, 3.360373f,
8.431025f, 0.069864f, 1.812656f, 0.022627f, 23.341823f,
22.485306f, 0.087191f, 6.972243f, 1.594115f
};

static const float THRESHOLD = 3.70875f;

static ai_handle ae_model = AI_HANDLE_NULL;

AI_ALIGNED(4) ai_u8 activations[AI_AE_MODEL_DATA_ACTIVATIONS_SIZE];
static ai_buffer *ai_input;
static ai_buffer *ai_output;

static int fault_counter=0;
static int healthy_counter=0;

#define FAULT_CONFIRM 3
#define HEALTHY_CONFIRM 4
void runInference(FeaturePacket_t *f)
{
    static float RUL_filtered = 0.0f;

    float input[14] = {
        f->vib_rms,
        f->vib_peak,
        f->vib_crest,
        f->vib_variance,
        f->vib_kurtosis,
        f->temperature,
        f->temp_rise,
        f->voltage,
        f->current,
        f->rpm,
        f->dominant_freq,
        f->harmonic_score,
        f->peak_count,
        f->vib_power_corr
    };

    AI_ALIGNED(4) ai_float in_data[AI_AE_MODEL_IN_1_SIZE];
    AI_ALIGNED(4) ai_float out_data[AI_AE_MODEL_OUT_1_SIZE];

        /* -------- NORMALIZATION -------- */
    for(int i=0;i<14;i++)
    {
        float std = SCALER_STD[i];
        if(std < 1e-6f) std = 1.0f;

        in_data[i] = (input[i] - SCALER_MEAN[i]) / std;
    }

    /* -------- RUN MODEL -------- */


    ai_input = ai_ae_model_inputs_get(ae_model, NULL);
    ai_output = ai_ae_model_outputs_get(ae_model, NULL);

    ai_input[0].data = AI_HANDLE_PTR(in_data);
    ai_output[0].data = AI_HANDLE_PTR(out_data);

    ai_ae_model_run(ae_model, ai_input, ai_output);

    /* -------- MSE -------- */
    float mse = 0.0f;

    for(int i = 0; i < 14; i++)
    {
        float diff = in_data[i] - out_data[i];
        mse += diff * diff;
    }

    mse = mse / 14.0f;

    /* safety clamp (prevents rare instability) */
    if(mse < 0.0f) mse = 0.0f;
    if(mse > 100.0f) mse = 100.0f;   // normalized space clamp (IMPORTANT FIX)


    /* -------- NORMALIZED ANOMALY SCORE -------- */
    float anomaly_score = mse / THRESHOLD;   // >1 = abnormal region

    /* -------- HEALTH (0–100, smooth + bounded) -------- */
    float health;

    if(anomaly_score <= 1.0f)
    {
        health = 100.0f * powf((1.0f - anomaly_score), 0.5f);
    }
    else
    {
        health = 100.0f * expf(1.0f - anomaly_score);
    }

    /* clamp */
    if(health < 0.0f) health = 0.0f;
    if(health > 100.0f) health = 100.0f;


    /* -------- CONFIDENCE (REAL PROBABILITY-LIKE METRIC) -------- */
    float confidence = 1.0f / (1.0f + anomaly_score);


    /* -------- ANOMALY LOGIC (ROBUST HYSTERESIS) -------- */
    uint8_t anomaly = (anomaly_score > 1.0f);

    if(anomaly)
    {
        fault_counter++;
        healthy_counter = 0;
    }
    else
    {
        healthy_counter++;
        fault_counter = 0;
    }


    /* -------- PRINT STATUS -------- */
    if(fault_counter >= FAULT_CONFIRM)
    {
    	//printf("MSE=%.4f | Score=%.3f | Health=%.1f | RUL=%.1f\r\n",
    	//       mse, anomaly_score, health, RUL_filtered);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    }
    else if(healthy_counter >= HEALTHY_CONFIRM)
    {
    	//printf("MSE=%.4f | Score=%.3f | Health=%.1f | RUL=%.1f\r\n",
    	//       mse, anomaly_score, health, RUL_filtered);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }

    /* -------- TREND (SINGLE SOURCE OF TRUTH) -------- */
    static float mse_prev = -1.0f;
    static float trend_filtered = 0.0f;
    static float mse_filtered = 0.0f;

    float trend = 0.0f;

    /* smooth MSE first (VERY IMPORTANT FIX) */
    mse_filtered = 0.90f * mse_filtered + 0.10f * mse;

    /* compute trend on filtered signal */
    if(mse_prev >= 0.0f)
    {
        trend = mse_filtered - mse_prev;
    }
    mse_prev = mse_filtered;

    /* smooth absolute trend */
    trend_filtered = 0.93f * trend_filtered + 0.07f * fabsf(trend);

    /* safety floor */
    float degradation_rate = trend_filtered + 1e-3f;


    /* -------- HEALTH (already computed earlier) -------- */
    float health_norm = health * 0.01f;   // faster + cheaper than /100


    /* -------- RUL (STABLE, NON-EXPLOSIVE MODEL) -------- */

    /*
       Key idea:
       - No log()
       - No exponential inversion
       - bounded inverse degradation model
    */

    float RUL = 0.0f;

    /* core stability equation */
    RUL = 1500.0f * health_norm / (1.0f + 6.0f * degradation_rate);

    /* mild protection for fresh startup */
    if(trend_filtered < 1e-5f)
    {
        RUL = 5000.0f;
    }

    /* clamp */
    if(RUL > 5000.0f) RUL = 5000.0f;
    if(RUL < 0.0f) RUL = 0.0f;


    RUL_filtered = 0.9f * RUL_filtered + 0.1f * RUL;


    /* -------- DTC (UNCHANGED BUT SAFE) -------- */
    /* ===== FLAGS ===== */
    int fault = (fault_counter >= FAULT_CONFIRM);

    int bearing = (f->vib_kurtosis > 4.5f && health < 70.0f);
    int thermal = (f->temp_rise > 3.0f);
    int electrical = (f->current > 1.5f && f->vib_rms < 0.8f);
    int mech = (f->harmonic_score > 0.5f && f->peak_count > 5);

    /* -------- SEND JSON (FINAL FIX) -------- */
    sendToESP32(
        f,
        mse,
        health,
        confidence,
        anomaly_score,
        trend_filtered,
        RUL_filtered,
        fault,
        bearing,
        thermal,
        electrical,
        mech
    );
}
#endif
extern UART_HandleTypeDef huart2;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM4_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_I2C1_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */
  arm_rfft_fast_init_f32(&fft_handler, FFT_SIZE);

  /* ADXL345 */
   printf("ADXL345 Test Starting...\r\n");
   if(ADXL345_Init(&hi2c1) != ADXL_OK)
   {
       printf("ADXL Unit Failed!\r\n");
       Error_Handler();
   }
   else
   {
       printf("ADXL Unit Success!\r\n");
   }
   uint8_t rate = 0x0D;   // 800 Hz output data rate
   HAL_I2C_Mem_Write(&hi2c1, ADXL345_ADDR, 0x2C, 1, &rate, 1, 100);
  /* DS18B20 */
  ow_init_t ow_init_struct;
  ow_init_struct.tim_handle   = &htim4;
  ow_init_struct.gpio         = GPIOC;
  ow_init_struct.pin          = GPIO_PIN_8;
  ow_init_struct.tim_cb       = OW_TimerCallback;
  ow_init_struct.done_cb      = NULL;
  ow_init_struct.rom_id_filter= DS18B20_ID;
  ds18b20_init(&ds18, &ow_init_struct);
  HAL_TIM_Base_Start_IT(&htim4);
  ds18b20_update_rom_id(&ds18);
  while(ds18b20_is_busy(&ds18));
  HAL_Delay(2000);

  /* ADC DMA */
  HAL_TIM_Base_Start(&htim3);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcBuffer, SAMPLES);
  HAL_Delay(1000);

  /* TIM2 microsecond counter */
  HAL_TIM_Base_Start(&htim2);

  /* ACS712 Calibration */
    printf("\n=== ACS712 Auto Calibration ===\r\n");

    printf("Remove load... Measuring baseline...\r\n");
    HAL_Delay(3000);
    baseline_A = readCurrentRMS();
    printf("Baseline: %.4f A\r\n", baseline_A);

    printf("Apply 444 mA load now...\r\n");
    HAL_Delay(8000);
    float loaded_A = readCurrentRMS();
    printf("Loaded: %.4f A\r\n", loaded_A);

    float measuredDelta = loaded_A - baseline_A;
    if(measuredDelta < 0.001f) measuredDelta = 0.001f;
    scaleFactor = KNOWN_CURRENT / measuredDelta;
    printf("Scale Factor: %.3f\r\n", scaleFactor);

    /* Voltage calibration */
    calibrateVoltage(230.0f);   // prints "Calibration Factor: X.XXX" inside
    HAL_Delay(1000);
  /* UART RX interrupt */
  HAL_UART_Receive_IT(&huart2, &rxByte, 1);
#ifdef AI_MODEL_DEPLOYED

ai_network_params ai_params =
{
AI_AE_MODEL_DATA_WEIGHTS(ai_ae_model_data_weights_get()),
AI_AE_MODEL_DATA_ACTIVATIONS(activations)
};


ai_error err;

err = ai_ae_model_create(&ae_model, AI_AE_MODEL_DATA_CONFIG);

if (err.type != AI_ERROR_NONE)
{
    printf("MODEL CREATE FAILED: type=%d code=%d\r\n", err.type, err.code);
    while(1);
}

if(!ai_ae_model_init(ae_model,&ai_params))
{
    printf("AI INIT FAILED\r\n");
}

#endif
  printf("STM32_READY\r\n");
  streamEnable = 1;
    // force clean state
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(currentMode == MODE_CALIBRATE)
	  {
	      runCalibration();
	  }

	       uint32_t now = HAL_GetTick();
	      // Then at the top of while(1) in main:

	       /* ── Vibration sampling every 2ms ── */
	       static uint32_t lastVibTime = 0;

	       if(now - lastVibTime >= 2)
	       {
	           lastVibTime = now;

	           ADXL345_ReadXYZ(&ax, &ay, &az);

	           /* Sensor validation */
	           if(isnan(ax) || isnan(ay) || isnan(az))
	               continue;



	           /* Store valid data */
	           axBuffer[magIndex] = ax;
	           ayBuffer[magIndex] = ay;
	           azBuffer[magIndex] = az;

	           magIndex++;

	           if(magIndex >= VIB_WINDOW)
	           {
	               magIndex = 0;
	               magBufferReady = 1;
	           }

	           /* Adaptive range every 1s */
	           static uint32_t lastRangeTime = 0;
	           if(now - lastRangeTime >= 1000)
	           {
	               lastRangeTime = now;

	               float rms_value = sqrtf(ax*ax + ay*ay + az*az);

	               if(rms_value > 3.2f && current_range < 0x02)
	               {
	                   current_range++;
	                   ADXL_SetRange(current_range);
	               }
	               else if(rms_value < 0.8f && current_range > 0x00)
	               {
	                   current_range--;
	                   ADXL_SetRange(current_range);
	               }
	           }
	       }
	       /* ── RPM timeout ── */
	       uint32_t nowUs = __HAL_TIM_GET_COUNTER(&htim2);

	       if(lastPulseUs != 0)
	       {
	           uint32_t elapsedUs;

	           if(nowUs >= lastPulseUs)
	               elapsedUs = nowUs - lastPulseUs;
	           else
	               elapsedUs = (0xFFFFFFFF - lastPulseUs) + nowUs + 1;

	           /* If no pulse for 1 sec → stopped */
	           if(elapsedUs > RPM_STOP_TIMEOUT_US)
	           {
	        	   rpm *= 0.97f;  // gradual decay instead of sudden zero

	               if(rpm < 10.0f)
	               {
	                   rpm = 0.0f;
	                   lastPulseUs = 0;
	               }
	           }
	       }

	       /* ── Temperature every 1s ── */
	       static uint32_t lastTempTime = 0;
	       if(now - lastTempTime >= 1000)
	       {
	           lastTempTime = now;
	           ds18b20_cnv(&ds18);
	           while(ds18b20_is_busy(&ds18));
	           while(!ds18b20_is_cnv_done(&ds18));
	           ds18b20_req_read(&ds18, 0);
	           while(ds18b20_is_busy(&ds18));
	           int16_t temp_raw = ds18b20_read_c(&ds18);
	           if(temp_raw != DS18B20_ERROR)
	               temperature = temp_raw / 100.0f;
	       }

	       /* ── ADC processing ── */
	       if(adcHalfReady) { adcHalfReady=0; processSamples(&adcBuffer[0], SAMPLES/2); }
	       if(adcFullReady) { adcFullReady=0; processSamples(&adcBuffer[SAMPLES/2], SAMPLES/2); }

	       /* ── Feature extraction every 640ms ── */
	  if(magBufferReady)
	  	  {
	  	      magBufferReady = 0;

	  	      if(warmupCount < WARMUP_WINDOWS)
	  	      {
	  	          warmupCount++;
	  	      }
	  	      else
	  	      {
	  	    	float localX[VIB_WINDOW], localY[VIB_WINDOW], localZ[VIB_WINDOW];

	  	    	__disable_irq();
	  	    	memcpy(localX, axBuffer, sizeof(axBuffer));
	  	    	memcpy(localY, ayBuffer, sizeof(ayBuffer));
	  	    	memcpy(localZ, azBuffer, sizeof(azBuffer));
	  	    	__enable_irq();

	  	    	/* Extract vibration features */
	  	    	extractVibrationFeatures(localX, localY, localZ, VIB_WINDOW, &features);

	  	    	/* Temperature already updated outside */
	  	    	features.temperature = temperature;

	  	    	/* Temperature rise */
	  	    	if(prevTempValid)
	  	    	{
	  	    	    features.temp_rise = temperature - prevTemp;
	  	    	}
	  	    	else
	  	    	{
	  	    	    features.temp_rise = 0;
	  	    	    prevTempValid = 1;
	  	    	}

	  	    	prevTemp = temperature;

	  	    	/* Snapshot values (IMPORTANT) */
	  	    	float rpm_snapshot = rpm;

	  	    	/* Assign electrical + speed */
	  	    	features.voltage = realVoltage;
	  	    	features.current = current;
	  	    	features.rpm     = rpm_snapshot;

	  	    	/* Send data */
	  	    	sendFeaturePacket(&features);
	  	      }
	  	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART2)
    {
        switch(rxByte)
        {
            case 's':
                currentMode = MODE_STREAM;
                printf("MODE: STREAM\r\n");
                break;

            case 'a':
                currentMode = MODE_AI;
                printf("MODE: AI\r\n");
                break;

            case 'c':
                currentMode = MODE_CALIBRATE;
                printf("MODE: CALIBRATION\r\n");
                break;
        }

        HAL_UART_Receive_IT(&huart2, &rxByte, 1);
    }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM4)
    {
        OW_TimerCallback(htim);
    }
}

void runCalibration(void)
{
    printf("\n=== CALIBRATION START ===\r\n");

    printf("\n=== ACS712 Auto Calibration ===\r\n");

      printf("Remove load... Measuring baseline...\r\n");
      HAL_Delay(3000);
      baseline_A = readCurrentRMS();
      printf("Baseline: %.4f A\r\n", baseline_A);

      printf("Apply 444 mA load now...\r\n");
      HAL_Delay(8000);
      float loaded_A = readCurrentRMS();
      printf("Loaded: %.4f A\r\n", loaded_A);

      float measuredDelta = loaded_A - baseline_A;
      if(measuredDelta < 0.001f) measuredDelta = 0.001f;
      scaleFactor = KNOWN_CURRENT / measuredDelta;
      printf("Scale Factor: %.3f\r\n", scaleFactor);

      /* Voltage calibration */
      calibrateVoltage(230.0f);
    printf("=== DONE ===\r\n");

    currentMode = MODE_STREAM;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_1)   // ⚠️ change if using different pin
    {
        uint32_t nowUs = __HAL_TIM_GET_COUNTER(&htim2);

        if(lastPulseUs != 0)
        {
            uint32_t diffUs;

            if(nowUs >= lastPulseUs)
                diffUs = nowUs - lastPulseUs;
            else
                diffUs = (0xFFFFFFFF - lastPulseUs) + nowUs + 1;

            /* Ignore noise pulses */
            if(diffUs >= MIN_PULSE_US && diffUs < 1000000)
            {
                float newRpm = 60000000.0f / (float)(diffUs * PULSES_PER_REV);

                /* Limit unrealistic RPM */
                if(newRpm <= RATED_RPM)
                {
                	float alpha = 0.2f;
                	        rpm = (1 - alpha) * rpm + alpha * newRpm;
                }

                lastPulseUs = nowUs;
            }
        }
        else
        {
            lastPulseUs = nowUs;
        }
    }
}
  // ✅ MUCH smaller

    void sendToESP32(FeaturePacket_t *f,
                     float mse, float health, float confidence,
                     float anomaly_score,
                     float trend, float RUL,
                     int fault, int bearing, int thermal, int electrical, int mech)
    {
        char buffer[900];   // increased size (IMPORTANT)

        sprintf(buffer,
            "JSON,\r\n"
            "{\r\n"

            "  \"mse\": %.4f,\r\n"
            "  \"health\": %.2f,\r\n"
            "  \"confidence\": %.2f,\r\n"
            "  \"score\": %.3f,\r\n"
            "  \"trend\": %.6f,\r\n"
            "  \"rul\": %.2f,\r\n"

            "  \"fault\": %d,\r\n"
            "  \"bearing\": %d,\r\n"
            "  \"thermal\": %d,\r\n"
            "  \"electrical\": %d,\r\n"
            "  \"mechanical\": %d,\r\n"

            "  \"vibration\": %.4f,\r\n"
            "  \"temperature\": %.2f,\r\n"
            "  \"temp_rise\": %.2f,\r\n"
            "  \"current\": %.4f,\r\n"
            "  \"rpm\": %.1f,\r\n"

            "  \"f0_vib_rms\": %.4f,\r\n"
            "  \"f1_vib_peak\": %.4f,\r\n"
            "  \"f2_vib_crest\": %.4f,\r\n"
            "  \"f3_vib_variance\": %.4f,\r\n"
            "  \"f4_vib_kurtosis\": %.4f,\r\n"
            "  \"f5_temp\": %.2f,\r\n"
            "  \"f6_temp_rise\": %.2f,\r\n"
            "  \"f7_voltage\": %.2f,\r\n"
            "  \"f8_current\": %.4f,\r\n"
            "  \"f9_rpm\": %.1f,\r\n"
            "  \"f10_dom_freq\": %.2f,\r\n"
            "  \"f11_harmonic\": %.4f,\r\n"
            "  \"f12_peak_count\": %d,\r\n"
            "  \"f13_power_corr\": %.4f\r\n"

            "}\r\n",

            /* AI */
            mse, health, confidence, anomaly_score, trend, RUL,

            /* FLAGS */
            fault, bearing, thermal, electrical, mech,

            /* RAW */
            f->vib_rms,
            f->temperature,
            f->temp_rise,
            f->current,
            f->rpm,

            /* FEATURES */
            f->vib_rms,
            f->vib_peak,
            f->vib_crest,
            f->vib_variance,
            f->vib_kurtosis,
            f->temperature,
            f->temp_rise,
            f->voltage,
            f->current,
            f->rpm,
            f->dominant_freq,
            f->harmonic_score,
			(int) f->peak_count,
            f->vib_power_corr
        );

        HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
    }

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
