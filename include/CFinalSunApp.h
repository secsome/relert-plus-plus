#pragma once

#include <Always.h>

class CLoading;

class CFinalSunApp : public CWinApp
{
public:
	CFinalSunApp();
	~CFinalSunApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

// Properties
	CString GlobalBuffer;
	char ExePath[MAX_PATH];
	char FilePath[MAX_PATH];
	char MapPath[MAX_PATH];
	CString Language;
	CString RecentFiles[4];
	bool FlatToGround;
	bool FrameMode;
	bool MarbleLoaded;
	bool ShowBuildingCells;
	bool DisableAutoShore;
	bool DisableAutoLat;
	bool NoSounds;
	bool DisableSlopeCorrection;
	CLoading* Loading;
	UINT ClipboardFormat;
	HACCEL AccTable;
};

extern CFinalSunApp theApp;
