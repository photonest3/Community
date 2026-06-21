// ============================================================
// ins.cpp : PhotoNest 安装程序语言选择 DLL
// 功能：提供安装后的语言选择对话框，并写入 prof.ini 配置，随后启动主程序
// ============================================================

#include <windows.h>
#include "resource.h"   // 资源 ID 定义
#include <string>
#include <vector>
#include <shlobj.h>     // Windows 特殊文件夹 API
#include "SimpleIni.h"  // 第三方 INI 读写库（保留开源性质，仅做使用层注释）
#include <time.h>
#include <codecvt>      // UTF-8 / 宽字符转换

using namespace std;

int _ty = 1;                     // 产品类型：0=Cutout, 1=Nest1, 2=Nest2
wstring _exeFullName = L"";      // 主程序完整路径
map<string, string> _lcid;       // LCID 映射表（语言代码 -> 标识）
map<string, string> _lang;       // 语言名称映射表（标识 -> 名称）

// ------------------------------------------------------------
// 宽字符串转换为 UTF-8 编码的 std::string
// ------------------------------------------------------------
string _w2u(wstring w)
{
	wstring_convert<codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(w);
}

// ------------------------------------------------------------
// UTF-8 编码的 std::string 转换为宽字符串
// ------------------------------------------------------------
wstring _u2w(string u)
{
	wstring_convert<codecvt_utf8<wchar_t>> conv;
	return conv.from_bytes(u);
}

// ------------------------------------------------------------
// 在宽字符串中仅替换第一个匹配的子串
// szBody : 原始字符串（引用，会被修改）
// szOld  : 待替换的子串
// szNew  : 用于替换的新子串
// 返回值 : 替换后的字符串（与 szBody 内容一致）
// ------------------------------------------------------------
wstring ReplaceOne(wstring& szBody, wstring szOld, wstring szNew)
{
	if (szOld == szNew)
	{
		return szBody;
	}

	wstring::size_type nFound = wstring::npos;

	nFound = szBody.find(szOld.c_str(), 0);
	if (nFound != wstring::npos)
	{
		szBody.replace(nFound, szOld.length(), szNew);
	}

	return szBody;
}

// ------------------------------------------------------------
// 字符串分割函数
// src        : 原始字符串
// tok        : 分隔符集合（每个字符都可作为分隔符）
// btrim      : 是否忽略空token
// null_subst : 当不忽略空token时的占位字符串
// 返回值     : 分割后的字符串数组
// ------------------------------------------------------------
vector<string> tokenize(const string& src, string tok, bool btrim, string null_subst)
{
	vector<string> v;
	if (src.empty() || tok.empty())
	{
		return v;
	}

	string::size_type pre_index = 0, index = 0, len = 0;
	while ((index = src.find_first_of(tok, pre_index)) != string::npos)
	{
		if ((len = index - pre_index) != 0)
		{
			v.push_back(src.substr(pre_index, len));
		}
		else if (!btrim)
		{
			v.push_back(null_subst);
		}
		pre_index = index + 1;
	}

	string endstr = src.substr(pre_index);
	if (!btrim)
	{
		v.push_back(endstr.empty() ? null_subst : endstr);
	}
	else if (!endstr.empty())
	{
		v.push_back(endstr);
	}

	return v;
}

// ------------------------------------------------------------
// 从 DLL 资源中读取 HTML/文本内容
// cr_id   : 资源 ID
// szhtml  : 输出缓冲区指针（由函数内部申请内存，调用方负责释放）
// 返回值  : 0 表示成功，-1 表示失败
// ------------------------------------------------------------
long Read_htm(unsigned int cr_id, char** szhtml)
{
	long ret = -1;
	HINSTANCE Inst = GetModuleHandle(L"ins.dll");
	HRSRC hrs = FindResource(Inst, MAKEINTRESOURCE(cr_id), RT_HTML);
	if (hrs != NULL)
	{
		DWORD dw = SizeofResource(Inst, hrs);
		size_t len = dw + 1;
		*szhtml = (char*)LocalAlloc(LMEM_ZEROINIT, len);
		if (*szhtml != NULL)
		{
			HGLOBAL hg = LoadResource(Inst, hrs);
			if (hg != NULL)
			{
				void* p = LockResource(hg);
				if (p != NULL)
				{
					memcpy(*szhtml, p, dw);
					ret = 0;
				}
				else
				{
					LocalFree(*szhtml);
				}
				FreeResource(hg);
			}
		}
	}

	return ret;
}

