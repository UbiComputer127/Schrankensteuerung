
#include <Arduino.h>
#include "RailroadCrossingGate.hpp"
#include "Key.hpp"
#include "Leds.hpp"
#include "Barrier.hpp"
#include "AudioIsd1730.hpp"
#include "S88.hpp"
#include "Dcc.hpp"

void RailroadCrossingGate_Class::Init()
{
    Switch = SETUP;
}

void RailroadCrossingGate_Class::process()
{
  switch(Switch)
  {
    case SETUP:
      Leds_Object1.setWhiteLedActive();
      Leds_Object2.setWhiteLedActive();
      Switch = IDLE;
    break;

    case IDLE:
    {
      Dcc_Class::DccCommand_Enum Cmd;
      Dcc_Object.getDccCommand(Cmd);
      if (Key_Object.getEvent() || (Cmd == Dcc_Class::DccCommand_Enum::CommandClose))
      {
          // Zug kommt, Schranke schliessen
          Leds_Object1.setWhiteLedSmartOff(); // Weisse LED ausschalten
          Leds_Object2.setWhiteLedSmartOff(); // Weisse LED ausschalten
          Switch = START_RED_LIGHT;
      }
    }
    break;

    case START_RED_LIGHT:
    // warten, bis weisse Leds aus sind
    if ((Leds_Object1.getLedsState() == Leds_Class::OFF) && (Leds_Object2.getLedsState() == Leds_Class::OFF))
    {
        // rote LEDs leuchten
        Leds_Object1.setRedLedsActive();
        Leds_Object2.setRedLedsActive();
        
        // Ton der Glocke starten
        AudioIsd1730_Object.playSound();

        Timer = millis() + TimeToCloseBatrrier;
        Switch = WAIT_TO_CLOSE_BARRIER;
    }
    break;

    case WAIT_TO_CLOSE_BARRIER:
        if (millis() > Timer)  
        {
            Barrier1_Object.Close();
            Barrier2_Object.Close();
            Switch = START_CLOSE_BARRIER;
        }
        break;
        
    // warten, bis die Schranken geschlossen sind 
    case START_CLOSE_BARRIER:
    if (Barrier1_Object.getState() == Barrier_Class::BARRIER_CLOSED)
    {   
        // Ton wieder ausschalten
        AudioIsd1730_Object.stopSound();

        // S88 Schrasnke gesclossen, zurück melden: das niederwertigste Bit wird auf 1 gesetzt
        S88_Object.setValue(true, 0x0001);

        Switch = CLOSE_BARRIER;
    }  
    break;

    // Schranken sind geschlossen
    case CLOSE_BARRIER:
      {
        // Wareten auf Zug
        Dcc_Class::DccCommand_Enum Cmd;
        Dcc_Object.getDccCommand(Cmd);
        if (Key_Object.getEvent() || (Cmd == Dcc_Class::DccCommand_Enum::CommandOpen))
        {
            // Zug ist vorbei
            Barrier1_Object.Open();
            Barrier2_Object.Open();
            Switch = OPEN_BARRIER;
        }
      }
      break;
    
    case OPEN_BARRIER:
        if (Barrier1_Object.getState() == Barrier_Class::BARRIER_OPEN)
        {
          // rotes Licht ausschalten
          Leds_Object1.setRedLedSmartOff();
          Leds_Object2.setRedLedSmartOff();
          Switch = WAIT_RED_LEDS_OFF;   
        }
    break;

    case WAIT_RED_LEDS_OFF:
        if ((Leds_Object1.getLedsState() == Leds_Class::OFF) && (Leds_Object2.getLedsState() == Leds_Class::OFF))
        {
            Leds_Object1.setWhiteLedActive();
            Leds_Object2.setWhiteLedActive();
            // über S88 signalisieren, dass Schranke wieder offen ist
            S88_Object.setValue(false, 0x0001);
            Switch = IDLE;
        }
    break;
    
    default:
    break;
  }
};

RailroadCrossingGate_Class RailroadCrossingGate_Object;