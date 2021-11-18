#include <CPalFile.h>

#include <CFileClass.h>
#include <CRGBClass.h>
#include <CHSVClass.h>

std::map<CString, std::map<CTintStruct, std::map<CTintStruct, CPalFile>>> CPalFile::Instances;

CPalFile::CPalFile() = default;

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

void CPalFile::CreateInstance(CString name, CTintStruct remap, CTintStruct tint)
{
    auto itr_name = CPalFile::Instances.find(name);
    if (itr_name == CPalFile::Instances.end())
    {
        CFileClass file;
        if (!file.Open(name))
            return;
        auto& palette = CPalFile::Instances[name][CTintStruct::NoRemap][CTintStruct::StandardTint];
        file.Read(&palette, sizeof(CPalFile::Colors));
        palette.Decode();
    }

    auto itr_remap = CPalFile::Instances[name].find(remap);
    if (tint.IsRemapTint() && itr_remap == CPalFile::Instances[name].end())
    {
        // Create remap tint
        auto rgb_remap = tint.GetRemapColor();
        auto& palette = CPalFile::Instances[name][remap][CTintStruct::StandardTint];
        memcpy_s(&palette, sizeof(CPalFile::Colors), &CPalFile::Instances[name][CTintStruct::NoRemap][CTintStruct::StandardTint], sizeof(CPalFile::Colors));

        for (int i = 16; i <= 31; ++i)
        {
            int ii = i - 16;
            double cosval = ii * 0.08144869842640204 + 0.3490658503988659;
            double sinval = ii * 0.04654211338651545 + 0.8726646259971648;
            if (!ii)
                cosval = 0.1963495408493621;

            CHSVClass hsv_remap = rgb_remap; // convert from rgb to hsv

            hsv_remap.H = hsv_remap.H;
            hsv_remap.S = (unsigned char)(std::sin(sinval) * hsv_remap.S);
            hsv_remap.V = (unsigned char)(std::cos(cosval) * hsv_remap.V);

            palette[i] = hsv_remap; // convert from hsv to rgb
        }
    }

    auto itr_tint = CPalFile::Instances[name][remap].find(tint);
    if (!tint.IsStandardTint() && itr_tint == CPalFile::Instances[name][remap].end())
    {
        auto& palette = CPalFile::Instances[name][remap][tint];
        memcpy_s(&palette, sizeof(CPalFile::Colors), &CPalFile::Instances[name][CTintStruct::NoRemap][CTintStruct::StandardTint], sizeof(CPalFile::Colors));
        // TODO : Import tint code from vanilla YR

    }
}

CPalFile* CPalFile::GetInstance(CString name, CTintStruct remap, CTintStruct tint)
{
    auto itr_name = CPalFile::Instances.find(name);
    if (itr_name == CPalFile::Instances.end())
        return nullptr;

    auto itr_remap = itr_name->second.find(remap);
    if (itr_remap == itr_name->second.end())
        return nullptr;

    auto itr_tint = itr_remap->second.find(tint);
    if (itr_tint == itr_remap->second.end())
        return nullptr;

    return &itr_tint->second;
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
