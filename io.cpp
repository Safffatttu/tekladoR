#include <iopair.hpp>
#include <io.hpp>
#include <Arduino.h>

IOPORT ai = {IODevice::expander1, 0, IOType::input};
IOPORT ao = {IODevice::local, 16, IOType::output};

IOPORT bi = {IODevice::expander1, 1, IOType::input};
IOPORT bo = {IODevice::local, 0, IOType::output};

IOPORT ci = {IODevice::expander1, 2, IOType::input};
IOPORT co = {IODevice::local, 2, IOType::output};

IOPORT Di = {IODevice::expander1, 3, IOType::input};
IOPORT Do = {IODevice::local, 14, IOType::output};

IOPORT ei = {IODevice::expander1, 4, IOType::input};
IOPORT eo = {IODevice::local, 12, IOType::output};

IOPORT fi = {IODevice::expander1, 5, IOType::input};
IOPORT fo = {IODevice::local, 13, IOType::output};

IOPORT gi = {IODevice::expander1, 6, IOType::input};
IOPORT go = {IODevice::local, 15, IOType::output};

IOPORT hi = {IODevice::expander1, 7, IOType::input};
IOPORT ho = {IODevice::local, 3, IOType::output};

IOPair io[8] = {
    IOPair(&ai, &ao, 0),
    IOPair(&bi, &bo, 1),
    IOPair(&ci, &co, 2),
    IOPair(&Di, &Do, 3),
    IOPair(&ei, &eo, 4),
    IOPair(&fi, &fo, 5),
    IOPair(&gi, &go, 6),
    IOPair(&hi, &ho, 7)
};

void setupIO(){
    mcp1.begin();
    for(auto&& pair : io){
        pair.setup();
    }
}

void checkInputs(){
    for(auto&& pair : io){
        pair.checkState();
    }
}