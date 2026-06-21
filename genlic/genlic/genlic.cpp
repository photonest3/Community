// ============================================================
// genlic.cpp : 授权码生成与校验工具
// 功能：通过 Blowfish 加密生成软件授权许可证，并提供图形对话框进行生成与验证
// ============================================================

#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "resource.h"   // 资源定义（对话框、控件 ID 等）
#include <CommCtrl.h>   // 通用控件支持
#include <string>
#include "Blowfish.h"   // Blowfish 加密算法实现

using namespace std;

// ------------------------------------------------------------
// 授权文件头结构：存储授权的基本元信息
// version : 版本号
// mode    : 授权模式（画廊/抠图）
// num     : 序列号
// chksum  : 校验和（用于验证数据完整性）
// ov      : 覆盖标记
// year    : 授权年限
// date    : 生成时间戳
// ------------------------------------------------------------
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

#define MODE_GALLERY 10   // 画廊模式授权
// #define MODE_ENCRYPTED 20 // 已废弃的加密模式
#define MODE_CUTOUT  30   // 抠图模式授权

// 使用 Windows 子系统入口，避免弹出控制台窗口
#pragma comment(linker, "/subsystem:windows /ENTRY:wmainCRTStartup")

CBlowfish _BF;       // Blowfish 加密实例，用于授权码加解密
string _strLic = ""; // 当前生成的授权码字符串（对话框中显示）

