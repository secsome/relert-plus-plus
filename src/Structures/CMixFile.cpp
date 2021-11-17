#include <CMixFile.h>

#include <CBlowFish.h>
#include <CPublicKey.h>

std::vector<CMixFile> CMixFile::Instances;

CMixFile::CMixFile(CString& filename)
{
    
}

CMixFile::CMixFile(CString&& filename)
{
    
}

CMixFile::CMixFile(byte* data)
{
    this->Count = 0;
    this->DataSize = 0;
    this->DataStart = 0;
    this->IsDigest = false;
    this->IsPlain = false;
    this->IsEncrypted = false;
    this->ByteData = data;
}

CMixFile::~CMixFile()
{
    if (this->ByteData)
        delete[] this->ByteData;
}

void CMixFile::LoadToMemory()
{
    auto pHeader = reinterpret_cast<CMixHeader*>(ByteData);
    CBlowFish bf;
    if (pHeader->Flags.IsPlain)
    {
        this->IsPlain = true;
        this->Count = pHeader->Data.Count;
        this->DataSize = pHeader->Data.DataSize;
    }
    else
    {
        this->IsDigest = pHeader->Flags.IsDigest;
        this->IsEncrypted = pHeader->Flags.IsEncrypted;
        if (!this->IsEncrypted)
        {
            pHeader = reinterpret_cast<CMixHeader*>(ByteData + 4);
            this->Count = pHeader->Data.Count;
            this->DataSize = pHeader->Data.DataSize;
        }
    }

    if (!pHeader->Flags.IsEncrypted)
    {
        byte* pBlockData;
        if (this->IsPlain)
            pBlockData = this->ByteData + 6;
        else
            pBlockData = this->ByteData + 10;

        CMixSubBlock* pBlock = reinterpret_cast<CMixSubBlock*>(pBlockData);
        for (int i = 0; i < this->Count; ++i)
        {
            this->SubBlocks[pBlock->CRC] = *pBlock;
            ++pBlock;
        }

        if (this->IsPlain)
            this->DataStart = 6 + sizeof(CMixSubBlock) * this->Count;
        else
            this->DataStart = 10 + sizeof(CMixSubBlock) * this->Count;
    }
    else
    {
        // Decrypt header
        byte key_source[80];
        memcpy(key_source, this->ByteData + 4, sizeof(key_source));
        byte key[56];
        CPublicKey::get_blowfish_key(key_source, key);
        bf.SetKey(key, sizeof(key));
        uint32_t* e = bf.Decrypt(reinterpret_cast<uint32_t*>(this->ByteData + 84), 2);
        this->Count = static_cast<uint16_t>(e[0]);
        // this->DataSize = tmp[1];
        // delete[] tmp;

        const int cb_index = this->Count * sizeof(CMixSubBlock);
        const int cb_f = cb_index + 5 & ~7;

        if (this->Count)
        {
            auto f = bf.Decrypt(reinterpret_cast<uint32_t*>(this->ByteData + 92), cb_f / 4);
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
        }

        delete[] e;
    }
}
