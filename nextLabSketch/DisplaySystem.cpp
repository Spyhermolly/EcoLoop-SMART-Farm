#include "WString.h"
#include "HardwareSerial.h"
#include "Arduino.h"
#include "DisplaySystem.h"

void DisplaySystem::init(){

  pinMode(PIN_DISP_BUTTON, INPUT_PULLUP); 
  pinMode(PIN_DISP_X, INPUT);

  pinMode(PIN_DISP_BUZZER, OUTPUT);
  pinMode(PIN_MULTI_LED_RED, OUTPUT);
  pinMode(PIN_MULTI_LED_GREEN, OUTPUT);
  pinMode(PIN_MULTI_LED_BLUE, OUTPUT);

  digitalWrite(PIN_DISP_BUZZER, HIGH);

  //lcd.init();
  //lcd.backlight();

  printLCD(0, 0, F("--Ferma  SMART--"));
  printLCD(0, 1, F("-Alege o planta-"));

  changeLED(LED_State::WHITE);

}

void DisplaySystem::update(SystemState &state){

  updateSound(state);

  switch (displayState) {
    if(!isAlarmStateOn){
      case DisplayState::IDLE: // We wait for input

        if(millis() - inputDelayTimer >= INPUT_TEST_DELAY){ // verificam joystickul
          inputDelayTimer = millis();
          if(abs(getJoystickX())>JOYSTICK_X_DEADZONE_TOLERANCE){
            displayState = DisplayState::SELECTION;
            changeLED(LED_State::YELLOW);
          }
        }

      break;

      case DisplayState::SELECTION: // We select plant info
      
        if(isButtonUsageValid()){
          playSound(SoundEffect::CONFIRMED);
          if(isPlantSelected){
            // choosing amount
            plantsAmount = hoveredAmount;

            printLCD(0, 0, F("-Planta  setata-"));
            printLCD(0, 1, F("Succes si spor!"), 16);

            displayState = DisplayState::INFORMATION;
            timeAtStart = millis();
            if (selectedPlant.name == "Patrunjel") timeAtStart = -4856167000; 

            state.isSystemON = true;

            calibrateLED_toPlant();
          }
            
          else{
            // choosing plant
            selectedPlant = availablePlants[hoveredPlantIndex];
            
            
            isPlantSelected = true; 

            printLCD(0, 0, F("---Excelent !---"));
            printLCD(0, 1, F("--Cate plante?--"));

            changeLED(LED_State::GREEN);

          }          
        } 

        if(isJoystickUsageValid()){
            playSound(SoundEffect::POP);
            if(isPlantSelected){
              // Choosing amount
                if(xVal>0) circleThroughList(hoveredAmount, MAXIMUM_PLANT_AMOUNT, true);
                else circleThroughList(hoveredAmount, MAXIMUM_PLANT_AMOUNT, false);

                printLCD(0, 0, "Numar fire:", 16);
                printLCD(0, 1, hoveredAmount, 16);
                printLCD(8, 1, "x bucati");
            }   

            else{ 
            // Choosing plant species
                if(xVal>0) circleThroughList(hoveredPlantIndex, TOTAL_PLANTS, true);
                else circleThroughList(hoveredPlantIndex, TOTAL_PLANTS, false);

                printLCD(0, 0, availablePlants[hoveredPlantIndex].name, 16);
                printLCD(0, 1, availablePlants[hoveredPlantIndex].latinName, 16);
            }        
        }

      break;

      case DisplayState::INFORMATION: // We display info

        if(isJoystickUsageValid()){
          playSound(SoundEffect::POP);
          int infoIdx = static_cast<int>(displayableInfo);
          int totalCategories = static_cast<int>(Displayables::TOTAL_DISPLAYABLES);
          
          if (xVal > 0) circleThroughList(infoIdx, totalCategories, true);
          else circleThroughList(infoIdx, totalCategories, false);
          
          displayableInfo = static_cast<Displayables>(infoIdx);

          // One time & dynamic variables (All types of variables)
          getInfo(state);
          initiateInformation();

        }

        if(isButtonUsageValid()){
          if(state.isWaterManuallyTurnedON) state.isWaterManuallyTurnedON = false;
          else state.isWaterManuallyTurnedON = true;
          Serial.println(state.isWaterManuallyTurnedON);
        }

        if(millis() - lcdRefreshTimer >= LCD_REFRESH_DURATION){
          lcdRefreshTimer = millis();

          // Only dynamic variables update in loop
          getInfo(state);
          updateInformation();
        }

      break;
    }
  }

  updateAlarm(state);

}

