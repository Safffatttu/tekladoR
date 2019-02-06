#pragma once
#include <Arduino.h>
#include <Adafruit_MCP23017.h>

extern Adafruit_MCP23017 mcp1;

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