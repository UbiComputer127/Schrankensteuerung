#include <EEPROM.h>
#include <Arduino.h>
#include "Barrier.hpp"

Barrier_Class::Barrier_Class(unsigned int Port, int EepromAdress, const int AngelPositionMin, const int AngelPositionMax)
: CurrentState(BARRIER_UNKNOWN)
, Port_(Port)
, EepromAddress_(EepromAdress)
, AngelPositionMinMicroseconds_(AngelPositionMin)   // Schranke unten/geschlossen
, AngelPositionMaxMicroseconds_(AngelPositionMax)   // Schranke oben/offen
{
   
}

void Barrier_Class::init()
{
    ServoMicroSeconds = ReadEepromInt(EepromAddress_);
    // ServoMicroSeconds = 1500;
    Servo_.attach(Port_);
    Servo_.writeMicroseconds(ServoMicroSeconds);
    Intervall = NormalTime;
    Timer = millis() + Intervall;
    Serial.print("aktuelle Pos: ");
    Serial.println(ServoMicroSeconds);
    Serial.print("Min: ");
    Serial.println(AngelPositionMinMicroseconds_);
    Serial.print("Max: ");
    Serial.println(AngelPositionMaxMicroseconds_);

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
            if (ServoMicroSeconds > AngelPositionMaxMicroseconds_)          // entspricht von 0 nach 90 Grad
            {
                ServoMicroSeconds = ServoMicroSeconds - 3;
                Servo_.writeMicroseconds(ServoMicroSeconds);
            }
            else
            {
                CurrentState = Barrier_Class::BARRIER_OPEN;
                WriteEepromInt(EepromAddress_, ServoMicroSeconds);
                Serial.print(ServoMicroSeconds);
                Serial.println(" Open");
                Serial.print("Min: ");
                Serial.println(AngelPositionMinMicroseconds_);
                Serial.print("Max: ");
                Serial.println(AngelPositionMaxMicroseconds_);
            }
            break;

            // Schranke wird geschlossen
            case BARRIER_TO_CLOSE:                     
            if (ServoMicroSeconds < AngelPositionMinMicroseconds_)
            {
                ServoMicroSeconds = ServoMicroSeconds + 3;
                Servo_.writeMicroseconds(ServoMicroSeconds);
            }
            else
            {
                CurrentState = BARRIER_ROCK;
                RockIndex = 0;
                Intervall = RockerTime;
            }
            break;

            // Wippen der Schranke am Ende des Schliessens
            case BARRIER_ROCK:
                if (RockIndex < RockIndexMax)
                {
                    Servo_.writeMicroseconds(AngelPositionMinMicroseconds_ - RockValues[RockIndex++]);
                }
                else
                {
                    CurrentState = BARRIER_CLOSED;
                    WriteEepromInt(EepromAddress_, ServoMicroSeconds);
                    Intervall = NormalTime;
                    Serial.print(ServoMicroSeconds);
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

// auf Address liegt der höherwertige Teil von Values
// auf Address+1 liegt der niederwertige Teil von Value
int Barrier_Class::ReadEepromInt(int Address)
{
    int ReturnValue;
    ReturnValue =  EEPROM.read(Address) << 8;
    ReturnValue += EEPROM.read(Address + 1);

    Serial.print("ReadEeprom ");
    Serial.print(Address);
    Serial.print(" ");
    Serial.println(ReturnValue);

    return ReturnValue;
}

void Barrier_Class::WriteEepromInt(int Address, int Value)
{
    EEPROM.write(Address, Value >> 8 );         // höherwertiger Anteil nach Address
    EEPROM.write(Address + 1, Value & 0x00FF);  // niederwertige Anteil nach Address+1

    Serial.print("WriteEeprom ");
    Serial.print(Address);
    Serial.print(" ");
    Serial.println(Value);

}


static const unsigned int PositionMin_1 = 1650; // Winkel des Servos - Schranke geschlossen
static const unsigned int PositionMax_1 = 1250; // Winkel des Servos - Schranke geöffnet 
Barrier_Class Barrier1_Object(10, 1, PositionMin_1, PositionMax_1);

static const unsigned int PositionMin_2 = 1650; // Winkel des Servos - Schranke geschlossen
static const unsigned int PositionMax_2 = 1250; // Winkel des Servos - Schranke geöffnet 
Barrier_Class Barrier2_Object(5, 3, PositionMin_2, PositionMax_2);
