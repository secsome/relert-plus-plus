#pragma once

#include <Always.h>

#include <map>

struct IndexBasedComparator
{
    bool operator() (const CString& s1, const CString& s2);
};

class CINISection
{
public:
    bool Parse(char* pBuffer);
    int GetCount() const;
    bool AddPair(CString key, CString value);
    bool DeleteKey(CString key);
    bool HasKey(CString key) const;
    void Sort();

    using const_iterator = std::map<CString, CString, IndexBasedComparator>::const_iterator;

    const_iterator& begin() const;
    const_iterator& end() const;
    const_iterator& find(CString key) const;

private:
    std::map<CString, CString, IndexBasedComparator> StringMap;
    std::map<CString, int> IndexMap;
    bool Sorted;
    int LastIndex;
};

class CINI
{
public:
    static CINI Rules;
    static CINI Art;
    static CINI Sound;
    static CINI Eva;
    static CINI Theme;
    static CINI Ai;
    static CINI Temperate;
    static CINI Snow;
    static CINI Urban;
    static CINI NewUrban;
    static CINI Lunar;
    static CINI Desert;
    static CINI FAData;
    static CINI FALanguage;
    static CINI CurrentDocument;

    bool ReadFromFile(CString path);
    bool Parse(char* pBuffer);
    void Clear();

    bool SectionExists(CString section) const;
    bool KeyExists(CString section, CString key) const;
    int GetSectionCount() const;
    int GetKeyCount(CString section) const;
    bool DeleteSection(CString section);
    bool DeleteKey(CString section, CString key);

    bool WriteString(CString section, CString key, CString&& value);
    bool WriteInteger(CString section, CString key, int&& value);
    bool WriteFloat(CString section, CString key, float&& value);
    bool WriteDouble(CString section, CString key, double&& value);
    bool WriteBool(CString section, CString key, bool&& value);

    CINISection* TryGetSection(CString section) const;
    CString* TryGetString(CString section, CString key) const;
    CString ReadString(CString section, CString key, CString&& defvalue = "") const;
    int ReadInteger(CString section, CString key, int&& defvalue = 0) const;
    float ReadFloat(CString section, CString key, float&& defvalue = 0.0f) const;
    double ReadDouble(CString section, CString key, double&& defvalue = 0.0) const;
    bool ReadBool(CString section, CString key, bool&& defvalue = false) const;

    using const_iterator = std::map<CString, CINISection>::const_iterator;

    const_iterator& begin() const;
    const_iterator& end() const;

private:
    std::map<CString, CINISection> Dict;
    char Path[MAX_PATH];
};