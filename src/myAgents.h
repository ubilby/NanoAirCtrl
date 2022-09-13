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
    }
};
