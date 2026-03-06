#pragma once

#include <Arduino.h> 

template<typename T, size_t Size> 
class Queue_Class
{
    public:

    bool PutData(T & Value);
    bool GetData(T & Value);
    
    private:

    T Data[Size];
    int Len;
    int WriteIndex;
    int ReadIndex;
};