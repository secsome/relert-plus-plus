#pragma once

#include <Always.h>

class CHSVClass;

class CRGBClass
{
public:
    explicit CRGBClass();
    CRGBClass(byte r, byte g, byte b);

    operator CHSVClass() const;

    byte R;
    byte G;
    byte B;
};