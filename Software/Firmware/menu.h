#define STATE_CALIBRATE_MENU    0
#define STATE_OPEN_MENU         1
#define STATE_RENAME_MENU       2
#define STATE_MIX_MENU          3
#define STATE_RESET_MENU        4
#define STATE_SAVE_MENU         5
#define STATE_QUIT_MENU         6
#define STATE_QUIT              100

int current_model;
int state;

void calibrate_action()
{
  HAL.console.print_title("Calibrate"); 
  HAL.console.print_command("Center sticks ..."); 
  HAL.transmiter.calibrate_center();
  
  HAL.console.print_title("Calibrate"); 
  HAL.console.print_command("Move sticks ..."); 
  HAL.transmiter.calibrate_minmax();
  
  HAL.storage.save_current_model(HAL.current_model);
  HAL.storage.save_calibration((int*) HAL.transmiter.valuesMinMax);
}

void calibrate_menu()
{
  HAL.console.print_title("Setup Menu"); 
  HAL.console.print_command("Calibrate");

  switch(HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      calibrate_action();
      return;
    case EVENT_UP_PRESSED:
      state = STATE_QUIT_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      state = STATE_OPEN_MENU;
      return;
  }
}

void open_action()
{
  int model = HAL.current_model;
  HAL.console.print_title("Open"); 
  HAL.console.read_value("Model? ", &model, 0, 100);

  HAL.current_model = model;
  HAL.storage.save_current_model(HAL.current_model);

  HAL.storage.load_name(HAL.current_model, HAL.current_model_name);
  HAL.storage.load_mixer(HAL.current_model, (int*) HAL.transmiter.mixer);
  HAL.storage.load_trims(HAL.current_model, (int*) HAL.transmiter.trims);
}

void open_menu()
{
  HAL.console.print_title("Setup Menu"); 
  HAL.console.print_command("Open");

  switch(HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      open_action();
      return;
    case EVENT_UP_PRESSED:
      state = STATE_CALIBRATE_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      state = STATE_RENAME_MENU;
      return;
  }
}

void rename_action()
{
  HAL.console.print_title("Rename"); 
  HAL.console.read_string("", HAL.current_model_name, 8);
}

void rename_menu()
{
  HAL.console.print_title("Setup Menu"); 
  HAL.console.print_command("Rename");

  switch(HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      rename_action();
      return;
    case EVENT_UP_PRESSED:
      state = STATE_OPEN_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      state = STATE_MIX_MENU;
      return;
  }
}

void mix_action()
{
  int in = 0;
  HAL.console.print_title("Mix"); 
  HAL.console.read_value("Input? ", &in, 0, PPM_MAX_CHANNELS);

  int out = 0;
  HAL.console.print_title("Mix"); 
  HAL.console.read_value("Output? ", &out, 0, PPM_MAX_CHANNELS);

  int val = map(HAL.transmiter.mixer[in][out], -256, 256, -100, 100);
  HAL.console.print_title("Mix"); 
  HAL.console.read_percent("Value? ", &val, -100, 100);

  HAL.transmiter.mixer[in][out] = map(val, -100, 100, -256, 256);
}

void mix_menu()
{
  HAL.console.print_title("Setup Menu"); 
  HAL.console.print_command("Mix");

  switch(HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      mix_action();
      return;
    case EVENT_UP_PRESSED:
      state = STATE_RENAME_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      state = STATE_RESET_MENU;
      return;
  }
}

void reset_action()
{
  HAL.console.print_title("Reset"); 
  
  int v = 0;
  HAL.console.read_YesNo("Confirm? ", &v, 0, 1);
  if(v == 1)
  {
    HAL.transmiter.reset();
  }
}

void reset_menu()
{
  HAL.console.print_title("Setup Menu"); 
  HAL.console.print_command("Reset");

  switch(HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      reset_action();
      return;
    case EVENT_UP_PRESSED:
      state = STATE_MIX_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      state = STATE_SAVE_MENU;
      return;
  }
}

void save_action()
{
  HAL.console.print_title("Save"); 
  
  int v = 0;
  HAL.console.read_YesNo("Confirm? ", &v, 0, 1);
  if(v == 1)
  {
    HAL.storage.save_name(HAL.current_model, HAL.current_model_name);
    HAL.storage.save_mixer(HAL.current_model, (int*) HAL.transmiter.mixer);
    HAL.storage.save_trims(HAL.current_model, (int*) HAL.transmiter.trims);
  }
}

void save_menu()
{
  HAL.console.print_title("Setup Menu"); 
  HAL.console.print_command("Save");

  switch(HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      save_action();
      return;
    case EVENT_UP_PRESSED:
      state = STATE_RESET_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      state = STATE_QUIT_MENU;
      return;
  }
}

void quit_menu()
{
  HAL.console.print_title("Setup Menu"); 
  HAL.console.print_command("Quit");

  switch(HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      state = STATE_QUIT;
      return;
    case EVENT_UP_PRESSED:
      state = STATE_SAVE_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      state = STATE_CALIBRATE_MENU;
      return;
  }
}

void start_setup_menu()
{
  HAL.console.clear_and_wakeup();

  state = STATE_CALIBRATE_MENU;

  while(state != STATE_QUIT)
  {
    switch(state)
    {
      case STATE_CALIBRATE_MENU:
        calibrate_menu();
        break;
      case STATE_OPEN_MENU:
        open_menu();
        break;
      case STATE_RENAME_MENU:
        rename_menu();
        break;
      case STATE_MIX_MENU:
        mix_menu();
        break;
      case STATE_RESET_MENU:
        reset_menu();
        break;
      case STATE_SAVE_MENU:
        save_menu();
        break;
      case STATE_QUIT_MENU:
        quit_menu();
        break;
    }
  }

  HAL.console.clear_and_sleep();
}

