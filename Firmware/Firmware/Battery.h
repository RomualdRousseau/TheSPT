class Battery
{
  public:
    int level;

    void init();

    void measure_level();

  private:
    int m_batteryLevel;
};

void Battery::init()
{
#ifdef DEBUG  
  Serial.println("Initializing battery watchdog ... ");
#endif

  m_batteryLevel = analogRead(HAL_BATTERY_PIN);
  level = constrain(map(m_batteryLevel, HAL_BATTERY_LOWCOEF, HAL_BATTERY_HIGHCOEF, 0, 100), 0, 100);
}

void Battery::measure_level()
{
  m_batteryLevel = min(m_batteryLevel, analogRead(HAL_BATTERY_PIN));
  level = constrain(map(m_batteryLevel, HAL_BATTERY_LOWCOEF, HAL_BATTERY_HIGHCOEF, 0, 100), 0, 100);
}
