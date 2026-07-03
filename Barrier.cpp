#include <EEPROM.h>
#include <Arduino.h>
#include "Barrier.hpp"

Barrier_Class::Barrier_Class(unsigned int Port, int EepromAdress, const int AngelPositionMin, const int AngelPositionMax)
: CurrentState(BARRIER_UNKNOWN)
, Port_(Port)
, EepromAddress_(EepromAdress)
, AngelPositionMin_(AngelPositionMin)
, AngelPositionMax_(AngelPositionMax)
{

}

void Barrier_Class::init()
{
    Position = EEPROM.read(EepromAddress_);
    Servo_.attach(Port_);
    Servo_.write(Position);
    Timer = millis() + Intervall;
    Serial.print(Position);
    Serial.println(" init");
    CurrentState = BARRIER_TO_OPEN;         // Zustand der Schranke offen
}

void Barrier_Class::process()
{
    if (millis() > Timer)
    {
        Timer = millis() + Intervall;
        switch (CurrentState)
        {
            // Schranke wird geöffnet
            case Barrier_Class::BARRIER_TO_OPEN:      
            if (Position > AngelPositionMax_)          // entspricht von 0 nach 90 Grad
            {
                Servo_.write(--Position);
            }
            else
            {
                CurrentState = Barrier_Class::BARRIER_OPEN;
                EEPROM.write(EepromAddress_, Position);
                Serial.print(Position);
                Serial.println(" Open");
            }
            break;

            // Schranke wird geschlossen
            case BARRIER_TO_CLOSE:                     
            if (Position < AngelPositionMin_)
            {
                Servo_.write(++Position);
            }
            else
            {
                CurrentState = BARRIER_ROCK;
                RockIndex = 0;
            }
            break;

            // Wippen der Schranke am Ende des Schliessens
            case BARRIER_ROCK:
                if (RockIndex < RockIndexMax)
                {
                    Servo_.write(AngelPositionMin_ + RockValues[RockIndex++]);
                }
                else
                {
                    CurrentState = BARRIER_CLOSED;
                    EEPROM.write(EepromAddress_, Position);
                    Serial.print(Position);
                    Serial.println(" Close");
                }
            break;

            default:
            break;
        }
    }
}

Barrier_Class::Barrier_Enum Barrier_Class::getState()
{
    return CurrentState;
}

void Barrier_Class::open()
{
    if ((CurrentState != Barrier_Class::BARRIER_TO_OPEN) && (CurrentState != Barrier_Class::BARRIER_OPEN))
    {
        CurrentState = Barrier_Class::BARRIER_TO_OPEN;
    }
}

void Barrier_Class::close()
{
    if ((CurrentState != Barrier_Class::BARRIER_TO_CLOSE) && (CurrentState != Barrier_Class::BARRIER_CLOSED))
    {
        CurrentState = Barrier_Class::BARRIER_TO_CLOSE;
    }
}

static const unsigned int PositionMin_1 = 70; // Winkel des Servos - Schranke offen
static const unsigned int PositionMax_1 = 35; // Winkel des Servos - Schranke zu 
Barrier_Class Barrier1_Object(10, 1, PositionMin_1, PositionMax_1);

static const unsigned int PositionMin_2 = 70; // Winkel des Servos - Schranke offen
static const unsigned int PositionMax_2 = 35; // Winkel des Servos - Schranke zu 
Barrier_Class Barrier2_Object(5, 2, PositionMin_2, PositionMax_2);
