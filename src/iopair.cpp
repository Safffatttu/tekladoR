#include "iopair.hpp"
#include "ioport.hpp"
#include "messages.hpp"

IOPair::IOPair(std::vector<IOPort *> i, std::vector<IOPort *> o, std::string n,
               TriggerMode mode)
    : name(n), state(false), initialState(i.size()), firstCycle(i.size()),
      inputPorts(std::move(i)), outputPorts(std::move(o)), mode(mode) {

	for (size_t j = 0; j < initialState.size(); j++) {
		initialState[j] = true;
	}

	for (size_t j = 0; j < firstCycle.size(); j++) {
		firstCycle[j] = true;
	}
}

void IOPair::setup() {
	for (const auto inputPort : inputPorts) {
		inputPort->setup();
	}

	for (const auto outputPort : outputPorts) {
		outputPort->setup();
		outputPort->portWrite(state);
	}

	for (size_t i = 0; i < inputPorts.size(); i++) {
		initialState[i] = inputPorts[i]->portRead();
	}
}

void IOPair::changeState(bool newState) {
	state = newState;
	for (const auto outputPort : outputPorts) {
		outputPort->portWrite(newState);
	}
}

void IOPair::checkState() {

	if (mode == TriggerMode::Momentary) {
		for (size_t i = 0; i < inputPorts.size(); i++) {
			bool newState = inputPorts[i]->portRead();

			if (newState != initialState[i]) {
				if (firstCycle[i]) {
					firstCycle[i] = false;
					state = !state;
					for (const auto outputPort :
					     outputPorts) {
						outputPort->portWrite(state);
					}
					updateMqttState();
				}
			} else {
				firstCycle[i] = true;
			}
		}
	} else { // TriggerMode::Bistable
		for (size_t i = 0; i < inputPorts.size(); i++) {

			auto &previousState = initialState[i];
			bool newState = inputPorts[i]->portRead();

			if (newState != previousState) {
				state = !state;
				previousState = newState;

				for (const auto outputPort : outputPorts) {
					outputPort->portWrite(state);
				}
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