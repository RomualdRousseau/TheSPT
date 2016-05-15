#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include <LiquidCrystal_I2C.h>
#include <ADS1115.h>
#include <CT8Z.h>

#define DEBUG 1

#define POWER_MIN               10.0
#define POWER_MAX               11.56
#define POWER_RATIO1            (POWER_MAX / (3.85 * 1023 / 5))
#define POWER_RATIO2            (100.0 / (POWER_MAX - POWER_MIN))

#define BUTTON_NOT_PRESSED      0
#define BUTTON_PRESSED          (1 << 0)
#define BUTTON_LONG_PRESSED     (1 << 1)

#define STATE_IDLE              0
#define STATE_MODEL_MENU        1
#define STATE_SELECT_MODEL      2
#define STATE_CALIBRATE_MENU    3
#define STATE_CALIBRATE         4
#define STATE_CONFIGURE_MENU    5
#define STATE_SELECT_CHANNEL    6
#define STATE_CONFIGURE_SCALE   7
#define STATE_CONFIGURE_MIX     8
#define STATE_RESET_MENU        9
#define STATE_RESET             10
#define STATE_RETURN_IDLE       100

#define BUTTON_HOME             6
#define BUTTON_DOWN             2
#define BUTTON_UP               3

const char models[5][8] =
{
  "Model0",
  "Model1",
  "Model2",
  "Model3",
  "Model4"
};

class TransmiterChannel
{
friend class Transmiter;

public:
  int   scaleMin;
  int   scaleMax;
  int   mixChannel;
  float mixValue;
  
  void init(int axisValue, int trimValue, bool keepCalibration = false)
  {
    m_x = axisValue;
    if(!keepCalibration)
    {
      m_xMin = 0;
      m_xMax = 1023;
    }

    m_trimInit = trimValue;
    m_trimCurr = 0;
    
    scaleMin = 0;
    scaleMax = 1023;
    mixChannel = -1;
    mixValue = 0;
  }
  
  void beginCalibrate()
  {
    m_xMin = 512;
    m_xMax = 512;
  }

  void calibrate(int axisValue)
  {
    m_xMin = min(m_xMin, axisValue);
    m_xMax = max(m_xMax, axisValue);
  }

  void update(int axisValue, int trimValue)
  {
    if(trimValue > m_trimInit * 1.05)
    {
      m_trimCurr+=10;
    }
    else if(trimValue < m_trimInit * 0.95)
    {
      m_trimCurr-=10;
    } 
    m_x = map(axisValue, m_xMin, m_xMax, scaleMin, scaleMax) + m_trimCurr;
  }

  void readFromFile(File& file)
  {
    m_trimCurr = (file.read() << 8) + file.read();
    m_xMin = (file.read() << 8) + file.read();
    m_xMax = (file.read() << 8) + file.read();
    scaleMin = (file.read() << 8) + file.read();
    scaleMax = (file.read() << 8) + file.read();
    mixChannel = (file.read() << 8) + file.read();
    mixValue = ((file.read() << 8) + file.read()) / 100.0;
#ifdef DEBUG
    Serial.print(m_trimCurr);
    Serial.print("\t");
    Serial.print(m_xMin);
    Serial.print("\t");
    Serial.print(m_xMax);
    Serial.print("\t");
    Serial.print(scaleMin);
    Serial.print("\t");
    Serial.print(scaleMax);
    Serial.print("\t");
    Serial.print(mixChannel);
    Serial.print("\t");
    Serial.print(mixValue);
    Serial.println();
#endif
  }

