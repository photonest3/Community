///////////////////////////////////////////////////////////////////////////////
// 文件名: imagedal.h
// 模块: scheme/scheme
// 功能描述: 图片数据访问层
//           提供图片、标签、人脸等数据的 CRUD 操作
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-04-20  PhotoNest Team  添加人脸数据管理功能
//   2024-06-01  PhotoNest Team  优化批量操作性能
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "sqlite3.h"
#include "stmdef.h"
#include "Singleton.h"

using namespace std;

/// @brief 图片数据访问层类
/// @details 封装图片、标签、人脸、分类关联等数据的数据库访问操作。
///          提供查询、添加、更新、删除等完整 CRUD 功能。
///          采用单例模式确保全局唯一数据库访问实例。
/// @note 此类所有数据库操作都使用预编译语句提升性能和安全性
/// @warning 调用写入操作前请确保已获取数据库连接
class Cimagedal : public Singleton<Cimagedal>
{
	friend class Singleton<Cimagedal>;

protected:
	/// @brief 构造函数
	/// @details 初始化图片数据访问对象
	Cimagedal(void);
	
	/// @brief 析构函数
	/// @details 清理资源
	~Cimagedal(void);

	// TODO: add your methods here.

public:
	/// @brief 设置数据库句柄
	/// @param[in] pDB SQLite 数据库句柄指针
	/// @note 此方法必须在所有数据库操作前调用
	void setDb(sqlite3* pDB);
	
	/// @brief 刷新排序字段
	/// @param[in,out] orderby 排序字段字符串（输出时被修改）
	/// @return 0: 成功; 其他: 错误码
	/// @note 更新内存中的排序字段缓存
	long refresh_orderby(string& orderby);

	/// @brief 检查图片是否已存在
	/// @param[in] cat_id 分类 ID
	/// @param[in] file 文件名
	/// @param[in] md5sum MD5 校验和
	/// @param[out] existId 已存在图片的 ID（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 用于防止重复导入同一图片
	long exist_image(string cat_id, string file, uint64_t md5sum, string& existId);
	
	/// @brief 获取图片 MD5 校验和错误列表
	/// @param[in] dir 目录路径
	/// @param[out] lstmd5 MD5 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 用于检测和修复图片文件的 MD5 校验和错误
	long get_imgerr(string dir, list<MD5_DTO>& lstmd5);

public:
	/// @brief 添加 MD5 校验和错误记录
	/// @param[in] cat_id 分类 ID
	/// @param[in] dto MD5 DTO
	/// @param[out] id 新记录 ID（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 记录图片文件的 MD5 校验和错误
	long _add_imgerr(string cat_id, const MD5_DTO dto, uint32_t& id);
	
	/// @brief 添加图片记录
	/// @param[in] cat_id 分类 ID
	/// @param[in] imageDto 图片 DTO
	/// @param[in] time_zone_bias 时区偏移（秒）
	/// @param[out] id 新图片 ID（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 添加图片记录时会自动提取 EXIF 时间信息
	long _add_image(string cat_id, const IMAGE_DTO imageDto, long time_zone_bias, uint32_t& id);
	
	/// @brief 批量更新图片信息
	/// @param[in] lstImage 图片 DTO 列表
	/// @param[in] time_zone_bias 时区偏移（秒）
	/// @return 0: 成功; 其他: 错误码
	/// @note 批量更新图片的元数据信息
	long _update_unit_images(list<IMAGE_DTO> lstImage, long time_zone_bias);
	
	/// @brief 批量更新图片质量信息
	/// @param[in] lstImage 模板 DTO 列表
	/// @return 0: 成功; 其他: 错误码
	/// @note 更新图片的质量评分、尺寸等信息
	long _update_quality(list<TEMPL_DTO> lstImage);
	
	/// @brief 更新图片 COI（Clip Object Image）信息
	/// @param[in] image_dto 图片 DTO
	/// @return 0: 成功; 其他: 错误码
	/// @note COI 是图片中检测到的对象区域
	long _update_coi(IMAGE_DTO image_dto);

