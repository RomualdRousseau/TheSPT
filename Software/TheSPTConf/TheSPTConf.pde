/*
 * Welcome to the TheSPTConf sketch
 */
 
float clipboardMixer[][] = new float[8][8];
float clipboardTrim[] = new float[8];
int savedTime;

void setup() {
  size(810, 490);
  noStroke();
  buildUI();
  buildRenameDialog();
  resetAllFunc();
}

void draw() {
  background(51);

  int ellapsedTime = millis() - savedTime;
  if (ellapsedTime > 50) {
    updatePartiallyFunc();
    savedTime = millis();
  }
}

void controlEvent(ControlEvent theEvent) {
  if (theEvent.getName() == "serial-port-list") {
    int i = (int) theEvent.getValue();
    selectedSerialPortName = Serial.list()[i];
    notifyUIDisconnected();
  }

  if (theEvent.getName() == "connect-button" && serial == null && selectedSerialPortName != null) {  
    //serial = new Serial(this, selectedSerialPortName, 57600);
    serial = new Serial(this, selectedSerialPortName, 115200);
    waitSerialUntilReady();
    notifyUIConnected();
    updateAllFunc();
  }

  if (theEvent.getName() == "disconnect-button" && serial != null) {
    serial.stop();
    serial = null;
    notifyUIDisconnected();
    resetAllFunc();
    updateAllFunc();
  }

  if (theEvent.getName() == "drone-list") {
    loadFunc((int) theEvent.getValue());
    updateAllFunc();
  }

  if (theEvent.getName() == "save-button") {
    saveFunc();
  }
  
  if (theEvent.getName() == "reset-button") {
    resetFunc();
  }

  if (theEvent.getName() == "copy-button") {
    copyFunc();
  }

  if (theEvent.getName() == "paste-button") {
    pasteFunc();
  }
  
  if (theEvent.getName() == "rename-button") {
    beginRenameFunc();
  }
  
  if (theEvent.getName() == "commit-rename-button") {
    commitRenameFunc();
    updateNameFunc();
    updateDroneListFunc();
  }
  
  if (theEvent.getName() == "cancel-rename-button") {
    rollbackRenameFunc();
  }
}

void keyPressed() {
  if(key == DELETE && controlWithFocus != null) {
    controlWithFocus.setValue(0);
  }
  if(key == '-') {
    controlWithFocus.setValue(-controlWithFocus.getValue());
  }
}
