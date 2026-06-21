///////////////////////////////////////////////////////////////////////////////
// 文件名: writedb.h
// 模块: scheme/scheme
// 功能描述: 数据库写入类
//           封装所有数据库写入操作（添加、更新、删除）
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-04-20  PhotoNest Team  添加批量写入功能
//   2024-06-01  PhotoNest Team  优化写入性能
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"

/// @brief 数据库写入类
/// @details 封装所有数据库写入操作（添加、更新、删除）。
///          包括图片、分类、标签、用户、配置等所有数据的写入。
///          采用单例模式确保全局唯一实例。
/// @note 此类是数据库写入操作的核心
/// @warning 写入操作需要加锁，防止多线程冲突
class CWritedb : public Singleton<CWritedb>
{
	friend class Singleton<CWritedb>;

protected:
	/// @brief 构造函数
	/// @details 初始化数据库写入对象，初始化临界区
	CWritedb(void);
	
	/// @brief 析构函数
	/// @details 清理资源，删除临界区
	~CWritedb(void);

public:
	/// @brief 添加 MD5 错误信息
	/// @param[in] cat_id 分类 ID
	/// @param[in] dto MD5 DTO
	/// @param[out] id 新记录 ID（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 记录图片文件的 MD5 校验和错误
	long add_imgerr(string cat_id, const MD5_DTO dto, uint32_t& id);

	/// @brief 添加图片记录
	/// @param[in] cat_id 分类 ID
	/// @param[in] imageDto 图片 DTO
	/// @param[in] time_zone_bias 时区偏移（秒）
	/// @param[out] id 新图片 ID（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 添加图片记录时会自动提取 EXIF 时间信息
	long add_image(string cat_id, const IMAGE_DTO imageDto, long time_zone_bias, uint32_t& id);

	/// @brief 批量更新图片信息
	/// @param[in] lstImage 图片 DTO 列表
	/// @param[in] time_zone_bias 时区偏移（秒）
	/// @return 0: 成功; 其他: 错误码
	/// @note 批量更新图片的元数据信息
	long update_unit_images(list<IMAGE_DTO> lstImage, long time_zone_bias);

	/// @brief 批量更新图片质量信息
	/// @param[in] lstImage 模板 DTO 列表
	/// @return 0: 成功; 其他: 错误码
	/// @note 更新图片的质量评分、尺寸等信息
	long update_quality(list<TEMPL_DTO> lstImage);

	/// @brief 更新图片 COI（Clip Object Image）信息
	/// @param[in] image_dto 图片 DTO
	/// @return 0: 成功; 其他: 错误码
	/// @note COI 是图片中检测到的对象区域
	long update_coi(IMAGE_DTO image_dto);

	/// @brief 添加人脸数据
	/// @param[in] ids 图片 ID 列表（逗号分隔）
	/// @param[in] lstface 人脸 DTO 列表
	/// @param[out] lstout 输出人脸 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 为图片添加人脸识别数据
	long add_face(string ids, list<FACE_DTO> lstface, list<FACE_DTO>& lstout);

	/// @brief 批量更新图片信息
	/// @param[in] action 操作类型（"set", "clear", "toggle"）
	/// @param[in] selection 选择范围（"all", "page", "selected"）
	/// @param[in] val 设置值
	/// @return 0: 成功; 其他: 错误码
	/// @note 批量设置图片属性（如收藏、星级等）
	long update_images(string action, string selection, string val);

	/// @brief 删除图片
	/// @param[in] id 图片 ID
	/// @return 0: 成功; 其他: 错误码
	/// @warning 此操作不可恢复，会同时删除图片文件和数据库记录
	long del_image(string id);

	/// @brief 更新图片点击次数
	/// @param[in] image_id 图片 ID
	/// @return 0: 成功; 其他: 错误码
	/// @note 每次查看图片时调用，用于统计热门图片
	long update_images_hit(string image_id);

