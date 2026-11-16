#include "pins_arduino.h"
#pragma once

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include "Config.h"
#include "SystemState.h"

enum class DisplayState{
  IDLE,
  SELECTION,
  INFORMATION
};

enum class LED_State{
  OFF,
  RED,
  YELLOW,
  GREEN,
  BLUE,
  WHITE
};

enum class SoundEffect {
  NONE,
  CLICK,
  POP,
  ALERT,
  BOINGNCOIN,
  WIN,
  BEEP,
  CONFIRMED,
  COIN
};

enum class Displayables{
  NONE,
  IDENTIFICATION,
  TEMPORALS,
  GASES,
  VITAMINS,
  MINERALS,
  FIBERS,
  ANTIOXIDANTS,
  PROTEINS,
  DISHES,
  WATER,
  MONEY,
  ENERGY,
  MASS,
  ARBORS,
  WASTE,
  METAN,
  TOTAL_DISPLAYABLES
};

struct DisplayedVariable{
  String variable1 = "";
  String variable2 = "";
  String variable3 = "";
  String variable4 = "";
  String variable5 = ""; 
};

class DisplaySystem{
  private:

    // Controller
    float getJoystickX();
    bool isJoystickUsageValid();
    bool isButtonUsageValid();
    float xVal = 0.00f;
    
    // LCD
    //LiquidCrystal_I2C lcd{0x27, 16, 2};

    void trimiteSerial(int row, int col, String text);

    template <typename T>
    void printLCD(int col, int row, T val, int clearWidth = 0) {
      String strVal = String(val);
      while (strVal.length() < clearWidth) {
        strVal += " ";
      }
      trimiteSerial(row, col, strVal);
    }
    void printLCD(int col, int row, const __FlashStringHelper* ifsh, int clearWidth = 0) {
      String strVal = String(ifsh);
      while (strVal.length() < clearWidth) {
        strVal += " ";
      }
      trimiteSerial(row, col, strVal);
    }
    void printLCD(int col, int row, float val, int decimals, int clearWidth = 0);

    // Plant info
    void initiateInformation();
    void updateInformation();
    DisplayedVariable info;
    void getInfo(SystemState &state);
    float growthPercentage = 0.00f;
    void getGrowthPercentage();
    float getScalar(float value);
    float getByMass(float value);

    // LED
    LED_State ledState;
    void changeLED(LED_State newLedState);
    void calibrateLED_toPlant();

    // Buzzer
    SoundEffect currentEffect = SoundEffect::NONE;
    uint8_t soundStep = 0;
    unsigned long soundTimer = 0;
    unsigned long stepDuration = 0;
    void playSound(SoundEffect effect);
    void updateSound(SystemState &state);
    void stopBuzzer();
    unsigned long stateSoundTimer = 0;

    void updateAlarm(SystemState &state);
    unsigned long alarmTimer = 0;
    bool isAlarmStateOn = false;

    // States
    DisplayState displayState = DisplayState::IDLE;
    Displayables displayableInfo = Displayables::TOTAL_DISPLAYABLES;

    // Timers
    unsigned long lcdRefreshTimer =  0;
    unsigned long inputDelayTimer = 0;
    unsigned long buttonDelayTimer = 0;
    void getNewTimeDelay(float xVal);
    unsigned int dynamicDelayTime = INPUT_TEST_DELAY;
    unsigned long long timeAtStart;

    // Plant selected
    PlantType selectedPlant = availablePlants[static_cast<uint8_t>(PlantSpecies::NONE)];
    int hoveredPlantIndex = 7;

    bool isPlantSelected = false;

    int plantsAmount = 0;
    int hoveredAmount = 49;
    
    void circleThroughList(int &initialIndex,  int maxValue, bool clockwise);

  public:
    void init();
    void update(SystemState &state);

};