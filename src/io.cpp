#include <Arduino.h>
#include <animationStore.hpp>
#include <io.hpp>
#include <iopair.hpp>

IOPort ai = IOPort(IODevice::expander1, 0, IOType::input);
IOPort ao = IOPort(IODevice::local, 16, IOType::output);

IOPort bi = IOPort(IODevice::expander1, 1, IOType::input);
IOPort bo = IOPort(IODevice::local, 0, IOType::output);

std::vector<IOPort> portsToTest = {};

IOPort Di = IOPort(IODevice::expander1, 3, IOType::input);
IOPort Do = IOPort(IODevice::local, 14, IOType::output);

IOPort ei = IOPort(IODevice::expander1, 4, IOType::input);
IOPort eo = IOPort(IODevice::local, 12, IOType::output);

IOPort fi = IOPort(IODevice::expander1, 5, IOType::input);
IOPort fo = IOPort(IODevice::local, 13, IOType::output);

IOPort gi = IOPort(IODevice::expander1, 6, IOType::input);
IOPort go = IOPort(IODevice::local, 15, IOType::output);

IOPort hi = IOPort(IODevice::expander1, 7, IOType::input);
IOPort ho = IOPort(IODevice::local, 3, IOType::output);

IOPair io[8] = {
    IOPair(std::vector<IOPort> {ai, bi}, std::vector<IOPort>{ao, bo}, "0"),
    IOPair(std::vector<IOPort> {bi}, std::vector<IOPort>{bo}, "1"),
    IOPair(std::vector<IOPort> {ci}, std::vector<IOPort>{co}, "2"),
    IOPair(std::vector<IOPort> {Di}, std::vector<IOPort>{Do}, "3"),
    IOPair(std::vector<IOPort> {ei}, std::vector<IOPort>{eo}, "4"),
    IOPair(std::vector<IOPort> {fi}, std::vector<IOPort>{fo}, "5"),
    IOPair(std::vector<IOPort> {gi}, std::vector<IOPort>{go}, "6"),
    IOPair(std::vector<IOPort> {hi}, std::vector<IOPort>{ho}, "7")
};

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