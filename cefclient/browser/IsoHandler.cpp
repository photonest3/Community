
#include <Windows.h>
#include "timestamp.h"
#include <iostream>
#include <shellapi.h>
#include <codecvt>

#include "SimpleIni.h"
#include "IsoHandler.h"
#include "../../scheme/scheme/scheme.h" 
#include "tests/cefclient/resources/win/resource.h"
#include <process.h>
#include <shlobj.h>
#include <commdlg.h>
#include <ShObjIdl.h>
#include <ShlGuid.h>
#include "include/internal/cef_string.h"
#include "include/base/cef_logging.h"

#define LIBARCHIVE_STATIC
#include <archive.h>
#include <archive_entry.h>

//#pragma comment( lib, "./release/zlib.lib" ) 
#pragma comment( lib, "./release/archive_static.lib" ) 
vector<string> _vecLoc;

IsoHandler::IsoHandler()
{
	hwndLoading_ = NULL;

	_cancel_iso = false;
	_start_iso = false;
	_pIsoPath = L"";
	fullscreen_ = false;
	_first_f11_over = false;

	//-----------
	username_ = "";
	album_ = L"";
	enckey_ = "";
	viewer_ = 0;
	flag_video_ = false;
	disabledrag_ = "0";

	_jsclose = 0;

	_appdata = L"";
	_moduledata = L"";
	/*
	//std::string str = "Some UTF8 string";
	//CefString cef_str(str); //第一种方法
	//cef_str = str;            //第二种方法
	//cef_str.FromString(str);//第三种方法
	//str = cef_str;
	//str = cef_str.ToString();

	std::wstring str = L"Some wide string";
	CefString cef_str(str);
	cef_str = str;
	cef_str.FromWString(str);
	str = cef_str;
	str = cef_str.ToWString();


	const char* cstr = "Some ASCII string";
	//CefString cef_str;
	cef_str.FromASCII(cstr);
const char* loc = "zh-CN";

CefString(&cSettings.locale).FromASCII(loc);

cef_string_from_ascii(loc, strlen(loc), &cSettings.locale);
	*/
}

IsoHandler::~IsoHandler()
{
}

BOOL CALLBACK IsoHandler::UilanguageEnumproc(LPSTR unnamedParam1, LONG_PTR unnamedParam2)
{
	_vecLoc.push_back(unnamedParam1);
	return TRUE;
}

int IsoHandler::ShowDialogBox(HWND hwnd, UINT id, LPARAM lParam)
{
	int result = 0;
	switch (id)
	{
	case IDD_DIALOG_ISO:
		DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ISO), hwnd, ISOProc, (LPARAM)this);
		break;
	case IDD_DIALOG_REGISTER:
		DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_REGISTER), hwnd, Register, (LPARAM)this);
		break;
	case IDD_DIALOG_ABOUT:
		DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ABOUT), hwnd, About, (LPARAM)this);
		break;
	case IDD_DIALOG_ALERRT:
		DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ALERRT), hwnd, AlertProc, lParam);
		break;
	case IDD_DIALOG_CONFIRM:
		result = DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_CONFIRM), hwnd, AlertProc, lParam);
		break;
	case IDD_DIALOG_LOADING:
	{
		if (!IsWindow(hwndLoading_))
		{
			hwndLoading_ = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_LOADING), hwnd, (DLGPROC)LoadingProc);
			ShowWindow(hwndLoading_, SW_SHOWNA);

			RECT rcWin;
			GetWindowRect(hwnd, &rcWin);
			int w = rcWin.right - rcWin.left;
			int h = rcWin.bottom - rcWin.top;

			RECT rc;
			GetWindowRect(hwndLoading_, &rc);

			int left = (w - (rc.right - rc.left)) / 2 + rcWin.left;
			int top = (h - (rc.bottom - rc.top)) / 2 + rcWin.top;

			MoveWindow(hwndLoading_, left, top, rc.right - rc.left, rc.bottom - rc.top, FALSE);
			SetForegroundWindow(root_hwnd_);
		}
	}
	break;
	default:
		break;
	}
	return result;
}

#pragma region f11  

void IsoHandler::Toggle(UINT nCmdShow)
{
	// Retrieve current window placement information.
	//WINDOWPLACEMENT placement;
	//::GetWindowPlacement(root_hwnd, &placement);

	//if (placement.showCmd == nCmdShow)
	//  ::ShowWindow(root_hwnd, SW_RESTORE);
	//else
	//  ::ShowWindow(root_hwnd, nCmdShow);


	//20230218 Toggle
	LONG_PTR Style = ::GetWindowLongPtr(root_hwnd_, GWL_STYLE);

	//LOG(ERROR) << "Toggle " << nCmdShow << "," << (Style & DS_MODALFRAME);


	if (Style & DS_MODALFRAME && SW_MAXIMIZE == nCmdShow)
	{
		//LOG(ERROR) << "Toggle " << "return";
		return;
	}

	if (Style & DS_MODALFRAME)
	{
		ShowWindow(::FindWindow(L"Shell_TrayWnd", NULL), SW_SHOW);
		//LOG(ERROR) << "Toggle " << "restore";
		Style &= ~DS_MODALFRAME;
		Style |= WS_THICKFRAME;
		Style |= WS_CAPTION;

		::SetWindowLongPtr(root_hwnd_, GWL_STYLE, Style);
		::SetWindowPos(root_hwnd_, NULL, _window_rect.left, _window_rect.top, _window_rect.right - _window_rect.left, _window_rect.bottom - _window_rect.top, SWP_NOSENDCHANGING);

		fullscreen_ = false;
		_first_f11_over = true;
		ShowWindow(hwndF11_, SW_HIDE);
		KillTimer(hwndF11_, IDT_TIMERTIP);
	}
	else
	{
		RECT rcTW;
		HWND tW = FindWindow(L"Shell_TrayWnd", NULL);
		GetWindowRect(tW, &rcTW);
		ShowWindow(tW, SW_HIDE);

		int ty = 0;
		int offW = 0;
		int offH = 0;
		if (rcTW.top == rcTW.left)
		{
			if (rcTW.right > rcTW.bottom)
			{
				//up
				offH = rcTW.bottom - rcTW.top;
				ty = 0;
			}
			else
			{
				//left
				offW = rcTW.right - rcTW.left;
				ty = 3;
			}
		}
		else if (rcTW.left == 0)
		{
			//down
			offH = rcTW.bottom - rcTW.top;
			ty = 3;
		}
		else if (rcTW.top == 0)
		{
			//right
			offW = rcTW.right - rcTW.left;
			ty = 1;
		}

		//LOG(ERROR) << "Toggle " << "max";
		Style &= ~WS_THICKFRAME;
		Style &= ~WS_CAPTION;
		Style |= DS_MODALFRAME;

		::SetWindowLongPtr(root_hwnd_, GWL_STYLE, Style);
		GetWindowRect(root_hwnd_, &_window_rect);

		int cx = GetSystemMetrics(SM_CXICON);
		//const int urlbar_height = GetURLBarHeight(hwnd_) + 4;

		int nHeadHeight = 24;// GetSystemMetrics(SM_CYCAPTION);
		nHeadHeight = nHeadHeight * cx / 32 + 4;


		//The number of px between parent window and inner browser
		//int nOffset = 1;

		MONITORINFO monitorinfo;
		monitorinfo.cbSize = sizeof(MONITORINFO);
		HMONITOR h = MonitorFromWindow(root_hwnd_, MONITOR_DEFAULTTONULL);
		GetMonitorInfo(h, &monitorinfo);

		RECT rcDesktop = monitorinfo.rcWork;

		int ox = GetSystemMetrics(SM_CXDLGFRAME);
		int oy = GetSystemMetrics(SM_CYDLGFRAME);
		//LOG(ERROR) << "SM_CYDLGFRAME " << oy;
		//LOG(ERROR) << "SM_CYFRAME " << GetSystemMetrics(SM_CYFRAME);

		int w1 = abs(rcDesktop.right - rcDesktop.left);
		int h1 = abs(rcDesktop.bottom - rcDesktop.top);
		//::SetWindowPos(root_hwnd_, NULL, rcDesktop.left - ox, -nHeadHeight, w1 + 2 * ox, nHeadHeight + rcDesktop.bottom + nOffset + oy, SWP_NOSENDCHANGING);

		int lf = rcDesktop.left - ox;
		int top = -nHeadHeight - oy;
		if (ty == 3)
		{
			lf -= offW;
		}
		if (ty == 0)
		{
			top -= offH;
		}
		::SetWindowPos(root_hwnd_, NULL, lf, top,
			w1 + 2 * ox + offW, nHeadHeight + rcDesktop.bottom + oy * 2 + offH, SWP_NOSENDCHANGING);
		fullscreen_ = true;
		_first_f11_over = false;
		if (!flag_video_)
		{

			show_dlgf11();
		}
	}
}

