
#include "RailroadCrossingGate.hpp"
#include "Leds.hpp"
#include "Key.hpp"
#include "Barrier.hpp"
#include "AudioIsd1730.hpp"
#include "Dcc.hpp"
#include "s88.hpp"

void setup()
{
    Serial.begin();
    Leds_Object1.InitLeds();
    Leds_Object2.InitLeds();
    RailroadCrossingGate_Object.Init();
    Barrier1_Object.Init();
    Barrier2_Object.Init();
    Key_Object.Init();
    S88_Object.init();
    Dcc_Object.init();
}

void loop()
{
    RailroadCrossingGate_Object.process();
    Leds_Object1.process();
    Leds_Object2.process();
    Key_Object.process();
    Barrier1_Object.process();
    Barrier2_Object.process();
    AudioIsd1730_Object.process();
}
