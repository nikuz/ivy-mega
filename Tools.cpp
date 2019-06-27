#include <Arduino.h>

#include "Tools.h"

Tools::Tools() {}

Tools::~Tools() {}

bool Tools::timerCheck(int interval, unsigned long lastInitiate) {
    unsigned long now = millis();
    return (interval * 1000L) < now && now - (interval * 1000L) > lastInitiate;
}

char *Tools::intToChar(int value) {
    static char result[10];
    int strLength = 1;
    if (value > 9999) {
        strLength = 5;
    } else if (value > 999) {
        strLength = 4;
    } else if (value > 99) {
        strLength = 3;
    } else if (value > 9) {
        strLength = 2;
    }
    dtostrf(value, strLength, 0, result);

    return result;
}

char *Tools::getUptime() {
    static char uptimeString[10];
    const float uptime = millis() / 1000.0L / 60.0L;
    dtostrf(uptime > 60 ? uptime / 60 : uptime, 3, 1, uptimeString);
    if (uptime > 60) {
        strcat(uptimeString, "h");
    } else {
        strcat(uptimeString, "m");
    }
    return uptimeString;
}
