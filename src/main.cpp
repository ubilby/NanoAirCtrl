// #pragma once
#include <Arduino.h>
#include <TimerMs.h>
#include <ArxSmartPtr.h>
#include <ArxContainer.h>
#include "myAgents.h"

// (период, мс), (0 не запущен / 1 запущен), (режим: 0 период / 1 таймер)
TimerMs tmr(5000, 1, 0);
TimerMs tmr2(1000, 1, 0);


// Класс наших наблюдений
DataPoint dp;

//  Инициация сенсоров
std::shared_ptr<Sensor> temperatureSensor = std::make_shared<TemperatureSensor>();
std::shared_ptr<Sensor> pms5003Sensor = std::make_shared<PMS5003Sensor>();

arx::vector<std::shared_ptr<Sensor>> sensors;

// Инициация агентов
std::shared_ptr<Agent> serialAgent = std::make_shared<SerialAgent>();

arx::vector<std::shared_ptr<Agent>> agents;


// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(115200);
    temperatureSensor->initiateSensor();
    pms5003Sensor->initiateSensor();
    sensors.push_back(temperatureSensor);
    sensors.push_back(pms5003Sensor);
    agents.push_back(serialAgent);
}


// the loop function runs over and over again forever
void loop() {

    if (tmr.tick()) {
        for (unsigned int i =0; i < sensors.size(); i++) {
            sensors[i]->updateData();
            sensors[i]->SaveDataToDataPoint(dp);
        }

        for (unsigned int i =0; i < agents.size(); i++) {
            agents[i]->sendData(dp);
        }
        // pms5003Sensor->updateData();
        // pms5003Sensor->SaveDataToDataPoint(dp);
        // serialAgent->sendData(dp);
    }
}
