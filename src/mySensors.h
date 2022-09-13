#include "Adafruit_SHTC3.h"


class DataPoint {
public:
    float temperature;
    float humidity;

};


class Sensor {
public:
    virtual void updateData() = 0;
    virtual void SaveDataToDataPoint(DataPoint& dp) = 0;
    virtual void initiateSensor() = 0;
};


class TemperatureSensor : public Sensor {
public:
    void updateData() {
        Serial.println("inside temperature updateData");
        shtc3.getEvent(&humidity, &temp);
    }

    void initiateSensor() {
        if (! shtc3.begin()) {
            Serial.println("Couldn't find SHTC3");
            while (1) delay(1);
        }
        Serial.println("Found SHTC3 sensor");
    }

    void SaveDataToDataPoint(DataPoint& dp) {
        dp.temperature = temp.temperature;
        dp.humidity = humidity.relative_humidity;
    }

    void test() {
        Serial.println("TemperatureSensor test ok!");
    }

    TemperatureSensor() {
        shtc3 = Adafruit_SHTC3();
    }

private:
    Adafruit_SHTC3 shtc3;
    sensors_event_t humidity, temp;
};
