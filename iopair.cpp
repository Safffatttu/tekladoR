#include <iopair.hpp>
#include <ioport.hpp>
#include <messages.hpp>

IOPair::IOPair(IOPORT i, IOPORT o, int n) {
    inputPort = i;
    outputPort = o;
    number = n;
    state = false;
}

void IOPair::setup(){
    inputPort.setup();
    outputPort.setup();
    outputPort.portWrite(0);
}

void IOPair::changeState(int newState){
    state = (bool) newState;
    inputPort.portWrite(newState);
}

void IOPair::checkState(){
    bool newState = (bool) inputPort.portRead();

    if ((bool) newState != state){
        state = newState;
        publishMqtt(number, state);
    }
}

void IOPair::switchState(){
    state = !state;
    outputPort.portWrite((int) state);
}
