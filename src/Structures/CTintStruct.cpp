#include <CTintStruct.h>

#include <CRGBClass.h>

CTintStruct CTintStruct::StandardTint{ 1000,1000,1000 };
CTintStruct CTintStruct::NoRemap{ INT_MAX,INT_MAX,INT_MAX };

bool CTintStruct::operator<(const CTintStruct& another) const
{
    if (this->Red < another.Red)
        return true;
    else if (this->Red > another.Red)
        return false;
    else if (this->Blue < another.Blue)
        return true;
    else if (this->Blue > another.Blue)
        return false;
    else if (this->Green < another.Green)
        return true;
    else
        return false;
}

bool CTintStruct::IsStandardTint() const
{
    return this->Red == 1000 && this->Green == 1000 && this->Blue == 1000;
}

bool CTintStruct::IsNoRemap() const
{
    return this->Red == INT_MAX && this->Green == INT_MAX && this->Blue == INT_MAX;
}

bool CTintStruct::IsRemapTint() const
{
    return this->Red <= 0 && this->Green <= 0 && this->Blue <= 0;
}

CRGBClass CTintStruct::GetRemapColor() const
{
    return CRGBClass(-this->Red, -this->Green, -this->Blue);
}
