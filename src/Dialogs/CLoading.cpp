#include "CLoading.h"

#include <CMixFile.h>
#include <CPalFile.h>
#include <CFileClass.h>

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
}

void CLoading::InitMixFiles()
{
	CString mixName;

	// Init_Bootstrap_Mixfiles
	for (int i = 99; i >= 0; --i)
	{
		mixName.Format("EXPANDMD%02d.MIX", i);
		CMixFile::LoadMixFile(mixName);
	}

	CMixFile::LoadMixFile("RA2MD.MIX");
	CMixFile::LoadMixFile("RA2.MIX");
	CMixFile::LoadMixFile("CACHEMD.MIX");
	CMixFile::LoadMixFile("CACHE.MIX");
	CMixFile::LoadMixFile("LOCALMD.MIX");
	CMixFile::LoadMixFile("LOCAL.MIX");

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

	Finder_Load_Mix("ECACHE*.MIX");
	Finder_Load_Mix("ELOCAL*.MIX");

	// Init_Secondary_Mixfiles
	CMixFile::LoadMixFile("CONQMD.MIX");
	CMixFile::LoadMixFile("GENERMD.MIX");
	CMixFile::LoadMixFile("GENERIC.MIX");
	CMixFile::LoadMixFile("ISOGEN.MIX");
	CMixFile::LoadMixFile("CONQUER.MIX");
	CMixFile::LoadMixFile("CAMEOMD.MIX");
	CMixFile::LoadMixFile("CAMEO.MIX");
	// Finder_Load_Mix("MAPS*.MIX");
	// CMixFile::LoadMixFile("MAPSMD%02d.MIX");
	CMixFile::LoadMixFile("MULTIMD.MIX");
	if (!CMixFile::LoadMixFile("THEMEMD.MIX"))
		CMixFile::LoadMixFile("THEME.MIX");
	// MOVIES

	// Init_Theaters
	CMixFile::LoadMixFile("TEMPERATMD.MIX");
	CMixFile::LoadMixFile("TEMPERAT.MIX");
	CMixFile::LoadMixFile("TEM.MIX");
	CMixFile::LoadMixFile("ISOTEMMD.MIX");
	CMixFile::LoadMixFile("ISOTEMP.MIX");

	CMixFile::LoadMixFile("SNOWMD.MIX");
	CMixFile::LoadMixFile("SNOW.MIX");
	CMixFile::LoadMixFile("SNO.MIX");
	CMixFile::LoadMixFile("ISOSNOMD.MIX");
	CMixFile::LoadMixFile("ISOSNOW.MIX");
	
	CMixFile::LoadMixFile("URBANMD.MIX");
	CMixFile::LoadMixFile("URBAN.MIX");
	CMixFile::LoadMixFile("URB.MIX");
	CMixFile::LoadMixFile("ISOURBMD.MIX");
	CMixFile::LoadMixFile("ISOURB.MIX");

	CMixFile::LoadMixFile("DESERTMD.MIX");
	CMixFile::LoadMixFile("DESERT.MIX");
	CMixFile::LoadMixFile("DES.MIX");
	CMixFile::LoadMixFile("ISODESMD.MIX");
	CMixFile::LoadMixFile("ISODES.MIX");

	CMixFile::LoadMixFile("URBANNMD.MIX");
	CMixFile::LoadMixFile("URBANN.MIX");
	CMixFile::LoadMixFile("UBN.MIX");
	CMixFile::LoadMixFile("ISOUBNMD.MIX");
	CMixFile::LoadMixFile("ISOUBN.MIX");

	CMixFile::LoadMixFile("LUNARMD.MIX");
	CMixFile::LoadMixFile("LUNAR.MIX");
	CMixFile::LoadMixFile("LUN.MIX");
	CMixFile::LoadMixFile("ISOLUNMD.MIX");
	CMixFile::LoadMixFile("ISOLUN.MIX");
}

void CLoading::InitPalettes()
{
	CPalFile::CreateInstance("ISOTEM.PAL");
	CPalFile::CreateInstance("ISOSNO.PAL");
	CPalFile::CreateInstance("ISOURB.PAL");
	CPalFile::CreateInstance("ISODES.PAL");
	CPalFile::CreateInstance("ISOUBN.PAL");
	CPalFile::CreateInstance("ISOLUN.PAL");
	
	CPalFile::CreateInstance("UNITTEM.PAL");
	CPalFile::CreateInstance("UNITSNO.PAL");
	CPalFile::CreateInstance("UNITURB.PAL");
	CPalFile::CreateInstance("UNITDES.PAL");
	CPalFile::CreateInstance("UNITUBN.PAL");
	CPalFile::CreateInstance("UNITLUN.PAL");

	CPalFile::CreateInstance("TEMPERAT.PAL");
	CPalFile::CreateInstance("SNOW.PAL");
	CPalFile::CreateInstance("URBAN.PAL");
	CPalFile::CreateInstance("DESERT.PAL");
	CPalFile::CreateInstance("URBAN.PAL");
	CPalFile::CreateInstance("URBANN.PAL");

	CPalFile::CreateInstance("LIB.PAL");
}

void CLoading::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoading, CDialogEx)
END_MESSAGE_MAP()


// CLoading 消息处理程序
