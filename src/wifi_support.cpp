//
// Created by development on 23.02.20.
//

#include "wifi_support.h"
#include <ESP8266HTTPClient.h>
#include "config.h"


uint32_t calculateCRC32( const uint8_t *data, size_t length ) {
    uint32_t crc = 0xffffffff;
    while( length-- ) {
        uint8_t c = *data++;
        for( uint32_t i = 0x80; i > 0; i >>= 1 ) {
            bool bit = crc & 0x80000000;
            if( c & i ) {
                bit = !bit;
            }

            crc <<= 1;
            if( bit ) {
                crc ^= 0x04c11db7;
            }
        }
    }

    return crc;
}

bool  InitWifi() {
    const char WiFiSSID[] = WIFI_SSID;
    const char WiFiPSK[] =  WIFI_PWD;

    IPAddress server(192, 168, 1, 100);

    DPL("Start");
    delay(100);
    WiFi.disconnect();
    delay(30);
    WiFi.forceSleepBegin(); //this sequence is magically needed
    delay(50);
    WiFi.forceSleepWake();
    delay(50); //this value must be long enough, otherwise it will not work

// Try to read WiFi settings from RTC memory

    bool rtcValid = false;

    if (ESP.rtcUserMemoryRead(0, (uint32_t *) &rtcData, sizeof(rtcData))) {
// Calculate the CRC of what we just read from RTC memory, but skip the first 4 bytes as that's the checksum itself.
        uint32_t crc = calculateCRC32(((uint8_t *) &rtcData) + 4, sizeof(rtcData) - 4);
        if (crc == rtcData.crc32) {
            rtcValid = true;
            DPL("Valid RTC");
            DPL(crc);

        } else {
            DPL("InValid RTC");
        }
    }


//    WiFi.persistent(false); // Bugfix connectivity to RP
//    WiFi.mode(WIFI_OFF);   // this is a temporary line, to be removed after SDK update to 1.5.4
    WiFi.mode(WIFI_STA);


    if (rtcValid) {
        DPL("Quick connection");
// The RTC data was good, make a quick connection
        WiFi.begin(WiFiSSID, WiFiPSK, rtcData.channel, rtcData.bssid, true);
        DPL("Channel");
        DPL(rtcData.channel);

    } else {
        DPL("Regular connection");
// The RTC data was not valid, so make a regular connection
        WiFi.begin(WiFiSSID, WiFiPSK);
    }

    WiFi.config(
            IPAddress(192, 168, 1, 107),
            IPAddress(192, 168, 1, 1),
            IPAddress(255, 255, 255, 0),
            IPAddress(192, 168, 1, 1));


    int retries = 0;
    int wifiStatus = WiFi.status();
    while (wifiStatus != WL_CONNECTED) {
        retries++;
        DP(".");
        if (retries == 100) {
            DPL("Not working, try regular");
// Quick connect is not working, reset WiFi and try regular connection
            WiFi.disconnect();
            delay(10);
            WiFi.forceSleepBegin();
            delay(10);
            WiFi.forceSleepWake();
            delay(10);
            WiFi.begin(WiFiSSID, WiFiPSK);
        }
        if (retries == 600) {
            DPL("Not working long time, deep sleep");
// Giving up after 30 seconds and going back to sleep
            WiFi.disconnect(true);
            delay(1);
            WiFi.mode(WIFI_OFF);
            return false; // Not expecting this to be called, the previous call will never return.
        }
        delay(50);
        wifiStatus = WiFi.status();
    }

// Write current connection info back to RTC

    rtcData.channel = WiFi.channel();
    memcpy(rtcData.bssid, WiFi.BSSID(), 6); // Copy 6 bytes of BSSID (AP's MAC address)
    rtcData.crc32 = calculateCRC32(((uint8_t *) &rtcData) + 4, sizeof(rtcData) - 4);
    ESP.rtcUserMemoryWrite(0, (uint32_t *) &rtcData, sizeof(rtcData));

    DPL("DONE IP Address");
    DPL(WiFi.localIP());
    return true;
}


