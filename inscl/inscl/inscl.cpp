
#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include <CommCtrl.h>
#include <string>
#include "lang3.h"
#include "unitil3.h"
#include <process.h>
#include <shlobj.h>


#define IDT_TIMERLOAD 2019
#include "../../scheme/scheme/scheme.h"

#pragma comment(linker, "/subsystem:windows /ENTRY:wmainCRTStartup")
#pragma comment( lib, "../../../PhotoNest/tests/cefclient/Release/bin/scheme.lib" ) 

int exitcode = 0;
wstring _procs = L"";
wstring _decrypt = L"";


wstring get_appdata_path()
{
	std::wstring dst = L"";

	wchar_t* szDocument;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &szDocument)))
	{
		dst = szDocument;
		CoTaskMemFree(szDocument);
	}
	wstring sz = dst;

	return sz;
}

long GetFolderPics(wstring strPath, vector<WIN32_FIND_DATAW >& vecffd)
{
	wchar_t* buf1 = (wchar_t*)malloc(1024 * sizeof(wchar_t));
	if (buf1 != NULL)
	{
		memset(buf1, 0, 1024 * sizeof(wchar_t));
		swprintf_s(buf1, 1024, L"%s\\*", strPath.c_str());
	}

	WIN32_FIND_DATAW ffd;
	HANDLE hFind = FindFirstFile(buf1, &ffd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
		{
			if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
			{
			}
			else
			{
				vecffd.insert(vecffd.end(), ffd);
			}
		}

		while (FindNextFileW(hFind, &ffd))
		{
			if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0)
			{
				if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
				{
				}
				else
				{
					vecffd.insert(vecffd.end(), ffd);
				}
			}
		}
		FindClose(hFind);
	}
	free(buf1);

	return 0;
}

void sc(wstring path, vector<WIN32_FIND_DATAW > vecffd)
{
	uint32_t len = (uint32_t)vecffd.size();
	for (uint32_t i = 0; i < len; i++)
	{
		WIN32_FIND_DATAW ffd = vecffd[i];
		wstring src = path + L"\\" + ffd.cFileName;

		//wstring::size_type  pos = src.find(L".krf");
		if (_wcsnicmp(ffd.cFileName, L"qr", 2) == 0)
		{
			src = path + L"\\" + ffd.cFileName;
			DeleteFile(src.c_str());
		}
	}
}

void sc_face(wstring path, vector<WIN32_FIND_DATAW > vecffd)
{
	uint32_t len = (uint32_t)vecffd.size();
	for (uint32_t i = 0; i < len; i++)
	{
		WIN32_FIND_DATAW ffd = vecffd[i];
		wstring src = path + L"\\" + ffd.cFileName;

		DeleteFile(src.c_str());
	}
}

int __stdcall RemoveSample(wstring datadir)
{
	wstring path0 = get_appdata_path();
	path0 += L"\\PhotoNest";
	wstring path = path0;
	if (datadir == L"\\data2")
	{
		path += L"\\Nest2";
	}
	else
	{
		path += L"\\Nest1";
	}

	vector<WIN32_FIND_DATAW > vecffd;

	wstring s1 = path + L"\\sample";
	wstring s2 = path + L"\\sample\\lena";
	wstring s3 = path + L"\\sample\\sea";

	//GetFolderPics(s1, vecffd);
	//sc(s1, vecffd);

	//vecffd.clear();
	//GetFolderPics(s2, vecffd);
	//sc(s2, vecffd);

	//vecffd.clear();
	//GetFolderPics(s3, vecffd);
	//sc(s3, vecffd);

	//RemoveDirectory(s3.c_str());
	//RemoveDirectory(s2.c_str());
	//RemoveDirectory(s1.c_str());

	s1 = path + datadir + L"\\_train\\_temp";
	s2 = path + datadir + L"\\_train\\_face";
	s3 = path + datadir + L"\\_train\\_model";

	vecffd.clear();
	GetFolderPics(s1, vecffd);
	sc_face(s1, vecffd);

	vecffd.clear();
	GetFolderPics(s2, vecffd);
	sc_face(s2, vecffd);

	vecffd.clear();
	GetFolderPics(s3, vecffd);
	sc_face(s3, vecffd);

	RemoveDirectory(s3.c_str());
	RemoveDirectory(s2.c_str());
	RemoveDirectory(s1.c_str());

	s1 = path + datadir + L"\\_train";
	RemoveDirectory(s1.c_str());

	s1 = path;
	vecffd.clear();
	GetFolderPics(s1, vecffd);
	sc_face(s1, vecffd);

	RemoveDirectory(s1.c_str());
	RemoveDirectory(path0.c_str());

	return 0;
}

