#include "framework.h"
#include "WindowsProject1.h"
#include "Wallpaper.h"
#include "NotifyIcon.h"
#include "resource.h"
#include <vlc/vlc.h>
#include <commdlg.h>

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING] = L"Live Wallpaper";
WCHAR szWindowClass[MAX_LOADSTRING] = L"LWALLPAPER";

std::string filePath = "";
HWND hWnd_CHOOSEFILE = NULL;
HWND hWnd_PLAY = NULL;
HWND hWnd_STOP = NULL;
HWND hWnd_EXIT = NULL;
HWND hWnd_HIDE = NULL;
HWND hWnd_PATHTEXTBOX = NULL;

libvlc_media_list_player_t* mediaListPlayer = NULL;

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	HWND hWnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		szWindowClass,
		szTitle,
		WS_OVERLAPPED,
		CW_USEDEFAULT,
		0,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

std::string TCHAR2STRING(TCHAR* str) {
	std::string strstr;
	try
	{
		int iLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);

		char* chRtn = new char[iLen * sizeof(char)];

		WideCharToMultiByte(CP_ACP, 0, str, -1, chRtn, iLen, NULL, NULL);

		strstr = chRtn;
	}
	catch (...) {}

	return strstr;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	NOTIFYICONDATA nid = { 0 };
	UINT WM_TASKBARCREATED;

	if (hWnd == NULL) return 0;
	WM_TASKBARCREATED = RegisterWindowMessage(TEXT("TaskbarCreated"));

	switch (message)
	{
	case WM_CREATE:
	{
#pragma region Create Contorls

		hWnd_CHOOSEFILE = CreateWindow(
			L"BUTTON",
			L"選擇檔案",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD,
			10,
			10,
			80,
			20,
			hWnd,
			(HMENU)USR_CHOOSE_FILE,
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			NULL
		);

		hWnd_PLAY = CreateWindow(
			L"BUTTON",
			L"播放",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_DISABLED,
			100,
			10,
			80,
			20,
			hWnd,
			(HMENU)USR_PLAY,
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			NULL
		);

		hWnd_STOP = CreateWindow(
			L"BUTTON",
			L"停止",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_DISABLED,
			190,
			10,
			80,
			20,
			hWnd,
			(HMENU)USR_STOP,
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			NULL
		);

		hWnd_EXIT = CreateWindow(
			L"BUTTON",
			L"退出",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			280,
			10,
			80,
			20,
			hWnd,
			(HMENU)USR_EXIT,
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			NULL
		);

		hWnd_HIDE = CreateWindow(
			L"BUTTON",
			L"隱藏",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			370,
			10,
			80,
			20,
			hWnd,
			(HMENU)USR_HIDE,
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			NULL
		);

		hWnd_PATHTEXTBOX = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"EDIT",
			L"",
			WS_CHILD | WS_VISIBLE | WS_DISABLED,
			10,
			40,
			440,
			20,
			hWnd,
			NULL,
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			NULL
		);
#pragma endregion

		nid.cbSize = __GetSizeNotifyIconData();
		nid.hWnd = hWnd;
		nid.uID = MINISITE_NEWS_TRAY_ICON_ID;
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
		nid.uCallbackMessage = WM_USER;
		nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
		nid.uTimeout = 1000;
		nid.uVersion = NOTIFYICON_VERSION_4;
		lstrcpy(nid.szTip, szWindowClass);
		nid.dwInfoFlags = NIIF_USER;
		Shell_NotifyIcon(NIM_SETVERSION, &nid);
		Shell_NotifyIcon(NIM_ADD, &nid);

		GetWallpaperHwnd();
		break;
	}

	case WM_COMMAND:
	{
		int param = LOWORD(wParam);
		switch (param)
		{
		case USR_CHOOSE_FILE:
		{
			TCHAR strFileName[MAX_PATH] = { 0 };
			OPENFILENAME ofn = { 0 };
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = TEXT("*.mp4\0*.mp4\0*.avi\0*.avi\0\0");
			ofn.nFilterIndex = 1;
			ofn.lpstrFile = strFileName;
			ofn.nMaxFile = sizeof(strFileName);
			ofn.lpstrInitialDir = NULL;
			ofn.lpstrTitle = NULL;
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
			if (GetOpenFileName(&ofn)) {
				filePath = TCHAR2STRING(strFileName);
				SetWindowText(hWnd_PATHTEXTBOX, strFileName);
				SetWindowLongPtr(hWnd_PLAY, GWL_STYLE, WS_VISIBLE | WS_CHILD);
				SetWindowLongPtr(hWnd_STOP, GWL_STYLE, WS_VISIBLE | WS_CHILD);
			}
			else {
				MessageBox(NULL, TEXT("請選擇一文件"), NULL, MB_ICONERROR);
			}
			break;
		}

		case USR_PLAY:
		{
			if (mediaListPlayer != NULL && libvlc_media_list_player_is_playing(mediaListPlayer)) {
				libvlc_media_list_player_stop(mediaListPlayer);
			}
			
			libvlc_instance_t* libvlcInst = libvlc_new(0, NULL);
			if (libvlcInst == NULL) {
				MessageBox(hWnd, L"Cannot Create Libvlc instance", L"Error", MB_ICONERROR);
				break;
			}

			libvlc_media_t* media = libvlc_media_new_path(libvlcInst, filePath.c_str());
			libvlc_media_list_t* mediaList = libvlc_media_list_new(libvlcInst);
			libvlc_media_player_t* mediaPlayer = libvlc_media_player_new_from_media(media);
			mediaListPlayer = libvlc_media_list_player_new(libvlcInst);

			libvlc_media_list_add_media(mediaList, media);
			libvlc_media_release(media);

			libvlc_media_list_player_set_media_list(mediaListPlayer, mediaList);
			libvlc_media_list_player_set_media_player(mediaListPlayer, mediaPlayer);
			libvlc_media_list_player_set_playback_mode(mediaListPlayer, libvlc_playback_mode_loop);
			libvlc_media_player_set_hwnd(mediaPlayer, workerw);
			libvlc_media_list_player_play(mediaListPlayer);
			break;
		}

		case USR_STOP: 
		{
			libvlc_media_list_player_stop(mediaListPlayer);
			RefreshWallpaper();
			break;
		}

		case USR_EXIT:
		{
			RefreshWallpaper();
			SendMessage(hWnd, WM_DESTROY, NULL, NULL);
			break;
		}

		case USR_HIDE:
		{
			ShowWindow(hWnd, SW_HIDE);
			break;
		}

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	}

	case WM_USER:
	{
		if (lParam == WM_LBUTTONDOWN) {
			ShowWindow(hWnd, SW_SHOW);
		}
		break;
	}

	case WM_DESTROY:
		Shell_NotifyIcon(NIM_DELETE, &nid);
		PostQuitMessage(0);
		break;

	default:
		if (message == WM_TASKBARCREATED)
			SendMessage(hWnd, WM_CREATE, wParam, lParam);
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