void IsoHandler::show_dlgf11()
{
	if (!IsWindow(hwndF11_))
	{
		hwndF11_ = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_F11), root_hwnd_, (DLGPROC)F11Proc);
		SetWindowLongPtr(hwndF11_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	}

	bool bVisible = (::GetWindowLong(hwndF11_, GWL_STYLE) & WS_VISIBLE) != 0;
	if (bVisible)
	{
		return;
	}

	if (!_first_f11_over)
	{
		ShowWindow(GetDlgItem(hwndF11_, IDC_BUTTON_CLOSE), SW_HIDE);
		SetLayeredWindowAttributes(hwndF11_, RGB(48, 48, 48), (255 * 100) / 100, LWA_ALPHA);
		SetTimer(hwndF11_, IDT_TIMERTIP, 2000, (TIMERPROC)MyTimerProc);
	}
	else
	{
		ShowWindow(GetDlgItem(hwndF11_, IDC_BUTTON_CLOSE), SW_SHOW);
		COLORREF color0 = RGB(0xbe, 0xbe, 0xbe);
		SetLayeredWindowAttributes(hwndF11_, color0, (255 * 100) / 100, LWA_COLORKEY | LWA_ALPHA);
		//HWND hwnd1 = GetDlgItem(hwndF11_, IDC_BUTTON_CLOSE);
		//InvalidateRect(hwnd1, NULL, TRUE);
	}

	ShowWindow(hwndF11_, SW_SHOWNA);

	RECT rcWin;
	GetWindowRect(root_hwnd_, &rcWin);

	int w = rcWin.right - rcWin.left;
	RECT rc;
	GetWindowRect(hwndF11_, &rc);

	int left = (w - (rc.right - rc.left)) / 2 + rcWin.left;
	MoveWindow(hwndF11_, left, 40, rc.right - rc.left, _first_f11_over ? 80 : 100, FALSE);

	SetForegroundWindow(root_hwnd_);
}

void IsoHandler::show_dlgf11_close()
{
	if (_first_f11_over)
	{
		POINT Point;
		GetCursorPos(&Point);
		ScreenToClient(root_hwnd_, &Point);
		int X = Point.x;
		int Y = Point.y;
		if (Y < 150)
		{
			show_dlgf11();
		}
		else
		{
			KillTimer(hwndF11_, IDT_TIMERTIP);
			ShowWindow(hwndF11_, SW_HIDE);
		}
	}
}
INT_PTR CALLBACK IsoHandler::F11Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	IsoHandler* self = (IsoHandler*)GetWindowLongPtr(hDlg, GWLP_USERDATA);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		//HWND hStaticEdit3 = CreateWindowEx(0, TEXT("Static"), NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP | SS_NOTIFY, lf, 2, w, w, hwndDlg, (HMENU)IDC_BUTTON_CLOSE, 0, 0);
		//SendMessage(hStaticEdit3, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmpClose_);
		SetWindowLong(hDlg, GWL_EXSTYLE, GetWindowLong(hDlg, GWL_EXSTYLE) | WS_EX_LAYERED);//| ~WS_EX_APPWINDOW); | LWA_COLORKEY
	}
	return TRUE;
	case WM_CTLCOLORSTATIC:
	{
		SetBkMode((HDC)wParam, TRANSPARENT);
		return (INT_PTR)(HBRUSH)GetStockObject(NULL_BRUSH);
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;

		hdc = BeginPaint(hDlg, &ps);
		RECT  rect;
		GetClientRect(hDlg, &rect);

		if (!self->_first_f11_over)
		{
			COLORREF color = RGB(48, 48, 48);
			HBRUSH hBrush = CreateSolidBrush(color);
			FillRect(hdc, &rect, hBrush);
			DeleteObject(hBrush);

			wstring tips = L"";
			transl("Press F11 to exit full screen", tips);

			HFONT control_font = (HFONT)SendMessage(hDlg, WM_GETFONT, 0, 0);
			HFONT old = (HFONT)SelectObject(hdc, control_font);

			SIZE size;
			GetTextExtentPoint32(hdc, tips.c_str(), tips.size(), &size);

			SetBkColor(hdc, color);
			SetTextColor(hdc, RGB(0, 255, 0));
			TextOut(hdc, (rect.right - rect.left - size.cx) / 2, 25, tips.c_str(), tips.size());

			SelectObject(hdc, old);
		}
		else
		{
			COLORREF color = RGB(0xbe, 0xbe, 0xbe);
			HBRUSH hBrush0 = CreateSolidBrush(color);
			FillRect(hdc, &rect, hBrush0);
			DeleteObject(hBrush0);

			HWND hwnd1 = GetDlgItem(self->hwndF11_, IDC_BUTTON_CLOSE);
			HDC hDC = ::GetDC(hwnd1);
			RECT rc;
			GetWindowRect(hwnd1, &rc);

			int h = rc.bottom - rc.top;
			int w = h;// rc.right - rc.left;


			HBRUSH hBrush = CreateSolidBrush(RGB(48, 48, 48));
			HGDIOBJ hOldBrush = SelectObject(hDC, hBrush);
			Ellipse(hDC, 0, 0, w, h);
			SelectObject(hDC, hOldBrush);
			DeleteObject(hBrush);

			int x = w / 2;
			int y = h / 2;
			HPEN hPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
			HGDIOBJ hOldPen = SelectObject(hDC, hPen);
			MoveToEx(hDC, x - 10, y - 10, (LPPOINT)NULL);
			LineTo(hDC, x + 10, y + 10);
			MoveToEx(hDC, x + 10, y - 10, (LPPOINT)NULL);
			LineTo(hDC, x - 10, y + 10);
			SelectObject(hDC, hOldPen);
			DeleteObject(hPen);

			ReleaseDC(hwnd1, hDC);
		}

		EndPaint(hDlg, &ps);
	}
	return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON_CLOSE)
		{
			self->Toggle(SW_RESTORE);
			ShowWindow(hDlg, SW_HIDE);
			KillTimer(hDlg, IDT_TIMERTIP);

			return (INT_PTR)TRUE;
		}
		break;
	}
	return FALSE;
}
#pragma endregion

#pragma region about  

