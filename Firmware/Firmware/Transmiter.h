#include <PPMDevice.h>
#include "fmath.h"

#define COMMAND_THROTTLE  0
#define COMMAND_RUDDER    1
#define COMMAND_ELEVATOR  2
#define COMMAND_AILERON   3
#define COMMAND_AUX1      4
#define COMMAND_AUX2      5
#define COMMAND_AUX3      6

#define FUNC_LOG 1
#define FUNC_EXP 2

#define FUNC_LIN 0
#define FUNC_LOG 1
#define FUNC_EXP 2

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

    int funcs[PPM_MAX_CHANNELS] =
    {
      0, 0, 0, 0, 0, 0, 0, 0
    };

    int valuesMinMax[4][2] =
    {
      {0, 1023}, {0, 1023}, {0, 1023}, {0, 1023}
    };

    int get_value(int i) {
      return m_values[i];
    }
    int get_command(int i) {
      return ppm.read(i);
    }
    
    int get_raw_command(int i) {
      return ppm.readRaw(i); 
    }

    void init();

    void reset();

    void reset_mixer();

    void reset_funcs();

    void reset_trims();

    void snap_trim(int command);

    void calibrate_center();

    void calibrate_minmax();

    void read_pilot_trims();

    void read_pilot_commands();

    void transmit();

  private:
    void read_trim(int i, int pin1, int pin2);

    void read_stick(int i, int pin);

    void read_switch(int i, int pin);

    void read_switch(int i, int pin1, int pin2);

    int m_values[PPM_MAX_CHANNELS];

    int m_dirs[PPM_MAX_CHANNELS] = HAL_TRANSMITER_DIRS;

    PPMDevice ppm;
};

void Transmiter::init()
{
#ifdef DEBUG
  Serial.println("Initializing transmiter ... ");
#endif

  ppm.begin();

  pinMode(HAL_TRANSMITER_RUDDER_TRIM1, INPUT);
  pinMode(HAL_TRANSMITER_RUDDER_TRIM2, INPUT);
  pinMode(HAL_TRANSMITER_THROTTLE_TRIM1, INPUT);
  pinMode(HAL_TRANSMITER_THROTTLE_TRIM2, INPUT);
  pinMode(HAL_TRANSMITER_AILERON_TRIM1, INPUT);
  pinMode(HAL_TRANSMITER_AILERON_TRIM2, INPUT);
  pinMode(HAL_TRANSMITER_ELEVATOR_TRIM1, INPUT);
  pinMode(HAL_TRANSMITER_ELEVATOR_TRIM2, INPUT);
  pinMode(HAL_TRANSMITER_TRIGGER_AUX1A_PIN, INPUT);
  pinMode(HAL_TRANSMITER_TRIGGER_AUX1B_PIN, INPUT);
  pinMode(HAL_TRANSMITER_TRIGGER_AUX2_PIN, INPUT);
  pinMode(HAL_TRANSMITER_TRIGGER_AUX3A_PIN, INPUT);
  pinMode(HAL_TRANSMITER_TRIGGER_AUX3B_PIN, INPUT);

  m_values[COMMAND_THROTTLE]  = analogRead(HAL_TRANSMITER_THROTTLE_PIN);
  m_values[COMMAND_RUDDER]    = analogRead(HAL_TRANSMITER_RUDDER_PIN);
  m_values[COMMAND_ELEVATOR]  = analogRead(HAL_TRANSMITER_ELEVATOR_PIN);
  m_values[COMMAND_AILERON]   = analogRead(HAL_TRANSMITER_AILERON_PIN);
  m_values[COMMAND_AUX1]      = (digitalRead(HAL_TRANSMITER_TRIGGER_AUX1A_PIN) - digitalRead(HAL_TRANSMITER_TRIGGER_AUX1B_PIN)) * 512 + 512;
  m_values[COMMAND_AUX2]      = digitalRead(HAL_TRANSMITER_TRIGGER_AUX2_PIN) * 1023;
  m_values[COMMAND_AUX3]      = (digitalRead(HAL_TRANSMITER_TRIGGER_AUX3A_PIN) - digitalRead(HAL_TRANSMITER_TRIGGER_AUX3B_PIN)) * 512 + 512;
}

void Transmiter::reset() {
  reset_mixer();
  reset_trims();
  reset_funcs();
}

void Transmiter::reset_mixer() {
  for (int i = 0; i < PPM_MAX_CHANNELS; i++) {
    for (int j = 0; j < PPM_MAX_CHANNELS; j++) {
      mixer[i][j] = (i == j) ? 256 : 0;
    }
  }
}

void Transmiter::reset_funcs() {
  for (int i = 0; i < PPM_MAX_CHANNELS; i++) {
    funcs[i] = 0;
  }
}

void Transmiter::reset_trims() {
  for (int i = 0; i < PPM_MAX_CHANNELS; i++) {
    trims[i] = 0;
  }
}

void Transmiter::snap_trim(int command) {
  trims[command] += m_values[command];
}

