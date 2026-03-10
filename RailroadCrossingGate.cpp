
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
      // erstes Bit S88 setzen
      S88_Object.setValue(true, 0x00001);
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
         
          S88_Object.setValue(true, 0x00002);
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
            Serial.println("Schranke schliesst");
        }
        break;
        
    // warten, bis die Schranken geschlossen sind 
    case START_CLOSE_BARRIER:
    if (Barrier1_Object.getState() == Barrier_Class::BARRIER_CLOSED)
    {
        Serial.println("Schranke ist geschlossen");   
        
        // Ton wieder ausschalten
        AudioIsd1730_Object.stopSound();

        // S88 Schrasnke gesclossen, zurück melden: die untersten 3 Bits sind geschlossen
        S88_Object.setValue(true, 0x0004);

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
            Serial.println("Schranke wird wieder geöffnet");
            Barrier1_Object.Open();
            Barrier2_Object.Open();

            // S88 Schrasnke gesclossen, zurück melden
            S88_Object.setValue(false, 0x0004);

            Switch = OPEN_BARRIER;
        }
      }
      break;
    
    case OPEN_BARRIER:
        if (Barrier1_Object.getState() == Barrier_Class::BARRIER_OPEN)
        {
          // rotes Licht aUSSCHALTEN
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
            S88_Object.setValue(false, 0x0002);
            Switch = IDLE;
        }
    break;
    
    default:
    break;
  }
};

RailroadCrossingGate_Class RailroadCrossingGate_Object;