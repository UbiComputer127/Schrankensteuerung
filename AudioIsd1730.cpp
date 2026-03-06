#include "AudioIsd1730.hpp"
#include <Arduino.h>

AudioIsd1730_Class::AudioIsd1730_Class(unsigned int PortRecord, unsigned int PortPlay, unsigned int PortReset, unsigned int PortErase)
: PortRecord_(PortRecord)
, PortPlay_(PortPlay)
, PortReset_(PortReset)
, PortErase_(PortErase)
, AudioState(IDLE)
{

}

void AudioIsd1730_Class::Init()
{
    // Ports einstellen als Ausgaben
    pinMode(PortRecord_, OUTPUT);     // hier die weisse LED
    pinMode(PortPlay_, OUTPUT);       // hier die rote und die Weisse
    pinMode(PortReset_, OUTPUT);      // hier nur die rote
    pinMode(PortErase_, OUTPUT);
    digitalWrite(PortRecord_, HIGH);
    digitalWrite(PortPlay_, HIGH);
    digitalWrite(PortReset_, HIGH);
    digitalWrite(PortErase_, HIGH);

    resetDevice();
}

void AudioIsd1730_Class::process()
{
    switch (AudioState)
    {
        case IDLE:
        break;
        case PLAY:
        break;
        case RESET:
          if (millis() > ResetTimer)
          {
              digitalWrite(PortReset_, HIGH);
              AudioState = IDLE;
          }
        break;
        case RECORD:
        break;
        case ERASE:
        break;
        default:
        break;
    }

}

void AudioIsd1730_Class::resetDevice()
{
    digitalWrite(PortReset_, LOW);
    ResetTimer = millis() + ResetTime;
    AudioState = RESET;
}

void AudioIsd1730_Class::playSound()
{
    digitalWrite(PortPlay_, LOW);
    AudioState = PLAY;
}

void AudioIsd1730_Class::stopSound()
{
    digitalWrite(PortPlay_, HIGH);
    AudioState = IDLE;
}

void AudioIsd1730_Class::record()
{

}

void AudioIsd1730_Class::erase()
{


}

AudioIsd1730_Class AudioIsd1730_Object(6, 8, 9, 10);