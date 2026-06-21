#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

class CHtmlgetIndexAction : public CImBase, public Singleton<CHtmlgetIndexAction>
{
	friend class Singleton<CHtmlgetIndexAction>;
protected:
	CHtmlgetIndexAction(void);
	~CHtmlgetIndexAction(void);

public:
	long set_display(string display);
	string get_categoryActions(int ty, list<CATEGORY_DTO> lst, uint32_t nb_images, const QUERY_REQ req, string& switchBoxSizeUrl, string& switchBoxSortUrl);
	string get_sortOrderBox(list<CATEGORY_DTO> lst, QUERY_REQ req, string args);
	string get_thumbnailCategories(string type, string cat_id, list<CATEGORY_DTO> lstCat);
	string get_thumbnails(QUERY_REQ req, list<IMAGE_DTO> lstImageDto, ORDER_DTO orderDto, list<CATEGORY_DTO> lst);

	uint32_t get_maxHeight();
	uint32_t get_maxWidth();
	string get_cation_url(const QUERY_REQ req);

private:
	string get_derivativeSwitchBox(string  display);
	long get_fullname(string full_id, list<CATEGORY_DTO> lstCat, string& fullname);
	uint32_t _maxWidth = 144;
	uint32_t _maxHeight = 144;
};