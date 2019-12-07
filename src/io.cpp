#include <Arduino.h>
#include <animationStore.hpp>
#include <io.hpp>
#include <iopair.hpp>

std::vector<IOPair> io = {};
std::vector<IOPort> portsToTest{};

void setupAnimations() {}

void testOutputs() {
    for (size_t i = 0; i < 9; i++) {
        for (auto &&port : portsToTest) {
            port.portWrite(i % 2);
            delay(200);
        }
    }
}

void setupIO() {
    mcp1.begin();
    mcp2.begin(1);
    for (auto &&pair : io) {
        pair.setup();
    }
}

void checkInputs() {
    for (auto &&pair : io) {
        pair.checkState();
    }
}