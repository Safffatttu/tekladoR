#pragma once
#include <ioport.hpp>

class IOPair {
    public:
        IOPair(IOPORT* i, IOPORT* o, int n);
         
        void setup();
        void changeState(int state);
        void switchState();        
        void checkState();
    private:
        bool state;
        bool inputState;
        bool firstCycle;
        
        IOPORT* inputPort;
        IOPORT* outputPort;
        int number;
};