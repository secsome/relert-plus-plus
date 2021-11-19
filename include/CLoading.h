#pragma once

#include <Always.h>

class CINI;

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
	void InitPalettes();
	bool LoadTSINI(const char* name, CINI* pINI);
	void InitINIFiles();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

private:

};
