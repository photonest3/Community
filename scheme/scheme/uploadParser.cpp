////////////////////////////////////////////////////////////////////
/// @file uploadParser.cpp
/// @brief 文件上传解析器实现文件
/// @details 此文件实现了 CUploadParser 类的所有方法，
///          包括文件上传解析、加密文件解密、
///          上传数据处理等功能。
///          使用 Blowfish 算法对加密的上传文件进行解密。
/// @note 支持加密和未加密的文件上传
/// @author PhotoNest Team
/// @date 2024
////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "uploadParser.h"

#include "unitil.h"
#include "stmdef.h"
#include "imlog.h"

CUploadParser::CUploadParser()
{
	_pBF = NULL;
	_enckey = "123456";

}

CUploadParser::~CUploadParser(void)
{
	if (_pBF != NULL)
	{
		delete _pBF;
		_pBF = NULL;
	}
}

void CUploadParser::SetKey(string enckey)
{
	_enckey = enckey;
	if (_pBF != NULL)
	{
		delete _pBF;
		_pBF = NULL;
	}
	_pBF = new CBlowfish();
	_pBF->SetPassword((char*)_enckey.c_str());
}

string CUploadParser::GetKey()
{
	return _enckey;
}

int CUploadParser::Decryptfile(wstring src, wstring Decryptname)
{
	if (_pBF == NULL)
	{
		return 1;
	}

	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFile(src.c_str(), &ffd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
	}
	else
	{
		return 1;
	}

	int ret = 0;
	HANDLE hWrite = CreateFile(Decryptname.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hWrite == INVALID_HANDLE_VALUE)
	{
		return 1;
	}

	HANDLE hFile = CreateFile(src.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hWrite);
		return 1;
	}

	// Retrive file size
	DWORD lengthActual = GetFileSize(hFile, NULL);

	int nTemp = 1024 * 1024;
	unsigned char * tempbuf = (unsigned char*)malloc(nTemp + 1);
	if (tempbuf != NULL)
	{
		DWORD npos = 0, dwRead = 0, dwWritten = 0;

		while (npos < lengthActual)
		{
			memset(tempbuf, 0, nTemp + 1);
			BOOL b = ReadFile(hFile, tempbuf, nTemp, &dwRead, 0);
			if (b)
			{
				if (npos == 0)
				{
					DE_HEADER *de_hdr = (DE_HEADER *)tempbuf;
					int offset = 8 + de_hdr->offset;

					if (de_hdr->crc != _pBF->_crc)
					{
						ret = 1;
						break;
					}

					_pBF->Decrypt((void*)(tempbuf + 8), dwRead - 8);
					WriteFile(hWrite, tempbuf + offset, dwRead - offset, &dwWritten, NULL);
				}
				else
				{
					_pBF->Decrypt((void*)tempbuf, dwRead);
					WriteFile(hWrite, tempbuf, dwRead, &dwWritten, NULL);
				}
				npos += dwRead;
			}
			else
			{
				break;
			}
		}
		free(tempbuf);
	}

	::SetFileTime(hWrite, &ffd.ftCreationTime, &ffd.ftLastAccessTime, &ffd.ftLastWriteTime);
	CloseHandle(hWrite);
	CloseHandle(hFile);

	return ret;
}

int CUploadParser::Encryptfile(wstring src, wstring Encryptname)
{
	if (_pBF == NULL)
	{
		return 1;
	}
	HANDLE hWrite = CreateFile(Encryptname.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hWrite == INVALID_HANDLE_VALUE)
	{
		return 1;
	}

	HANDLE hFile = CreateFile(src.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hWrite);
		return 1;
	}

	// Retrive file size
	DWORD lengthActual = GetFileSize(hFile, NULL);

	unsigned char buf_hdr[100] = { 0 };

	DE_HEADER *de_hdr = (DE_HEADER *)buf_hdr;
	de_hdr->type = 1;

	div_t div_result = div(lengthActual, 8);
	if (div_result.rem != 0)
	{
		de_hdr->offset = 8 - div_result.rem;
	}

	de_hdr->crc = _pBF->_crc;

	srand((unsigned int)time(NULL));
	de_hdr->ot1 = rand() * 255;
	de_hdr->ot2 = rand() * 255;
	de_hdr->ot3 = 2024;

	DWORD dwWritten = 0;
	WriteFile(hWrite, buf_hdr, sizeof(DE_HEADER), &dwWritten, NULL);

	DWORD npos = 0, dwRead = 0;

	int nTemp = 1024 * 1024;
	unsigned char * tempbuf = (unsigned char*)malloc(nTemp + 1);
	if (tempbuf != NULL)
	{
		while (npos < lengthActual)
		{
			memset(tempbuf, 0, nTemp + 1);

			if (npos == 0 && div_result.rem != 0)
			{
				BOOL b = ReadFile(hFile, tempbuf + de_hdr->offset, div_result.rem, &dwRead, 0);
				if (b)
				{
					_pBF->Encrypt((void*)tempbuf, 8);
					WriteFile(hWrite, tempbuf, 8, &dwWritten, NULL);
				}
				else
				{
					break;
				}
			}
			else
			{
				BOOL b = ReadFile(hFile, tempbuf, nTemp, &dwRead, 0);
				if (b)
				{
					_pBF->Encrypt((void*)tempbuf, dwRead);
					WriteFile(hWrite, tempbuf, dwRead, &dwWritten, NULL);
				}
				else
				{
					break;
				}

			}

			npos += dwRead;
		}

		free(tempbuf);
	}

	FILETIME CreationTime, LastAccessTime, LastWriteTime;
	GetFileTime(hFile, &CreationTime, &LastAccessTime, &LastWriteTime);
	SetFileTime(hWrite, &CreationTime, &LastAccessTime, &LastWriteTime);

	CloseHandle(hWrite);
	CloseHandle(hFile);
	return 0;

}

