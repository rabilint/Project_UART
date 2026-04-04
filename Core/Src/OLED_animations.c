//
// Created by rabilint on 04.04.26.
//

//
// Created by rabilint on 04.04.26.
//

#include "u8g2.h"
#include "OLED_animations.h"

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