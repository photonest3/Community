#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

class CHtmlSearchProc : public CImBase, public Singleton<CHtmlSearchProc>
{
	friend class Singleton<CHtmlSearchProc>;
protected:
	CHtmlSearchProc(void);
	~CHtmlSearchProc(void);

public:
	long proc_search_htm(list<CATEGORY_DTO> lst, const QUERY_REQ req, string num_tags, string &menubar, string &tpl);
	long get_search_req(REQUEST_DTO request_dto, SEARCH_REQ &search_req);
	long proc_slider_req_htm(SEARCH_REQ req, string& szResponse);
	long get_search_form(list<CATEGORY_DTO> lst, string& search_form, const QUERY_REQ req);

private:

	long get_slider_josn(SLIDER_VALUES sliderValues, Writer<StringBuffer>* writer, string type, string width_min, string width_max, string slider_item);

};