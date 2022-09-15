#include "Adafruit_SHTC3.h"
#include <SoftwareSerial.h>


class DataPoint {
public:
    float temperature;
    float humidity;

    uint16_t pm10_standard;
    uint16_t pm25_standard;
    uint16_t pm100_standard;

    uint16_t particles_03um;
    uint16_t particles_05um;
    uint16_t particles_10um;
};

struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};

struct pms5003data data;


class Sensor {
public:
    virtual void updateData() = 0;
    virtual void SaveDataToDataPoint(DataPoint& dp) = 0;
    virtual void initiateSensor() = 0;
};


//temperature sensor SHTC3 sda - A4, scl A5
class TemperatureSensor : public Sensor {
public:
    void updateData() {
        Serial.println("6");
        shtc3.getEvent(&humidity, &temp);
        Serial.println("7");

    }

    void initiateSensor() {
        if (! shtc3.begin()) {
            Serial.println("Couldn't find SHTC3");
            // while (1) delay(1);
        }
        else {
            Serial.println("Found SHTC3 sensor");
        }
    }

    void SaveDataToDataPoint(DataPoint& dp) {
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


//pms5003 sensor
class PMS5003Sensor : public Sensor {
public:
    PMS5003Sensor() : pmsSerial(2,3) {

    }

    void updateData() {
        Serial.println("4");

        if(readPMSdata(&pmsSerial)) {
            Serial.println("5");

            Serial.println("send to pms5003 sensor");
        }
        else {
            Serial.println("coudn't send to pms5003 sensor");

        }
    }

    void SaveDataToDataPoint(DataPoint& dp) {
        dp.pm10_standard = data.pm10_standard;
        dp.pm25_standard = data.pm25_standard;;
        dp.pm100_standard = data.pm100_standard;

        dp.particles_03um = data.particles_03um;
        dp.particles_05um = data.particles_05um;
        dp.particles_10um = data.particles_10um;
    };

    void initiateSensor() {
        pmsSerial.begin(9600);
        if (readPMSdata(&pmsSerial)) {
            Serial.println("Found pms5003 sensor");
        }
        else {
            Serial.println("Couldn't Found pms5003 sensor");
        };
    }

    boolean readPMSdata(Stream *s) {
        if (! s->available()) {
            return false;
        }

        // Read a byte at a time until we get to the special '0x42' start-byte
        if (s->peek() != 0x42) {
            s->read();
            return false;
        }

        // Now read all 32 bytes
        if (s->available() < 32) {
            return false;
        }

        uint8_t buffer[32];
        uint16_t sum = 0;
        s->readBytes(buffer, 32);

        // get checksum ready
        for (uint8_t i=0; i<30; i++) {
            sum += buffer[i];
        }

  /* debugging
  for (uint8_t i=2; i<32; i++) {
    Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(", ");
  }
  Serial.println();
  */

    // The data comes in endian'd, this solves it so it works on all platforms
    uint16_t buffer_u16[15];
    for (uint8_t i=0; i<15; i++) {
        buffer_u16[i] = buffer[2 + i*2 + 1];
        buffer_u16[i] += (buffer[2 + i*2] << 8);
    }

    // put it into a nice struct :)
    memcpy((void *)&data, (void *)buffer_u16, 30);

    if (sum != data.checksum) {
        Serial.println("Checksum failure");
        return false;
    }
  // success!
  return true;
}

    pms5003data data;
    SoftwareSerial pmsSerial;
};