	/// @brief 添加图片标签关联
	/// @param[in] element_ids 元素 ID 列表（逗号分隔）
	/// @param[in] lstImageTag 图片标签 DTO 列表
	/// @return 0: 成功; 其他: 错误码
	/// @note 为图片添加标签关联
	long add_image_tags(string element_ids, list<IMAGE_TAG_DTO> lstImageTag);

	/// @brief 批量更新图片排序等级
	/// @param[in] lstRank 图片分类 DTO 列表（包含排序等级）
	/// @return 0: 成功; 其他: 错误码
	/// @note 更新图片在分类中的显示顺序
	long update_images_rank(list<IMAGE_CATEGORY_DTO>& lstRank);

	/// @brief 根据时区更新图片时间
	/// @param[in] time_zone_bias 时区偏移（秒）
	/// @return 0: 成功; 其他: 错误码
	/// @note 批量修正图片的拍摄时间（考虑时区）
	long update_images_byzone(long time_zone_bias);

	/// @brief 保存图片信息（完整保存）
	/// @param[in] photo_dto 图片信息 DTO
	/// @param[in] time_zone_bias 时区偏移（秒）
	/// @return 0: 成功; 其他: 错误码
	/// @note 保存图片的完整信息（元数据、标签、分类等）
	long save_photo(PHOTO_DTO photo_dto, long time_zone_bias);

	/// @brief 删除孤立图片记录
	/// @return 0: 成功; 其他: 错误码
	/// @note 删除数据库中已不存在文件对应的记录
	/// @warning 此操作不可恢复
	long delete_orphans();

	/// @brief 更新图片分类关联
	/// @param[in] image_id 图片 ID
	/// @param[in] op 操作类型（"add", "del", "set"）
	/// @param[in] name 分类名称
	/// @return 0: 成功; 其他: 错误码
	/// @note 修改图片所属的分类
	long update_image_category(string image_id, string op, string name);

	/// @brief 更新图片标签关联
	/// @param[in] image_id 图片 ID
	/// @param[in] op 操作类型（"add", "del", "set"）
	/// @param[in] name 标签名称
	/// @return 0: 成功; 其他: 错误码
	/// @note 修改图片的标签
	long update_image_tag(string image_id, string op, string name);

	/// @brief 设置分类的代表图片
	/// @param[in] image_id 图片 ID
	/// @note 设置某张图片作为其所属分类的代表图片（用于分类封面）
	void set_representative_picture_id(string image_id);

	/// @brief 设置分类可见性
	/// @param[in,out] changed 变更状态（输出时被修改）
	/// @return 0: 成功; 其他: 错误码
	/// @note 根据图片数量自动设置分类的可见性
	long set_categories_visible(string& changed);

	//-------------------------------------------------

	/// @brief 添加根目录记录
	/// @param[in] root 根目录 DTO
	/// @param[out] id 新记录 ID（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 添加根目录记录到数据库
	long add_root(ROOT_DTO root, uint32_t& id);

	/// @brief 删除根目录记录
	/// @param[in] id 根目录 ID
	/// @return 0: 成功; 其他: 错误码
	/// @warning 此操作不可恢复
	long del_root(string id);

	/// @brief 添加分类记录
	/// @param[in] category 分类 DTO
	/// @param[out] id 新记录 ID（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 添加分类记录到数据库
	long add_category(CATEGORY_DTO category, uint32_t& id);

	/// @brief 批量添加分类记录
	/// @param[in] lstCategory 分类 DTO 列表
	/// @return 0: 成功; 其他: 错误码
	/// @note 批量添加分类记录到数据库
	long add_category(list<CATEGORY_DTO> lstCategory);

	/// @brief 更新分类全 ID
	/// @param[in] map_full_id 全 ID 映射表
	/// @return 0: 成功; 其他: 错误码
	/// @note 更新分类的完整 ID（包含父分类路径）
	long update_full_id(list<TEMPL_DTO> map_full_id);

	/// @brief 设置分类排序
	/// @param[in] lstCatIds 分类 ID 列表
	/// @return 0: 成功; 其他: 错误码
	/// @note 批量设置分类的排序等级
	long set_order_categories(list<string> lstCatIds);

