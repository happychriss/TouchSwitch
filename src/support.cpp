//
// Created by development on 15.03.20.
//
#include "main.h"
#include "menu_start_scanner.h"
#include "support.h"
#include <Arduino.h>
#include <support.h>


int SerialKeyWait() {// Wait for Key
//    Serial.setDebugOutput(true);

    Serial.println("Hit a key to start...");
    Serial.flush();

    while (true) {
        int inbyte = Serial.available();
        delay(500);
        if (inbyte > 0) break;
    }

    return Serial.read();


}

void update_status(String msg_text, uint line) {
    tft.setTextColor(C_TEXT, C_BACK);
    tft.fillRect(0, (Y_MAX / 2) + B_Y_SPACE*(line+1),X_MAX,7,ST7735_WHITE);
    tft.setCursor(B_X_SPACE, (Y_MAX / 2) + B_Y_SPACE*(line+1));
    String my_msg_text = msg_text.substring(0, 22);
    uint padlen = (22 - my_msg_text.length()) / 2;
    char tmptxt[22];
    char prttxt[50];
    my_msg_text.toCharArray(tmptxt, 22);
    sprintf(prttxt, "%*s%s%*s\n", padlen, "", tmptxt, padlen, "");
    DP("Message:");
    DP(prttxt);
    DPL("<-");
//    tft.print("*                    *");
    tft.print(prttxt);
}

String start_scanner(String command) {
    String msg_result;

    DPL("******************* Execute Scan");

    String PostData = "";
    String response;
    if (PostMessage(command, PostData, &response)) {
        raise_error("Could not start -execute scan-");
        return "FATAL";
    }
    // we found a scanner, wait for an answer
    if (response.length() > 3) {
        msg_result= read_server_answer();
    } else {
        msg_result="Scanner not found";
    }
    DP("**RESULT: ");DPL(msg_result);
    return msg_result;

}
