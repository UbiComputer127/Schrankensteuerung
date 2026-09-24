#pragma once
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "FspTimer.h"  // Arduino UNO R4 timer API


/**
    Soundausgabe zum Schleissen der Schranke
    Es wird alle 125µs ein Wert über den DAC ausgegeben.
    Der ausgegbene Wert ist ein 8 Bit Wert.
*/
class Sound_Class
{
    public:
    
    /**
        Konstruktor
    */
    Sound_Class();

    /**
        Einstellen des DAC und Timers
    */
    void Init();

    /**
        Sound starten
    */
    void playSound();
   
    /**
        Sound stoppen
    */
    void stopSound();

    /**
        Sound stoppen aber warten bis letzte Glocke vorbei ist
    */
    void stopSoundWait();

    /**
        Test, ob Sound wirklich fertig ist
    */
    bool SoundStopIsReady();

    /**
        Methode in Hauptschleife
        wird nur zum Ausschalten des Sound benutzt
    */
    void process();


    private:

    enum StopSound_Enum
    {
        Idle = 0,           // Sound nicht aktiv
        SoundRunning,       // Sound läuft
        SoundShouldStop,    // Sound soll gestoppt werden Message aus Hauptschleife
        SoundReadStop       // setzen in Timerinterrupt, wenn Schlaife zu Ende ist 
    };

    // Flag zum Ausschalten
    static StopSound_Enum SoundStop;

    static void timer_callback(timer_callback_args_t __attribute((unused)) *p_args);
    
    // Create a hardware timer (Timer 0 in this case)
    FspTimer SoundTimer;                    // Timer object

    // Initialisierung des Timers 8000Hz (125µs Takt)
    bool initAudioTimer(float Rate);

};

extern Sound_Class Sound_Object;