INT_PTR CALLBACK IsoHandler::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		char* szResponse = 0;
		char* szMimeType = 0;
		mygetfunc("c.html?act=c.about", &szResponse, &szMimeType);

		string lic = szResponse;
		string exp = szMimeType;

		LocalFree(szResponse);
		LocalFree(szMimeType);

		wstring sz = CefString(lic);
		//if (ov != "2")
		//{
		//	transl("30 days trial period", sz);
		sz += L"\r\n";
		//}

		sz += CefString(exp);
		SetDlgItemText(hDlg, IDC_EDIT_LIC, sz.c_str());

		//transl2(hDlg, IDC_EDIT_LIC);
		transl2(hDlg, IDC_STATIC_COPY);
		transl2(hDlg, IDC_STATIC_LIC);
		transl2(hDlg, IDC_STATIC_WARN);
		transl2(hDlg, IDC_STATIC_THIRD);
		transl2(hDlg, IDOK);

		wstring szDst = L"";
		transl("About", szDst);
		SetWindowText(hDlg, szDst.c_str());
	}
	return (INT_PTR)TRUE;

	case WM_CTLCOLORSTATIC:
	{
		if ((HWND)lParam == GetDlgItem(hDlg, IDC_STATIC_THIRD))
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (INT_PTR)GetStockObject((NULL_BRUSH));
		}
	}
	return (INT_PTR)0;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_STATIC_THIRD)
		{
			HMODULE hModule = ::GetModuleHandle(NULL);

			wchar_t buf[MAX_PATH] = { 0 };
			GetModuleFileNameW(hModule, buf, MAX_PATH);

			wchar_t* p = wcsrchr(buf, L'\\');
			if (p != NULL)
			{
				p[0] = 0;
			}

			wstring szFileName = buf;
			szFileName += L"\\help\\credits.html";

			ShellExecute(NULL, L"open", szFileName.c_str(), NULL, NULL, SW_SHOWNORMAL);
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK IsoHandler::Register(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	IsoHandler* self = (IsoHandler*)GetWindowLongPtr(hDlg, GWLP_USERDATA);

	int wmId, wmEvent;
	switch (message)
	{
	case WM_INITDIALOG:
	{
		IsoHandler* pThis = (IsoHandler*)lParam;
		SetWindowLongPtr(hDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

		//if (g_system_ini.mode == 20)
		//{
		//	wchar_t szTitle[MAX_LOADSTRING];
		//	LoadStringW(m_hrcInst, IDS_BBS_THANK, szTitle, MAX_LOADSTRING);
		//	SetDlgItemTextW(hDlg, IDC_STATIC_DAY, szTitle);
		//}
		//else if (g_system_ini.mode == 10)
		//{
		//	wchar_t szTitle[MAX_LOADSTRING];
		//	LoadStringW(m_hrcInst, IDS_IM_THANK, szTitle, MAX_LOADSTRING);
		//	SetDlgItemTextW(hDlg, IDC_STATIC_DAY, szTitle);
		//}
		string url = "c.html?act=c.about";

		char* szResponse = 0;
		char* szMimeType = 0;
		mygetfunc(url.c_str(), &szResponse, &szMimeType);

		string lic = szResponse;
		string exp = szMimeType;

		LocalFree(szResponse);
		LocalFree(szMimeType);

		SetDlgItemText(hDlg, IDC_STATIC_30, CefString(lic).ToWString().c_str());
		SetDlgItemText(hDlg, IDC_STATIC_TIP3, CefString(exp).ToWString().c_str());

		transl2(hDlg, IDC_BUTTON_BUY, 1);
		transl2(hDlg, IDC_BUTTON_REGISTER, 1);
		//transl2(hDlg, IDC_STATIC_DAY);
		transl2(hDlg, IDC_STATIC_30);
		transl2(hDlg, IDOK);

		if (exp == "")
		{
			//ShowWindow(GetDlgItem(hDlg, IDC_STATIC_30), SW_HIDE);
			ShowWindow(GetDlgItem(hDlg, IDC_EDIT_CODE), SW_HIDE);
			ShowWindow(GetDlgItem(hDlg, IDC_BUTTON_REGISTER), SW_HIDE);
			ShowWindow(GetDlgItem(hDlg, IDC_BUTTON_BUY), SW_HIDE);
			ShowWindow(GetDlgItem(hDlg, IDC_STATIC_TIP3), SW_HIDE);
			ShowWindow(GetDlgItem(hDlg, IDC_STATIC0), SW_HIDE);

			ShowWindow(GetDlgItem(hDlg, IDC_STATIC_TIP), SW_HIDE);
			SetDlgItemText(hDlg, IDC_STATIC_TIP, L"Activated.");
		}
		transl2(hDlg, IDC_STATIC_TIP);
		SendMessage(GetDlgItem(hDlg, IDC_EDIT_CODE), EM_LIMITTEXT, 48, 0L);

		wstring szDst = L"";
		transl("Registration", szDst);
		SetWindowText(hDlg, szDst.c_str());
	}
	return (INT_PTR)TRUE;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDC_BUTTON_REGISTER:
		{
			wchar_t code_buf[MAX_PATH] = { 0 };
			GetDlgItemText(hDlg, IDC_EDIT_CODE, code_buf, MAX_PATH);

			if (wcslen(code_buf) > 0)
			{
				CefString s1 = L"";
				CefString s2 = code_buf;
				string url = string("c.html?act=c.register&s1=") + s1.ToString() + "&s2=" + s2.ToString();

				char* szResponse = 0;
				char* szMimeType = 0;
				mygetfunc(url.c_str(), &szResponse, &szMimeType);

				char enckey[MAX_PATH] = { 0 };
				strcpy_s(enckey, MAX_PATH, szResponse);
				LocalFree(szResponse);

				wstring szTitle = L"";
				transl("Registration", szTitle);

				wstring szCongratulate = L"";
				if (stricmp(enckey, "0") == 0)
				{
					transl("Activation completed!", szCongratulate);
				}
				else if (stricmp(enckey, "overdue") == 0)
				{
					transl("This license is expired.", szCongratulate);
				}
				else if (stricmp(enckey, "inuse") == 0)
				{
					transl("This license is in use.", szCongratulate);
				}
				else
				{
					transl("Please check your license key, and then try again.", szCongratulate);
				}

				MessageBoxW(hDlg, szCongratulate.c_str(), szTitle.c_str(), MB_OK | MB_ICONINFORMATION);
				if (stricmp(enckey, "0") == 0)
				{
					EndDialog(hDlg, LOWORD(wParam));
					PostMessage(self->root_hwnd_, WM_ACTIVATED, 0, 0);
				}

			}
		}
		return (INT_PTR)TRUE;
		case IDC_BUTTON_BUY:
		{
			wstring buyURL = L"";
			if (buyURL.empty())
			{
				// BuyURL doesn't exsits in registry, default it
				buyURL = L"https://www.photonest.io/index.html#subscribe";
			}

			ShellExecute(NULL, L"open", buyURL.c_str(), NULL, NULL, SW_SHOWNORMAL);
		}
		return (INT_PTR)TRUE;
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// can not be child style in win7
INT_PTR CALLBACK IsoHandler::LoadingProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		// Set WS_EX_LAYERED on this window 
		SetWindowLong(hDlg, GWL_EXSTYLE, GetWindowLong(hDlg, GWL_EXSTYLE) | WS_EX_LAYERED);//| ~WS_EX_APPWINDOW); | LWA_COLORKEY LWA_ALPHA

		COLORREF color0 = GetSysColor(COLOR_3DFACE);
		SetLayeredWindowAttributes(hDlg, color0, (255 * 80) / 100, LWA_COLORKEY | LWA_ALPHA);
	}
	return (INT_PTR)TRUE;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hDlg, &ps);

		RECT  rect;
		GetClientRect(hDlg, &rect);

		wchar_t txt[] = L"Loading...";

		SIZE str_size = { 0 };
		int dis_num = 0, line_width = 0;
		GetTextExtentExPoint(hdc, txt, wcslen(txt), line_width, &dis_num, NULL, &str_size);

		COLORREF color = GetSysColor(COLOR_3DFACE);
		::SetBkColor(hdc, color);
		::SetTextColor(hdc, RGB(201, 201, 201));//201 224 128
		TextOut(hdc, (rect.right - rect.left - str_size.cx) / 2, 15, txt, wcslen(txt));

		EndPaint(hDlg, &ps);
	}
	return (INT_PTR)TRUE;

	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK IsoHandler::MyTimerProc(HWND hDlg, UINT message, UINT idTimer, DWORD dwTime)
{
	IsoHandler* self = (IsoHandler*)GetWindowLongPtr(hDlg, GWLP_USERDATA);
	ShowWindow(hDlg, SW_HIDE);
	KillTimer(hDlg, IDT_TIMERTIP);
	self->_first_f11_over = true;
	return 0;
}

INT_PTR CALLBACK IsoHandler::AlertProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		wchar_t lpBuffer[1000] = { 0 };

		ATOM hData = lParam;
		GlobalGetAtomName(hData, lpBuffer, 1000);
		DeleteAtom(hData);

		//wchar_t *ptr = (wchar_t*)lParam;
		SetDlgItemText(hDlg, IDC_STATIC_MESSAGE, lpBuffer);

		transl2(hDlg, IDOK);
		transl2(hDlg, IDCANCEL);
	}
	return (INT_PTR)TRUE;
	case WM_COMMAND:
	{
		UINT id = LOWORD(wParam);
		switch (id) {
		case IDOK:
			EndDialog(hDlg, id);
			return (INT_PTR)id;
		case IDCANCEL:
			EndDialog(hDlg, id);
			return (INT_PTR)id;
		}
		break;
	}
	case WM_CLOSE:
		return TRUE;
	}
	return (INT_PTR)FALSE;
	//must be 
	//return DefWindowProc(hwnd, message, wParam, lParam);
}
#pragma endregion

