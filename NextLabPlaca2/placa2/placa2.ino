#include <LiquidCrystal.h>

// CONEXIUNE DIRECTĂ PE PINI (Fără I2C!)
// Pinii clasici conectați la LCD: RS, E, D4, D5, D6, D7
// Modifică numerele pinilor în funcție de cum i-ai legat fizic tu pe placă:
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); // Setează dimensiunea LCD-ului tău (ex: 16 coloane, 2 rânduri)
  lcd.print("Astept date...");
  delay(1000);
  lcd.clear();
}

void loop() {
  if (Serial.available() > 0) {
    String mesaj = Serial.readStringUntil('\n');
    
    int primaVirgula = mesaj.indexOf(',');
    int aDouaVirgula = mesaj.indexOf(',', primaVirgula + 1);
    
    if (primaVirgula != -1 && aDouaVirgula != -1) {
      int row = mesaj.substring(0, primaVirgula).toInt();
      int col = mesaj.substring(primaVirgula + 1, aDouaVirgula).toInt();
      String text = mesaj.substring(aDouaVirgula + 1);
      
      lcd.setCursor(col, row);
      lcd.print(text);
    }
  }
}