// ------------------------------------------------------------
// 将十六进制字符串转换为字节数组（查表法加速）
// dest : 输出字节数组缓冲区
// src  : 输入十六进制字符串（如 "1A2B3C"）
// 返回值 : 固定返回 0
// ------------------------------------------------------------
int hexchange(unsigned char* dest, const char* src)
{
	int i;
	char buf[256 * 2 + 1];

	// 申请 64KB 查找表，用于快速将 2 字符十六进制映射为 1 字节
	unsigned char* table = (unsigned char*)malloc(65536);
	if (table != NULL)
	{
		memset(table, 0, 65536);
		unsigned short* shortSrc = (unsigned short*)buf;

		// 预生成 00~FF 的十六进制字符串表示
		for (i = 0; i <= 255; i++)
		{
			sprintf_s(buf + i * 2, 3, "%02X", i);
		}

		// 建立十六进制字符串到数值的映射表
		for (i = 0; i <= 255; i++)
		{
			table[shortSrc[i]] = i;
		}

		// 逐 2 字符转换源字符串到目标字节数组
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

// ------------------------------------------------------------
// 计算缓冲区校验和（类似 IP 首部校验和算法）
// buffer : 待校验数据缓冲区（按 USHORT 对齐）
// size   : 数据长度（字节）
// 返回值 : 16 位校验和结果
// ------------------------------------------------------------
unsigned short checksum(USHORT* buffer, int size)
{
	unsigned long cksum = 0;
	// 按字（2字节）累加
	while (size > 1)
	{
		cksum += *buffer++;
		size -= sizeof(USHORT);
	}
	// 若长度为奇数，追加剩余字节
	if (size != 0)
	{
		cksum += *(UCHAR*)buffer;
	}

	// 将高 16 位进位加到低 16 位
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	return (USHORT)(~cksum); // 取反返回
}

// ------------------------------------------------------------
// 生成 GUID 字符串
// szguid : 输出 GUID 字符串（如 "A1B2C3D4-E5F6..." 格式去掉横线）
// 返回值 : 0 表示成功，非零为 COM 错误码
// ------------------------------------------------------------
long get_guid(string& szguid)
{
	GUID guid;
	char buf[100], tmp[20];

	// 调用 Windows COM API 创建全局唯一标识符
	HRESULT hr = CoCreateGuid(&guid);
	if (hr != S_OK)
	{
		return hr;
	}

	// 格式化 GUID 前 5 个字段
	sprintf_s(buf, "%08X%04X%04X%02X%02X",
		guid.Data1, guid.Data2,
		guid.Data3, guid.Data4[0], guid.Data4[1]);

	// 追加剩余 6 个字节的十六进制表示
	for (int i = 2; i < 8; i++)
	{
		sprintf_s(tmp, "%02X", guid.Data4[i]);
		strcat_s(buf, tmp);
	}

	szguid = buf;

	return 0;
}

// ------------------------------------------------------------
// 生成单条授权码
// ver          : 授权版本号
// num          : 序列号
// perm         : 授权模式（MODE_GALLERY / MODE_CUTOUT）
// year         : 授权年限
// current_time : 授权生成时间戳
// sLic         : 输出授权码字符串（十六进制）
// 返回值       : 固定返回 TRUE
// ------------------------------------------------------------
BOOL Grn(unsigned char ver, unsigned short num, unsigned char perm, unsigned char year, time_t current_time, string& sLic)
{
	lic_header lic;
	memset(&lic, 0, sizeof(lic_header));

	// 填充授权头信息
	lic.version = ver;
	lic.mode = perm;
	lic.num = num;
	lic.chksum = 0;   // 先置 0，后续计算
	lic.ov = 0;
	lic.year = year;
	lic.date = current_time;
	// string szguid;
	// get_guid(szguid);
	 // strncpy_s(lic.guid, szguid.c_str(), 7);

	// 计算并填充校验和
	unsigned short sum = checksum((USHORT*)(&lic), sizeof(lic_header));
	lic.chksum = sum;

	unsigned char* ptr = (unsigned char*)&lic;

	// 使用 Blowfish 加密授权头
	_BF.Encrypt(ptr, sizeof(lic_header));

	// 将加密后的字节数组转换为十六进制字符串
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


// ------------------------------------------------------------
// 授权生成对话框消息处理回调函数
// hDlg    : 对话框窗口句柄
// message : Windows 消息类型
// wParam / lParam : 消息参数
// 返回值  : TRUE 表示已处理该消息，FALSE 表示未处理
// ------------------------------------------------------------
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG: // 对话框初始化：设置默认选中项和默认值
	{
		SendDlgItemMessage(hDlg, IDC_GALLERY, BM_SETCHECK, BST_CHECKED, 0);
		SetDlgItemText(hDlg, IDC_EDIT_LICENSE, L"");
		SetDlgItemText(hDlg, IDC_EDIT_YEAR, L"1");
	}
	return (INT_PTR)TRUE;

	case WM_CLOSE: // 关闭对话框
	{
		EndDialog(hDlg, LOWORD(wParam));
	}
	return (INT_PTR)TRUE;

	case WM_COMMAND: // 处理按钮点击等命令消息
		if (LOWORD(wParam) == IDC_BUTTON_GRN) // 【生成授权码】按钮
		{
			unsigned int perm = MODE_GALLERY;
			// 根据单选按钮确定授权模式
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

			// 读取授权年限
			wchar_t buf[100] = { 0 };
			::GetDlgItemText(hDlg, IDC_EDIT_YEAR, buf, 100);
			unsigned char year = _wtoi(buf);

			// 根据年限决定起始序列号
			unsigned short st = 2;
			if (year == 1) {
				st = 2;
			}
			else if (year == 2) {
				st = 100;
			}
			else {
				st = 200;
			}

			_strLic = "";
			time_t current_time;
			time(&current_time);

			// 批量生成授权码：按不同时间偏移生成多条授权
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
		else if (LOWORD(wParam) == IDC_BUTTONCHECK) // 【校验授权码】按钮
		{
			char buf2[1000] = { 0 };
			GetDlgItemTextA(hDlg, IDC_EDIT_LICENSE, buf2, 1000);
			_strLic = buf2;

			if (_strLic != "")
			{
				// 取第一行授权码进行校验
				string str = _strLic.substr(0, 32);

				unsigned char hexsn[1024] = { 0 };
				hexchange(hexsn, str.c_str());

				// Blowfish 解密
				_BF.Decrypt(hexsn, sizeof(lic_header));

				// 验证校验和
				unsigned short sum = checksum((USHORT*)hexsn, sizeof(lic_header));
				if (sum == 0)
				{
					lic_header* p = (lic_header*)hexsn;
					// 校验版本号和序列号范围
					if (p->version >= 20 && p->num > 0 && p->num < 99999)
					{
						time_t t0 = p->date;

						struct tm t;
						localtime_s(&t, &t0);

						char stamped[MAX_PATH] = { 0 };
						strftime(stamped, MAX_PATH, "%Y/%m/%d", &t);

						// 显示解密后的授权信息
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

// ------------------------------------------------------------
// 程序入口：初始化 Blowfish 密码并弹出授权生成对话框
// argc / argv : 命令行参数（本程序未使用）
// 返回值 : 固定返回 0
// ------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	// 设置 Blowfish 加密密钥（硬编码密码：20223567）
	char buf[10] = { 0 };
	sprintf_s(buf, 10, "%d%d", 2022, 3567);
	_BF.SetPassword(buf);

	int d = sizeof(unsigned int);
	int d1 = sizeof(unsigned long);

	// 弹出模态对话框，由 About 函数处理消息
	DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_IMGEN_DIALOG), 0, (DLGPROC)About, (LPARAM)0);

	return 0;
}