// ------------------------------------------------------------
// 从资源文件加载语言列表，并填充 LCID 与语言名称映射
// lcid : 输出 LCID 映射表
// lang : 输出语言名称映射表
// ------------------------------------------------------------
void get_language(map<string, string>& lcid, map<string, string>& lang)
{
	string szhtml = "";
	char* szResponse = 0;
	int ret = Read_htm(IDR_LANG_TXT, &szResponse);
	if (ret == 0)
	{
		szhtml = szResponse;
		LocalFree(szResponse);
	}

	// 按行分割资源文本
	vector<string> v1 = tokenize(szhtml, "\n", true, "");
	size_t len = v1.size();
	// 从第 3 行开始解析 LCID 映射（每行格式：语言名,LCID）
	for (size_t i = 2; i < len; i++)
	{
		vector<string> v2 = tokenize(v1[i], ",", true, "");
		lcid.insert(map<string, string>::value_type(v2[1], v2[0]));
	}

	// 第 1 行为语言 ID 列表，第 2 行为对应的语言名称列表
	vector<string> lang_id = tokenize(v1[0], ",", true, "");
	vector<string> lang_name = tokenize(v1[1], ",", true, "");

	len = lang_id.size();
	for (size_t i = 0; i < len; i++)
	{
		lang.insert(map<string, string>::value_type(lang_id[i], lang_name[i]));
	}
}

// ------------------------------------------------------------
// 获取应用数据存放路径
// ty : 产品类型（决定子目录名称）
// 返回值 : 本地 AppData 下的 PhotoNest 子目录路径
// ------------------------------------------------------------
wstring get_appdata_path(int ty)
{
	std::wstring dst = L"";
	wchar_t lpszDefaultDir[MAX_PATH] = { 0 };
	wchar_t szDocument[MAX_PATH] = { 0 };

	LPITEMIDLIST pidl = NULL;
	// 获取当前用户的 Local AppData 目录
	SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
	if (pidl && SHGetPathFromIDList(pidl, szDocument))
	{
		GetShortPathName(szDocument, lpszDefaultDir, _MAX_PATH);
		dst = lpszDefaultDir;
		dst += L"\\PhotoNest";
		CreateDirectory(dst.c_str(), NULL);

		// 根据产品类型追加子目录
		if (ty == 0)
		{
			dst += L"\\Cutout";
		}
		else if (ty == 2)
		{
			dst += L"\\Nest2";
		}
		else
		{
			dst += L"\\Nest1";
		}
		CreateDirectory(dst.c_str(), NULL);
	}
	return dst;
}

// ------------------------------------------------------------
// 获取当前应使用的语言标识
// 查找顺序：1) prof.ini 配置文件 2) 注册表（Inno Setup 安装语言）3) 系统默认 UI 语言
// id : 输出语言标识（如 "en", "zh" 等）
// 返回值 : 固定返回 0
// ------------------------------------------------------------
int get_cur_lang(string& id)
{
	id = "";

	CSimpleIniA ini;
	ini.SetUnicode(true);
	wstring iniPath = get_appdata_path(_ty) + L"\\prof.ini";
	ini.LoadFile(iniPath.c_str());

	bool b = false;
	const char* pv1 = ini.GetValue("section", "language");
	if (pv1 == nullptr)
	{
		// 未找到配置文件时，尝试读取注册表中 Inno Setup 记录的语言
		wstring subKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\PhotoNest_is1";
		if (_ty == 0)
		{
			subKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\PhotoNest Cutout_is1";
		}
		else if (_ty == 2)
		{
			subKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\PhotoNest Encryption_is1";
		}

		HKEY hkey;
		long ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subKey.c_str(), 0, KEY_QUERY_VALUE, &hkey);
		if (ret == ERROR_SUCCESS)
		{
			wchar_t buf[100] = { 0 };

			DWORD dwType = REG_SZ;
			DWORD dwSize = sizeof(buf);

			RegQueryValueEx(hkey, L"Inno Setup: Language", NULL, &dwType, (BYTE*)buf, &dwSize);
			id = _w2u(buf);

			RegCloseKey(hkey);
		}
		else
		{
			// 注册表也读取失败时， fallback 到系统默认 UI 语言
			LCID lcd = GetUserDefaultUILanguage();
			char buf[100] = { 0 };
			sprintf_s(buf, 100, "0x%04X", lcd);

			map<string, string>::iterator I = _lcid.find(buf);
			if (I != _lcid.end())
			{
				id = I->second;
			}
		}
	}
	else
	{
		id = pv1;
	}

	// 将最终确定的语种写回 prof.ini，供下次使用
	ini.SetValue("section", "language", id.c_str());
	ini.SaveFile(iniPath.c_str());

	return 0;
}