#pragma region dir  

wstring IsoHandler::OnDir(HWND hwnd, int dirType)
{
	_dirType = dirType;

	HRESULT hr;
	LPITEMIDLIST pItemList;
	BROWSEINFO browseinfo;

	wchar_t szDir[MAX_PATH] = { 0 };

	memset(&browseinfo, 0, sizeof(BROWSEINFO));
	browseinfo.hwndOwner = hwnd;
	browseinfo.pszDisplayName = szDir;
	browseinfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;

	wchar_t szTitle[MAX_PATH] = { 0 };
	wstring title = L"";
	if (_dirType == 0)
	{
		transl("Select the folder containing your photos", title);
	}
	else
	{
		transl("Export", title);
	}
	wcscpy_s(szTitle, MAX_PATH, title.c_str());
	browseinfo.lpszTitle = szTitle;

	browseinfo.lParam = (LPARAM)this;
	browseinfo.iImage = 0;
	browseinfo.pidlRoot = 0;
	browseinfo.lpfn = (BFFCALLBACK)BrowseCallbackProc;

	bool b = false;
	pItemList = ::SHBrowseForFolder(&browseinfo);
	if (pItemList)
	{
		if (SHGetPathFromIDList(pItemList, szDir))
		{
			b = true;
			if (_dirType == 0)
			{
				CefString s1 = szDir;

				wstring tmp = s1;
				transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);

				wstring p = get_module_path();
				wstring::size_type pos = p.find(L"\\windowsapps\\photonest");
				if (pos != wstring::npos)
				{
					p = get_appdata_path();
				}

				string::size_type pos0 = tmp.find(p);
				string::size_type pos1 = p.find(tmp);

				//LOG(ERROR) << tmp.c_str() << "," << p;

				if (pos0 != string::npos || pos1 != string::npos)
				{
					_currDir = p + L"\\sample";
					DWORD dwAttr = GetFileAttributes(_currDir.c_str());
					if (dwAttr == -1)
					{
						_currDir = p;
					}
				}
				else
				{
					_currDir = szDir;
				}
				string s = CefString(_currDir.c_str()).ToString();

				IsoHandler::Instance().WritePrivate("alb_dir", s.c_str());
			}
			else
			{
				string s = CefString(szDir).ToString();

				IsoHandler::Instance().WritePrivate("down_dir", s.c_str());

			}
		}

		IMalloc* pMalloc = NULL;
		hr = SHGetMalloc(&pMalloc);
		if (FAILED(hr)) {
			return L"";
		}

		pMalloc->Free(pItemList);
		pMalloc->Release();
		pMalloc = NULL;
	}

	if (b)
	{
		CefString s1 = szDir;
		return s1;
	}
	return L"";
}

INT_PTR CALLBACK IsoHandler::BrowseCallbackProc(HWND hDlg, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	IsoHandler* self = (IsoHandler*)lpData;

	wchar_t szDir[MAX_PATH] = { 0 };

	switch (uMsg) {
	case BFFM_INITIALIZED:
	{
		//HFONT control_font = (HFONT)SendMessage(self->hwndLoading_, WM_GETFONT, 0, 0);
		//HDC hDC = ::GetDC(hDlg);
		//HFONT old = (HFONT)SelectObject(hDC, control_font);

	//	SelectObject(hDC, old);



		if (self->_dirType == 1)//L"down_box")
		{
			wstring rDir = L"";
			string s = "";
			IsoHandler::Instance().ReadPrivate("down_dir", s);
			if (s == "")
			{
				wchar_t buffer[MAX_PATH] = { 0 };
				SHGetSpecialFolderPath(NULL, buffer, CSIDL_MYDOCUMENTS, FALSE);
				rDir = buffer;
				rDir.erase(rDir.find_last_of(L"\\"));
				rDir = CefString(s).ToWString() + L"\\Downloads";
			}
			else
			{
				rDir = CefString(s).ToWString();
			}

			SendMessage(hDlg, BFFM_SETSELECTION, TRUE, (LPARAM)rDir.c_str());
		}
		else
		{
			if (self->_currDir == L"")
			{
				string s = "";
				IsoHandler::Instance().ReadPrivate("alb_dir", s);
				if (s != "")
				{
					self->_currDir = CefString(s).ToWString();

					wchar_t szbuf[MAX_PATH] = { 0 };
					DWORD dwSize = GetFullPathName(self->_currDir.c_str(), MAX_PATH, szbuf, NULL);

					BOOL b = FALSE;
					DWORD dwAttr = GetFileAttributes(szbuf);
					if (dwAttr == -1)
					{
						self->_currDir = L"";
					}
				}
			}

			if (self->_currDir == L"")
			{
				//GetCurrentDirectoryW(sizeof(szDir) / sizeof(wchar_t), szDir);
				wstring p = get_module_path();
				wstring::size_type pos = p.find(L"\\windowsapps\\photonest");
				if (pos != wstring::npos)
				{
					wchar_t* szDocument;
					if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Pictures, 0, NULL, &szDocument)))
					{
						p = szDocument;
						CoTaskMemFree(szDocument);
					}
				}
				else
				{
					p += L"\\sample";
				}
				self->_currDir = p;

				//::SetFileAttributes(p.c_str(), FILE_ATTRIBUTE_NORMAL);
				//LOG(ERROR) << p;
				//LPITEMIDLIST pidl = NULL;
				//wchar_t szDocument[MAX_PATH] = { 0 };
				//SHGetSpecialFolderLocation(NULL, CSIDL_MYDOCUMENTS, &pidl);
				//if (pidl && SHGetPathFromIDList(pidl, szDocument))
				//{
				//	GetShortPathName(szDocument, szDir, _MAX_PATH);
				//	self->_currDir = szDir;
				//}
			}
			SendMessage(hDlg, BFFM_SETSELECTION, TRUE, (LPARAM)self->_currDir.c_str());
		}

		//wcscpy_s(szDir, MAX_PATH, _currDir.c_str());
		// WParam is TRUE since you are passing a path.
		// It would be FALSE if you were passing a pidl.
		//SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)szDir);
	}
	return (INT_PTR)TRUE;
	case BFFM_SELCHANGED:
	{
		// Set the status window to the currently selected path.
		if (SHGetPathFromIDListW((LPITEMIDLIST)lParam, szDir))
		{
			memset(szDir, 0, MAX_PATH * sizeof(wchar_t));
		}
	}
	return (INT_PTR)TRUE;
	default:
		break;
	}
	return (INT_PTR)FALSE;
}
#pragma endregion

#pragma region iso  

