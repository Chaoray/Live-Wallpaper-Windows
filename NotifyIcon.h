#pragma once
#include "WindowsProject1.h"
#include <shlwapi.h>
#include <tchar.h>
#include <winbase.h>
#include <versionhelpers.h>
#define MINISITE_NEWS_TRAY_ICON_ID 26
#define TRAYICON_TIMEROUT_ID 30

ULONGLONG __GetVersion(LPCTSTR lpszDllName)
{
	HINSTANCE hinstDll;
	ULONGLONG dwVersion = 0;
	hinstDll = LoadLibrary(lpszDllName);
	if (hinstDll)
	{
		DLLGETVERSIONPROC pDllGetVersion;
		pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, "DllGetVersion");
		if (pDllGetVersion)
		{
			DLLVERSIONINFO dvi;
			HRESULT hr;
			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);
			hr = (*pDllGetVersion)(&dvi);
			if (SUCCEEDED(hr))
			{
				dwVersion = MAKEDLLVERULL(dvi.dwMajorVersion, dvi.dwMinorVersion,
					dvi.dwBuildNumber, dvi.dwPlatformID);
			}
		}
		FreeLibrary(hinstDll);
	}
	return dwVersion;
}


DWORD __GetSizeNotifyIconData()
{
	TCHAR lpszDllName[128];
	GetWindowsDirectory(lpszDllName, _TRUNCATE);
	_tcsncat_s(lpszDllName, __T("\\System32\\Shell32.dll"), _TRUNCATE);
	ULONGLONG NIDdllVer = __GetVersion(lpszDllName);
	// before windows 2000
	if (NIDdllVer < MAKEDLLVERULL(5, 0, 0, 0)) {
		return NOTIFYICONDATA_V1_SIZE;
	}
	// Windows 2000
	else if (NIDdllVer < MAKEDLLVERULL(6, 0, 0, 0)) {
		return NOTIFYICONDATA_V2_SIZE;
	}
	// Windows XP or 2003
	else if (NIDdllVer < MAKEDLLVERULL(6, 0, 6000, 0)) {
		return NOTIFYICONDATA_V3_SIZE;
	}
	// Windows Vista and later
	else {
		return sizeof(NOTIFYICONDATA);
	}
}