UINT _IDC_PWD_ERR = 0;

VOID CALLBACK LoadTimerProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
	SetDlgItemText(hwnd, _IDC_PWD_ERR, (_decrypt + _procs).c_str());
	_procs += L"...";

	if (_procs.size() > 9)
	{
		_procs = L"";
	}
}

UINT __stdcall handle_uins0(LPVOID pParam)
{
	HWND hDlg = (HWND)pParam;

	char* szResponse = 0;
	char* szMimeType = 0;
	mygetfunc("c.html?act=c.unins&alb=public", &szResponse, &szMimeType);
	LocalFree(szResponse);
	RemoveSample(L"\\data1");

	exitcode = 100;
	KillTimer(hDlg, IDT_TIMERLOAD);
	EndDialog(hDlg, IDOK);

	return 0;
}

UINT __stdcall handle_uins(LPVOID pParam)
{
	HWND hDlg = (HWND)pParam;

	char buf2[100] = { 0 };
	GetDlgItemTextA(hDlg, IDC_EDIT_PASSWORD, buf2, 100);

	char url[500] = { 0 };
	sprintf_s(url, 500, "c.html?act=c.unins&alb=private&a=%s&p=%s", "admin", buf2);

	char* szResponse = 0;
	char* szMimeType = 0;
	mygetfunc(url, &szResponse, &szMimeType);

	if (strcmp(szResponse, "err") == 0)
	{
		_procs = L"";
		KillTimer(hDlg, IDT_TIMERLOAD);

		string sz = "Invalid password!";
		CLang::Instance().translate(sz);
		SetDlgItemText(hDlg, IDC_PWD_ERR, _u2w(sz).c_str());
		EnableWindow(GetDlgItem(hDlg, IDOK), TRUE);
	}
	else
	{
		RemoveSample(L"\\data2");

		exitcode = 100;
		KillTimer(hDlg, IDT_TIMERLOAD);
		EndDialog(hDlg, IDOK);
	}
	LocalFree(szResponse);

	return 0;
}

void transl2(HWND hDlg, UINT id)
{
	char buf[2048] = { 0 };
	GetDlgItemTextA(hDlg, id, buf, 2048);

	string sz = buf;
	CLang::Instance().translate(sz);
	::SetDlgItemText(hDlg, id, _u2w(sz).c_str());
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		transl2(hDlg, IDC_DECRYPT);
		transl2(hDlg, IDOK);
		transl2(hDlg, IDCANCEL);
		transl2(hDlg, IDC_STATIC_PWD);
		transl2(hDlg, IDC_STATIC_GROP);
		SetDlgItemText(hDlg, IDC_PWD_ERR, L"");

		_procs = L"";
		SendMessage(GetDlgItem(hDlg, IDC_EDIT_PASSWORD), EM_LIMITTEXT, 48, 0L);
	}
	return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			SetDlgItemText(hDlg, IDC_PWD_ERR, L"");

			char buf2[100] = { 0 };
			GetDlgItemTextA(hDlg, IDC_EDIT_PASSWORD, buf2, 100);

			if (strlen(buf2) == 0)
			{
				SetFocus(GetDlgItem(hDlg, IDC_EDIT_PASSWORD));
				string sz = "Invalid password!";
				CLang::Instance().translate(sz);
				SetDlgItemText(hDlg, IDC_PWD_ERR, _u2w(sz).c_str());
			}
			else
			{
				char url[500] = { 0 };
				sprintf_s(url, 500, "c.html?act=c.uninspass&alb=private&a=%s&p=%s", "admin", buf2);

				char* szResponse = 0;
				char* szMimeType = 0;
				mygetfunc(url, &szResponse, &szMimeType);

				if (strcmp(szResponse, "err") == 0)
				{
					LocalFree(szResponse);

					SetFocus(GetDlgItem(hDlg, IDC_EDIT_PASSWORD));
					string sz = "Invalid password!";
					CLang::Instance().translate(sz);
					SetDlgItemText(hDlg, IDC_PWD_ERR, _u2w(sz).c_str());
				}
				else
				{
					LocalFree(szResponse);

					_procs = L"...";
					SetDlgItemText(hDlg, IDC_PWD_ERR, _procs.c_str());

					_IDC_PWD_ERR = IDC_PWD_ERR;
					EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);
					SetTimer(hDlg, IDT_TIMERLOAD, 300, (TIMERPROC)LoadTimerProc);

					string sz = "Decrypt";
					CLang::Instance().translate(sz);
					_decrypt = _u2w(sz) + L" ";

					_beginthreadex(NULL, 0, handle_uins, hDlg, NULL, NULL);
				}
			}
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			exitcode = 200;
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK About0(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		_IDC_PWD_ERR = IDC_PWD_ERR0;
		_procs = L"...";
		SetDlgItemText(hDlg, IDC_PWD_ERR0, _procs.c_str());
		SetTimer(hDlg, IDT_TIMERLOAD, 300, (TIMERPROC)LoadTimerProc);

		string sz = "Delete thumbnails image";
		CLang::Instance().translate(sz);
		_decrypt = _u2w(sz) + L" ";

		_beginthreadex(NULL, 0, handle_uins0, hDlg, NULL, NULL);
	}
	return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			exitcode = 200;
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}

	return (INT_PTR)FALSE;
}

