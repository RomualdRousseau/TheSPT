/*
    Welcome to TheSPT v1.2 firmware

    To upload this firmware, select the following board:
    Board: "ATMEGA 1284P"
    Clock: "16MHz"
    Bootloader: "Optiboot"
*/

#include "hal_config.h"
#include "hal.h"
#include "menu.h"
#include "at.h"
#include "tasks.h"
#include "scripts.h"

#define TASK_200HZ   DEFINE_TASK_FREQ(200)
#define TASK_20HZ   DEFINE_TASK_FREQ(20)
#define TASK_10HZ   DEFINE_TASK_FREQ(10)
#define TASK_1HZ    DEFINE_TASK_FREQ(1)

Task tasks[] = {
  {read_commands, TASK_200HZ},
  {process_script, TASK_20HZ},
  {read_trims, TASK_10HZ},
  {update_osd, TASK_1HZ}
};

Scriptlet scriptlets[] = {
  {snap_trims, COMMAND_AUX1, -511, -384},
#ifdef DEBUG
  {print_tasks, COMMAND_AUX1, 384, 511}
#endif
};

void setup()
{
  HAL.init();

  HAL.scheduler.calibrate(&tasks[0], 100);
  HAL.scheduler.register_tasks(tasks, sizeof(tasks) / sizeof(Task));

  HAL.script.register_scriptlets(scriptlets, sizeof(scriptlets) / sizeof(Scriptlet));
  
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

void Script::run()
{
  for (int i = 0; i < _scriptlet_count; i++) {
    run_one_scriptlet(&_scriptlets[i], HAL.transmiter.get_value(_scriptlets[i].command));
  }
}
