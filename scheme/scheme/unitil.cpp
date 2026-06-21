////////////////////////////////////////////////////////////////////
/// @file unitil.cpp
/// @brief 工具函数实现文件
/// @details 此文件实现了各种工具函数，
///          包括字符编码转换、URL 编解码、字符串处理等。
///          提供跨平台（Windows/Linux）的实用工具函数。
/// @note 使用 C++11 的 codecvt 进行 UTF-8 和 Unicode 转换
/// @author PhotoNest Team
/// @date 2024
////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "unitil.h"
#include <codecvt>
#include <shlobj.h>

////////////////////////////////////////////////////////////////////
/// @brief 将宽字符串（Unicode）转换为 UTF-8 字符串
/// @param[in] w 宽字符串（Unicode）
/// @return UTF-8 编码的字符串
/// @note 使用 C++11 的 codecvt_utf8 进行转换
////////////////////////////////////////////////////////////////////
string _w2u(wstring w)
{
	wstring_convert<codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(w);
}

////////////////////////////////////////////////////////////////////
/// @brief 将 UTF-8 字符串转换为宽字符串（Unicode）
/// @param[in] u UTF-8 编码的字符串
/// @return 宽字符串（Unicode）
/// @note 使用 C++11 的 codecvt_utf8 进行转换
////////////////////////////////////////////////////////////////////
wstring _u2w(string u)
{
	wstring_convert<codecvt_utf8<wchar_t>> conv;
	return conv.from_bytes(u);
}

////////////////////////////////////////////////////////////////////
/// @brief 将十六进制字符转换为数字
/// @param[in] c 十六进制字符（0-9, A-F, a-f）
/// @return 对应的数字（0-15），如果输入不是十六进制字符则返回 -1
////////////////////////////////////////////////////////////////////
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

