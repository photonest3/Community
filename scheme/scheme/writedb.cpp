////////////////////////////////////////////////////////////////////
/// @file writedb.cpp
/// @brief 数据库写入操作封装实现文件
/// @details 此文件实现了 writedb 类的所有方法，
///          封装了对 imagedal 和 chatdal 的写入操作，
///          并使用临界区（CriticalSection）保证线程安全。
///          所有写入数据库的操作都通过此类进行，
///          确保多线程环境下的数据一致性。
/// @note 使用单例模式，通过 CriticalSection 保证线程安全
/// @author PhotoNest Team
/// @date 2024
////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include "writedb.h"
#include "imagedal.h"
#include "chatdal.h"
#include "HtmlgetMenubar.h"

////////////////////////////////////////////////////////////////////
/// @brief 构造函数
/// @details 初始化临界区对象，用于保证线程安全
////////////////////////////////////////////////////////////////////
writedb::writedb(void)
{
	InitializeCriticalSection(&_cs);
}

////////////////////////////////////////////////////////////////////
/// @brief 析构函数
/// @details 释放临界区对象资源
////////////////////////////////////////////////////////////////////
writedb::~writedb(void)
{
	DeleteCriticalSection(&_cs);
}

////////////////////////////////////////////////////////////////////
/// @brief 添加图片错误记录
/// @param[in] cat_id 分类 ID
/// @param[in] dto MD5 数据传递对象
/// @param[out] id 返回新记录的 ID
/// @return 0: 成功
/// @note 使用临界区保护数据库写入操作
////////////////////////////////////////////////////////////////////
long writedb::add_imgerr(string cat_id, const MD5_DTO dto, uint32_t& id)
{
	EnterCriticalSection(&_cs);
	Cimagedal::Instance()._add_imgerr(cat_id, dto, id);
	LeaveCriticalSection(&_cs);
	return 0;
}

