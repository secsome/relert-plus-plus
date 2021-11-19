#pragma once

// This class will provide a set of methods to read a file
// MixFiles will be included too

#include <CMixFile.h>

#include <unordered_set>

class CFileClass
{
public:
    CFileClass();
    ~CFileClass();

    enum PathType
    {
        ExePath = 0,
        FilePath,
        FullPath,
    };

    bool Open(CString&& file, PathType path = FilePath);
    bool Open(CString& file, PathType path = FilePath);
    bool Open(uint32_t id);
    void Close();

    bool IsEOF() const;
    void* GetBuffer() const;
    size_t GetSize() const;
    size_t GetPosition() const;

    size_t Read(void* buffer, size_t size);

private:
    void Reset();

    CMixCachedFile* Cache;
    bool IsInCache;
    size_t Position;
};