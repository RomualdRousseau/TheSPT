#include "Scheduler.h"
#include "Console.h"
#include "Battery.h"
#include "Transmiter.h"
#include "Storage.h"

class _HAL_t
{
public:
  Scheduler     scheduler;
  Console       console;
  Battery       battery;
  Transmiter    transmiter;
  Storage       storage;
  
  int current_model;

  char current_model_name[9];
  
  void init();
};

static _HAL_t HAL;

void _HAL_t::init()
{
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("SPT v1.0 @ Romuald Rousseau");

  console.init();

  battery.init();

  transmiter.init();

  storage.init();

  storage.load_current_model(&current_model);
  storage.load_calibration((int*) transmiter.valuesMinMax);

  storage.load_name(HAL.current_model, HAL.current_model_name);
  storage.load_mixer(current_model, (int*) transmiter.mixer);
  storage.load_trims(current_model, (int*) transmiter.trims);

#ifdef DEBUG
  Serial.print("Current model: ");
  Serial.println(current_model);
  Serial.print("Current model name: ");
  Serial.println(HAL.current_model_name);
  Serial.println("=== Calibration:");
  for(int i = 0; i < 4; i++)
  {
    for(int j = 0; j < 2; j++)
    {
      Serial.print("\t"); Serial.print(transmiter.valuesMinMax[i][j]);
    }
    Serial.println();
  }
  Serial.println("=== Mixer:");
  for(int i = 0; i < PPM_MAX_CHANNELS; i++)
  {
    for(int j = 0; j < PPM_MAX_CHANNELS; j++)
    {
      Serial.print("\t"); Serial.print(transmiter.mixer[i][j]);
    }
    Serial.println();
  }
  Serial.println("=== Trims:");
  for(int i = 0; i < PPM_MAX_CHANNELS; i++)
  {
    Serial.print("\t"); Serial.print(transmiter.trims[i]);
  }
  Serial.println();
#endif
  
  delay(1000);
}

