#pragma once

#include <Always.h>

#include <fstream>

class CFinalSunLog
{
public:
    static CFinalSunLog Instance;

    CFinalSunLog();
    ~CFinalSunLog();

    void Open(LPCSTR lpFile);
    void Close();

    template<typename T>
    CFinalSunLog& operator<<(T&& x);

    void Write(LPCSTR lpFormat, ...);
    void Log(LPCSTR lpString);

private:
    std::ofstream FileHandle;
    char Buffer[0x400];
};