//
// Created by development on 10.04.20.
//

#ifndef TOUCHSWITCH_SUPPORT_H
#define TOUCHSWITCH_SUPPORT_H

void update_status(String msg_text, uint line);
String start_scanner(String command);

#define MYDEBUG
#ifdef MYDEBUG
#define MYDEBUG_CORE
#define DP(x)     Serial.print (x)
#define DPD(x)     Serial.print (x, DEC)
#define DPL(x)  Serial.println (x)
#else
#define DP(x)
#define DPD(x)
#define DPL(x)
#endif

#endif //TOUCHSWITCH_SUPPORT_H
