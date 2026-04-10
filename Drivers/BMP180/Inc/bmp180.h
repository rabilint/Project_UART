//
// Created by rabilint on 04.04.26.
//

#ifndef PROJECT_UART_BMP180_H
#define PROJECT_UART_BMP180_H

#include "stm32f4xx_hal.h"

#define BMP180_ADDR 0xEE

typedef enum {
    BMP180_IDLE,

    BMP180_START_TEMP,
    BMP180_PROCESS_TEMP,
    BMP180_READ_TEMP,

    BMP180_START_PRESSURE,
    BMP180_PROCESS_PRESSURE,
    BMP180_READ_PRESSURE,

    BMP180_GET_ALTITUDE,

    BMP180_DATA_READY
} BMP180_State_t;

typedef struct {
    I2C_HandleTypeDef *hi2c;

    int16_t AC1, AC2, AC3, B1, B2, MB, MC, MD;
    uint16_t AC4, AC5, AC6;
    int32_t B5;

    int32_t pressure;
    float temperature;
    float Alt;
    int32_t AtmospherePressure;

    BMP180_State_t state;
    uint32_t start_tick;
    uint8_t i2c_buffer[3]; // buffer for I2C
    uint8_t current_oss;   // current oss
    int32_t UT;            // raw temperature
    int32_t UP;            // raw pressure

} BMP180_t;

uint8_t BMP180_Init(BMP180_t *dev, I2C_HandleTypeDef *hi2c,float AtmospherePressure);
void BMP180_Read_Blocking(BMP180_t *dev, uint8_t oss);
float BMP180_GetAltitude(int32_t pressure, float sea_level_pressure);
void BMP180_GetPressure_Blocking(BMP180_t *dev, uint8_t oss);
void BMP180_GetTemp_Blocking(BMP180_t *dev);

void BMP180_Get_Data_Asyc(BMP180_t *dev);


#endif //PROJECT_UART_BMP180_H
