#pragma once

#include <Always.h>

class CLoading : public CDialogEx
{
	DECLARE_DYNAMIC(CLoading)

public:
	CLoading(CWnd* pParent = nullptr);
	virtual ~CLoading();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLOADING };
#endif

	void Load();
	void InitMixFiles();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

private:

};
