#include <ioport.hpp>
#include <Arduino.h>

MCP23017 mcp1 = MCP23017(EXP_ADDR_1);
MCP23017 mcp2 = MCP23017(EXP_ADDR_2);

int IOPORT::portRead(){
    int newState;
    if (dev == IODevice::local){
        newState = digitalRead(pin);
    }
    else if (dev == IODevice::expander1){
        newState = mcp1.digitalRead(pin);
    }
    else{
        newState = mcp2.digitalRead(pin);
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
    else{
        mcp2.digitalWrite(pin, state);
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
    else{
        mcp2.pinMode(pin, t);
    }
}


