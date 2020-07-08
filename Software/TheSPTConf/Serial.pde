import processing.serial.*;

Serial serial = null;
String selectedSerialPortName = null;

void waitSerialUntilReady() {
  if (serial == null) {
    return;
  }

  while (true) { // TODO: set a limit of try
    serialRequest("AT");

    delay(100);
    if (serial.available() > 0) {
      break;
    }

    delay(1000);
  }

  serial.clear();
}

String[] sendSerialCommand(String cmd) {
  if (serial == null) {
    return null;
  }

  serialRequest(cmd);

  ArrayList<String> list = new ArrayList<String>();

  for(int i = 0; i < 1000; i++) { //Expect less than 1000 responses
    String result = serialResponse(); 
    if (result.equals("OK")) {
      break;
    }

    list.add(result);
  }

  return list.toArray(new String[list.size()]);
}

void serialRequest(String cmd) {
  serial.write(cmd + "\n");
}

String serialResponse() {
  String result = null;

  do { // TODO: set a limit of try
    while (serial.available() <= 0) delay(10);
    result = serial.readStringUntil('\n');
  } while (result == null);

  return result.replaceAll("\r", "").replaceAll("\n", "");
}
