#pragma once

#include <Arduino.h> 

class Dcc_Class
{
public:

      /**
            Constructor, stellt den Eingang für das DCC Signal zur Verfügung
      */
      Dcc_Class(unsigned int Input);
      
      /**
            Initialisiert den Flankeninterrupt und setzt den Startpunkt des DCC Empfangsautomaten
      */
      void init();

      /**
            Intzerruptroutine bei steigenden Flanken
      */
      static void RisingDcc();

      enum DccCommand_Enum
      {
            CommandIdle = 0,
            CommandOpen,
            CommandClose
      };

      /**
            Abfrage, ob Kommando fürSchranke empfangen wurde
      */
      bool getDccCommand(DccCommand_Enum & Cmd); 

      enum Switch_Enum
      {
            IDLE = 1,
            SEARCH_PREAMBEL,
            READ_DATA,
            LOW_BYTE,
            XOR_TEST
      };

     
private:

      // Port DCC Signal
      unsigned int Input_;

      // Null-bit: 150 bis 10000 µs
      static const unsigned long NullBitMin = 150;
      static const unsigned long NullBitMax = 10000;

      // Eins-Bit: 80 bis 130 µs
      static const unsigned long EinsBitMin = 80;
      static const unsigned long EinsBitMax = 130; 
      
      static unsigned long CurrentTimer_;
      static unsigned long LastTimer_;
      static const uint8_t RawDataMax = 10;
      static const uint16_t MinSyncBits = 10;

      static uint8_t RawData[RawDataMax];
      static uint16_t PreambelBitCount;
      static uint16_t DataIndex;
      static uint16_t BitPosition;
      static Switch_Enum Switch;
      static uint8_t XorTest;
      
      static DccCommand_Enum DccCommand;
};

extern Dcc_Class Dcc_Object;