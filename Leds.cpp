#include "api/Common.h"
#include "Leds.hpp"
#include <Arduino.h>

Leds_Class::Leds_Class(unsigned int PortLed1, unsigned int PortLed2, unsigned int PortLed3)
: PortLed1_(PortLed1)       // weisse LED Anode (schwarz)
, PortLed2_(PortLed2)       // rote und weisse LED Kathode (rot)
, PortLed3_(PortLed3)       // rote LED (grün)
{

}

 void Leds_Class::InitLeds()
 {
    // und auf 0 gesetzt
    pinMode(PortLed1_, OUTPUT);   // hier die weisse LED
    pinMode(PortLed2_, OUTPUT);   // hier die rote und die Weisse
    pinMode(PortLed3_, OUTPUT);   // hier nur die rote
    digitalWrite(PortLed1_, LOW);
    digitalWrite(PortLed2_, LOW);
    digitalWrite(PortLed3_, LOW);
    LedsState = OFF;
 }

 void Leds_Class::process()
 {
    switch (LedsState)
    {
      case ON_WITE:
          
          if (TriggerTime < millis())
          {
              // Led umschalten (P2)
              if (LedOnWhite == false)
              {
                  digitalWrite(PortLed1_, HIGH);
                  TriggerTime = millis() + WhiteLedOn;
                  LedOnWhite = true;
              }
              else
              {
                  digitalWrite(PortLed1_, LOW);
                  TriggerTime = millis() + WhiteLedOff;
                  LedOnWhite = false;
              }
          }
       break;
      case ON_RED:
        if (TriggerTime < millis())
        {
            if (LedOnRed == true)
            {
                digitalWrite(PortLed2_, LOW);
                digitalWrite(PortLed3_, HIGH);
                LedOnRed = false;
            }
            else 
            {
                digitalWrite(PortLed2_, HIGH);
                digitalWrite(PortLed3_, LOW);
                LedOnRed = true; 
            }
            //Serial.println("Red LEDs trigger");
            TriggerTime = millis() + RedLedsswitch;
        }
       break;

       default:
            // keine LED aktiv
            break;
    }
 }

void Leds_Class::setWhiteLedActive()
{
    // P3 und P4 auf 1
    digitalWrite(PortLed2_, LOW);
    digitalWrite(PortLed3_, LOW);
    // P2 auf Low, dann leuchtet sie
    digitalWrite(PortLed1_, HIGH);
    TriggerTime = millis() + WhiteLedOn;
    LedsState = ON_WITE;
    LedOnWhite = true;
}

void Leds_Class::setRedLedsActive()
{
    digitalWrite(PortLed1_, LOW);
    digitalWrite(PortLed2_, HIGH);
    digitalWrite(PortLed3_, LOW);
    LedOnRed = true;
    TriggerTime = millis() + RedLedsswitch;
    LedsState = ON_RED;
}

void Leds_Class::setLedsOff()
{
    digitalWrite(PortLed1_, LOW);
    digitalWrite(PortLed2_, LOW);
    digitalWrite(PortLed3_, LOW);
    LedsState = OFF;
}

Leds_Class Leds_Object1(A0, A1, A2);
Leds_Class Leds_Object2(A3, A4, A5);

