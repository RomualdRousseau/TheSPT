#include <Wire.h>
#include <WiiChuck.h>

WiiChuck wiichuck;

void setup()
{
	Serial.begin(9600);
	wiichuck.begin();
}

void loop()
{
	if(wiichuck.ready())
	{
		Serial.print (wiichuck.JX, DEC);
		Serial.print ("\t");

		Serial.print (wiichuck.JY, DEC);
		Serial.print ("\t");

		Serial.print (wiichuck.AX, DEC);
		Serial.print ("\t");

		Serial.print (wiichuck.AY, DEC);

		Serial.print ("\t");

		Serial.print (wiichuck.AZ, DEC);
		Serial.print ("\t");

		Serial.print (wiichuck.BZ, DEC);
		Serial.print ("\t");

		Serial.print (wiichuck.BC, DEC);
		Serial.print ("\t");

		Serial.print ("\r\n");
	}
	delay(10);
}


