//
// Created by development on 25.02.20.
//

#include "menu_start_scanner.h"
#include "main.h"


// On the Main-Menu the Scanner Button is pressed,  show the scann button and check for scanner


uint menu_cancel_scan() {
    DPL("Button: Cancel Scan");
    return GO_HOME;
}

uint execute_scan() {
    update_status("Scanning....",0);
    String res=start_scanner("start_scanner_from_hardware");

    if (res.indexOf("FATAL")>0) {
        return GO_ERROR;
    }

    update_status(res,0);
    return GO_OK;

}

uint menu_start_scanner() {

    // Scan and Done Buttons
    Adafruit_GFX_Button buttons[2];
    enum M1 {
        B_SCAN = 0, B_CANCEL = 1
    };

    buttons[B_SCAN].initButtonUL(&tft, B_X_SPACE, B_Y_SPACE, B_X_WIDTH, B_Y_HEIGHT, C_BUTTON, C_BACK, C_TEXT, "Scan", 1);
    buttons[B_CANCEL].initButtonUL(&tft, B_X_SPACE + (X_MAX / 2), B_Y_SPACE, B_X_WIDTH, B_Y_HEIGHT, C_BUTTON, C_BACK, C_TEXT, "Done", 1);
    struct _button_handler buttonHandler[2];


    // Begin Loop ************************************************ WE keep the initial screen as is (with black button)

    String result = "";
    result=start_scanner("start_scanner_from_hardware");

    tft.fillScreen(ST7735_WHITE);
    for (int i = B_SCAN; i <= B_CANCEL; i++) {
        buttons[i].drawButton();
        buttons[i].press(false);
    }

    update_status(result, 0);result=""; //result from initial call, only used once

    while (true) {

        String PostData = "";
        String status_response = "";
        if (PostMessage("get_docbox_status", PostData, &status_response)) {
            raise_error("Could not start -get docbox status-");
            return GO_ERROR;
        }

        update_status(status_response, 2); // Status

        buttonHandler[B_CANCEL].button_action = menu_cancel_scan;
        buttonHandler[B_SCAN].button_action = execute_scan;
        uint res = HandleButtons(buttons, 2, buttonHandler, TOUCH_SECONDS_WAIT * 4);
        for (int i = B_SCAN; i <= B_CANCEL; i++) {
            buttons[i].drawButton();
            buttons[i].press(false);
        }

        if (res != GO_OK) {
            DP("**** LEAVING: Start Scanner Menu with");
            DPL(res);
            return res;
        }


    }

}