	/// @brief 更新分类信息
	/// @param[in] cat_id 分类 ID
	/// @param[in] cat_dto 分类 DTO
	/// @return 0: 成功; 其他: 错误码
	/// @note 更新分类的名称、注释、排序等信息
	long update_categories(string cat_id, CATEGORY_DTO cat_dto);

	/// @brief 删除分类
	/// @param[in] lstIds 分类 ID 列表
	/// @param[in] mapCategroyOut 分类 DTO 映射表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @warning 此操作不可恢复，会同时删除分类下的所有图片
	long del_category(list<string> lstIds, map<string, list<CATEGORY_DTO>> mapCategroyOut);

	/// @brief 移动分类
	/// @param[in] mov_map 移动映射表（源分类 ID -> 目标分类 ID）
	/// @return 0: 成功; 其他: 错误码
	/// @note 批量移动分类到新的父分类下
	long move_categories(multimap<string, string> mov_map);

	/// @brief 更新分类代表图片
	/// @param[in] cat_id 分类 ID
	/// @param[in] picid 图片 ID
	/// @return 0: 成功; 其他: 错误码
	/// @note 设置分类的代表图片（用于分类封面）
	long update_categories_representative(string cat_id, string picid);

	/// @brief 更新分类图片排序方式
	/// @param[in] cat_id 分类 ID
	/// @param[in] order 排序方式
	/// @return 0: 成功; 其他: 错误码
	/// @note 设置分类中图片的排序方式
	long update_categories_imageorder(string cat_id, string order);

	/// @brief 添加收藏
	/// @param[in] type 类型（"image", "category" 等）
	/// @param[in] username 用户名
	/// @param[in] element_ids 元素 ID 列表（逗号分隔）
	/// @return 0: 成功; 其他: 错误码
	/// @note 为用户添加收藏记录
	long add_favorites(string type, string username, string element_ids);

	/// @brief 添加购物车
	/// @param[in] username 用户名
	/// @param[in] op 操作类型（"add", "del", "set"）
	/// @param[in] element_ids 元素 ID 列表（逗号分隔）
	/// @return 0: 成功; 其他: 错误码
	/// @note 为用户添加购物车记录（待处理图片）
	long add_caddie(string username, string op, string element_ids);

	/// @brief 清空购物车
	/// @param[in] username 用户名
	/// @param[out] nb 删除数量（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 清空用户的购物车
	long clear_caddie(string username, uint32_t& nb);

	/// @brief 添加标签
	/// @param[in] tag 标签 DTO
	/// @param[out] id 新记录 ID（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 添加标签记录到数据库
	long add_tag(TAG_DTO tag, uint32_t& id);

	/// @brief 更新标签
	/// @param[in] mode 更新模式
	/// @param[in] lstTag 标签 DTO 列表
	/// @return 0: 成功; 其他: 错误码
	/// @note 批量更新标签信息
	long update_tags(string mode, list<TAG_DTO> lstTag);

	/// @brief 删除标签
	/// @param[in] tags 标签 ID 列表（逗号分隔）
	/// @return 0: 成功; 其他: 错误码
	/// @warning 此操作不可恢复
	long delete_tags(string tags);

	/// @brief 合并标签
	/// @param[in] destination_tag 目标标签 ID
	/// @param[in] merge_list 待合并标签 ID 列表（逗号分隔）
	/// @return 0: 成功; 其他: 错误码
	/// @note 将多个标签合并为一个标签
	long merge_tags(string destination_tag, string merge_list);

	/// @brief 添加收藏记录
	/// @param[in] favorite 收藏 DTO
	/// @return 0: 成功; 其他: 错误码
	/// @note 添加图片收藏记录
	long add_favorite(FAVORITE_DTO favorite);

	/// @brief 添加配置项
	/// @param[in] lstConfig 配置 DTO 列表
	/// @return 0: 成功; 其他: 错误码
	/// @note 批量添加配置项到数据库
	long add_configs(list<CONFIG_DTO> lstConfig);