void Transmiter::calibrate_center() {
  for (int i = 0; i < 100; i++) {
    valuesMinMax[COMMAND_THROTTLE][0] = analogRead(HAL_TRANSMITER_THROTTLE_PIN);
    valuesMinMax[COMMAND_RUDDER][0]   = analogRead(HAL_TRANSMITER_RUDDER_PIN);
    valuesMinMax[COMMAND_ELEVATOR][0] = analogRead(HAL_TRANSMITER_ELEVATOR_PIN);
    valuesMinMax[COMMAND_AILERON][0]  = analogRead(HAL_TRANSMITER_AILERON_PIN);

    valuesMinMax[COMMAND_THROTTLE][1] = analogRead(HAL_TRANSMITER_THROTTLE_PIN);
    valuesMinMax[COMMAND_RUDDER][1]   = analogRead(HAL_TRANSMITER_RUDDER_PIN);
    valuesMinMax[COMMAND_ELEVATOR][1] = analogRead(HAL_TRANSMITER_ELEVATOR_PIN);
    valuesMinMax[COMMAND_AILERON][1]  = analogRead(HAL_TRANSMITER_AILERON_PIN);

    delay(100);
  }
}

void Transmiter::calibrate_minmax() {
  for (int i = 0; i < 200; i++) {
    valuesMinMax[COMMAND_THROTTLE][0] = min(valuesMinMax[COMMAND_THROTTLE][0], analogRead(HAL_TRANSMITER_THROTTLE_PIN));
    valuesMinMax[COMMAND_RUDDER][0]   = min(valuesMinMax[COMMAND_RUDDER][0], analogRead(HAL_TRANSMITER_RUDDER_PIN));
    valuesMinMax[COMMAND_ELEVATOR][0] = min(valuesMinMax[COMMAND_ELEVATOR][0], analogRead(HAL_TRANSMITER_ELEVATOR_PIN));
    valuesMinMax[COMMAND_AILERON][0]  = min(valuesMinMax[COMMAND_AILERON][0], analogRead(HAL_TRANSMITER_AILERON_PIN));

    valuesMinMax[COMMAND_THROTTLE][1] = max(valuesMinMax[COMMAND_THROTTLE][1], analogRead(HAL_TRANSMITER_THROTTLE_PIN));
    valuesMinMax[COMMAND_RUDDER][1]   = max(valuesMinMax[COMMAND_RUDDER][1], analogRead(HAL_TRANSMITER_RUDDER_PIN));
    valuesMinMax[COMMAND_ELEVATOR][1] = max(valuesMinMax[COMMAND_ELEVATOR][1], analogRead(HAL_TRANSMITER_ELEVATOR_PIN));
    valuesMinMax[COMMAND_AILERON][1]  = max(valuesMinMax[COMMAND_AILERON][1], analogRead(HAL_TRANSMITER_AILERON_PIN));

    delay(100);
  }
}

void Transmiter::read_pilot_trims() {
  read_trim(COMMAND_THROTTLE, HAL_TRANSMITER_THROTTLE_TRIM1, HAL_TRANSMITER_THROTTLE_TRIM2);
  read_trim(COMMAND_RUDDER, HAL_TRANSMITER_RUDDER_TRIM1, HAL_TRANSMITER_RUDDER_TRIM2);
  read_trim(COMMAND_ELEVATOR, HAL_TRANSMITER_ELEVATOR_TRIM1, HAL_TRANSMITER_ELEVATOR_TRIM2);
  read_trim(COMMAND_AILERON, HAL_TRANSMITER_AILERON_TRIM1, HAL_TRANSMITER_AILERON_TRIM2);
}

void Transmiter::read_pilot_commands() {
  read_stick(COMMAND_THROTTLE, HAL_TRANSMITER_THROTTLE_PIN);
  read_stick(COMMAND_RUDDER, HAL_TRANSMITER_RUDDER_PIN);
  read_stick(COMMAND_ELEVATOR, HAL_TRANSMITER_ELEVATOR_PIN);
  read_stick(COMMAND_AILERON, HAL_TRANSMITER_AILERON_PIN);
  read_switch(COMMAND_AUX1, HAL_TRANSMITER_TRIGGER_AUX1A_PIN, HAL_TRANSMITER_TRIGGER_AUX1B_PIN);
  read_switch(COMMAND_AUX2, HAL_TRANSMITER_TRIGGER_AUX2_PIN);
  read_switch(COMMAND_AUX3, HAL_TRANSMITER_TRIGGER_AUX3A_PIN, HAL_TRANSMITER_TRIGGER_AUX3B_PIN);
}

void Transmiter::transmit() {
  for (int i  = 0; i < PPM_MAX_CHANNELS; i++) {

    int v = 0;
    for (int j = 0; j < PPM_MAX_CHANNELS; j++) {
      v = fma(m_values[j] + trims[j], mixer[j][i], v);
    }

    ppm.write(i, constrain(v + 512, 0, 1023));
  }
}

void Transmiter::read_trim(int i, int pin1, int pin2) {
  trims[i] += (digitalRead(pin1) - digitalRead(pin2));
  trims[i] = constrain(trims[i], -511, 511);
}

void Transmiter::read_stick(int i, int pin) {
  int new_value = map(analogRead(pin), valuesMinMax[i][0], valuesMinMax[i][1], -511, 511) * m_dirs[i];
  new_value = constrain(new_value, -511, 511);
  if (funcs[i] == FUNC_LIN)
  {
    new_value = new_value;
  }
  else if (funcs[i] == FUNC_LOG)
  {
    new_value = flog(new_value);
  }
  else if (funcs[i] == FUNC_EXP)
  {
    new_value = fexp(new_value);
  }
  m_values[i] = flowf(m_values[i], new_value);
}

void Transmiter::read_switch(int i, int pin)
{
  m_values[i] = (digitalRead(pin) * 2 - 1) * 511 * m_dirs[i];
}

void Transmiter::read_switch(int i, int pin1, int pin2)
{
  m_values[i] = (digitalRead(pin1) - digitalRead(pin2)) * 511 * m_dirs[i];
}
