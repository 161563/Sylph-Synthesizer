#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <tchar.h>
#include <shellapi.h>
#include <atlbase.h>
#include <iostream>
#include <string>
#include <fstream>

#include "resource.h"
#include "Song.h"

#define MAX_STRING_SIZE 100

WCHAR szTitle[] = _T("Synthesizer");
WCHAR szWindowClass[] = _T("Synth");
HINSTANCE Instance;

Song* song;
bool loaded;
bool cancel;

bool load(std::string file);

std::wstring s2ws(std::string string) {
	int len;
	int slength = (int)string.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, string.c_str(), slength, 0, 0);
	wchar_t* buffer = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, string.c_str(), slength, buffer, len);
	std::wstring r(buffer);
	delete[] buffer;
	return r;
}

ATOM createScreen(HINSTANCE hInstance);

LRESULT CALLBACK WndProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
INT_PTR CALLBACK SaveDialog(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrev,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {

	std::freopen("log.txt", "w", stdout);

	int argc;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	

	if (!createScreen(hInstance)) {
		MessageBox(NULL, _T("registering window failed"), _T("synthesizer"), NULL);
		return -1;
	}

	HWND window = CreateWindow(
		szWindowClass, szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1080,
		766,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!window) {
		MessageBox(NULL, _T("creating window failed"), _T("synthesizer"), NULL);
		return -1;
	}

	HACCEL accel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	ShowWindow(window, nCmdShow);
	UpdateWindow(window);

	Instance = hInstance;

	if (argc == 2) {
		song = new Song(std::string(CW2A(argv[1])));
	}
	else {
		song = new Song();
	}

	loaded = true;

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		TranslateAccelerator(window, accel, &msg);
		DispatchMessage(&msg);
	}

	return 0;

}

bool load(std::string location){
	std::ifstream in(location);

	if (!in.is_open()) {
		std::cout << "file " << location << " not found" << std::endl;
		return false;
	}
	else {
		in.close();
		return true;
	}
}

LRESULT CALLBACK WndProc(
	_In_ HWND hwnd,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam) {

	HDC hdc;
	PAINTSTRUCT ps;

	switch (uMsg) {
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
	}
		break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);

		EndPaint(hwnd, &ps);
	}
		break;
	case WM_COMMAND:
	{
		int wmid = LOWORD(wParam);
		switch (wmid) {
		case ID_FILE_NEW:
		{
			if (loaded) {
				cancel = false;
				DialogBox(Instance, MAKEINTRESOURCE(IDD_SAVE_DIALOG), hwnd, SaveDialog);
				if (!cancel) {
					delete song;
					song = new Song();
				}
			}

		}
		break;
		case ID_FILE_LOAD:
		{
			if (loaded) {
				cancel = false;
				DialogBox(Instance, MAKEINTRESOURCE(IDD_SAVE_DIALOG), hwnd, SaveDialog);
			}
			if (!cancel||!loaded) {

				wchar_t fileName[4096];

				OPENFILENAME openFileName;

				ZeroMemory(fileName, sizeof(fileName));

				ZeroMemory(&openFileName, sizeof(openFileName));

				openFileName.lStructSize = sizeof(openFileName);
				openFileName.hwndOwner = NULL;
				openFileName.lpstrFile = fileName;
				openFileName.lpstrFile[0] = '\0';
				openFileName.nMaxFile = sizeof(fileName);
				openFileName.lpstrFilter = _T("Note Files\0*.not\0Any File\0*.*\0");
				openFileName.nFilterIndex = 0;
				openFileName.lpstrFileTitle = NULL;
				openFileName.nMaxFileTitle = 0;
				openFileName.lpstrInitialDir = NULL;
				openFileName.lpstrTitle = _T("Select the file to save to");
				openFileName.Flags = OFN_DONTADDTORECENT | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				if (GetOpenFileName(&openFileName)) {
					delete song;
					song = new Song(std::string(CW2A(fileName)));
				}
			}
		}
		break;
		case ID_FILE_SAVE:
		{
			if (song->hasSaveLocation()) {
				song->save(song->getSaveLocation());
			}

			wchar_t fileName[4096];

			OPENFILENAME openFileName;

			ZeroMemory(fileName, sizeof(fileName));

			ZeroMemory(&openFileName, sizeof(openFileName));

			openFileName.lStructSize = sizeof(openFileName);
			openFileName.hwndOwner = NULL;
			openFileName.lpstrFile = fileName;
			openFileName.lpstrFile[0] = '\0';
			openFileName.nMaxFile = sizeof(fileName);
			openFileName.lpstrFilter = _T("Note Files\0*.not\0Any File\0*.*\0");
			openFileName.nFilterIndex = 0;
			openFileName.lpstrFileTitle = NULL;
			openFileName.nMaxFileTitle = 0;
			openFileName.lpstrInitialDir = NULL;
			openFileName.lpstrTitle = _T("Select the file to save to");
			openFileName.Flags = OFN_DONTADDTORECENT | OFN_PATHMUSTEXIST;

			if (GetSaveFileName(&openFileName)) {
				song->save(std::string(CW2A(fileName)));
			}

		}
		break;
		case ID_FILE_SAVEAS:
		{
			wchar_t fileName[4096];

			OPENFILENAME openFileName;

			ZeroMemory(fileName, sizeof(fileName));

			ZeroMemory(&openFileName, sizeof(openFileName));

			openFileName.lStructSize = sizeof(openFileName);
			openFileName.hwndOwner = NULL;
			openFileName.lpstrFile = fileName;
			openFileName.lpstrFile[0] = '\0';
			openFileName.nMaxFile = sizeof(fileName);
			openFileName.lpstrFilter = _T("Note Files\0*.not\0Any File\0*.*\0");
			openFileName.nFilterIndex = 0;
			openFileName.lpstrFileTitle = NULL;
			openFileName.nMaxFileTitle = 0;
			openFileName.lpstrInitialDir = NULL;
			openFileName.lpstrTitle = _T("Select the file to save to");
			openFileName.Flags = OFN_DONTADDTORECENT | OFN_PATHMUSTEXIST;

			if (GetSaveFileName(&openFileName)) {
				song->save(std::string(CW2A(fileName)));
			}
		}
		break;
		default:
			MessageBox(hwnd, std::to_wstring(wmid).c_str(), L"action", NULL);
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}
	return 0;

	
	return 0;
}

