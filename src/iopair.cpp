#include <iopair.hpp>
#include <ioport.hpp>
#include <messages.hpp>


IOPair::IOPair(std::vector<IOPort> i, std::vector<IOPort> o, std::string n)
{
    inputPorts = i;
    outputPorts = o;
    name = n;
    state = false;
    inputState = std::vector<bool>(false);
    firstCycle = std::vector<bool>(true);
}

void IOPair::setup()
{
    for (auto &&inputPort : inputPorts)
    {
        inputPort.setup();
    }

    for (auto &&outputPort : outputPorts)
    {
        outputPort.setup();
        outputPort.portWrite((uint8_t)state);
    }

    for (size_t i = 0; i < inputPorts.size(); i++)
    {
        inputState[i] = (bool)inputPorts[i].portRead();
    }
}

void IOPair::changeState(int newState)
{
    state = (bool)newState;

    for (auto &&outputPort : outputPorts)
    {
        outputPort.portWrite((uint8_t)state);
    }
}

void IOPair::checkState()
{
    for (size_t i = 0; i < inputPorts.size(); i++)
    {
        bool newState = (bool)inputPorts[i].portRead();

        if (newState != inputState[i])
        {
            if (firstCycle[i])
            {
                firstCycle[i] = false;
                state = !state;
                publishPairMqtt(name, state);
                for (auto &&outputPort : outputPorts)
                {
                    outputPort.portWrite((uint8_t)state);
                }
            }
        }
        else
        {
            firstCycle[i] = true;
        }
    }
}

void IOPair::switchState()
{
    state = !state;
    for (auto &&outputPort : outputPorts)
    {
        outputPort.portWrite((uint8_t)state);
    }
}

void IOPair::updateMqttState()
{
    publishPairMqtt(name, state);
}