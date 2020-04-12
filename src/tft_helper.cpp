//
// Created by development on 02.02.20.
//

#include <WiFiClient.h>
#include "tft_helper.h"
#include "main.h"
#include "support.h"

void drawtext(char *text, uint16_t color) {
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.setTextWrap(true);
    tft.print(text);
}

void raise_error(const String str_text) {
    char text[100];
    str_text.toCharArray(text,100);
    tft.fillScreen(ST7735_RED);
    drawtext(text, ST7735_YELLOW);
    delay(6000);
    global_status=GLOBAL_ERROR;
    DP("ERROR:");DPL(str_text);
}

uint HandleButtons(Adafruit_GFX_Button *buttons, uint button_count, struct _button_handler *ButtonHandler, uint touch_seconds_wait= TOUCH_SECONDS_WAIT) {

    int wait_for_touch_cnt = 0;


    while (true) {

        wait_for_touch_cnt++;

        if (ts.touched()) {
            TS_Point p = ts.getPoint();

            for (int i = 0; i<button_count; i++) {
                // go thru all the buttons, checking if they were pressed
                if (buttons[i].contains(p.y * TFT_X_SCALE, p.x * TFT_Y_SCALE)) {
                    buttons[i].press(true);  // tell the button it is pressed
                } else {
                    buttons[i].press(false);  // tell the button it is NOT pressed
                }

                // now we can ask the buttons if their state has changed
                if (buttons[i].justReleased()) //
                    DP("Button Released: ");DPL(i);
                {
                    buttons[i].drawButton(false);  // draw normal

                }

                // **** Button was pressed, call action ********************
                if (buttons[i].justPressed())
                {
                    DP("Button Pressed: ");DPL(i);
                    buttons[i].drawButton(true);  // draw invert
                    return (*ButtonHandler[i].button_action)();

                }
            }
        }

//*** Go Back to sleep if no button is pressed for some-time ***********************

        if (wait_for_touch_cnt > touch_seconds_wait * 10) {
            return GO_SLEEP;
        }

        delay(200);
        yield();
    } //end of main while loop, end up below in case of sleep or error
}