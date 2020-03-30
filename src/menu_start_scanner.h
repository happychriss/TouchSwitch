//
// Created by development on 25.02.20.
//

#ifndef TOUCHSWITCH_MENU_START_SCANNER_H
#define TOUCHSWITCH_MENU_START_SCANNER_H
#include <Arduino.h>
#include <pins_arduino.h>
#include <XPT2046_Touchscreen.h> // Touchscreen
#include <Adafruit_ST7735.h>   // Adafruit Display ST7735 TFT library
#include <Adafruit_GFX.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClientSecure.h>

#include "support.h"
#include <tft_helper.h>
#include "wifi_support.h"

uint menu_start_scanner();

void update_status(Adafruit_ST7735 tft, char* text);
#endif //TOUCHSWITCH_MENU_START_SCANNER_H
