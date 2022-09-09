class DataPoint {
public:
    int temperature;
};

class Sensor {
public:

    virtual void SaveDataToDataPoint(DataPoint& dp) = 0;
};

class TemperatureSensor {
public:
    void SaveDataToDataPoint(DataPoint& dp) {
        dp.temperature += 1;
    }
};
