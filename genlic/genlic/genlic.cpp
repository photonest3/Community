
#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "resource.h"
#include <CommCtrl.h>
#include <string>
#include "resource.h"
#include "Blowfish.h"

using namespace std;

typedef struct _lic_header
{
	unsigned char version;
	unsigned char mode;
	unsigned short num;
	unsigned short chksum;
	unsigned char ov;
	unsigned char year;
	unsigned long long date;
} lic_header;

#define MODE_GALLERY 10
//#define MODE_ENCRYPTED 20
#define MODE_CUTOUT 30

#pragma comment(linker, "/subsystem:windows /ENTRY:wmainCRTStartup")

CBlowfish _BF;
string _strLic = "";

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
			sprintf_s(buf + i * 2, 3, "%02X", i);
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

long get_guid(string& szguid)
{
	GUID guid;
	char buf[100], tmp[20];

	HRESULT hr = CoCreateGuid(&guid);
	if (hr != S_OK)
	{
		return hr;
	}

	sprintf_s(buf, "%08X%04X%04X%02X%02X",
		guid.Data1, guid.Data2,
		guid.Data3, guid.Data4[0], guid.Data4[1]);

	for (int i = 2; i < 8; i++)
	{
		sprintf_s(tmp, "%02X", guid.Data4[i]);
		strcat_s(buf, tmp);
	}

	szguid = buf;

	return 0;
}

BOOL Grn(unsigned char ver, unsigned short num, unsigned char perm, unsigned char year, time_t current_time, string& sLic)
{
	lic_header lic;
	memset(&lic, 0, sizeof(lic_header));

	lic.version = ver;
	lic.mode = perm;
	lic.num = num;
	lic.chksum = 0;
	lic.ov = 0;
	lic.year = year;
	lic.date = current_time;
	//string szguid;
	//get_guid(szguid);

	//strncpy_s(lic.guid, szguid.c_str(), 7);

	unsigned short sum = checksum((USHORT*)(&lic), sizeof(lic_header));
	lic.chksum = sum;

	unsigned char* ptr = (unsigned char*)&lic;

	_BF.Encrypt(ptr, sizeof(lic_header));

	char buf[100] = { 0 }, tmp[20] = { 0 };

	for (int i = 0; i < sizeof(lic_header); i++)
	{
		memset(tmp, 0, 20);
		sprintf_s(tmp, "%02X", ptr[i]);
		strcat_s(buf, tmp);
	}

	sLic = buf;

	return TRUE;
}


INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_GALLERY, BM_SETCHECK, BST_CHECKED, 0);
		SetDlgItemText(hDlg, IDC_EDIT_LICENSE, L"");
		SetDlgItemText(hDlg, IDC_EDIT_YEAR, L"1");
	}
	return (INT_PTR)TRUE;
	case WM_CLOSE:
	{
		EndDialog(hDlg, LOWORD(wParam));
	}
	return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON_GRN)
		{
			unsigned int perm = MODE_GALLERY;
			//int chk = (int)SendDlgItemMessage(hDlg, IDC_ENCRYPTED, BM_GETCHECK, 0, 0);
			//if (chk != 0)
			//{
			//	perm = MODE_ENCRYPTED;
			//}
			int	chk = (int)SendDlgItemMessage(hDlg, IDC_GALLERY, BM_GETCHECK, 0, 0);
			if (chk != 0)
			{
				perm = MODE_GALLERY;
			}
			chk = (int)SendDlgItemMessage(hDlg, IDC_CUTOUT, BM_GETCHECK, 0, 0);
			if (chk != 0)
			{
				perm = MODE_CUTOUT;
			}
			wchar_t buf[100] = { 0 };
			::GetDlgItemText(hDlg, IDC_EDIT_YEAR, buf, 100);
			unsigned char year = _wtoi(buf);
			
			unsigned short st = 2;
			if(year == 1){
				st = 2;
			}
			else if(year == 2){
				st = 100;
			}
			else
			{
				st = 200;
			}
			_strLic = "";
			time_t current_time;
			time(&current_time);


			for (unsigned short i = st; i < 1000; i++)
			{
				time_t t = current_time - i * 100;
				string sLic = "";
				Grn(20, i, perm, year, t, sLic);

				_strLic += sLic;
				_strLic += "\r\n";
			}
			SetDlgItemTextA(hDlg, IDC_EDIT_LICENSE, _strLic.c_str());

			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_BUTTONCHECK)
		{
			char buf2[1000] = { 0 };
			GetDlgItemTextA(hDlg, IDC_EDIT_LICENSE, buf2, 1000);
			_strLic = buf2;

			if (_strLic != "")
			{
				string str = _strLic.substr(0, 32);

				unsigned char hexsn[1024] = { 0 };
				hexchange(hexsn, str.c_str());

				_BF.Decrypt(hexsn, sizeof(lic_header));

				unsigned short sum = checksum((USHORT*)hexsn, sizeof(lic_header));
				if (sum == 0)
				{
					lic_header* p = (lic_header*)hexsn;
					if (p->version >= 20 && p->num > 0 && p->num < 99999)
					{
						time_t t0 = p->date;

						struct tm t;
						localtime_s(&t, &t0);

						char stamped[MAX_PATH] = { 0 };
						strftime(stamped, MAX_PATH, "%Y/%m/%d", &t);


						char buf[100] = { 0 };
						sprintf_s(buf, 100, "mode:%d, num:%d, year:%d, ov:%d, date:%s", (int)p->mode, p->num, p->year, p->ov, stamped);
						MessageBoxA(hDlg, buf, "genlic", MB_OK);
					}
				}
			}
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	char buf[10] = { 0 };
	sprintf_s(buf, 10, "%d%d", 2022, 3567);
	_BF.SetPassword(buf);

	int d = sizeof(unsigned int);
	int d1 = sizeof(unsigned long);

	DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_IMGEN_DIALOG), 0, (DLGPROC)About, (LPARAM)0);

	return 0;
}