#pragma once
#include <ioport.hpp>

class IOPair
{
  public:
    IOPair(IOPort *i, IOPort *o, int n);

    void setup();
    void changeState(int state);
    void switchState();
    void checkState();
    void updateMqttState();

    int number;

  private:
    bool state;
    bool inputState;
    bool firstCycle;

    IOPort *inputPort;
    IOPort *outputPort;
};