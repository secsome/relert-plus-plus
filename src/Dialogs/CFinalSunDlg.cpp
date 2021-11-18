#include <CFinalSunApp.h>
#include <CFinalSunDlg.h>

#include <CCRC.h>
#include <CINI.h>
#include <CMixFile.h>

CFinalSunDlg::CFinalSunDlg(CWnd* pParent)
	: CDialogEx(IDD_CFINALSUN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CFinalSunDlg::~CFinalSunDlg()
{
	this->CDialogEx::~CDialogEx();
}

void CFinalSunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFinalSunDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CFinalSunDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CFinalSunLog::Instance.Log(__FUNCTION__" called\n");
	this->ShowWindow(SW_HIDE);
	const int cx = GetSystemMetrics(SM_CXFULLSCREEN);
	const int cy = GetSystemMetrics(SM_CYFULLSCREEN);
	this->SetWindowPos(nullptr, 0, 0, cx, cy, NULL);
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	this->SetWindowText("Relert-sharp-sharp - (NoMapLoaded)");
	// Create CFrameWnd
	// Create CRebarCtrl
	// Create ToolBar3
	// Create ToolBar2
	// Create ToolBar1
	// Show Rebar
	// Show Toolbar3
	// Show Toolbar2
	// Show BrushSize
	// if (failed to create rebar)
	CFinalSunLog::Instance.Log("Updating menu\n");
	// Update dialogs
	/*if (this->CLoading.m_hWnd == NULL)
	{
		this->MessageBox("Loading dialog could not be created. Please close any programs and start again.");
		exit(-99);
	}*/
	// this->CLoading.ShowWindow(SW_SHOW);
	this->CLoading.Load();

	// Marble loaded

	this->ShowWindow(SW_SHOWMAXIMIZED);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFinalSunDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

void CFinalSunDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFinalSunDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

