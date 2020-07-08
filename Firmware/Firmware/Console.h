#include <LiquidCrystal_I2C.h>

#define BUTTON_NOT_PRESSED      0
#define BUTTON_PRESSED          (1 << 0)
#define BUTTON_LONG_PRESSED     (1 << 1)

#define EVENT_HOME_PRESSED        0
#define EVENT_HOME_LONG_PRESSED   1
#define EVENT_UP_PRESSED          2
#define EVENT_DOWN_PRESSED        3

#define STATE_CALIBRATE_MENU      0
#define STATE_OPEN_MENU           1
#define STATE_RENAME_MENU         2
#define STATE_MIX_MENU            3
#define STATE_RESET_MENU          4
#define STATE_RESET_ALL_MENU      41
#define STATE_RESET_TRIMS_MENU    42
#define STATE_RESET_ONE_TRIM_MENU 43
#define STATE_RESET_BACK_MENU     49
#define STATE_SAVE_MENU           5
#define STATE_QUIT_MENU           6
#define STATE_QUIT                100

void menu_reset();
void menu_calibrate_menu();
void menu_open_menu();
void menu_rename_menu();
void menu_mix_menu();
void menu_reset_menu();
void menu_reset_all_menu();
void menu_reset_trims_menu();
void menu_reset_one_trim_menu();
void menu_reset_back_menu();
void menu_save_menu();
void menu_quit_menu();

class Console
{
  public:
    int menu_state;
    int menu_dirty;

    Console():
      m_lcd(0x27, 16, 2)
    {
    }

    void init();

    void start_menu();

    void clear_and_sleep();

    void clear_and_wakeup();

    void ready(char* name, int level);

    void alert_low_battery(char* name, int level);

    void print_title(const char* s);

    void print_command(const char* s);

    void print_error(int error_code);

    void print_percent(const int value);

    int wait_next_event();

    int press_button(int pinNumber);

    void read_string(const char* label, char* value, int len);

    void read_value(const char* label, int* value, int toLow, int toHigh);

    void read_percent(const char* label, int* value);

    void read_YesNo(const char* label, int* value, int toLow, int toHigh);

  private:
    LiquidCrystal_I2C m_lcd;

    void readIncDec(char* value, int toLow, int toHigh);

    void readIncDec(int* value, int toLow, int toHigh);
};

void Console::init()
{
#ifdef DEBUG  
  Serial.println("Initializing console ... ");
#endif

  m_lcd.init();
  m_lcd.backlight();
  m_lcd.print("Initializing ...");

  pinMode(HAL_CONSOLE_HOME_PIN, INPUT);
}

void Console::start_menu()
{
  clear_and_wakeup();

  menu_state = STATE_OPEN_MENU;
  menu_dirty = false;
  
  while (menu_state != STATE_QUIT)
  {
    switch (menu_state)
    {
      case STATE_CALIBRATE_MENU:
        menu_calibrate_menu();
        break;
      case STATE_OPEN_MENU:
        menu_open_menu();
        break;
      case STATE_RENAME_MENU:
        menu_rename_menu();
        break;
      case STATE_MIX_MENU:
        menu_mix_menu();
        break;
      case STATE_RESET_MENU:
        menu_reset_menu();
        break;
      case STATE_RESET_ALL_MENU:
        menu_reset_all_menu();
        break;
      case STATE_RESET_TRIMS_MENU:
        menu_reset_trims_menu();
        break;
      case STATE_RESET_ONE_TRIM_MENU:
        menu_reset_one_trim_menu();
        break;
      case STATE_RESET_BACK_MENU:
        menu_reset_back_menu();
        break;
      case STATE_SAVE_MENU:
        menu_save_menu();
        break;
      case STATE_QUIT_MENU:
        menu_quit_menu();
        break;
    }
  }

  clear_and_sleep();
}

void Console::clear_and_sleep()
{
  m_lcd.clear();
  m_lcd.noBacklight();
}

void Console::clear_and_wakeup()
{
  m_lcd.clear();
  m_lcd.backlight();
}

void Console::ready(char* name, int level)
{
  m_lcd.setCursor(0, 0);
  m_lcd.print(name);
  m_lcd.setCursor(12, 0);
  m_lcd.print(level);
  m_lcd.print("%  ");
  m_lcd.setCursor(0, 1);
  m_lcd.print("Ready");
}

void Console::alert_low_battery(char* name, int level)
{
  m_lcd.setCursor(0, 0);
  m_lcd.print(name);
  m_lcd.setCursor(12, 0);
  m_lcd.print(level);
  m_lcd.print("%  ");
  m_lcd.setCursor(0, 1);
  m_lcd.print("Low Bat.");
}

