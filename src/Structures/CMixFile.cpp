#include <CMixFile.h>

std::vector<CMixFile> CMixFile::Instances;

CMixFile::CMixFile(char* buffer)
{
}

int CMixFile::CreateInstance(char* buffer)
{
    Instances.push_back(CMixFile(buffer));
    return Instances.size() - 1;
}

CMixFile* CMixFile::GetInstance(int index)
{
    return index >= 0 && index < (int)Instances.size() ? &Instances[index] : nullptr;
}