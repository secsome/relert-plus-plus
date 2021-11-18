#include <CMixFile.h>

#include <CBlowFish.h>
#include <CPublicKey.h>
#include <CCRC.h>

#include <CFinalSunApp.h>

std::vector<CMixFile> CMixFile::Instances;
std::unordered_map<uint32_t, CMixCachedFile> CMixFile::CachedFiles;

CMixFile::CMixFile() = default;

CMixFile::~CMixFile()
{
    if (!this->Parent)
        fclose(this->FilePointer);
}

bool CMixFile::LoadMixFile(CString filename)
{
    FILE* fp;
    fopen_s(&fp, theApp.FilePath + filename, "rb");
    if (fp)
    {
        CMixFile::Instances.emplace_back(CMixFile());
        auto& mix = CMixFile::Instances.back();
        mix.Filename = filename;
        mix.Parent = nullptr;
        mix.FilePointer = fp;
        mix.LoadToMemory();
        return true;
    }
    else // Notice we will never cache a mix file
    {
        for (auto& parent : CMixFile::Instances)
        {
            CCRC crc;
            if (auto pBlock = parent.TryGetSubBlock(crc(filename)))
            {
                CMixFile::Instances.emplace_back(CMixFile());
                auto& mix = CMixFile::Instances.back();
                mix.Filename = filename;
                mix.Parent = &parent;
                mix.Offset = pBlock->Offset;
                mix.LoadToMemory();

                return true;
            }
        }
    }

    return false;
}

byte* CMixFile::Retreive(CString&& filename, size_t* pSize, bool cache)
{
    return CMixFile::Retreive(std::forward<CString&>(filename), pSize, cache);
}

byte* CMixFile::Retreive(CString& filename, size_t* pSize, bool cache)
{
    CCRC crc;
    uint32_t id = crc(filename);
    if (auto pCache = CMixFile::GetCachedData(id))
    {
        if (pSize)
            *pSize = pCache->Size;
        return reinterpret_cast<byte*>(pCache->Buffer);
    }
    
    if (cache)
    {
        if (CMixFile::CacheFileData(id))
        {
            if (auto pCache = CMixFile::GetCachedData(id))
            {
                if (pSize)
                    *pSize = pCache->Size;
                return reinterpret_cast<byte*>(pCache->Buffer);
            }
            return nullptr;
        }
    }

    for (auto& mix : CMixFile::Instances)
    {
        if (auto pBlock = mix.TryGetSubBlock(id))
        {
            byte* ret = new byte[pBlock->Size];
            if (pSize)
                *pSize = pBlock->Size;
            mix.SeekTo(pBlock->Offset, SEEK_SET);
            mix.ReadBytes(ret, pBlock->Size, 1);
            return ret;
        }
    }

    return nullptr;
}

CMixCachedFile* CMixFile::GetCachedData(uint32_t id)
{
    auto itr = CMixFile::CachedFiles.find(id);
    return itr != CMixFile::CachedFiles.end() ? &itr->second : nullptr;
}

bool CMixFile::CacheFileData(uint32_t id)
{
    if (CMixFile::GetCachedData(id))
        return true; // Already cached

    // Cache the file
    for (auto& mix : CMixFile::Instances)
    {
        if (mix.CacheFile(id))
            return true;
    }

    // Failed to cache
    return false;
}

bool CMixFile::CacheFileData(CString&& filename)
{
    return CMixFile::CacheFileData(std::forward<CString&>(filename));
}

bool CMixFile::CacheFileData(CString& filename)
{
    CCRC crc;
    uint32_t id = crc(filename);

    return CMixFile::CacheFileData(id);
}

void CMixFile::ClearAllCache()
{
    for (auto& cache : CMixFile::CachedFiles)
        delete[] cache.second.Buffer;

    CMixFile::CachedFiles.clear();
}

