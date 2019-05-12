#ifndef AppI2C_h
#define AppI2C_h

#include <Arduino.h>

class AppI2C {
public:
    AppI2C();

    ~AppI2C();

    static void initiate();
    static void scan();
    static void select(uint8_t id);
};

#endif /* AppI2C_h */
