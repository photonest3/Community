///////////////////////////////////////////////////////////////////////////////
// 文件名: chatdal.h (清理后版本)
// 模块: scheme/scheme
// 功能描述: 分类和标签数据访问层
//           提供分类、标签、收藏、用户等数据的 CRUD 操作
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-05-10  PhotoNest Team  添加胶片管理功能
//   2024-06-15  PhotoNest Team  优化批量操作性能
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "sqlite3.h"
#include "stmdef.h"
#include "Singleton.h"

using namespace std;

/// @brief 分类和标签数据访问类
/// @details 封装分类、标签、收藏、用户、胶片等相关数据的
///          数据库访问操作。提供查询、添加、更新、删除等完整 CRUD 功能。
///          采用单例模式确保全局唯一数据库访问实例。
/// @note 此类所有数据库操作都使用预编译语句提升性能和安全性
/// @warning 调用写入操作前请确保已获取数据库连接
class Cchatdal : public Singleton<Cchatdal>
{
	friend class Singleton<Cchatdal>;
	
protected:
	/// @brief 构造函数
	Cchatdal(void);
	
	/// @brief 析构函数
	~Cchatdal(void);

public:
	/// @brief 设置数据库句柄
	void setDb(sqlite3* pDB);
	
	// ========================================================================
	// 查询方法
	// ========================================================================
	
	/// @brief 获取根分类下的图片列表
	long get_roots(string image_id, string cat_id, list<ROOT_DTO>& lstRoot);
	
	/// @brief 获取所有根分类
	long get_roots2(list<ROOT_DTO>& lstRoot);
	
	/// @brief 获取图片的 MD5 哈希值
	long get_image_md5(string catids, MAP_MD5& lstmd5);

	/// @brief 获取分类列表（按类型）
	long get_categories2(string ty, list<CATEGORY_DTO>& lstCategory);
	
	/// @brief 获取子分类列表
	long get_categories(string catid, string order, list<CATEGORY_DTO>& lstCategory);

	/// @brief 获取图片的标签列表
	long get_caddies(string pic_id, list<CADDIE_DTO>& lstCaddie);
	
	/// @brief 获取用户收藏列表
	long get_favorites(string user_id, list<FAVORITE_DTO>& lstFavorite);
	
	/// @brief 获取仪表盘数据
	long get_dashboard(DASHBOARD_DTO& dashboard_dto);
	
	/// @brief 获取配置列表
	long get_configs(string ids, list<CONFIG_DTO>& lstConfig);
	
	/// @brief 获取历史摘要统计
	long get_summary(string year, string month, string day, long time_zone_bias, list<HISTORY_SUMMARY_DTO>& lstSummary);
	
	/// @brief 获取查看器日志
	long get_log_viewer(string folder, string date0, string date1, list<LOG_VIEWER_DTO>& lstViewers);
	
	/// @brief 获取日志文件夹列表
	long get_log_folder(list<string>& lstFolders);
	
	/// @brief 设置当前相册
	long set_alb(string alb);
	
	/// @brief 获取分类数量统计
	long get_cat_nb(string cat_id, list<CAT_NB_DTO>& lstNb);
	
	/// @brief 获取胶片列表
	long get_films(string artid, uint64_t md5sum, list<FILM_DTO>& lstfilm);
	
	/// @brief 获取用户列表
	long get_users(list<USER_DTO>& lstUsers);

	// ========================================================================
	// 写入方法
	// ========================================================================
	
	/// @brief 添加根分类
	long _add_root(ROOT_DTO root, uint32_t& id);
	
	/// @brief 删除根分类
	long _del_root(string id);
	
	/// @brief 添加分类
	long _add_category(CATEGORY_DTO category, uint32_t& id);
	
	/// @brief 批量添加分类
	long _add_category(list<CATEGORY_DTO> lstCategory);

	/// @brief 更新分类全路径 ID
	long _update_full_id(list<TEMPL_DTO> map_full_id);
	
	/// @brief 设置分类排序
	long _set_order_categories(list<string> lstCatIds);
	
	/// @brief 更新分类信息
	long _update_categories(string cat_id, CATEGORY_DTO cat_dto);
	
	/// @brief 移动分类
	long _move_categories(multimap<string, string> mov_map);
	
	/// @brief 更新分类的代表图片
	long _update_categories_representative(string cat_id, string picid);

	/// @brief 更新分类的图片排序方式
	long _update_categories_imageorder(string cat_id, string order);
	
	/// @brief 添加收藏
	long _add_favorites(string type, string username, string element_ids);
	
	/// @brief 添加标签到图片
	long _add_caddie(string username, string op, string element_ids);
	
	/// @brief 清空用户标签篮
	long _clear_caddie(string username, uint32_t& nb);
	
	/// @brief 添加标签
	long _add_tag(TAG_DTO tag, uint32_t& id);

	/// @brief 更新标签
	long _update_tags(string mode, list<TAG_DTO> lstTag);
	
	/// @brief 删除标签
	long _delete_tags(string tags);
	
	/// @brief 合并标签
	long _merge_tags(string destination_tag, string merge_list);
	
	/// @brief 添加收藏记录
	long _add_favorite(FAVORITE_DTO favorite);
	
	/// @brief 添加配置项
	long _add_configs(list<CONFIG_DTO> lstConfig);
	
	/// @brief 合并人脸标签
	long _merge_face(string destination_face, string merge_list);
	
	/// @brief 删除人脸标签
	long _delete_face(string ids, list<string>& lstFile, uint32_t &nb);

	/// @brief 添加用户
	long _add_users(USER_DTO users_dto);
	
	/// @brief 更新用户信息
	long _updte_users(USER_DTO users_dto);
	
	/// @brief 删除所有用户
	long _del_users();

	/// @brief 添加历史记录
	long _add_history(HISTORY_DTO historyDto);
	
	/// @brief 添加查看器日志
	long _add_log_viewer(LOG_VIEWER_DTO viewerDto);
	
	/// @brief 删除查看器日志
	long _del_log_viewer(string path, string fname);
	
	/// @brief 添加胶片记录
	long _add_film(FILM_DTO film_dto, uint32_t& id);
	
	/// @brief 更新胶片记录
	long _update_film(FILM_DTO film_dto);

	/// @brief 删除胶片记录
	long _del_films(string artids);
	
	/// @brief 更新分类数量统计
	long _update_cat_nb(list<CAT_NB_DTO> lstNb);
	
	/// @brief 更新最后导入时间
	long _update_last_import();
	
	/// @brief 卸载（清理资源）
	long _unins();

private:
	/// @brief 打印 SQL 错误信息（C 字符串版本）
	long print_sql_msg(char* func, char* zSQL, char* errmsg);
	
	/// @brief 打印 SQL 错误信息（C++ string 版本）
	long print_sql2_msg(char* func, string sql, char* errmsg);
	
	/// @brief 根据 SQL 查询获取分类列表
	long get_categories_by(string ty, string sql, list<CATEGORY_DTO>& lstCategory);
	
	/// @brief 根据 SQL 查询获取根分类列表
	long get_roots_by(string sql, list<ROOT_DTO>& lstRoot);
	
	/// @brief 获取最后导入时间
	long get_last_import_time(uint64_t& t);

private:
	/// @brief SQLite 数据库句柄
	sqlite3* _pDB;
	
	/// @brief 模块路径
	string _modulePath = "";
};
