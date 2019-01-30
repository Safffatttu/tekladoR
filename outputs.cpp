#include <constants.hpp>
#include <outputs.hpp>
#include <MCP23017.h>


void setupOutputs() {

  for(int i = 0; i < RELAYS_COUNT; i++) 
  {
    int pin = relayPins[i];
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }

}

void writeToOutput(int pin, int value){

}