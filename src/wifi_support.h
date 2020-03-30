//
// Created by development on 23.02.20.
//

#ifndef TOUCHSWITCH_WIFI_SUPPORT_H
#define TOUCHSWITCH_WIFI_SUPPORT_H

#include <ESP8266mDNS.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "tft_helper.h"
#include "main.h"
#include "support.h"

struct  {
    char server[10];
    int port;
    char ip[14];
} docbox_server;

struct {
    uint32_t crc32;   // 4 bytes
    uint8_t channel;  // 1 byte,   5 in total
    uint8_t bssid[6]; // 6 bytes, 11 in total
    uint8_t padding;  // 1 byte,  12 in total
} rtcData;

uint8 PostMessage(String Action, const String postData, String *Response);
uint32_t calculateCRC32( const uint8_t *data, size_t length );
void MDNSServiceQueryCallback(MDNSResponder::MDNSServiceInfo serviceInfo, MDNSResponder::AnswerType answerType, bool p_bSetContent);

bool  InitWifi();
#endif //TOUCHSWITCH_WIFI_SUPPORT_H
