void resetFunc() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (i == j) {
        cp5.get(Numberbox.class, "mix" + i + j).setValue(256);
      } else {
        cp5.get(Numberbox.class, "mix" + i + j).setValue(0);
      }
    }
  }

  for (int i = 0; i < 8; i++) {
    cp5.get(Numberbox.class, "trim" + i).setValue(0);
  }
}

void resetAllFunc() {
  resetFunc();
  
  for (int i = 0; i < 8; i++) {
    cp5.get(Slider.class, "command" + i).setValue(1600);
  }
  
  for (int i = 0; i < 8; i++) {
    cp5.get(Slider.class, "trim2" + i).setValue(0);
  }
}

void loadFunc(int i) {
  sendSerialCommand("AT+LMDL=" + i);
}

void saveFunc() {
  String cmd;
  boolean firstPass;
  
  for (int i = 0; i < 8; i++) {
    cmd = "AT+WMXR=" + i;
    for (int j = 0; j < 8; j++) {
      int value = int(cp5.get(Numberbox.class, "mix" + j + i).getValue());
      cmd += "," + value;
    }
    sendSerialCommand(cmd);
  }

  cmd = "AT+WTRM=";
  firstPass = true;
  for (int i = 0; i < 8; i++) {
    int value = int(cp5.get(Numberbox.class, "trim" + i).getValue());
    if (firstPass) {
      cmd += value;
      firstPass = false;
    } else { 
      cmd += "," + value;
    }
  }
  sendSerialCommand(cmd);
  
  cmd = "AT+WFUN=";
  firstPass = true;
  for (int i = 0; i < 8; i++) {
    int value = int(cp5.get(Slider.class, "func" + i).getValue());
    if (firstPass) {
      cmd += value;
      firstPass = false;
    } else { 
      cmd += "," + value;
    }
  }
  sendSerialCommand(cmd);
  
  sendSerialCommand("AT+SMDL");
}

void copyFunc() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      clipboardMixer[i][j] = cp5.get(Numberbox.class, "mix" + i + j).getValue();
    }
  }

  for (int i = 0; i < 8; i++) {
    clipboardTrim[i] = cp5.get(Numberbox.class, "trim" + i).getValue();
  }
  
  for (int i = 0; i < 8; i++) {
    clipboardFunc[i] = cp5.get(Numberbox.class, "func" + i).getValue();
  }
}

void pasteFunc() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      cp5.get(Numberbox.class, "mix" + i + j).setValue(clipboardMixer[i][j]);
    }
  }

  for (int i = 0; i < 8; i++) {
    cp5.get(Numberbox.class, "trim" + i).setValue(clipboardTrim[i]);
  }
  
  for (int i = 0; i < 8; i++) {
    cp5.get(Numberbox.class, "func" + i).setValue(clipboardFunc[i]);
  }
}

void beginRenameFunc() {
  cp5.get(Textfield.class, "new-name-field").setText(""); 
  cp5.get(Group.class, "rename-dialog").show();
}

void commitRenameFunc() {
  String newName = cp5.get(Textfield.class, "new-name-field").getText().toUpperCase(); 
  sendSerialCommand("AT+RNMDL=" + newName);
  sendSerialCommand("AT+SMDL");
  cp5.get(Group.class, "rename-dialog").hide();
}

void rollbackRenameFunc() {
  cp5.get(Group.class, "rename-dialog").hide();
}

void updatePartiallyFunc() {
  updatePowerFunc();
  updateCommandFunc();
  updateTrim2Func();
}

void updateAllFunc() {
  updateVersionFunc();
  updateDroneListFunc();
  updateNameFunc();
  updateMixerFunc();
  updateTrimFunc();
  updateFuncFunc();
  updatePowerFunc();
  updateCommandFunc();
  updateTrim2Func();
}

void updateVersionFunc() {
  String[] result = sendSerialCommand("AT");
  if (result == null) {
    cp5.get(Textlabel.class, "version-label").setText("Version:");
  } else {
    cp5.get(Textlabel.class, "version-label").setText("Version: " + result[0]);
  }
}

void updateDroneListFunc() {
  cp5.get(ScrollableList.class, "drone-list").clear();

  String[] result = sendSerialCommand("AT+LSMDL");
  if (result == null) {
    return;
  }
  
  for(int i = 0; i < result.length; i++) {
    result[i] = result[i].split(",")[0];
  }

  cp5.get(ScrollableList.class, "drone-list").addItems(result);
}

void updateNameFunc() {
  String[] name = sendSerialCommand("AT+GMDL");
  if (name == null) {
    cp5.get(Textlabel.class, "name-label").setText("Name:");
  } else {
    cp5.get(Textlabel.class, "name-label").setText("Name: " + name[0].split(",")[0]);
  }
}

void updatePowerFunc() {
  String[] power = sendSerialCommand("AT+RBAT");
  if (power == null) {
    cp5.get(Textlabel.class, "power-label").setText("Power: 0%");
  } else {
    cp5.get(Textlabel.class, "power-label").setText("Power: " + power[0] + "%");
  }
}

void updateMixerFunc() {
  String result[] = sendSerialCommand("AT+RMXR");
  if (result == null) {
    return;
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      int value = int(result[i * 8 + j]);
      cp5.get(Numberbox.class, "mix" + j + i).setValue((float) value);
    }
  }
}

void updateTrimFunc() {
  String result[] = sendSerialCommand("AT+RTRM");
  if (result == null) {
    return;
  }

  for (int i = 0; i < 8; i++) {
    int value = int(result[i]);
    cp5.get(Numberbox.class, "trim" + i).setValue(value);
  }
}

void updateFuncFunc() {
  String result[] = sendSerialCommand("AT+RFUN");
  if (result == null) {
    return;
  }

  for (int i = 0; i < 8; i++) {
    int value = int(result[i]);
    cp5.get(Slider.class, "func" + i).setValue(value);
  }
}

void updateCommandFunc() {
  String result[] = sendSerialCommand("AT+RCOM");
  if (result == null) {
    return;
  }

  for (int i = 0; i < 8; i++) {
    int value = int(result[i]);
    cp5.get(Slider.class, "command" + i).setValue(value);
  }
}

void updateTrim2Func() {
  String result[] = sendSerialCommand("AT+RTRM");
  if (result == null) {
    return;
  }

  for (int i = 0; i < 8; i++) {
    int value = int(result[i]);
    if(cp5.get(Slider.class, "trim2" + i).getValue() != value) {
      cp5.get(Numberbox.class, "trim" + i).setValue(value);
    }
    cp5.get(Slider.class, "trim2" + i).setValue(value);
  }
}
