////////////////////////////////////////////////////////////////////
/// @file imlog.cpp
/// @brief 日志记录实现文件
/// @details 此文件实现了 Cimlog 类的所有方法，
///          包括日志文件创建、日志写入、日志级别控制等功能。
///          支持按日期和大小自动轮转日志文件，
///          并提供不同级别的日志记录（错误、警告、信息）。
/// @note 使用 Windows API 进行文件操作
/// @author PhotoNest Team
/// @date 2024
////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "imlog.h"
#include "unitil.h"
#include <time.h>

Cimlog::Cimlog()
{
}

Cimlog::~Cimlog()
{
}

void Cimlog::setLogName(wstring szLog)
{
	_szFilename = szLog;
}

void Cimlog::LogToFile(string text, int lv)
{
	if (lv == 0)
	{
		return;
	}

	SYSTEMTIME st;
	GetLocalTime(&st);

	int len = (int)text.size() + 1024;

	char *ptr = (char *)malloc(len);
	if (ptr == NULL)
	{
		return;
	}

	memset(ptr, 0, len);
	sprintf_s(ptr, len, "%u/%u/%04u %02u:%02u:%02u %s\r\n", st.wMonth, st.wDay, st.wYear, st.wHour, st.wMinute, st.wSecond, text.c_str());

	string temp = ptr;
	free(ptr);

	HANDLE hFile = CreateFile(_szFilename.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		int lengthActual = GetFileSize(hFile, NULL);
		if (lengthActual > 1024 * 1000 * 20)
		{
			SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
			SetEndOfFile(hFile);
		}
		else
		{
			SetFilePointer(hFile, 0, NULL, FILE_END);
		}

		DWORD dwWritten = 0;
		WriteFile(hFile, temp.c_str(), (DWORD)temp.length(), &dwWritten, NULL);

		CloseHandle(hFile);

	}
}

