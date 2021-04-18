#include "iopair.hpp"
#include "ioport.hpp"
#include "messages.hpp"

IOPair::IOPair(std::vector<IOPort *> i, std::vector<IOPort *> o, std::string n)
    : name(n), state(false), inputPorts(std::move(i)), outputPorts(std::move(o)) {}

void IOPair::setup() {
	for (auto inputPort : inputPorts) {
		inputPort->setup();
	}

	for (auto outputPort : outputPorts) {
		outputPort->setup();
		outputPort->portWrite(state);
	}
}

void IOPair::changeState(bool newState) {
	state = newState;
	for (auto outputPort : outputPorts) {
		outputPort->portWrite(newState);
	}
}

void IOPair::checkState() {
	for (auto input : inputPorts) {
		bool changed = input->checkState();
		if (changed) {
			state = !state;
			for (auto outputPort : outputPorts) {
				outputPort->portWrite(state);
				updateMqttState();
			}
		}
	}
}

void IOPair::switchState() {
	state = !state;
	for (const auto outputPort : outputPorts) {
		outputPort->portWrite(state);
	}
}

void IOPair::updateMqttState() { publishPairMqtt(name, state); }