	/// @brief 添加人脸数据
	/// @param[in] ids 图片 ID 列表（逗号分隔）
	/// @param[in] lstface 人脸 DTO 列表
	/// @param[out] lstout 输出人脸 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 为图片添加人脸识别数据
	long _add_face(string ids, list<FACE_DTO> lstface, list<FACE_DTO>& lstout);

	/// @brief 批量更新图片信息
	/// @param[in] action 操作类型（"set", "clear", "toggle"）
	/// @param[in] selection 选择范围（"all", "page", "selected"）
	/// @param[in] val 设置值
	/// @return 0: 成功; 其他: 错误码
	/// @note 批量设置图片属性（如收藏、星级等）
	long _update_images(string action, string selection, string val);
	
	/// @brief 删除图片
	/// @param[in] id 图片 ID
	/// @return 0: 成功; 其他: 错误码
	/// @warning 此操作不可恢复，会同时删除图片文件和数据库记录
	long _del_image(string id);
	
	/// @brief 更新图片点击次数
	/// @param[in] image_id 图片 ID
	/// @return 0: 成功; 其他: 错误码
	/// @note 每次查看图片时调用，用于统计热门图片
	long _update_images_hit(string image_id);
	
	/// @brief 添加图片标签关联
	/// @param[in] element_ids 元素 ID 列表（逗号分隔）
	/// @param[in] lstImageTag 图片标签 DTO 列表
	/// @return 0: 成功; 其他: 错误码
	/// @note 为图片添加标签关联
	long _add_image_tags(string element_ids, list<IMAGE_TAG_DTO> lstImageTag);
	
	/// @brief 批量更新图片排序等级
	/// @param[in] lstRank 图片分类 DTO 列表（包含排序等级）
	/// @return 0: 成功; 其他: 错误码
	/// @note 更新图片在分类中的显示顺序
	long _update_images_rank(list<IMAGE_CATEGORY_DTO>& lstRank);

	/// @brief 根据时区更新图片时间
	/// @param[in] time_zone_bias 时区偏移（秒）
	/// @return 0: 成功; 其他: 错误码
	/// @note 批量修正图片的拍摄时间（考虑时区）
	long _update_images_byzone(long time_zone_bias);
	
	/// @brief 保存图片信息（完整保存）
	/// @param[in] photo_dto 图片信息 DTO
	/// @param[in] time_zone_bias 时区偏移（秒）
	/// @return 0: 成功; 其他: 错误码
	/// @note 保存图片的完整信息（元数据、标签、分类等）
	long _save_photo(PHOTO_DTO photo_dto, long time_zone_bias);
	
	/// @brief 删除孤立图片记录
	/// @return 0: 成功; 其他: 错误码
	/// @note 删除数据库中已不存在文件对应的记录
	/// @warning 此操作不可恢复
	long _delete_orphans();
	
	/// @brief 更新图片分类关联
	/// @param[in] image_id 图片 ID
	/// @param[in] op 操作类型（"add", "del", "set"）
	/// @param[in] name 分类名称
	/// @return 0: 成功; 其他: 错误码
	/// @note 修改图片所属的分类
	long _update_image_category(string image_id, string op, string name);
	
	/// @brief 更新图片标签关联
	/// @param[in] image_id 图片 ID
	/// @param[in] op 操作类型（"add", "del", "set"）
	/// @param[in] name 标签名称
	/// @return 0: 成功; 其他: 错误码
	/// @note 修改图片的标签
	long _update_image_tag(string image_id, string op, string name);

	/// @brief 设置分类的代表图片
	/// @param[in] image_id 图片 ID
	/// @note 设置某张图片作为其所属分类的代表图片（用于分类封面）
	void _set_representative_picture_id(string image_id);
	
