#include <Adafruit_MCP23017.h>
#include <Arduino.h>

#include <set>

#include "ioport.hpp"

std::array<Adafruit_MCP23017, 8> IOPort::expanders;

static std::set<DeviceType> expandersToSetup;

IOPort::IOPort(uint8_t pin, IOType type, DeviceType deviceType, TriggerMode mode, RestingState restingState,
               bool invert)
    : pin(pin), type(type), deviceType(deviceType), mode(mode), restingState(restingState), invert(invert) {
	expandersToSetup.emplace(deviceType);
}

bool IOPort::portRead() const {
	if (deviceType == DeviceType::local) {
		return digitalRead(pin);
	} else {
		const auto expanderNumber = expanderNumberFromDeviceType(deviceType);
		auto &expander = expanders[expanderNumber];
		return expander.digitalRead(pin);
	}
}

void IOPort::portWrite(bool state) const {
	if (deviceType == DeviceType::local) {
		digitalWrite(pin, invert ^ state);
	} else {
		const auto expanderNumber = expanderNumberFromDeviceType(deviceType);
		auto &expander = expanders[expanderNumber];
		expander.digitalWrite(pin, invert ^ state);
	}
}

void IOPort::setup() {
	uint8_t t = static_cast<uint8_t>(type);
	if (deviceType == DeviceType::local) {
		pinMode(pin, t);
	} else {
		const auto expanderNumber = expanderNumberFromDeviceType(deviceType);
		auto &expander = expanders[expanderNumber];
		expander.pinMode(pin, t);
	}

	firstState = portRead();
	previousState = firstState;
}

void IOPort::setupExpanders() {
	for (const auto deviceType : expandersToSetup) {
		if (deviceType == DeviceType::local)
			continue;
		const auto expanderNumber = expanderNumberFromDeviceType(deviceType);
		auto &expander = expanders[expanderNumber];
		expander.begin(expanderNumber);
	}

	expandersToSetup.clear();
}

bool IOPort::checkState() {
	bool newState = portRead();

	if (mode == TriggerMode::Momentary) {

		bool activeState;

		switch (restingState) {
		case RestingState::low:
			activeState = 0;
			break;
		case RestingState::high:
			activeState = 1;
			break;
		case RestingState::firstValue:
			activeState = firstState;
			break;
		}

		if (newState == activeState) {
			if (firstCycle) {
				firstCycle = false;
				return true;
			}
		} else {
			firstCycle = true;
		}

		return false;
	} else if (mode == TriggerMode::Bistable) {
		if (newState != previousState) {
			previousState != previousState;
			return true;
		}
		return false;
	}
}