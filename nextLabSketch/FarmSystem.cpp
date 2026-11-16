#include "HardwareSerial.h"
#include "Arduino.h"
#include "FarmSystem.h"

void FarmSystem::init(){
  pinMode(PIN_FARM_PUMP, OUTPUT);
  pinMode(PIN_FARM_SENSOR_SOIL, INPUT);

  digitalWrite(PIN_FARM_PUMP, HIGH);

  timer = millis();

}

void FarmSystem::update(SystemState &state){
  if(millis()-timer >= SOIL_SENSOR_DELAY){
    timer = millis();
    
     bool isSoilDry = digitalRead(PIN_FARM_SENSOR_SOIL);

    if(state.isFARM_SYS_OPERATIONAL && state.isSystemON){
      if(isSoilDry || state.isWaterManuallyTurnedON){
        if (!state.isFarmSystemBusy){
          state.isFarmSystemBusy = true;
          
          digitalWrite(PIN_FARM_PUMP, LOW);
        }        
      }
      else{
        if (state.isFarmSystemBusy){
          state.isFarmSystemBusy = false;
          digitalWrite(PIN_FARM_PUMP, HIGH);
        }
      }
    } 
  }
}