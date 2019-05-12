//#define PRODUCTION 1    // production
#define PRODUCTION 0    // development
//#define DEBUG 1

#define VERSION_ID "1"

#if PRODUCTION
#define VERSION_MARKER "P"
#else
#define VERSION_MARKER "D"
#endif

#define VERSION VERSION_MARKER VERSION_ID

#ifdef DEBUG
#define DEBUG_PRINT(x)  Serial.print(x)
#define DEBUG_PRINTLN(x)  Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

#define ONE_WIRE_BUS A0
#define SHT_DATA_PIN  10
#define SHT_CLOCK_PIN 11

#define I2C_LIGHT_SENSOR_ID 5
#define I2C_SOIL_SENSOR_1_ID 4
#define I2C_SOIL_SENSOR_2_ID 3
#define I2C_SOIL_SENSOR_3_ID 2

// relays defined in Relay.cpp
