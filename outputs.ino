void setupOutputs() {

  for(int i = 0; i < RELAYS_COUNT; i++) 
  {
    int pin = relayPins[i];
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }

}
