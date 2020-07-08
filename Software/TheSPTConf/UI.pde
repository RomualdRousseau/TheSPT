import controlP5.*;

String[] mixNames = { "THRUST", "YAW", "PITCH", "ROLL", "AUX1", "AUX2", "AUX3", "AUX4" };
ControlP5 cp5;
Numberbox controlWithFocus = null;

void buildUI() {
  cp5 = new ControlP5(this);

  cp5.addTextlabel("version-label")
    .setPosition(10, 10)
    .setText("Version:");

  cp5.addTextlabel("name-label")
    .setPosition(10, 25)
    .setText("Name:");

  cp5.addTextlabel("power-label")
    .setPosition(10, 40)
    .setText("Power: 0%");

  cp5.addScrollableList("serial-port-list")
    .setPosition(10, 60)
    .setSize(100, 100)
    .setLabel("SELECT PORT")
    .setType(ScrollableList.LIST)
    .addItems(Serial.list());

  cp5.addButton("connect-button")
    .setPosition(10, 150)
    .setSize(100, 20)
    .setLabel("CONNECT")
    .hide();

  cp5.addButton("disconnect-button")
    .setPosition(10, 150)
    .setSize(100, 20)
    .setLabel("DISCONNECT")
    .hide();

  cp5.addScrollableList("drone-list")
    .setPosition(10, 180)
    .setSize(100, 200)
    .setLabel("SELECT MODEL")
    .setType(ScrollableList.LIST)
    .hide();

  cp5.addButton("rename-button")
    .setPosition(10, 373)
    .setSize(100, 20)
    .setLabel("RENAME")
    .hide();

  cp5.addButton("save-button")
    .setPosition(10, 395)
    .setSize(100, 20)
    .setLabel("SAVE")
    .hide();

  cp5.addButton("reset-button")
    .setPosition(10, 417)
    .setSize(100, 20)
    .setLabel("RESET")
    .hide();

  cp5.addButton("copy-button")
    .setPosition(10, 440)
    .setSize(100, 20)
    .setLabel("COPY")
    .hide();

  cp5.addButton("paste-button")
    .setPosition(10, 462)
    .setSize(100, 20)
    .setLabel("PASTE")
    .hide();

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      Numberbox n = cp5.addNumberbox("mix" + i + j)
        .setRange(-256, 256)
        .setValue(0)
        .setPosition(120 + j * 60, 20 + i * 20)
        .setSize(50, 14)
        .setDirection(Controller.HORIZONTAL)
        .setMultiplier(1)
        .setLabel("");
      n.onEnter(new CallbackListener() {
        public void controlEvent(CallbackEvent theEvent) {
          controlWithFocus = n;
        }
      }
      ).onLeave(new CallbackListener() {
        public void controlEvent(CallbackEvent theEvent) {
          controlWithFocus = null;
        }
      }
      );
    }
  }

  for (int i = 0; i < 8; i++) {
    Numberbox n = cp5.addNumberbox("trim" + i)
      .setRange(-511, 511)
      .setValue(0)
      .setPosition(610, 20 + i * 20)
      .setSize(50, 14)
      .setDirection(Controller.HORIZONTAL)
      .setMultiplier(1)
      .setLabel("");
    n.onEnter(new CallbackListener() {
        public void controlEvent(CallbackEvent theEvent) {
          controlWithFocus = n;
        }
      }
      ).onLeave(new CallbackListener() {
        public void controlEvent(CallbackEvent theEvent) {
          controlWithFocus = null;
        }
      }
      );
  }
  
  for (int i = 0; i < 8; i++) {
    cp5.addSlider("func" + i)
      .setPosition(680, 20 + i * 20)
      .setSize(50, 14)
      .setRange(0, 2)
      .setNumberOfTickMarks(3)
      .setLabel("");
  }

  for (int i = 0; i < 8; i++) {
    cp5.addLabel("label-vert" + i)
      .setPosition(740, 22 + i * 20)
      .setText(mixNames[i]);
    cp5.addLabel("label-horiz" + i)
      .setPosition(120 + i * 60, 5)
      .setText(mixNames[i]);
  }
  cp5.addLabel("label-trim")
    .setPosition(610, 5)
    .setText("TRIM");
  cp5.addLabel("label-func")
    .setPosition(680, 5)
    .setText("FUNC");
    
  for (int i = 0; i < 8; i++) {
    cp5.addSlider("command" + i, 900, 2300, 1600, 120, 180 + i * 38, 540, 35)
      .lock()
      .setLabel("");
  }

  for (int i = 0; i < 8; i++) {
    cp5.addSlider("trim2" + i, -512, 512, 0, 680, 180 + i * 38, 80, 35)
      .lock()
      .setLabel("");
  }
}

void notifyUIConnected() {
  cp5.get(Button.class, "connect-button").hide();
  cp5.get(Button.class, "disconnect-button").show();
  cp5.get(ScrollableList.class, "drone-list").show();
  cp5.get(Button.class, "save-button").show();
  cp5.get(Button.class, "rename-button").show();
  cp5.get(Button.class, "reset-button").show();
  cp5.get(Button.class, "copy-button").show();
  cp5.get(Button.class, "paste-button").show();
}

void notifyUIDisconnected() {
  cp5.get(Button.class, "connect-button").show();
  cp5.get(Button.class, "disconnect-button").hide();
  cp5.get(ScrollableList.class, "drone-list").hide();
  cp5.get(Button.class, "save-button").hide();
  cp5.get(Button.class, "rename-button").hide();
  cp5.get(Button.class, "reset-button").hide();
  cp5.get(Button.class, "copy-button").hide();
  cp5.get(Button.class, "paste-button").hide();
}

void buildRenameDialog() {
  cp5.addGroup("rename-dialog")
    .setPosition((width - 300) / 2, (height - 80) / 2)
    .setSize(300, 80)
    .setBackgroundColor(color(0, 160))
    .hideBar()
    .hide();

  cp5.addTextfield("new-name-field")
    .setPosition(8, 8)
    .setSize(284, 20)
    .setGroup("rename-dialog")
    .setColorBackground(color(100))
    .setColorForeground(color(100))
    .setLabel("NEW NAME");

  cp5.addButton("commit-rename-button")
    .setPosition(27, 50)
    .setSize(120, 20)
    .setGroup("rename-dialog")
    .setColorBackground(color(100))
    .setColorForeground(color(150))
    .setLabel("OK");

  cp5.addButton("cancel-button")
    .setPosition(173, 50)
    .setSize(120, 20)
    .setGroup("rename-dialog")
    .setColorBackground(color(100))
    .setColorForeground(color(150))
    .setLabel("CANCEL");
}