string URLDecode(string& sURL, int type)
{
	string sDecodedURL;
	int nLength = (int)sURL.length();
	for (int i = 0; i < nLength; i++)
	{
		char c1 = sURL[i];
		if (c1 == '+')
		{
			sDecodedURL += ' ';
		}
		else if (c1 != '%')
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

	if (type == 1)
	{
		myEntity(sURL);
	}

	return sDecodedURL;
}

string URLDecode2(string& sURL)
{
	string sDecodedURL;
	int nLength = (int)sURL.length();
	for (int i = 0; i < nLength; i++)
	{
		char c1 = sURL[i];
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


//
string Replace(string& szBody, string szOld, string szNew)
{
	if (szOld == szNew)
	{
		return szBody;
	}

	int nLen = (int)szNew.length();

	string::size_type nFound = string::npos;
	do
	{
		nFound = szBody.find(szOld.c_str(), nFound != string::npos ? nFound + nLen : 0);
		if (nFound != string::npos)
		{
			szBody.replace(nFound, szOld.length(), szNew);
		}
	} while (nFound != string::npos);

	return szBody;
}


string ReplaceOne(string& szBody, string szOld, string szNew)
{
	if (szOld == szNew)
	{
		return szBody;
	}

	string::size_type nFound = string::npos;

	nFound = szBody.find(szOld, 0);
	if (nFound != string::npos)
	{
		szBody.replace(nFound, szOld.length(), szNew);
	}

	return szBody;
}


wstring ReplaceOne(wstring& szBody, wstring szOld, wstring szNew)
{
	if (szOld == szNew)
	{
		return szBody;
	}

	wstring::size_type nFound = wstring::npos;

	nFound = szBody.find(szOld, 0);
	if (nFound != wstring::npos)
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

	int nLen = (int)szNew.length();

	wstring::size_type nFound = wstring::npos;
	do
	{
		nFound = szBody.find(szOld, nFound != wstring::npos ? nFound + nLen : 0);
		if (nFound != wstring::npos)
		{
			szBody.replace(nFound, szOld.length(), szNew);
		}
	} while (nFound != wstring::npos);

	return szBody;
}

//
string trim(string& s, const string drop)
{
	// trim right
	s.erase(s.find_last_not_of(drop) + 1);
	// trim left
	return s.erase(0, s.find_first_not_of(drop));
}

string ltrim(string& s, const string drop)
{
	// trim left
	return s.erase(0, s.find_first_not_of(drop));
}

string rtrim(string& s, const string drop)
{
	// trim right
	return s.erase(s.find_last_not_of(drop) + 1);
}

int lookitem(string sz1, const char* szname, string& value, int nstart)
{
	//sz1 = "type=form_dis_u&id=&flag=0&txt=%20&amp;%20hh%20?%20hh&gt;&lt;&gt;&department=Lawrence%20Block&e=1327475260421";
	//type=form_dis_u&id=ad&flag=0&txt=%E7%94%84%E7%A7%80%E5%B3%B0=%20&amp;%20hh%20?%20hh=&lt;&gt;&department=Lawrence%20Block&e=1327476687109

	value = "";
	string sz = "&" + sz1;

	char buf[MAX_PATH] = { 0 };
	sprintf_s(buf, MAX_PATH, "&%s", szname);

	int nLen = (int)strlen(buf);

	string::size_type n = sz.find(buf, nstart);
	if (n != string::npos)
	{
		string temp = "";
		string::size_type n1 = sz.find("=", n + nLen);
		if (n1 != string::npos)
		{
			temp = sz.substr(n + nLen, n1 - n - nLen);
		}
		else
		{
			temp = sz.substr(n + nLen);
		}

		n1 = temp.rfind("&");
		if (n1 != string::npos)
		{
			value = temp.substr(0, n1);
		}
		else
		{
			value = temp;
		}
		return (int)(n + nLen);
	}
	return -1;
}

string my_tolower(string& s)
{
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}

wstring my_tolower(wstring& s)
{
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}
//
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

int hexchange(unsigned char* dest, const char* src)
{
	int i;
	char buf[256 * 2 + 1];

	unsigned char* table = (unsigned char*)malloc(65536);
	if (table != NULL)
	{
		memset(table, 0, 65536);
		unsigned short* shortSrc = (unsigned short*)buf;

		for (i = 0; i <= 255; i++)
		{
			sprintf_s(buf + i * 2, 3, "%02x", i);
		}

		for (i = 0; i <= 255; i++)
		{
			table[shortSrc[i]] = i;
		}

		while (*src)
		{
			*dest = table[*((unsigned short*)src)];
			src += 2;
			dest++;
		}

		free(table);
	}
	return 0;
}

unsigned short checksum(USHORT* buffer, int size)
{
	unsigned long cksum = 0;
	while (size > 1)
	{
		cksum += *buffer++;
		size -= sizeof(USHORT);
	}
	if (size != 0)
	{
		cksum += *(UCHAR*)buffer;
	}

	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	return (USHORT)(~cksum);
}

wstring get_module_path0(HMODULE hModule)
{
	wchar_t buf[MAX_PATH] = { 0 };
	long lSize = ::GetModuleFileName(hModule, buf, MAX_PATH);

	wchar_t* p = wcsrchr(buf, L'\\');
	if (p != NULL)
	{
		p[0] = 0;
	}

	wstring sz = buf;
	Replace(sz, L"\\", L"/");
	my_tolower(sz);

	return sz;
}

wstring get_module_path(HMODULE hModule)
{
	wchar_t buf[MAX_PATH] = { 0 };
	long lSize = ::GetModuleFileName(hModule, buf, MAX_PATH);

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

	wstring sz = buf;
	Replace(sz, L"\\", L"/");
	my_tolower(sz);

	return sz;
}

wstring get_appdata_path(string alb)
{
	std::wstring dst = L"";

	wchar_t* szDocument;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &szDocument)))
	{
		dst = szDocument;

		wstring p = get_module_path(NULL);
		wstring::size_type pos = p.find(L"/windowsapps/photonest");
		if (pos != wstring::npos)
		{
			dst += L"\\PhotoNest_App";
			dst += L"\\Nest1";
		}
		else
		{
			dst += L"\\PhotoNest";
			if (alb == "private")
			{
				dst += L"\\Nest2";
			}
			else
			{
				dst += L"\\Nest1";
			}
		}
		CoTaskMemFree(szDocument);
	}

	wstring sz = dst;
	Replace(sz, L"\\", L"/");
	my_tolower(sz);
	return sz;
}


string float2Str(float num, char* fmt)
{
	char buf[100] = { 0 };
	sprintf_s(buf, 100, fmt, num);
	return buf;
}

