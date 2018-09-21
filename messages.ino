void messageReceived(String &topic, String &payload) {

  int index = topic.lastIndexOf('/');
  int relayNumber = topic.substring(index + 1).toInt();
  int state = payload.toInt();

  if (relayNumber < 0 || relayNumber >= RELAYS_COUNT) {
    return;
  }

  int pin = relayPins[relayNumber];

  if (state > 1 || state < 0){
    return;
  }

  states[relayNumber] = state;

  digitalWrite(pin, state != 1);
}

void checkMqttConnection() {
  if (!client.connected()) {
    connectMqtt();
  }
}

void connectMqtt() {
  client.connect("Arduino");
  String topic = String(deviceTopic);
  topic.concat("#");
  client.subscribe(topic, 2);
}

void connectToWifi(){
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED);
}