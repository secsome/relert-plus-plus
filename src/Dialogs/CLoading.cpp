#include "CLoading.h"

#include <CMixFile.h>

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
}

void CLoading::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoading, CDialogEx)
END_MESSAGE_MAP()


// CLoading 消息处理程序