void aaa(wstring subKey)
{
	wstring lang = L"en";

	HKEY hkey;
	long ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subKey.c_str(), 0, KEY_QUERY_VALUE, &hkey);
	if (ret == ERROR_SUCCESS)
	{
		wchar_t buf[100] = { 0 };// | KEY_WOW64_64KEY

		DWORD dwType = REG_SZ;
		DWORD dwSize = sizeof(buf);

		RegQueryValueEx(hkey, L"Inno Setup: Language", NULL, &dwType, (BYTE*)buf, &dwSize);
		lang = buf;

		RegCloseKey(hkey);
	}

	UINT id = IDR_EN_HTML;
	if (lang == L"de")
	{
		id = IDR_DE_HTML;
	}
	else if (lang == L"en")
	{
		id = IDR_EN_HTML;
	}
	else if (lang == L"es")
	{
		id = IDR_ES_HTML;
	}
	else if (lang == L"fr")
	{
		id = IDR_FR_HTML;
	}
	else if (lang == L"it")
	{
		id = IDR_IT_HTML;
	}
	else if (lang == L"ja")
	{
		id = IDR_JA_HTML;
	}
	else if (lang == L"ru")
	{
		id = IDR_RU_HTML;
	}
	else if (lang == L"zh")
	{
		id = IDR_ZH_HTML;
	}
	else
	{
		id = IDR_EN_HTML;
		lang = L"en";
	}

	CLang::Instance().proc_lang(id);// lang + L".properties");//zh.properties
}

void ccc(HWND hwnd, string alb, wstring subKey)
{
	string url = "c.html?act=c.uninsbefore&alb=" + alb;

	char* szResponse = 0;
	char* szMimeType = 0;

	mygetfunc(url.c_str(), &szResponse, &szMimeType);
	if (strcmp(szResponse, "ok") == 0)
	{
		LocalFree(szResponse);
		RemoveSample((alb == "private") ? L"\\data2" : L"\\data1");
		exitcode = 0;
	}
	else
	{
		LocalFree(szResponse);
		//aaa(subKey);

		if (alb == "private")
		{
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, (DLGPROC)About, (LPARAM)0);
		}
		else
		{
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG0), hwnd, (DLGPROC)About0, (LPARAM)0);
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 3)
	{
		return 0;
	}

	string alb = "";
	wstring subKey = L"";
	HWND hwnd = (HWND)_wtoi64(argv[1]);

	if (wcscmp(argv[2], L"decryption") == 0)
	{
		alb = "private";
		subKey = L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\PhotoNest Encryption_is1";
	}
	else if (wcscmp(argv[2], L"nest1") == 0)
	{
		alb = "public";
		subKey = L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\PhotoNest_is1";
	}

	if (alb != "")
	{
		ccc(hwnd, alb, subKey);
	}

	return exitcode;
}