INT_PTR CALLBACK IsoHandler::ISOProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	IsoHandler* self = (IsoHandler*)GetWindowLongPtr(hDlg, GWLP_USERDATA);
	switch (message) {
	case WM_INITDIALOG:
	{
		_vecLoc.clear();
		EnumUILanguagesA(UilanguageEnumproc, MUI_LANGUAGE_NAME, NULL);// MUI_ALL_INSTALLED_LANGUAGES);

		IsoHandler* pThis = (IsoHandler*)lParam;
		SetWindowLongPtr(hDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

		SendMessage(GetDlgItem(hDlg, IDC_PROGRESS1), PBM_SETRANGE, 0, MAKELPARAM(0, 100));
		SendMessage(GetDlgItem(hDlg, IDC_PROGRESS1), PBM_SETPOS, (WPARAM)0, 0);
		SendMessage(GetDlgItem(hDlg, IDC_PROGRESS1), PBM_SETSTEP, (WPARAM)1, 0);

		transl2(hDlg, IDC_BTN_CREATE_ISO, 1);
		transl2(hDlg, IDC_STATIC_ISO);
		transl2(hDlg, IDC_BTN_CANCEL);
		transl2(hDlg, IDC_BTN_OPEN);
		transl2(hDlg, IDOK);

		wstring szDst = L"";
		transl("Dump Image Files (ISO)", szDst);
		SetWindowText(hDlg, szDst.c_str());

		//wstring path = get_module_path();
		wstring drive = L"D:\\";//path.substr(0, 2);
		UINT u = GetDriveType(drive.c_str());
		if (u != DRIVE_FIXED)
		{
			drive = L"E:\\";
			u = GetDriveType(drive.c_str());
		}
		if (u != DRIVE_FIXED)
		{
			drive = L"F:\\";
			u = GetDriveType(drive.c_str());
		}
		if (u != DRIVE_FIXED)
		{
			drive = L"C:\\";
		}


		wchar_t pIsoPath[MAX_PATH] = { 0 };

		if (pThis->viewer_ == 3)
		{
			swprintf_s(pIsoPath, L"%sPhotoNest Encryption.iso", drive.c_str());
		}
		else
		{
			swprintf_s(pIsoPath, L"%sPhotoNest.iso", drive.c_str());
		}

		SetDlgItemText(hDlg, IDC_STATIC_TITLE, L"");
		SetDlgItemText(hDlg, IDC_EDIT_ISO, pIsoPath);

		//SendDlgItemMessage(hDlg, IDC_ROOT_LIST, LB_ADDSTRING, 0, (LPARAM)path.c_str());

		map<string, string> ::iterator I;
		for (I = pThis->_roots_map.begin(); I != pThis->_roots_map.end(); I++)
		{
			CefString s = I->second;
			wstring s3 = s.ToWString();
			Replace(s3, L"/", L"\\");
			SendDlgItemMessage(hDlg, IDC_ROOT_LIST, LB_ADDSTRING, 0, (LPARAM)s3.c_str());
		}

		unsigned int			ThreadD_ID;
		_beginthreadex(NULL, 0, SetFolderGB, (LPVOID)hDlg, NULL, &ThreadD_ID);
	}
	return (INT_PTR)TRUE;

	case WM_GB:
	{
		if (wParam != NULL)
		{
			wchar_t* pBuf = (wchar_t*)wParam;
			SetDlgItemText(hDlg, IDC_STATIC_SRC, pBuf);

			LocalFree(pBuf);
			wParam = NULL;
			pBuf = NULL;
		}
	}
	return (INT_PTR)TRUE;

	case WM_STEP:
	{
		if (wParam != NULL)
		{
			char* pBuf = (char*)wParam;

			if (stricmp(pBuf, "Formating...") == 0)
			{
				transl2(hDlg, IDC_STATIC_TITLE, L"Formating...");
			}
			else if (stricmp(pBuf, "ok") == 0 || stricmp(pBuf, "cancel") == 0)
			{
				SendMessage(GetDlgItem(hDlg, IDC_PROGRESS1), PBM_SETPOS, (WPARAM)0, 0);
				ShowWindow(GetDlgItem(hDlg, IDC_PROGRESS1), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, IDC_BTN_CANCEL), SW_HIDE);
				if (stricmp(pBuf, "ok") == 0)
				{
					transl2(hDlg, IDC_STATIC_TITLE, L"The operation completed successfully.");
					EnableWindow(GetDlgItem(hDlg, IDC_BTN_CREATE_ISO), TRUE);
					EnableWindow(GetDlgItem(hDlg, IDC_BTN_BROWSE), TRUE);

					wstring szDst = L"";
					wstring szTitle = L"";
					transl("Dump Image Files (ISO)", szDst);
					transl("The operation completed successfully.", szTitle);

					MessageBox(hDlg, szTitle.c_str(), szDst.c_str(), MB_OK | MB_ICONINFORMATION);
					EndDialog(hDlg, LOWORD(wParam));
					wstring s1 = L"/select, " + self->_pIsoPath;
					ShellExecute(NULL, L"open", L"explorer.exe", s1.c_str(), NULL, SW_SHOWNORMAL);
				}
				else
				{
					transl2(hDlg, IDC_STATIC_TITLE, L"Canceled");
				}
			}
			else
			{
				wstring_convert<codecvt_utf8<wchar_t>> conv;
				wstring s0 = conv.from_bytes(pBuf);

				SendMessage(GetDlgItem(hDlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
				SetDlgItemText(hDlg, IDC_STATIC_TITLE, s0.c_str());
				//transl2(hDlg, IDC_STATIC_TITLE, pBuf);
			}

			LocalFree(pBuf);
			wParam = NULL;
			pBuf = NULL;
		}
	}
	return (INT_PTR)TRUE;
	case WM_CLOSE:
	{
		if (!self->_start_iso)
		{
			EndDialog(hDlg, LOWORD(wParam));
		}
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDOK)
		{
			if (!self->_start_iso)
			{
				EndDialog(hDlg, LOWORD(wParam));
			}
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_BTN_CANCEL)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_BTN_CREATE_ISO), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_BTN_BROWSE), TRUE);
			self->_cancel_iso = true;
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_BTN_OPEN)
		{
			wchar_t pIsoPath[2048] = { 0 };
			GetDlgItemText(hDlg, IDC_EDIT_ISO, pIsoPath, 2048);

			wchar_t* ptr = wcsrchr(pIsoPath, L'\\');
			ptr[0] = 0;
			ShellExecute(NULL, L"open", pIsoPath, NULL, NULL, SW_SHOWNORMAL);
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_BTN_BROWSE)
		{
			wstring path = get_module_path();
			wstring drive = path.substr(0, 2);

			wstring _isofile = L"PhotoNest";
			wchar_t strFilter[] = L"ISO Files(*.iso)\0*.iso\0";

			wchar_t strFile[MAX_PATH] = { 0 };
			wcscpy_s(strFile, MAX_PATH, _isofile.c_str());

			wchar_t strFileTitle[MAX_PATH] = { 0 };
			wchar_t strInitiaDir[MAX_PATH] = { 0 };

			wcscpy_s(strInitiaDir, MAX_PATH, drive.c_str());
			wchar_t strTitle[] = L"Save As";

			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
#ifdef OPENFILENAME_SIZE_VERSION_400
			// we are not going to use new fields any way
			ofn.lStructSize = OPENFILENAME_SIZE_VERSION_400;
#else
			ofn.lStructSize = sizeof(ofn);
#endif
			ofn.hwndOwner = hDlg;
			ofn.lpstrFilter = strFilter;
			ofn.nFilterIndex = 0;
			ofn.lpstrFile = strFile;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFileTitle = strFileTitle;
			ofn.nMaxFileTitle = MAX_PATH;
			ofn.lpstrInitialDir = strInitiaDir;
			ofn.lpstrTitle = strTitle;
			ofn.Flags = 0;
			ofn.nFileOffset = 0;
			ofn.nFileExtension = 0;

			ofn.lpstrDefExt = L"iso";

			if (GetSaveFileName(&ofn))
			{
				SetDlgItemTextW(hDlg, IDC_EDIT_ISO, ofn.lpstrFile);
			}

			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_BTN_CREATE_ISO)
		{
			wchar_t pIsoPath[2048] = { 0 };
			GetDlgItemText(hDlg, IDC_EDIT_ISO, pIsoPath, 2048);

			SetDlgItemText(hDlg, IDC_STATIC_TITLE, L"");

			HANDLE hWrite = CreateFile(pIsoPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
			if (hWrite == INVALID_HANDLE_VALUE)
			{
				wstring txt = L"";
				transl("This file is currently in use", txt);
				txt = L"(" + wstring(pIsoPath) + L")" + txt;
				SetDlgItemText(hDlg, IDC_STATIC_TITLE, txt.c_str());

				return (INT_PTR)TRUE;
			}
			else
			{
				CloseHandle(hWrite);
				DeleteFile(pIsoPath);
			}

			EnableWindow(GetDlgItem(hDlg, IDC_BTN_CREATE_ISO), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_BTN_BROWSE), FALSE);
			ShowWindow(GetDlgItem(hDlg, IDC_PROGRESS1), SW_SHOW);
			ShowWindow(GetDlgItem(hDlg, IDC_BTN_OPEN), SW_HIDE);
			ShowWindow(GetDlgItem(hDlg, IDC_BTN_CANCEL), SW_SHOW);

			//char* szResponse = 0;
			//char* szMimeType = 0;
			//mygetfunc("c.html?act=c.backup", &szResponse, &szMimeType);

			SendMessage(GetDlgItem(hDlg, IDC_PROGRESS1), PBM_SETRANGE, 0, MAKELPARAM(0, 100));
			SendMessage(GetDlgItem(hDlg, IDC_PROGRESS1), PBM_SETPOS, (WPARAM)0, 0);

			self->_cancel_iso = false;
			self->_pIsoPath = pIsoPath;

			unsigned int			ThreadD_ID;
			_beginthreadex(NULL, 0, SaveIso, (LPVOID)hDlg, NULL, &ThreadD_ID);
		}
		else if (LOWORD(wParam) == IDC_ROOT_LIST)
		{
			if (HIWORD(wParam) == LBN_DBLCLK)
			{
				int selectedIndex = SendDlgItemMessage(hDlg, IDC_ROOT_LIST, LB_GETCURSEL, 0, 0);
				wchar_t pIsoPath[2048] = { 0 };
				SendDlgItemMessage(hDlg, IDC_ROOT_LIST, LB_GETTEXT, selectedIndex, (LPARAM)pIsoPath);

				ShellExecute(NULL, L"open", pIsoPath, NULL, NULL, SW_SHOWNORMAL);
				return (INT_PTR)TRUE;
			}
		}
		return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
}