string Int2Str(int num)
{
	char buf[100] = { 0 };
	sprintf_s(buf, 100, "%d", num);

	return buf;
}

string Int2Str(uint32_t num)
{
	char buf[100] = { 0 };
	sprintf_s(buf, 100, "%u", num);

	return buf;
}

string uint2str(uint32_t num)
{
	char buf[100] = { 0 };
	sprintf_s(buf, 100, "%u", num);

	return buf;
}

string uint64str(uint64_t num)
{
	char buf[100] = { 0 };
	sprintf_s(buf, 100, "%llu", num);

	return buf;
}

uint32_t get_uint32(char* ptr)
{
	if (ptr)
	{
		return static_cast<uint32_t>(_atoi64(ptr));
	}
	else
	{
		return 0;
	}
}

uint32_t get_uint32(const char* ptr)
{
	if (ptr)
	{
		return static_cast<uint32_t>(_atoi64(ptr));
	}
	else
	{
		return 0;
	}
}

uint64_t get_uint64(char* ptr)
{
	if (ptr)
	{
		return static_cast<uint64_t>(_atoi64(ptr));
	}
	else
	{
		return 0;
	}
}


void get_request_url(const char* szUrl, std::string& strhost, std::string& strUrl, std::string& strArgs)
{
	strUrl = "";
	strArgs = "";
	string temp = szUrl;

	string::size_type ndx = temp.find('?');
	if (ndx != string::npos)
	{
		// yup; save the args....
		strArgs = temp.substr(ndx + 1);
		// strip from file name....
		strUrl = temp.substr(0, ndx);
	}
	else
	{
		strUrl = temp;
	}


	ndx = strUrl.find("//");
	if (ndx != string::npos)
	{
		strUrl = strUrl.substr(ndx + 2);
	}

	ndx = strUrl.find('/');
	if (ndx != -1)
	{
		strhost = strUrl.substr(0, ndx);
		strUrl = strUrl.substr(ndx);
	}

}

long lookitem_arr(string szBody, const char* szname, string& value)
{
	value = "";

	int nstart = 0;
	for (int i = 0; i < 100; i++)
	{
		string temp = "";

		nstart = lookitem(szBody, szname, temp, nstart);
		if (nstart == -1)
		{
			break;
		}
		value += temp + ",";
	}

	if (!value.empty())
	{
		rtrim(value, ",");
	}

	return 0;
}


string myEntity(string& s)
{
	//s.replace("<", "&lt;");
	//s.replace(">", "&gt;");
	//s.replace(">", "&gt;");
	//s.replace("\"", "&quot;");
	//s.replace("'", "&#39;");
	//{ "&amp;",  5, '&' },
	//{ "&lt;",   4, '<' },
	//{ "&gt;",   4, '>' },
	//{ "&quot;", 6, '\"' },
	//{ "&apos;", 6, '\'' }

	Replace(s, "&", "&amp;");
	Replace(s, "<", "&lt;");
	Replace(s, ">", "&gt;");
	//Replace(s, "%", "%25");

	return s;
}

string _data2_thumb(string& src)
{
	string::size_type  pos = src.find("galleries");
	if (pos == string::npos)
	{
		ReplaceOne(src, "/_data/", "/_thumb/");
	}
	else
	{
		ReplaceOne(src, "/galleries_", "/_thumb/galleries_");
	}

	return src;
}

char* get_value(char* ptr)
{
	if (ptr)
	{
		return ptr;
	}
	else
	{
		return "";
	}
}

float get_float(char* ptr)
{
	if (ptr)
	{
		return static_cast<float>(atof(ptr));
	}
	else
	{
		return 0;
	}
}

string get_today(time_t tt)
{
	string strDate = "";

	time_t current_time;
	time(&current_time);
	int64_t uuu = tt - current_time;
	uuu = uuu / 60;
	if (tt < 137000000 || uuu > 60 * 24 * 36500)
	{
		return "";
	}

	//tt is localtime
	struct tm t;
	gmtime_s(&t, &tt);
	if (t.tm_year == -1 || t.tm_mon < 0 || t.tm_mon >= 12)
	{
		return strDate;
	}

	char stamped[MAX_PATH] = { 0 };
	strftime(stamped, MAX_PATH, "%Y-%m-%d", &t);

	strDate = stamped;

	return strDate;
}
