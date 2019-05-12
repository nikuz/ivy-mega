#include "Relay.h"
#include "AppSerial.h"
#include "AppI2C.h"
#include "AppTime.h"
#include "Sensor.h"
#include "Tools.h"
#include "def.h"

int TimeReadInterval = 1;  // read sensor once in two seconds
unsigned long TimeReadLastTime = millis();

int SensorsReadInterval = 2;  // read sensor once in two seconds
unsigned long SensorsReadLastTime = millis();

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ;
    }
    Serial.print("Version: ");
    Serial.print(VERSION);

    Serial1.begin(115200);
    while (!Serial1) {
        ;
    }

    AppI2C::initiate();
#ifdef DEBUG
    AppI2C::scan();
#endif
    Relay::initiate();
    Sensor::initiate();
    AppTime::RTCBegin();
}

void loop() {
    SerialFrame serialFrame = AppSerial::getFrame();
    if (strcmp(serialFrame.command, "") != 0) {
        // relay
        bool performRelayCommand = Relay::parseSerialCommand(serialFrame.command, serialFrame.param);
        if (performRelayCommand) {
            AppSerial::sendFrame(&serialFrame);
        }
        // sync RTC time by NTP
        AppTime::RTCDateTimeUpdate(serialFrame.command, serialFrame.param);
//        // serial speed test
//        if (strcmp(serialFrame.command, "test") == 0) {
//            unsigned long testStart = millis();
//            SerialFrame testFirstFrame = SerialFrame("test", "first");
//            AppSerial::sendFrame(&testFirstFrame);
//            while (millis() - testStart < 1000) {
//                unsigned long val = millis() - testStart;
//                char *testStr = Tools::intToChar(val);
//                SerialFrame testFrame = SerialFrame("test", testStr);
//                AppSerial::sendFrame(&testFrame);
//            }
//            SerialFrame testLastFrame = SerialFrame("test", "last");
//            AppSerial::sendFrame(&testLastFrame);
//        }
    }

    // RTC time and temperature
    if (Tools::timerCheck(TimeReadInterval, TimeReadLastTime)) {
        const char *timeParam = AppTime::RTCGetCurrentTime();
        SerialFrame timeFrame = SerialFrame("time", timeParam);
        AppSerial::sendFrame(&timeFrame);

        const char *timeTempParam = AppTime::RTCGetTemperature();
        SerialFrame timeTemperatureFrame = SerialFrame("ttemp", timeTempParam);
        AppSerial::sendFrame(&timeTemperatureFrame);

        const char *timeBatteryParam = AppTime::RTCBattery();
        SerialFrame timeBatteryFrame = SerialFrame("tbtr", timeBatteryParam);
        AppSerial::sendFrame(&timeBatteryFrame);

        TimeReadLastTime = millis();
    }
    // sensors
    if (Tools::timerCheck(SensorsReadInterval, SensorsReadLastTime)) {
        Sensor::read();

        const char *tempParam = Sensor::getTemperature();
        SerialFrame temperatureFrame = SerialFrame("temp", tempParam);
        AppSerial::sendFrame(&temperatureFrame);

        const char *humParam = Sensor::getHumidity();
        SerialFrame humidityFrame = SerialFrame("hum", humParam);
        AppSerial::sendFrame(&humidityFrame);

        int soilSensors[3] = {1, 2, 3};
        int soilSensorsCounts = *(&soilSensors + 1) - soilSensors;
        for (int i = 0; i < soilSensorsCounts; i++) {
            const char command[] = "sh";
            const char *commandEnd = Tools::intToChar(i + 1);
            strcat(command, commandEnd);
            const char *soilHumidityParam = Sensor::getSoilMoisture(soilSensors[i]);
            SerialFrame soilHumidityFrame = SerialFrame(command, soilHumidityParam);
            AppSerial::sendFrame(&soilHumidityFrame);
        }

        int soilTemperatureSensors[3] = {0, 1, 2};
        int soilTemperatureSensorsCounts = *(&soilTemperatureSensors + 1) - soilTemperatureSensors;
        for (int j = 0; j < soilTemperatureSensorsCounts; j++) {
            const char command[] = "st";
            const char *commandEnd = Tools::intToChar(j + 1);
            strcat(command, commandEnd);
            const char *soilTemperatureParam = Sensor::getSoilTemperature(soilTemperatureSensors[j]);
            SerialFrame soilTemperatureFrame = SerialFrame(command, soilTemperatureParam);
            AppSerial::sendFrame(&soilTemperatureFrame);
        }

        const char *lightIntensityParam = Sensor::getLightIntensity();
        SerialFrame lightIntensityFrame = SerialFrame("light", lightIntensityParam);
        AppSerial::sendFrame(&lightIntensityFrame);

        SensorsReadLastTime = millis();
        Sensor::sleep();
    }
}
