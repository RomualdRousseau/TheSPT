#include <LiquidCrystal_I2C.h>
#include <CT8Z.h>

#define TASK_100HZ  10
#define TASK_1HZ    1000

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

const int switchHome = 0;

const int switchTop = 1;

const int triggerRight1 = 3;
const int triggerRight2 = 2;

const int triggerLeft1 = 11;
const int triggerLeft2 = 10;

LiquidCrystal_I2C lcd(0x27,16,2);

int mixer[PPM_MAX_CHANNELS][PPM_MAX_CHANNELS] = 
{
  {-256,    0,    0,  -64,    0,    0,    0,    0},
  {   0, +256,    0,    0,    0,    0,    0,    0},
  {   0,    0, +256,    0,    0,    0,    0,    0},
  {   0,    0,    0, +256,    0,    0,    0,    0},
  {   0,    0,    0,    0, +256,    0,    0,    0},
  {   0,    0,    0,    0,    0, +256,    0,    0},
  {   0,    0,    0,    0,    0,    0, +256,    0},
  {   0,    0,    0,    0,    0,    0,    0, +256}
};

int values[PPM_MAX_CHANNELS] =
{
  512, 512, 512, 512, 512, 512, 512, 512
};

int trims[PPM_MAX_CHANNELS] =
{
  0, 0, 0, 0, 0, 0, 0, 0
};

int tickCount = 0;
int batteryLevel = 0;

void normalOperation()
{
  trims[0] += digitalRead(trimThrottle1) - digitalRead(trimThrottle2);
  values[0] = (values[0] * 3 + analogRead(stickThrottle)) / 4 + trims[0];
  
  trims[1] += digitalRead(trimRudder1) - digitalRead(trimRudder2);
  values[1] = (values[1] * 3 + analogRead(stickRudder)) / 4 + trims[1];
  
  trims[1] += digitalRead(trimElevator1) - digitalRead(trimElevator2);
  values[2] = (values[2] * 3 + analogRead(stickElevator)) / 4 + trims[2];
  
  trims[3] += digitalRead(trimAileron1) - digitalRead(trimAileron2);
  values[3] = (values[3] * 3 + analogRead(stickAileron)) / 4 + trims[3];

  values[4] = digitalRead(switchTop) * 1023;
  values[5] = (digitalRead(triggerRight1) - digitalRead(triggerRight2)) * 512 + 512;
  values[6] = (digitalRead(triggerLeft1) - digitalRead(triggerLeft2)) * 512 + 512;
  
  CT8Z.analogWrite(values, mixer);
}

void updateOSDHome()
{
  batteryLevel = min(batteryLevel, analogRead(batteryPin));
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
}

void setup()
{
  lcd.init(); 
  lcd.backlight();
  lcd.print("Initializing ...");
  
	CT8Z.begin();

  pinMode(trimRudder1, INPUT);
  pinMode(trimRudder2, INPUT);
  pinMode(trimThrottle1, INPUT);
  pinMode(trimThrottle2, INPUT);
  pinMode(trimAileron1, INPUT);
  pinMode(trimAileron2, INPUT);
  pinMode(trimElevator1, INPUT);
  pinMode(trimElevator2, INPUT);
  pinMode(switchHome, INPUT);
  pinMode(switchTop, INPUT);
  pinMode(triggerRight1, INPUT);
  pinMode(triggerRight2, INPUT);
  pinMode(triggerLeft1, INPUT);
  pinMode(triggerLeft2, INPUT);

  batteryLevel = analogRead(batteryPin);
  
  values[0] = analogRead(stickThrottle);
  values[1] = analogRead(stickRudder);
  values[2] = analogRead(stickElevator);
  values[3] = analogRead(stickAileron);
  values[4] = digitalRead(switchTop) * 1023;

  delay(1000);
  lcd.clear();
  lcd.noBacklight();
}

void loop()
{
  if(tickCount % TASK_100HZ == 0)
  {
    normalOperation();  
  }
  if(tickCount % TASK_1HZ == 0)
  {
    updateOSDHome();
    tickCount= 0;
  }
  delay(1);
  tickCount++;
}


