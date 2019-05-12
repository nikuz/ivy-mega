#ifndef AppTime_h
#define AppTime_h

#include <Arduino.h>

class AppTime {
public:
    AppTime();

    ~AppTime();

    static void RTCBegin();

    static char *RTCGetTemperature();

    static char *RTCBattery();

    static bool RTCIsDateTimeValid();

    static char *RTCGetCurrentTime();

    static void RTCDateTimeUpdate(const char *command, const char *param);
};

#endif /* AppTime_h */