UINT __stdcall IsoHandler::SaveIso(LPVOID pParam)
{
	HWND hDlg = (HWND)pParam;
	IsoHandler* self = (IsoHandler*)GetWindowLongPtr(hDlg, GWLP_USERDATA);

	vector<wstring> s2;
	//wstring path = get_module_path();

	//s2.push_back(path + L"\\bin");
	//s2.push_back(path + L"\\templ");

	//if (self->viewer_ == 3)
	//{
	//	s2.push_back(path + L"\\data2\\_thumb");
	//	s2.push_back(path + L"\\data2\\_train");
	//}
	//else
	//{
	//	s2.push_back(path + L"\\data1\\_thumb");
	//	s2.push_back(path + L"\\data1\\_data");
	//	s2.push_back(path + L"\\data1\\_train");
	//}

	map<string, string> ::iterator I;
	for (I = self->_roots_map.begin(); I != self->_roots_map.end(); I++)
	{
		s2.push_back(CefString(I->second).c_str());
	}

	self->_start_iso = true;
	self->create(hDlg, self->_pIsoPath.c_str(), s2);
	self->_start_iso = false;

	return 0;
}

UINT __stdcall IsoHandler::SetFolderGB(LPVOID pParam)
{
	HWND hDlg = (HWND)pParam;

	wchar_t* pBuf = (wchar_t*)LocalAlloc(LMEM_ZEROINIT, sizeof(wchar_t) * MAX_PATH);
	LONG64 filesize = 0;

	char* szResponse = 0;
	char* szMimeType = 0;
	string url = "c.html?act=c.foldersize";
	mygetfunc(url.c_str(), &szResponse, &szMimeType);

	uint64_t  pub_filesize = (uint64_t)_atoi64(szResponse);
	LocalFree(szResponse);

	//float bin_filesize = 223.0f * 1024; //L"\\bin"
	//float template_filesize = 4.29f * 1024; //\\templ
	filesize = pub_filesize;// + bin_filesize + template_filesize;

	if (pub_filesize == -1)
	{
		filesize = -1;
	}

	if (filesize == -1)
	{
		if (pBuf != NULL)
		{
			swprintf_s(pBuf, MAX_PATH, L"");
		}
	}
	else
	{
		double d = filesize / 1024.0 / 1024.0;

		if (pBuf != NULL)
		{
			swprintf_s(pBuf, MAX_PATH, L"%.2f GB", d);
		}
	}
	SendMessage(hDlg, WM_GB, (WPARAM)pBuf, 0);

	return 0;
}

void IsoHandler::write_archive(string SrcDir, char* buff, void* a0, struct stat& st, int ty)
{
	struct archive* a = (archive*)a0;
	struct archive_entry* entry;
	size_t len;
	FILE* fd = 0;

	string pFile = "";
	string pName = "";

	if (ty == 1)
	{
		//pFile = SrcDir + "\\data1\\_temp\\photo1.db";
		//pName = SrcDir + "\\data1\\_db\\photo1.db";
		//if (viewer_ == 3)
		//{
		//	pFile = SrcDir + "\\data2\\_temp\\photo2.db";
		//	pName = SrcDir + "\\data2\\_db\\photo2.db";
		//}
	}
	else if (ty == 2)
	{
		//pFile = SrcDir + "\\data1\\_temp\\launch.lnk";
		//string exe = SrcDir + "\\bin\\nest1.exe";
		//wstring desc = L"PhotoNest";

		//pName = "launch.lnk";
		//if (viewer_ == 3)
		//{
		//	pFile = SrcDir + "\\data2\\_temp\\launch.lnk";
		//	exe = SrcDir + "\\bin\\nest2.exe";
		//	desc = L"PhotoNest Encryption";
		//	pName = "launch.lnk";
		//}
		//wstring lnk = CefString(pFile);

		//wstring exe0 = CefString(exe);
		//ReplaceOne(exe0, L":", L"_");

		//CreateLink(L"%SystemRoot%\\explorer.exe", lnk.c_str(), desc.c_str(), exe0.c_str());
	}
	else
	{
		//pName = SrcDir + "\\data1\\_db\\archive.ini";
		//if (viewer_ == 3)
		//{
		//	pName = SrcDir + "\\data2\\_db\\archive.ini";
		//}
	}

	if (pFile != "")
	{
		stat(pFile.c_str(), &st);
	}

	entry = archive_entry_new(); // Note 2
	archive_entry_set_pathname(entry, pName.c_str());

	if (pFile != "")
	{
		archive_entry_set_size(entry, st.st_size); // Note 3
	}
	else
	{
		archive_entry_set_size(entry, 5); // Note 3
	}
	archive_entry_set_atime(entry, st.st_atime, 0);
	archive_entry_set_ctime(entry, st.st_ctime, 0);
	archive_entry_set_mtime(entry, st.st_mtime, 0);

	archive_entry_set_filetype(entry, AE_IFREG);
	archive_entry_set_perm(entry, 0644);
	archive_write_header(a, entry);

	if (pFile != "")
	{
		errno_t err0 = fopen_s(&fd, pFile.c_str(), "rb");
		if (err0 == 0)
		{
			memset(buff, 0, 16384);
			len = fread(buff, 1, sizeof(buff), fd);
			while (len > 0)
			{
				archive_write_data(a, buff, len);
				memset(buff, 0, 16384);
				len = fread(buff, 1, sizeof(buff), fd);
			}
			fclose(fd);
		}
		DeleteFileA(pFile.c_str());
	}
	else
	{
		archive_write_data(a, "iso=1", 5);
	}
	archive_entry_free(entry);
}

