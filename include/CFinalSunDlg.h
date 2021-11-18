#pragma once

#include <Always.h>

#include <CLoading.h>

class CFinalSunDlg : public CDialogEx
{
// Construction
public:
	CFinalSunDlg(CWnd* pParent = nullptr);
	virtual ~CFinalSunDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CFINALSUN_DIALOG };
#endif

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CLoading CLoading;
};
