//
// Created by rabilint on 04.04.26.
//

#ifndef PROJECT_UART_OLED_ANIMATIONS_H
#define PROJECT_UART_OLED_ANIMATIONS_H
#include "u8g2.h"
#include "bmp180.h"

void drawScaledHeart(u8g2_t *u8g2, int x, int y, int scale);
void drawTP(u8g2_t *u8g2, int x, int y, int x2, int y2, int x3, int y3, BMP180_t *dev);

#endif //PROJECT_UART_OLED_ANIMATIONS_H
