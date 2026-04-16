//
// Created by rabilint on 04.04.26.
//

#include "u8g2.h"
#include "OLED_animations.h"
#include <stdio.h>

static const char heart_source[11][14] ={
    "  111   111  ",
    " 11111 11111 ",
    "1111111111111",
    "1111111111111",
    "1111111111111",
    " 11111111111 ",
    "  111111111  ",
    "   1111111   ",
    "    11111    ",
    "     111     ",
    "      1      "
  };


void drawScaledHeart(u8g2_t *u8g2, int x, int y, int scale) {
    if (scale <= 0) return;

    for (int row = 0; row < 11; row++) {
        for (int col = 0; col < 13; col++) {
            if (heart_source[row][col] == '1') {
                u8g2_DrawBox(u8g2, x + col * scale, y + row * scale, scale, scale);
            }
        }
    }
}

void drawTP(u8g2_t *u8g2, int x, int y, int x2, int y2, int x3, int y3, BMP180_t *dev)
{
    if (dev == NULL) return;

    char buffer[32];
    int temp_int = (int)dev->temperature;
    int temp_frac = (int)((dev->temperature - temp_int) * 10);

    int ret = snprintf(buffer, sizeof buffer, "Tmp: %d.%d °C", temp_int, temp_frac);
    if (ret > 0 && ret < sizeof buffer)
    {
        u8g2_DrawStr(u8g2, x, y, buffer);
    }

    uint32_t mm_Hg = (uint32_t)(dev->pressure * 0.00750062f);
    ret = snprintf(buffer, sizeof buffer, "Press: %lu mmHg", mm_Hg);
    if (ret > 0 && ret < sizeof buffer)
    {
        u8g2_DrawStr(u8g2, x2, y2, buffer);
    }

    char alt_buf[20];
    int alt_int = (int)dev->Alt;
    int alt_frac = (int)((dev->Alt - alt_int) * 10);

    if (alt_frac < 0) alt_frac = -alt_frac;

    snprintf(alt_buf, sizeof alt_buf, "Alt: %d.%d m", alt_int, alt_frac);
    u8g2_DrawStr(u8g2, x3, y3, alt_buf);
}
