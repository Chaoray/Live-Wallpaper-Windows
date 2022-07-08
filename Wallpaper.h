#pragma once
#include "WindowsProject1.h"
#define PLAYER_PLAY 1
#define PLAYER_STOP 2
#define PLAYER_IDLE 0

HWND hWnd_Wallpaper = NULL;
HWND workerw = NULL;
std::string WINDOW_NAME = "wallpaper";
int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	HWND p = FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL);
	if (p == NULL) return 1;
	workerw = FindWindowEx(NULL, hwnd, L"WorkerW", NULL);
}

void GetWallpaperHwnd() {
	HWND hProgman = FindWindowA("Progman", "Program Manager");
	if (!hProgman) MessageBox(NULL, L"找不到Progman視窗", L"錯誤", MB_OK);
	SendMessage(hProgman, (UINT)(WM_USER + 300), 0, 0);

	EnumWindows(EnumWindowsProc, (LPARAM)NULL);

	if (workerw == NULL) {
		MessageBox(NULL, L"找不到WorkerW視窗", L"錯誤", MB_OK);
	}
}

void RefreshWallpaper() {
	WCHAR path[MAX_PATH] = { L'\0' };
	SystemParametersInfo(SPI_GETDESKWALLPAPER, MAX_PATH, &path, 0);
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, path, 0);
}
