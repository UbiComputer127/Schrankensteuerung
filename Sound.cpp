#include "Sound.hpp"
#include <Arduino.h>
#include "GlockeKurz.hpp" 

Sound_Class::Sound_Class()
{
    
}

// callback method used by timer
void Sound_Class::timer_callback(timer_callback_args_t __attribute((unused)) *p_args)
{
    /*
    // Testdaten
    analogWrite(DAC, pgm_read_byte(&AmplifierTest[AmplifierTestIndex++]));
    
    if (AmplifierTestIndex >= sizeof(AmplifierTest))
    {
        AmplifierTestIndex = 0;
    } 
    */
    //  Audiodaten
    // Wert ausgeben zum DAC
    analogWrite(DAC, pgm_read_byte(&AudioDate[DateIndex++]));
    
    if (DateIndex >= sizeof(AudioDate))
    {
        DateIndex = 0;
    } 
}

bool Sound_Class::initAudioTimer(float Rate)
{
    bool Return = false;
    uint8_t timer_type = GPT_TIMER;
    int8_t tindex = FspTimer::get_available_timer(timer_type);
    if (tindex < 0)
    {
        // Fehler: nichts tun, Return ist schon false

    }
    else
    {

    }

    FspTimer::force_use_of_pwm_reserved_timer();

    if(!SoundTimer.begin(TIMER_MODE_PERIODIC, timer_type, tindex, Rate, 0.0f, Sound_Class::timer_callback))
    {
        return false;
    }

    if (!SoundTimer.setup_overflow_irq())
    {
        return false;
    }

    if (!SoundTimer.open())
    {
        return false;
    }

    return true;
}

void Sound_Class::Init()
{
    // Ports einstellen als Ausgaben
    analogWriteResolution(8); // 8-bit: values 0–255
    
    // Timer
    initAudioTimer(8000); 


}

void Sound_Class::playSound()
{
    // Index der Soundwerte auf 0 setzen
    DateIndex = 0;  
    SoundTimer.start();
}

void Sound_Class::stopSound()
{
    SoundTimer.stop();
    // Test, Ausgabe 0 zum DAC
    analogWrite(DAC, 0);      // Output near 0V on A0
}

Sound_Class Sound_Object;
