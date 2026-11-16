#include "pins_arduino.h"
#include <stdint.h>
#pragma once

#pragma region pins

// Sortare
constexpr uint8_t PIN_SORT_SENSOR_MOISTURE = A0;
constexpr uint8_t PIN_SORT_ULTRASONIC_TRIG = 12;
constexpr uint8_t PIN_SORT_ULTRASONIC_ECHO = 13;
constexpr uint8_t PIN_SORT_SERVOMOTOR      = 9;

// Mix
constexpr uint8_t PIN_MIX_DHT11_HUMIDITY = A1;
constexpr uint8_t PIN_MIX_N20_BLADE      = 11;
constexpr uint8_t PIN_MIX_FAN            = 7;

// Farm
constexpr uint8_t PIN_FARM_SENSOR_SOIL = 8;
constexpr uint8_t PIN_FARM_MQ5_GAS     = A3;
constexpr uint8_t PIN_FARM_PUMP        = 4;

// Farm & Display
constexpr uint8_t PIN_MULTI_LED_RED   = 5;
constexpr uint8_t PIN_MULTI_LED_GREEN = 6;
constexpr uint8_t PIN_MULTI_LED_BLUE  = 2;

// Display
constexpr uint8_t PIN_DISP_LCD_SCL = A5;
constexpr uint8_t PIN_DISP_LCD_SDA = A4;
constexpr uint8_t PIN_DISP_BUZZER  = 3;
constexpr uint8_t PIN_DISP_BUTTON  = 10;
constexpr uint8_t PIN_DISP_X       = A2;

#pragma endregion

#pragma region constants

constexpr unsigned int AVEARAGE_WASTE_MASS =                  20; //g
constexpr unsigned int DRY_WASTE_HUMIDITY =                   100;
constexpr unsigned int WET_WASTE_HUMIDITY =                   700;
constexpr float WASTE_RETENTION =                             0.50f;
constexpr float METAN_PER_DRY_WASTE =                         0.08f;
constexpr float CO2_ECHIVALENT_FOR_METHANE =                  10.25f;
constexpr unsigned int THRESHOLD_WASTE_MOISTURE =             140;
constexpr unsigned int THRESHOLD_COMPOST_HUMIDITY =           50; //50
constexpr unsigned int THRESHOLD_HAND_DISTANCE =              15;
constexpr unsigned int ULTRASONIC_TIMEOUT =                   15000;
constexpr unsigned int WASTE_MOISTURE_SENSOR_DELAY =          2500;
constexpr unsigned int ULTRASONIC_DELAY =                     60; 
constexpr unsigned int SERVO_TURN_DURATION =                  1000;
constexpr unsigned int SERVO_TILT_ANGLE =                     45;
constexpr unsigned int MOTOR_DELAY_TIME =                     60;
constexpr unsigned int MOTOR_FRICTION_COEFICIENT =            20;
constexpr unsigned int MAXIMUM_MOTOR_POWER =                  255;
constexpr unsigned int MIX_INTERVAL_DURATION =                4000;
constexpr float MIX_INTERVAL_RANDOM_COEFICIENT =              0.25;
constexpr unsigned int SOIL_SENSOR_DELAY =                    1000;
constexpr unsigned int THRESHOLD_SOIL_MOISTURE =              784;
constexpr float JOYSTICK_X_DEADZONE_TOLERANCE =               0.5;
constexpr unsigned int JOYSTICK_X_PRACTICAL_CENTER =          540;
constexpr unsigned int JOYSTICK_X_TEORETICAL_CENTER =         512;
constexpr float JOYSTICK_X_MULTIPLY_ERROR =                   1.75;
constexpr unsigned int LCD_REFRESH_DURATION =                 250;
constexpr unsigned int INPUT_TEST_DELAY =                     50;
constexpr unsigned int TOTAL_PLANTS =                         9;
constexpr unsigned int MAXIMUM_PLANT_AMOUNT =                 50;
constexpr unsigned int INPUT_SWITCH_MAX_DURATION =            750;
constexpr unsigned int INPUT_SIWTCH_MIN_DURATION =            250;
constexpr unsigned int INPUT_BUTTON_DELAY =                   500;
constexpr unsigned int SOUND_STATE_VERIFY_DELAY =             100;
constexpr unsigned int HUMIDITY_ALARM_DELAY =                 1000;
constexpr unsigned int HUMIDITY_ALARM_INTERVAL_DURRATION =    500;

