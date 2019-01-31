#include <MCP23017.h>
#include <iopair.hpp>
#include <io.hpp>

IOPair io[1] = {
    IOPair({local, 0, input}, {local, 0, input}, 0)
};

void setupIO(){
    for(auto&& pair : io){
        pair.setup();
    }
}

void checkInputs(){
    for(auto&& pair : io){
        pair.checkState();
    }
}