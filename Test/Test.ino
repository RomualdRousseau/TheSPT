#include <Wire.h>

#include <CT8Z.h>
#include <ADS1115.h>

CT8Z ct8z;
ADS1115 ads0(ADS1115_ADDRESS0);
ADS1115 ads1(ADS1115_ADDRESS1);

void setup() {
  Serial.begin(19200);

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);

  pinMode(A0, INPUT);
  pinMode(A0, INPUT);
  pinMode(A0, INPUT);
  pinMode(A0, INPUT);

  ads0.begin();
}

void loop() {
  if(ads0.ready())
  {
    Serial.print(ads0.analogRead(ADS1115_A0) >> 4, DEC);
    Serial.print("\t");

    Serial.print(ads0.analogRead(ADS1115_A1) >> 4, DEC);
    Serial.print("\t");

    Serial.print(ads0.analogRead(ADS1115_A2) >> 4, DEC);
    Serial.print("\t");

    Serial.print(ads0.analogRead(ADS1115_A3) >> 4, DEC);
    Serial.print("\t");

    Serial.print(digitalRead(2), DEC);
    Serial.print("\t");

    Serial.print(digitalRead(3), DEC);
    Serial.print("\t");

    Serial.print(digitalRead(4), DEC);
    Serial.print("\t");

    Serial.print(digitalRead(5), DEC);
    Serial.print("\t");

    Serial.print(digitalRead(6), DEC);
    Serial.print("\t");

    Serial.print(digitalRead(7), DEC);
    Serial.print("\t");

    Serial.print(digitalRead(8), DEC);
    Serial.print("\t");

    Serial.print(digitalRead(A0), DEC);
    Serial.print("\t");

    Serial.print(digitalRead(A1), DEC);
    Serial.print("\t");

    Serial.print(digitalRead(A2), DEC);
    Serial.print("\t");

    Serial.print(digitalRead(A3), DEC);
    Serial.print("\t");

    Serial.print("\r\n");
  }
}
