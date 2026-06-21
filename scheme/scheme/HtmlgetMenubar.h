#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

class CHtmlgetMenubar : public CImBase, public Singleton<CHtmlgetMenubar>
{
	friend class Singleton<CHtmlgetMenubar>;
protected:
	CHtmlgetMenubar(void);
	~CHtmlgetMenubar(void);

public:
	long clear();
	long get_categories_order_by_rank(int ty, list<CATEGORY_DTO>& lstCategory);
	long get_categories_order_by_root_dir(int ty, list<CATEGORY_DTO>& lstCategory);

	long get_categories(string catids, list<CATEGORY_DTO>& lstCategory);

	long get_order_categories(string order, list<CATEGORY_DTO>& lstCategory);
	long get_sub_categories_siblings(string cat_id, list<CATEGORY_DTO> lstCat, list<CATEGORY_DTO>& lstSub);
	string get_menubar_categories(string cat_id, uint32_t nb_cat_images, list<CATEGORY_DTO>* lstCategroy);
	long proc_categories(list<CATEGORY_DTO>* lstCategroy);

private:
	string get_menubar_categories_li(string cat_id, CATEGORY_DTO categroyDto, list<CATEGORY_DTO> lstCat);
	string get_menubar_categories_li_content(CATEGORY_DTO categroyDto);
	string get_sel_categories(string cat_id, list<CATEGORY_DTO> lstCat);
	string get_sel_category_substring(string cat_id, list<CATEGORY_DTO> lstCat);


	list<CATEGORY_DTO> lstCategory_order_by_rank_;
	list<CATEGORY_DTO> lstCategory_order_by_root_dir_;
	CRITICAL_SECTION _cs;
};