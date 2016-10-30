#include <LiquidCrystal_I2C.h>
#include <CT8Z.h>

const int batteryPin = A7;

const int stickAileron = A0;
const int trimAileron1 = 19;
const int trimAileron2 = 18;

const int stickElevator = A1;
const int trimElevator1 = 14;
const int trimElevator2 = 15;

const int stickRudder = A3;
const int trimRudder1 = 20;
const int trimRudder2 = 21;

const int stickThrottle = A2;
const int trimThrottle1 = 22;
const int trimThrottle2 = 23;

LiquidCrystal_I2C lcd(0x27,16,2);

int mixer[9][8] = 
{
  {+256,    0,    0,  -64,    0, +256,    0,    0},
  {   0, +256,    0,    0,    0,    0,    0,    0},
  {   0,    0, +256,    0,    0,    0,    0,    0},
  {   0,    0,    0, +256,    0,    0,    0,    0},
  {   0,    0,    0,    0, +256,    0,    0,    0},
  {   0,    0,    0,    0,    0, +256,    0,    0},
  {   0,    0,    0,    0,    0,    0, +256,    0},
  {   0,    0,    0,    0,    0,    0,    0, +256},
  {   0,    0,    0,    0,    0,    0,    0,    0}
};

int values[9] =
{
  512, 512, 512, 512, 512, 512, 512, 512, 1023
};

int* trims = (int*) &mixer[8];

int batteryLevel = 0;

void setup()
{
  lcd.init(); 
  lcd.backlight();
  lcd.print("Initializing ...");
  
	CT8Z.begin();

  batteryLevel = analogRead(batteryPin);
  
  values[0] = analogRead(stickThrottle);
  values[1] = analogRead(stickRudder);
  values[2] = analogRead(stickElevator);
  values[3] = analogRead(stickAileron);

  delay(1000);
  lcd.clear();
  lcd.noBacklight();
}

void loop()
{
  batteryLevel = (batteryLevel * 3 + analogRead(batteryPin)) / 4;
  int level = constrain(map(batteryLevel, 320, 512, 0, 100), 0, 100);

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
  
  values[0] = (values[0] * 3 + analogRead(stickThrottle)) / 4;
  trims[0] += digitalRead(trimThrottle1) - digitalRead(trimThrottle2);
  
  values[1] = (values[1] * 3 + analogRead(stickRudder)) / 4;
  trims[1] += digitalRead(trimRudder1) - digitalRead(trimRudder2);

  values[2] = (values[2] * 3 + analogRead(stickElevator)) / 4;
  trims[2] += digitalRead(trimElevator1) - digitalRead(trimElevator2);

  values[3] = (values[3] * 3 + analogRead(stickAileron)) / 4;
  trims[3] += digitalRead(trimAileron1) - digitalRead(trimAileron2);

  CT8Z.analogWrite(values, mixer);
  
  delay(1);
}


