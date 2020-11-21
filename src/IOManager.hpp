#pragma once

#include <vector>

#include "iopair.hpp"
#include "ioport.hpp"

class IOManager {
public:
	IOManager();

	void loop();
	void setup();

	std::vector<IOPair> &getPairs() { return pairs; }

	static IOManager &the();

private:
	std::vector<IOPort> ports;
	std::vector<IOPair> pairs;
};