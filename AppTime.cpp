#include <Arduino.h>
#include <RtcDS3231.h>
#include <Wire.h>
#include "AppI2C.h"

#include "def.h"
#include "AppTime.h"
#include "Tools.h"

RtcDS3231 <TwoWire> Rtc(Wire);

bool rtcBatteryAlive = true;
static const char months[][4] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

AppTime::AppTime() {}

AppTime::~AppTime() {}

void AppTime::RTCBegin() {
    AppI2C::select(0);
    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    if (!Rtc.IsDateTimeValid()) {
        Rtc.SetDateTime(compiled);
        Serial.println("RTC lost confidence in the DateTime!");
    }
    if (!Rtc.GetIsRunning()) {
        Rtc.SetIsRunning(true);
        Serial.println("RTC was not actively running, starting now");
    }
    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) {
        Rtc.SetDateTime(compiled);
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
    }

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
}

char *AppTime::RTCGetTemperature() {
    AppI2C::select(0);
    RtcTemperature rtcTemp = Rtc.GetTemperature();
    static char temperatureStr[2];
    float rtcTempFloat = rtcTemp.AsFloatDegC();

    char *result = Tools::intToChar(rtcTempFloat);
    return result;
}

char *AppTime::RTCBattery() {
    return rtcBatteryAlive ? "1" : "0";
}

bool AppTime::RTCIsDateTimeValid() {
    AppI2C::select(0);
    const bool isValid = Rtc.IsDateTimeValid();

    if (!isValid) {
        rtcBatteryAlive = false;
    } else {
        rtcBatteryAlive = true;
    }

    return isValid;
}

char *AppTime::RTCGetCurrentTime() {
    AppI2C::select(0);
    RtcDateTime rtcTime = Rtc.GetDateTime();
    AppTime::RTCIsDateTimeValid();

    static char timeStr[20];
    snprintf_P(
            timeStr,
            sizeof timeStr,
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            rtcTime.Month(),
            rtcTime.Day(),
            rtcTime.Year(),
            rtcTime.Hour(),
            rtcTime.Minute(),
            rtcTime.Second()
    );

    return timeStr;
}

void AppTime::RTCDateTimeUpdate(const char *command, const char *param) {
    // 10/10/2018 00:09:21
    if (strcmp(command, "time") == 0) {
        int month = -1;
        int day = -1;
        int year = -1;
        char date[15];
        char *time;

        char dateString[20];
        strncpy(dateString, param, 20);

        char *pch;
        const char *delimiter = " /";
        pch = strtok(dateString, delimiter);
        int i = 0;
        while (pch != NULL) {
            switch (i) {
                case 0:
                    month = atoi(pch);
                    break;
                case 1:
                    day = atoi(pch);
                    break;
                case 2:
                    year = atoi(pch);
                    break;
                case 3:
                    time = pch;
                    break;
            }
            pch = strtok(NULL, delimiter);
            i++;
        }

        sprintf(
                date,
                "%.3s%3d %4d",
                months[month - 1],
                day,
                year
        );

        RtcDateTime ntpDateTime = RtcDateTime(date, time);
        AppI2C::select(0);
        Rtc.SetDateTime(ntpDateTime);
    }
}