#pragma once

#include <Arduino.h>
#include "Config.h"
#include "SystemState.h"

class FarmSystem{
  private:
    unsigned long timer = 0;

  public:
    void init();
    void update(SystemState &state);

};