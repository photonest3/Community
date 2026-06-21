#pragma once

#include "ImBase.h"
#include "stmdef.h"
#include "Singleton.h"

class CHtmlupload : public CImBase, public Singleton<CHtmlupload>
{
	friend class Singleton<CHtmlupload>;
protected:
	CHtmlupload(void);
	~CHtmlupload(void);

public:
	long post_upload_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long set_roots();

private:
	void mk_dir(wstring path, wstring src);
	BOOL CreateDirectoryFile(string cat_id, string type, wstring szFile, wstring& fullname, wstring& toname, wstring& upload_url);
	void ProcThumbnailFunc(UPLOAD_DATA data, IMAGE_DTO& imageDto);
	void splitFile(wstring file_name, wstring& szDir, wstring& szName, wstring& szExt);
	BOOL CreateMyDirectory(wstring path);
	long get_cover(wstring szFile, wstring szName, wstring fullname, wstring upload_url, string& url, IMAGE_DTO& imageDto);
	long Encrypt_szFile(wstring szFile, wstring krf);
	long set_category(string cat_id);
	uint64_t convertDateTimeOriginalToUint64(const std::string& dateTimeOriginal);

private:
	CRITICAL_SECTION _cs;
	list<string> _fileSet;
	list<CATEGORY_DTO2> _lstCategory;
	list<ROOT_DTO2> _lstRoot2;
	list<wstring> _lstMydir;
string album_="public";
};
