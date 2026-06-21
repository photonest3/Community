#pragma once  

#include <list>
#include <string>
#include <vector>
#include <map>
#include "Singleton.h"

using namespace std;

#define WM_STEP WM_USER+170
#define WM_GB WM_USER+175
#define WM_ACTIVATED WM_USER+176
#define IDT_TIMERTIP 2011


class IsoHandler : public Singleton<IsoHandler>
{
	friend class Singleton<IsoHandler>;
protected:
	IsoHandler(void);
	~IsoHandler(void);

public:
	int ShowDialogBox(HWND hwnd, UINT id, LPARAM lParam);
	void Toggle(UINT nCmdShow);
	void show_dlgf11_close();
	wstring OnDir(HWND hwnd, int dirType);

private:
	void show_dlgf11();
	static INT_PTR CALLBACK ISOProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK Register(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK F11Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK LoadingProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK MyTimerProc(HWND hDlg, UINT message, UINT idTimer, DWORD dwTime);
	static INT_PTR CALLBACK AlertProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK BrowseCallbackProc(HWND hDlg, UINT uMsg, LPARAM lParam, LPARAM lpData);
	static UINT __stdcall SaveIso(LPVOID pParam);
	static UINT __stdcall SetFolderGB(LPVOID pParam);

	static BOOL CALLBACK UilanguageEnumproc(LPSTR unnamedParam1, LONG_PTR unnamedParam2);

	void create(HWND hDlg, const wchar_t* filename, vector<wstring> s2);
	void write_archive(string SrcDir, char* buff, void* a0, struct stat& st, int ty);
	HRESULT CreateLink(LPCWSTR lpszPathObj, LPCWSTR lpszPathLink, LPCWSTR lpszDesc = NULL, LPCWSTR lpszArgs = NULL, LPCWSTR lpszIcon = NULL);
	long mySendMessage(HWND hDlg, const char* pSrcDir, const char* pName);

	volatile bool _cancel_iso;
	volatile bool _start_iso;
	wstring _pIsoPath;
	wstring _appdata;
	wstring _moduledata;

	RECT _window_rect;


	HWND hwndF11_;
	bool _first_f11_over;

	int _dirType;
	wstring _currDir;

public:
	bool fullscreen_;
	HWND hwndLoading_;
	map<string, string> _roots_map;

public:
	void set_app_path(int type, wstring album);
	void get_prof_rcwin(int& zm, int& pub_default, RECT& rcWin);
	void WritePrivate(RECT rcWin, int zm);
	void WritePrivate(const char* key, const char* val);
	void ReadPrivate(const char* key, string& val);
private:
	wstring _iniPath;

public:
	string username_;
	string enckey_;
	wstring album_;
	string disabledrag_;

	HWND root_hwnd_;
	int viewer_;
	bool flag_video_;
	bool debug_ = false;

	int _jsclose;

	wstring get_appdata_path();
	wstring get_module_path2();
	void reset_appdata_path(wstring& p);
	wstring get_extension_dir();

};