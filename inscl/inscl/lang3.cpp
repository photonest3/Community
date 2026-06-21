///////////////////////////////////////////////////////////////////////
/// @file lang3.cpp
/// @brief 多语言支持类实现(第三版)
/// @details 实现 CLang 类的成员函数,包括语言文件读取、
///          翻译处理、HTML 内容解析等功能
/// @author PhotoNest Team
/// @date 2024
/// @version 1.0
/// @note 本文件是 inscl 模块的多语言支持实现文件
///////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include "unitil3.h"
#include "lang3.h"

CLang::CLang(void)
{
}

CLang::~CLang(void)
{
}

bool CLang::proc_lang(UINT id)
{
	HINSTANCE hrcInst = GetModuleHandle(L"inscl.dll");

	string szhtml = "";
	read_htm(hrcInst, id, szhtml);

	proc_html(szhtml);

	return true;
}

bool CLang::translate(string& strText)
{
	string szT = strText;

	map<string, string>::iterator I = _langMap.find(strText);
	if (I != _langMap.end())
	{
		strText = I->second;
		//trim(szT, " ");
	}
	if (strText == "")
	{
		strText = szT;
	}

	return true;
}

long CLang::read_htm(HINSTANCE hrcInst, UINT cr_id, string& szhtml)
{
	szhtml = "";

	HRSRC hrs = FindResource(hrcInst, MAKEINTRESOURCE(cr_id), RT_HTML);
	if (hrs != NULL)
	{
		DWORD dw = SizeofResource(hrcInst, hrs);
		HGLOBAL hg = LoadResource(hrcInst, hrs);
		if (hg != NULL)
		{
			void* p = LockResource(hg);
			if (p != NULL)
			{
				szhtml = string((LPSTR)p, dw);
			}
			FreeResource(hg);
		}
	}

	return 0;
}

void CLang::proc_html(string common)
{
	string fout = "";
	int pattern = 3;

	char last = '\0';
	for (uint32_t i = 0; i < common.length(); i++)
	{
		status_change(fout, pattern, common[i], last);
	}

	string key = "";
	string val = "";
	vector<string> v1 = tokenize(fout, "\n", true, "");
	int len = (int)v1.size();

	for (int i = len - 1; i >= 0; i--)
	{
		string::size_type ndx = v1[i].find('=');
		if (ndx != string::npos)
		{
			// yup; save the args....
			val = v1[i].substr(ndx + 1);
			// strip from file name....
			key = v1[i].substr(0, ndx);

			trim(key, " ");
			trim(key, "'");

			trim(val, " ");
			trim(val, "'");

			_langMap.insert(map<string, string>::value_type(key, val));
		}
	}
}

void CLang::status_change(string& fout, int& pattern, char& c, char& last)
{
	switch (pattern)
	{
	case 0:
	{
		fout += c;
		if (c == '"')
			pattern = 3;
		break;
	}
	case 1:
	{
		if (last != '\\' && c == '\n')
		{
			fout += c;
			c = '\0';
			pattern = 3;
		}

		break;
	}
	case 2:
	{
		if (last == '*' && c == '/')
		{
			c = '\0';
			pattern = 3;
		}
		break;
	}
	case 3:
	{
		//fout<<c;
		if (c == '"')
		{
			fout += c;
			pattern = 0;
		}
		else if (c == '/')
		{
			if (last == '/')
				pattern = 1;
		}
		else if (c == '*')
		{
			if (last == '/')
				pattern = 2;
		}
		else
		{
			if (last == '/')
			{
				fout += last;
			}

			fout += c;
			pattern = 3;
		}
		break;
	}
	}
	last = c;
}
