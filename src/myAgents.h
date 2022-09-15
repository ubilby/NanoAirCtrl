#include "mySensors.h"

class Agent {
public:
    virtual void sendData(const DataPoint& dp) = 0;
};


class SerialAgent : public Agent {
public:
    void sendData(const DataPoint& dp) {
        Serial.print("Temperature: "); Serial.print(dp.temperature, 1); Serial.println(" degrees C");
        Serial.print("Humidity: "); Serial.print(dp.humidity, 0); Serial.println("% rH");
        Serial.print("Particles > 0.3um / 0.1L air:"); Serial.println(dp.particles_03um);
        Serial.print("Particles > 0.5um / 0.1L air:"); Serial.println(dp.particles_05um);
        Serial.print("Particles > 1.0um / 0.1L air:"); Serial.println(dp.particles_10um);
        Serial.print("PM 1.0: "); Serial.print(dp.pm10_standard);
        Serial.print("\t\tPM 2.5: "); Serial.print(dp.pm25_standard);
        Serial.print("\t\tPM 10: "); Serial.println(dp.pm100_standard);
    }

    void foo() {

    }
};
