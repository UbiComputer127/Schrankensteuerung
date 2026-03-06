#include <Arduino.h>
#include "Key.hpp"

Key_Class::Key_Class(unsigned int Port)
: Port_(Port)
{

}

void Key_Class::Init()
{
    pinMode(Port_, INPUT);   // hier Eingang für Taste
    LastKeyInterval = millis() + KeyInterval;
    LastKey = false;
    KeyEvent = false;
    Switch = FIRST_VALUE;
}

void Key_Class::process()
{
    if (LastKeyInterval <= millis())
    {
        LastKeyInterval = millis() + KeyInterval;
        switch(Switch)
        {
            case FIRST_VALUE:
            {
                bool Value = digitalRead(Port_);
                if (Value != LastKey)
                {
                    // Taste hat sich geändert, Entprellen starten
                    PrellTimer = millis() + PrellTime;
                    LastKey = Value;
                    Switch = SECOND_VALUE;     
                } 
            }   
            break;
            case SECOND_VALUE:
                if (millis() > PrellTimer)
                {
                    if (digitalRead(Port_) == LastKey)
                    {
                        // Taste jetzt entprelled
                        if (LastKey == false)
                        {
                            // Tastenevent auslösen
                            KeyEvent = true;
                            Serial.println("Key Pressed");
                        }
                    }
                    Switch = FIRST_VALUE;
                }
            break; 
        }                  
    }
}

bool Key_Class::getEvent()
{
    bool Return = false;
    if (KeyEvent == true)
    {
        KeyEvent = false;
        Return = true;
    }
    return Return;
}

Key_Class Key_Object(D0);