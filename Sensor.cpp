#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BH1750FVI.h>
#include <SHT1x.h>
#include <I2CSoilMoistureSensor.h>

#include "def.h"
#include "Sensor.h"
#include "Tools.h"
#include "AppI2C.h"

float currentTemperature = 0;
float currentHumidity = 0;
uint16_t currentLightIntensity = 0;
int soilMoisture1 = 0;
int soilMoisture2 = 0;
int soilMoisture3 = 0;

OneWire oneWire(ONE_WIRE_BUS);

SHT1x sht1x(SHT_DATA_PIN, SHT_CLOCK_PIN);

DallasTemperature soilTemperatureSensors(&oneWire);

I2CSoilMoistureSensor soilSensor1(0x41);
I2CSoilMoistureSensor soilSensor2(0x42);
I2CSoilMoistureSensor soilSensor3(0x43);

BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);

Sensor::Sensor() {}

Sensor::~Sensor() {}

void Sensor::initiate() {
    soilTemperatureSensors.begin();
    DEBUG_PRINTLN(" -- SoilTemperatureSensors initiated");
    AppI2C::select(I2C_LIGHT_SENSOR_ID);
    DEBUG_PRINTLN(" ---- AppI2C port for LIGHT selected");
    LightSensor.begin();
    DEBUG_PRINTLN(" -- LightSensor initiated");
    AppI2C::select(I2C_SOIL_SENSOR_1_ID);
    soilSensor1.begin();
    DEBUG_PRINTLN(" -- soilSensor1 initiated");
    AppI2C::select(I2C_SOIL_SENSOR_2_ID);
    soilSensor2.begin();
    DEBUG_PRINTLN(" -- soilSensor2 initiated");
    AppI2C::select(I2C_SOIL_SENSOR_3_ID);
    soilSensor3.begin();
    DEBUG_PRINTLN(" -- soilSensor3 initiated");
}

void Sensor::read() {
    soilTemperatureSensors.requestTemperatures();
    currentTemperature = sht1x.readTemperatureC();
    currentHumidity = sht1x.readHumidity();

    AppI2C::select(I2C_LIGHT_SENSOR_ID);
    currentLightIntensity = LightSensor.GetLightIntensity();

    AppI2C::select(I2C_SOIL_SENSOR_1_ID);
    soilMoisture1 = soilSensor1.getCapacitance();

    AppI2C::select(I2C_SOIL_SENSOR_2_ID);
    soilMoisture2 = soilSensor2.getCapacitance();

    AppI2C::select(I2C_SOIL_SENSOR_3_ID);
    soilMoisture3 = soilSensor3.getCapacitance();
}

void Sensor::sleep() {
    AppI2C::select(I2C_SOIL_SENSOR_1_ID);
    soilSensor1.sleep();

    AppI2C::select(I2C_SOIL_SENSOR_2_ID);
    soilSensor2.sleep();

    AppI2C::select(I2C_SOIL_SENSOR_3_ID);
    soilSensor3.sleep();
}

// temperature

char *Sensor::getTemperature() {
    char *result = Tools::intToChar(currentTemperature);
    return result;
}

// humidity

char *Sensor::getHumidity() {
    char *result = Tools::intToChar(currentHumidity);
    return result;
}

// soil

char *Sensor::getSoilTemperature(int sensorId) {
    const float temperature = soilTemperatureSensors.getTempCByIndex(sensorId);
    char *result = Tools::intToChar(temperature);
    return result;
}

char *Sensor::getSoilMoisture(int sensorId) {
    int moisture;
    switch (sensorId) {
        case 1:
            moisture = soilMoisture1;
            break;
        case 2:
            moisture = soilMoisture2;
            break;
        case 3:
            moisture = soilMoisture3;
            break;
    }

    char *result = Tools::intToChar(moisture);
    return result;
}

// light

char *Sensor::getLightIntensity() {
    char *result = Tools::intToChar(currentLightIntensity);
    return result;
}
