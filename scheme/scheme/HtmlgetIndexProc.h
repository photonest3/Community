#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

class CHtmlgetIndexProc : public CImBase, public Singleton<CHtmlgetIndexProc>
{
	friend class Singleton<CHtmlgetIndexProc>;
protected:
	CHtmlgetIndexProc(void);
	~CHtmlgetIndexProc(void);

public:
	long clear_data();
	long proc_art_edit(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long proc_art_film(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long proc_art_zoom(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long update_art_title(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	string proc_art_list(string szArgs);

	long proc_profile_htm(REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	string proc_tags_all(string szArgs, list<TAG_DTO>* lstTag);

	long proc_tumb_htm(list<CATEGORY_DTO> lst, QUERY_REQ req, ORDER_DTO orderDto, uint32_t& nb_images, string& tpl);
	long update_cache_image(string id, IMAGE_DTO imageDto);
	long push_cache_images(list<IMAGE_DTO> lstImage);

	long get_max_page(QUERY_REQ req, uint32_t& maxPage);
	long get_init_page(QUERY_REQ req, list<IMAGE_DTO>& lstDto);
	long get_nav_data(string id, string& data);
	long post_img_say(REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long show_paint(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	long get_action(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	uint64_t get_md5sum(string artids, list<IMAGE_DTO>& lstOrder);
	long get_cache(int ty, list<TEMPL_DTO>& lstDto);
	long update_cache(list<TEMPL_DTO> lstQuality);

private:
	string get_banner();

	string get_title(const QUERY_REQ req);
	long get_size(IMAGE_DTO dto, uint32_t& w, uint32_t& h);

	void mk_path(string& title1);

private:
	list<IMAGE_DTO> _lstImageDto;
	CRITICAL_SECTION _cs;


};