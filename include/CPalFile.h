#pragma once

#include <Always.h>

#include <CRGBClass.h>
#include <CTintStruct.h>

#include <map>

class CPalFile
{
public:
    CPalFile();

    using iterator = CRGBClass*;

    iterator begin();
    iterator end();

    CRGBClass& operator[](int index);

    static void CreateInstance(CString name, CTintStruct remap = CTintStruct::NoRemap, CTintStruct tint = CTintStruct::StandardTint);
    static CPalFile* GetInstance(CString name, CTintStruct remap = CTintStruct::NoRemap, CTintStruct tint = CTintStruct::StandardTint);

private:
    static std::map<CString, std::map<CTintStruct, std::map<CTintStruct, CPalFile>>> Instances;

private:
    void Decode();
    void Encode();

private:
    CRGBClass Colors[0x100];
};