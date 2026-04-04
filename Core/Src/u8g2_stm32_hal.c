//
// Created by rabilint on 04.04.26.
//
#include "u8g2.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;

uint8_t u8x8_gpio_and_delay_stm32(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    switch (msg) {
    case U8X8_MSG_DELAY_MILLI:
        HAL_Delay(arg_int);
        break;
    case U8X8_MSG_GPIO_DC:
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, arg_int);
        break;
    case U8X8_MSG_GPIO_RESET:
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, arg_int);
        break;
    case U8X8_MSG_GPIO_CS:
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, arg_int);
        break;
    }
    return 1;
}

uint8_t u8x8_byte_stm32_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    switch (msg) {
    case U8X8_MSG_BYTE_SEND:
        HAL_SPI_Transmit(&hspi1, (uint8_t *)arg_ptr, arg_int, 100);
        break;
    case U8X8_MSG_BYTE_INIT:
        break;
    case U8X8_MSG_BYTE_SET_DC:
        // КЕРУЄМО ПІНОМ DC ЧЕРЕЗ GPIO КОЛБЕК
        u8x8_gpio_and_delay_stm32(u8x8, U8X8_MSG_GPIO_DC, arg_int, NULL);
        break;
    case U8X8_MSG_BYTE_START_TRANSFER:
        // АКТИВУЄМО CS (CHIP SELECT)
        u8x8_gpio_and_delay_stm32(u8x8, U8X8_MSG_GPIO_CS, u8x8->display_info->chip_enable_level, NULL);
        break;
    case U8X8_MSG_BYTE_END_TRANSFER:
        // ДЕАКТИВУЄМО CS
        u8x8_gpio_and_delay_stm32(u8x8, U8X8_MSG_GPIO_CS, u8x8->display_info->chip_disable_level, NULL);
        break;
    default:
        return 0;
    }
    return 1;
}
