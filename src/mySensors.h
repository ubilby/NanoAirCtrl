#include "Adafruit_SHTC3.h"
#include <PMserial.h> // Arduino library for PM sensors with serial interface


class DataPoint {
public:
    float temperature;
    float humidity;

    unsigned int pm10_standard;
    unsigned int pm25_standard;
    unsigned int pm100_standard;

    unsigned int particles_03um;
    unsigned int particles_05um;
    unsigned int particles_10um;
};


class Sensor {
public:
    virtual void updateData() = 0;
    virtual void SaveDataToDataPoint(DataPoint& dp) = 0;
    virtual void initiateSensor() = 0;
};


//temperature sensor SHTC3 sda - A4, scl A5
class TemperatureSensor : public Sensor {
public:
    void updateData() override  {
        shtc3.getEvent(&humidity, &temp);

    }

    void initiateSensor() override  {
        if (! shtc3.begin()) {
            Serial.println("Couldn't find SHTC3");
            // while (1) delay(1);
        }
        else {
            Serial.println("Found SHTC3 sensor");
        }
    }

    void SaveDataToDataPoint(DataPoint& dp) override  {
        dp.temperature = temp.temperature;
        dp.humidity = humidity.relative_humidity;
    }

    TemperatureSensor() {
        shtc3 = Adafruit_SHTC3();
    }

private:
    Adafruit_SHTC3 shtc3;
    sensors_event_t humidity, temp;
};


//pms5003 sensor's rx - 3, tx - 2 
class PMS5003Sensor : public Sensor {
public:
    PMS5003Sensor() : pms (PMS5003, 2, 3){

    }

    void updateData() override {
        pms.read();
    }

    void SaveDataToDataPoint(DataPoint& dp) override  {
        dp.pm10_standard = pms.pm01 ;
        dp.pm25_standard = pms.pm25;
        dp.pm100_standard = pms.pm10;

        dp.particles_03um = pms.n0p3;
        dp.particles_05um = pms.n0p5;
        dp.particles_10um = pms.n1p0;
    };

    void initiateSensor() override  {
        pms.init();
        if (not pms) {
            Serial.println("Couldn't find pms5003");
        }
    }

    SerialPM pms;

};
