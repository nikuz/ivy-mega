#ifndef Sensor_h
#define Sensor_h

#include <Arduino.h>

class Sensor {
public:
    Sensor();

    ~Sensor();

    static void initiate();

    static void read();

    static void sleep();

    static char *getSoilTemperature(int sensorId);

    static char *getTemperature();

    static char *getHumidity();

    static char *getSoilMoisture(int sensorId);

    static char *getLightIntensity();
};

#endif /* Sensor_h */