/*
long GetFolderPics(wstring strPath, vector<WIN32_FIND_DATAW >& vecffd)
{
	strPath.erase(strPath.find_last_not_of(L"/") + 1);

	wchar_t* buf1 = (wchar_t*)malloc(1024 * sizeof(wchar_t));
	if (buf1 != NULL)
	{
		memset(buf1, 0, 1024 * sizeof(wchar_t));
		swprintf_s(buf1, 1024, L"%s/*", strPath.c_str());
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

		wstring dst = src;
		ReplaceOne(dst, L"\\templ\\sample\\", L"\\sample\\");

		wstring dst2 = dst + L".krf";
		DWORD dwAttr = GetFileAttributesW(dst2.c_str());
		if (dwAttr != -1)
		{
		}
		else
		{
			CopyFile(src.c_str(), dst.c_str(), TRUE);
		}

		DeleteFile(src.c_str());
	}
}

void get_first(string szSubKey, string& first)
{
	// | KEY_WOW64_64KEY
	HKEY hKey;
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, szSubKey.c_str(), 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwType = REG_SZ;
		char buf[1024] = { 0 };
		DWORD dwSize = 1024;
		long ret = RegQueryValueExA(hKey, "i3", NULL, &dwType, (LPBYTE)buf, &dwSize);
		first = buf;

		RegCloseKey(hKey);
	}
}

long add_first(string szSubKey)
{
	HKEY hKey;

	//DWORD dwDisposition = REG_OPENED_EXISTING_KEY;
	LONG lRet = RegCreateKeyExA(
		HKEY_LOCAL_MACHINE,
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
		time_t current_time;
		time(&current_time);

		char buf[100] = { 0 };
		sprintf_s(buf, 100, "%llu", (uint64_t)current_time);
		string t = buf;
		RegSetValueExA(hKey, "i3", 0, REG_SZ, (BYTE*)t.c_str(), (DWORD)t.size());

		RegCloseKey(hKey);
	}
	return 0;
}

long init(int ty)
{
	//Wow6432Node
	string szSubKey = "Software\\PhotoNest\\Nest";
	if (ty == 0)
	{
		szSubKey = "Software\\PhotoNest\\Cutout";
	}
	else if (ty == 2)
	{
		szSubKey = "Software\\PhotoNest\\Nest";
		//szSubKey = "Software\\PhotoNest\\Encryption";
	}
	string tmp = "";
	get_first(szSubKey, tmp);
	time_t first1 = _atoi64(tmp.c_str());
	if (first1 == 0)
	{
		add_first(szSubKey);
	}
	return 0;
}
*/

