#include <ioport.hpp>
#include <Arduino.h>

Adafruit_MCP23017 mcp1;

int IOPORT::portRead(){
    int newState;
    if (dev == IODevice::local){
        newState = digitalRead(pin);
    }
    else if (dev == IODevice::expander1){
        newState = mcp1.digitalRead(pin);
    }

    return newState;
}

void IOPORT::portWrite(uint8_t state){
    if (dev == IODevice::local){
        digitalWrite(pin, state);
    }
    else if (dev == IODevice::expander1){
        mcp1.digitalWrite(pin, state);
    }
}

void IOPORT::setup(){
    uint8_t t = static_cast<uint8_t> (type);
    
    if (dev == IODevice::local){
        pinMode(pin, t);
    }
    else if (dev == IODevice::expander1){
        mcp1.pinMode(pin, t);
    }
}