	/// @brief 合并人脸
	/// @param[in] destination_face 目标人脸 ID
	/// @param[in] merge_list 待合并人脸 ID 列表（逗号分隔）
	/// @return 0: 成功; 其他: 错误码
	/// @note 将多个人脸合并为一个人员
	long merge_face(string destination_face, string merge_list);

	/// @brief 删除人脸
	/// @param[in] ids 人脸 ID 列表（逗号分隔）
	/// @param[out] lstFile 文件列表（输出）
	/// @param[out] nb 删除数量（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @warning 此操作不可恢复
	long delete_face(string ids, list<string>& lstFile, uint32_t& nb);

	/// @brief 添加用户
	/// @param[in] users_dto 用户 DTO
	/// @return 0: 成功; 其他: 错误码
	/// @note 添加用户记录到数据库
	long add_users(USER_DTO users_dto);

	/// @brief 更新用户
	/// @param[in] users_dto 用户 DTO
	/// @return 0: 成功; 其他: 错误码
	/// @note 更新用户信息（密码、提示问题等）
	long updte_users(USER_DTO users_dto);

	/// @brief 删除用户
	/// @return 0: 成功; 其他: 错误码
	/// @warning 此操作不可恢复，会同时删除用户的所有数据
	long del_users();

	/// @brief 添加历史记录
	/// @param[in] historyDto 历史记录 DTO
	/// @return 0: 成功; 其他: 错误码
	/// @note 添加用户历史记录（如最近查看的图片）
	long add_history(HISTORY_DTO historyDto);

	/// @brief 添加日志查看器记录
	/// @param[in] viewerDto 日志查看器 DTO
	/// @return 0: 成功; 其他: 错误码
	/// @note 添加日志查看器记录（用于查看上传日志）
	long add_log_viewer(LOG_VIEWER_DTO viewerDto);

	/// @brief 删除日志查看器记录
	/// @param[in] path 文件夹路径
	/// @param[in] fname 文件名
	/// @return 0: 成功; 其他: 错误码
	/// @warning 此操作不可恢复
	long del_log_viewer(string path, string fname);

	/// @brief 添加胶片记录
	/// @param[in] film_dto 胶片 DTO
	/// @param[out] id 新记录 ID（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 添加胶片记录（图片拼接或幻灯片）
	long add_film(FILM_DTO film_dto, uint32_t& id);

	/// @brief 更新胶片记录
	/// @param[in] film_dto 胶片 DTO
	/// @return 0: 成功; 其他: 错误码
	/// @note 更新胶片信息（标题、图片列表、音乐等）
	long update_film(FILM_DTO film_dto);

	/// @brief 删除胶片记录
	/// @param[in] artids 胶片 ID 列表（逗号分隔）
	/// @return 0: 成功; 其他: 错误码
	/// @warning 此操作不可恢复
	long del_films(string artids);

	/// @brief 更新分类数量信息
	/// @param[in] lstNb 分类数量 DTO 列表
	/// @return 0: 成功; 其他: 错误码
	/// @note 批量更新分类的图片数量信息
	long update_cat_nb(list<CAT_NB_DTO> lstNb);

	/// @brief 更新最后导入时间
	/// @return 0: 成功; 其他: 错误码
	/// @note 更新数据库的最后导入时间
	long update_last_import();

	/// @brief 卸载（清理数据）
	/// @return 0: 成功; 其他: 错误码
	/// @note 清理数据库缓存，释放资源
	long unins();

	/// @brief 添加人脸数据（重载）
	/// @param[in] ids 图片 ID 列表（逗号分隔）
	/// @param[in] lstface 人脸 DTO 列表
	/// @param[out] lstout 输出人脸 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 为图片添加人脸识别数据（重载版本）
	long add_face(string ids, list<FACE_DTO> lstface, list<FACE_DTO>& lstout);

private:
	/// @brief 临界区对象，用于多线程同步
	CRITICAL_SECTION _cs;
	
};