// ------------------------------------------------------------
// 语言选择对话框消息处理回调函数
// hDlg    : 对话框窗口句柄
// message : Windows 消息类型
// wParam / lParam : 消息参数
// 返回值  : TRUE 表示已处理，FALSE 表示未处理
// ------------------------------------------------------------
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG: // 对话框初始化：加载图标、填充语言下拉框并选中当前语言
	{
		RECT rc;
		GetWindowRect(GetDlgItem(hDlg, IDC_STATIC_TITLE), &rc);
		POINT pt;
		pt.x = rc.left;
		pt.y = rc.top;
		ScreenToClient(hDlg, &pt);
		pt.y -= 32 / 2 - 7;

		// 创建静态图标控件并加载程序图标
		HWND hStaticEdit3 = CreateWindowEx(0, TEXT("Static"), NULL, WS_VISIBLE | WS_CHILD | SS_ICON, 10, pt.y, 32, 32, hDlg, NULL, 0, 0);
		HINSTANCE hInstance = GetModuleHandle(L"ins.dll");
		HICON hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0);
		SendMessage(hStaticEdit3, STM_SETICON, (WPARAM)hIcon, 0);

		// 填充语言下拉框
		HWND hCombo1 = GetDlgItem(hDlg, IDC_COMBO_LANG);
		string id = "";
		get_cur_lang(id);

		int k = 1;
		int i = 0;
		map<string, string>::iterator I2;
		for (I2 = _lang.begin(); I2 != _lang.end(); I2++)
		{
			SendMessage(hCombo1, CB_ADDSTRING, i, (LPARAM)_u2w(I2->second).c_str());
			if (I2->first == id)
			{
				k = i; // 记录当前语言所在索引
			}
			i++;
		}
		SendMessage(hCombo1, CB_SETCURSEL, (WPARAM)k, (LPARAM)0);
	}
	return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK) // 用户点击确定：保存语言选择并启动主程序
		{
			CSimpleIniA ini;
			ini.SetUnicode(true);
			wstring iniPath = get_appdata_path(_ty) + L"\\prof.ini";
			ini.LoadFile(iniPath.c_str());

			wchar_t buf[100] = { 0 };
			wcscpy_s(buf, 100, L"/LANG=");
			int idx = SendMessage(GetDlgItem(hDlg, IDC_COMBO_LANG), CB_GETCURSEL, (WPARAM)0, (LPARAM)0);

			if (idx < 8)
			{
				int i = 0;
				map<string, string>::iterator I2;
				for (I2 = _lang.begin(); I2 != _lang.end(); I2++)
				{
					if (idx == i)
					{
						// 将选中的语言标识写入 INI
						ini.SetValue("section", "language", I2->first.c_str());
						wcscat_s(buf, 100, _u2w(I2->first).c_str());
						break;
					}
					i++;
				}
			}
			else
			{
				wcscat_s(buf, 100, L"en");
				ini.SetValue("section", "language", "en");
			}

			ini.SaveFile(iniPath.c_str());

			// 启动主程序，并传入 /LANG=xxx 参数
			ShellExecute(NULL, L"open", _exeFullName.c_str(), buf, NULL, SW_SHOWNORMAL);
			EndDialog(hDlg, LOWORD(wParam));

			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL) // 取消：直接关闭对话框
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// ------------------------------------------------------------
// 显示语言选择对话框（导出函数，供安装程序调用）
// hWnd   : 父窗口句柄
// ty     : 产品类型
// lpText : 主程序完整路径
// 返回值 : 固定返回 0
// ------------------------------------------------------------
int __stdcall ShowLangDialog(HWND hWnd, int ty, wchar_t* lpText)
{
	_ty = ty;
	get_language(_lcid, _lang);

	_exeFullName = lpText;
	HINSTANCE hInstance = GetModuleHandle(L"ins.dll");
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, About);

	return 0;
}

// ------------------------------------------------------------
// 静默选择语言并直接启动主程序（不显示对话框）
// ty     : 产品类型
// lpText : 主程序完整路径
// 返回值 : 固定返回 0
// ------------------------------------------------------------
int __stdcall SelectLangIni(int ty, wchar_t* lpText)
{
	_ty = ty;
	get_language(_lcid, _lang);

	string id = "";
	get_cur_lang(id);

	wchar_t buf[100] = { 0 };
	wcscpy_s(buf, 100, L"/LANG=");

	// 查找当前语言对应名称
	bool b = false;
	map<string, string>::iterator I2;
	for (I2 = _lang.begin(); I2 != _lang.end(); I2++)
	{
		if (I2->first.c_str() == id)
		{
			b = true;
			wcscat_s(buf, 100, _u2w(I2->first).c_str());
			break;
		}
	}

	if (!b)
	{
		wcscat_s(buf, 100, L"en"); // 默认英语
	}

	// 启动主程序
	ShellExecute(NULL, L"open", lpText, buf, NULL, SW_SHOWNORMAL);

	return 0;
}

/*
int __stdcall CopySample(int ty, wchar_t* lpDir)
{
	init(ty);
	if (ty != 0)
	{
		wstring path = lpDir;
		vector<WIN32_FIND_DATAW > vecffd;

		wstring s1 = path + L"\\templ\\sample\\lena";
		wstring s2 = path + L"\\templ\\sample\\sea";
		wstring s3 = path + L"\\templ\\sample";

		wstring s5 = path + L"\\sample";
		CreateDirectory(s5.c_str(), NULL);

		s5 = path + L"\\sample\\lena";
		CreateDirectory(s5.c_str(), NULL);

		s5 = path + L"\\sample\\sea";
		CreateDirectory(s5.c_str(), NULL);

		GetFolderPics(s1, vecffd);
		sc(s1, vecffd);

		vecffd.clear();
		GetFolderPics(s2, vecffd);
		sc(s2, vecffd);

		vecffd.clear();
		GetFolderPics(s3, vecffd);
		sc(s3, vecffd);

		RemoveDirectory(s1.c_str());
		RemoveDirectory(s2.c_str());
		RemoveDirectory(s3.c_str());
	}

	return 0;
}
*/