void DisplaySystem::circleThroughList(int &initialIndex, int maxValue, bool clockwise){
  int step = clockwise ? 1 : -1;
  initialIndex = (initialIndex + step) % maxValue;
  if(initialIndex == 0){
    if(clockwise) initialIndex = 1;
    else initialIndex = maxValue-1;
  }
}

void DisplaySystem::getNewTimeDelay(float xVal){
  dynamicDelayTime = map(abs(xVal)*100, 10, 100, INPUT_SWITCH_MAX_DURATION, INPUT_SIWTCH_MIN_DURATION);
}

void DisplaySystem::printLCD(int col, int row, float val, int decimals, int clearWidth = 0) {
  String strVal = String(val, decimals);
  while (strVal.length() < clearWidth) {
    strVal += " ";
  }
  trimiteSerial(row, col, strVal);
}

float DisplaySystem::getJoystickX(){
  int rawX = analogRead(PIN_DISP_X)-(JOYSTICK_X_PRACTICAL_CENTER-JOYSTICK_X_TEORETICAL_CENTER)  ;
  float xVal = map(rawX, 0, 1023, -100, 100) / 100.0f;
  if(abs(xVal) < JOYSTICK_X_DEADZONE_TOLERANCE){ // Deadzone negate
    xVal = 0.00f;
  }
  xVal = constrain(xVal*JOYSTICK_X_MULTIPLY_ERROR, -1.00f, 1.00f);  
  return xVal;
}

void DisplaySystem::trimiteSerial(int row, int col, String text) {
  Serial.print(row);
  Serial.print(",");
  Serial.print(col);
  Serial.print(",");
  Serial.println(text);
}

bool DisplaySystem::isJoystickUsageValid(){
  if(millis() - inputDelayTimer >= dynamicDelayTime){ // Verificam timerul
        // Timer done
        inputDelayTimer = millis();
        xVal = getJoystickX();
      
        // Are we even touching joystick?
        if(abs(xVal)>JOYSTICK_X_DEADZONE_TOLERANCE){  
          getNewTimeDelay(xVal);
          return true;
        }
        else{
          dynamicDelayTime = INPUT_TEST_DELAY;
          return false;
        }
  }
  else return false;

}

bool DisplaySystem::isButtonUsageValid(){
  if(digitalRead(PIN_DISP_BUTTON) == LOW && millis() - buttonDelayTimer >= INPUT_BUTTON_DELAY){
    buttonDelayTimer = millis();
    return true;
  }
  return false;
}