#pragma endregion

#pragma region plant

enum PreferredLight : uint8_t{
  LIGHT_RED   = 0,  
  LIGHT_GREEN = 1,
  LIGHT_BLUE  = 2
};

enum Vitamins : char{
  VITAMIN_A = 'A',
  VITAMIN_C = 'C',
  VITAMIN_K = 'K'
};

constexpr float PRET_LITRU_APA =           0.016f;     // lei/litru
constexpr unsigned long ENERGIE_ARBORE =   15000000UL; // watts

constexpr float DRZ_A =  0.80f; //mg
constexpr float DRZ_C = 80.00f; //mg
constexpr float DRZ_K =  0.10f; // mg

enum Minerals : uint8_t{
  MINERAL_FIER =    0,
  MINERAL_CALCIU =  1,
  MINERAL_POTASIU = 2
};

struct PlantType{
  const char* name;
  const char* latinName;
  PreferredLight preferredLight;

  // Mililitrii finali
  float oxigenProduction;
  float carbonDioxideExtraction;

  // Value mg/100g of biomass
  Vitamins vitamin;
  float vitaminMass;

  // Value mg/100g of biomass
  Minerals mineral;
  float mineralMass;

  // Value mg/100g of biomass
  float fibersMass;
  float antioxidantsMass;
  float proteinsMass;

  float finalMass; // Grame 

  float marketPrice; // Lei/100g
  float waterUsage; // Litrii consumati/bucata
  float energyUsage; // Watts consumati/bucata

  unsigned long growthTime; // Secunde

  float portiiDeHrana; // Aproximat/bucata

};

enum class PlantSpecies : uint8_t {
  NONE            = 0,
  PATRUNJEL       = 1,
  BUSUIOC         = 2,
  SALATA_PITICA   = 3,
  CEAPA_VERDE     = 4,
  GERMENI_DIVERSI = 5,
  ROSII_CHERRY    = 6,
  SPANAC          = 7,
  MENTA           = 8
};

