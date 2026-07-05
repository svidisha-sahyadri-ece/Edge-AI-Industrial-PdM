#include "ADXL345.h"

static I2C_HandleTypeDef *adxl_i2c;
static float scaleFactor = 0.004f;   // 4mg per LSB (FULL_RES)

/* ================= LOW LEVEL ================= */

static void ADXL_Write(uint8_t reg, uint8_t value)
{
    HAL_I2C_Mem_Write(adxl_i2c,
                      ADXL345_I2C_SLAVE_ADDRESS,
                      reg,
                      1,
                      &value,
                      1,
                      HAL_MAX_DELAY);
}

static void ADXL_Read(uint8_t reg, uint8_t *data, uint8_t length)
{
    HAL_I2C_Mem_Read(adxl_i2c,
                     ADXL345_I2C_SLAVE_ADDRESS,
                     reg,
                     1,
                     data,
                     length,
                     HAL_MAX_DELAY);
}

/* ================= INITIALIZATION ================= */

ADXL_Status ADXL345_Init(I2C_HandleTypeDef *hi2c)
{
    adxl_i2c = hi2c;

    uint8_t deviceID = 0;

    ADXL_Read(ADXL345_DEVID, &deviceID, 1);

    if (deviceID != 0xE5)
        return ADXL_ERR;

    /* Data rate = 200 Hz */
    ADXL_Write(ADXL345_BW_RATE, 0x0C);

    /* FULL_RES + ±4g (start range) */
    ADXL_Write(ADXL345_DATA_FORMAT, 0x09);

    /* Measurement mode */
    ADXL_Write(ADXL345_POWER_CTL, 0x08);

    scaleFactor = 0.004f;   // 4mg/LSB

    return ADXL_OK;
}

/* ================= RANGE CONTROL ================= */

void ADXL_SetRange(uint8_t range)
{
    uint8_t data = 0x08 | range;   // FULL_RES + range bits
    ADXL_Write(ADXL345_DATA_FORMAT, data);
}

/* ================= READ XYZ ================= */

void ADXL345_ReadXYZ(float *x, float *y, float *z)
{
    uint8_t rawData[6];
    int16_t xRaw, yRaw, zRaw;

    ADXL_Read(ADXL345_DATAX0, rawData, 6);

    xRaw = (int16_t)((rawData[1] << 8) | rawData[0]);
    yRaw = (int16_t)((rawData[3] << 8) | rawData[2]);
    zRaw = (int16_t)((rawData[5] << 8) | rawData[4]);

    *x = xRaw * scaleFactor;
    *y = yRaw * scaleFactor;
    *z = zRaw * scaleFactor;
}
