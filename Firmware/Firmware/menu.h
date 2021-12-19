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

void menu_calibrate_menu()
{
  HAL.console.print_title("Setup Menu");
  HAL.console.print_command("Calibrate");

  switch (HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      calibrate_action();
      return;
    case EVENT_UP_PRESSED:
      HAL.console.menu_state = STATE_QUIT_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      HAL.console.menu_state = STATE_OPEN_MENU;
      return;
  }
}

void open_action()
{
  int model = HAL.current_model;
  HAL.console.print_title("Open");
  HAL.console.read_value("Model? ", &model, 0, HAL_STORAGE_MAX_MODELS);

  HAL.current_model = model;
  HAL.storage.save_current_model(HAL.current_model);

  HAL.storage.load_name(HAL.current_model, HAL.current_model_name);
  HAL.storage.load_mixer(HAL.current_model, (int*) HAL.transmiter.mixer);
  HAL.storage.load_trims(HAL.current_model, (int*) HAL.transmiter.trims);
  HAL.storage.load_funcs(HAL.current_model, (int*) HAL.transmiter.funcs);
}

void menu_open_menu()
{
  HAL.console.print_title("Setup Menu");
  HAL.console.print_command("Open");

  switch (HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      open_action();
      return;
    case EVENT_UP_PRESSED:
      HAL.console.menu_state = STATE_CALIBRATE_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      HAL.console.menu_state = STATE_RENAME_MENU;
      return;
  }
}

void rename_action()
{
  HAL.console.print_title("Rename");
  HAL.console.read_string("", HAL.current_model_name, 8);

  HAL.console.menu_dirty = true;
}

