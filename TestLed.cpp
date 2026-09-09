 #include "TestLed.hpp" 
 #include <Arduino.h>
 
 TestLed_Class::TestLed_Class(int Port)
 : Port_(Port)
 , LedIsBlinking_(false)
 , PortIsOn_(false)
 , Timer_(0)
 {

 }

void TestLed_Class::init()
{
    pinMode(Port_, OUTPUT);       // hier die weisse LED
    digitalWrite(Port_, LOW);     // alle Ports 0, dann LEDs aus
}

void TestLed_Class::process()
{
    if (LedIsBlinking_)
    {
        if (millis() >= Timer_)
        {
            // Jetzt Port wechseln
            if (PortIsOn_)
            {
                // LED is on -> swoitch to off
                digitalWrite(Port_, LOW);
            }
            else
            {
                // LED is off -> swoitch to on
                digitalWrite(Port_, HIGH);
            }
            PortIsOn_ = !PortIsOn_;
            Timer_ = millis() + Time_;
        }
    }
}

void TestLed_Class::LedOff()
{
    LedIsBlinking_ = false;
    PortIsOn_ = false;
    digitalWrite(Port_, LOW);
}

void TestLed_Class::LedBlinking()
{
    LedIsBlinking_ = true;
    digitalWrite(Port_, HIGH);
    PortIsOn_ = true;
    Timer_ = millis() + Time_;
}

void TestLed_Class::LedOn()
{
    LedIsBlinking_ = false;
    PortIsOn_ = true;
    digitalWrite(Port_, HIGH);
}

TestLed_Class TestLed_Object(A4);