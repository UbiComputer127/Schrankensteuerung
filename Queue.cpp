

#include "Queue.hpp"

template<typename T, size_t Size> 
bool Queue_Class<T, Size>::PutData(T & Value)
{
    bool Return = false;
    if (Len < Size)
    {
        Data[WriteIndex++] = Value;
        Len++;
        Return = true;
    }
    return Return;
}

template<typename T, size_t Size> 
bool Queue_Class<T, Size>::GetData(T & Value)
{
    bool Return = false;
    if (Len > 0)
    {
        noInterrupts();
        Value = Data[ReadIndex--];
        Len--;
        interrupts();
        Return = true;
    } 
    return Return;
}
    
    