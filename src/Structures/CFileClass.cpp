#include <CFileClass.h>

#include <CFinalSunApp.h>
#include <CCRC.h>

CFileClass::CFileClass()
{
    this->Reset();
}

CFileClass::~CFileClass() 
{
    this->Close();
}

bool CFileClass::Open(CString&& file, PathType path)
{
    return this->Open(std::forward<CString&>(file), path);
}

bool CFileClass::Open(CString& file, PathType path)
{
    CString fullpath;
    if (path == CFileClass::ExePath)
        fullpath = theApp.ExePath + file;
    else if (path == CFileClass::FilePath)
        fullpath = theApp.FilePath + file;
    else // FullPath
        fullpath = file;

    std::ifstream fin;
    fin.open(fullpath, std::ios::in | std::ios::binary);
    if (fin.is_open())
    {
        fin.seekg(0, std::ios::end);
        const int size = static_cast<int>(fin.tellg());
        if (size == 0)
            return false;

        this->Cache = new CMixCachedFile;
        this->Cache->Buffer = new byte[size];
        this->Cache->Size = size;
        this->IsInCache = false;
        this->Position = 0;

        fin.seekg(0, std::ios::beg);
        fin.read((char*)this->Cache->Buffer, size);
        fin.close();
        return true;
    }

    if (this->Open(CCRC()(file)))
        return true;

    return false;
}

bool CFileClass::Open(uint32_t id)
{
    this->Close();

    if (auto pCache = CMixFile::GetCachedData(id))
    {
        this->Cache = pCache;
        this->IsInCache = true;
        this->Position = 0;
        return true;
    }
    
    if (!CMixFile::CacheFileData(id))
        return false;

    if (auto pCache = CMixFile::GetCachedData(id))
    {
        this->Cache = pCache;
        this->IsInCache = true;
        this->Position = 0;
        return true;
    }

    return false;
}

void CFileClass::Close()
{
    if (Cache && !IsInCache)
    {
        delete[] Cache->Buffer;
        delete Cache;
    }
    this->Reset();
}

bool CFileClass::IsEOF() const
{
    return this->Position >= this->Cache->Size;
}

void* CFileClass::GetBuffer() const
{
    return this->Cache->Buffer;
}

size_t CFileClass::GetSize() const
{
    return this->Cache->Size;
}

size_t CFileClass::GetPosition() const
{
    return this->Position;
}

size_t CFileClass::Read(void* buffer, size_t size)
{
    if (this->Position + size >= this->Cache->Size)
        size = this->Cache->Size - this->Position - 1;

    memcpy(buffer, reinterpret_cast<byte*>(this->Cache->Buffer) + this->Position, size);
    this->Position += size;

    return size;
}

void CFileClass::Reset()
{
    Cache = nullptr;
    IsInCache = false;
    Position = 0;
}