void DisplaySystem::initiateInformation(){
  switch (displayableInfo) {
    case Displayables::IDENTIFICATION:
      printLCD(0, 0, info.variable1, 16);
      printLCD(0, 1, info.variable2, 16);
    break; 
    case Displayables::TEMPORALS:
      printLCD(0, 0, info.variable4, 16);
      printLCD(2, 0, F("z"));
      printLCD(4, 0, info.variable3);
      printLCD(6, 0, F("h"));
      printLCD(8, 0, info.variable2);
      printLCD(10, 0, F("m"));
      printLCD(12, 0, info.variable1);
      printLCD(14, 0, F("s"));
      printLCD(0, 1, F("Dezv:          %"), 16);
      printLCD(8, 1, info.variable5);
    break; 
    case Displayables::GASES:

      printLCD(0, 0, F("CO2:          ml"));
      printLCD(5, 0, info.variable1);

      printLCD(0, 1, F(" O2:          ml"));
      printLCD(5, 1, info.variable2);

    break;
    case Displayables::VITAMINS:

      printLCD(0, 0, F("Vit.  :       mg"), 16);
      printLCD(5, 0, char(selectedPlant.vitamin));
      printLCD(8, 0, info.variable1);

      printLCD(0, 1, F("DZR   :        %"), 16);
      printLCD(9, 1, info.variable2);

    break;
    case Displayables::MINERALS:
      printLCD(0, 0, F("Minerale "), 16);
      switch (selectedPlant.mineral) {
        case MINERAL_FIER: printLCD(9, 0, F("fier:")); break;
        case MINERAL_CALCIU: printLCD(9, 0, F("calciu:")); break;
        case MINERAL_POTASIU: printLCD(9, 0, F("potasiu")); break;
      }

      printLCD(0, 1, F("              mg"), 16); 
      printLCD(0, 1, info.variable1);  
    break;  
    case Displayables::FIBERS:
      printLCD(0, 0, F("Fibre totale:"), 16);

      printLCD(0, 1, F("              mg"), 16); 
      printLCD(0, 1, info.variable1);  
    break;  
    case Displayables::ANTIOXIDANTS:
      printLCD(0, 0, F("Antioxidanti:"), 16);

      printLCD(0, 1, F("              mg"), 16); 
      printLCD(0, 1, info.variable1);  
    break;  
    case Displayables::PROTEINS:
      printLCD(0, 0, F("Proteine totale:"), 16);

      printLCD(0, 1, F("              mg"), 16); 
      printLCD(0, 1, info.variable1);  
    break;  
    case Displayables::DISHES:
      printLCD(0, 0, F("Portii de masa:"), 16);

      printLCD(0, 1, F("          portii"), 16); 
      printLCD(0, 1, info.variable1);  
    break;
    case Displayables::WATER:
      printLCD(0, 0, F("Apa economisita:"), 16);

      printLCD(0, 1, F("              ml"), 16); 
      printLCD(0, 1, info.variable1);  
    break;
    case Displayables::MONEY:
      printLCD(0, 0, F("Bani salvati:"), 16);

      printLCD(0, 1, F("  lei, "), 16);
      printLCD(0, 1, info.variable1);

      printLCD(7, 1, F("     bani"), 16);
      printLCD(7, 1, info.variable2);
    break;
    case Displayables::ENERGY:
      printLCD(0, 0, F("Energie salvata:"), 16);

      printLCD(0, 1, F("           Watts"), 16); 
      printLCD(0, 1, info.variable1);  
    break;
    case Displayables::MASS:
      printLCD(0, 0, F("Greutate:"), 16);

      printLCD(0, 1, F("               g"), 16); 
      printLCD(0, 1, info.variable1);  
    break;
    case Displayables::ARBORS:
      printLCD(0, 0, F("Arbori echiv.:"), 16);
      printLCD(0, 1, info.variable1, 16);
    break;
    case Displayables::WASTE:
      printLCD(0, 0, F("Deseu umed : "), 16);
      printLCD(13, 0, info.variable1);

      printLCD(0, 1, F("Deseu uscat: "), 16);
      printLCD(13, 1, info.variable2);  
    break;
    case Displayables::METAN:
      printLCD(0, 0, F("CH4 prvnt.:    l"), 16);
      printLCD(11, 0, info.variable1);

      printLCD(0, 1, F("Echiv. CO2:    l"), 16);
      printLCD(11, 1, info.variable2);  
    break;
    default:
      printLCD(0, 0, info.variable1);
      printLCD(0, 1, info.variable2);    
    break;
  }
}

