#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "../../FreeImage/Source/FreeImage.h"
#include <mfapi.h>
#include <mfplay.h>
#include <mfreadwrite.h>
#include <mferror.h>
#include <wmcodecdsp.h>

typedef int(_cdecl *pfnGetStatus)(const wchar_t * szFile, const wchar_t * dst, int &width, int &height, unsigned char **pYUVBuffer);

class CImageProc : public Singleton<CImageProc>
{
	friend class Singleton<CImageProc>;
protected:
	CImageProc(void);
	~CImageProc(void);

public:
	BOOL jp_test(const wchar_t *lpszPathName, const wchar_t *lpszImagePathName, int scale_denom, long rotation);
	BOOL testSaveThumbnail(const wchar_t *lpszPathName, const wchar_t *lpszImagePathName, long rotation, float maxWidth, float maxHeight, int &ftype);
		long get_exif_value(wstring filename, string type, map<string, string> &attr);
	HRESULT save_mp4_thumbnail(UINT cx, wstring szFile, wstring dst);
	long get_Orientation_value(wstring filename);
	int proc_mp4_thumbnail(wstring szFile, wstring dst, long rotate);

private:
	unsigned char * read_jpeg_file(const wchar_t *filename, int scale_denom, unsigned int &outwidth, unsigned int &outheight);
	int write_jpeg_file(const wchar_t * filename, unsigned char* image_buffer, int quality, unsigned int image_width, unsigned int image_height);

	int xxxx(const wchar_t *lpszPathName, const wchar_t *lpszImagePathName, long rotation);
	HRESULT proc_bimap(HBITMAP hbmp, wstring dst);

	//unsigned long GetUniqueID();
	int _seq_id;
	//HRESULT GetShellThumbnailImage(LPCWSTR pszPath, HBITMAP* pThumbnail);

	int yuv420sp_to_jpg(const wchar_t *filename, int width, int height, unsigned char *pYUVBuffer);

private:
	HINSTANCE _mfthumb_dll;
	pfnGetStatus _func;

};
