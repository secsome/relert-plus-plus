#include "CLoading.h"

#include <CINI.h>
#include <CMixFile.h>
#include <CPalFile.h>
#include <CFileClass.h>

#include <FileNames.h>

IMPLEMENT_DYNAMIC(CLoading, CDialogEx)

CLoading::CLoading(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLOADING, pParent)
{

}

CLoading::~CLoading()
{
}

void CLoading::Load()
{
	CFinalSunLog::Instance.Log(__FUNCTION__" called\n\n");

	CFinalSunLog::Instance.Log("Initializing mix files\n");
	this->InitMixFiles();

	CFinalSunLog::Instance.Log("Initializing palettes\n");
	this->InitPalettes();

	CFinalSunLog::Instance.Log("Initializing ini files\n");
	this->InitINIFiles();
	
}

void CLoading::InitMixFiles()
{
	CString mixName;

	// Init_Bootstrap_Mixfiles
	for (int i = 99; i >= 0; --i)
	{
		mixName.Format(FileNames::MIX_EXPANDMD, i);
		CMixFile::LoadMixFile(mixName);
	}

	CMixFile::LoadMixFile(FileNames::MIX_RA2MD);
	CMixFile::LoadMixFile(FileNames::MIX_RA2);
	CMixFile::LoadMixFile(FileNames::MIX_CACHEMD);
	CMixFile::LoadMixFile(FileNames::MIX_CACHE);
	CMixFile::LoadMixFile(FileNames::MIX_LOCALMD);
	CMixFile::LoadMixFile(FileNames::MIX_LOCAL);

	// Init_Expansion_Files
	// ECACHE & ELOCAL
	auto Finder_Load_Mix = [this](const char* lpFileName)
	{
		WIN32_FIND_DATA fd;
		HANDLE hFile = FindFirstFile(lpFileName, &fd);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			do
			{
				// TEMPORARY | DIRECTORY | SYSTEM | HIDDEN
				if ((fd.dwFileAttributes & 278) == 0)
					CMixFile::LoadMixFile(fd.cFileName);
			}
			while (FindNextFile(hFile, &fd));
			FindClose(hFile);
		}
	};

	Finder_Load_Mix(FileNames::MIX_ECACHE);
	Finder_Load_Mix(FileNames::MIX_ELOCAL);

	// Init_Secondary_Mixfiles
	CMixFile::LoadMixFile(FileNames::MIX_CONQMD);
	CMixFile::LoadMixFile(FileNames::MIX_GENERMD);
	CMixFile::LoadMixFile(FileNames::MIX_GENERIC);
	CMixFile::LoadMixFile(FileNames::MIX_ISOGEN);
	CMixFile::LoadMixFile(FileNames::MIX_CONQUER);
	CMixFile::LoadMixFile(FileNames::MIX_CAMEOMD);
	CMixFile::LoadMixFile(FileNames::MIX_CAMEO);
	// Finder_Load_Mix(FileNames::MIX_MAPS);
	// CMixFile::LoadMixFile(FileNames::MIX_MAPSMD);
	CMixFile::LoadMixFile(FileNames::MIX_MULTIMD);
	if (!CMixFile::LoadMixFile(FileNames::MIX_THEMEMD))
		CMixFile::LoadMixFile(FileNames::MIX_THEME);
	// MOVIES

	// Init_Theaters
	CMixFile::LoadMixFile(FileNames::MIX_TEMPERATMD);
	CMixFile::LoadMixFile(FileNames::MIX_TEMPERAT);
	CMixFile::LoadMixFile(FileNames::MIX_TEM);
	CMixFile::LoadMixFile(FileNames::MIX_ISOTEMMD);
	CMixFile::LoadMixFile(FileNames::MIX_ISOTEMP);

	CMixFile::LoadMixFile(FileNames::MIX_SNOWMD);
	CMixFile::LoadMixFile(FileNames::MIX_SNOW);
	CMixFile::LoadMixFile(FileNames::MIX_SNO);
	CMixFile::LoadMixFile(FileNames::MIX_ISOSNOMD);
	CMixFile::LoadMixFile(FileNames::MIX_ISOSNOW);
	
	CMixFile::LoadMixFile(FileNames::MIX_URBANMD);
	CMixFile::LoadMixFile(FileNames::MIX_URBAN);
	CMixFile::LoadMixFile(FileNames::MIX_URB);
	CMixFile::LoadMixFile(FileNames::MIX_ISOURBMD);
	CMixFile::LoadMixFile(FileNames::MIX_ISOURB);

	CMixFile::LoadMixFile(FileNames::MIX_DESERTMD);
	CMixFile::LoadMixFile(FileNames::MIX_DESERT);
	CMixFile::LoadMixFile(FileNames::MIX_DES);
	CMixFile::LoadMixFile(FileNames::MIX_ISODESMD);
	CMixFile::LoadMixFile(FileNames::MIX_ISODES);

	CMixFile::LoadMixFile(FileNames::MIX_URBANNMD);
	CMixFile::LoadMixFile(FileNames::MIX_URBANN);
	CMixFile::LoadMixFile(FileNames::MIX_UBN);
	CMixFile::LoadMixFile(FileNames::MIX_ISOUBNMD);
	CMixFile::LoadMixFile(FileNames::MIX_ISOUBN);

	CMixFile::LoadMixFile(FileNames::MIX_LUNARMD);
	CMixFile::LoadMixFile(FileNames::MIX_LUNAR);
	CMixFile::LoadMixFile(FileNames::MIX_LUN);
	CMixFile::LoadMixFile(FileNames::MIX_ISOLUNMD);
	CMixFile::LoadMixFile(FileNames::MIX_ISOLUN);
}

