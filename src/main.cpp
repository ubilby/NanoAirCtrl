#include <Arduino.h>
#include "myClasses.h"
#include <TimerMs.h>

// (период, мс), (0 не запущен / 1 запущен), (режим: 0 период / 1 таймер)
TimerMs tmr(2000, 1, 0);

TemperatureSensor ts;
DataPoint dp;
// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(115200);
  // initialize digital pin LED_BUILTIN as an output.
  dp.temperature = 0;
}

// the loop function runs over and over again forever
void loop() {
    if (tmr.tick()) {
        ts.SaveDataToDataPoint(dp);
        Serial.println(dp.temperature);
    }
}
