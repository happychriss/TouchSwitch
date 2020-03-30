//
// Created by development on 12.02.20.
//

#ifndef TOUCHSWITCH_SUPPORT_H
#define TOUCHSWITCH_SUPPORT_H
#define MYDEBUG
#ifdef MYDEBUG
#define MYDEBUG_CORE
#define DP(x)     Serial.print (x)
#define DPD(x)     Serial.print (x, DEC)
#define DPL(x)  Serial.println (x)
#define DF Serial.flush()
#else
#define DP(x)
#define DPD(x)
#define DPL(x)
#endif

int SerialKeyWait();

#endif //TOUCHSWITCH_SUPPORT_H
