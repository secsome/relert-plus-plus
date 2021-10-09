#pragma once

#include <Always.h>

class CMixFile;
class CPalFile;

enum class TheaterType : int
{
    Invalid = -1,
    Temperate = 0, 
    Snow, 
    Urban, 
    NewUrban, 
    Lunar, 
    Desert, 
    Count 
};

struct ExpandMix
{
    CMixFile* Parent;
    CMixFile* Ecache01;
    CMixFile* Conquer;
    CMixFile* Local;
    CMixFile* IsoSnowmd;
    CMixFile* IsoTempmd;
    CMixFile* IsoUrbmd;
    CMixFile* IsoGen;
    CMixFile* IsoLun;
    CMixFile* IsoDes;
    CMixFile* IsoUbn;
    CMixFile* IsoGenmd;
    CMixFile* IsoLunmd;
    CMixFile* IsoDesmd;
    CMixFile* IsoUbnmd;
    CMixFile* Temperatmd;
    CMixFile* Snowmd;
    CMixFile* Urbanmd;
    CMixFile* Lunarmd;
    CMixFile* Urbannmd;
    CMixFile* Desertmd;
    CMixFile* Genericmd;
    CMixFile* Temmd;
    CMixFile* Snomd;
    CMixFile* Urbmd;
    CMixFile* Ubn;
    CMixFile* Lun;
    CMixFile* Des;
    CMixFile* Marblemd;
};

class CLoading
{
public:


private:
    char TheaterIdentifier;
    CStatic CSCVersion;
    CStatic CSCBuiltBy;
    CStatic CSCLoading;
    CProgressCtrl CPCProgress;
    CMixFile* PAL_ISO[EnumCount<TheaterType>];
    CMixFile* PAL_UNIT[EnumCount<TheaterType>];
    CMixFile* PAL_THEATER[EnumCount<TheaterType>];
    CMixFile* MIX_ISO[EnumCount<TheaterType>];
    CMixFile* MIX_THEATER[EnumCount<TheaterType>];
    CMixFile* MIX_ISOGEN;
    CMixFile* MIX_LOCAL;
    CMixFile* MIX_CACHE;
    CMixFile* MIX_CONQUER;
    CMixFile* MIX_LANGUAGE;
    CMixFile* MIX_LANGMD;
    CMixFile* MIX_MARBLE;
    CMixFile* MIX_ECACHE[100];
    ExpandMix MIX_EXPAND[100];
    CMixFile* MIX_RA2YR;
};