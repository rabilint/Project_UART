//
// Created by rabilint on 04.04.26.
//

#include "bmp180.h"

#include <math.h>

uint8_t BMP180_Init(BMP180_t *dev, I2C_HandleTypeDef *hi2c, float AtmospherePressure)
{
    dev->hi2c = hi2c;
    dev->AtmospherePressure = AtmospherePressure;
    uint8_t chip_id = 0;
    if (HAL_I2C_Mem_Read(dev->hi2c, BMP180_ADDR, 0xD0, 1, &chip_id, 1, 100) != HAL_OK || chip_id != 0x55) {
        return 1;
    }
    uint8_t calib[22] = {0};
    for(uint8_t i = 0; i < 22; i++) {
        if(HAL_I2C_Mem_Read(dev->hi2c, BMP180_ADDR, 0xAA + i, I2C_MEMADD_SIZE_8BIT, &calib[i], 1, 50) != HAL_OK) {
            return 1;
        }
    }

    dev->AC1 = (int16_t)((calib[0] << 8) | calib[1]);
    dev->AC2 = (int16_t)((calib[2] << 8) | calib[3]);
    dev->AC3 = (int16_t)((calib[4] << 8) | calib[5]);
    dev->AC4 = (uint16_t)((calib[6] << 8) | calib[7]);
    dev->AC5 = (uint16_t)((calib[8] << 8) | calib[9]);
    dev->AC6 = (uint16_t)((calib[10] << 8) | calib[11]);
    dev->B1  = (int16_t)((calib[12] << 8) | calib[13]);
    dev->B2  = (int16_t)((calib[14] << 8) | calib[15]);
    dev->MB  = (int16_t)((calib[16] << 8) | calib[17]);
    dev->MC  = (int16_t)((calib[18] << 8) | calib[19]);
    dev->MD  = (int16_t)((calib[20] << 8) | calib[21]);

    return 0;
}

void BMP180_Read(BMP180_t *dev, uint8_t oss)
{
    uint8_t data[3] = {0};
    uint8_t cmd;
    int32_t UT, UP, X1, X2, X3, B3, B6, p;
    uint32_t B4, B7;

    //Get UT
    cmd = 0x2E;
    if (HAL_I2C_Mem_Write(dev->hi2c, BMP180_ADDR, 0xF4, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100) != HAL_OK) return;
    HAL_Delay(5);
    if (HAL_I2C_Mem_Read(dev->hi2c, BMP180_ADDR, 0xF6, I2C_MEMADD_SIZE_8BIT, data, 2, 100) != HAL_OK) return;
    UT = (data[0] << 8) | data[1];

    //Get real tmp
    X1 = ((UT - (int32_t)dev->AC6) * (int32_t)dev->AC5) / 32768;
    X2 = ((int32_t)dev->MC * 2048) / (X1 + dev->MD);
    dev->B5 = X1 + X2;
    dev->temperature = (dev->B5 + 8) / 160.0f;

    //Get UP
    cmd = 0x34 + (oss << 6);
    if (HAL_I2C_Mem_Write(dev->hi2c, BMP180_ADDR, 0xF4, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100) != HAL_OK) return;

    uint32_t delays[] = {5, 8, 14, 26};
    HAL_Delay(delays[oss & 0x03] + 2);

    if (HAL_I2C_Mem_Read(dev->hi2c, BMP180_ADDR, 0xF6, I2C_MEMADD_SIZE_8BIT, data, 3, 100) != HAL_OK) return;
    UP = (((uint32_t)data[0] << 16) | ((uint32_t)data[1] << 8) | data[2]) >> (8 - oss);

    //Get real pressure
    B6 = dev->B5 - 4000;
    X1 = ((int32_t)dev->B2 * ((B6 * B6) / 4096)) / 2048;
    X2 = ((int32_t)dev->AC2 * B6) / 2048;
    X3 = X1 + X2;
    B3 = (((((int32_t)dev->AC1 * 4) + X3) << oss) + 2) / 4;

    X1 = ((int32_t)dev->AC3 * B6) / 8192;
    X2 = ((int32_t)dev->B1 * ((B6 * B6) / 4096)) / 65536;
    X3 = ((X1 + X2) + 2) / 4;
    B4 = ((uint32_t)dev->AC4 * (uint32_t)(X3 + 32768)) / 32768;

    B7 = ((uint32_t)UP - (uint32_t)B3) * (50000 >> oss);
    if (B7 < 0x80000000) p = (B7 * 2) / B4;
    else p = (B7 / B4) * 2;

    X1 = (p / 256) * (p / 256);
    X1 = (X1 * 3038) / 65536;
    X2 = (-7357 * p) / 65536;
    dev->pressure = p + ((X1 + X2 + 3791) / 16);

    if (dev->AtmospherePressure > 0) {
        float pressure_ratio = (float)dev->pressure / (int32_t)dev->AtmospherePressure;
        dev->Alt = 44330.0f * (1.0f - powf(pressure_ratio, 0.19029495718f));
    }
}