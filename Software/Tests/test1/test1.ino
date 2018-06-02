const int batteryLevel = A7;

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

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

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
}

void loop() {
  Serial.print(analogRead(batteryLevel));
  Serial.print(" ");
  
  Serial.print(analogRead(stickRudder));
  Serial.print(" ");
  Serial.print(digitalRead(trimRudder1));
  Serial.print(" ");
  Serial.print(digitalRead(trimRudder1));
  Serial.print(" ");

  Serial.print(analogRead(stickThrottle));
  Serial.print(" ");
  Serial.print(digitalRead(trimThrottle1));
  Serial.print(" ");
  Serial.print(digitalRead(trimThrottle2));
  Serial.print(" ");

  Serial.print(analogRead(stickAileron));
  Serial.print(" ");
  Serial.print(digitalRead(trimAileron1));
  Serial.print(" ");
  Serial.print(digitalRead(trimAileron2));
  Serial.print(" ");

  Serial.print(analogRead(stickElevator));
  Serial.print(" ");
  Serial.print(digitalRead(trimElevator1));
  Serial.print(" ");
  Serial.print(digitalRead(trimElevator2));
  Serial.print(" ");

  Serial.print(digitalRead(switchHome));
  Serial.print(" ");
  
  Serial.print(digitalRead(switchTop));
  Serial.print(" ");

  Serial.print(digitalRead(triggerRight1));
  Serial.print(" ");
  Serial.print(digitalRead(triggerRight2));
  Serial.print(" ");

  Serial.print(digitalRead(triggerLeft1));
  Serial.print(" ");
  Serial.print(digitalRead(triggerLeft2));
  Serial.print(" ");
  
  Serial.println();
  
  delay(1);
}

