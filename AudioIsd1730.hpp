#pragma once

class AudioIsd1730_Class
{
    public:
    
    AudioIsd1730_Class(unsigned int PortRecord, unsigned int PortPlay, unsigned int PortReset, unsigned int PortErase);

    void Init();

    void process();

    void resetDevice();

    void playSound();
    void stopSound();

    void record();

    void erase();

    private:

    enum AudioState_Enum
    {
        IDLE = 1,
        RESET,
        PLAY,
        RECORD,
        ERASE
    };

    AudioState_Enum AudioState;

    unsigned int PortRecord_;
    unsigned int PortPlay_;
    unsigned int PortReset_;
    unsigned int PortErase_;

    unsigned long ResetTimer;
    static const unsigned long ResetTime = 2;

};

extern AudioIsd1730_Class AudioIsd1730_Object;