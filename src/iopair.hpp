#pragma once
#include "ioport.hpp"

class IOPair {
public:
	IOPair(std::vector<IOPort *> i, std::vector<IOPort *> o, std::string n);

	void setup();
	void changeState(bool state);
	void switchState();
	void checkState();
	void updateMqttState();

	std::string name;

private:
	bool state;

	const std::vector<IOPort *> inputPorts;
	const std::vector<IOPort *> outputPorts;
};