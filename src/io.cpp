#include <Arduino.h>
#include <animationStore.hpp>
#include <io.hpp>
#include <iopair.hpp>

IOPort animacjaInputKlatka = IOPort(IODevice::expander2, 0, IOType::input); // Klatka
IOPort animacjaInputPs3 = IOPort(IODevice::expander2, 1, IOType::input); // PS3
IOPort animacjaInputLazienka = IOPort(IODevice::expander2, 2, IOType::input); // Łazienka
IOPort lampa5Input = IOPort(IODevice::expander2, 3, IOType::input);
IOPort lampa6Input = IOPort(IODevice::expander2, 4, IOType::input);
IOPort lampaKinkietInput = IOPort(IODevice::expander2, 5, IOType::input);
IOPort lampaLazienkaInput = IOPort(IODevice::expander2, 6, IOType::input);
IOPort wentylatorLazienkaInput = IOPort(IODevice::expander2, 7, IOType::input);
IOPort lampaSchodyInput = IOPort(IODevice::expander2, 8, IOType::input);
IOPort dzwonekInput = IOPort(IODevice::expander2, 9, IOType::input);

IOPort lampa1 = IOPort(IODevice::expander1, 0, IOType::output);
IOPort lampa2 = IOPort(IODevice::expander1, 1, IOType::output);
IOPort lampa3 = IOPort(IODevice::expander1, 2, IOType::output);
IOPort lampa4 = IOPort(IODevice::expander1, 3, IOType::output);
IOPort lampa5 = IOPort(IODevice::expander1, 4, IOType::output);
IOPort lampa6 = IOPort(IODevice::expander1, 5, IOType::output);
IOPort lampa7 = IOPort(IODevice::expander1, 6, IOType::output);
IOPort lampa8 = IOPort(IODevice::expander1, 7, IOType::output);
IOPort lampa9 = IOPort(IODevice::expander1, 8, IOType::output);
IOPort lampa10 = IOPort(IODevice::expander1, 9, IOType::output);
IOPort lampaKinkiet = IOPort(IODevice::expander1, 10, IOType::output);
IOPort lampaLazienka = IOPort(IODevice::expander1, 11, IOType::output);
IOPort wentylatorLazienka = IOPort(IODevice::expander1, 12, IOType::output);
IOPort lampaSchody = IOPort(IODevice::expander1, 13, IOType::output);

std::vector<IOPort> portsToTest = {
       lampa1, lampa2, lampa3, lampa4, lampa5, lampa6, lampa7, lampa8, lampa9, lampa10, lampaKinkiet, lampaLazienka, wentylatorLazienka, lampaSchody
};

// IOPort input = IOPort(IODevice::local, 5, IOType::input);
// IOPort output = IOPort(IODevice::local, 15, IOType::output);

// IOPair io[1] = {
//        IOPair(std::vector<IOPort>{input}, std::vector<IOPort>{output},
//        "lampa")
// };
IOPair io[6] = {
    IOPair(std::vector<IOPort>{},
           std::vector<IOPort>{lampa1, lampa2, lampa3, lampa4, lampa5, lampa6,
                               lampa7, lampa8, lampa9, lampa10},
           "lampaSufit"),
    IOPair(std::vector<IOPort>{lampa5Input}, std::vector<IOPort>{lampa5},
           "lampa5"),
    IOPair(std::vector<IOPort>{lampa6Input}, std::vector<IOPort>{lampa6},
           "lampa6"),
    IOPair(std::vector<IOPort>{lampaKinkietInput},
           std::vector<IOPort>{lampaKinkiet}, "lampaKinkiet"),
    IOPair(std::vector<IOPort>{lampaLazienkaInput},
           std::vector<IOPort>{lampaLazienka}, "lampaLazienka"),
//     IOPair(std::vector<IOPort>{wentylatorLazienkaInput},
//            std::vector<IOPort>{wentylatorLazienka}, "wentylatorLazienka"),
    IOPair(std::vector<IOPort>{lampaSchodyInput},
           std::vector<IOPort>{lampaSchody}, "lampaSchody")};