  void writeToFile(File& file)
  {
#ifdef DEBUG
    Serial.print(m_trimCurr);
    Serial.print("\t");
    Serial.print(m_xMin);
    Serial.print("\t");
    Serial.print(m_xMax);
    Serial.print("\t");
    Serial.print(scaleMin);
    Serial.print("\t");
    Serial.print(scaleMax);
    Serial.print("\t");
    Serial.print(mixChannel);
    Serial.print("\t");
    Serial.print(mixValue);
    Serial.println();
#endif
    file.write(m_trimCurr >> 8); file.write(m_trimCurr);
    file.write(m_xMin >> 8); file.write(m_xMin);
    file.write(m_xMax >> 8); file.write(m_xMax);
    file.write(scaleMin >> 8); file.write(scaleMin);
    file.write(scaleMax >> 8); file.write(scaleMax);
    file.write(mixChannel >> 8); file.write(mixChannel);
    file.write((int)(mixValue * 100) >> 8); file.write((int)(mixValue * 100));
  }

private:
  int m_x;
  int m_xMin;
  int m_xMax;
  int m_trimCurr;
  int m_trimInit;
};

class Transmiter
{
public:
  TransmiterChannel channels[7];
  
  Transmiter():
    m_ads0(ADS1115_ADDRESS1),
    m_ads1(ADS1115_ADDRESS0)
  {
    
  }
  
  void init(File& file)
  {
    m_ads0.begin();
    m_ads1.begin();
    resetAll();
    loadAll(file);
    m_driver.begin();
  }

  void resetAll(bool keepCalibration = false)
  {
    channels[0].init(m_ads0.analogRead(ADS1115_A0) >> 4, m_ads0.analogRead(ADS1115_A2) >> 4, keepCalibration);
    channels[1].init(m_ads0.analogRead(ADS1115_A1) >> 4, m_ads0.analogRead(ADS1115_A3) >> 4, keepCalibration);
    channels[2].init(m_ads1.analogRead(ADS1115_A0) >> 4, m_ads1.analogRead(ADS1115_A2) >> 4, keepCalibration);
    channels[3].init(m_ads1.analogRead(ADS1115_A1) >> 4, m_ads1.analogRead(ADS1115_A3) >> 4, keepCalibration);
    channels[4].init(512, 0);
    channels[5].init(0, 0);
    channels[6].init(512, 0);
  }

  void loadAll(File& file)
  {
    if(file)
    {
      file.seek(0);
      for(int i = 0; i < 7; i++)
      {
        channels[i].readFromFile(file);
      }
      file.close();
    }
  }

  void saveAll(File& file)
  {
    if(file)
    {
      file.seek(0);
      for(int i = 0; i < 7; i++)
      {
        channels[i].writeToFile(file);
      }
      file.close();
    }
  }

  void beginCalibrate()
  {
    channels[0].beginCalibrate();
    channels[1].beginCalibrate();
    channels[2].beginCalibrate();
    channels[3].beginCalibrate();
  }

  void calibrate()
  {
    if(m_ads0.ready())
    {
      channels[0].calibrate(m_ads0.analogRead(ADS1115_A0) >> 4);
      channels[1].calibrate(m_ads0.analogRead(ADS1115_A1) >> 4);
    }
    if(m_ads1.ready())
    {
      channels[2].calibrate(m_ads1.analogRead(ADS1115_A0) >> 4);
      channels[3].calibrate(m_ads1.analogRead(ADS1115_A1) >> 4);
    }
  }

  void transmitAll()
  {
    if(m_ads0.ready())
    {
      channels[0].update(m_ads0.analogRead(ADS1115_A0) >> 4, m_ads0.analogRead(ADS1115_A2) >> 4);
      channels[1].update(m_ads0.analogRead(ADS1115_A1) >> 4, m_ads0.analogRead(ADS1115_A3) >> 4);
    }
    if(m_ads1.ready())
    {
      channels[2].update(m_ads1.analogRead(ADS1115_A0) >> 4, m_ads1.analogRead(ADS1115_A2) >> 4);
      channels[3].update(m_ads1.analogRead(ADS1115_A1) >> 4, m_ads1.analogRead(ADS1115_A3) >> 4);
    }
    channels[4].update(digitalRead(2) == HIGH ? 0 : (digitalRead(3) == HIGH  ? 1023 : 512), 0);
    channels[5].update(digitalRead(4) == HIGH  ? 1023 : 0, 0);
    channels[6].update(digitalRead(7) == HIGH  ? 0 : (digitalRead(8) == HIGH  ? 1023 : 512), 0);
    
    for(int i = 0; i < 7; i++)
    {
      TransmiterChannel* channel = &channels[i];
      if((channel->mixChannel == -1) || (channels[i].mixValue == 0))
      {
        m_driver.analogPulse(i, channel->m_x);
      }
      else if(channels[i].mixValue > 0)
      {
        m_driver.analogPulseMix(i, channels[channel->mixChannel].m_x, channel->m_x, channel->mixValue);
      }
      else
      {
        m_driver.analogPulseMix(i, 1023 - channels[channel->mixChannel].m_x, channel->m_x, -channel->mixValue);
      }
    }
  }

private:
  CT8Z      m_driver;
  ADS1115   m_ads0;
  ADS1115   m_ads1;
};

