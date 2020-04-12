#include "main.h"
#include "config.h"





//**** Global Variables **************************************+

uint8 global_status = GLOBAL_ACTION_INIT;





// initialize ST7735 TFT library with hardware SPI module
// MOSI=11, MISO=12, SCK=13 are default
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

XPT2046_Touchscreen ts(CS_PIN_Touch);  // Param 2 - NULL - No interrupts

char tft_text[30] = {0};
char web_server_uri[100] = {0};
char post_parameter[100] = {0};


String host ;
int port ;


//************************* Setup ************************************************************************

void setup() {
    pinMode(PIC16_NO_SLEEP, OUTPUT);
    digitalWrite(PIC16_NO_SLEEP, HIGH);

#ifdef MYDEBUG
    Serial.begin(115200);
#endif

#ifdef MYDEBUG_SERIAL
    SerialKeyWait();
#endif
}



//*************************Loop ************************************************************************

void loop() {


    while (global_status != GLOBAL_ERROR) {

        DPL("*** Init Display and Touch");
        ts.begin();
        ts.setRotation(0);

        tft.initR(INITR_GREENTAB);   // initialize a ST7735S chip, CN: My-chip has a "green flag at the protection foil"
        tft.setRotation(1);

        tft.fillScreen(ST7735_WHITE);
        tft.setCursor(B_X_SPACE, B_Y_SPACE);
        tft.setTextColor(C_TEXT);
        tft.print("One Moment!");

        DPL("*** Init Wifi");
        // Init Wifi and find Docbox Server (Avahi)
        if (!InitWifi()) {
            raise_error("WIFI: Connection Error");
            break;
        }

        DP("*** Search Server:");
        DPL(AVAHI_SERVICE);
        if (!MDNS.begin("ESP")) {
            raise_error("WIFI: Avahi-Server not started");
            break;
        }
        int n = MDNS.queryService(AVAHI_SERVICE, "tcp");
        if (n != 1) {
            raise_error("WIFI: Avahi-Server not found");
            break;
        }

        host=MDNS.IP(0).toString();
        port=MDNS.port(0);

        DP("Server found:");DP(host);DP(":");DPL(port);

        String response;
        DPL("**** Register TouchSwitch ");
        String PostData = "{\"connector\":{\"service\":\"TouchSwitch\",\"uri\":\"" + WiFi.localIP().toString() + ":8081\",\"uid\":\"5\",\"prio\":\"1\"}}";

        if (PostMessage("connectors", PostData, &response)) {
            raise_error("Could not connect to DocBox Server");
            break;
        }

        //******************** Main Menu Loop until GO to Sleep **********************************

        while(main_menu()!=GO_SLEEP){};

        break;

    }

    // Work Done, go back to sleep (normal operation or error)
    tft.fillScreen(ST7735_WHITE);
    digitalWrite(PIC16_NO_SLEEP, LOW);
    delay(400);
    ESP.deepSleep(0, WAKE_RF_DISABLED);
    delay(100);


}

/*
    while(true) {
        if (ts.touched()) {
            TS_Point p = ts.getPoint();
            //      tft.fillScreen(ST7735_BLACK);
            sprintf(tft_text, "x:%d y:%d", p.x, p.y);
            //      testdrawtext(tft_text,ST7735_WHITE);
            tft.drawPixel(p.y * x_tft_scale, p.x * y_tft_scale, ST7735_GREEN);
        }
        delay(5);
    }*/