HRESULT IsoHandler::CreateLink(LPCWSTR lpszPathObj, LPCWSTR lpszPathLink, LPCWSTR lpszDesc, LPCWSTR lpszArgs, LPCWSTR lpszIcon)
{
	HRESULT hres;
	IShellLink* psl;

	// Get a pointer to the IShellLink interface. It is assumed that CoInitialize
	// has already been called.
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
	//DWORD errorCode = ::GetLastError();

	if (SUCCEEDED(hres))
	{
		IPersistFile* ppf;

		// Set the path to the shortcut target and add the description. 
		psl->SetPath(lpszPathObj);
		if (lpszDesc)
		{
			psl->SetDescription(lpszDesc);
		}
		if (lpszIcon)
		{
			psl->SetIconLocation(lpszIcon, 0);
		}
		if (lpszArgs)
		{
			psl->SetArguments(lpszArgs);
		}

		// Query IShellLink for the IPersistFile interface, used for saving the 
		// shortcut in persistent storage. 
		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

		if (SUCCEEDED(hres))
		{
			// Save the link by calling IPersistFile::Save. 
			hres = ppf->Save(lpszPathLink, TRUE);
			ppf->Release();
		}
		psl->Release();
	}
	return hres;
}

long IsoHandler::mySendMessage(HWND hDlg, const char* pSrcDir, const char* pName)
{
	char* pBuf = (char*)LocalAlloc(LMEM_ZEROINIT, sizeof(char) * MAX_PATH);
	if (pBuf != NULL)
	{
		strcpy_s(pBuf, MAX_PATH, pSrcDir);
		strcat_s(pBuf, MAX_PATH, pName);

		SendMessage(hDlg, WM_STEP, (WPARAM)pBuf, 0);
	}

	return 0;
}
#pragma endregion

#pragma region prof  
void IsoHandler::get_prof_rcwin(int& zm, int& pub_default, RECT& rcWin)
{
	rcWin.left = 0;
	rcWin.top = 0;
	rcWin.right = 0;
	rcWin.bottom = 0;

	string szSubKey = "Software\\OpenCV\\HighGUI\\Windows\\ZK2i3";
	HKEY hKey;

	if (RegOpenKeyExA(HKEY_CURRENT_USER, szSubKey.c_str(), 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
	{
		// Yes we are installed.
		DWORD dwType = 0;
		DWORD dwSize = sizeof(int);

		RegQueryValueExA(hKey, "zoomed", NULL, &dwType, (BYTE*)&zm, &dwSize);

		RegQueryValueExA(hKey, "right", NULL, &dwType, (BYTE*)&rcWin.right, &dwSize);
		if (rcWin.right > 0)
		{
			RegQueryValueExA(hKey, "left", NULL, &dwType, (BYTE*)&rcWin.left, &dwSize);
			RegQueryValueExA(hKey, "top", NULL, &dwType, (BYTE*)&rcWin.top, &dwSize);
			RegQueryValueExA(hKey, "bottom", NULL, &dwType, (BYTE*)&rcWin.bottom, &dwSize);
		}
		else
		{
			zm = 1;
		}
	}
}

void IsoHandler::WritePrivate(const char* key, const char* val)
{
	CSimpleIniA ini;
	ini.SetUnicode(true);
	ini.LoadFile(_iniPath.c_str());

	ini.SetValue("section", key, val);
	ini.SaveFile(_iniPath.c_str());
}

void IsoHandler::ReadPrivate(const char* key, string& val)
{
	val = "";

	CSimpleIniA ini;
	ini.SetUnicode(true);
	ini.LoadFile(_iniPath.c_str());

	const char* pv1 = ini.GetValue("section", key);
	if (pv1 != nullptr) {
		val = pv1;
	}
}

void IsoHandler::WritePrivate(RECT rcWin, int zm)
{
	string szSubKey = "Software\\OpenCV\\HighGUI\\Windows\\ZK2i3";
	HKEY hKey;

	//DWORD dwDisposition = REG_OPENED_EXISTING_KEY;
	LONG lRet = RegCreateKeyExA(
		HKEY_CURRENT_USER,
		szSubKey.c_str(),
		0,
		NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
		NULL,
		&hKey,
		NULL
	);

	if (lRet == ERROR_SUCCESS)
	{
		if (rcWin.left < 0 || rcWin.left>4096) {
			rcWin.left = 0;
			zm = 1;
		}
		if (rcWin.top < 0 || rcWin.top>4096) {
			rcWin.top = 0;
			zm = 1;
		}
		if (zm == -1)
		{
			//before max

			RegSetValueExA(hKey, "left", 0, REG_DWORD, (BYTE*)&rcWin.left, sizeof(rcWin.left));
			RegSetValueExA(hKey, "top", 0, REG_DWORD, (BYTE*)&rcWin.top, sizeof(rcWin.top));
			RegSetValueExA(hKey, "right", 0, REG_DWORD, (BYTE*)&rcWin.right, sizeof(rcWin.right));
			RegSetValueExA(hKey, "bottom", 0, REG_DWORD, (BYTE*)&rcWin.bottom, sizeof(rcWin.bottom));
			zm = 1;
			RegSetValueExA(hKey, "zoomed", 0, REG_DWORD, (BYTE*)&zm, sizeof(zm));
		}
		else
		{
			//close
			if (zm == 1)
			{
				//zoomed
				RegSetValueExA(hKey, "zoomed", 0, REG_DWORD, (BYTE*)&zm, sizeof(zm));
			}
			else
			{
				//normal
				RegSetValueExA(hKey, "left", 0, REG_DWORD, (BYTE*)&rcWin.left, sizeof(rcWin.left));
				RegSetValueExA(hKey, "top", 0, REG_DWORD, (BYTE*)&rcWin.top, sizeof(rcWin.top));
				RegSetValueExA(hKey, "right", 0, REG_DWORD, (BYTE*)&rcWin.right, sizeof(rcWin.right));
				RegSetValueExA(hKey, "bottom", 0, REG_DWORD, (BYTE*)&rcWin.bottom, sizeof(rcWin.bottom));
				if (zm == 0)
				{
					RegSetValueExA(hKey, "zoomed", 0, REG_DWORD, (BYTE*)&zm, sizeof(zm));
				}
			}
		}
		RegCloseKey(hKey);
	}
}

void IsoHandler::set_app_path(int type, wstring album)
{
	viewer_ = type;
	album_ = album;

	wstring dst = L"";

	wchar_t* szDocument;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &szDocument)))
	{
		dst = szDocument;
		wstring p = get_module_path();
		wstring::size_type pos = p.find(L"\\windowsapps\\photonest");
		if (pos != wstring::npos)
		{
			dst += L"\\PhotoNest_App";
			if (viewer_ == 1)
			{
				dst += L"\\Viewer";
			}
			else if (viewer_ == 2)
			{
				dst += L"\\Nest1";
			}
		}
		else
		{
			dst += L"\\PhotoNest";

			if (viewer_ == 1)
			{
				dst += L"\\Viewer";
			}
			else if (viewer_ == 2)
			{
				dst += L"\\Nest1";
			}
			else
			{
				dst += L"\\Nest2";
			}
		}
		CoTaskMemFree(szDocument);
	}

	if (dst == L"")
	{
		dst = get_module_path();
	}

	_iniPath = dst + L"\\prof.ini";
}

wstring IsoHandler::get_module_path2()
{
	if (_moduledata == L"")
	{
		_moduledata = get_module_path();
	}
	return _moduledata;
}

wstring IsoHandler::get_appdata_path()
{
	if (_appdata == L"")
	{
		std::wstring dst = L"";

		wchar_t* szDocument;
		if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &szDocument)))
		{
			dst = szDocument;

			wstring p = get_module_path();
			wstring::size_type pos = p.find(L"\\windowsapps\\photonest");
			if (pos != wstring::npos)
			{
				dst += L"\\PhotoNest_App";
				dst += L"\\Nest1";
			}
			else
			{
				dst += L"\\PhotoNest";

				if (album_ == L"private")
				{
					dst += L"\\Nest2";
				}
				else
				{
					dst += L"\\Nest1";
				}
			}
			transform(dst.begin(), dst.end(), dst.begin(), ::tolower);

			_appdata = dst;
			CoTaskMemFree(szDocument);
		}
	}
	return _appdata;
}