class Console
{
public:
  Console():
    m_lcd(0x27,16,2)
  {
  }

  void init()
  {
    m_lcd.init(); 
    m_lcd.backlight();
    m_lcd.print("Initializing ...");
    delay(1000);
  }

  void wakeup()
  {
    m_lcd.backlight();
  }
  
  void ready(const char* s)
  {
    delay(1000);
    m_lcd.noBacklight();
    m_lcd.clear();
    m_lcd.print(s);
    m_lcd.setCursor(0, 1);
    m_lcd.print("Ready");
  }

  void printTitle(const char* s)
  {
    m_lcd.clear();
    m_lcd.print(s);
  }

  void printCommand(const char* s)
  {
    m_lcd.setCursor(0, 1);
    m_lcd.print(s);
  }

  void printError(int error_code)
  {
    m_lcd.setCursor(0, 1);
    m_lcd.print("Error: 0x");
    m_lcd.print(error_code, HEX);
    delay(1000);
  }

  void printPercent(const int value)
  {
    m_lcd.setCursor(0, 1);
    m_lcd.print(value);
    m_lcd.print("%   ");
  }

  int pressButton(int pinNumber)
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
  
  void readValue(const char* label, int* value, int toLow, int toHigh)
  {
    while(pressButton(BUTTON_HOME) == LOW)
    {
      m_lcd.setCursor(0, 1);
      m_lcd.print(label);
      m_lcd.print(*value, DEC);
      m_lcd.print("    ");
      
      readIncDec(value, toLow, toHigh);
    }
  }

  void readPercent(const char* label, int* value, int toLow, int toHigh)
  {
    while(pressButton(BUTTON_HOME) == LOW)
    {
      m_lcd.setCursor(0, 1);
      m_lcd.print(label);
      m_lcd.print(*value, DEC);
      m_lcd.print("%   ");
      
      readIncDec(value, toLow, toHigh);
    }
  }

  void readYesNo(const char* label, int* value, int toLow, int toHigh)
  {
    while(pressButton(BUTTON_HOME) == LOW)
    {
      m_lcd.setCursor(0, 1);
      m_lcd.print(label);
      m_lcd.print(*value == 1 ? "Yes" : "No ");
      
      readIncDec(value, toLow, toHigh);
    }
  }

private:
  LiquidCrystal_I2C m_lcd;

  void readIncDec(int* value, int toLow, int toHigh)
  {
    if(digitalRead(BUTTON_UP) == HIGH)
    {
      while(digitalRead(BUTTON_UP) == HIGH) delay(1);
      *value = (*value >= toHigh) ? toLow : *value + 1;
    }
    if(digitalRead(BUTTON_DOWN) == HIGH)
    {
      while(digitalRead(BUTTON_DOWN) == HIGH) delay(1);
      *value = (*value <= toLow) ? toHigh : *value - 1;
    }
  
    delay(100);
  }
};

Console console;
Transmiter transmiter;

int state = STATE_IDLE;
int lastSelectedModel = 0;
int lastSelectedChannel = 0;

void idle()
{
  switch(console.pressButton(BUTTON_HOME))
  {
  case BUTTON_PRESSED:
    showPower();
    console.ready(models[lastSelectedModel]);
    break;
  case BUTTON_LONG_PRESSED:
    console.wakeup();
    state = STATE_MODEL_MENU;
    break;
  }
  transmiter.transmitAll();
}

