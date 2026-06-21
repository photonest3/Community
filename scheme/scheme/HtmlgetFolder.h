#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"
#include <shlobj.h>
//#include <regex>
#include <queue>

#import "scrrun.dll" raw_interfaces_only \
 rename("CopyFile", "ExclCopyFile") \
 rename("DeleteFile", "ExclDeleteFile") \
 rename("MoveFile", "ExclMoveFile") 

class CHtmlgetFolder : public CImBase, public Singleton<CHtmlgetFolder>
{
	friend class Singleton<CHtmlgetFolder>;
protected:
	CHtmlgetFolder(void);
	~CHtmlgetFolder(void);

public:
	long GetSubFolders(int ty, wstring strPath, list<wstring >& lstSubs);
	long GetFolderPics(string cat_id, wstring strPath, uint8_t crc, list<FILE_DTO >& lstFile);
	long GetFolderPics(int ty, wstring strPath, list<WIN32_FIND_DATA >& vecffd);
	long GetFolderSize(list<ROOT_DTO> lstRoot, string album, uint64_t& fsize);

	long DelFolderFiles(wstring strPath);
	long GetFolderPics(wstring strPath, queue<wstring>& lstFile);
	long GetSubFolderNb(wstring dDir, map<wstring, uint32_t>& lstSubs);
	void CopyAppData(string alb, int msix, int& bAttr);

private:
	bool inline is_picture(wchar_t* cFileName, uint8_t crc);
	int zz(int ty, Scripting::IFolderPtr folder, list<wstring >& lstSubs);
	long zz0(wstring strPath, list<wstring>& lstFile, uint32_t& fsize);
	uint64_t variantSize(_variant_t vsize2);
	long get_file_crc(wchar_t* src, uint8_t& crc);
	int zz5(Scripting::IFolderPtr folder, map<wstring, uint32_t>& lstSubs);

	const wchar_t* _exts[12] = { L".jpg", L".jpeg", L".png", L".webp" ,L".webm", L".gif", L".bmp", L".dib", L".mp3", L".mp4", L".ogg", NULL };
	//regex _re;
	void sc(wstring path, wstring appDataDir, wstring subfolder);

	HRESULT CreateLink(LPCWSTR lpszPathObj, LPCWSTR lpszPathLink, LPCWSTR lpszDesc, LPCWSTR lpszArgs, LPCWSTR lpszIcon);
	wstring get_appdata_path2(string alb, int ty);
};