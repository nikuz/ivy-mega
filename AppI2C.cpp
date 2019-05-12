#include <Arduino.h>
#include "AppI2C.h"
#include "Wire.h"
extern "C" {
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}

#define TCAADDR 0x70

AppI2C::AppI2C() {}

AppI2C::~AppI2C() {}

void AppI2C::initiate() {
    Wire.begin();
}

void AppI2C::scan() {
    Serial.println("Start TCA devices scanning");
    for (uint8_t i = 0; i < 8; i++) {
        select(i);
        Serial.print("TCA Port #");
        Serial.println(i);

        for (uint8_t addr = 0; addr <= 127; addr++) {
            if (addr == TCAADDR) continue;

            uint8_t data;
            if (!twi_writeTo(addr, &data, 0, 1, 1)) {
                Serial.print("Found I2C 0x");
                Serial.println(addr, HEX);
            }
        }
    }
    Serial.println("Done!");
}

void AppI2C::select(uint8_t id) {
    if (id > 7) return;

    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << id);
    Wire.endTransmission();
}