#include <LiquidCrystal_I2C.h>

#define BUTTON_NOT_PRESSED      0
#define BUTTON_PRESSED          (1 << 0)
#define BUTTON_LONG_PRESSED     (1 << 1)

#define EVENT_HOME_PRESSED        0
#define EVENT_HOME_LONG_PRESSED   1
#define EVENT_UP_PRESSED          2
#define EVENT_DOWN_PRESSED        3

class Console
{
public:
  Console():
    m_lcd(0x27, 16, 2)
  {
  }

  void init()
  {
    Serial.println("Initializing console ... ");
    
    m_lcd.init(); 
    m_lcd.backlight();
    m_lcd.print("Initializing ...");

    pinMode(HAL_CONSOLE_HOME_PIN, INPUT);
  }

  void clear_and_sleep()
  {
    m_lcd.clear();
    m_lcd.noBacklight();
  }

  void clear_and_wakeup()
  {
    m_lcd.clear();
    m_lcd.backlight();
  }
  
  void ready(char* name, int level)
  {
    m_lcd.setCursor(0, 0);
    m_lcd.print(name);
    m_lcd.setCursor(12, 0);
    m_lcd.print(level);
    m_lcd.print("%  ");
    m_lcd.setCursor(0, 1);
    m_lcd.print("Ready");
  }

  void alert_low_battery(char* name, int level)
  {
    m_lcd.setCursor(0, 0);
    m_lcd.print(name);
    m_lcd.setCursor(12, 0);
    m_lcd.print(level);
    m_lcd.print("%  ");
    m_lcd.setCursor(0, 1);
    m_lcd.print("Low Bat.");
  }

  void print_title(const char* s)
  {
    m_lcd.clear();
    m_lcd.print(s);
  }

  void print_command(const char* s)
  {
    m_lcd.setCursor(0, 1);
    m_lcd.print(s);
  }

  void print_error(int error_code)
  {
    m_lcd.setCursor(0, 1);
    m_lcd.print("Error: 0x");
    m_lcd.print(error_code, HEX);
    delay(1000);
  }

  void print_percent(const int value)
  {
    m_lcd.setCursor(0, 1);
    m_lcd.print(value);
    m_lcd.print("%  ");
  }

  int wait_next_event()
  {
    while(true)
    {
      switch(press_button(HAL_CONSOLE_HOME_PIN))
      {
      case BUTTON_PRESSED:
        return EVENT_HOME_PRESSED;
      case BUTTON_LONG_PRESSED:
        return EVENT_HOME_LONG_PRESSED;
      }
      if(press_button(HAL_CONSOLE_DOWN_PIN) == BUTTON_PRESSED)
      {
        return EVENT_DOWN_PRESSED;
      }
      if(press_button(HAL_CONSOLE_UP_PIN) == BUTTON_PRESSED)
      {
        return EVENT_UP_PRESSED;
      }
      delay(100);
    }
  }

  int press_button(int pinNumber)
  {
    if(digitalRead(pinNumber) == HIGH)
    {
      int t = 0;
      while(digitalRead(pinNumber) == HIGH) { t++; delay(1); }
      return (t > 1000) ? BUTTON_LONG_PRESSED : BUTTON_PRESSED;
    }
    else
    {
      return BUTTON_NOT_PRESSED;
    }
  }

  void read_string(const char* label, char* value, int len)
  {
    for(int i = 0; i <= len; i++)
    {
      value[i] = 0;
    }
    for(int i = 0; i < len; i++)
    {
      m_lcd.cursor();
      while(press_button(HAL_CONSOLE_HOME_PIN) == BUTTON_NOT_PRESSED)
      {
        m_lcd.setCursor(0, 1);
        m_lcd.print(label);
        m_lcd.print(value);
        m_lcd.setCursor(i, 1);

        readIncDec(&value[i], 'A', 'Z');
      }
      m_lcd.noCursor();
    }
  }
  
  void read_value(const char* label, int* value, int toLow, int toHigh)
  {
    while(press_button(HAL_CONSOLE_HOME_PIN) == BUTTON_NOT_PRESSED)
    {
      m_lcd.setCursor(0, 1);
      m_lcd.print(label);
      m_lcd.print(*value, DEC);
      m_lcd.print("    ");
      
      readIncDec(value, toLow, toHigh);
    }
  }

  void read_percent(const char* label, int* value, int toLow, int toHigh)
  {
    while(press_button(HAL_CONSOLE_HOME_PIN) == BUTTON_NOT_PRESSED)
    {
      m_lcd.setCursor(0, 1);
      m_lcd.print(label);
      m_lcd.print(*value, DEC);
      m_lcd.print("%  ");
      
      readIncDec(value, toLow, toHigh);
    }
  }

  void read_YesNo(const char* label, int* value, int toLow, int toHigh)
  {
    while(press_button(HAL_CONSOLE_HOME_PIN) == BUTTON_NOT_PRESSED)
    {
      m_lcd.setCursor(0, 1);
      m_lcd.print(label);
      m_lcd.print(*value == 1 ? "Yes" : "No ");
      
      readIncDec(value, toLow, toHigh);
    }
  }

private:
  LiquidCrystal_I2C m_lcd;

  void readIncDec(char* value, int toLow, int toHigh)
  {
    while(digitalRead(HAL_CONSOLE_DOWN_PIN) == LOW && digitalRead(HAL_CONSOLE_UP_PIN) == LOW && digitalRead(HAL_CONSOLE_HOME_PIN) == LOW) delay(1);
    
    if(digitalRead(HAL_CONSOLE_DOWN_PIN) == HIGH)
    {
      *value = (*value == 0 || *value >= toHigh) ? toLow : *value + 1;
    }
    if(digitalRead(HAL_CONSOLE_UP_PIN) == HIGH)
    {
      *value = (*value == 0 || *value <= toLow) ? toHigh : *value - 1;
    }
    delay(200);
  }

  void readIncDec(int* value, int toLow, int toHigh)
  {
    while(digitalRead(HAL_CONSOLE_DOWN_PIN) == LOW && digitalRead(HAL_CONSOLE_UP_PIN) == LOW && digitalRead(HAL_CONSOLE_HOME_PIN) == LOW) delay(1);
    
    if(digitalRead(HAL_CONSOLE_DOWN_PIN) == HIGH)
    {
      *value = (*value >= toHigh) ? toLow : *value + 1;
    }
    if(digitalRead(HAL_CONSOLE_UP_PIN) == HIGH)
    {
      *value = (*value <= toLow) ? toHigh : *value - 1;
    }
    delay(200);
  }
};

