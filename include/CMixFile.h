#pragma once

#include <Always.h>

#include <vector>

// Codes mainly from XCC
// References from CnC_Remastered_Collection

struct CMixBlock
{
    int CRC;    // CRC code for embedded file.
    int Offset; // Offset from start of data section.
    int Size;   // Size of data subfile.

    int operator<=>(CMixBlock& another) const { return CRC - another.CRC; }
};

class CMixFile
{
public:
    CMixFile(char* buffer);

    static int CreateInstance(char* buffer);
    static CMixFile* GetInstance(int index);

private:
    static std::vector<CMixFile> Instances;

private:


private:

};