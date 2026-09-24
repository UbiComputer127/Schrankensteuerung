
#include "RailroadCrossingGate.hpp"
#include "Leds.hpp"
#include "Key.hpp"
#include "Barrier.hpp"
#include "Sound.hpp"
#include "Dcc.hpp"
#include "s88.hpp"
#include "TestLed.hpp"

void setup()
{
    Serial.begin();
    Leds_Object1.InitLeds();
    RailroadCrossingGate_Object.Init();
    Barrier1_Object.init();
    Barrier2_Object.init();
    Key_Object.Init();
    S88_Object.init();
    Dcc_Object.init();
    Sound_Object.Init();
    TestLed_Object.init();
}

void loop()
{
    RailroadCrossingGate_Object.process();
    Leds_Object1.process();
    Key_Object.process();
    Barrier1_Object.process();
    Barrier2_Object.process();
    TestLed_Object.process();
    Sound_Object.process();
}
