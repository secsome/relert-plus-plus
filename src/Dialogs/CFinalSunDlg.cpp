#include <CFinalSunApp.h>
#include <CFinalSunDlg.h>

#include <CINI.h>
#include <CMixFile.h>

CFinalSunDlg::CFinalSunDlg(CWnd* pParent)
	: CDialogEx(IDD_CFINALSUN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	std::ifstream fin;
	fin.open("D:\\Games\\Mental Omega\\Mental Omega 3.3.6\\expandmo98.mix", std::ios::in | std::ios::binary);
	if (fin.is_open())
	{
		fin.seekg(0, std::ios::end);
		const int size = static_cast<int>(fin.tellg());
		if (size == 0)
			return false;

		fin.seekg(0, std::ios::beg);
		auto pBuffer = new byte[size];
		fin.read((char*)pBuffer, size);
		fin.close();
		CMixFile expandmo98(pBuffer);
		expandmo98.LoadToMemory();
	}


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