void DisplaySystem::getInfo(SystemState &state){
  switch (displayableInfo) {  
    case Displayables::IDENTIFICATION:{
      info.variable1 = selectedPlant.name;
      info.variable2 = selectedPlant.latinName;
      break;
    }
    case Displayables::TEMPORALS: { 
      unsigned long secunde = selectedPlant.growthTime-((millis()-timeAtStart) / 1000UL);
      unsigned long minute = secunde/60UL;
      secunde %= 60;
      unsigned long ore = minute/60UL;
      minute %= 60;
      unsigned int zile = ore/24;
      ore %= 24;

      info.variable1 = secunde;
      info.variable2 = minute;
      info.variable3 = ore;
      info.variable4 = zile; 

      getGrowthPercentage();
      info.variable5 = String(growthPercentage*100.00f, 4);  
      break;
    }
    case Displayables::GASES:{
      getGrowthPercentage();
      info.variable1 = String(getScalar(selectedPlant.carbonDioxideExtraction), 3);      
      info.variable2 = String(getScalar(selectedPlant.oxigenProduction), 3);
      break; 
    }
    case Displayables::VITAMINS: {
      info.variable1 = String(getByMass(selectedPlant.vitaminMass), 2);
      
      float dzrLimit;
      switch (selectedPlant.vitamin){
        case VITAMIN_A: dzrLimit = DRZ_A; break;
        case VITAMIN_C: dzrLimit = DRZ_C; break;
        case VITAMIN_K: dzrLimit = DRZ_K; break;
      }  

      info.variable2 = String((getByMass(selectedPlant.vitaminMass))/(selectedPlant.growthTime/86400)/dzrLimit*100, 2);
      break;
    }
    case Displayables::MINERALS: {
      info.variable1 = String(getByMass(selectedPlant.mineralMass), 2);
      break;
    }
    case Displayables::FIBERS: {
      info.variable1 = String(getByMass(selectedPlant.fibersMass), 2);
      break;
    }
    case Displayables::ANTIOXIDANTS: {
      info.variable1 = String(getByMass(selectedPlant.antioxidantsMass), 2);
      break;
    }
    case Displayables::PROTEINS: {
      info.variable1 = String(getByMass(selectedPlant.proteinsMass), 2);
      break;
    }
    case Displayables::DISHES: {
      info.variable1 = String(selectedPlant.portiiDeHrana*plantsAmount, 2);
      break;
    }
    case Displayables::WATER: {
      getGrowthPercentage();
      info.variable1 = String(getScalar(selectedPlant.waterUsage*1000)+state.wetWasteWater, 3);
      break;
    }   
    case Displayables::MONEY: {
      getGrowthPercentage();
      // Pret apa utilizata + produs
      float bani = (getScalar(selectedPlant.marketPrice))+(getScalar(selectedPlant.waterUsage)*PRET_LITRU_APA);
      int lei = int(bani);
      bani = float(bani-lei)*100;
      info.variable1 = lei;
      info.variable2 = String(bani, 2);
      break;
    }   
    case Displayables::ENERGY: {
      getGrowthPercentage();
      info.variable1 = String(getScalar(selectedPlant.energyUsage), 5);
      break;
    }  
    case Displayables::MASS: {
      getGrowthPercentage();
      info.variable1 = String(getScalar(selectedPlant.finalMass), 4);
      break;
    }  
    case Displayables::ARBORS: {
      getGrowthPercentage();
      info.variable1 = String(double(getScalar(selectedPlant.energyUsage)/ENERGIE_ARBORE), 14);
      break;
    }
    case Displayables::WASTE: {
      info.variable1 = state.wetWaste;
      info.variable2 = state.dryWaste;
      break;
    }
    case Displayables::METAN: {
      info.variable1 = String(state.metanPrevented, 2);
      info.variable2 = String(state.metanPrevented*CO2_ECHIVALENT_FOR_METHANE, 2);
    break;
    }
    default:
      info.variable1 = F("Categorie:");
      info.variable2 = F("In lucru...");
    break;
  }
}

void DisplaySystem::updateInformation(){
  switch (displayableInfo) {
    case Displayables::TEMPORALS:{
      printLCD(0, 0, F("  "));
      printLCD(0, 0, info.variable4);
      printLCD(4, 0, F("  "));
      printLCD(4, 0, info.variable3);
      printLCD(8, 0, F("  "));
      printLCD(8, 0, info.variable2);
      printLCD(12, 0, F("  "));
      printLCD(12, 0, info.variable1);

      printLCD(8, 1, info.variable5);
      break; 
    }
    case Displayables::GASES:{
      printLCD(5, 0, info.variable1);
      printLCD(5, 1, info.variable2); 
      break;
    }
    case Displayables::WATER:{
      printLCD(0, 1, info.variable1); 
      break;
    }
    case Displayables::MONEY:{
      printLCD(0, 1, F("  "));
      printLCD(0, 1, info.variable1);
      printLCD(7, 1, F("     "));
      printLCD(7, 1, info.variable2);

      break;
    }
    case Displayables::ENERGY:{
      printLCD(0, 1, info.variable1);  
      break;
    }
    case Displayables::MASS:{
      printLCD(0, 1, info.variable1);  
      break;
    }
    case Displayables::ARBORS:{
      getGrowthPercentage();
      printLCD(0, 1, info.variable1, 16);
      break;
    }
    case Displayables::WASTE:{
      printLCD(13, 0, info.variable1);
      printLCD(13, 1, info.variable2);  
      break;
    }
    case Displayables::METAN:{
      printLCD(11, 0, info.variable1);
      printLCD(11, 1, info.variable2); 
      break;
    } 
    default:
      //NULL
    break;
  }
}

