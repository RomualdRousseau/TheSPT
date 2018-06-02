#include <CT8Z.h>

class Transmiter
{
public:
  int mixer[PPM_MAX_CHANNELS][PPM_MAX_CHANNELS] =
  {
    { +256,   0,    0,    0,    0,    0,    0,    0},
    {   0, +256,    0,    0,    0,    0,    0,    0},
    {   0,    0, +256,    0,    0,    0,    0,    0},
    {   0,    0,    0, +256,    0,    0,    0,    0},
    {   0,    0,    0,    0, +256,    0,    0,    0},
    {   0,    0,    0,    0,    0, +256,    0,    0},
    {   0,    0,    0,    0,    0,    0, +256,    0},
    {   0,    0,    0,    0,    0,    0,    0, +256}
  };

  int trims[PPM_MAX_CHANNELS] =
  {
    0, 0, 0, 0, 0, 0, 0, 0
  };

  int valuesMinMax[4][2] = 
  {
    {0, 1023}, {0, 1023}, {0, 1023}, {0, 1023}
  };

  void init();

  void reset();

  void calibrate_center();

  void calibrate_minmax();

  void read_pilot_commands();

  void transmit();

private:
  void read_trim(int i, int pin1, int pin2);

  void read_stick(int i, int pin);

  void read_switch(int i, int pin);

  void read_switch(int i, int pin1, int pin2);

  int m_values[PPM_MAX_CHANNELS];
};

void Transmiter::init()
{
  Serial.println("Initializing transmiter ... ");
    
  CT8Z.begin();

  pinMode(HAL_TRANSMITER_RUDDER_TRIM1, INPUT);
  pinMode(HAL_TRANSMITER_RUDDER_TRIM2, INPUT);
  pinMode(HAL_TRANSMITER_THROTTLE_TRIM1, INPUT);
  pinMode(HAL_TRANSMITER_THROTTLE_TRIM2, INPUT);
  pinMode(HAL_TRANSMITER_AILERON_TRIM1, INPUT);
  pinMode(HAL_TRANSMITER_AILERON_TRIM2, INPUT);
  pinMode(HAL_TRANSMITER_ELEVATOR_TRIM1, INPUT);
  pinMode(HAL_TRANSMITER_ELEVATOR_TRIM2, INPUT);
  pinMode(HAL_TRANSMITER_TRIGGER_TOP_PIN, INPUT);
  pinMode(HAL_TRANSMITER_TRIGGER_RIGHT1_PIN, INPUT);
  pinMode(HAL_TRANSMITER_TRIGGER_RIGHT2_PIN, INPUT);
  pinMode(HAL_TRANSMITER_TRIGGER_LEFT1_PIN, INPUT);
  pinMode(HAL_TRANSMITER_TRIGGER_LEFT2_PIN, INPUT);

  m_values[0] = analogRead(HAL_TRANSMITER_THROTTLE_PIN);
  m_values[1] = analogRead(HAL_TRANSMITER_RUDDER_PIN);
  m_values[2] = analogRead(HAL_TRANSMITER_ELEVATOR_PIN);
  m_values[3] = analogRead(HAL_TRANSMITER_AILERON_PIN);
  m_values[4] = digitalRead(HAL_TRANSMITER_TRIGGER_TOP_PIN) * 1023;
  m_values[5] = (digitalRead(HAL_TRANSMITER_TRIGGER_RIGHT1_PIN) - digitalRead(HAL_TRANSMITER_TRIGGER_RIGHT2_PIN)) * 512 + 512;
  m_values[6] = (digitalRead(HAL_TRANSMITER_TRIGGER_LEFT1_PIN) - digitalRead(HAL_TRANSMITER_TRIGGER_LEFT2_PIN)) * 512 + 512;
}

void Transmiter::reset()
{
  for(int i = 0; i < PPM_MAX_CHANNELS; i++)
  {
    for(int j = 0; j < PPM_MAX_CHANNELS; j++)
    {
      mixer[i][j] = (i == j) ? 256 : 0;
    }

    trims[i] = 0;
  }
}

