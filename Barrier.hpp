#pragma once

#include <Servo.h>
#include <Arduino.h> 

class Barrier_Class
{

public:

// Zustand der Schranke
enum Barrier_Enum
{
    BARRIER_UNKNOWN = 1,    // unbekannt, beim Einschalten der Steuerung
    BARRIER_OPEN,           // Schranke ist offen
    BARRIER_CLOSED,         // Schranke ist geschlossen
    BARRIER_TO_OPEN,        // Öffnen der Schranke
    BARRIER_TO_CLOSE,       // Schliessen der Schranke
    BARRIER_ROCK            // Wippen der Schranke am Ende des Schliessens
};

  // Konstruktor
  // Port: Controlport für Servo
  // EepromAddress: Merker für Zustand der Schranke  
  Barrier_Class(unsigned int Port, const int EepromAdress, const int AngelPositionMin, const int AngelPositionMax);

  // Initiaylisierung
  void init();

  // hzyklisc he Triggedrung der Steuerung
  void process();

  // augenblicklicher Zustand der Schranke
  Barrier_Enum getState(); 

  // Kommando zum Öffnen der Schranke
  void open();

  // Kommando zum Schliessen derf Schranke
  void close();

private:

int ReadEepromInt(int Address);
void WriteEepromInt(int Address, int Value);

unsigned int Port_;
Barrier_Enum CurrentState;
Servo Servo_;

const int AngelPositionMinMicroseconds_;
const int AngelPositionMaxMicroseconds_;

unsigned long Timer;
int ServoMicroSeconds;

static const unsigned long NormalTime = 30;
static const unsigned long RockerTime = 60;

unsigned long Intervall = 30;    // Timer wird an Hand des Zustands gesetzt
static const unsigned int RockIndexMax = 10; 
const uint8_t RockValues[RockIndexMax]={10, 15, 20, 15, 10, 0, 10, 15, 10, 0};
unsigned int RockIndex;
const int EepromAddress_;
};

extern Barrier_Class Barrier1_Object;
extern Barrier_Class Barrier2_Object;
