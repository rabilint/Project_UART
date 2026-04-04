//
// Created by rabilint on 04.04.26.
//

#ifndef PROJECT_UART_BMP180_H
#define PROJECT_UART_BMP180_H

#include "stm32f4xx_hal.h"
#define BMP180_ADDR 0xEE

typedef struct {
    I2C_HandleTypeDef *hi2c;

    int AC1, AC2, AC3, B1, B2, MB, MC, MD;
    uint16_t AC4, AC5, AC6;
    int32_t B5;

    int32_t pressure;
    float temperature;
    float Alt;
    int32_t AtmospherePressure;
} BMP180_t;

uint8_t BMP180_Init(BMP180_t *dev, I2C_HandleTypeDef *hi2c,float AtmospherePressure);
void BMP180_Read(BMP180_t *dev, uint8_t oss);

#endif //PROJECT_UART_BMP180_H
