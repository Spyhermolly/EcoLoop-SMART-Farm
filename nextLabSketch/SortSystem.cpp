#include "HardwareSerial.h"
#include "Arduino.h"
#include "SortSystem.h"

void SortSystem::init(){

  pinMode(PIN_SORT_ULTRASONIC_ECHO, INPUT);  
  pinMode(PIN_SORT_ULTRASONIC_TRIG, OUTPUT);

  pinMode(PIN_SORT_SENSOR_MOISTURE, INPUT);

  sortingServo.attach(PIN_SORT_SERVOMOTOR);
  sortingServo.write(SERVO_TILT_ANGLE);

  stateTimer = millis();
  lastUltrasonicReadTime = millis();

}

void SortSystem::update(SystemState &state){
  
  switch (currentState) {
    case SortState::IDLE: {
      if(millis() - lastUltrasonicReadTime >= ULTRASONIC_DELAY){
        lastUltrasonicReadTime = millis();
        if(isHandDetected() && state.isSORT_SYS_OPERATIONAL){
          stateTimer = millis();
          state.isSortSystemBusy = true;
          //state.soundEffectFireEvent_BEEP = true;
          currentState = SortState::READING_MOISTURE;
        }
      }
    break; }

    case SortState::READING_MOISTURE: {
      if(millis() - stateTimer >= WASTE_MOISTURE_SENSOR_DELAY){
        stateTimer = millis();
        wasteMoisture = analogRead(PIN_SORT_SENSOR_MOISTURE);
        //Serial.println(wasteMoisture);

        isWasteWet = wasteMoisture >= THRESHOLD_WASTE_MOISTURE;

        currentState = SortState::TILT;
      }

      break; }

    case SortState::TILT: {
      
      //state.soundEffectFireEvent_BOING = true;
      if (isWasteWet){
        sortingServo.write(SERVO_TILT_ANGLE*3);

        state.wetWaste++;
        state.wetWasteWater += AVEARAGE_WASTE_MASS * WASTE_RETENTION * float(float(wasteMoisture-DRY_WASTE_HUMIDITY) / float(WET_WASTE_HUMIDITY - DRY_WASTE_HUMIDITY));
        state.metanPrevented += METAN_PER_DRY_WASTE*(AVEARAGE_WASTE_MASS - (AVEARAGE_WASTE_MASS * WASTE_RETENTION * float(float(wasteMoisture-DRY_WASTE_HUMIDITY) / float(WET_WASTE_HUMIDITY - DRY_WASTE_HUMIDITY))));
      }
      else{
        sortingServo.write(0);

        state.dryWaste++;
      }
      stateTimer = millis();
      currentState = SortState::RESET_POSITION;
    break; }

    case SortState::RESET_POSITION: {
      if(millis() - stateTimer >= SERVO_TURN_DURATION){
        state.soundEffectFireEvent_COIN = true;
        stateTimer = millis();
        sortingServo.write(SERVO_TILT_ANGLE);
        state.isSortSystemBusy = false;
        currentState = SortState::IDLE;
      }
    break; }
  }
}

bool SortSystem::isHandDetected(){
  digitalWrite(PIN_SORT_ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_SORT_ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(PIN_SORT_ULTRASONIC_TRIG, LOW);
    
  unsigned long durationUltrasonic = pulseIn(PIN_SORT_ULTRASONIC_ECHO, HIGH, ULTRASONIC_TIMEOUT);

  if(durationUltrasonic == false){
    return false;
  }

  float distanceCm = (durationUltrasonic * 0.0343f) / 2.0f;
  //Serial.println(distanceCm);

  return (distanceCm <= THRESHOLD_HAND_DISTANCE);

}