#include <CINI.h>

#include <vector>

bool IndexBasedComparator::operator()(const CString& s1, const CString& s2)
{
    const int l1 = s1.GetLength();
    const int l2 = s2.GetLength();
    return l1 < l2 || l1 == l2 && strcmp(s1, s2) < 0;
}

bool CINISection::Parse(char* pBuffer)
{
    return false;
}

int CINISection::GetCount() const
{
    return StringMap.size();
}

bool CINISection::AddPair(CString key, CString value)
{
    bool ret = true;
    if (HasKey(key))
        ret = false;

    StringMap[key] = value;
    IndexMap[key] = ++LastIndex;

    Sorted = false;

    return ret;
}

bool CINISection::DeleteKey(CString key)
{
    if (HasKey(key))
    {
        StringMap.erase(key);
        IndexMap.erase(key);
        Sorted = false;
        return true;
    }

    return false;
}

bool CINISection::HasKey(CString key) const
{
    return StringMap.find(key) != StringMap.end();
}

void CINISection::Sort()
{
    if (!Sorted)
    {
        std::map<int, CString> collector;
        for (auto& pair : IndexMap)
            collector[pair.second] = pair.first;
        IndexMap.clear();

        int i = 0;
        for (auto& pair : collector)
            IndexMap[pair.second] = i++;

        Sorted = true;
    }
}

CINISection::const_iterator& CINISection::begin() const
{
    return StringMap.begin();
}

CINISection::const_iterator& CINISection::end() const
{
    return StringMap.end();
}

CINISection::const_iterator& CINISection::find(CString key) const
{
    return StringMap.find(key);
}

CINI CINI::Rules;
CINI CINI::Art;
CINI CINI::Sound;
CINI CINI::Eva;
CINI CINI::Theme;
CINI CINI::Ai;
CINI CINI::Temperate;
CINI CINI::Snow;
CINI CINI::Urban;
CINI CINI::NewUrban;
CINI CINI::Lunar;
CINI CINI::Desert;
CINI CINI::FAData;
CINI CINI::FALanguage;
CINI CINI::CurrentDocument;

bool CINI::ReadFromFile(CString path)
{
    return false;
}

bool CINI::Parse(char* pBuffer)
{
    return false;
}

void CINI::Clear()
{
    Dict.clear();
    Path[0] = '\0';
}

bool CINI::SectionExists(CString section) const
{
    auto itr = Dict.find(section);
    return itr != Dict.end() && itr->second.GetCount() != 0;
}

bool CINI::KeyExists(CString section, CString key) const
{
    if (auto pSection = TryGetSection(section))
        return pSection->HasKey(key);
    else
        return false;
}

int CINI::GetSectionCount() const
{
    return Dict.size();
}

int CINI::GetKeyCount(CString section) const
{
    if (auto pSection = TryGetSection(section))
        return pSection->GetCount();
    else
        return 0;
}

bool CINI::DeleteSection(CString section)
{
    auto itr = Dict.find(section);
    if (itr == Dict.end())
        return false;

    Dict.erase(itr);
    return true;
}

bool CINI::DeleteKey(CString section, CString key)
{
    auto itr = Dict.find(section);
    if (itr == Dict.end())
        return false;

    bool ret = itr->second.DeleteKey(key);
    if (itr->second.GetCount() == 0)
        Dict.erase(itr);
    return ret;
}

CINISection* CINI::TryGetSection(CString section) const
{
    auto itr = Dict.find(section);
    if (itr == Dict.end())
        return nullptr;

    return const_cast<CINISection*>(&itr->second);
}

CString* CINI::TryGetString(CString section, CString key) const
{
    auto itr1 = Dict.find(section);
    if (itr1 == Dict.end())
        return nullptr;

    auto itr2 = itr1->second.find(key);
    if (itr2 == itr1->second.end())
        return nullptr;

    return const_cast<CString*>(&itr2->second);
}

CString CINI::ReadString(CString section, CString key, CString&& defvalue = "") const
{
    if (auto ppStr = TryGetString(section, key))
        return *ppStr;
    return defvalue;
}

int CINI::ReadInteger(CString section, CString key, int&& defvalue) const
{
    if (auto ppStr = TryGetString(section, key))
    {
        int ret;
        if (sscanf_s(*ppStr, "%d", &ret) == 1)
            return ret;
    }
    return defvalue;
}

float CINI::ReadFloat(CString section, CString key, float&& defvalue) const
{
    if (auto ppStr = TryGetString(section, key))
    {
        float ret;
        if (sscanf_s(*ppStr, "%f", &ret) == 1)
            return ret;
    }
    return defvalue;
}

double CINI::ReadDouble(CString section, CString key, double&& defvalue) const
{
    if (auto ppStr = TryGetString(section, key))
    {
        double ret;
        if (sscanf_s(*ppStr, "%lf", &ret) == 1)
            return ret;
    }
    return defvalue;
}

bool CINI::ReadBool(CString section, CString key, bool&& defvalue) const
{
    if (auto ppStr = TryGetString(section, key))
    {
        switch (toupper(*ppStr[0]))
        {
        case '1':
        case 'T':
        case 'Y':
            return true;
        case '0':
        case 'F':
        case 'N':
            return false;
        }
    }
    return defvalue;
}

CINI::const_iterator& CINI::begin() const
{
    return Dict.begin();
}

CINI::const_iterator& CINI::end() const
{
    return Dict.end();
}
