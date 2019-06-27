#ifndef Tools_h
#define Tools_h

#include <Arduino.h>

class Tools {
public:
    Tools();

    ~Tools();

    static bool timerCheck(int interval, unsigned long lastInitiate);

    static char *intToChar(int value);

    static char *getUptime();
};

#endif /* Tools_h */
