#include <iopair.hpp>
#include <ioport.hpp>
#include <messages.hpp>

IOPair::IOPair(IOPORT* i, IOPORT* o, int n) {
    inputPort = i;
    outputPort = o;
    number = n;
    state = false;
    inputState = false;
    firstCycle = true;
}

void IOPair::setup(){
    (*inputPort).setup();
    (*outputPort).setup();
    (*outputPort).portWrite(1);
    inputState = (bool) (*inputPort).portRead();
}

void IOPair::changeState(int newState){
    state = (bool) newState;
    (*inputPort).portWrite(newState);
}

void IOPair::checkState(){
    bool newState = (bool) (*inputPort).portRead();

    if (newState != inputState){
        if (firstCycle){
            firstCycle = false;
            state = !state;
            publishMqtt(number, state);
            (*outputPort).portWrite((int) state);
        }
    }else{
        firstCycle = true;
    }
}

void IOPair::switchState(){
    state = !state;
    (*outputPort).portWrite((int) state);
}