///////////////////////////////////////////////////////////////////////
/// @file locales.cpp
/// @brief PhotoNest 多语言资源 DLL 实现
/// @details 实现从资源中读取 XML 格式的语言文件,
///          支持多语言界面显示
/// @author PhotoNest Team
/// @date 2024
/// @version 1.0
/// @note 本文件是 locales DLL 的实现文件
///////////////////////////////////////////////////////////////////////

#include "framework.h"
#include "locales.h"

HINSTANCE _hrcInstLoc = NULL;

LOCALES_API long Read_xml(unsigned int cr_id, const wchar_t* filename, char** szhtml)
{
	long ret = -1;

	if (cr_id == 0)
	{
		wchar_t buf[MAX_PATH] = { 0 };
		long lSize = ::GetModuleFileName(NULL, buf, MAX_PATH);

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

		wcscat_s(buf, L"\\");
		wcscat_s(buf, filename);

		HANDLE hFile = CreateFile(buf, GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
			NULL);

		if (hFile != INVALID_HANDLE_VALUE)
		{
			// Retrive file size
			int lengthActual = GetFileSize(hFile, NULL);
			DWORD dwNumBytesRead = 0;
			size_t v = (size_t)lengthActual + 1;

			*szhtml = (char*)LocalAlloc(LMEM_ZEROINIT, v);
			if (*szhtml != NULL)
			{
				BOOL bResult = ReadFile(hFile, *szhtml, lengthActual, &dwNumBytesRead, 0);
				if (bResult)
				{
					ret = 0;
				}
				else
				{
					LocalFree(*szhtml);
				}
			}

			CloseHandle(hFile);
		}
	}
	else
	{
		//if (_hrcInstLoc == NULL)
		//{
		//	_hrcInstLoc = GetModuleHandle(L"locales.dll");
		//}

		HRSRC hrs = FindResource(_hrcInstLoc, MAKEINTRESOURCE(cr_id), RT_HTML);
		if (hrs != NULL)
		{
			DWORD dw = SizeofResource(_hrcInstLoc, hrs);
			DWORD len = dw + 1;
			*szhtml = (char*)LocalAlloc(LMEM_ZEROINIT, len);
			if (*szhtml != NULL)
			{
				HGLOBAL hg = LoadResource(_hrcInstLoc, hrs);
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
	}

	return ret;
}
