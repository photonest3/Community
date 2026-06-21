///////////////////////////////////////////////////////////////////////
/// @file unitil3.cpp
/// @brief 安装程序工具函数实现(inscl 模块)
/// @details 实现字符串转换、路径处理、文本处理等工具函数,
///          是 inscl 模块专用的工具函数实现文件
/// @author PhotoNest Team
/// @date 2024
/// @version 1.0
/// @note 本文件与 photol/ 模块的 unitil3.cpp 功能类似但独立
///////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "unitil3.h"
#include <codecvt>

string _w2u(wstring w)
{
	wstring_convert<codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(w);
}

wstring _u2w(string u)
{
	wstring_convert<codecvt_utf8<wchar_t>> conv;
	return conv.from_bytes(u);
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

string trim(string& s, const string drop)
{
	// trim right
	s.erase(s.find_last_not_of(drop) + 1);
	// trim left
	return s.erase(0, s.find_first_not_of(drop));
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

wstring get_module_path(HMODULE hModule)
{
	wchar_t buf[MAX_PATH];
	memset(buf, 0, MAX_PATH * sizeof(wchar_t));
	long lSize = ::GetModuleFileNameW(hModule, buf, MAX_PATH);

	wchar_t* p = wcsrchr(buf, L'\\');
	if (p != NULL)
	{
		p[0] = 0;
	}

	wstring s = buf;
	Replace(s, L"\\", L"/");

	transform(s.begin(), s.end(), s.begin(), ::tolower);

	return s;
}