void modelMenu()
{
  console.printTitle("Setup Menu"); 
  console.printCommand("Model"); 
  while(true)
  {
    switch(console.pressButton(BUTTON_HOME))
    {
    case BUTTON_PRESSED:
      state = STATE_SELECT_MODEL;
      return;
    case BUTTON_LONG_PRESSED:
      state = STATE_RETURN_IDLE;
      return;
    }
    if(console.pressButton(BUTTON_DOWN) == BUTTON_PRESSED)
    {
      state = STATE_RESET_MENU;
      return;
    }
    if(console.pressButton(BUTTON_UP) == BUTTON_PRESSED)
    {
      state = STATE_CALIBRATE_MENU;
      return;
    }
    delay(100);
  }
}

void selectModel()
{
  console.printTitle("Model");
  console.readValue("Select: ", &lastSelectedModel, 0, 4);

  File fileModel = SD.open(models[lastSelectedModel]);
  transmiter.loadAll(fileModel);
  
  state = STATE_MODEL_MENU;
}

void calibrateMenu()
{
  console.printTitle("Setup Menu"); 
  console.printCommand("Calibrate"); 
  while(true)
  {
    switch(console.pressButton(BUTTON_HOME))
    {
    case BUTTON_PRESSED:
      state = STATE_CALIBRATE;
      return;
    case BUTTON_LONG_PRESSED:
      state = STATE_RETURN_IDLE;
      return;
    }
    if(console.pressButton(BUTTON_DOWN) == BUTTON_PRESSED)
    {
      state = STATE_MODEL_MENU;
      return;
    }
    if(console.pressButton(BUTTON_UP) == BUTTON_PRESSED)
    {
      state = STATE_CONFIGURE_MENU;
      return;
    }
    delay(100);
  }
}

void calibrate()
{
  console.printTitle("Calibrate");
  
  console.printCommand("Center sticks");
  transmiter.beginCalibrate();
  while(console.pressButton(BUTTON_HOME) != BUTTON_PRESSED) delay(100);

  console.printCommand("Move sticks  ");
  while(console.pressButton(BUTTON_HOME) != BUTTON_PRESSED)
  {
    transmiter.calibrate();
  }

  state = STATE_CALIBRATE_MENU;
}

void configureMenu()
{
  console.printTitle("Setup Menu"); 
  console.printCommand("Configure");
  while(true)
  {
    switch(console.pressButton(BUTTON_HOME))
    {
    case BUTTON_PRESSED:
      state = STATE_SELECT_CHANNEL;
      return;
    case BUTTON_LONG_PRESSED:
      state = STATE_RETURN_IDLE;
      return;
    }
    if(console.pressButton(BUTTON_DOWN) == BUTTON_PRESSED)
    {
      state = STATE_CALIBRATE_MENU;
      return;
    }
    if(console.pressButton(BUTTON_UP) == BUTTON_PRESSED)
    {
      state = STATE_RESET_MENU;
      return;
    }
    delay(100);
  }
}

void selectChannel()
{
  console.printTitle("Configure");
  console.readValue("Select: ", &lastSelectedChannel, 0, 6);
  state = STATE_CONFIGURE_SCALE;
}

void configureScale()
{
  int v = 0;
  
  console.printTitle("Scale");
  v = (float(transmiter.channels[lastSelectedChannel].scaleMax - transmiter.channels[lastSelectedChannel].scaleMin) / 1023.0) * 100;
  console.readPercent("Value: ", &v, -100, 100);
  if(v >= 0)
  {
    transmiter.channels[lastSelectedChannel].scaleMin = 0;
    transmiter.channels[lastSelectedChannel].scaleMax = (float(v) / 100.0) * 1023;
  }
  else
  {
    transmiter.channels[lastSelectedChannel].scaleMin = 1023;
    transmiter.channels[lastSelectedChannel].scaleMax = (1.0 + float(v) / 100.0) * 1023;
  }
  //console.readValue("Min: ", &transmiter.channels[lastSelectedChannel].scaleMin, 0, 1023);
  //console.readValue("Max: ", &transmiter.channels[lastSelectedChannel].scaleMax, 0, 1023);
  state = STATE_CONFIGURE_MIX;
}

