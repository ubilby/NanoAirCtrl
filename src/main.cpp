// #pragma once
#include <Arduino.h>
#include <TimerMs.h>
#include <ArxSmartPtr.h>
#include <ArxContainer.h>
#include "myAgents.h"

bool check = true;

// (период, мс), (0 не запущен / 1 запущен), (режим: 0 период / 1 таймер)
TimerMs tmr(1000, 1, 0);

// Класс наших наблюдений
DataPoint dp;

//  Классы Сенсоров
//  Это нужно будет убрать в отдельный класс, возможно
std::shared_ptr<Sensor> temperatureSensor = std::make_shared<TemperatureSensor>();
arx::vector<std::shared_ptr<Sensor>> sensors;

// Классы Агентов
std::shared_ptr<Agent> serialAgent = std::make_shared<SerialAgent>();
arx::vector<std::shared_ptr<Agent>> agents;

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(115200);
    temperatureSensor->initiateSensor();
    sensors.push_back(temperatureSensor);
    agents.push_back(serialAgent);
}

// the loop function runs over and over again forever
void loop() {

    if (tmr.tick()) {
        for (auto sensor : sensors) {
            sensor->updateData();
            sensor->SaveDataToDataPoint(dp);
        }
        for(auto agent: agents) {
            agent->sendData(dp);
        }
    }
}
