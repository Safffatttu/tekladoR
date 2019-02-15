#pragma once
#include <ioport.hpp>

class IOPair
{
  public:
    IOPair(IOPort *i, IOPort *o, std::string n);

    void setup();
    void changeState(int state);
    void switchState();
    void checkState();
    void updateMqttState();

    std::string name;

  private:
    bool state;
    bool inputState;
    bool firstCycle;

    IOPort *inputPort;
    IOPort *outputPort;
};