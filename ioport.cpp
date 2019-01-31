#include <ioport.hpp>
#include <Arduino.h>
#include <constants.hpp>

MCP23017 mcp1 = MCP23017(EXP_ADDR_1);
MCP23017 mcp2 = MCP23017(EXP_ADDR_2);

int IOPORT::portRead(){
    int newState;
    if (dev == local){
        newState = digitalRead(port);
    }
    else if (dev == expander1){
        newState = mcp1.digitalRead(port);
    }
    else{
        newState = mcp2.digitalRead(port);
    }

    return newState;
}

void IOPORT::portWrite(uint8_t state){
    if (dev == local){
        digitalWrite(port, state);
    }
    else if (dev == expander1){
        mcp1.digitalWrite(port, state);
    }
    else{
        mcp2.digitalWrite(port, state);
    }
}

void IOPORT::setup(){
    if (dev == local){
        pinMode(port, type);
    }
    else if (dev == expander1){
        mcp1.pinMode(port, !type);
    }
    else{
        mcp2.pinMode(port, !type);
    }
}