void DisplaySystem::getGrowthPercentage(){
  growthPercentage = ((millis()-timeAtStart)/1000.00f)/selectedPlant.growthTime;
}

float DisplaySystem::getScalar(float value){
  return value*growthPercentage*plantsAmount;
}

float DisplaySystem::getByMass(float value){
  return value*selectedPlant.finalMass*plantsAmount/100;
}

void DisplaySystem::changeLED(LED_State newLedState) {
  switch (newLedState) {
    case LED_State::OFF:
      digitalWrite(PIN_MULTI_LED_RED, LOW);
      digitalWrite(PIN_MULTI_LED_GREEN, LOW);
      digitalWrite(PIN_MULTI_LED_BLUE, LOW);
      break;

    case LED_State::RED:
      digitalWrite(PIN_MULTI_LED_RED, HIGH);
      digitalWrite(PIN_MULTI_LED_GREEN, LOW);
      digitalWrite(PIN_MULTI_LED_BLUE, LOW);
      break;

    case LED_State::YELLOW: // Roșu + Verde = Galben
      digitalWrite(PIN_MULTI_LED_RED, HIGH);
      digitalWrite(PIN_MULTI_LED_GREEN, HIGH);
      digitalWrite(PIN_MULTI_LED_BLUE, LOW);
      break;

    case LED_State::GREEN:
      digitalWrite(PIN_MULTI_LED_RED, LOW);
      digitalWrite(PIN_MULTI_LED_GREEN, HIGH);
      digitalWrite(PIN_MULTI_LED_BLUE, LOW);
      break;

    case LED_State::BLUE:
      digitalWrite(PIN_MULTI_LED_RED, LOW);
      digitalWrite(PIN_MULTI_LED_GREEN, LOW);
      digitalWrite(PIN_MULTI_LED_BLUE, HIGH);
      break;

    case LED_State::WHITE: // Roșu + Verde + Albastru = Alb
      digitalWrite(PIN_MULTI_LED_RED, HIGH);
      digitalWrite(PIN_MULTI_LED_GREEN, HIGH);
      digitalWrite(PIN_MULTI_LED_BLUE, HIGH);
      break;
  }
}

void DisplaySystem::calibrateLED_toPlant(){
  if(selectedPlant.preferredLight == PreferredLight::LIGHT_RED){
    changeLED(LED_State::RED);
  }
  else if(selectedPlant.preferredLight == PreferredLight::LIGHT_GREEN){
    changeLED(LED_State::GREEN);
  }
  else if(selectedPlant.preferredLight == PreferredLight::LIGHT_BLUE){
    changeLED(LED_State::BLUE);
  }
  
}

void DisplaySystem::playSound(SoundEffect effect) {
  currentEffect = effect;
  soundStep = 0;
  soundTimer = 0;
  stepDuration = 0;
}

