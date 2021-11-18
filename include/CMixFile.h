#pragma once

#include <Always.h>

#include <vector>
#include <unordered_map>

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

struct CMixCachedFile
{
    int32_t Size;
    void* Buffer;
};

class CMixFile
{
public:
    CMixFile();
    ~CMixFile();

    static bool LoadMixFile(CString filename);
    static byte* Retreive(CString&& filename, size_t* pSize = nullptr, bool cache = true);
    static byte* Retreive(CString& filename, size_t* pSize = nullptr, bool cache = true);
    static CMixCachedFile* GetCachedData(uint32_t id);
    static bool CacheFileData(uint32_t id);
    static bool CacheFileData(CString&& filename);
    static bool CacheFileData(CString& filename);
    static void ClearAllCache();
    static void ClearAllFile();

private:
    static std::vector<CMixFile> Instances;
    static std::unordered_map<uint32_t, CMixCachedFile> CachedFiles;

    void LoadToMemory();
    bool CacheFile(uint32_t id) const;
    bool HasFile(uint32_t id) const;
    const CMixSubBlock* TryGetSubBlock(uint32_t id) const;

    CMixFile* GetParent() const;
    int SeekTo(int offset, int where) const;
    size_t ReadBytes(void* buffer, size_t element_size, size_t element_count) const;

    CString Filename;
    bool IsDigest;
    bool IsEncrypted;
    bool IsPlain;
    int Count;
    int DataSize;
    int DataStart;
    std::unordered_map<uint32_t, CMixSubBlock> SubBlocks;
    union
    {
        int Offset; // For the mixes in the raw root, we store a raw file pointer here
        FILE* FilePointer; //  and for sub mixes, we just store their offset in their parent
    };
    size_t Parent; // Index in the global array
};