#include <CPalFile.h>

std::vector<CPalFile> CPalFile::Instances;

CPalFile::CPalFile(char* buffer)
{
    memcpy_s(begin(), sizeof(Colors), buffer, sizeof(Colors));
    Decode();
}

CPalFile::iterator CPalFile::begin()
{
    return &Colors[0];
}

CPalFile::iterator CPalFile::end()
{
    return &Colors[255];
}

CRGBClass& CPalFile::operator[](int index)
{
    return Colors[index];
}

int CPalFile::CreateInstance(char* buffer)
{
    Instances.push_back(CPalFile(buffer));
    return Instances.size() - 1;
}

CPalFile* CPalFile::GetInstance(int index)
{
    return index >= 0 && index < (int)Instances.size() ? &Instances[index] : nullptr;
}

void CPalFile::Decode()
{
    for (auto& color : Colors)
    {
        color.R <<= 2;
        color.G <<= 2;
        color.B <<= 2;
    }
}

void CPalFile::Encode()
{
    for (auto& color : Colors)
    {
        color.R >>= 2;
        color.G >>= 2;
        color.B >>= 2;
    }
}
