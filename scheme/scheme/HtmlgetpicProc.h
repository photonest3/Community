#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

class CHtmlgetpicProc : public CImBase, public Singleton<CHtmlgetpicProc>
{
	friend class Singleton<CHtmlgetpicProc>;
protected:
	CHtmlgetpicProc(void);
	~CHtmlgetpicProc(void);
public:
	string get_meta_exif(string url);
	long proc_boxshow_htm(string osm, uint32_t from, uint32_t to, string& tpl);
	long proc_slide_htm(list<string> plstImageIds, uint32_t from, uint32_t to, string& slider_nav, string& picture_content);
	long get_details(string type, const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long get_src(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long get_speed(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long get_all_exif(IMAGE_DTO curr, string cat_name, string& metadata, string& all);
};