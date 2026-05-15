#pragma once
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "FspTimer.h"  // Arduino UNO R4 timer API

class Sound_Class
{
    public:
    
    Sound_Class();

    void Init();

    void playSound();
    void stopSound();

    private:

    static void timer_callback(timer_callback_args_t __attribute((unused)) *p_args);
    
    // Create a hardware timer (Timer 0 in this case)
    FspTimer SoundTimer;                    // Timer object

    // Initialisierung des Timers 8000Hz (125µs Takt)
    bool initAudioTimer(float Rate);

};

extern Sound_Class Sound_Object;