#include <ESP8266WiFi.h>
#include <constants.hpp>
#include <inputs.hpp>
#include <outputs.hpp>

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

        if (newState != switchStates[i]) {
            switchStates[i] = newState;

            states[i] = (states[i] != 1);
            newState = states[i];

            int outputPin = relayPins[i];

            digitalWrite(outputPin, newState != 1);
            writeToOutput(outputPin, newState != 1);
            
            String topic = String(deviceTopic);
            topic.concat(i);

            // client.publish(topic, String(newState));
        }
    }
}