	/// @brief 设置分类可见性
	/// @param[in,out] changed 变更状态（输出时被修改）
	/// @return 0: 成功; 其他: 错误码
	/// @note 根据图片数量自动设置分类的可见性
	long _set_categories_visible(string& changed);

public:
	/// @brief 获取图片作者列表
	/// @param[out] mapAuthor 作者-图片数量映射表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 统计每个作者的图片数量
	long get_image_authors(map<string, uint32_t >& mapAuthor);
	
	/// @brief 获取图片人脸数据
	/// @param[in] ids 图片 ID 列表（逗号分隔）
	/// @param[in] notids 排除的图片 ID 列表（逗号分隔）
	/// @param[out] lstface 人脸 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取指定图片的人脸识别数据
	long get_face(string ids, string notids, list<FACE_DTO>& lstface);

	/// @brief 获取图片标签关联
	/// @param[in] image_ids 图片 ID 列表（逗号分隔）
	/// @param[out] lstImageTag 图片标签 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取指定图片的标签关联
	long get_image_tags(string image_ids, list<IMAGE_TAG_DTO>& lstImageTag);
	
	/// @brief 获取图片标签数量
	/// @param[in] image_ids 图片 ID 列表（逗号分隔）
	/// @param[out] nb 标签数量（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 统计指定图片的标签数量
	long get_image_tags_count(string image_ids, uint32_t& nb);
	
	/// @brief 获取菜单栏标签列表
	/// @param[in] image_ids 图片 ID 列表（逗号分隔）
	/// @param[in] cat_ids 分类 ID 列表（逗号分隔）
	/// @param[out] lstImageTag 图片标签 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取用于菜单栏显示的标签列表
	long get_menubar_tags(string image_ids, string cat_ids, list<IMAGE_TAG_DTO>& lstImageTag);
	
	/// @brief 获取图片属性（分类和标签）
	/// @param[in] image_id 图片 ID
	/// @param[out] cats 分类 ID 列表字符串（输出）
	/// @param[out] tags 标签 ID 列表字符串（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取指定图片的所有分类和标签
	long get_image_properties(string image_id, string& cats, string& tags);

public:
	/// @brief 获取图片 ID 列表（综合查询）
	/// @param[in] req 查询请求对象
	/// @param[in] search_req 搜索请求对象
	/// @param[out] lstImageIds 图片 ID 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 根据查询条件和搜索条件获取图片 ID 列表
	long get_image_ids(QUERY_REQ req, SEARCH_REQ search_req, list<string>& lstImageIds);
	
	/// @brief 获取过滤后的 ID 数量
	/// @param[in] req 搜索请求对象
	/// @return 过滤后的图片数量
	/// @note 根据搜索条件统计图片数量
	long get_filter_ids(SEARCH_REQ req);
	
	/// @brief 生成搜索 SQL WHERE 子句
	/// @param[in] req 搜索请求对象
	/// @param[out] sql SQL WHERE 子句（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 根据搜索条件生成 SQL WHERE 子句
	long get_search_where(SEARCH_REQ req, string& sql);

	/// @brief 获取分类中的图片数量信息
	/// @param[in] cat_id 分类 ID
	/// @param[out] min 最小图片 ID（输出）
	/// @param[out] max 最大图片 ID（输出）
	/// @param[out] nb_images 图片数量（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取指定分类中的图片统计信息
	long get_category_images(string cat_id, uint64_t& min, uint64_t& max, uint32_t& nb_images);
	
	/// @brief 获取重复图片 ID 列表
	/// @param[out] lstImageIds 图片 ID 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 根据 MD5 校验和查找重复的图片
	long get_duplicates(list<string>& lstImageIds);

	/// @brief 获取图片 ID 列表（简单查询）
	/// @param[in] req 查询请求对象
	/// @param[out] lstImageIds 图片 ID 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 根据查询条件获取图片 ID 列表（简化版）
	long get_image_ids(QUERY_REQ req, list<string>& lstImageIds);

public:
	/// @brief 获取图片滑块筛选值范围
	/// @param[out] sliderValues 滑块值范围 DTO（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取用于滑块筛选的最小值和最大值（如尺寸、时间等）
	long get_images_slider_values(SLIDER_VALUES& sliderValues);

