#include <Arduino.h>
#include <messages.hpp>
#include <io.hpp>
#include <Adafruit_MCP23017.h>

/*

Pary wej/wyj Wejscie na NODEMCU/Wyjście na MCP23017
D0 - A1
D3 - A2
D4 - A3
D5 - A4
D6 - A5
D7 - A6
D8 - A7
D9 - A8

*/

void setup() {
  Serial.begin(9600);
  setupNetwork();
  setupIO();
}

void loop() {
  checkInputs();
}