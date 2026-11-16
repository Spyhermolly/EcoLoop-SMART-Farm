#include "HardwareSerial.h"
#include "pins_arduino.h"
#include "Arduino.h"
#include "MixSystem.h"

MixSystem::MixSystem() : dht(PIN_MIX_DHT11_HUMIDITY, DHT11){}

void MixSystem::init(){
  pinMode(PIN_MIX_N20_BLADE, OUTPUT);
  pinMode(PIN_MIX_FAN, OUTPUT);
  pinMode(PIN_MIX_DHT11_HUMIDITY, INPUT);
  dht.begin();



  digitalWrite(PIN_MIX_FAN, HIGH);
  setBladeVelocity(0);

  calculateIntervalDuration();
  intevalTimer = millis();
  
}

void MixSystem::update(SystemState &state){ 
  if (millis() - intevalTimer >= currentIntervalDuration){
    calculateIntervalDuration();
    intevalTimer = millis();
    rampTimer = millis();
    practicalVelocity = random(0, 100)/100.0f;
    //Serial.println(practicalVelocity);
    
    if(state.isMixSystemBusy == false){
      setBladeVelocity(toPercentage(practicalVelocity));
    }
  }
  
  if(state.isSortSystemBusy || !state.isMIX_SYS_OPERATIONAL || !state.isSystemON){
    // We turn off
    state.isMixSystemBusy = true;
    setBladeVelocity(0);
    digitalWrite(PIN_MIX_FAN, HIGH);
  }
  else{
    // We turn on
    state.isMixSystemBusy = false;
    setBladeVelocity(practicalVelocity);
    digitalWrite(PIN_MIX_FAN, LOW);
  }

  updateBladeVelocity();

  if(state.isMIX_SYS_OPERATIONAL && state.isSystemON) checkCompostHumidity(state);

}

void MixSystem::setBladeVelocity(float setVelocityPercentage){

  int setVelocity = toPower(setVelocityPercentage);
  targetVelocity = setVelocity;   

}

void MixSystem::updateBladeVelocity(){

  if(millis() - rampTimer >= MOTOR_DELAY_TIME){
    // We update the new velocity
    rampTimer = millis();

    float diff = targetVelocity - currentVelocity;
    if(abs(diff) < 1.0f){
      currentVelocity = targetVelocity;
    }
    else{ // Snap to targetVelocity
      currentVelocity += diff / MOTOR_FRICTION_COEFICIENT;
    }
    //Serial.println(currentVelocity);

    // We rewrite the motor
    analogWrite(PIN_MIX_N20_BLADE, abs(currentVelocity));
  }
}

void MixSystem::calculateIntervalDuration(){
  currentIntervalDuration = MIX_INTERVAL_DURATION - random((MIX_INTERVAL_RANDOM_COEFICIENT*-100), (MIX_INTERVAL_RANDOM_COEFICIENT*100))/100.0f * MIX_INTERVAL_DURATION;
}

float MixSystem::toPercentage(int power){
  return (float)power/MAXIMUM_MOTOR_POWER;
}

int MixSystem::toPower(float percentage){
  return round(percentage*MAXIMUM_MOTOR_POWER);
}

void MixSystem::checkCompostHumidity(SystemState &state){
  if(millis() - alarmTimer >= HUMIDITY_ALARM_DELAY){
    alarmTimer = millis();
    float tempHumidity = dht.readHumidity();
    Serial.println(tempHumidity);
    if(!isnan(tempHumidity)){
      lastCompostHumidity = tempHumidity;
      Serial.println(lastCompostHumidity);
      if(lastCompostHumidity<THRESHOLD_COMPOST_HUMIDITY){
        state.compostLowHumidityAlarm = true;
      }
      else{
        state.compostLowHumidityAlarm = false;
      }
    }
    else{
      state.compostLowHumidityAlarm = false;
    }
  }
}