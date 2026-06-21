#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

class CHtmlgetViewer : public CImBase, public Singleton<CHtmlgetViewer>
{
	friend class Singleton<CHtmlgetViewer>;
protected:
	CHtmlgetViewer(void);
	~CHtmlgetViewer(void);

public:
	long get_viewer_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long get_delbox(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long del_viewer(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long get_thumb_file(REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);

private:
	long set_viewer_data(string items, string& history_file, string& tpl);
	long get_viewer_items(wstring path0, string& items, int& idx);
	void splitFile(wstring file_name, wstring& szDir, wstring& szName, wstring& szExt);

private:
	list<WIN32_FIND_DATA > _lstffd;
	wstring _strPath = L"";
	wstring _dstP = L"";

};