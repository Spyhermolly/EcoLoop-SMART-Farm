#pragma once

struct SystemState{

  int dryWaste = 0;
  int wetWaste = 0;
  float wetWasteWater = 0;  // ml
  float metanPrevented = 0; // l
  
  bool compostLowHumidityAlarm = false;

  bool isWaterManuallyTurnedON = false;
  
  bool isSortSystemBusy = false;
  bool isMixSystemBusy = false;
  bool isFarmSystemBusy = false;

  bool isSystemON = false;

  bool isMIX_SYS_OPERATIONAL = true;
  bool isSORT_SYS_OPERATIONAL = true;  
  bool isFARM_SYS_OPERATIONAL = true;

  bool soundEffectFireEvent_BEEP = false;
  bool soundEffectFireEvent_BOING = false;
  bool soundEffectFireEvent_COIN = false;



};