void menu_rename_menu()
{
  HAL.console.print_title("Setup Menu");
  HAL.console.print_command("Rename");

  switch (HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      rename_action();
      return;
    case EVENT_UP_PRESSED:
      HAL.console.menu_state = STATE_OPEN_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      HAL.console.menu_state = STATE_MIX_MENU;
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

  int val = HAL.transmiter.mixer[in][out];
  HAL.console.print_title("Mix");
  HAL.console.read_value("Value? ", &val, -256, 256);

  int func = HAL.transmiter.funcs[in];
  HAL.console.print_title("Mix");
  HAL.console.read_value("Func? ", &func, 0, 2);

  HAL.transmiter.mixer[in][out] = val;
  HAL.transmiter.funcs[in] = func;

  HAL.console.menu_dirty = true;
}

void menu_mix_menu()
{
  HAL.console.print_title("Setup Menu");
  HAL.console.print_command("Mix");

  switch (HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      mix_action();
      return;
    case EVENT_UP_PRESSED:
      HAL.console.menu_state = STATE_RENAME_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      HAL.console.menu_state = STATE_MAIDEN_MENU;
      return;
  }
}

void maiden_action()
{
  int v = 0;
  HAL.console.print_title("Maiden");
  HAL.console.read_YesNo("Confirm? ", &v, 0, 1);
  
  HAL.maiden_flag = (v == 1);
}

void menu_maiden_menu()
{
  HAL.console.print_title("Setup Menu");
  HAL.console.print_command("Maiden");

  switch (HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      maiden_action();
      return;
    case EVENT_UP_PRESSED:
      HAL.console.menu_state = STATE_MIX_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      HAL.console.menu_state = STATE_RESET_MENU;
      return;
  }
}

void menu_reset_back_menu()
{
  HAL.console.print_title("Reset");
  HAL.console.print_command("Back");

  switch (HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      HAL.console.menu_state = STATE_RESET_MENU;
      return;
    case EVENT_UP_PRESSED:
      HAL.console.menu_state = STATE_RESET_ONE_TRIM_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      HAL.console.menu_state = STATE_RESET_ALL_MENU;
      return;
  }
}

void reset_one_trim_action()
{
  int in = 0;
  HAL.console.print_title("Trim");
  HAL.console.read_value("Input? ", &in, 0, PPM_MAX_CHANNELS);

  HAL.transmiter.trims[in] = 0;
  
  HAL.console.menu_dirty = true;
}

void menu_reset_one_trim_menu()
{
  HAL.console.print_title("Reset");
  HAL.console.print_command("One Trim");

  switch (HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      reset_one_trim_action();
      return;
    case EVENT_UP_PRESSED:
      HAL.console.menu_state = STATE_RESET_TRIMS_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      HAL.console.menu_state = STATE_RESET_BACK_MENU;
      return;
  }
}

void reset_trims_action()
{
  int v = 0;
  HAL.console.read_YesNo("Confirm? ", &v, 0, 1);
  if (v == 1)
  {
    HAL.transmiter.reset_trims();
    
    HAL.console.menu_dirty = true;
  }
}

void menu_reset_trims_menu()
{
  HAL.console.print_title("Reset");
  HAL.console.print_command("All Trims");

  switch (HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      reset_trims_action();
      return;
    case EVENT_UP_PRESSED:
      HAL.console.menu_state = STATE_RESET_ALL_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      HAL.console.menu_state = STATE_RESET_ONE_TRIM_MENU;
      return;
  }
}

void reset_all_action()
{
  int v = 0;
  HAL.console.read_YesNo("Confirm? ", &v, 0, 1);
  if (v == 1)
  {
    HAL.transmiter.reset();
    
    HAL.console.menu_dirty = true;
  }
}

void menu_reset_all_menu()
{
  HAL.console.print_title("Reset");
  HAL.console.print_command("All");

  switch (HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      reset_all_action();
      return;
    case EVENT_UP_PRESSED:
      HAL.console.menu_state = STATE_RESET_BACK_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      HAL.console.menu_state = STATE_RESET_TRIMS_MENU;
      return;
  }
}

void menu_reset_menu()
{
  HAL.console.print_title("Setup Menu");
  HAL.console.print_command("Reset");

  switch (HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      HAL.console.menu_state = STATE_RESET_ALL_MENU;
      return;
    case EVENT_UP_PRESSED:
      HAL.console.menu_state = STATE_MAIDEN_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      HAL.console.menu_state = STATE_SAVE_MENU;
      return;
  }
}

void save_action()
{
  HAL.console.print_title("Save");

  int v = 0;
  HAL.console.read_YesNo("Confirm? ", &v, 0, 1);
  if (v == 1)
  {
    HAL.storage.save_name(HAL.current_model, HAL.current_model_name);
    HAL.storage.save_mixer(HAL.current_model, (int*) HAL.transmiter.mixer);
    HAL.storage.save_trims(HAL.current_model, (int*) HAL.transmiter.trims);
    HAL.storage.save_funcs(HAL.current_model, (int*) HAL.transmiter.funcs);
    HAL.console.menu_dirty = false;
  }
}

void menu_save_menu()
{
  HAL.console.print_title("Setup Menu");
  HAL.console.print_command("Save");

  switch (HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      save_action();
      return;
    case EVENT_UP_PRESSED:
      HAL.console.menu_state = STATE_RESET_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      HAL.console.menu_state = STATE_QUIT_MENU;
      return;
  }
}

void menu_quit_menu()
{
  HAL.console.print_title("Setup Menu");
  HAL.console.print_command("Quit");

  switch (HAL.console.wait_next_event())
  {
    case EVENT_HOME_PRESSED:
    case EVENT_HOME_LONG_PRESSED:
      if (HAL.console.menu_dirty)
      {
        save_action();
      }
      HAL.console.menu_state = STATE_QUIT;
      return;
    case EVENT_UP_PRESSED:
      HAL.console.menu_state = STATE_SAVE_MENU;
      return;
    case EVENT_DOWN_PRESSED:
      HAL.console.menu_state = STATE_CALIBRATE_MENU;
      return;
  }
}
