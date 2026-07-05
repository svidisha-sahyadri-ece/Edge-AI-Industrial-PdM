#ifndef INC_ADXL345_H_
#define INC_ADXL345_H_

#define ADXL345_ADDR (0x53 << 1)

#include "stm32f4xx_hal.h"
#include <stdint.h>

/* I2C Address */
#define ADXL345_DEVICE_ADDRESS      0x53
#define ADXL345_I2C_SLAVE_ADDRESS   (ADXL345_DEVICE_ADDRESS << 1)

/* Register Map */
#define ADXL345_DEVID        0x00
#define ADXL345_BW_RATE      0x2C
#define ADXL345_POWER_CTL    0x2D
#define ADXL345_DATA_FORMAT  0x31
#define ADXL345_DATAX0       0x32

typedef enum
{
    ADXL_ERR = 0,
    ADXL_OK
} ADXL_Status;

/* Public Functions */
ADXL_Status ADXL345_Init(I2C_HandleTypeDef *hi2c);
void ADXL345_ReadXYZ(float *x, float *y, float *z);
void ADXL_SetRange(uint8_t range);

#endif
