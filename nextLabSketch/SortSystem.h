#pragma once

#include <Servo.h>
#include <Arduino.h>
#include "Config.h"
#include "SystemState.h"

enum class SortState{
  IDLE,
  READING_MOISTURE,
  TILT,
  RESET_POSITION
};

class SortSystem{
  private:
    Servo sortingServo;
    
    SortState currentState = SortState::IDLE;
    unsigned long stateTimer = 0;

    bool isWasteWet;
    int wasteMoisture;

    bool isHandDetected(); 
    unsigned long lastUltrasonicReadTime = 0;

  public:
    void init();
    void update(SystemState &state);
};