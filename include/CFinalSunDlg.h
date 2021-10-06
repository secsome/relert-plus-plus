#pragma once

#include <Always.h>

class CFinalSunDlg : public CDialogEx
{
// Construction
public:
	CFinalSunDlg(CWnd* pParent = nullptr);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
