void setupInputs(){
  
  for(int i = 0; i < RELAYS_COUNT; i++)
  {
      int pin = inputPins[i];
      pinMode(pin, INPUT);
  }

}

void checkInputs(){
    for(int i = 0; i < RELAYS_COUNT; i++)
    {
        int pinNum = inputPins[i];
        int newState = digitalRead(pinNum);

        if (newState != states[i]) {
            states[i] = newState;
            int outputPin = relayPins[i];
            digitalWrite(outputPin, newState != 1);

            String topic = String(deviceTopic);
            topic.concat(i);

            client.publish(topic, String(newState));
        }
    }
}
