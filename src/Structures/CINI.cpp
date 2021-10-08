#include <CINI.h>

bool IndexBasedComparator::operator()(const CString& s1, const CString& s2) const
{
    const int l1 = s1.GetLength();
    const int l2 = s2.GetLength();
    return l1 < l2 || l1 == l2 && strcmp(s1, s2) < 0;
}

CINISection::CINISection()
{
    Sorted = false;
    LastIndex = 0;
}

void CINISection::Clear()
{
    StringMap.clear();
    IndexMap.clear();
    Sorted = false;
    LastIndex = 0;
}

int CINISection::GetCount() const
{
    return StringMap.size();
}

bool CINISection::AddPair(CString&& key, CString&& value)
{
    bool ret = true;
    if (HasKey(key))
        ret = false;

    StringMap[key] = value;
    IndexMap[key] = ++LastIndex;

    Sorted = false;

    return ret;
}

bool CINISection::AddPair(CString& key, CString& value)
{
    return AddPair(std::forward<CString>(key), std::forward<CString>(value));
}

bool CINISection::DeleteKey(CString&& key)
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

bool CINISection::DeleteKey(CString& key)
{
    return DeleteKey(std::forward<CString>(key));
}

bool CINISection::HasKey(CString&& key) const
{
    return StringMap.find(key) != StringMap.end();
}

