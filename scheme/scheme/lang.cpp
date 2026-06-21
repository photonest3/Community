////////////////////////////////////////////////////////////////////
/// @file lang.cpp
/// @brief 多语言翻译处理实现文件
/// @details 此文件实现了 CLang 类的所有方法，
///          包括语言文件解析、翻译映射表构建、
///          文本翻译、语言切换等功能。
///          支持 XML 格式的语言文件，提供多语言支持。
/// @note 使用 TinyXML 解析 XML 语言文件
/// @author PhotoNest Team
/// @date 2024
////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "lang.h"
#include "unitil.h"
#include "../../locales/locales/resource.h"
#include "../../locales/locales/locales.h"
#pragma comment( lib, "../../../PhotoNest/tests/cefclient/Release/bin/locales.lib" ) 

CLang::CLang(void)
{
	_language = "";
}

CLang::~CLang(void)
{
	_listTempDto.clear();
	_langMap.clear();
}

long CLang::reset(string ty)
{
	if ("templ" == ty)
	{
		_listTempDto.clear();
	}
	return 0;
}

long CLang::refresh_language()
{
	_langMap.clear();
	proc_lang(_u2w(_language));

	return 0;
}

int CLang::ParseLangXML(string ty, string szhtml)
{
	TiXmlDocument* pDoc = new TiXmlDocument();
	pDoc->Parse(szhtml.c_str(), NULL, TIXML_ENCODING_UTF8);

	if (pDoc->Error())
	{
		delete pDoc;
		return 1;
	}

	TiXmlNode* root = pDoc;

	list<TiXmlNode*> n1;
	GetElementsByTagName(root, "item", n1);

	if (n1.size() > 0)
	{
		int index = 0;
		list<TiXmlNode*>::iterator it;
		for (it = n1.begin(); it != n1.end(); it++)
		{
			TiXmlNode* ev = (TiXmlNode*)*it;

			TEMPL_DTO dto;
			int ret = Parse(ev, dto);
			if (ret == 0)
			{
				if ("templ" == ty)
				{
					_listTempDto.push_back(dto);
				}
				else
				{
					if (dto.val.empty())
					{
						dto.val = dto.id;
					}
					_langMap.insert(map<string, string>::value_type("{'" + dto.id + "'}", dto.val));
				}
			}
		}
	}

	delete pDoc;

	return 0;
}

//string CLang::procLangData(string common)
//{
//	string fout = "";
//	int pattern = 3;
//
//	char last = '\0';
//	for (uint32_t i = 0; i < common.length(); i++)
//	{
//		StatusChange(fout, pattern, common[i], last);
//	}
//
//	string key = "";
//	string val = "";
//	vector<string> v1 = tokenize(fout, "\n", true, "");
//	int len = (int)v1.size();
//
//	//for (int i = len - 1; i >= 0; i--)
//	for (int i = 0; i < len; i++)
//	{
//		string::size_type ndx = v1[i].find('=');
//		if (ndx != string::npos)
//		{
//			// yup; save the args....
//			val = v1[i].substr(ndx + 1);
//			// strip from file name....
//			key = v1[i].substr(0, ndx);
//			trim(key, " ");
//
//			ReplaceOne(key, "$lang[", "{");
//			ReplaceOne(key, "]", "}");
//
//			rtrim(val, ";");
//			trim(val, " ");
//			trim(val, "'");
//			trim(val, "\"");
//
//			_langMap.insert(map<string, string>::value_type(key, val));
//		}
//	}
//	return "";
//}

bool CLang::proc_lang(wstring lang)
{
	UINT id_1 = IDR_1_EN_XML;
	UINT id_0 = IDR_0_EN_XML;

	if (lang == L"de")
	{
		id_1 = IDR_1_DE_XML;
		id_0 = IDR_0_DE_XML;
	}
	else if (lang == L"en")
	{
		id_1 = IDR_1_EN_XML;
		id_0 = IDR_0_EN_XML;
	}
	else if (lang == L"es")
	{
		id_1 = IDR_1_ES_XML;
		id_0 = IDR_0_ES_XML;
	}
	else if (lang == L"fr")
	{
		id_1 = IDR_1_FR_XML;
		id_0 = IDR_0_FR_XML;
	}
	else if (lang == L"it")
	{
		id_1 = IDR_1_IT_XML;
		id_0 = IDR_0_IT_XML;
	}
	else if (lang == L"ja")
	{
		id_1 = IDR_1_JA_XML;
		id_0 = IDR_0_JA_XML;
	}
	else if (lang == L"ru")
	{
		id_1 = IDR_1_RU_XML;
		id_0 = IDR_0_RU_XML;
	}
	else if (lang == L"zh")
	{
		id_1 = IDR_1_ZH_XML;
		id_0 = IDR_0_ZH_XML;
	}

	_langMap.clear();
	string parData = "";
	//Read_file(L"locales/album/1_" + lang + L".xml"), parData);
	//ParseLangXML("lang", parData);

	//parData = "";
	//Read_file(L"locales/paint/0_" + lang + L".xml"), parData);
	//ParseLangXML("lang", parData);
	wstring fname = L"locales/album/1_" + lang + L".xml";

	char* szResponse = 0;
	int ret = Read_xml(id_1, fname.c_str(), &szResponse);
	if (ret == 0)
	{
		parData = szResponse;
		LocalFree(szResponse);
	}
	ParseLangXML("lang", parData);

	fname = L"locales/paint/0_" + lang + L".xml";
	ret = Read_xml(id_0, fname.c_str(), &szResponse);
	if (ret == 0)
	{
		parData = szResponse;
		LocalFree(szResponse);
	}
	ParseLangXML("lang", parData);

	//procLangData(parData);
	return true;
}

