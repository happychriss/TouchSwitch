//
// Created by development on 02.02.20.
//



#ifndef TOUCHSWITCH_TFT_HELPER_H
#define TOUCHSWITCH_TFT_HELPER_H

#include <main.h>

//*****************  TFT Display values
#define X_MAX 160
#define Y_MAX 128

#define TFT_X_SCALE X_MAX/4096
#define TFT_Y_SCALE Y_MAX/4096

#define B_X_SPACE 10
#define B_Y_SPACE 10

#define B_X_WIDTH (( X_MAX / 2)-(2*B_X_SPACE))
#define B_Y_HEIGHT (( Y_MAX / 2)-(2*B_Y_SPACE))

#define C_BUTTON ST7735_BLACK
#define C_BACK ST7735_WHITE
#define C_TEXT ST7735_BLACK

void drawtext(char *text, uint16_t color);
void raise_error(const String str_text);
uint HandleButtons(Adafruit_GFX_Button *buttons, uint button_count, struct _button_handler * ButtonHandler);

#endif //TOUCHSWITCH_TFT_HELPER_H