ATOM createScreen(HINSTANCE hInstance) {
	WNDCLASSEX window;

	window.cbSize = sizeof(WNDCLASSEX);
	window.style = CS_HREDRAW | CS_VREDRAW;
	window.lpfnWndProc = WndProc;
	window.cbClsExtra = 0;
	window.cbWndExtra = 0;
	window.hInstance = hInstance;
	window.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	window.hCursor = LoadCursor(NULL, IDC_ARROW);
	window.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	window.lpszMenuName = MAKEINTRESOURCEW(IDR_MENU1);
	window.lpszClassName = szWindowClass;
	window.hIconSm = LoadIcon(window.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassEx(&window);

}

INT_PTR CALLBACK SaveDialog(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {

	switch (uMsg) {
	case WM_INITDIALOG:
		return (INT_PTR)true;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_SAVE_DIALOG_SAVE) {
			if (song->hasSaveLocation()) {
				song->save(song->getSaveLocation());
			}
			
			wchar_t fileName[4096];
			
			OPENFILENAME openFileName;

			ZeroMemory(fileName, sizeof(fileName));
			
			ZeroMemory(&openFileName, sizeof(openFileName));
			
			openFileName.lStructSize = sizeof(openFileName);
			openFileName.hwndOwner = NULL;
			openFileName.lpstrFile = fileName;
			openFileName.lpstrFile[0] = '\0';
			openFileName.nMaxFile = sizeof(fileName);
			openFileName.lpstrFilter = _T("Note Files\0*.not\0Any File\0*.*\0");
			openFileName.nFilterIndex = 0;
			openFileName.lpstrFileTitle = NULL;
			openFileName.nMaxFileTitle = 0;
			openFileName.lpstrInitialDir = NULL;
			openFileName.lpstrTitle = _T("Select the file to save to");
			openFileName.Flags = OFN_DONTADDTORECENT | OFN_PATHMUSTEXIST;

			if (GetSaveFileName(&openFileName)) {
				song->save(std::string(CW2A(fileName)));
				EndDialog(hwnd, LOWORD(wParam));
				return (INT_PTR)true;
			}

		}
		if (LOWORD(wParam) == IDC_SAVE_DIALOG_NOSAVE) {
			EndDialog(hwnd, LOWORD(wParam));
			return (INT_PTR)true;
		}
		if (LOWORD(wParam) == IDC_SAVE_DIALOG_CANCEL) {
			EndDialog(hwnd, LOWORD(wParam));
			cancel = true;
			return (INT_PTR)true;
		}
		break;
	}
	return (INT_PTR)false;

}