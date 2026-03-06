#include "Dcc.hpp"
#include <Arduino.h>

Dcc_Class::Switch_Enum Switch = Dcc_Class::IDLE;
uint8_t Dcc_Class::RawData[] = {0,0,0,0,0,0,0,0,0,0};
uint16_t Dcc_Class::PreambelBitCount = 0;
uint16_t Dcc_Class::DataIndex = 0;
uint16_t Dcc_Class::BitPosition = 0;
uint8_t Dcc_Class::XorTest = 0;
unsigned long Dcc_Class::CurrentTimer_ = 0;
unsigned long Dcc_Class::LastTimer_ = 0;
Dcc_Class::Switch_Enum Dcc_Class::Switch = Dcc_Class::IDLE;
Dcc_Class::DccCommand_Enum Dcc_Class::DccCommand = Dcc_Class::CommandIdle;

Dcc_Class::Dcc_Class(unsigned int Input)
: Input_(Input)
{
    
}

void Dcc_Class::init()
{
    pinMode(Input_, INPUT);
    attachInterrupt(digitalPinToInterrupt(Input_), Dcc_Class::RisingDcc, RISING);
    Switch = SEARCH_PREAMBEL;
}

void Dcc_Class::RisingDcc()
{
    // steigende Flanke
    CurrentTimer_ = micros();
    unsigned long Diff = CurrentTimer_ - LastTimer_;
    LastTimer_ = CurrentTimer_;
    bool Bit = false;

    // Test Eins Bit
    if ((Diff >= EinsBitMin) && (Diff <= EinsBitMax))
    {
        //  Bit ist 1
        Bit = true;
    }
    // Test Null Bit
    else if ((Diff >= NullBitMin) && (Diff <= NullBitMax))
    {
        // Bit ist eine Null
        Bit = false;
    }
    else
    {
        // ungültiger Wert: verwerfen
        return;
    }

    switch (Switch)
    {
        case IDLE:
            Switch = SEARCH_PREAMBEL; 
        break;
        
        case SEARCH_PREAMBEL:
            if (Bit)
            {
                // Eins-Bit
                PreambelBitCount++;
            }
            else
            {
                // Null-Bit
                if (PreambelBitCount >= MinSyncBits)
                {
                    // minimale Einsbits wurden empfangen
                    DataIndex = 0;
                    BitPosition = 8;
                    XorTest = 0;
                    // alle Bits Null setzen
                    for (int i=0; i<RawDataMax; i++)
                    {
                        RawData[i] = 0;
                    }
                    Switch = READ_DATA;
                }
                else
                {
                    PreambelBitCount = 0;
                }
            }
        break;

        case READ_DATA:
            // Bit setzen
            BitPosition--;
            if (Bit)
            {
                RawData[DataIndex] |= 1 << BitPosition;
            }

            if (BitPosition == 0)
            {
                DataIndex++;
                BitPosition = 8;
                Switch = LOW_BYTE;
            }
            break;
        
        case LOW_BYTE:
            if (Bit)
            {
                // Ende der Message
                Switch = XOR_TEST;
            }
            else
            {
                // nächstes Byte einlesen
                Switch = READ_DATA;
            }
            break;
        
        case XOR_TEST:
            {
                // Zubehörkommandos ausfiltern 
                uint8_t Cmd = RawData[0];
                if ((Cmd & 0xC0) == 0x80)
                {
                    // 3 Bit der Adresse invertieren
                    RawData[1] ^= 0x70; 

                    // Test des Kommandos für Schranke auf
                    if ((RawData[0] == 0x99) && (RawData[1] == 0x8F))
                    {
                        DccCommand = CommandOpen;
                    }
                    else if ((RawData[0] == 0x99) && (RawData[1] == 0x8E))
                    {
                        DccCommand = CommandClose;
                    }
                    else
                    {

                    }
                } 
                Switch = SEARCH_PREAMBEL;
            }
            break;        
    }
}

bool Dcc_Class::getDccCommand(DccCommand_Enum & Cmd)
{
    Cmd = DccCommand;
    if (DccCommand != CommandIdle)
    {
        DccCommand = CommandIdle;
        return true;
    }
    else
    {
        return false; 
    }
}

Dcc_Class Dcc_Object(D2);