	/// @brief 获取图片所属分类 ID 列表
	/// @param[in] image_ids 图片 ID 列表（逗号分隔）
	/// @param[out] lstcategoryid 分类 ID 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取指定图片所属的所有分类 ID
	long get_image_gategories(string image_ids, list<string>& lstcategoryid);

	/// @brief 获取图片完整信息
	/// @param[in] id 图片 ID
	/// @param[out] photo_dto 图片信息 DTO（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取图片的完整信息（元数据、标签、分类等）
	long get_photo(string id, PHOTO_DTO& photo_dto);
	
	/// @brief 生成日历视图 SQL 语句
	/// @param[in] req 查询请求对象
	/// @param[in] search_req 搜索请求对象
	/// @param[out] outsql 输出 SQL 语句（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 生成用于日历视图的 SQL 查询语句
	long get_image_cal_sql(QUERY_REQ req, SEARCH_REQ search_req, string& outsql);
	
	/// @brief 获取日历视图数据
	/// @param[in] sql SQL 查询语句
	/// @param[out] mapCal 日期-图片数量映射表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 根据 SQL 查询获取每日的图片数量
	long get_image_cal(string sql, map<uint32_t, uint32_t >& mapCal);
	
	/// @brief 生成搜索请求 SQL 语句
	/// @param[in] req 查询请求对象
	/// @param[in] search_req 搜索请求对象
	/// @param[in] type SQL 类型
	/// @return 生成的 SQL 语句字符串
	/// @note 生成用于搜索的 SQL 查询语句
	string get_search_req_sql(QUERY_REQ req, SEARCH_REQ search_req, int type);
	
	/// @brief 获取图片文件列表
	/// @param[in] pic_id 图片 ID
	/// @param[in] cat_id 分类 ID
	/// @param[out] lstFiles 文件列表（输出）
	/// @param[out] lstCoi COI 文件列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取指定图片的所有文件（原图、缩略图、COI 等）
	long get_files(string pic_id, string cat_id, list<string>& lstFiles, list<string>& lstCoi);

	/// @brief 获取孤立图片数量
	/// @param[out] nb 孤立图片数量（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 统计数据库中已不存在文件对应的记录数量
	long get_orphan_nb_photos(uint32_t& nb);

	/// @brief 获取随机图片
	/// @param[in] cat_id 分类 ID
	/// @param[in] curr_picid 当前图片 ID
	/// @param[out] dto 图片 DTO（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 从指定分类中随机获取一张图片（排除当前图片）
	long get_rand_images(string cat_id, string curr_picid, IMAGE_DTO& dto);
	
	/// @brief 填充图片 DTO（从数据库结果）
	/// @param[in] dbResult 数据库查询结果
	/// @param[in] nColumn 列数
	/// @param[in] idx 索引
	/// @param[out] dto 图片 DTO（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 从数据库查询结果中填充图片 DTO
	long full_image_dto(char** dbResult, int nColumn, int idx, IMAGE_DTO& dto);
	
	/// @brief 获取日历视图图片列表
	/// @param[in] req 查询请求对象
	/// @param[in] search_req 搜索请求对象
	/// @param[out] lstDay 每日图片列表 DTO（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取日历视图中每日的图片列表
	long get_cal_images(QUERY_REQ req, SEARCH_REQ search_req, list<DAY_IMAGES_DTO>& lstDay);
	
	/// @brief 获取按周分组的图片列表
	/// @param[in] req 查询请求对象
	/// @param[in] search_req 搜索请求对象
	/// @param[out] lstDay 每周图片列表 DTO（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取按周分组的图片列表
	long get_weekly_images(QUERY_REQ req, SEARCH_REQ search_req, list<WEEK_IMAGES_DTO>& lstDay);

	/// @brief 获取图片质量信息列表
	/// @param[in] imageids 图片 ID 列表（逗号分隔）
	/// @param[out] lstImage 模板 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取指定图片的质量评分、尺寸等信息
	long get_images_quality(string imageids, list<TEMPL_DTO>& lstImage);
	
