#pragma once
#include <Arduino.h>
#include <MCP23017.h>

extern MCP23017 mcp1;
extern MCP23017 mcp2;

enum IODevice {
    local = 0,
    expander1 = 1,
    expander2 = 2
};

enum IOType {
    input = 0,
    output = 1
};

struct IOPORT {
    IODevice dev;
    uint16 port;
    IOType type;

    int portRead();
    void portWrite(uint8_t state);
    void setup();
};
