#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

class CHtmlgetBatch : public CImBase, public Singleton<CHtmlgetBatch>
{
	friend class Singleton<CHtmlgetBatch>;
protected:
	CHtmlgetBatch(void);
	~CHtmlgetBatch(void);

public:
	long proc_batch_htm(REQUEST_DTO request_dto, string batch_infos, string& tpl);

private:
	string get_tags_string(string image_id, list<IMAGE_TAG_DTO> lstImageTag);
	string get_navigationBar_global(string ty, int per, string navigationBar, string& navigationBar_global);
	long proc_filter_req_htm(list<CATEGORY_DTO> lst, string filter, string& tpl);

public:
	string _szArgs = "";
	string _picture_data = "";
	string _element_ids = "";

	SEARCH_REQ _f_req;

};
