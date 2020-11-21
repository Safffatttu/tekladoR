#include <Adafruit_MCP23017.h>
#include <Arduino.h>

#include "ioport.hpp"

std::array<Adafruit_MCP23017, 8> IOPort::expanders;
std::vector<uint8_t> IOPort::expandersToSetup = {};

int IOPort::portRead() const {
	if (deviceNum == 0) {
		return digitalRead(pin);
	} else {
		auto &expander = expanders[deviceNum - 1];
		return expander.digitalRead(pin);
	}
}

void IOPort::portWrite(bool state) const {
	if (deviceNum == 0) {
		digitalWrite(pin, invert ^ state);
	} else {
		auto &expander = expanders[deviceNum - 1];
		expander.digitalWrite(pin, invert ^ state);
	}
}

void IOPort::setup() const {
	uint8_t t = static_cast<uint8_t>(type);
	if (deviceNum == 0) {
		pinMode(pin, t);
	} else {
		auto &expander = expanders[deviceNum - 1];
		expander.pinMode(pin, t);
	}
}

void IOPort::setupExpanders() {
	for (const auto index : expandersToSetup) {
		auto &expander = expanders[index];
		expander.begin(index);
	}
	expandersToSetup.clear();
};
