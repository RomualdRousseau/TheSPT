void at_version()
{
  Serial.println("TheSPT v" VERSION);
  Serial.println("OK");
}

void at_list_model()
{
  for (int i = 0; i < HAL_STORAGE_MAX_MODELS; i++)
  {
    char name[9];
    HAL.storage.load_name(i, name);

    Serial.print(name);
    Serial.print(",");
    Serial.println(i);
  }
  Serial.println("OK");
}

void at_get_model()
{
  Serial.print(HAL.current_model_name);
  Serial.print(",");
  Serial.println(HAL.current_model);
  Serial.println("OK");
}

void at_load_model(int model)
{
  HAL.current_model = model;
  HAL.storage.save_current_model(HAL.current_model);
  HAL.storage.load_name(HAL.current_model, HAL.current_model_name);
  HAL.storage.load_mixer(HAL.current_model, (int*) HAL.transmiter.mixer);
  HAL.storage.load_trims(HAL.current_model, (int*) HAL.transmiter.trims);
  HAL.storage.load_funcs(HAL.current_model, (int*) HAL.transmiter.funcs);
  HAL.console.clear_and_sleep();
  Serial.println("OK");
}

void at_save_model()
{
  HAL.storage.save_name(HAL.current_model, HAL.current_model_name);
  HAL.storage.save_mixer(HAL.current_model, (int*) HAL.transmiter.mixer);
  HAL.storage.save_trims(HAL.current_model, (int*) HAL.transmiter.trims);
  HAL.storage.save_funcs(HAL.current_model, (int*) HAL.transmiter.funcs);
  Serial.println("OK");
}

void at_rename_model(char* newname)
{
  strcpy(HAL.current_model_name, newname);
  HAL.console.clear_and_sleep();
  Serial.println("OK");
}

void at_read_mixer()
{
  for (int i = 0; i < PPM_MAX_CHANNELS; i++)
  {
    for (int j = 0; j < PPM_MAX_CHANNELS; j++)
    {
      Serial.println(HAL.transmiter.mixer[i][j]);
    }
  }
  Serial.println("OK");
}

void at_write_mixer(char** argv)
{
  int i = atoi(argv[1]);
  for (int j = 0; j < PPM_MAX_CHANNELS; j++)
  {
    int value = atoi(argv[j + 2]);
    HAL.transmiter.mixer[i][j] = value;
  }
  Serial.println("OK");
}

void at_read_trim()
{
  for (int i = 0; i < PPM_MAX_CHANNELS; i++)
  {
    Serial.println(HAL.transmiter.trims[i]);
  }
  Serial.println("OK");
}

void at_write_trim(char** argv)
{
  for (int i = 0; i < PPM_MAX_CHANNELS; i++)
  {
    int value = atoi(argv[i + 1]);
    HAL.transmiter.trims[i] = value;
  }
  Serial.println("OK");
}


void at_read_func()
{
  for (int i = 0; i < PPM_MAX_CHANNELS; i++)
  {
    Serial.println(HAL.transmiter.funcs[i]);
  }
  Serial.println("OK");
}

void at_write_func(char** argv)
{
  for (int i = 0; i < PPM_MAX_CHANNELS; i++)
  {
    int value = atoi(argv[i + 1]);
    HAL.transmiter.funcs[i] = value;
  }
  Serial.println("OK");
}

void at_read_battery()
{
  Serial.println(HAL.battery.level);
  Serial.println("OK");
}

void at_read_command()
{
  for (int i = 0; i < PPM_MAX_CHANNELS; i++)
  {
    Serial.println(HAL.transmiter.get_raw_command(i));
  }
  Serial.println("OK");
}