void CMixFile::LoadToMemory()
{
    CBlowFish bf;
    CMixHeader header;
    this->SeekTo(0, SEEK_SET);
    this->ReadBytes(&header, sizeof(header), 1);

    if (header.Flags.IsPlain)
    {
        this->IsPlain = true;
        this->Count = header.Data.Count;
        this->DataSize = header.Data.DataSize;
    }
    else
    {
        this->IsDigest = header.Flags.IsDigest;
        this->IsEncrypted = header.Flags.IsEncrypted;
        if (!this->IsEncrypted)
        {
            this->SeekTo(4, SEEK_SET);
            this->ReadBytes(&header, sizeof(header), 1);
            this->Count = header.Data.Count;
            this->DataSize = header.Data.DataSize;
        }
    }

    if (!header.Flags.IsEncrypted)
    {
        for (int i = 0; i < this->Count; ++i)
        {
            CMixSubBlock subBlock;
            this->ReadBytes(&subBlock, sizeof(subBlock), 1);
            this->SubBlocks[subBlock.CRC] = subBlock;
        }

        if (this->IsPlain)
            this->DataStart = 6 + sizeof(CMixSubBlock) * this->Count;
        else
            this->DataStart = 10 + sizeof(CMixSubBlock) * this->Count;
    }
    else
    {
        // Decrypt header
        this->SeekTo(4, SEEK_SET);
        byte key_source[80];
        this->ReadBytes(key_source, sizeof(key_source), 1);
        byte key[56];
        CPublicKey::get_blowfish_key(key_source, key);
        bf.SetKey(key, sizeof(key));
        uint32_t e[2];
        this->ReadBytes(e, sizeof(e), 1);
        auto pHeader = bf.Decrypt(e, 2);
        this->Count = static_cast<uint16_t>(e[0]);
        this->DataSize = e[1];

        const int cb_index = this->Count * sizeof(CMixSubBlock);
        const int cb_f = cb_index + 5 & ~7;

        if (this->Count)
        {
            auto buffer = new byte[cb_f];
            this->ReadBytes(buffer, cb_f, 1);
            auto f = bf.Decrypt(reinterpret_cast<uint32_t*>(buffer), cb_f / 4);
            CMixSubBlock tmp;
            memcpy(&tmp, reinterpret_cast<byte*>(e) + 6, 2);
            memcpy(reinterpret_cast<byte*>(&tmp) + 2, f, 10);
            this->SubBlocks[tmp.CRC] = tmp;
            
            CMixSubBlock* pBlock = reinterpret_cast<CMixSubBlock*>(reinterpret_cast<byte*>(f) + 10);
            for (int i = 1; i < this->Count; ++i)
            {
                this->SubBlocks[pBlock->CRC] = *pBlock;
                ++pBlock;
            }

            delete[] f;
            delete[] buffer;
        }

        delete[] pHeader;
    }
}

bool CMixFile::CacheFile(uint32_t id) const
{
    auto itr = this->SubBlocks.find(id);
    if (itr != this->SubBlocks.end())
    {
        auto& cache = this->CachedFiles[id];
        cache.Size = itr->second.Size;
        cache.Buffer = new byte[itr->second.Size];
        this->SeekTo(this->DataStart + itr->second.Offset, SEEK_SET);
        this->ReadBytes(cache.Buffer, itr->second.Size, 1);
        return true;
    }
    return false;
}

bool CMixFile::HasFile(uint32_t id) const
{
    return this->TryGetSubBlock(id) != nullptr;
}

const CMixSubBlock* CMixFile::TryGetSubBlock(uint32_t id) const
{
    auto itr = this->SubBlocks.find(id);

    return itr != this->SubBlocks.end() ? &itr->second : nullptr;
}

void CMixFile::SeekTo(int offset, int where) const
{
    if (!this->Parent)
        fseek(this->FilePointer, offset, where);
    else
        this->Parent->SeekTo(this->Parent->DataStart + this->Offset + offset, where);
}

void CMixFile::ReadBytes(void* buffer, size_t element_size, size_t element_count) const
{
    if (!this->Parent)
        fread(buffer, element_size, element_count, this->FilePointer);
    else
        this->Parent->ReadBytes(buffer, element_size, element_count);
}
