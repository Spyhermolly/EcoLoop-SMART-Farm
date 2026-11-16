#pragma once

#include <Arduino.h>
#include <DHT.h>
#include "Config.h"
#include "SystemState.h"

class MixSystem{
  private:
    DHT dht;
    void setBladeVelocity(float setVelocityPercentage);
    void updateBladeVelocity();
    void calculateIntervalDuration();
    float toPercentage(int power);
    int toPower(float percentage);

    void checkCompostHumidity(SystemState &state);
    unsigned long alarmTimer = 0;
    float lastCompostHumidity = 0.00f;

    float targetVelocity = 0;
    float currentVelocity = targetVelocity;

    float practicalVelocity = 0;

    unsigned long rampTimer = 0;
    unsigned long intevalTimer = 0;
    unsigned long currentIntervalDuration = 0;

  public:
    void init();
    void update(SystemState &state);
    MixSystem();

};