void IsoHandler::reset_appdata_path(wstring& p)
{
	wstring path = get_module_path();
	wstring::size_type pos = path.find(L"\\windowsapps\\photonest");
	if (pos != wstring::npos)
	{
		pos = path.find(L"x64__");
		if (pos != wstring::npos)
		{
			path = path.substr(pos + 5);
			Replace(p, L"\\local\\photonest_app\\", L"\\local\\packages\\photonest.photonest_" + path + L"\\localcache\\local\\photonest_app\\");
		}
	}
}
wstring IsoHandler::get_extension_dir()
{
	std::wstring dst = L"";

	wchar_t* szDocument;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &szDocument)))
	{
		dst = szDocument;
		dst += L"\\PhotoNest";
		CoTaskMemFree(szDocument);
	}

	DWORD dwAttr = 0;
	wstring dst2 = dst + L"\\background\\default.jpg";
	dwAttr = GetFileAttributesW(dst2.c_str());
	if (dwAttr == -1)
	{
		wstring s5 = dst;
		CreateDirectory(s5.c_str(), NULL);

		s5 = dst + L"\\background";
		CreateDirectory(s5.c_str(), NULL);

		wstring szPath = get_module_path();
		wstring src = szPath + L"\\background\\default.jpg";
		CopyFile(src.c_str(), dst2.c_str(), TRUE);
	}
	return dst;
}

#pragma endregion

void IsoHandler::create(HWND hDlg, const wchar_t* filename, vector<wstring> s2)
{
	//setlocale(LC_ALL, "en_US.UTF-8");
	string loc = "";
	if (_vecLoc.size() > 1)
	{
		vector<string>::iterator I;
		for (I = _vecLoc.begin(); I != _vecLoc.end(); I++)
		{
			string temp = *I;
			string::size_type pos = temp.find("en-");
			if (pos == string::npos)
			{
				loc = *I;
				break;
			}
			//LOG(ERROR) << "temp" << temp;
		}
	}
	//loc = ".UTF-8";

	//LOG(ERROR) << "setlocale" << loc;
	setlocale(LC_ALL, loc.c_str());

	struct archive* a;
	struct archive_entry* entry;
	size_t len;
	FILE* fd = 0;
	char* buff = (char*)malloc(16384);

	a = archive_write_new();

	//archive_write_set_options(a, "hdrcharset=ISO-IR 10646 2000 UTF-8");
	//archive_write_set_format_zip(a);
	archive_write_set_format_iso9660(a);
	//archive_write_add_filter_compress(a);
	//archive_write_set_option(a, NULL, "zisofs", "1");
	//archive_write_set_option(a, NULL, "compression-level", "9");

	//archive_write_set_options(a, "hdrcharset=UTF-16");

	//archive_write_set_options(a, "hdrcharset=CP936");

	//archive_write_set_option(a, NULL, "pad", NULL);
	//archive_write_set_options(a, "iso-level=4,!rockridge");
	//archive_write_set_options(a, "joliet=long");
	//joliet=long
	//archive_write_set_bytes_per_block(a, 1);
	//	archive_write_set_bytes_in_last_block(a, 1);
	//assertA(0 == archive_write_open_memory(a, buff, buffsize, used));

	archive_write_set_options(a, "!rockridge");
	archive_write_set_options(a, "joliet=long");
	//archive_write_set_options(a, "compression-level=8");

	//	archive_write_set_options(a, "zip:experimental");

	//typedef codecvt_byname<wchar_t, char, mbstate_t> F;
	//wstring_convert<F> strCnv(new F(loc.c_str()));

	wstring_convert<codecvt_utf8<wchar_t>> conv;

	//		archive_write_set_format_ustar(a);
	//if (filename != NULL && strcmp(filename, "-") == 0)
	//	filename = NULL;
	archive_write_open_filename_w(a, filename);

	string err = "";
	vector<wstring>::iterator I;
	for (I = s2.begin(); I != s2.end(); I++)
	{
		struct archive* disk = archive_read_disk_new();
		archive_read_disk_set_standard_lookup(disk);

		int r = archive_read_disk_open_w(disk, I->c_str());
		if (r != ARCHIVE_OK) {
			//errmsg(archive_error_string(disk));
			//errmsg("\n");
			//exit(1);
			err = archive_error_string(disk);
			break;
		}

		if (_cancel_iso)
		{
			break;
		}

		wstring t = *I;
		size_t pos = t.size();
		ReplaceOne(t, L":/", L"_");
		Replace(t, L"/", L"_");

		string path0 = conv.to_bytes(t);
		wstring path01 = t;

		for (;;)
		{
			int needcr = 0;

			if (_cancel_iso)
			{
				break;
			}

			entry = archive_entry_new();
			r = archive_read_next_header2(disk, entry);
			if (r == ARCHIVE_EOF)
			{
				break;
			}

			if (r != ARCHIVE_OK)
			{
				//err = archive_error_string(disk);
				//errmsg("\n");
				//exit(1);
			}
			else {
				archive_read_disk_descend(disk);
				//if (verbose) {
				//	msg("a ");
				//	msg(archive_entry_pathname(entry));
				//	needcr = 1;
				//}

				bool b = false;
				const wchar_t* t1 = archive_entry_pathname_w(entry);
				if (t1 != NULL)
				{
					wstring t3 = t1;
					t3 = path01 + t3.substr(pos);
					//t2 = strCnv.to_bytes(t3);
					archive_entry_copy_pathname_w(entry, t3.c_str());
					b = true;
				}
				else
				{
					const char* t0 = archive_entry_pathname(entry);
					if (t0 != NULL)
					{
						string t2 = t0;
						t2 = path0 + t2.substr(pos);
						archive_entry_set_pathname(entry, t2.c_str());
						b = true;
					}
				}

				if (b)
				{
					r = archive_write_header(a, entry);
					if (r < ARCHIVE_OK)
					{
						//errmsg(": ");
						//errmsg(archive_error_string(a));
						//needcr = 1;
					}

					if (r == ARCHIVE_FATAL)
					{
						//err = 1;
						//break;
						//exit(1);
					}
					else
					{
						if (r > ARCHIVE_FAILED)
						{
							//wstring t2 = archive_entry_pathname_w(entry);
							//archive_entry_set_pathname(entry, "e:/77/52/0.webm");
							//archive_entry_update_pathname_utf8(entry, "e:\\77\\52\\0.webm");
							//archive_entry_update_pathname(entry, _w2u(t2).c_str());

							errno_t err0 = 1;
							const wchar_t* t2 = archive_entry_sourcepath_w(entry);
							if (t2 != NULL)
							{
								err0 = _wfopen_s(&fd, t2, L"rb");
							}
							else
							{
								const char* t1 = archive_entry_sourcepath(entry);
								if (t1 != NULL)
								{
									err0 = fopen_s(&fd, t1, "rb");
								}
							}

							if (err0 == 0)
							{
								memset(buff, 0, 16384);
								len = fread(buff, 1, sizeof(buff), fd);
								while (len > 0)
								{
									archive_write_data(a, buff, len);
									memset(buff, 0, 16384);
									len = fread(buff, 1, sizeof(buff), fd);
								}
								fclose(fd);
							}
							else
							{
								string msg = "";
								const wchar_t* t1 = archive_entry_pathname_w(entry);
								if (t1 != NULL)
								{
									msg = conv.to_bytes(t1);
								}
								else
								{
									const char* t0 = archive_entry_pathname(entry);
									if (t0 != NULL)
									{
										msg = t0;
									}
								}
								mySendMessage(hDlg, msg.c_str(), "");
							}
						}
					}
				}
			}
			archive_entry_free(entry);
		}

		archive_read_close(disk);
		archive_read_free(disk);
	}
	if (!_cancel_iso)
	{
		//struct stat st;
		//string SrcDir = get_module_pathA();
		//write_archive(SrcDir, buff, a, st, 1);
		//write_archive(SrcDir, buff, a, st, 2);
		//write_archive(SrcDir, buff, a, st, 3);

		mySendMessage(hDlg, "Formating...", "");
	}
	archive_write_close(a);
	archive_write_free(a);

	free(buff);

	if (_cancel_iso)
	{
		mySendMessage(hDlg, "cancel", "");
	}
	else
	{
		if (err != "")
		{
			mySendMessage(hDlg, err.c_str(), "");
		}
		else
		{
			mySendMessage(hDlg, "ok", "");
		}
	}
}
