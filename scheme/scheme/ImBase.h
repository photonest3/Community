#pragma once

#include "stmdef.h"
#include "Singleton.h"


class CImBase
{
public:
	CImBase(void);
	~CImBase(void);

protected:
	string get_title(list<CATEGORY_DTO> lst, string szUrl, const QUERY_REQ req, string picid, uint32_t nb_images);
	long get_req(int type, const REQUEST_DTO request_dto, QUERY_REQ& req);
	long set_req(string posted, QUERY_REQ& req);

	int getdays(int year, int month);
	string get_picurl(const QUERY_REQ req);
	string get_up_url(const QUERY_REQ req);

	long get_imgsize(IMAGE_DTO dto, uint32_t maxWidth, uint32_t maxHeight, uint32_t& outW, uint32_t& outH);
	long get_flex_size(IMAGE_DTO dto, uint32_t maxWidth, uint32_t maxHeight, uint32_t& outW, uint32_t& outH);
	string get_time_string(uint64_t tt, string fmt);


	string get_navigationBar(string url, int total, int curr, int per, int css = 0);
	string get_photo_string(uint32_t nb);

	uint32_t murmur3_32(const uint8_t* key, int len, uint32_t seed);
	string get_dur_string(uint32_t duration);
	string get_sLongDate();
	string get_sYearMonth();
	BOOL my_DeleteFile(wstring dst, int ty = 0);

	long esc_string(string& name);
	bool translate(string& strText, string intVal, string szVal);
	string get_angle(uint32_t rotation);

	string ori2path(list<ROOT_DTO> lstRoot, int type, string ori);

	long Htmlread(UINT cr_id, wstring filename, string& szhtml);
};
