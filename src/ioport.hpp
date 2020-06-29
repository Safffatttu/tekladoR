#pragma once

#include <Adafruit_MCP23017.h>
#include <Arduino.h>

enum class IODevice : uint8_t { local = 0, expander1 = 1, expander2 = 2 };

enum class IOType : uint8_t { input = 0, output = 1 };

class IOPort {
  public:
    IOPort(uint8_t pin, IOType type, uint8_t deviceNum, bool invert = false)
        : deviceNum(deviceNum), pin(pin), type(type), invert(invert) {
        expandersToSetup.push_back(deviceNum - 1);
    }

    int portRead() const;
    void portWrite(bool state) const;
    void setup() const;

    static void setupExpanders();

  private:
    const uint8_t deviceNum;
    const uint8_t pin;
    const IOType type;
    const bool invert;

    static std::array<Adafruit_MCP23017, 8> expanders;
    static std::vector<uint8_t> expandersToSetup;
};