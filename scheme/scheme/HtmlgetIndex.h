#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

class CHtmlgetIndex : public CImBase, public Singleton<CHtmlgetIndex>
{
	friend class Singleton<CHtmlgetIndex>;
protected:
	CHtmlgetIndex(void);
	~CHtmlgetIndex(void);

public:
	long get_index_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long get_b_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long get_register_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long get_login_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long get_page_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long get_skins_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long get_js(UINT id, wstring js, RESPONSE_DTO& response_dto);
	long get_css(UINT id, wstring css, RESPONSE_DTO& response_dto);
	long get_last_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);

	void proc_first_index(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long proc_delbox(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);

private:
	long get_index_template(list<CATEGORY_DTO> lst, const QUERY_REQ req, string num_tags, string& menubar, string& tpl);

	BOOL CreateMyDirectory(wstring path);
	long get_dateType(QUERY_REQ req, ORDER_DTO& orderDto);

	static UINT __stdcall sb_nb(LPVOID pParam);

private:
	list<CATEGORY_DTO> _lstCategory;
};