void configureMix()
{
  console.printTitle("Mix");
  console.readValue("Select: ", &transmiter.channels[lastSelectedChannel].mixChannel, 0, 6);
  if(transmiter.channels[lastSelectedChannel].mixChannel >= 0)
  {
    int v = transmiter.channels[lastSelectedChannel].mixValue * 100;
    console.readPercent("Value: ", &v, -100, 100);
    transmiter.channels[lastSelectedChannel].mixValue = v / 100.0;
  }
  state = STATE_CONFIGURE_MENU;
}

void resetMenu()
{
  console.printTitle("Setup Menu"); 
  console.printCommand("Reset");
  while(true)
  {
    switch(console.pressButton(BUTTON_HOME))
    {
    case BUTTON_PRESSED:
      state = STATE_RESET;
      return;
    case BUTTON_LONG_PRESSED:
      state = STATE_RETURN_IDLE;
      return;
    }
    if(console.pressButton(BUTTON_DOWN) == BUTTON_PRESSED)
    {
      state = STATE_CONFIGURE_MENU;
      return;
    }
    if(console.pressButton(BUTTON_UP) == BUTTON_PRESSED)
    {
      state = STATE_MODEL_MENU;
      return;
    }
    delay(100);
  }
}

void reset()
{
  int v = 0;
  
  console.printTitle("Reset");
  console.readYesNo("Sure? ", &v, 0, 1);
  if(v == 1)
  {
    transmiter.resetAll(true);
  }
  state = STATE_RESET_MENU;
}

void save()
{
  console.printTitle("Saving ...");
  
  File file = SD.open("conf", FILE_WRITE);
  if(!file)
  {
    console.printError(0x02);
    return;
  }
  file.seek(0);
  file.write(lastSelectedModel);
  file.close();
#ifdef DEBUG
    Serial.print(lastSelectedModel);
    Serial.println();
#endif
  
  file = SD.open(models[lastSelectedModel], FILE_WRITE);
  if (!file)
  {
    console.printError(0x03);
    return;
  }
  transmiter.saveAll(file);
  
  console.ready(models[lastSelectedModel]);
  state = STATE_IDLE;
}

void showPower()
{
  console.printTitle("Battery");
  console.printPercent(min(100, int((analogRead(A3) * POWER_RATIO1 - POWER_MIN) * POWER_RATIO2)));
}

void setup()
{
  console.init();
  
#ifdef DEBUG
  Serial.begin(19200);
#endif

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  
  if(!SD.begin(9))
  {
    console.printError(0x01);
    return;
  }

  File file = SD.open("conf");
  if(file)
  {
    lastSelectedModel = file.read();
    file.close();
  }
#ifdef DEBUG
    Serial.print(lastSelectedModel);
    Serial.println();
#endif

  file = SD.open(models[lastSelectedModel]);
  transmiter.init(file);

  showPower();
  console.ready(models[lastSelectedModel]);
}

void loop()
{
  switch(state)
  {
  case STATE_IDLE:
    idle();
    break;
  case STATE_MODEL_MENU:
    modelMenu();
    break;
  case STATE_SELECT_MODEL:
    selectModel();
    break;
  case STATE_CALIBRATE_MENU:
    calibrateMenu();
    break;
  case STATE_CALIBRATE:
    calibrate();
    break;
  case STATE_CONFIGURE_MENU:
    configureMenu();
    break;
  case STATE_SELECT_CHANNEL:
    selectChannel();
    break;
  case STATE_CONFIGURE_SCALE:
    configureScale();
    break;
  case STATE_CONFIGURE_MIX:
    configureMix();
    break;
  case STATE_RESET_MENU:
    resetMenu();
    break;
  case STATE_RESET:
    reset();
    break;
  case STATE_RETURN_IDLE:
    save();
    break;
  }
}
