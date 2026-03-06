#include <EEPROM.h>
#include <Arduino.h>
#include "Barrier.hpp"

Barrier_Class::Barrier_Class(unsigned int Port, int EepromAdress)
: CurrentState(BARRIER_UNKNOWN)
, Port_(Port)
, EepromAddress_(EepromAdress)
{

}

void Barrier_Class::Init()
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
            case Barrier_Class::BARRIER_TO_OPEN:       // entspricht von 90 nach 0 Grad
            if (Position > PositionMin)
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

            case BARRIER_TO_CLOSE:                      // entspricht von 0 nach 90 Grad
            if (Position < PositionMax)
            {
                Servo_.write(++Position);
            }
            else
            {
                CurrentState = BARRIER_ROCK;
                RockIndex = 0;
            }
            break;

            case BARRIER_ROCK:
                if (RockIndex < RockIndexMax)
                {
                    Servo_.write(PositionMax - RockValues[RockIndex++]);
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

void Barrier_Class::Open()
{
    if ((CurrentState != Barrier_Class::BARRIER_TO_OPEN) && (CurrentState != Barrier_Class::BARRIER_OPEN))
    {
        CurrentState = Barrier_Class::BARRIER_TO_OPEN;
    }
}

void Barrier_Class::Close()
{
    if ((CurrentState != Barrier_Class::BARRIER_TO_CLOSE) && (CurrentState != Barrier_Class::BARRIER_CLOSED))
    {
        CurrentState = Barrier_Class::BARRIER_TO_CLOSE;
    }
}

Barrier_Class Barrier1_Object(10, 1);
Barrier_Class Barrier2_Object(5, 2);
