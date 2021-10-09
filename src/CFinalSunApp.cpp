
// relert-plus-plus.cpp : Defines the class behaviors for the application.
//

#include <CFinalSunApp.h>
#include <CFinalSunDlg.h>

#include <CINI.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CFinalSunApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CFinalSunApp::CFinalSunApp() = default;

CFinalSunApp::~CFinalSunApp()
{
	CFinalSunLog::Instance.Close();
}

CFinalSunApp theApp;

BOOL CFinalSunApp::InitInstance()
{
	// Original FA2 CTOR
	this->ClipboardFormat = RegisterClipboardFormat("RELERT*FORMAT"); // FINAL*FORMAT
	if (!this->ClipboardFormat)
		::MessageBox(0, "Failed to register clipboard format, clipboard functions not available", "", MB_OK);

	GetModuleFileName(0, this->ExePath, MAX_PATH);
	strrchr(this->ExePath, '\\')[1] = '\0';

	this->Language = "English";
	this->FlatToGround = false;
	this->MarbleLoaded = false;
	this->FrameMode = false;

	this->GlobalBuffer = this->ExePath;
	this->GlobalBuffer += "\\finalalert2log.txt";

	CFinalSunLog::Instance.Open(this->GlobalBuffer);
	CFinalSunLog::Instance.Log("FinalAlert 2(tm) log file\n----------------------\n\n");
	CFinalSunLog::Instance.Log("CFinalSunApp::CFinalSunApp() called\n");

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	if (!InitCommonControlsEx(&InitCtrls))
	{
		CFinalSunLog::Instance.Log("Error: Common controls could not be initialized\n");
		::MessageBox(NULL, "Common controls could not be initialized.", "Error", MB_OK);
	}

	CFinalSunLog::Instance.Log("\n\n\n");

	// Original FA2 InitInstance begins

	CWinApp::InitInstance();
	AfxEnableControlContainer();
	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;
	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// FinalAlert 2 Codes:
	this->AccTable = LoadAccelerators(this->m_hInstance, MAKEINTRESOURCE(IDD_ACCEL));
	const HWND hDesktopWnd = ::GetDesktopWindow();
	const HDC hDesktopDC = ::GetDC(hDesktopWnd);
	if (::GetDeviceCaps(hDesktopDC, BITSPIXEL) <= 8)
	{
		::MessageBox(
			NULL,
			"You currently only have 8 bit color mode enabled. FinalAlert 2(tm) will "
			"not work in 8 bitcolor mode. See readme.txt for further information!",
			"Error", 
			MB_OK
		);
		exit(0);
	}

	// CMDLine stuff
	memset(this->MapPath, 0, MAX_PATH);

	// CINI stuff(fadata and falanguage)
	this->GlobalBuffer = this->ExePath;
	this->GlobalBuffer += "\\FAData.ini";
	CINI::FAData.ReadFromFile(this->GlobalBuffer);

	this->GlobalBuffer = this->ExePath;
	this->GlobalBuffer += "\\FALanguage.ini";
	CINI::FALanguage.ReadFromFile(this->GlobalBuffer);
	if (CINI::FALanguage.GetSectionCount() == 0)
	{
		::MessageBox(
			NULL,
			"FALanguage.ini does not exist or is not valid (download corrupt?)",
			"",
			MB_OK
		);
		exit(0);
	}

	// FinalAlert INI, get FilePath, initialize members like ShowBuildingCells
	this->GlobalBuffer = this->ExePath;
	this->GlobalBuffer += "\\FinalAlert.ini";
	CINI::FinalAlert.ReadFromFile(this->GlobalBuffer);

	this->DisableAutoShore = CINI::FinalAlert.ReadBoolean("UserInterface", "DisableAutoShore");
	this->ShowBuildingCells = CINI::FinalAlert.ReadBoolean("UserInterface", "ShowBuildingCells");
	this->DisableAutoLat = CINI::FinalAlert.ReadBoolean("UserInterface", "DisableAutoLat");
	this->NoSounds = CINI::FinalAlert.ReadBoolean("UserInterface", "NoSounds");
	this->DisableSlopeCorrection = CINI::FinalAlert.ReadBoolean("UserInterface", "DisableSlopeCorrection");
	
	this->RecentFiles[0] = CINI::FinalAlert.ReadString("Files", "0");
	this->RecentFiles[1] = CINI::FinalAlert.ReadString("Files", "1");
	this->RecentFiles[2] = CINI::FinalAlert.ReadString("Files", "2");
	this->RecentFiles[3] = CINI::FinalAlert.ReadString("Files", "3");

	// Create CLoading

	CFinalSunDlg cFinalSunDlg;
	this->m_pMainWnd = &cFinalSunDlg;
	cFinalSunDlg.DoModal();

	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

	return FALSE;
}

