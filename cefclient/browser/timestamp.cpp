#include <Windows.h>
#include "timestamp.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "../../scheme/scheme/scheme.h" 
#include "include/internal/cef_string.h"
#include <shellapi.h>
#include <shlobj.h>

int HexDigit(char c)
{
	int rVal = -1;

	unsigned char ch = (unsigned char)c;

	if (isdigit(ch))
		rVal = c - '0';
	else if (c >= 'A' && c <= 'F')
		rVal = 10 + c - 'A';
	else if (c >= 'a' && c <= 'f')
		rVal = 10 + c - 'a';

	return rVal;
}

string URLDecode(string& sURL)
{
	string sDecodedURL;
	size_t nLength = sURL.length();
	for (size_t i = 0; i < nLength; i++)
	{
		char c1 = sURL[i];
		//if (c1 == '+')
		//{
		//	sDecodedURL += ' ';
		//}
		//else
		if (c1 != '%')
		{
			sDecodedURL += c1;
		}
		else
		{
			if (i < nLength - 2)
			{
				int msb = HexDigit(sURL[i + 1]);
				int lsb = HexDigit(sURL[i + 2]);
				if (msb != -1 && lsb != -1)
				{
					int nChar = (msb << 4) + lsb;
					sDecodedURL += char(nChar);
					i += 2;
				}
				else
					sDecodedURL += c1;
			}
			else
				sDecodedURL += c1;
		}
	}
	sURL = sDecodedURL;
	return sDecodedURL;
}

void string2map(const wchar_t* szSyn, map<string, string>& m)
{
	m.clear();
	string s = CefString(szSyn);
	transform(s.begin(), s.end(), s.begin(), ::tolower);

	vector<string> v1 = tokenize(s, "|", true, "");
	int len = (int)v1.size();
	for (int i = 0; i < len; i++)
	{
		string part = v1[i];
		string::size_type pos0 = part.find(";");
		if (pos0 != string::npos)
		{
			string id = "/galleries_" + part.substr(0, pos0) + "/";
			string root_dir = part.substr(pos0 + 1);
			m.insert(map<string, string>::value_type(id, root_dir));

		}
	}
}

vector<string> tokenize(const string& src, string tok, bool btrim, string null_subst)
{
	vector<string> v;
	if (src.empty() || tok.empty())
	{
		return v;
		//throw "tokenize: empty string\0";
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
string ReplaceOne(string& szBody, string szOld, string szNew)
{
	if (szOld == szNew)
	{
		return szBody;
	}

	string::size_type nFound = string::npos;

	nFound = szBody.find(szOld.c_str(), 0);
	if (nFound != string::npos)
	{
		szBody.replace(nFound, szOld.length(), szNew);
	}

	return szBody;
}

wstring Replace(wstring& szBody, wstring szOld, wstring szNew)
{
	if (szOld == szNew)
	{
		return szBody;
	}

	size_t nLen = szNew.length();

	wstring::size_type nFound = wstring::npos;
	do
	{
		nFound = szBody.find(szOld.c_str(), nFound != wstring::npos ? nFound + nLen : 0);
		if (nFound != wstring::npos)
		{
			szBody.replace(nFound, szOld.length(), szNew);
		}
	} while (nFound != wstring::npos);

	return szBody;
}

wstring get_module_path()
{
	HMODULE hModule = ::GetModuleHandle(NULL);

	wchar_t buf[MAX_PATH] = { 0 };
	GetModuleFileNameW(hModule, buf, MAX_PATH);

	wchar_t* p = wcsrchr(buf, L'\\');
	if (p != NULL)
	{
		p[0] = 0;
	}

	p = wcsrchr(buf, L'\\');
	if (p != NULL)
	{
		p[0] = 0;
	}
	wstring s = buf;
	transform(s.begin(), s.end(), s.begin(), ::tolower);

	return s;
}

string get_module_pathA()
{
	HMODULE hModule = ::GetModuleHandle(NULL);

	char buf[MAX_PATH] = { 0 };
	GetModuleFileNameA(hModule, buf, MAX_PATH);

	char* p = strrchr(buf, L'\\');
	if (p != NULL)
	{
		p[0] = 0;
	}

	p = strrchr(buf, L'\\');
	if (p != NULL)
	{
		p[0] = 0;
	}

	string s = buf;
	transform(s.begin(), s.end(), s.begin(), ::tolower);

	return s;
}

long transl(const char* szSrc, wstring& szDst)
{
	string url = string("c.html?act=c.transl&s2=") + szSrc;

	char* szResponse = 0;
	char* szMimeType = 0;
	mygetfunc(url.c_str(), &szResponse, &szMimeType);

	szDst = CefString(szResponse);
	LocalFree(szResponse);

	return 0;
}

void transl2(HWND hDlg, UINT id, int isBtn)
{
	wchar_t buf[2048] = { 0 };
	GetDlgItemText(hDlg, id, buf, 2048);

	wstring sz = L"";
	string s = CefString(buf);
	transl(s.c_str(), sz);

	::SetDlgItemText(hDlg, id, sz.c_str());

	if (isBtn == 1)
	{
		HDC hDC = ::GetDC(hDlg);
		HFONT control_font = (HFONT)SendMessage(hDlg, WM_GETFONT, 0, 0);
		HFONT old = (HFONT)SelectObject(hDC, control_font);
		SIZE size;
		GetTextExtentPoint32(hDC, sz.c_str(), sz.size(), &size);

		SelectObject(hDC, old);
		ReleaseDC(hDlg, hDC);

		HWND hwnd = ::GetDlgItem(hDlg, id);
		RECT rcWin;
		::GetWindowRect(hwnd, &rcWin);

		int newcx = size.cx;
		if (newcx > rcWin.right - rcWin.left - 50)
		{
			POINT pt;
			pt.x = rcWin.left;
			pt.y = rcWin.top;
			if (id == IDOK)
			{
				pt.x = rcWin.right - newcx - 50;
			}
			ScreenToClient(hDlg, &pt);
			MoveWindow(hwnd, pt.x, pt.y, newcx + 50, rcWin.bottom - rcWin.top, TRUE);
		}
	}
}

void transl2(HWND hDlg, UINT id, wchar_t* txt)
{
	wstring sz = L"";
	string s = CefString(txt);
	transl(s.c_str(), sz);
	::SetDlgItemText(hDlg, id, sz.c_str());
}

bool isurlok(string val)
{
	bool b = false;
	const char* urls[] = { ".pc/index.html",NULL };
	const char** filename = urls;

	while (*filename)
	{
		string::size_type pos = val.find(*filename);
		if (pos != string::npos)
		{
			b = true;
			break;
		}
		filename++;
	}

	//if (b)
	{
		string::size_type pos1 = val.find("index.logout");
		string::size_type pos2 = val.find("admin.firstrun");
		if (pos1 != string::npos || pos2 != string::npos)
		{
			b = false;
		}
	}
	return b;
}

