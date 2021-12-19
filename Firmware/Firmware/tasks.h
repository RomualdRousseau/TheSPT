void read_commands(float dt)
{
  HAL.transmiter.read_pilot_commands();
  HAL.transmiter.transmit();
}

void process_script(float dt) {
  HAL.script.run();
}

void read_trims(float dt)
{
  HAL.transmiter.read_pilot_trims();
}

void update_osd(float dt)
{
  HAL.battery.measure_level();

  if (HAL.battery.level > 10) {
    HAL.console.ready(HAL.current_model_name, HAL.battery.level);
  }
  else {
    HAL.console.alert_low_battery(HAL.current_model_name, HAL.battery.level);
  }

  if (HAL.console.press_button(HAL_CONSOLE_HOME_PIN) == BUTTON_LONG_PRESSED) {
    HAL.console.start_menu();
  }
}