	/// @brief 根据 ID 列表获取图片
	/// @param[in] imageids 图片 ID 列表（逗号分隔）
	/// @param[out] lstImage 图片 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 根据 ID 列表批量获取图片信息
	long get_images_by_ids(string imageids, list<IMAGE_DTO>& lstImage);
	
	/// @brief 根据 ID 列表获取图片-分类映射
	/// @param[in] imageids 图片 ID 列表（逗号分隔）
	/// @param[out] mapCat 图片 ID-分类 ID 映射表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取图片与其分类的映射关系
	long get_images_group(string imageids, map<string, uint32_t>& mapCat);
	
	/// @brief 获取有人脸的图片 ID 列表
	/// @param[out] lstId 图片 ID 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取包含人脸的图片 ID 列表
	long get_images_face(list<string>& lstId);

	/// @brief 根据随机条件获取图片
	/// @param[in] lstSub 子分类列表
	/// @param[out] lstImage 图片 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 根据随机条件获取图片（用于随机浏览功能）
	long get_images_by_rnd(list<CATEGORY_DTO> lstSub, list<IMAGE_DTO>& lstImage);
	
	/// @brief 根据 ID 列表获取所有图片（带完整信息）
	/// @param[in] imageids 图片 ID 列表（逗号分隔）
	/// @param[out] plstImageIds 图片 ID 列表指针（输出）
	/// @param[out] lstImage 图片 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 根据 ID 列表获取图片的完整信息
	long get_images_all_ids(string imageids, list<string>* plstImageIds, list<IMAGE_DTO>& lstImage);
	
	/// @brief 根据分类 ID 获取图片（分页）
	/// @param[in] catid 分类 ID
	/// @param[in] page 页码（从 1 开始）
	/// @param[out] lstImage 图片 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取指定分类中的图片（支持分页）
	long get_images_by_cats(string catid, int page, list<IMAGE_DTO>& lstImage);
	
	/// @brief 获取所有图片 ID 映射
	/// @param[out] lstImageIds 图片 ID 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取数据库中所有图片的 ID 列表
	long get_images_all_map(list<string>& lstImageIds);
	
	/// @brief 获取最近访问的分类 ID 列表
	/// @param[out] ids 分类 ID 列表字符串（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取用户最近访问过的分类 ID 列表
	long get_recent_cats(string& ids);
	
	/// @brief 根据标签 ID 获取标签列表
	/// @param[in] tag_id 标签 ID
	/// @param[out] lstTag 标签 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取指定标签 ID 的标签信息
	long get_tags(string tag_id, list<TAG_DTO>& lstTag);

public:
	/// @brief 获取分类图片数量
	/// @param[out] nb_cat_images 分类图片数量（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取所有分类的图片总数
	long get_nb_cat_images(uint32_t& nb_cat_images);
	
	/// @brief 获取 SQLite 错误信息字符串
	/// @param[in] err SQLite 错误码
	/// @return 错误信息字符串
	/// @note 将 SQLite 错误码转换为可读的错误信息
	string get_sqlite_error_string(int err);
	
	/// @brief 设置相册名称
	/// @param[in] alb 相册名称
	/// @return 0: 成功; 其他: 错误码
	/// @note 设置当前操作的相册名称
	long set_alb(string alb);

private:
	/// @brief 生成查询请求 SQL 语句
	/// @param[in] req 查询请求对象
	/// @param[in] type SQL 类型
	/// @return 生成的 SQL 语句字符串
	/// @note 根据查询请求生成 SQL 查询语句
	string get_query_req_sql(QUERY_REQ req, int type);
	
	/// @brief 获取图片排序 SQL 片段
	/// @param[in] image_order 图片排序方式
	/// @return 排序 SQL 片段字符串
	/// @note 生成 ORDER BY 子句
	string get_image_order(string image_order);
	
