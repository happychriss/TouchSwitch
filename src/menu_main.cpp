//
// Created by development on 29.03.20.
//
#include "main.h"
#include "menu_main.h"
#include "menu_copy.h"

uint main_menu() {

    //****** Paint the Display ************************************************

    DPL("**** Paint Main-Menu Display");

    tft.fillScreen(ST7735_WHITE);

    enum M1 {
        B_SCAN = 0, B_COPY = 1, B_HOME = 2, B_STATUS = 3
    };

    Adafruit_GFX_Button buttons[4];
    buttons[B_SCAN].initButtonUL(&tft, B_X_SPACE, B_Y_SPACE, B_X_WIDTH, B_Y_HEIGHT, C_BUTTON, C_BACK, C_TEXT, "SCAN", 1);
    buttons[B_COPY].initButtonUL(&tft, B_X_SPACE + (X_MAX / 2), B_Y_SPACE, B_X_WIDTH, B_Y_HEIGHT, C_BUTTON, C_BACK, C_TEXT, "COPY", 1);
    buttons[B_HOME].initButtonUL(&tft, B_X_SPACE, (Y_MAX / 2) + B_Y_SPACE, B_X_WIDTH, B_Y_HEIGHT, C_BUTTON, C_BACK, C_TEXT, "HOME", 1);
    buttons[B_STATUS].initButtonUL(&tft, (X_MAX / 2) + B_X_SPACE, (Y_MAX / 2) + B_Y_SPACE, B_X_WIDTH, B_Y_HEIGHT, C_BUTTON, C_BACK, C_TEXT, "STATUS", 1);

    for (int i = B_SCAN; i <= B_STATUS; i++) {
        buttons[i].drawButton();
        buttons[i].press(false);
    }

    global_status = GLOBAL_ACTION_MAIN_MENU;
    struct _button_handler buttonHandler[4];
    buttonHandler[0].button_action = menu_start_scanner;
    buttonHandler[1].button_action = menu_start_copy;

    return HandleButtons(buttons, 2, buttonHandler, TOUCH_SECONDS_WAIT);
}

