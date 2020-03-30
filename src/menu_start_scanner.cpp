//
// Created by development on 25.02.20.
//

#include "menu_start_scanner.h"
#include "main.h"

void update_status(String msg_text) {
    tft.setTextColor(C_TEXT, C_BACK);
    tft.setCursor(B_X_SPACE, (Y_MAX / 2) + B_Y_SPACE);
    String my_msg_text=msg_text.substring(0,20);
    uint padlen = (20 - my_msg_text.length()) / 2;
    char tmptxt[20];
    char prttxt[50];
    my_msg_text.toCharArray(tmptxt, 20);
    sprintf(prttxt, "%*s%s%*s\n", padlen, "", tmptxt, padlen, "");
    DP("Message:");DP(prttxt);DPL("<-");
    tft.print(prttxt);
}

// On the Main-Menu the Scanner Button is pressed,  show the scann button and check for scanner

uint menu_retry_scan() {
    DPL("Button: Retry Scan");
    return GO_BACK;
}

uint menu_cancel_scan() {
    DPL("Button: Cancel Scan");
    return GO_HOME;
}

uint execute_scan () {
DPL("Execute Scan");
    while(true) {
yield();
}
}

uint menu_start_scanner() {

    tft.fillScreen(ST7735_WHITE);
    tft.setCursor(B_X_SPACE, Y_MAX - B_Y_SPACE);
    tft.setTextColor(C_TEXT);
    tft.print("Checking for Scanner...");

    String response;

    // ***************** Search for Scanner  ***************
    DPL("************ Search Scanner  **************");
    while (true) {

        if (PostMessage("scan_info", "", &response)) {
            raise_error("Could not start scann-service to DocBox Server");
        }

        DP("Scan-Info Response:"); DPL(response);

        if (response.length() > 3) {
            break; // we found the scanner
        }

        DP("No scanner found!");

        enum M1 {
            B_CANCEL = 0, B_RETRY = 1
        };

        Adafruit_GFX_Button buttons[2];
        buttons[B_CANCEL].initButtonUL(&tft, B_X_SPACE, B_Y_SPACE, B_X_WIDTH, B_Y_HEIGHT, C_BUTTON, C_BACK, C_TEXT, "CANCEL", 1);
        buttons[B_RETRY].initButtonUL(&tft, B_X_SPACE + (X_MAX / 2), B_Y_SPACE, B_X_WIDTH, B_Y_HEIGHT, C_BUTTON, C_BACK, C_TEXT, "RETRY", 1);
        tft.fillScreen(ST7735_WHITE);
        for (int i = B_CANCEL; i <= B_RETRY; i++) {
            buttons[i].drawButton();
            buttons[i].press(false);
        }
        update_status("No scanner found, try again");

        struct _button_handler buttonHandler[2];
        buttonHandler[B_CANCEL].button_action = menu_cancel_scan;
        buttonHandler[B_RETRY].button_action = menu_retry_scan;
        uint res = HandleButtons(buttons, 2, buttonHandler);
        if (res != GO_REMAIN) {
            DP("HandleButtonResult:");DPL(res);
            return res;
        }

    }

    // We found the scanner, now we can really scan

//    tft.setCursor(B_X_SPACE, Y_MAX - B_Y_SPACE);

    // response = response.substring(3, response.indexOf(":") - 2) + ":Ready";

    DPL("************ Start Scanning **************");

    while(true) {

        Adafruit_GFX_Button buttons[2];
        enum M1 {
            B_SCAN = 0, B_CANCEL = 1
        };

        buttons[B_SCAN].initButtonUL(&tft, B_X_SPACE, B_Y_SPACE, B_X_WIDTH, B_Y_HEIGHT, C_BUTTON, C_BACK, C_TEXT, "Scan", 1);
        buttons[B_CANCEL].initButtonUL(&tft, B_X_SPACE + (X_MAX / 2), B_Y_SPACE, B_X_WIDTH, B_Y_HEIGHT, C_BUTTON, C_BACK, C_TEXT, "Done", 1);
        tft.fillScreen(ST7735_WHITE);

        for (int i = B_SCAN; i <= B_CANCEL; i++) {
            buttons[i].drawButton();
            buttons[i].press(false);
        }

        String scanner=response.substring(2,response.indexOf(":")-2);
        DP("Scanner:");DPL(scanner);

        update_status(scanner);
        DPL(7);
        struct _button_handler buttonHandler[2];
        buttonHandler[B_CANCEL].button_action = menu_cancel_scan;
        buttonHandler[B_SCAN].button_action = execute_scan;
        DPL(8);
        uint res = HandleButtons(buttons, 2, buttonHandler);
        if (res != GO_REMAIN) {
        DP("**** LEAVING: Start Scanner Menu with");DPL(res);
            return res;
        }
    }

}