uint8 PostMessage(String Action, const String postData, String *Response) {

    String postHeader = "";
    postHeader += ("POST /" + Action + " HTTP/1.1\r\n");
    postHeader += ("Host: " + host +":" + String(port) + "\r\n");
    postHeader += ("Accept: application/json\r\n");
    postHeader += ("Content-Type: application/json\r\n");
    postHeader += ("Connection: close\r\n");
    postHeader += ("Content-Length: ");
    postHeader += (postData.length());
    postHeader += ("\r\n\r\n");

    WiFiClient client;

    if (!client.connect(host, port)) {
        raise_error("PostMsg("+Action+")failed!");
        return ERROR;
    }

    //******* Send Request *****************************************


    String request=postHeader+postData;
    DPL("sendHttpRequest");

    size_t res=client.print(request);
    DP("Request->");DP(request);DP("<- Size:");DPL(res);
    DP("Request sent, checking response with heap:");DPL(ESP.getFreeHeap());

    //******* Read Answer  *****************************************
    DPL("readHttpRequest");
    // Reading Headers ****************************
    String str_header;
    uint32 length = 0;
    bool b_chunked = false;
    long http_code = 0;
    String result="";

    while (true) {
        str_header = client.readStringUntil('\n');
        DP("->");DP(str_header);DPL("<-");

        if (str_header.indexOf("Transfer-Encoding: chunked") != -1) {
            b_chunked = true;
            DPL("Header: Found Chunked");
        }

        if (str_header.indexOf("Content-Length:") != -1) {
            length = str_header.substring(str_header.indexOf(":") + 1).toDouble();
            DP("Header: Found Length:");
            DPL(length);
        }

        if (str_header.indexOf("HTTP/1") != -1) {
            http_code = str_header.substring(str_header.indexOf(" ") + 1, str_header.indexOf(" ") + 4).toInt();
            DP("Header: HTTP-Status:");
            DPL(http_code);
        }

        if (str_header == "\r") {
            Serial.println("DONE: headers received");
            break;
        }
    }

    if (http_code == 0) {
        raise_error("PostMsg: No HTTP/1 Found in Header!");
        return ERROR;

    }

    if ((http_code != 200) && (http_code != 428)) {
        raise_error("PostMsg: HTTP-Error: "+String(http_code));
        return ERROR;
    }


    DPL("******** Receiving Data :");

    if (b_chunked) {
        DPL("*** Chunked Reading ***");
        while (true) {
            String line = client.readStringUntil('\r\n');
            DP("Chunk-Len:");
            DP(line);
            DPL("<-");

            char chr_line[20];
            line.toCharArray(chr_line, 20);
            long len = strtol(chr_line, NULL, 16);
            DP("Chunk-Len LongInt:");
            DPL(len);
            DP("BUFFER->");
            if (len != 0) {
                char *my_buffer = (char *) malloc(len + 1);
                memset(my_buffer, 0, len + 1);
                client.readBytes(my_buffer, len);
                DP(my_buffer);
                result = result + String(my_buffer);
                free(my_buffer);
                client.readStringUntil('\r\n');
            } else {
                break;
            }
            DPL("<-BUFFER");

        }
    } else if (length != 0) {
        DPL("*** Content-Length Reading ***");
        DP("BUFFER->");

        char *my_buffer = (char *) malloc(length + 1);
        memset(my_buffer, 0, length + 1);
        client.readBytes(my_buffer, length);
        result = String(my_buffer);
        DP(result);
        DPL("<-BUFFER");


    } else {
        DPL("*** Connected Reading (most slow) ***");
        DP("BUFFER->");

        while (client.connected()) {
            String line = client.readStringUntil('\r');
            DP(line);
            result += line;
        }
        DPL("<-BUFFER");
    }

    DPL("closing connection");
    client.stop();

    *Response=result;

    return NO_ERROR;

}

String readHttpRequest(WiFiClient client) {

    DPL("readHttpRequest");
    // Reading Headers ****************************
    String str_header;
    uint32 length = 0;
    bool b_chunked = false;
    long http_code = 0;
    String result="";

    while (true) {
        str_header = client.readStringUntil('\n');
        DP("->");DP(str_header);DPL("<-");

        if (str_header.indexOf("Transfer-Encoding: chunked") != -1) {
            b_chunked = true;
            DPL("Header: Found Chunked");
        }

        if (str_header.indexOf("Content-Length:") != -1) {
            length = str_header.substring(str_header.indexOf(":") + 1).toDouble();
            DP("Header: Found Length:");
            DPL(length);
        }

        if (str_header.indexOf("HTTP/1") != -1) {
            http_code = str_header.substring(str_header.indexOf(" ") + 1, str_header.indexOf(" ") + 4).toInt();
            DP("Header: HTTP-Status:");
            DPL(http_code);
        }

        if (str_header == "\r") {
            Serial.println("DONE: headers received");
            break;
        }
    }

    if (http_code == 0) {
        result = "CN_ERROR: No HTTP/1 Found in Header!";
        DPL(result);
        return result;

    }

    if ((http_code != 200) && (http_code != 428)) {
        result = "CN_ERROR -  HTTP:" + String(http_code);
        DPL(result);
        return result;
    }


    DPL("******** Receiving Data :");

    if (b_chunked) {
        DPL("*** Chunked Reading ***");
        while (true) {
            String line = client.readStringUntil('\r\n');
            DP("Chunk-Len:");
            DP(line);
            DPL("<-");

            char chr_line[20];
            line.toCharArray(chr_line, 20);
            long len = strtol(chr_line, NULL, 16);
            DP("Chunk-Len LongInt:");
            DPL(len);
            DP("BUFFER->");
            if (len != 0) {
                    char *my_buffer = (char *) malloc(len + 1);
                    memset(my_buffer, 0, len + 1);
                    client.readBytes(my_buffer, len);
                    DP(my_buffer);
                    result = result + String(my_buffer);
                    free(my_buffer);
                client.readStringUntil('\r\n');
            } else {
                break;
            }
            DPL("<-BUFFER");

        }
    } else if (length != 0) {
        DPL("*** Content-Length Reading ***");
        DP("BUFFER->");

            char *my_buffer = (char *) malloc(length + 1);
            memset(my_buffer, 0, length + 1);
            client.readBytes(my_buffer, length);
            result = String(my_buffer);
            DP(result);
        DPL("<-BUFFER");


    } else {
        DPL("*** Connected Reading (most slow) ***");
        DP("BUFFER->");

            while (client.connected()) {
                String line = client.readStringUntil('\r');
                DP(line);
                result += line;
            }
        DPL("<-BUFFER");
    }

    DPL("closing connection");
    client.stop();

    return result;

}
