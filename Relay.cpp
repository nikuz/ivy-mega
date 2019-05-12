#include <Arduino.h>

#include "def.h"
#include "Relay.h"

// RelayItem({pin}, {name}, {highLevelTrigger}) - highLevelTrigger is true by default
static RelayItem relays[] = {
//        RelayItem(53, "light"), // 220V
//        RelayItem(52, "vent"), // 220V
//        RelayItem(51, "humidity"), // 220V
//        RelayItem(50, "water"), // 220V
//        RelayItem(49, "wmixing"), // 220V
//        RelayItem(48, "free220"), // 220V
//        RelayItem(25, "s1", false), // 12V
//        RelayItem(23, "s2", false), // 12V
//        RelayItem(32, "s3", false), // 12V
//        RelayItem(30, "s4", false), // 12V
//        RelayItem(28, "sHumidity", false), // 12V
//        RelayItem(26, "wind", false) // 12V
};

const char relayOnSerialCommand[] = "rOn";
const char relayOffSerialCommand[] = "rOf";

Relay::Relay() {}

Relay::~Relay() {}

// public

void Relay::initiate() {
    int relaysCounts = *(&relays + 1) - relays;
    for (int i = 0; i < relaysCounts; i++) {
        // turn off relays by default
        // not all relays is HIGH level triggered
        if (relays[i].highLevelTrigger) {
            Serial.print("HIGH level trigger: ");
            Serial.println(relays[i].name);
            digitalWrite(relays[i].pin, LOW);
        } else {
            Serial.print("LOW level trigger: ");
            Serial.println(relays[i].name);
            digitalWrite(relays[i].pin, HIGH);
        }
        pinMode(relays[i].pin, OUTPUT);
    }
}

bool Relay::parseSerialCommand(const char *command, const char *param) {
    if (strcmp(command, relayOnSerialCommand) == 0) return Relay::on(param);
    if (strcmp(command, relayOffSerialCommand) == 0) return Relay::off(param);

    return false;
}

// private

bool Relay::on(const char *name) {
    RelayItem relayItem = Relay::getRelayPin(name);
    if (relayItem.pin != -1) {
        DEBUG_PRINT("Relay ON: ");
        DEBUG_PRINTLN(relayItem.name);
        if (relayItem.highLevelTrigger) {
            digitalWrite(relayItem.pin, HIGH);
        } else {
            digitalWrite(relayItem.pin, LOW);
        }
        return true;
    }

    DEBUG_PRINT("Relay doesn't exist: ");
    DEBUG_PRINTLN(name);
    return false;
}

bool Relay::off(const char *name) {
    RelayItem relayItem = Relay::getRelayPin(name);
    if (relayItem.pin != -1) {
        DEBUG_PRINT("Relay OFF: ");
        DEBUG_PRINTLN(relayItem.name);
        if (relayItem.highLevelTrigger) {
            digitalWrite(relayItem.pin, LOW);
        } else {
            digitalWrite(relayItem.pin, HIGH);
        }

        return true;
    }

    DEBUG_PRINT("Relay doesn't exist: ");
    DEBUG_PRINTLN(name);
    return false;
}

RelayItem Relay::getRelayPin(const char *name) {
    int relaysCounts = *(&relays + 1) - relays;
    for (int i = 0; i < relaysCounts; i++) {
        if (strcmp(relays[i].name, name) == 0) {
            return relays[i];
        }
    }

    return RelayItem(-1, "");
}
