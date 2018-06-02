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
  Serial.println("Initializing battery watchdog ... ");
  
  m_batteryLevel = analogRead(HAL_BATTERY_PIN);
  level = constrain(map(m_batteryLevel, 320, 512, 0, 100), 0, 100);
}

void Battery::measure_level()
{
  m_batteryLevel = min(m_batteryLevel, analogRead(HAL_BATTERY_PIN));
  level = constrain(map(m_batteryLevel, 320, 512, 0, 100), 0, 100);
}


