#include <CFinalSunLog.h>

#include <stdarg.h>

CFinalSunLog CFinalSunLog::Instance;

CFinalSunLog::CFinalSunLog()
{
}

CFinalSunLog::~CFinalSunLog()
{
}

void CFinalSunLog::Open(LPCSTR lpFile)
{
    this->FileHandle.open(lpFile, std::ios::out | std::ios::trunc);
}

void CFinalSunLog::Close()
{
    this->FileHandle.flush();
    this->FileHandle.close();
}

void CFinalSunLog::Write(LPCSTR lpFormat, ...)
{
    va_list args;
    va_start(args, lpFormat);
    vsprintf_s(this->Buffer, lpFormat, args);
    *this << this->Buffer;
    va_end(lpFormat);
}

void CFinalSunLog::Log(LPCSTR lpString)
{
    *this << lpString;
}

template<typename T>
inline CFinalSunLog& CFinalSunLog::operator<<(T&& x)
{
    if (this->FileHandle.is_open())
        this->FileHandle << x;
    return *this;
}