long CLang::getOneData(string id, string& value)
{
	list<TEMPL_DTO>::iterator iter;
	for (iter = _listTempDto.begin(); iter != _listTempDto.end(); iter++)
	{
		if (iter->id == id)
		{
			value = iter->val;
			break;
		}
	}
	return 0;
}

bool CLang::translate(string& strText, string intVal, string szVal)
{
	list<string> lstKey;
	Judge(strText, lstKey);

	Replace(strText, "|@translate", "");

	string szT = "";
	list<string>::iterator iter;
	for (iter = lstKey.begin(); iter != lstKey.end(); iter++)
	{
		szT = *iter;
		map<string, string>::iterator I = _langMap.find(*iter);
		if (I != _langMap.end())
		{
			//trim(szT, " ");
			if (szT != "")
			{
				szT = I->second;
			}
		}
		else
		{
			ltrim(szT, "{'");
			rtrim(szT, "'}");
		}

		if (!intVal.empty())
		{
			ReplaceOne(szT, "%d", intVal);
		}
		if (!szVal.empty())
		{
			ReplaceOne(szT, "%s", szVal);
		}

		ReplaceOne(strText, *iter, szT);
	}

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

	ltrim(strText, "{'");
	rtrim(strText, "'}");

	return true;
}

long CLang::Parse(TiXmlNode* parent, TEMPL_DTO& dto)
{
	if (!parent) return 1;

	TiXmlNode* pChild;

	for (pChild = parent->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
	{
		string val = pChild->ValueStr();

		if (val == "id")
		{
			dto.id = _GetNodeValue(pChild);
		}
		else if (val == "templ" || val == "transl")
		{
			dto.val = _GetNodeValue(pChild);
		}
	}

	return 0;
}

list<TiXmlNode*> CLang::GetElementsByTagName(TiXmlNode* parent, string name, list<TiXmlNode*>& list)
{
	if (!parent)
	{
		return list;
	}

	string val = parent->ValueStr();
	if (val == name)
	{
		list.push_back(parent);
	}

	TiXmlNode* pChild;
	for (pChild = parent->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
	{
		GetElementsByTagName(pChild, name, list);
	}

	return list;
}

string CLang::_GetNodeValue(TiXmlNode* node)
{
	string txt = node->FirstChild() != NULL ? node->FirstChild()->Value() : "";
	//if (txt.empty())
	//{
	//	txt = node->Value();
	//}

	return txt;
}

//void CLang::StatusChange(string& fout, int& pattern, char& c, char& last)
//{
//	switch (pattern)
//	{
//	case 0:
//	{
//		fout += c;
//		if (c == '"')
//			pattern = 3;
//		break;
//	}
//	case 1:
//	{
//		if (last != '\\' && c == '\n')
//		{
//			fout += c;
//			c = '\0';
//			pattern = 3;
//		}
//
//		break;
//	}
//	case 2:
//	{
//		if (last == '*' && c == '/')
//		{
//			c = '\0';
//			pattern = 3;
//		}
//		break;
//	}
//	case 3:
//	{
//		//fout<<c;
//		if (c == '"')
//		{
//			fout += c;
//			pattern = 0;
//		}
//		else if (c == '/')
//		{
//			if (last == '/')
//				pattern = 1;
//		}
//		else if (c == '*')
//		{
//			if (last == '/')
//				pattern = 2;
//		}
//		else
//		{
//			if (last == '/')
//			{
//				fout += last;
//			}
//
//			fout += c;
//			pattern = 3;
//		}
//		break;
//	}
//	}
//	last = c;
//}

long CLang::Read_file(wstring filename, string& szhtml)
{
	szhtml = "";
	wstring szPath = get_module_path0(NULL);
	wstring	s1 = szPath + L"/";
	s1 += filename;

	HANDLE hFile = CreateFile(s1.c_str(), GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		// Retrive file size
		int lengthActual = GetFileSize(hFile, NULL);
		DWORD dwNumBytesRead = 0;

		char* pBuf = (char*)malloc(lengthActual + 1);
		if (pBuf != NULL)
		{
			memset(pBuf, 0, lengthActual + 1);

			BOOL bResult = ReadFile(hFile, pBuf, lengthActual, &dwNumBytesRead, 0);
			if (bResult)
			{
				szhtml = string(pBuf, lengthActual);
			}

			free(pBuf);
		}

		CloseHandle(hFile);
	}
	return 0;
}

bool CLang::Judge(string str, list<string>& lstKey)
{
	//stack <char>s;
	int idx1 = 0;
	string val = "";
	int len = (int)str.length();
	for (int i = 0; i < len; i++)
	{
		switch (str[i]) {
		case '{':
			val = "";
			idx1 = i;
			break;

		case '}':

			val = str.substr(idx1 + 1, i - idx1 - 1);
			if (val != "")
			{
				string::size_type pos = val.find("|@translate");
				if (pos != string::npos)
				{
					lstKey.push_back("{" + val.substr(0, pos) + "}");
				}
			}

			break;
		}

	}
	return 0;
}
