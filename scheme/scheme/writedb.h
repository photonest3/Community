#pragma once

#include "stmdef.h"
#include "Singleton.h"

class writedb : public Singleton<writedb>
{
	friend class Singleton<writedb>;
protected:
	writedb(void);
	~writedb(void);

public:
	long add_imgerr(string cat_id, const MD5_DTO dto, uint32_t& id);
	long add_image(string cat_id, const IMAGE_DTO imageDto, long time_zone_bias, uint32_t& id);
	long update_unit_images(list<IMAGE_DTO> lstImage, long time_zone_bias);
	long update_quality(list<TEMPL_DTO> lstImage);
	long update_coi(IMAGE_DTO image_dto);

	long update_images(string action, string selection, string val);
	long del_image(string id);
	long update_images_hit(string image_id);
	long add_image_tags(string element_ids, list<IMAGE_TAG_DTO> lstImageTag);
	long update_images_rank(list<IMAGE_CATEGORY_DTO>& lstRank);

	long update_images_byzone(long time_zone_bias);
	long save_photo(PHOTO_DTO photo_dto, long time_zone_bias);
	long delete_orphans();
	long update_image_category(string image_id, string op, string name);
	long update_image_tag(string image_id, string op, string name);

	void set_representative_picture_id(string image_id);

	//-------------------------------------------------

	long add_root(ROOT_DTO root, uint32_t& id);
	long del_root(string id);
	long add_category(CATEGORY_DTO category, uint32_t& id);
	long add_category(list<CATEGORY_DTO> lstCategory);

	long update_full_id(list<TEMPL_DTO> map_full_id);
	long set_order_categories(list<string> lstCatIds);
	//long del_category(list<string> lstIds, map<string, list<CATEGORY_DTO>> mapCategroyOut);
	long move_categories(multimap<string, string> mov_map);
	long update_categories(string cat_id, CATEGORY_DTO cat_dto);

	long update_categories_representative(string cat_id, string picid);
	long update_categories_imageorder(string cat_id, string order);
	long add_favorites(string type, string username, string element_ids);
	long add_caddie(string username, string op, string element_ids);
	long clear_caddie(string username, uint32_t& nb);

	long add_tag(TAG_DTO tag, uint32_t& id);
	long update_tags(string mode, list<TAG_DTO> lstTag);
	long delete_tags(string tags);
	long merge_tags(string destination_tag, string merge_list);
	long add_favorite(FAVORITE_DTO favorite);

	long merge_face(string destination_face, string merge_list);
	long delete_face(string ids, list<string>& lstFile, uint32_t nb);
	long add_configs(list<CONFIG_DTO> lstConfig);
	long add_users(USER_DTO users_dto);
	long updte_users(USER_DTO users_dto);
	long del_users();
	long set_categories_visible(string& changed);

	long add_history(HISTORY_DTO historyDto);
	long add_log_viewer(LOG_VIEWER_DTO viewerDto);
	long del_log_viewer(string path, string fname);
	long add_film(FILM_DTO film_dto, uint32_t& id);
	long update_film(FILM_DTO film_dto);

	long del_films(string artids);
	long update_cat_nb(list<CAT_NB_DTO> lstNb);
	long update_last_import();
	long unins();

	long add_face(string ids, list<FACE_DTO> lstface, list<FACE_DTO>& lstout);

private:
	CRITICAL_SECTION _cs;
};
