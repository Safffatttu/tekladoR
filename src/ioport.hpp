#pragma once

#include <Adafruit_MCP23017.h>
#include <Arduino.h>

enum class IOType : uint8_t { input = 0, output = 1 };
enum class RestingState : uint8_t { low = 0, high = 1, firstValue = 2 };
enum class TriggerMode : uint8_t { Momentary = 0, Bistable = 1 };

enum class DeviceType : uint8_t {
	expander0,
	expander1,
	expander2,
	expander3,
	expander4,
	expander5,
	expander6,
	expander7,
	local,
	__COUNT__
};

inline uint8_t expanderNumberFromDeviceType(DeviceType type) {
	uint8_t deviceNumber = static_cast<uint8_t>(type);
	uint8_t maxNumber = static_cast<uint8_t>(DeviceType::local);

	if (deviceNumber >= maxNumber)
		abort(); // TODO Handle this case

	return deviceNumber;
}

class IOPort {
public:
	IOPort(uint8_t pin, IOType type, DeviceType deviceType, TriggerMode mode, RestingState, bool invert = false);

	bool portRead() const;
	void portWrite(bool state) const;
	void setup();

	bool checkState();

	static void setupExpanders();

private:
	// Configuration
	const uint8_t pin;
	const IOType type;
	const DeviceType deviceType;
	const TriggerMode mode;
	const RestingState restingState;
	const bool invert;

	// State
	bool firstState;
	bool firstCycle = true;
	bool previousState;

	static std::array<Adafruit_MCP23017, 8> expanders;
};
