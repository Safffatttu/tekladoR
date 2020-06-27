#include <Arduino.h>

#include "ioport.hpp"

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;

int IOPort::portRead() const {
    int newState;
    if (device == IODevice::local) {
        newState = digitalRead(pin);
    } else if (device == IODevice::expander1) {
        newState = mcp1.digitalRead(pin);
    } else {
        newState = mcp2.digitalRead(pin);
    }

    return newState;
}

void IOPort::portWrite(bool state) const {
    if (device == IODevice::local) {
        digitalWrite(pin, !state);
    } else if (device == IODevice::expander1) {
        mcp1.digitalWrite(pin, !state);
    } else {
        mcp2.digitalWrite(pin, !state);
    }
}

void IOPort::setup() const {
    uint8_t t = static_cast<uint8_t>(type);

    if (device == IODevice::local) {
        pinMode(pin, t);
    } else if (device == IODevice::expander1) {
        mcp1.pinMode(pin, t);
    } else {
        mcp2.pinMode(pin, t);
    }
}
