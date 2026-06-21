///////////////////////////////////////////////////////////////////////
/// @file paint.cpp
/// @brief PhotoNest 绘图应用入口程序
/// @details 实现绘图应用的 main 函数,调用 photol DLL 提供的
///          图像处理功能,支持命令行参数调用
/// @author PhotoNest Team
/// @date 2024
/// @version 1.0
/// @note 本文件是绘图应用的入口点,使用 photol 库进行图像处理
///////////////////////////////////////////////////////////////////////

//

#include <tchar.h>
#include <windows.h>

typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

#include "../../photol/photol/photol.h"

#pragma comment(linker, "/subsystem:windows /ENTRY:wmainCRTStartup")
#pragma comment( lib, "../../../PhotoNest/tests/cefclient/Release/bin/photol.lib" ) 

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc == 9)
	{
		HMODULE hModule = ::GetModuleHandle(NULL);
		wchar_t buf[MAX_PATH] = { 0 };
		GetModuleFileNameW(hModule, buf, MAX_PATH);// L"mypaint_mutex"
		wchar_t* p = wcsrchr(buf, L'\\');
		if (p != NULL)
		{
			p[0] = 0;
		}

		wchar_t buf2[MAX_PATH] = { 0 };
		swprintf_s(buf2, MAX_PATH, L"mypaint_%s_%d", argv[6], (int)wcslen(buf));

		HANDLE paint_mgr = NULL;
		paint_mgr = CreateMutex(
			(LPSECURITY_ATTRIBUTES)NULL,
			(BOOL)TRUE,
			buf2
		);

		DWORD dwLastError = GetLastError();

		if (paint_mgr == NULL || dwLastError == ERROR_ALREADY_EXISTS)
		{
			//window_w32.cpp
			//HWND child = NULL;
			//do {
			//	child = FindWindowEx(NULL, child, NULL, NULL);

			//	wchar_t buf[MAX_PATH];
			//	memset(buf, 0, sizeof(wchar_t)*MAX_PATH);
			//	::GetWindowText(child, buf, MAX_PATH);

			//	if (wcsstr(buf, L"PhotoNest") != NULL || wcsstr(buf, L"PhotoNest Encryption") != NULL)
			//	{

			//		ATOM atom = GlobalAddAtom(argv[3]);

			//		UINT WM_PAINT_FILE = WM_USER + 113;
			//		PostMessageW(child, WM_PAINT_FILE, 0, (LPARAM)atom);


			//		ShowWindow(child, SW_RESTORE);
			//		SetForegroundWindow(child);
			//		break;
			//	}

			//} while (child);

		}
		else
		{
			uint64_t hParent = (uint64_t)_wtoi64(argv[1]);
			int type = (int)_wtoi(argv[2]);
			int pixelsy = (int)_wtoi(argv[3]);
			int ov = (int)_wtoi(argv[8]);
			int ret = paint_show(type, hParent, pixelsy, 1152, 768, argv[4], argv[5], argv[6], argv[7], ov);
		}

		if (paint_mgr != NULL)
		{
			CloseHandle(paint_mgr);
		}
	}
	return 0;
}

