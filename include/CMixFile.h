#pragma once

#include <Always.h>

#include <vector>
#include <map>

struct CMixHeaderFlag
{
    short IsPlain : 16;
    short IsDigest : 1;
    short IsEncrypted : 1;
    short UnknownFlags : 14;
};

struct CMixHeaderData
{
    uint16_t Count;
    int DataSize;
};

union CMixHeader
{
    CMixHeaderFlag Flags;
    CMixHeaderData Data;
};

struct CMixSubBlock
{
    uint32_t CRC;
    int32_t Offset;
    int32_t Size;
};

class CMixFile
{
public:
    CMixFile(CString& filename);
    CMixFile(CString&& filename);
    CMixFile(byte* data);
    ~CMixFile();

    void LoadToMemory();

private:
    static std::vector<CMixFile> Instances;

    CString Filename;
    bool IsDigest;
    bool IsEncrypted;
    bool IsPlain;
    int Count;
    int DataSize;
    int DataStart;
    std::map<uint32_t, CMixSubBlock> SubBlocks;
    byte* ByteData;
};