	/// @brief 从字符串解析时间
	/// @param[in] datestr 时间字符串（如 "2024-06-21 15:30:00"）
	/// @return 时间戳（秒）
	/// @note 将时间字符串转换为时间戳
	uint64_t get_time_from_string(string datestr);
	
	/// @brief 填充图片 DTO 列表（从数据库结果）
	/// @param[in] dbResult 数据库查询结果
	/// @param[in] nRow 行数
	/// @param[in] nColumn 列数
	/// @param[out] lstImage 图片 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 从数据库查询结果中填充图片 DTO 列表
	long full_lstImage(char** dbResult, int nRow, int nColumn, list<IMAGE_DTO>& lstImage);

	/// @brief 打印 SQL 错误信息
	/// @param[in] func 函数名称
	/// @param[in] zSQL SQL 语句
	/// @param[in] errmsg 错误信息
	/// @return 0: 成功; 其他: 错误码
	/// @note 打印 SQL 执行错误的详细信息
	long print_sql_msg(char* func, char* zSQL, char* errmsg);
	
	/// @brief 打印 SQL 错误信息（字符串版本）
	/// @param[in] func 函数名称
	/// @param[in] sql SQL 语句字符串
	/// @param[in] errmsg 错误信息
	/// @return 0: 成功; 其他: 错误码
	/// @note 打印 SQL 执行错误的详细信息（字符串参数版本）
	long print_sql2_msg(char* func, string sql, char* errmsg);

	/// @brief 生成查询请求 SQL 语句（日历视图）
	/// @param[in] req 查询请求对象
	/// @return 生成的 SQL 语句字符串
	/// @note 生成用于日历视图的 SQL 查询语句
	string get_query_req_sql_cal(QUERY_REQ req);

	/// @brief 填充图片 DTO 列表（从数据库结果，指定 ID）
	/// @param[in] imageids 图片 ID 列表（逗号分隔）
	/// @param[in] dbResult 数据库查询结果
	/// @param[in] nRow 行数
	/// @param[in] nColumn 列数
	/// @param[out] lstImage 图片 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 从数据库查询结果中填充指定 ID 的图片 DTO 列表
	long full_lstImage2(string imageids, char** dbResult, int nRow, int nColumn, list<IMAGE_DTO>& lstImage);
	
	/// @brief 填充图片 DTO 列表（从数据库结果，指定 ID，输出 ID 列表）
	/// @param[in] imageids 图片 ID 列表（逗号分隔）
	/// @param[out] plstImageIds 图片 ID 列表指针（输出）
	/// @param[in] dbResult 数据库查询结果
	/// @param[in] nRow 行数
	/// @param[in] nColumn 列数
	/// @param[out] lstImage 图片 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 从数据库查询结果中填充指定 ID 的图片 DTO 列表，并输出 ID 列表
	long full_lstImage3(string imageids, list<string>* plstImageIds, char** dbResult, int nRow, int nColumn, list<IMAGE_DTO>& lstImage);
	
	/// @brief 获取时区偏移字符串
	/// @param[in] time_zone_bias 时区偏移（秒）
	/// @return 时区偏移字符串（如 "+08:00"）
	/// @note 将时区偏移转换为字符串格式
	string get_bias(long time_zone_bias);
	
	/// @brief 获取最后导入时间
	/// @param[out] t 最后导入时间戳（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取最后一次图片导入的时间
	long get_last_import_time(uint64_t& t);

private:
	/// @brief 排序字段缓存
	string _orderby = "";
	
	/// @brief SQLite 数据库句柄指针
	sqlite3* _pDB;
	
	/// @brief 可见性 SQL WHERE 子句（基础）
	string _visible0_ = " where 1=1 ";
	
	/// @brief 可见性条件 0
	string _v0 = "";
	
	/// @brief 可见性条件 1
	string _v1 = "";
	
	/// @brief 模块路径
	string _modulePath = "";

	/// @brief 图片 ID 查询 SQL 缓存
	string _image_ids_sql = "";
	
	/// @brief 过滤 SQL 缓存
	string _filter_sql = "";
};
