#pragma once

#include <Always.h>

#include <CRGBClass.h>

#include <vector>

class CPalFile
{
public:
    CPalFile(char* buffer);

    using iterator = CRGBClass*;

    iterator begin();
    iterator end();

    CRGBClass& operator[](int index);

    static int CreateInstance(char* buffer);
    static CPalFile* GetInstance(int index);

private:
    static std::vector<CPalFile> Instances;

private:
    void Decode();
    void Encode();

private:
    CRGBClass Colors[0x100];
};