bool CINISection::HasKey(CString& key) const
{
    return HasKey(std::forward<CString>(key));
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

CINISection::const_iterator CINISection::begin() const
{
    return StringMap.begin();
}

CINISection::const_iterator CINISection::end() const
{
    return StringMap.end();
}

CINISection::const_iterator CINISection::find(CString key) const
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

bool CINI::ReadFromFile(CString&& path)
{
    std::ifstream fin;
    fin.open(path, std::ios::in | std::ios::binary);
    
    if (!fin.is_open())
        return false;

    fin.seekg(0, std::ios::end);
    const int size = static_cast<int>(fin.tellg());
    if (size == 0)
        return false;

    fin.seekg(0, std::ios::beg);
    
    auto buffer = new char[size];
    fin.read(buffer, size);
    bool result = Parse(buffer, size);
    delete[] buffer;

    return result;
}

bool CINI::ReadFromFile(CString& path)
{
    return ReadFromFile(std::forward<CString>(path));
}

bool CINI::Parse(char* buffer, int size)
{
    char* ptr = buffer;
    char* pend = buffer + size;

    auto get_line = [pend](char* ptr) -> int
    {
        int counter = 0;
        while (*ptr != '\n' && ptr < pend)
        {
            ++counter;
            ++ptr;
        }
        return counter;
    };

    CString strbuf;

    while (ptr < pend)
    {
        int line_len = get_line(ptr);
        if (ptr + line_len >= pend)
            return false;

        ptr[line_len] = '\0';
        strbuf = ptr;
        ptr += line_len + 1;
        strbuf.Trim();

        if (strbuf[0] == '[' && strbuf.Find(']') != -1)
            break;
    }

    while (ptr < pend)
    {
        auto last = strbuf.Find(']');
        if (last != -1)
            strbuf = strbuf.Mid(1, last - 1);
        CString sectionName = strbuf;
        LPCSTR pstr = sectionName.operator LPCSTR();
        CINISection& section = Dict[strbuf];

        while (ptr < pend)
        {
            int line_len = get_line(ptr);
            if (ptr + line_len < pend)
                ptr[line_len] = '\0';
            strbuf = ptr;
            ptr += line_len + 1;

            strbuf.Trim();
            if (strbuf[0] == '[' && strbuf.Find(']') != -1)
                break;
        
            int comment_flag = strbuf.Find(';');
            if (comment_flag != -1)
            {
                strbuf = strbuf.Mid(0, comment_flag);
                strbuf.TrimRight();
            }

            if (strbuf.IsEmpty() || strbuf[0] == ';' || strbuf[0] == '=')
                continue;

            auto divider = strbuf.Find('=');
            if (divider == -1)
                continue;

            CString key = strbuf.Mid(0, divider);
            CString value = strbuf.Mid(divider + 1);
            section.AddPair(key, value);
        }

        if (!section.GetCount())
            Dict.erase(sectionName);
    }

    return true;
}

void CINI::Clear()
{
    Dict.clear();
    Path[0] = '\0';
}

bool CINI::SectionExists(CString&& section) const
{
    auto itr = Dict.find(section);
    return itr != Dict.end() && itr->second.GetCount() != 0;
}

bool CINI::SectionExists(CString& section) const
{
    return SectionExists(std::forward<CString>(section));
}

bool CINI::KeyExists(CString&& section, CString&& key) const
{
    if (auto pSection = TryGetSection(section))
        return pSection->HasKey(key);
    else
        return false;
}

bool CINI::KeyExists(CString& section, CString& key) const
{
    return KeyExists(std::forward<CString>(section), std::forward<CString>(key));
}

int CINI::GetSectionCount() const
{
    return Dict.size();
}

int CINI::GetKeyCount(CString&& section) const
{
    if (auto pSection = TryGetSection(section))
        return pSection->GetCount();
    else
        return 0;
}

int CINI::GetKeyCount(CString& section) const
{
    return GetKeyCount(std::forward<CString>(section));
}

bool CINI::DeleteSection(CString&& section)
{
    auto itr = Dict.find(section);
    if (itr == Dict.end())
        return false;

    Dict.erase(itr);
    return true;
}

bool CINI::DeleteSection(CString& section)
{
    return DeleteSection(std::forward<CString>(section));
}

bool CINI::DeleteKey(CString&& section, CString&& key)
{
    auto itr = Dict.find(section);
    if (itr == Dict.end())
        return false;

    bool ret = itr->second.DeleteKey(key);
    if (itr->second.GetCount() == 0)
        Dict.erase(itr);
    return ret;
}

bool CINI::DeleteKey(CString& section, CString& key)
{
    return DeleteKey(std::forward<CString>(section), std::forward<CString>(key));
}

bool CINI::WriteString(CString&& section, CString&& key, CString&& value)
{
    bool result = true;

    auto itr1 = Dict.find(section);
    if (itr1 == Dict.end())
    {
        itr1 = Dict.insert(itr1, std::make_pair(section, CINISection()));
        result = false;
    }

    return result && itr1->second.AddPair(key, value);
}

bool CINI::WriteString(CString& section, CString& key, CString&& value)
{
    return WriteString(std::forward<CString>(section), std::forward<CString>(key), std::forward<CString>(value));
}

bool CINI::WriteString(CString&& section, CString&& key, CString& value)
{
    return WriteString(std::forward<CString>(section), std::forward<CString>(key), std::forward<CString>(value));
}

bool CINI::WriteString(CString& section, CString& key, CString& value)
{
    return WriteString(std::forward<CString>(section), std::forward<CString>(key), std::forward<CString>(value));
}

bool CINI::WriteInteger(CString&& section, CString&& key, int&& value)
{
    CString buffer;
    buffer.Format("%d", value);
    return WriteString(section, key, std::move(buffer));
}

bool CINI::WriteInteger(CString&& section, CString&& key, int& value)
{
    return WriteInteger(std::forward<CString>(section), std::forward<CString>(key), std::forward<int>(value));
}

bool CINI::WriteInteger(CString& section, CString& key, int&& value)
{
    return WriteInteger(std::forward<CString>(section), std::forward<CString>(key), std::forward<int>(value));
}

bool CINI::WriteInteger(CString& section, CString& key, int& value)
{
    return WriteInteger(std::forward<CString>(section), std::forward<CString>(key), std::forward<int>(value));
}

bool CINI::WriteFloat(CString&& section, CString&& key, float&& value)
{
    CString buffer;
    buffer.Format("%f", value);
    return WriteString(section, key, std::move(buffer));
}

bool CINI::WriteFloat(CString&& section, CString&& key, float& value)
{
    return WriteFloat(std::forward<CString>(section), std::forward<CString>(key), std::forward<float>(value));
}

bool CINI::WriteFloat(CString& section, CString& key, float&& value)
{
    return WriteFloat(std::forward<CString>(section), std::forward<CString>(key), std::forward<float>(value));
}

bool CINI::WriteFloat(CString& section, CString& key, float& value)
{
    return WriteFloat(std::forward<CString>(section), std::forward<CString>(key), std::forward<float>(value));
}

bool CINI::WriteDouble(CString&& section, CString&& key, double&& value)
{
    CString buffer;
    buffer.Format("%lf", value);
    return WriteString(section, key, std::move(buffer));
}

bool CINI::WriteDouble(CString&& section, CString&& key, double& value)
{
    return WriteDouble(std::forward<CString>(section), std::forward<CString>(key), std::forward<double>(value));
}

bool CINI::WriteDouble(CString& section, CString& key, double&& value)
{
    return WriteDouble(std::forward<CString>(section), std::forward<CString>(key), std::forward<double>(value));
}

bool CINI::WriteDouble(CString& section, CString& key, double& value)
{
    return WriteDouble(std::forward<CString>(section), std::forward<CString>(key), std::forward<double>(value));
}

bool CINI::WriteBoolean(CString&& section, CString&& key, bool&& value)
{
    return WriteString(section, key, value ? "Yes" : "No");
}

bool CINI::WriteBoolean(CString&& section, CString&& key, bool& value)
{
    return WriteBoolean(std::forward<CString>(section), std::forward<CString>(key), std::forward<bool>(value));
}

bool CINI::WriteBoolean(CString& section, CString& key, bool&& value)
{
    return WriteBoolean(std::forward<CString>(section), std::forward<CString>(key), std::forward<bool>(value));
}

bool CINI::WriteBoolean(CString& section, CString& key, bool& value)
{
    return WriteBoolean(std::forward<CString>(section), std::forward<CString>(key), std::forward<bool>(value));
}

CINISection* CINI::TryGetSection(CString&& section) const
{
    auto itr = Dict.find(section);
    if (itr == Dict.end())
        return nullptr;

    return const_cast<CINISection*>(&itr->second);
}

CINISection* CINI::TryGetSection(CString& section) const
{
    return TryGetSection(std::forward<CString>(section));
}

CString* CINI::TryGetString(CString&& section, CString&& key) const
{
    auto itr1 = Dict.find(section);
    if (itr1 == Dict.end())
        return nullptr;

    auto itr2 = itr1->second.find(key);
    if (itr2 == itr1->second.end())
        return nullptr;

    return const_cast<CString*>(&itr2->second);
}

CString* CINI::TryGetString(CString& section, CString& key) const
{
    return TryGetString(std::forward<CString>(section), std::forward<CString>(key));
}

CString CINI::ReadString(CString&& section, CString&& key, CString&& defvalue) const
{
    if (auto ppStr = TryGetString(section, key))
        return *ppStr;
    return defvalue;
}

CString CINI::ReadString(CString&& section, CString&& key, CString& defvalue) const
{
    return ReadString(std::forward<CString>(section), std::forward<CString>(key), std::forward<CString>(defvalue));
}

CString CINI::ReadString(CString& section, CString& key, CString&& defvalue) const
{
    return ReadString(std::forward<CString>(section), std::forward<CString>(key), std::forward<CString>(defvalue));
}

CString CINI::ReadString(CString& section, CString& key, CString& defvalue) const
{
    return ReadString(std::forward<CString>(section), std::forward<CString>(key), std::forward<CString>(defvalue));
}

int CINI::ReadInteger(CString&& section, CString&& key, int&& defvalue) const
{
    if (auto ppStr = TryGetString(section, key))
    {
        int ret;
        if (sscanf_s(*ppStr, "%d", &ret) == 1)
            return ret;
    }
    return defvalue;
}

int CINI::ReadInteger(CString&& section, CString&& key, int& defvalue) const
{
    return ReadInteger(std::forward<CString>(section), std::forward<CString>(key), std::forward<int>(defvalue));
}

int CINI::ReadInteger(CString& section, CString& key, int&& defvalue) const
{
    return ReadInteger(std::forward<CString>(section), std::forward<CString>(key), std::forward<int>(defvalue));
}

int CINI::ReadInteger(CString& section, CString& key, int& defvalue) const
{
    return ReadInteger(std::forward<CString>(section), std::forward<CString>(key), std::forward<int>(defvalue));
}

float CINI::ReadFloat(CString&& section, CString&& key, float&& defvalue) const
{
    if (auto ppStr = TryGetString(section, key))
    {
        float ret;
        if (sscanf_s(*ppStr, "%f", &ret) == 1)
            return ret;
    }
    return defvalue;
}

float CINI::ReadFloat(CString&& section, CString&& key, float& defvalue) const
{
    return ReadFloat(std::forward<CString>(section), std::forward<CString>(key), std::forward<float>(defvalue));
}

float CINI::ReadFloat(CString& section, CString& key, float&& defvalue) const
{
    return ReadFloat(std::forward<CString>(section), std::forward<CString>(key), std::forward<float>(defvalue));
}

float CINI::ReadFloat(CString& section, CString& key, float& defvalue) const
{
    return ReadFloat(std::forward<CString>(section), std::forward<CString>(key), std::forward<float>(defvalue));
}

double CINI::ReadDouble(CString&& section, CString&& key, double&& defvalue) const
{
    if (auto ppStr = TryGetString(section, key))
    {
        double ret;
        if (sscanf_s(*ppStr, "%lf", &ret) == 1)
            return ret;
    }
    return defvalue;
}

double CINI::ReadDouble(CString&& section, CString&& key, double& defvalue) const
{
    return ReadDouble(std::forward<CString>(section), std::forward<CString>(key), std::forward<double>(defvalue));
}

double CINI::ReadDouble(CString& section, CString& key, double&& defvalue) const
{
    return ReadDouble(std::forward<CString>(section), std::forward<CString>(key), std::forward<double>(defvalue));
}

double CINI::ReadDouble(CString& section, CString& key, double& defvalue) const
{
    return ReadDouble(std::forward<CString>(section), std::forward<CString>(key), std::forward<double>(defvalue));
}

bool CINI::ReadBoolean(CString&& section, CString&& key, bool&& defvalue) const
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

bool CINI::ReadBoolean(CString&& section, CString&& key, bool& defvalue) const
{
    return ReadBoolean(std::forward<CString>(section), std::forward<CString>(key), std::forward<bool>(defvalue));
}

bool CINI::ReadBoolean(CString& section, CString& key, bool&& defvalue) const
{
    return ReadBoolean(std::forward<CString>(section), std::forward<CString>(key), std::forward<bool>(defvalue));
}

bool CINI::ReadBoolean(CString& section, CString& key, bool& defvalue) const
{
    return ReadBoolean(std::forward<CString>(section), std::forward<CString>(key), std::forward<bool>(defvalue));
}

CINI::const_iterator CINI::begin() const
{
    return Dict.begin();
}

CINI::const_iterator CINI::end() const
{
    return Dict.end();
}