void DisplaySystem::updateSound(SystemState &state) {
  if (millis() - stateSoundTimer >= SOUND_STATE_VERIFY_DELAY){
    stateSoundTimer = millis();
    if(state.soundEffectFireEvent_BEEP){
      state.soundEffectFireEvent_BEEP = false;
      playSound(SoundEffect::BEEP);
    }
    if(state.soundEffectFireEvent_BOING){
      state.soundEffectFireEvent_BOING = false;
      playSound(SoundEffect::BOINGNCOIN);
    }
  }  

  if (currentEffect == SoundEffect::NONE) return;

  if (millis() - soundTimer >= stepDuration) {
    soundTimer = millis();

    switch (currentEffect) {

      case SoundEffect::CLICK:
        if (soundStep == 0) {
          tone(PIN_DISP_BUZZER, 1400); // Corpul de la apăsarea mecanică ("Tup")
          stepDuration = 120;           // 15 ms - suficient pentru timing-ul din update()
          soundStep++;
        } else if (soundStep == 1) {
          tone(PIN_DISP_BUZZER, 2400); // Declick-ul/rebound-ul mecanic ("cik")
          stepDuration = 180;           // 22 ms pentru un final ferm
          soundStep++;
        } else {
          stopBuzzer();
          currentEffect = SoundEffect::NONE;
        }
        break;
      case SoundEffect::POP:
        if (soundStep == 0) {
          tone(PIN_DISP_BUZZER, 800);
          stepDuration = 25;
          soundStep++;
        } else if (soundStep == 1) {
          tone(PIN_DISP_BUZZER, 1900);
          stepDuration = 50;
          soundStep++;
        } else {
          stopBuzzer();
          currentEffect = SoundEffect::NONE;
        }
        break;

      case SoundEffect::ALERT:
        if (soundStep == 0) {
          tone(PIN_DISP_BUZZER, 1000);
          stepDuration = 100;
          soundStep++;
        } else if (soundStep == 1) {
          stopBuzzer(); // Pauză între bătăi
          stepDuration = 30;
          soundStep++;
        } else if (soundStep == 2) {
          tone(PIN_DISP_BUZZER, 600);
          stepDuration = 200;
          soundStep++;
        } else {
          stopBuzzer();
          currentEffect = SoundEffect::NONE;
        }
        break;

      case SoundEffect::BOINGNCOIN:
        if (soundStep < 18) {
          int freq = 300 + (soundStep * 50);
          tone(PIN_DISP_BUZZER, freq);
          stepDuration = 10;
          soundStep++;
        } else {
          stopBuzzer();
          currentEffect = SoundEffect::NONE; 
          playSound(SoundEffect::COIN);
        }
        break;

      case SoundEffect::WIN:
        if (soundStep == 0) {
          tone(PIN_DISP_BUZZER, 523);
          stepDuration = 110;
          soundStep++;
        } else if (soundStep == 1) {
          tone(PIN_DISP_BUZZER, 659);
          stepDuration = 110;
          soundStep++;
        } else if (soundStep == 2) {
          tone(PIN_DISP_BUZZER, 784);
          stepDuration = 110;
          soundStep++;
        } else if (soundStep == 3) {
          tone(PIN_DISP_BUZZER, 1047);
          stepDuration = 300;
          soundStep++;
        } else {
          stopBuzzer();
          currentEffect = SoundEffect::NONE;
        }
        break;

      case SoundEffect::BEEP:
        if (soundStep == 0) {
          tone(PIN_DISP_BUZZER, 2000);
          stepDuration = 40;
          soundStep++;
        } else {
          stopBuzzer();
          currentEffect = SoundEffect::NONE;
        }
        break;

      case SoundEffect::CONFIRMED:
        if (soundStep == 0) {
          tone(PIN_DISP_BUZZER, 440);
          stepDuration = 70;
          soundStep++;
        } else if (soundStep == 1) {
          tone(PIN_DISP_BUZZER, 880);
          stepDuration = 140;
          soundStep++;
        } else {
          stopBuzzer();
          currentEffect = SoundEffect::NONE;
        }
        break;

      case SoundEffect::COIN:
        if (soundStep == 0) {
          tone(PIN_DISP_BUZZER, 988);
          stepDuration = 80;
          soundStep++;
        } else if (soundStep == 1) {
          tone(PIN_DISP_BUZZER, 1319);
          stepDuration = 250;
          soundStep++;
        } else {
          stopBuzzer();
          currentEffect = SoundEffect::NONE;
        }
        break;

      default:
        stopBuzzer();
        currentEffect = SoundEffect::NONE;
        break;
    }
  }
}

void DisplaySystem::stopBuzzer() {
  noTone(PIN_DISP_BUZZER);
  digitalWrite(PIN_DISP_BUZZER, HIGH);
}

void DisplaySystem::updateAlarm(SystemState &state) {
  if(state.compostLowHumidityAlarm == true){
    state.isFARM_SYS_OPERATIONAL = false;
    if(millis() - alarmTimer >= HUMIDITY_ALARM_INTERVAL_DURRATION){
      printLCD(0, 0, F("ALARMA COMPOST!"), 16);
      printLCD(0, 1, F("UMID. SCAZUTA"), 16);
      alarmTimer = millis();
      if(isAlarmStateOn){
        // We turn alarm off
        isAlarmStateOn = false;
        changeLED(LED_State::OFF);

        
      }
      else{
        // We turn alarm on
        isAlarmStateOn = true;
        changeLED(LED_State::RED);
        //playSound(SoundEffect::ALERT);

      }
    }
  }
  else{
    calibrateLED_toPlant();
  }
}

