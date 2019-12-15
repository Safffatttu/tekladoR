#pragma once
#include <Adafruit_MCP23017.h>
#include <Arduino.h>

extern Adafruit_MCP23017 mcp1;
extern Adafruit_MCP23017 mcp2;

enum class IODevice : uint8_t { local = 0, expander1 = 1, expander2 = 2 };

enum class IOType : uint8_t { input = 0, output = 1 };

class IOPort {
  public:
    IOPort(IODevice device, uint8_t pin, IOType type)
        : device(device), pin(pin), type(type) {}

    int portRead() const;
    void portWrite(bool state) const;
    void setup() const;

  private:
    const IODevice device;
    const uint8_t pin;
    const IOType type;
};