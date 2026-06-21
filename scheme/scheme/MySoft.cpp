////////////////////////////////////////////////////////////////////
/// @file MySoft.cpp
/// @brief 软件授权验证实现文件
/// @details 此文件实现了 CMySoft 类的所有方法，
///          包括序列号验证、授权文件解析、
///          授权模式检测（标准版、专业版）等功能。
///          用于保护软件知识产权和控制功能访问。
/// @note 使用 Blowfish 算法进行授权文件解密
/// @author PhotoNest Team
/// @date 2024
////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "MySoft.h"
#include "unitil.h"
#include "Blowfish.h"
#include "stmdef.h"

CMySoft::CMySoft(void)
{
}

CMySoft::~CMySoft(void)
{
}

//return ERROR_LIC || MODE_STD || ret == MODE_PRO || ERROR_SIGN
int CMySoft::Verify(string s2, int mode, lic_header& lic)
{
	string serial = s2;

	if (serial.empty())
	{
		return ERROR_LIC;
	}

	int ret = Reg(serial, lic);
	char name[30];
	for (int i = 0; i < 200; i++)
	{
		ScrubBlob(name, 30);
	}
	//good
	if (ret == mode)
	{
		return 0;
	}

	return ret;
}

int CMySoft::Reg(string s2, lic_header& lic)
{
	s2 = my_tolower(s2);
	unsigned int len = 0;
	unsigned int i, j;

	if (s2.size() < 32)
	{
		return ERROR_LIC;
	}

	if (s2.size() == 32)
	{
		time_t t2;
		time(&t2);

		tm tm1;
		tm1.tm_year = 2024 - 1900;
		tm1.tm_mon = 0;
		tm1.tm_mday = 1;
		tm1.tm_hour = 0;
		tm1.tm_min = 0;
		tm1.tm_sec = 0;
		tm1.tm_isdst = 0;

		time_t t1 = _mkgmtime(&tm1);
		int day = (t2 - t1) / 60 / 60 / 24;

		char buf2[10] = { 0 };
		_itoa_s(day, buf2, 16);
		s2 += buf2;
	}

	string day = s2.substr(32);
	if (day.size() < 2 || day.size() > 4)
	{
		return ERROR_LIC;
	}

	
	s2 = s2.substr(0, 32);

	char sn[256] = { 0 };
	strncpy_s(sn, 255, s2.c_str(), 255);
	len = (unsigned int)strlen(sn);

	for (i = 0, j = 0; i < len; i++)
	{
		if (isxdigit(sn[i]) == 0)
		{
			j = 1;
			break;
		}
	}

	if (len == 32 && j != 1 && len != 0)
	{
		CBlowfish  m_BF;

		char buf[10] = { 0 };
		sprintf_s(buf, 10, "%d%d", 2022, 3567);

		m_BF.SetPassword(buf);

		unsigned char hexsn[1024] = { 0 };
		hexchange(hexsn, sn);

		m_BF.Decrypt(hexsn, sizeof(lic_header));

		unsigned short sum = checksum((USHORT*)hexsn, sizeof(lic_header));

		if (sum == 0)
		{
			lic_header* p = (lic_header*)hexsn;
			if (p->version >= 20 && p->num > 0 && p->num < 99999)
			{
				lic.version = p->version;
				lic.mode = p->mode;
				lic.num = p->num;
				lic.chksum = p->chksum;
				lic.ov = p->ov;
				lic.year = p->year;
				lic.date = p->date;

				int a = strtol(day.c_str(), NULL, 16);

				tm tm1;
				tm1.tm_year = 2024 - 1900;
				tm1.tm_mon = 0;
				tm1.tm_mday = 1;
				tm1.tm_hour = 0;
				tm1.tm_min = 0;
				tm1.tm_sec = 0;
				tm1.tm_isdst = 0;

				time_t t1 = _mkgmtime(&tm1) + a * EXP_SPAN;
				lic.date = t1;

				tm1.tm_year = 2024 - 1900 + lic.year;
				time_t t2 = _mkgmtime(&tm1) + a * EXP_SPAN;

				if (lic.year == 0)
				{
					t2 += 30 * EXP_SPAN;
				}

				time_t current_time;
				time(&current_time);
				if (current_time > t2)
				{
					return ERROR_OVERDUE;

				}


				//u = p->users;
				//if (MODE_PRO == p->mode)
				//{
				//	u = 10000;
				//}
				return p->mode;
			}
		}
	}

	return ERROR_LIC;
}

void CMySoft::ScrubBlob(void* b, DWORD cb)
{
	for (int i = 0; i < 7; i++)
	{
		memset(b, 0xFF, cb); // all 1's
		memset(b, 0x00, cb); // all 0's
		memset(b, 0xAA, cb); // 10101010
		memset(b, 0x55, cb); // 01010101
	}
	ZeroMemory(b, cb);
}

void CMySoft::get_lic(string szSubKey, string& v3)
{
	HKEY hKey;
	if (RegOpenKeyExA(HKEY_CURRENT_USER, szSubKey.c_str(), 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwSize = 1024;
		char buf[1024] = { 0 };
		DWORD dwType = REG_SZ;
		long ret = RegQueryValueExA(hKey, "wo3", NULL, &dwType, (LPBYTE)buf, &dwSize);
		v3 = buf;

		RegCloseKey(hKey);
	}
}

long CMySoft::add_lic(string szSubKey, string key, string val)
{
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
		RegSetValueExA(hKey, key.c_str(), 0, REG_SZ, (BYTE*)val.c_str(), (DWORD)val.size());

		RegCloseKey(hKey);
	}
	return 0;
}

BOOL CMySoft::Grn(lic_header lic, string& sLic)
{
	time_t t2 = lic.date;

	CBlowfish  m_BF;

	char buf0[10] = { 0 };
	sprintf_s(buf0, 10, "%d%d", 2022, 3567);

	m_BF.SetPassword(buf0);

	unsigned short sum = checksum((USHORT*)(&lic), sizeof(lic_header));
	lic.chksum = sum;

	unsigned char* ptr = (unsigned char*)&lic;

	m_BF.Encrypt(ptr, sizeof(lic_header));

	char buf[100] = { 0 }, tmp[20] = { 0 };

	for (int i = 0; i < sizeof(lic_header); i++)
	{
		memset(tmp, 0, 20);
		sprintf_s(tmp, "%02x", ptr[i]);
		strcat_s(buf, tmp);
	}

	sLic = buf;

	tm tm1;
	tm1.tm_year = 2024 - 1900;
	tm1.tm_mon = 0;
	tm1.tm_mday = 1;
	tm1.tm_hour = 0;
	tm1.tm_min = 0;
	tm1.tm_sec = 0;
	tm1.tm_isdst = 0;

	time_t t1 = _mkgmtime(&tm1);
	int day = (t2 - t1) / 60 / 60 / 24;

	char buf2[10] = { 0 };
	_itoa_s(day, buf2, 16);
	sLic += buf2;

	return TRUE;
}
