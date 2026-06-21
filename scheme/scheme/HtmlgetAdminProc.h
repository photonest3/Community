#pragma once
#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

class CHtmlgetAdminProc : public CImBase, public Singleton<CHtmlgetAdminProc>
{
	friend class Singleton<CHtmlgetAdminProc>;
protected:
	CHtmlgetAdminProc(void);
	~CHtmlgetAdminProc(void);

public:
	long proc_cat_list_htm(const REQUEST_DTO request_dto, string cat_list_infos, string& tpl);
	long proc_head_htm(string page, uint32_t &nbCaddie, uint32_t &nbDuplicates, uint32_t &nbOrphan, string& tpl);

	long proc_tag_htm(const REQUEST_DTO request_dto, string& tpl);
	long proc_photo_htm(string page, const REQUEST_DTO request_dto, string photo_infos, string& tpl);
	long proc_stats_htm(const REQUEST_DTO request_dto, string photo_infos, string& tpl);

	long proc_properties_htm(string page, const REQUEST_DTO request_dto, string cat_infos, string& tpl);
	long proc_cat_name_htm(const REQUEST_DTO request_dto, string cat_infos, string& tpl);
	long move_categories(string cat_id, string parent);
	long proc_config_htm(string page, const REQUEST_DTO request_dto, string config_infos, string& tpl);

	string get_ago_string(uint64_t date_available);
	long get_categoryUl(list<CATEGORY_DTO> lst, string parent_id, string& categoryUl);
	long get_category_title(list<CATEGORY_DTO> lst, string type, string parent_id, string cat_id, string& titlename);
	long get_category_options(string ty, list<CATEGORY_DTO> lst, string parent_id, string cat_id, string& options);

private:
	string get_image_order_htm(string image_order_tpl, string image_order);
	string get_sel_opt(string orderby, string val);
	long get_stats_title(string year, string month, string day, string& title);

	string get_day_name(int i, int weekstart);
	string get_month_name(int i);
	string get_width_px(uint32_t nb_pages, uint32_t nb_max);
};
