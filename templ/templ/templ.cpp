
#include "framework.h"
#include "templ.h"

HINSTANCE _hrcInst = NULL;

TEMPL_API long Read_htm(unsigned int cr_id, const wchar_t* filename, char** szhtml)
{
	long ret = -1;
	/*
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
		*/
	//{
		//if (_hrcInst == NULL)
		//{
		//	_hrcInst = GetModuleHandle(L"templ.dll");
		//}

		HRSRC hrs = FindResource(_hrcInst, MAKEINTRESOURCE(cr_id), RT_HTML);
		if (hrs != NULL)
		{
			DWORD dw = SizeofResource(_hrcInst, hrs);
			DWORD len = dw + 1;
			*szhtml = (char*)LocalAlloc(LMEM_ZEROINIT, len);
			if (*szhtml != NULL)
			{
				HGLOBAL hg = LoadResource(_hrcInst, hrs);
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
	//}

	return ret;
}
