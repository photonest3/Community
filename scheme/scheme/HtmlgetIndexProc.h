///////////////////////////////////////////////////////////////////////////////
// 文件名: HtmlgetIndexProc.h
// 模块: scheme/scheme
// 功能描述: 首页核心处理逻辑类
//           处理图片列表、分页、缓存、搜索等核心业务
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-05-15  PhotoNest Team  添加图片缓存机制
//   2024-06-01  PhotoNest Team  优化分页查询性能
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

/// @brief 首页核心处理逻辑类
/// @details 处理首页的核心业务逻辑，包括图片列表查询、分页处理、
///          缓存管理、搜索处理、图片操作（编辑、缩放、胶片化）等。
///          继承自 CImBase 和 Singleton 模式。
/// @note 此类是首页功能的核心处理类，包含大量业务逻辑
/// @warning 缓存操作需要加锁，防止多线程冲突
class CHtmlgetIndexProc : public CImBase, public Singleton<CHtmlgetIndexProc>
{
	friend class Singleton<CHtmlgetIndexProc>;

protected:
	/// @brief 构造函数
	/// @details 初始化首页核心处理对象，初始化临界区
	CHtmlgetIndexProc(void);
	
	/// @brief 析构函数
	/// @details 清理资源，删除临界区
	~CHtmlgetIndexProc(void);

public:
	/// @brief 清理缓存数据
	/// @return 0: 成功; 其他: 错误码
	/// @note 清理图片列表、导航数据等缓存
	long clear_data();
	
	/// @brief 处理图片编辑请求
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 生成图片编辑界面 HTML
	long proc_art_edit(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 处理图片胶片化请求
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 将图片添加到胶片中
	long proc_art_film(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 处理图片缩放请求
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 生成图片缩放查看界面
	long proc_art_zoom(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 更新图片标题
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 更新图片的标题和描述信息
	long update_art_title(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 处理图片列表请求
	/// @param[in] szArgs 请求参数
	/// @return 图片列表 HTML 字符串
	/// @note 生成图片列表的 JSON 数据
	string proc_art_list(string szArgs);
	
	/// @brief 处理用户资料页面请求
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 生成用户资料编辑页面
	long proc_profile_htm(REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 处理所有标签列表请求
	/// @param[in] szArgs 请求参数
	/// @param[in] lstTag 标签列表指针
	/// @return 标签列表 HTML 字符串
	/// @note 生成所有可用标签的列表
	string proc_tags_all(string szArgs, list<TAG_DTO>* lstTag);
	
	/// @brief 处理缩略图页面 HTML 生成
	/// @param[in] lst 分类列表
	/// @param[in] req 查询请求对象
	/// @param[in] orderDto 排序 DTO
	/// @param[out] nb_images 图片总数（输出）
	/// @param[out] tpl 生成的 HTML 模板（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 生成缩略图页面的完整 HTML
	long proc_tumb_htm(list<CATEGORY_DTO> lst, QUERY_REQ req, ORDER_DTO orderDto, uint32_t& nb_images, string& tpl);
	
	/// @brief 更新图片缓存
	/// @param[in] id 图片 ID
	/// @param[in] imageDto 图片 DTO
	/// @return 0: 成功; 其他: 错误码
	/// @note 更新指定图片的缓存信息
	long update_cache_image(string id, IMAGE_DTO imageDto);
	
	/// @brief 批量推送图片到缓存
	/// @param[in] lstImage 图片 DTO 列表
	/// @return 0: 成功; 其他: 错误码
	/// @note 批量更新图片缓存，提升访问性能
	long push_cache_images(list<IMAGE_DTO> lstImage);
	
	/// @brief 获取最大页数
	/// @param[in] req 查询请求对象
	/// @param[out] maxPage 最大页数（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 根据图片总数和每页显示数量计算最大页数
	long get_max_page(QUERY_REQ req, uint32_t& maxPage);
	
	/// @brief 获取初始化页面数据
	/// @param[in] req 查询请求对象
	/// @param[out] lstDto 图片 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取首页初始化需要的图片数据
	long get_init_page(QUERY_REQ req, list<IMAGE_DTO>& lstDto);
	
	/// @brief 获取导航数据
	/// @param[in] id 图片 ID
	/// @param[out] data 导航数据 JSON 字符串（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 获取图片导航所需的数据（上一张/下一张等）
	long get_nav_data(string id, string& data);
	
	/// @brief 处理图片评论提交
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 处理用户提交的图片评论
	long post_img_say(REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 处理绘画/标注请求
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 生成绘画/标注工具界面
	long show_paint(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 获取操作按钮 HTML
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 生成图片操作按钮（编辑、删除、收藏等）
	long get_action(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 计算图片 MD5 校验和
	/// @param[in] artids 图片 ID 列表（逗号分隔）
	/// @param[out] lstOrder 图片 DTO 列表（输出）
	/// @return MD5 校验和
	/// @note 用于图片去重和完整性校验
	uint64_t get_md5sum(string artids, list<IMAGE_DTO>& lstOrder);
	
	/// @brief 获取缓存数据
	/// @param[in] ty 缓存类型
	/// @param[out] lstDto 模板 DTO 列表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 从缓存中获取模板数据
	long get_cache(int ty, list<TEMPL_DTO>& lstDto);
	
	/// @brief 更新缓存数据
	/// @param[in] lstQuality 模板 DTO 列表
	/// @return 0: 成功; 其他: 错误码
	/// @note 批量更新模板缓存数据
	long update_cache(list<TEMPL_DTO> lstQuality);

private:
	/// @brief 获取横幅 HTML
	/// @return 横幅 HTML 字符串
	/// @note 生成页面顶部横幅区域
	string get_banner();

	/// @brief 获取页面标题
	/// @param[in] req 查询请求对象
	/// @return 页面标题字符串
	/// @note 根据请求参数生成页面标题
	string get_title(const QUERY_REQ req);

	/// @brief 获取图片尺寸
	/// @param[in] dto 图片 DTO
	/// @param[out] w 宽度（输出）
	/// @param[out] h 高度（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 从图片 DTO 中解析图片尺寸
	long get_size(IMAGE_DTO dto, uint32_t& w, uint32_t& h);

	/// @brief 创建路径
	/// @param[in,out] title1 标题字符串（可能被修改）
	/// @note 根据标题创建文件存储路径
	void mk_path(string& title1);

private:
	/// @brief 图片 DTO 列表缓存
	list<IMAGE_DTO> _lstImageDto;

	/// @brief 临界区对象，用于多线程同步
	CRITICAL_SECTION _cs;
};