void Console::print_title(const char* s)
{
  m_lcd.clear();
  m_lcd.print(s);
}

void Console::print_command(const char* s)
{
  m_lcd.setCursor(0, 1);
  m_lcd.print(s);
}

void Console::print_error(int error_code)
{
  m_lcd.setCursor(0, 1);
  m_lcd.print("Error: 0x");
  m_lcd.print(error_code, HEX);
  delay(1000);
}

void Console::print_percent(const int value)
{
  m_lcd.setCursor(0, 1);
  m_lcd.print(value);
  m_lcd.print("%  ");
}

int Console::wait_next_event()
{
  while (true)
  {
    switch (press_button(HAL_CONSOLE_HOME_PIN))
    {
      case BUTTON_PRESSED:
        return EVENT_HOME_PRESSED;
      case BUTTON_LONG_PRESSED:
        return EVENT_HOME_LONG_PRESSED;
    }
    if (press_button(HAL_CONSOLE_DOWN_PIN) == BUTTON_PRESSED)
    {
      return EVENT_DOWN_PRESSED;
    }
    if (press_button(HAL_CONSOLE_UP_PIN) == BUTTON_PRESSED)
    {
      return EVENT_UP_PRESSED;
    }
    delay(100);
  }
}

int Console::press_button(int pinNumber)
{
  if (digitalRead(pinNumber) == HIGH)
  {
    int t = 0;
    while (digitalRead(pinNumber) == HIGH) {
      t++;
      delay(1);
    }
    return (t > 1000) ? BUTTON_LONG_PRESSED : BUTTON_PRESSED;
  }
  else
  {
    return BUTTON_NOT_PRESSED;
  }
}

void Console::read_string(const char* label, char* value, int len)
{
  for (int i = 0; i <= len; i++)
  {
    value[i] = 0;
  }
  for (int i = 0; i < len; i++)
  {
    m_lcd.cursor();
    while (press_button(HAL_CONSOLE_HOME_PIN) == BUTTON_NOT_PRESSED)
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

void Console::read_value(const char* label, int* value, int toLow, int toHigh)
{
  while (press_button(HAL_CONSOLE_HOME_PIN) == BUTTON_NOT_PRESSED)
  {
    m_lcd.setCursor(0, 1);
    m_lcd.print(label);
    m_lcd.print(*value, DEC);
    m_lcd.print("    ");

    readIncDec(value, toLow, toHigh);
  }
}

void Console::read_percent(const char* label, int* value)
{
  while (press_button(HAL_CONSOLE_HOME_PIN) == BUTTON_NOT_PRESSED)
  {
    m_lcd.setCursor(0, 1);
    m_lcd.print(label);
    m_lcd.print(*value, DEC);
    m_lcd.print("%  ");

    readIncDec(value, 0, 100);
  }
}

void Console::read_YesNo(const char* label, int* value, int toLow, int toHigh)
{
  while (press_button(HAL_CONSOLE_HOME_PIN) == BUTTON_NOT_PRESSED)
  {
    m_lcd.setCursor(0, 1);
    m_lcd.print(label);
    m_lcd.print(*value == 1 ? "Yes" : "No ");

    readIncDec(value, toLow, toHigh);
  }
}

void Console::readIncDec(char* value, int toLow, int toHigh)
{
  while (digitalRead(HAL_CONSOLE_DOWN_PIN) == LOW && digitalRead(HAL_CONSOLE_UP_PIN) == LOW && digitalRead(HAL_CONSOLE_HOME_PIN) == LOW) delay(1);

  if (digitalRead(HAL_CONSOLE_DOWN_PIN) == HIGH)
  {
    *value = (*value == 0 || *value >= toHigh) ? toLow : *value + 1;
  }
  if (digitalRead(HAL_CONSOLE_UP_PIN) == HIGH)
  {
    *value = (*value == 0 || *value <= toLow) ? toHigh : *value - 1;
  }
  delay(200);
}

void Console::readIncDec(int* value, int toLow, int toHigh)
{
  while (digitalRead(HAL_CONSOLE_DOWN_PIN) == LOW && digitalRead(HAL_CONSOLE_UP_PIN) == LOW && digitalRead(HAL_CONSOLE_HOME_PIN) == LOW) delay(1);

  if (digitalRead(HAL_CONSOLE_DOWN_PIN) == HIGH)
  {
    *value = (*value >= toHigh) ? toLow : *value + 1;
  }
  if (digitalRead(HAL_CONSOLE_UP_PIN) == HIGH)
  {
    *value = (*value <= toLow) ? toHigh : *value - 1;
  }
  delay(200);
}
