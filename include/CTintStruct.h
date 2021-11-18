#pragma once

class CRGBClass;

struct CTintStruct
{
    static CTintStruct StandardTint;
    static CTintStruct NoRemap;

    bool operator<(const CTintStruct& another) const;
    bool IsStandardTint() const;
    bool IsNoRemap() const;
    bool IsRemapTint() const;
    CRGBClass GetRemapColor() const;

    int Red;
    int Green;
    int Blue;
};