
#include <Arduino.h>
#include "RailroadCrossingGate.hpp"
#include "Key.hpp"
#include "Leds.hpp"
#include "Barrier.hpp"
#include "Sound.hpp"
#include "S88.hpp"
#include "Dcc.hpp"
#include "TestLed.hpp"

void RailroadCrossingGate_Class::Init()
{
    Switch = SETUP;
}

void RailroadCrossingGate_Class::process()
{
  switch(Switch)
  {
    // initialisieren der Schranke nach einschalten des Stromes
    case SETUP:
        TestLed_Object.LedOn();
        Leds_Object1.setWhiteLedActive();
        Switch = IDLE;
    break;


    // Schranke ist geöffnet
    case IDLE:
    {
      Dcc_Class::DccCommand_Enum Cmd;
      Dcc_Object.getDccCommand(Cmd);
      if (Key_Object.getEvent() ||
         (Cmd == Dcc_Class::DccCommand_Enum::CommandClose))
      {
          // Zug kommt, Schranke schliessen
          Leds_Object1.setWhiteLedSmartOff(); // Weisse LED ausschalten
          TestLed_Object.LedBlinking();
          Switch = START_RED_LIGHT;
      }
    }
    break;

    // Zug kommt bald - Seisse LED aus und rote LEDs einschaltens
    case START_RED_LIGHT:
    // warten, bis weisse Leds aus sind
    if (Leds_Object1.getLedsState() == Leds_Class::OFF)
    {
        // rote LEDs leuchten
        Leds_Object1.setRedLedsActive();
        
        // Ton der Glocke starten
        Sound_Object.playSound();

        Timer = millis() + TimeToCloseBatrrier;
        Switch = WAIT_TO_CLOSE_BARRIER;
    }
    break;

    // Schranken schliessen
    case WAIT_TO_CLOSE_BARRIER:
        if (millis() > Timer)  
        {
            Barrier1_Object.close();
            Barrier2_Object.close();
            Switch = START_CLOSE_BARRIER;
        }
        break;
        
    // warten, bis die Schranken geschlossen sind 
    case START_CLOSE_BARRIER:
    if ((Barrier1_Object.getState() == Barrier_Class::BARRIER_CLOSED) &&
        (Barrier2_Object.getState() == Barrier_Class::BARRIER_CLOSED))
    {   
        // Ton wieder ausschalten
        Sound_Object.stopSoundWait();

        // S88 Schrasnke gesclossen, zurück melden: das niederwertigste Bit wird auf 1 gesetzt
        S88_Object.setValue(true, 0x0001);

        Switch = WAIT_SOUND_IS_OFF;
    }  
    break;

    // warten, bis der Klingelton zu Ende ist
    case WAIT_SOUND_IS_OFF:
      if (Sound_Object.SoundStopIsReady() == true)
      {
          Switch = CLOSE_BARRIER;
      }
    break;

    // Schranken sind geschlossen
    case CLOSE_BARRIER:
      {
        // Wareten auf Zug
        Dcc_Class::DccCommand_Enum Cmd;
        Dcc_Object.getDccCommand(Cmd);
        if (Key_Object.getEvent() || 
           (Cmd == Dcc_Class::DccCommand_Enum::CommandOpen))
        {
            // Zug ist vorbei
            Barrier1_Object.open();
            Barrier2_Object.open();
            Switch = OPEN_BARRIER;
        }
      }
      break;

    // Zug ist vorbei, Schranken wieder öffnen und warten, bis Schranken oben sind
    case OPEN_BARRIER:
        if ((Barrier1_Object.getState() == Barrier_Class::BARRIER_OPEN) &&
            (Barrier2_Object.getState() == Barrier_Class::BARRIER_OPEN))
        {
          // rotes Licht ausschalten
          Leds_Object1.setRedLedSmartOff();
          Switch = WAIT_RED_LEDS_OFF;
          Timer = millis() + TimeWaitToWhiteLed;
            
        }
    break;

    case WAIT_RED_LEDS_OFF:
        if (Timer < millis())
        {
          if (Leds_Object1.getLedsState() == Leds_Class::OFF)
          {
              Leds_Object1.setWhiteLedActive();
              // über S88 signalisieren, dass Schranke wieder offen ist
              S88_Object.setValue(false, 0x0001);
              TestLed_Object.LedOn();
              Switch = IDLE;
          }
        }
    break;
    
    default:
    // do nothing
    break;
    }
};

RailroadCrossingGate_Class RailroadCrossingGate_Object;