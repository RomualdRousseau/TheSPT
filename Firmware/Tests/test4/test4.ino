#include <CT8Z.h>

const int stickRudder = A3;
const int trimRudder1 = 20;
const int trimRudder2 = 21;

const int stickThrottle = A2;
const int trimThrottle1 = 22;
const int trimThrottle2 = 23;

const int stickAileron = A0;
const int trimAileron1 = 19;
const int trimAileron2 = 18;

const int stickElevator = A1;
const int trimElevator1 = 14;
const int trimElevator2 = 15;

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

void setup()
{
	CT8Z.begin();	
}

void loop()
{
  values[0] = analogRead(stickThrottle);
  trims[0] += digitalRead(trimThrottle1) - digitalRead(trimThrottle2);
  
  values[1] = analogRead(stickRudder);
  trims[1] += digitalRead(trimRudder1) - digitalRead(trimRudder2);

  values[2] = analogRead(stickElevator);
  trims[2] += digitalRead(trimElevator1) - digitalRead(trimElevator2);

  values[3] = analogRead(stickAileron);
  trims[3] += digitalRead(trimAileron1) - digitalRead(trimAileron2);

  CT8Z.analogWrite(values, mixer);
  
  delay(1);
}