void Transmiter::calibrate_center() {
  for (int i = 0; i < 2000; i++) {
    valuesMinMax[0][0] = analogRead(HAL_TRANSMITER_THROTTLE_PIN);
    valuesMinMax[1][0] = analogRead(HAL_TRANSMITER_RUDDER_PIN);
    valuesMinMax[2][0] = analogRead(HAL_TRANSMITER_ELEVATOR_PIN);
    valuesMinMax[3][0] = analogRead(HAL_TRANSMITER_AILERON_PIN);

    valuesMinMax[0][1] = analogRead(HAL_TRANSMITER_THROTTLE_PIN);
    valuesMinMax[1][1] = analogRead(HAL_TRANSMITER_RUDDER_PIN);
    valuesMinMax[2][1] = analogRead(HAL_TRANSMITER_ELEVATOR_PIN);
    valuesMinMax[3][1] = analogRead(HAL_TRANSMITER_AILERON_PIN);

    delay(1);
  }
}

void Transmiter::calibrate_minmax() {
  for (int i = 0; i < 2000; i++) {
    valuesMinMax[0][0] = min(valuesMinMax[0][0], analogRead(HAL_TRANSMITER_THROTTLE_PIN));
    valuesMinMax[1][0] = min(valuesMinMax[1][0], analogRead(HAL_TRANSMITER_RUDDER_PIN));
    valuesMinMax[2][0] = min(valuesMinMax[2][0], analogRead(HAL_TRANSMITER_ELEVATOR_PIN));
    valuesMinMax[3][0] = min(valuesMinMax[3][0], analogRead(HAL_TRANSMITER_AILERON_PIN));

    valuesMinMax[0][1] = max(valuesMinMax[0][1], analogRead(HAL_TRANSMITER_THROTTLE_PIN));
    valuesMinMax[1][1] = max(valuesMinMax[1][1], analogRead(HAL_TRANSMITER_RUDDER_PIN));
    valuesMinMax[2][1] = max(valuesMinMax[2][1], analogRead(HAL_TRANSMITER_ELEVATOR_PIN));
    valuesMinMax[3][1] = max(valuesMinMax[3][1], analogRead(HAL_TRANSMITER_AILERON_PIN));

    delay(1);
  }
}

void Transmiter::read_pilot_commands() {
  read_trim(0, HAL_TRANSMITER_THROTTLE_TRIM1, HAL_TRANSMITER_THROTTLE_TRIM2);
  read_stick(0, HAL_TRANSMITER_THROTTLE_PIN);
  read_trim(1, HAL_TRANSMITER_RUDDER_TRIM1, HAL_TRANSMITER_RUDDER_TRIM2);
  read_stick(1, HAL_TRANSMITER_RUDDER_PIN);
  read_trim(2, HAL_TRANSMITER_ELEVATOR_TRIM1, HAL_TRANSMITER_ELEVATOR_TRIM2);
  read_stick(2, HAL_TRANSMITER_ELEVATOR_PIN);
  read_trim(3, HAL_TRANSMITER_AILERON_TRIM1, HAL_TRANSMITER_AILERON_TRIM2);
  read_stick(3, HAL_TRANSMITER_AILERON_PIN);
  read_switch(4, HAL_TRANSMITER_TRIGGER_TOP_PIN);
  read_switch(5, HAL_TRANSMITER_TRIGGER_RIGHT1_PIN, HAL_TRANSMITER_TRIGGER_RIGHT2_PIN);
  read_switch(6, HAL_TRANSMITER_TRIGGER_LEFT1_PIN, HAL_TRANSMITER_TRIGGER_LEFT2_PIN);
}

void Transmiter::transmit() {
  CT8Z.analogWrite(m_values, mixer);
}

void Transmiter::read_trim(int i, int pin1, int pin2)
{
  trims[i] += constrain(digitalRead(pin1) - digitalRead(pin2), -512, 512);
}

void Transmiter::read_stick(int i, int pin)
{
  int new_value = map(analogRead(pin), valuesMinMax[i][0], valuesMinMax[i][1], 0, 1023);
  m_values[i] = (m_values[i] * 3 + new_value) / 4 + trims[i];
}

void Transmiter::read_switch(int i, int pin)
{
  m_values[i] = digitalRead(pin) * 1023;
}

void Transmiter::read_switch(int i, int pin1, int pin2)
{
  m_values[i] = (digitalRead(pin1) - digitalRead(pin2)) * 512 + 512;
}



