#pragma once
#include "WindowsProject1.h"
#define PLAYER_PLAY 1
#define PLAYER_STOP 2
#define PLAYER_IDLE 0

HWND hWnd_Wallpaper = NULL;
std::string WINDOW_NAME = "wallpaper";
int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
int PLAYER_STATE = PLAYER_IDLE;

HWND workerw = NULL;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	HWND p = FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL);
	if (p == NULL) return 1;
	workerw = FindWindowEx(NULL, hwnd, L"WorkerW", NULL);
}

void GetWallpaperHwnd() {
	HWND hProgman = FindWindowA("Progman", "Program Manager");
	if (!hProgman) MessageBox(NULL, L"找不到Progman視窗", L"錯誤", MB_OK);
	SendMessageA(hProgman, (UINT)1324, 0, 0);

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