constexpr PlantType availablePlants[] = {
  {
    "NONE",
    "",
    LIGHT_GREEN,
    0.0f,
    0.0f,
    VITAMIN_A,
    0.0f,
    MINERAL_FIER,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0,
    0UL, 
    0.0f
  },
  {
    "Patrunjel",
    "P. crispum",
    LIGHT_BLUE,           
    4500.0f,                // oxigenProduction (ml O2)
    5200.0f,                // carbonDioxideExtraction (ml CO2)
    VITAMIN_K,              // vitaminMassAMIN_A, VITAMIN_C, VITAMIN_K)
    1.64f,                  // vitaminMass (mg / 100g)
    MINERAL_FIER,           // mineral (MINERAL_FIER, MINERAL_CALCIU, MINERAL_POTASIU)
    6.2f,                   // mineralMass (mg / 100g)
    3300.0f,                // fibersMass (mg / 100g)
    120.0f,                 // antioxidantsMass (mg / 100g)
    3000.0f,                // proteinsMass (mg / 100g)
    80.0f,                  // finalMass (g / ghiveci)
    4.5f,                   // marketPrice (Lei / 100g)
    2.5f,                   // waterUsage (Litri consumati / ghiveci)
    15.0f,                  // energyUsage (Watts economisiti / ghiveci)
    5184000UL,              // growthTime (secunde = ~60 zile)
    4.0f                    // portiiDeHrana (porții / ghiveci)
  },
  {
    "Busuioc",
    "Ocimum basilicum",
    LIGHT_BLUE,
    3800.0f,                // oxigenProduction (ml O2)
    4400.0f,                // carbonDioxideExtraction (ml CO2)
    VITAMIN_K,              // vitaminMassAMIN_A, VITAMIN_C, VITAMIN_K)
    0.42f,                  // vitaminMass (mg / 100g)
    MINERAL_POTASIU,        // mineral (MINERAL_FIER, MINERAL_CALCIU, MINERAL_POTASIU)
    295.0f,                 // mineralMass (mg / 100g)
    1600.0f,                // fibersMass (mg / 100g)
    250.0f,                 // antioxidantsMass (mg / 100g)
    3150.0f,                // proteinsMass (mg / 100g)
    60.0f,                  // finalMass (g / ghiveci)
    7.0f,                   // marketPrice (Lei / 100g)
    2.0f,                   // waterUsage (Litri consumati / ghiveci)
    12.0f,                  // energyUsage (Watts economisiti / ghiveci)
    4320000UL,              // growthTime (secunde = ~50 zile)
    3.0f                    // portiiDeHrana (porții / ghiveci)
  },
  {
    "Salata pitica",
    "Lactuca sativa",
    LIGHT_GREEN,
    2800.0f,                // oxigenProduction (ml O2)
    3300.0f,                // carbonDioxideExtraction (ml CO2)
    VITAMIN_A,              // vitaminMassAMIN_A, VITAMIN_C, VITAMIN_K)
    0.44f,                  // vitaminMass (mg / 100g)
    MINERAL_POTASIU,        // mineral (MINERAL_FIER, MINERAL_CALCIU, MINERAL_POTASIU)
    194.0f,                 // mineralMass (mg / 100g)
    1100.0f,                // fibersMass (mg / 100g)
    80.0f,                  // antioxidantsMass (mg / 100g)
    1350.0f,                // proteinsMass (mg / 100g)
    120.0f,                 // finalMass (g / ghiveci)
    3.0f,                   // marketPrice (Lei / 100g)
    1.8f,                   // waterUsage (Litri consumati / ghiveci)
    10.0f,                  // energyUsage (Watts economisiti / ghiveci)
    3456000UL,              // growthTime (secunde = ~40 zile)
    2.0f                    // portiiDeHrana (porții / ghiveci)
  },
  {
    "Ceapa verde",
    "Allium cepa",
    LIGHT_BLUE,
    3200.0f,                // oxigenProduction (ml O2)
    3700.0f,                // carbonDioxideExtraction (ml CO2)
    VITAMIN_C,              // vitaminMassAMIN_A, VITAMIN_C, VITAMIN_K)
    18.8f,                  // vitaminMass (mg / 100g)
    MINERAL_POTASIU,        // mineral (MINERAL_FIER, MINERAL_CALCIU, MINERAL_POTASIU)
    276.0f,                 // mineralMass (mg / 100g)
    2600.0f,                // fibersMass (mg / 100g)
    95.0f,                  // antioxidantsMass (mg / 100g)
    1830.0f,                // proteinsMass (mg / 100g)
    70.0f,                  // finalMass (g / ghiveci)
    3.5f,                   // marketPrice (Lei / 100g)
    1.5f,                   // waterUsage (Litri consumati / ghiveci)
    9.0f,                   // energyUsage (Watts economisiti / ghiveci)
    3024000UL,              // growthTime (secunde = ~35 zile)
    3.0f                    // portiiDeHrana (porții / ghiveci)
  },
  {
    "Germeni diversi",
    "Germen",
    LIGHT_BLUE,
    4.0f,                   // oxigenProduction (ml O2 / fir)
    5.0f,                   // carbonDioxideExtraction (ml CO2 / fir)
    VITAMIN_C,              // vitaminMass
    60.0f,                  // vitaminMass (mg / 100g)
    MINERAL_POTASIU,        // mineral
    300.0f,                 // mineralMass (mg / 100g)
    1800.0f,                // fibersMass (mg / 100g)
    350.0f,                 // antioxidantsMass (mg / 100g)
    2300.0f,                // proteinsMass (mg / 100g)
    0.2f,                   // finalMass (0.2 grame / fir)
    12.0f,                  // marketPrice (Lei / 100g)
    0.0025f,                // waterUsage (0.0025 Litri = 2.5 ml apa / fir)
    0.02f,                  // energyUsage (Watts economisiti / fir)
    691200UL,               // growthTime (secunde = 8 zile)
    0.005f                  // portiiDeHrana (porții / fir -> ~200 fire pentru o porție)
  },
  {
    "Rosii cherry",
    "S. lycopersicum",
    LIGHT_RED,
    18000.0f,               // oxigenProduction (ml O2)
    21000.0f,               // carbonDioxideExtraction (ml CO2)
    VITAMIN_C,              // vitaminMassAMIN_A, VITAMIN_C, VITAMIN_K)
    13.7f,                  // vitaminMass (mg / 100g)
    MINERAL_POTASIU,        // mineral (MINERAL_FIER, MINERAL_CALCIU, MINERAL_POTASIU)
    237.0f,                 // mineralMass (mg / 100g)
    1200.0f,                // fibersMass (mg / 100g)
    180.0f,                 // antioxidantsMass (mg / 100g)
    900.0f,                 // proteinsMass (mg / 100g)
    450.0f,                 // finalMass (g / ghiveci)
    2.5f,                   // marketPrice (Lei / 100g)
    12.0f,                  // waterUsage (Litri consumati / ghiveci)
    35.0f,                  // energyUsage (Watts economisiti / ghiveci)
    6912000UL,              // growthTime (secunde = ~80 zile)
    6.0f                    // portiiDeHrana (porții / ghiveci)
  },
  {
    "Spanac",
    "S. oleracea",
    LIGHT_BLUE,
    3500.0f,                // oxigenProduction (ml O2)
    4100.0f,                // carbonDioxideExtraction (ml CO2)
    VITAMIN_K,              // vitaminMassAMIN_A, VITAMIN_C, VITAMIN_K)
    0.48f,                  // vitaminMass (mg / 100g)
    MINERAL_FIER,           // mineral (MINERAL_FIER, MINERAL_CALCIU, MINERAL_POTASIU)
    2.7f,                   // mineralMass (mg / 100g)
    2200.0f,                // fibersMass (mg / 100g)
    110.0f,                 // antioxidantsMass (mg / 100g)
    2900.0f,                // proteinsMass (mg / 100g)
    100.0f,                 // finalMass (g / ghiveci)
    3.5f,                   // marketPrice (Lei / 100g)
    2.0f,                   // waterUsage (Litri consumati / ghiveci)
    11.0f,                  // energyUsage (Watts economisiti / ghiveci)
    3888000UL,              // growthTime (secunde = ~45 zile)
    2.5f                    // portiiDeHrana (porții / ghiveci)
  },
  {
    "Menta",
    "Mentha piperita",
    LIGHT_BLUE,
    4100.0f,                // oxigenProduction (ml O2)
    4800.0f,                // carbonDioxideExtraction (ml CO2)
    VITAMIN_A,              // vitaminMassAMIN_A, VITAMIN_C, VITAMIN_K)
    0.21f,                  // vitaminMass (mg / 100g)
    MINERAL_POTASIU,        // mineral (MINERAL_FIER, MINERAL_CALCIU, MINERAL_POTASIU)
    569.0f,                 // mineralMass (mg / 100g)
    8000.0f,                // fibersMass (mg / 100g)
    280.0f,                 // antioxidantsMass (mg / 100g)
    3750.0f,                // proteinsMass (mg / 100g)
    75.0f,                  // finalMass (g / ghiveci)
    6.0f,                   // marketPrice (Lei / 100g)
    2.2f,                   // waterUsage (Litri consumati / ghiveci)
    13.0f,                  // energyUsage (Watts economisiti / ghiveci)
    4752000UL,              // growthTime (secunde = ~55 zile)
    3.5f                    // portiiDeHrana (porții / ghiveci)
  }
};

#pragma endregion
