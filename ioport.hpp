#pragma once
#include <Arduino.h>
#include <MCP23017.h>

#define EXP_ADDR_1 0x20
#define EXP_ADDR_2 0x21

extern MCP23017 mcp1;
extern MCP23017 mcp2;

enum class IODevice : uint8_t{
    local = 0,
    expander1 = 1,
    expander2 = 2
};

enum class IOType : uint8_t{
    input = 0,
    output = 1
};

struct IOPORT {
    IODevice dev;
    uint8_t pin;
    IOType type;

    int portRead();
    void portWrite(uint8_t state);
    void setup();
};

typedef struct IOPORT IOPORT;