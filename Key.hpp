#pragma once

class Key_Class
{
public:

Key_Class(unsigned int Port);

void Init();

void process();

bool getEvent();

private:

enum Key_Enum
{
    FIRST_VALUE = 1,
    SECOND_VALUE
};

Key_Enum Switch;

bool LastKey;
unsigned long LastKeyInterval;
unsigned long PrellTimer;
const static unsigned long PrellTime = 50;
const static unsigned long KeyInterval = 30;
bool KeyEvent;
unsigned int Port_;
 
};

extern Key_Class Key_Object;
