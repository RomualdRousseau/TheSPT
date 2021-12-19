void print_tasks() {
  HAL.scheduler.debug_tasks();
}

void snap_trims() {
  if (HAL.maiden_flag) {
    HAL.transmiter.snap_trim(COMMAND_RUDDER);
    HAL.transmiter.snap_trim(COMMAND_ELEVATOR);
    HAL.transmiter.snap_trim(COMMAND_AILERON);
  }
}