void CLoading::InitPalettes()
{
	CPalFile::CreateInstance(FileNames::PAL_ISOTEM);
	CPalFile::CreateInstance(FileNames::PAL_ISOSNO);
	CPalFile::CreateInstance(FileNames::PAL_ISOURB);
	CPalFile::CreateInstance(FileNames::PAL_ISODES);
	CPalFile::CreateInstance(FileNames::PAL_ISOUBN);
	CPalFile::CreateInstance(FileNames::PAL_ISOLUN);
	
	CPalFile::CreateInstance(FileNames::PAL_UNITTEM);
	CPalFile::CreateInstance(FileNames::PAL_UNITSNO);
	CPalFile::CreateInstance(FileNames::PAL_UNITURB);
	CPalFile::CreateInstance(FileNames::PAL_UNITDES);
	CPalFile::CreateInstance(FileNames::PAL_UNITUBN);
	CPalFile::CreateInstance(FileNames::PAL_UNITLUN);

	CPalFile::CreateInstance(FileNames::PAL_TEMPERAT);
	CPalFile::CreateInstance(FileNames::PAL_SNOW);
	CPalFile::CreateInstance(FileNames::PAL_URBAN);
	CPalFile::CreateInstance(FileNames::PAL_DESERT);
	CPalFile::CreateInstance(FileNames::PAL_URBANN);
	CPalFile::CreateInstance(FileNames::PAL_LUNAR);

	CPalFile::CreateInstance(FileNames::PAL_LIB);
}

bool CLoading::LoadTSINI(const char* name, CINI* pINI)
{
	CFileClass file;
	if (file.Open(name))
		return pINI->Parse(reinterpret_cast<char*>(file.GetBuffer()), file.GetSize());
	return false;
}

void CLoading::InitINIFiles()
{
	auto Load_INI = [this](const char* name, CINI* pINI, bool critical = false)
	{
		if (!this->LoadTSINI(name, pINI))
		{
			CFinalSunLog::Instance.Write("Failed to parse ini file %s!\n");
			if (critical)
				exit(98);
		}
	};

	Load_INI(FileNames::INI_Rules, &CINI::Rules, true);
	Load_INI(FileNames::INI_Art, &CINI::Art, true);
	Load_INI(FileNames::INI_Sound, &CINI::Sound);
	Load_INI(FileNames::INI_Eva, &CINI::Eva);
	Load_INI(FileNames::INI_Theme, &CINI::Theme);
	Load_INI(FileNames::INI_Ai, &CINI::Ai);
	Load_INI(FileNames::INI_Temperate, &CINI::Temperate, true);
	Load_INI(FileNames::INI_Snow, &CINI::Snow, true);
	Load_INI(FileNames::INI_Urban, &CINI::Urban, true);
	Load_INI(FileNames::INI_Desert, &CINI::Desert, true);
	Load_INI(FileNames::INI_NewUrban, &CINI::NewUrban, true);
	Load_INI(FileNames::INI_Lunar, &CINI::Lunar, true);
}

void CLoading::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoading, CDialogEx)
END_MESSAGE_MAP()
