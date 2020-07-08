#include <LiquidCrystal_I2C.h>

const int batteryLevel = A7;

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  lcd.init(); 
  lcd.backlight();
}

void loop() {
  int level = analogRead(batteryLevel);
  level = constrain(map(level, 320, 512, 0, 100), 0, 100);
  
  lcd.setCursor(12, 0);
  lcd.print(level);
  lcd.print("%  ");
  lcd.setCursor(0, 1);
  if(level < 10) {
    lcd.print("Low Bat.");
  }
  else {
    lcd.print("Ready");
  }
  delay(1000);
}

