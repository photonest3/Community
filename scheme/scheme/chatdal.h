#pragma once

#include "sqlite3.h"
#include "stmdef.h"
#include "Singleton.h"

using namespace std;

class Cchatdal : public Singleton<Cchatdal>
{
	friend class Singleton<Cchatdal>;
protected:
	Cchatdal(void);
	~Cchatdal(void);

	// TODO: add your methods here.
public:
	void setDb(sqlite3* pDB);
	long get_roots(string image_id, string cat_id, list<ROOT_DTO>& lstRoot);
	long get_roots2(list<ROOT_DTO>& lstRoot);
	long get_image_md5(string catids, MAP_MD5& lstmd5);

	long get_categories2(string ty, list<CATEGORY_DTO>& lstCategory);
	long get_categories(string catid, string order, list<CATEGORY_DTO>& lstCategory);

	long get_caddies(string pic_id, list<CADDIE_DTO>& lstCaddie);
	long get_favorites(string user_id, list<FAVORITE_DTO>& lstFavorite);
	long get_dashboard(DASHBOARD_DTO& dashboard_dto);
	long get_configs(string ids, list<CONFIG_DTO>& lstConfig);
	long get_summary(string year, string month, string day, long time_zone_bias, list<HISTORY_SUMMARY_DTO>& lstSummary);
	long get_log_viewer(string folder, string date0, string date1, list<LOG_VIEWER_DTO>& lstViewers);
	long get_log_folder(list<string>& lstFolders);
	long set_alb(string alb);
	long get_cat_nb(string cat_id, list<CAT_NB_DTO>& lstNb);
	long get_films(string artid, uint64_t md5sum, list<FILM_DTO>& lstfilm);
	long get_users(list<USER_DTO>& lstUsers);



	long _add_root(ROOT_DTO root, uint32_t& id);
	long _del_root(string id);
	long _add_category(CATEGORY_DTO category, uint32_t& id);
	long _add_category(list<CATEGORY_DTO> lstCategory);

	long _update_full_id(list<TEMPL_DTO> map_full_id);
	long _set_order_categories(list<string> lstCatIds);
	long _update_categories(string cat_id, CATEGORY_DTO cat_dto);
	//long _del_category(list<string> lstIds, map<string, list<CATEGORY_DTO>> mapCategroyOut);
	long _move_categories(multimap<string, string> mov_map);
	long _update_categories_representative(string cat_id, string picid);

	long _update_categories_imageorder(string cat_id, string order);
	long _add_favorites(string type, string username, string element_ids);
	long _add_caddie(string username, string op, string element_ids);
	long _clear_caddie(string username, uint32_t& nb);
	long _add_tag(TAG_DTO tag, uint32_t& id);

	long _update_tags(string mode, list<TAG_DTO> lstTag);
	long _delete_tags(string tags);
	long _merge_tags(string destination_tag, string merge_list);
	long _add_favorite(FAVORITE_DTO favorite);
	long _add_configs(list<CONFIG_DTO> lstConfig);
	long _merge_face(string destination_face, string merge_list);
	long _delete_face(string ids, list<string>& lstFile, uint32_t &nb);

	long _add_users(USER_DTO users_dto);
	long _updte_users(USER_DTO users_dto);
	long _del_users();

	long _add_history(HISTORY_DTO historyDto);
	long _add_log_viewer(LOG_VIEWER_DTO viewerDto);
	long _del_log_viewer(string path, string fname);
	long _add_film(FILM_DTO film_dto, uint32_t& id);
	long _update_film(FILM_DTO film_dto);

	long _del_films(string artids);
	long _update_cat_nb(list<CAT_NB_DTO> lstNb);
	long _update_last_import();
	long _unins();

private:
	long print_sql_msg(char* func, char* zSQL, char* errmsg);
	long print_sql2_msg(char* func, string sql, char* errmsg);
	//wstring get_path(string alb);
	long get_categories_by(string ty, string sql, list<CATEGORY_DTO>& lstCategory);
	long get_roots_by(string sql, list<ROOT_DTO>& lstRoot);
	long get_last_import_time(uint64_t& t);

private:
	sqlite3* _pDB;
	//string _isoPath = "";
	string _modulePath = "";

};
