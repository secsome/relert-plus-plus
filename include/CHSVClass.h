#pragma once

#include <Always.h>

class CRGBClass;

class CHSVClass
{
public:
    explicit CHSVClass();
    CHSVClass(byte h, byte s, byte v);

    operator CRGBClass() const;

    byte H;
    byte S;
    byte V;
};