#pragma once
#include "ioport.hpp"

enum class TriggerMode { Momentary, Bistable };

class IOPair {
public:
	IOPair(std::vector<IOPort *> i, std::vector<IOPort *> o, std::string n,
	       TriggerMode mode);

	void setup();
	void changeState(bool state);
	void switchState();
	void checkState();
	void updateMqttState();

	std::string name;

private:
	bool state;

	std::vector<uint8_t> initialState;
	std::vector<uint8_t> firstCycle;

	const std::vector<IOPort *> inputPorts;
	const std::vector<IOPort *> outputPorts;
	const TriggerMode mode;
};