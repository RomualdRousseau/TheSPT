/*
    Welcome to TheSPT v1.2 firmware

    To upload this firmware, select the following board:
    Board: "ATMEGA 1284P"
    Clock: "16Mgz"
    Bootloader: "Optiboot"
*/

#include "hal_config.h"
#include "hal.h"
#include "menu.h"
#include "at.h"

#define TASK_100HZ   DEFINE_TASK_FREQ(100)
#define TASK_20HZ    DEFINE_TASK_FREQ(20)
#define TASK_1HZ     DEFINE_TASK_FREQ(1)

void update_osd(float dt)
{
  HAL.battery.measure_level();

  if (HAL.battery.level > 20) {
    HAL.console.ready(HAL.current_model_name, HAL.battery.level);
  }
  else {
    HAL.console.alert_low_battery(HAL.current_model_name, HAL.battery.level);
  }

  if (HAL.console.press_button(HAL_CONSOLE_HOME_PIN) == BUTTON_LONG_PRESSED) {
    HAL.console.start_menu();
  }
}

void process_trims(float dt)
{
  HAL.transmiter.read_pilot_trims();
}

void process_command_control(float dt)
{
  HAL.transmiter.read_pilot_commands();
  HAL.transmiter.transmit();
}

Task tasks[] = {
  {process_command_control, TASK_100HZ, 0},
  {process_trims, TASK_20HZ, 0},
  {update_osd, TASK_1HZ, 0}
};

void setup()
{
  HAL.init();

  HAL.scheduler.calibrate(&tasks[0], 100);
  HAL.scheduler.register_tasks(tasks, 3);

  HAL.console.clear_and_sleep();
}

void loop()
{
  HAL.scheduler.run_mainloop();
}

void serialEvent()
{
  HAL.serial.process_at_command();
}