////////////////////////////////////////////////////////////////////
/// @brief 添加图片记录
/// @param[in] cat_id 分类 ID
/// @param[in] imageDto 图片数据传递对象
/// @param[in] time_zone_bias 时区偏移（分钟）
/// @param[out] id 返回新记录的 ID
/// @return 0: 成功
/// @note 使用临界区保护数据库写入操作
////////////////////////////////////////////////////////////////////
long writedb::add_image(string cat_id, const IMAGE_DTO imageDto, long time_zone_bias, uint32_t& id)
{
	EnterCriticalSection(&_cs);
	Cimagedal::Instance()._add_image(cat_id, imageDto, time_zone_bias, id);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::update_unit_images(list<IMAGE_DTO> lstImage, long time_zone_bias)
{
	EnterCriticalSection(&_cs);
	Cimagedal::Instance()._update_unit_images(lstImage, time_zone_bias);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::update_quality(list<TEMPL_DTO> lstImage)
{
	EnterCriticalSection(&_cs);
	Cimagedal::Instance()._update_quality(lstImage);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::update_coi(IMAGE_DTO image_dto)
{
	EnterCriticalSection(&_cs);
	Cimagedal::Instance()._update_coi(image_dto);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::update_images(string action, string selection, string val)
{
	EnterCriticalSection(&_cs);
	Cimagedal::Instance()._update_images(action, selection, val);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::del_image(string id)
{
	EnterCriticalSection(&_cs);
	Cimagedal::Instance()._del_image(id);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::update_images_hit(string image_id)
{
	EnterCriticalSection(&_cs);
	Cimagedal::Instance()._update_images_hit(image_id);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::add_image_tags(string element_ids, list<IMAGE_TAG_DTO> lstImageTag)
{
	EnterCriticalSection(&_cs);
	Cimagedal::Instance()._add_image_tags(element_ids, lstImageTag);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::update_images_rank(list<IMAGE_CATEGORY_DTO>& lstRank)
{
	EnterCriticalSection(&_cs);
	Cimagedal::Instance()._update_images_rank(lstRank);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::update_images_byzone(long time_zone_bias)
{
	EnterCriticalSection(&_cs);
	Cimagedal::Instance()._update_images_byzone(time_zone_bias);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::save_photo(PHOTO_DTO photo_dto, long time_zone_bias)
{
	EnterCriticalSection(&_cs);
	Cimagedal::Instance()._save_photo(photo_dto, time_zone_bias);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::delete_orphans()
{
	EnterCriticalSection(&_cs);
	Cimagedal::Instance()._delete_orphans();
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::update_image_category(string image_id, string op, string name)
{
	EnterCriticalSection(&_cs);
	Cimagedal::Instance()._update_image_category(image_id, op, name);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::update_image_tag(string image_id, string op, string name)
{
	EnterCriticalSection(&_cs);
	Cimagedal::Instance()._update_image_tag(image_id, op, name);
	LeaveCriticalSection(&_cs);
	return 0;
}

void writedb::set_representative_picture_id(string image_id)
{
	EnterCriticalSection(&_cs);
	Cimagedal::Instance()._set_representative_picture_id(image_id);
	LeaveCriticalSection(&_cs);
}

long writedb::set_categories_visible(string& changed)
{
	EnterCriticalSection(&_cs);
	Cimagedal::Instance()._set_categories_visible(changed);
	LeaveCriticalSection(&_cs);
	return 0;
}

//---------------

long writedb::add_root(ROOT_DTO root, uint32_t& id)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._add_root(root, id);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::del_root(string id)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._del_root(id);
	LeaveCriticalSection(&_cs);
	CHtmlgetMenubar::Instance().clear();
	return 0;
}

long writedb::add_category(CATEGORY_DTO category, uint32_t& id)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._add_category(category, id);
	LeaveCriticalSection(&_cs);
	CHtmlgetMenubar::Instance().clear();
	return 0;
}
long writedb::add_category(list<CATEGORY_DTO> lstCategory)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._add_category(lstCategory);
	LeaveCriticalSection(&_cs);
	CHtmlgetMenubar::Instance().clear();
	return 0;
}
long writedb::update_full_id(list<TEMPL_DTO> map_full_id)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._update_full_id(map_full_id);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::set_order_categories(list<string> lstCatIds)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._set_order_categories(lstCatIds);
	LeaveCriticalSection(&_cs);
	CHtmlgetMenubar::Instance().clear();
	return 0;
}
//long writedb::del_category(list<string> lstIds, map<string, list<CATEGORY_DTO>> mapCategroyOut)
//{
//	EnterCriticalSection(&_cs);
//	Cchatdal::Instance()._del_category(lstIds, mapCategroyOut);
//	LeaveCriticalSection(&_cs);
//	CHtmlgetMenubar::Instance().clear();
//	return 0;
//}
long writedb::move_categories(multimap<string, string> mov_map)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._move_categories(mov_map);
	LeaveCriticalSection(&_cs);
	CHtmlgetMenubar::Instance().clear();
	return 0;
}

long writedb::update_categories(string cat_id, CATEGORY_DTO cat_dto)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._update_categories(cat_id, cat_dto);
	LeaveCriticalSection(&_cs);
	CHtmlgetMenubar::Instance().clear();
	return 0;
}
long writedb::update_categories_representative(string cat_id, string picid)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._update_categories_representative(cat_id, picid);
	LeaveCriticalSection(&_cs);
	CHtmlgetMenubar::Instance().clear();
	return 0;
}
long writedb::update_categories_imageorder(string cat_id, string order)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._update_categories_imageorder(cat_id, order);
	LeaveCriticalSection(&_cs);
	CHtmlgetMenubar::Instance().clear();
	return 0;
}

long writedb::add_favorites(string type, string username, string element_ids)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._add_favorites(type, username, element_ids);
	LeaveCriticalSection(&_cs);
	return 0;
}
long writedb::add_caddie(string username, string op, string element_ids)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._add_caddie(username, op, element_ids);
	LeaveCriticalSection(&_cs);
	return 0;
}
long writedb::clear_caddie(string username, uint32_t& nb)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._clear_caddie(username, nb);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::add_tag(TAG_DTO tag, uint32_t& id)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._add_tag(tag, id);
	LeaveCriticalSection(&_cs);
	return 0;
}
long writedb::update_tags(string mode, list<TAG_DTO> lstTag)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._update_tags(mode, lstTag);
	LeaveCriticalSection(&_cs);
	return 0;
}
long writedb::delete_tags(string tags)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._delete_tags(tags);
	LeaveCriticalSection(&_cs);
	return 0;
}
long writedb::merge_tags(string destination_tag, string merge_list)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._merge_tags(destination_tag, merge_list);
	LeaveCriticalSection(&_cs);
	return 0;
}
long writedb::merge_face(string destination_face, string merge_list)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._merge_face(destination_face, merge_list);
	LeaveCriticalSection(&_cs);
	return 0;
}
long writedb::delete_face(string ids, list<string>& lstFile, uint32_t nb)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._delete_face(ids, lstFile, nb);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::add_favorite(FAVORITE_DTO favorite)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._add_favorite(favorite);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::add_configs(list<CONFIG_DTO> lstConfig)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._add_configs(lstConfig);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::add_users(USER_DTO users_dto)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._add_users(users_dto);
	LeaveCriticalSection(&_cs);
	return 0;
}
long writedb::updte_users(USER_DTO users_dto)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._updte_users(users_dto);
	LeaveCriticalSection(&_cs);
	return 0;
}
long writedb::del_users()
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._del_users();
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::add_history(HISTORY_DTO historyDto)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._add_history(historyDto);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::add_log_viewer(LOG_VIEWER_DTO viewerDto)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._add_log_viewer(viewerDto);
	LeaveCriticalSection(&_cs);
	return 0;
}
long writedb::del_log_viewer(string path, string fname)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._del_log_viewer(path, fname);
	LeaveCriticalSection(&_cs);
	return 0;
}

long writedb::add_film(FILM_DTO film_dto, uint32_t& id)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._add_film(film_dto, id);
	LeaveCriticalSection(&_cs);
	return 0;
}
long writedb::update_film(FILM_DTO film_dto)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._update_film(film_dto);
	LeaveCriticalSection(&_cs);
	return 0;
}
long writedb::del_films(string artids)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._del_films(artids);
	LeaveCriticalSection(&_cs);
	return 0;
}
long writedb::update_cat_nb(list<CAT_NB_DTO> lstNb)
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._update_cat_nb(lstNb);
	LeaveCriticalSection(&_cs);
	return 0;
}
long writedb::update_last_import()
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._update_last_import();
	LeaveCriticalSection(&_cs);
	return 0;
}
long writedb::unins()
{
	EnterCriticalSection(&_cs);
	Cchatdal::Instance()._unins();
	LeaveCriticalSection(&_cs);
	return 0;
}
long writedb::add_face(string ids, list<FACE_DTO> lstface, list<FACE_DTO>& lstout)
{
	EnterCriticalSection(&_cs);
	Cimagedal::Instance()._add_face(ids, lstface, lstout);
	LeaveCriticalSection(&_cs);
	return 0;
}
