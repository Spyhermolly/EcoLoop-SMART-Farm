#include "SystemState.h"

#include "SortSystem.h"
#include "MixSystem.h"
#include "FarmSystem.h"
#include "DisplaySystem.h"

SystemState state;
SortSystem sortSystem;
MixSystem mixSystem;
FarmSystem farmSystem;
DisplaySystem displaySystem;

void setup() {
  Serial.begin(9600);

  sortSystem.init();
  mixSystem.init();
  farmSystem.init();
  displaySystem.init();
}

void loop() {
  sortSystem.update(state);
  mixSystem.update(state);
  farmSystem.update(state);
  displaySystem.update(state);
}
