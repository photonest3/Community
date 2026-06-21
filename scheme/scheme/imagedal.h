#pragma once

#include "sqlite3.h"
#include "stmdef.h"
#include "Singleton.h"

using namespace std;

class Cimagedal : public Singleton<Cimagedal>
{
	friend class Singleton<Cimagedal>;
protected:
	Cimagedal(void);
	~Cimagedal(void);

	// TODO: add your methods here.
public:
	void setDb(sqlite3* pDB);
	long refresh_orderby(string& orderby);

	long exist_image(string cat_id, string file, uint64_t md5sum, string& existId);
	long get_imgerr(string dir, list<MD5_DTO>& lstmd5);

public:

	long _add_imgerr(string cat_id, const MD5_DTO dto, uint32_t& id);
	long _add_image(string cat_id, const IMAGE_DTO imageDto, long time_zone_bias, uint32_t& id);
	long _update_unit_images(list<IMAGE_DTO> lstImage, long time_zone_bias);
	long _update_quality(list<TEMPL_DTO> lstImage);
	long _update_coi(IMAGE_DTO image_dto);

	long _add_face(string ids, list<FACE_DTO> lstface, list<FACE_DTO>& lstout);

	long _update_images(string action, string selection, string val);
	long _del_image(string id);
	long _update_images_hit(string image_id);
	long _add_image_tags(string element_ids, list<IMAGE_TAG_DTO> lstImageTag);
	long _update_images_rank(list<IMAGE_CATEGORY_DTO>& lstRank);

	long _update_images_byzone(long time_zone_bias);
	long _save_photo(PHOTO_DTO photo_dto, long time_zone_bias);
	long _delete_orphans();
	long _update_image_category(string image_id, string op, string name);
	long _update_image_tag(string image_id, string op, string name);

	void _set_representative_picture_id(string image_id);
	long _set_categories_visible(string& changed);

public:
	long get_image_authors(map<string, uint32_t >& mapAuthor);
	long get_face(string ids, string notids, list<FACE_DTO>& lstface);

	long get_image_tags(string image_ids, list<IMAGE_TAG_DTO>& lstImageTag);
	long get_image_tags_count(string image_ids, uint32_t& nb);
	long get_menubar_tags(string image_ids, string cat_ids, list<IMAGE_TAG_DTO>& lstImageTag);
	long get_image_properties(string image_id, string& cats, string& tags);



	long get_image_ids(QUERY_REQ req, SEARCH_REQ search_req, list<string>& lstImageIds);
	long get_filter_ids(SEARCH_REQ req);
	long get_search_where(SEARCH_REQ req, string& sql);

	long get_category_images(string cat_id, uint64_t& min, uint64_t& max, uint32_t& nb_images);
	long get_duplicates(list<string>& lstImageIds);

	long get_image_ids(QUERY_REQ req, list<string>& lstImageIds);


	long get_images_slider_values(SLIDER_VALUES& sliderValues);

	long get_image_gategories(string image_ids, list<string>& lstcategoryid);

	long get_photo(string id, PHOTO_DTO& photo_dto);
	long get_image_cal_sql(QUERY_REQ req, SEARCH_REQ search_req, string& outsql);
	long get_image_cal(string sql, map<uint32_t, uint32_t >& mapCal);
	string get_search_req_sql(QUERY_REQ req, SEARCH_REQ search_req, int type);
	long get_files(string pic_id, string cat_id, list<string>& lstFiles, list<string>& lstCoi);

	long get_orphan_nb_photos(uint32_t& nb);

	long get_rand_images(string cat_id, string curr_picid, IMAGE_DTO& dto);
	long full_image_dto(char** dbResult, int nColumn, int idx, IMAGE_DTO& dto);
	long get_cal_images(QUERY_REQ req, SEARCH_REQ search_req, list<DAY_IMAGES_DTO>& lstDay);
	long get_weekly_images(QUERY_REQ req, SEARCH_REQ search_req, list<WEEK_IMAGES_DTO>& lstDay);

	long get_images_quality(string imageids, list<TEMPL_DTO>& lstImage);
	long get_images_by_ids(string imageids, list<IMAGE_DTO>& lstImage);
	long get_images_group(string imageids, map<string, uint32_t>& mapCat);
	long get_images_face(list<string>& lstId);

	long get_images_by_rnd(list<CATEGORY_DTO> lstSub, list<IMAGE_DTO>& lstImage);
	long get_images_all_ids(string imageids, list<string>* plstImageIds, list<IMAGE_DTO>& lstImage);
	long get_images_by_cats(string catid, int page, list<IMAGE_DTO>& lstImage);
	long get_images_all_map(list<string>& lstImageIds);
	long get_recent_cats(string& ids);
	long get_tags(string tag_id, list<TAG_DTO>& lstTag);

public:
	long get_nb_cat_images(uint32_t& nb_cat_images);
	string get_sqlite_error_string(int err);
	long set_alb(string alb);

private:
	string get_query_req_sql(QUERY_REQ req, int type);
	string get_image_order(string image_order);
	uint64_t get_time_from_string(string datestr);
	long full_lstImage(char** dbResult, int nRow, int nColumn, list<IMAGE_DTO>& lstImage);

	long print_sql_msg(char* func, char* zSQL, char* errmsg);
	long print_sql2_msg(char* func, string sql, char* errmsg);

	string get_query_req_sql_cal(QUERY_REQ req);

	long full_lstImage2(string imageids, char** dbResult, int nRow, int nColumn, list<IMAGE_DTO>& lstImage);
	long full_lstImage3(string imageids, list<string>* plstImageIds, char** dbResult, int nRow, int nColumn, list<IMAGE_DTO>& lstImage);
	string get_bias(long time_zone_bias);
	long get_last_import_time(uint64_t& t);

	string _orderby = "";
	sqlite3* _pDB;
	string _visible0_ = " where 1=1 ";
	string _v0 = "";
	string _v1 = "";
	string _modulePath = "";

	string _image_ids_sql = "";
	string _filter_sql = "";
};
