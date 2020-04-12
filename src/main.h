

#ifndef TOUCHSWITCH_MAIN_H
#define TOUCHSWITCH_MAIN_H

#include <c_types.h>
#include <WiFiClientSecure.h>
#include <Adafruit_ST7735.h>
#include <XPT2046_Touchscreen.h>
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
#include "menu_start_scanner.h"
#include "tft_helper.h"
#include "menu_main.h"

#define NO_ERROR false
#define ERROR true
// ******************  Status Handling ****************************************
#define GLOBAL_ACTION_INIT 1
#define GLOBAL_ACTION_MAIN_MENU 2
#define GLOBAL_ACTION_SLEEP 98
#define GLOBAL_ERROR 99

#define DOCBOX_PORT_SCAN_UPDATE 9000

extern uint8 global_status;


extern String host ;
extern  int port;


// initialize ST7735 TFT library with hardware SPI module
// MOSI=11, MISO=12, SCK=13 are default
extern Adafruit_ST7735 tft ;

extern XPT2046_Touchscreen ts;

struct _button_handler {
    uint (*button_action)() ;
//    uint button_id;
};
#define GO_HOME 1
#define GO_BACK 2
#define GO_REMAIN 3
#define GO_SLEEP 4

#define GO_ERROR 5
#define GO_WARNING 6
#define GO_OK 7


// ******************  General  ****************************************
// change to production, so we connect to production server!!!!!!!!!!!
#define AVAHI_SERVICE "_development_docbox"

// ******************  Pin Definitions ****************************************

// TO: Touch Panel
#define CS_PIN_Touch  15   //Chip Select of Touch Function

// TO: Display - ST7735 TFT module connections
#define TFT_RST   2
#define TFT_CS    0
#define TFT_DC    4

// TO: PIC
#define PIC16_NO_SLEEP 5

// ******************  User Interface  ****************************************
#define TOUCH_SECONDS_WAIT 5


#endif //TOUCHSWITCH_MAIN_H