void setupAnimations() {
    Animation animacjaKlatka = Animation({animacjaInputKlatka}, 
    {lampa1, lampa2, lampa3, lampa4, lampa5, lampa6, lampa7, lampa8, lampa9, lampa10},
    {
         {
                {false, false, false, false, false, false, false, false,false, false},
                {true,  true,  false, false, false, false, false, false,false, false},
                {true,  true,  true,  true,  false, false, false, false,false, false},
                {true,  true,  true,  true,  true,  true,  false, false,false, false},
                {true,  true,  true,  true,  true,  true,  true,  true, false, false},
                {true,  true,  true,  true,  true,  true,  true,  true, true,  true}
         },{
                {true,  true,  true,  true,  true,  true,  true,  true, true,  true},
                {true,  true,  true,  true,  true,  true,  true,  true, false, false},
                {true,  true,  true,  true,  true,  true,  false, false,false, false},
                {true,  true,  true,  true,  false, false, false, false,false, false},
                {true,  true,  false, false, false, false, false, false,false, false},
                {false, false, false, false, false, false, false, false,false, false}
         }
    },
    false, {"lampa1", "lampa2", "lampa3", "lampa4", "lampa5", "lampa6", "lampa7", "lampa8", "lampa9", "lampa10"}, 0.1);

    Animation animacjaPs3 = Animation({animacjaInputPs3}, 
    {lampa1, lampa2, lampa3, lampa4, lampa5, lampa6, lampa7, lampa8, lampa9, lampa10},
    {
         {
                {false, false, false, false, false, false, false, false,false, false},
                {false, false, false, false, false, false, false, false,true,  true},
                {false, false, false, false, false, false, true,  true, true,  true},
                {false, false, false, false, true,  true,  true,  true, true,  true},
                {false, false, true,  true,  true,  true,  true,  true, true,  true},
                {true,  true,  true,  true,  true,  true,  true,  true, true,  true}
         },{
                {true,  true,  true,  true,  true,  true,  true,  true,  true,  true},
                {false, false, true,  true,  true,  true,  true,  true,  true,  true},
                {false, false, false, false, true,  true,  true,  true,  true,  true},
                {false, false, false, false, false, false, true,  true,  true,  true},
                {false, false, false, false, false, false, false, false, true,  true},
                {false, false, false, false, false, false, false, false, false, false}
         }
    },
    false, {"lampa1", "lampa2", "lampa3", "lampa4", "lampa5", "lampa6", "lampa7", "lampa8", "lampa9", "lampa10"}, 0.1);

    Animation animacjaLazienka = Animation({animacjaInputLazienka}, 
    {lampa1, lampa2, lampa3, lampa4, lampa5, lampa6, lampa7, lampa8, lampa9, lampa10},
    {
         {
                {false, false, false, false, false, false, false, false,false, false},
                {false, false, false, false, true,  true,  false, false,false, false},
                {false, false, true,  true,  true,  true,  true,  true, false, false},
                {true,  true,  true,  true,  true,  true,  true,  true, true,  true}
         },{
                {true,  true,  true,  true,  true,  true,  true,  true,  true,  true},
                {false, false, true,  true,  true,  true,  true,  true,  false, false},
                {false, false, false, false, true,  true,  false, false, false, false},
                {false, false, false, false, false, false, false, false, false, false}
         }
    },
    false, {"lampa1", "lampa2", "lampa3", "lampa4", "lampa5", "lampa6", "lampa7", "lampa8", "lampa9", "lampa10"}, 0.1);

    Animation animacjaWentylator = Animation({wentylatorLazienkaInput},{wentylatorLazienka}, {{{false, true, false}}},false, {"wentylatorLazienka"}, 0.1);
    AnimationStore::getInstance()->addAnimations({animacjaKlatka, animacjaPs3, animacjaLazienka, animacjaWentylator});

    AnimationStore::getInstance()->addAnimationGroup(std::set<Animation*>{&animacjaKlatka, &animacjaPs3, &animacjaLazienka});
  //    Animation anim = Animation({input},{output},
  //  {
  //    {{true}, {false}, {true}, {false},{true}, {false}, {true},
  //    {false},{true}, {false}, {true}, {false}},
  //    {{true}, {true}, {true}, {false},{true}, {true}, {true}, {false},{true},
  //    {true}, {true}, {false}}
  //   },false, {"pierwsze"}, 0.5);
  //  AnimationStore::getInstance()->addAnimations({anim});
}

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