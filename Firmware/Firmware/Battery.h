#include "fmath.h"

class Battery
{
  public:
    int level;

    void init();

    void measure_level();

  private:
    unsigned int m_batteryLevel;
    unsigned int m_voltage;
};

void Battery::init()
{
#ifdef DEBUG  
  Serial.println("Initializing battery watchdog ... ");
#endif

  m_batteryLevel = analogRead(HAL_BATTERY_PIN);
  m_voltage = m_batteryLevel * 35 / 256; // 10 * 5 / (1024 * (100 / 280)) where (100 / 280) is the internal circuit diviser
  level = constrain(map(m_batteryLevel, HAL_BATTERY_LOWCOEF, HAL_BATTERY_HIGHCOEF, 0, 100), 0, 100);
}

void Battery::measure_level()
{
  m_batteryLevel = flowf(m_batteryLevel, analogRead(HAL_BATTERY_PIN));
  m_voltage = m_batteryLevel * 35 / 256; // 10 * 5 / (1024 * (100 / 280)) where (100 / 280) is the internal circuit diviser
  level = constrain(map(m_batteryLevel, HAL_BATTERY_LOWCOEF, HAL_BATTERY_HIGHCOEF, 0, 100), 0, 100);
}
