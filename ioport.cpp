#include <ioport.hpp>
#include <Arduino.h>

Adafruit_MCP23017 mcp1;

int IOPort::portRead()
{
    int newState;
    if (device == IODevice::local)
    {
        newState = digitalRead(pin);
    }
    else if (device == IODevice::expander1)
    {
        newState = mcp1.digitalRead(pin);
    }

    return newState;
}

void IOPort::portWrite(uint8_t state)
{
    if (device == IODevice::local)
    {
        digitalWrite(pin, state);
    }
    else if (device == IODevice::expander1)
    {
        mcp1.digitalWrite(pin, state);
    }
}

void IOPort::setup()
{
    uint8_t t = static_cast<uint8_t>(type);

    if (device == IODevice::local)
    {
        pinMode(pin, t);
    }
    else if (device == IODevice::expander1)
    {
        mcp1.pinMode(pin, t);
    }
}
