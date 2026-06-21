///////////////////////////////////////////////////////////////////////////////
// 文件名: HtmlgetAdminProc.h
// 模块: scheme/scheme
// 功能描述: 管理界面 HTML 生成处理类
//           生成管理界面的各个页面 HTML 内容
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

/// @brief 管理界面 HTML 生成处理类
/// @details 生成管理界面各个页面的 HTML 内容，包括：
///          - 分类列表页面
///          - 标签管理页面
///          - 图片属性页面
///          - 统计信息页面
///          - 配置管理页面
///          采用单例模式确保全局唯一实例。
/// @note 此类负责 HTML 生成，不包含业务逻辑
/// @warning 生成的 HTML 应进行 XSS 过滤
class CHtmlgetAdminProc : public CImBase, public Singleton<CHtmlgetAdminProc>
{
	friend class Singleton<CHtmlgetAdminProc>;
	
protected:
	/// @brief 构造函数
	/// @details 初始化 HTML 生成处理对象
	CHtmlgetAdminProc(void);
	
	/// @brief 析构函数
	/// @details 释放资源
	~CHtmlgetAdminProc(void);

public:
	// ========================================================================
	// 页面 HTML 生成方法
	// ========================================================================
	
	/// @brief 生成分类列表页面 HTML
	/// @details 生成分类列表的 HTML 内容，包括分类树、操作按钮等
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[in] cat_list_infos 分类列表信息（JSON 字符串）
	/// @param[out] tpl 输出参数，生成的 HTML 模板字符串
	/// @return 0: 成功; 其他: 错误码
	/// @note 支持分类树形结构展示
	long proc_cat_list_htm(const REQUEST_DTO request_dto, string cat_list_infos, string& tpl);
	
	/// @brief 生成页面头部 HTML
	/// @details 生成管理界面的头部 HTML（导航栏、用户信息、统计信息等）
	/// @param[in] page 页面名称
	/// @param[out] nbCaddie 输出参数，标签篮数量
	/// @param[out] nbDuplicates 输出参数，重复图片数量
	/// @param[out] nbOrphan 输出参数，孤立图片数量
	/// @param[out] tpl 输出参数，生成的 HTML 模板字符串
	/// @return 0: 成功; 其他: 错误码
	long proc_head_htm(string page, uint32_t &nbCaddie, uint32_t &nbDuplicates, uint32_t &nbOrphan, string& tpl);

	/// @brief 生成标签管理页面 HTML
	/// @details 生成标签管理界面的 HTML 内容
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[out] tpl 输出参数，生成的 HTML 模板字符串
	/// @return 0: 成功; 其他: 错误码
	long proc_tag_htm(const REQUEST_DTO request_dto, string& tpl);
	
	/// @brief 生成图片属性页面 HTML
	/// @details 生成图片详细信息的 HTML 内容（EXIF、标签、分类等）
	/// @param[in] page 页面名称
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[in] photo_infos 图片信息（JSON 字符串）
	/// @param[out] tpl 输出参数，生成的 HTML 模板字符串
	/// @return 0: 成功; 其他: 错误码
	long proc_photo_htm(string page, const REQUEST_DTO request_dto, string photo_infos, string& tpl);
	
	/// @brief 生成统计信息页面 HTML
	/// @details 生成统计信息的 HTML 内容（导入历史、存储用量等）
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[in] photo_infos 图片信息（JSON 字符串）
	/// @param[out] tpl 输出参数，生成的 HTML 模板字符串
	/// @return 0: 成功; 其他: 错误码
	long proc_stats_htm(const REQUEST_DTO request_dto, string photo_infos, string& tpl);

	/// @brief 生成属性编辑页面 HTML
	/// @details 生成图片或分类属性编辑界面的 HTML 内容
	/// @param[in] page 页面名称
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[in] cat_infos 分类信息（JSON 字符串）
	/// @param[out] tpl 输出参数，生成的 HTML 模板字符串
	/// @return 0: 成功; 其他: 错误码
	long proc_properties_htm(string page, const REQUEST_DTO request_dto, string cat_infos, string& tpl);
	
	/// @brief 生成分类名称编辑页面 HTML
	/// @details 生成分类名称编辑界面的 HTML 内容
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[in] cat_infos 分类信息（JSON 字符串）
	/// @param[out] tpl 输出参数，生成的 HTML 模板字符串
	/// @return 0: 成功; 其他: 错误码
	long proc_cat_name_htm(const REQUEST_DTO request_dto, string cat_infos, string& tpl);
	
	/// @brief 移动分类
	/// @details 将一个或多个分类移动到新的父分类下
	/// @param[in] cat_id 要移动的分类 ID
	/// @param[in] parent 新的父分类 ID
	/// @return 0: 成功; 其他: 错误码
	/// @note 移动后会自动更新分类的全路径信息
	long move_categories(string cat_id, string parent);
	
	/// @brief 生成配置管理页面 HTML
	/// @details 生成系统配置管理界面的 HTML 内容
	/// @param[in] page 页面名称
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[in] config_infos 配置信息（JSON 字符串）
	/// @param[out] tpl 输出参数，生成的 HTML 模板字符串
	/// @return 0: 成功; 其他: 错误码
	long proc_config_htm(string page, const REQUEST_DTO request_dto, string config_infos, string& tpl);

	/// @brief 获取时间差字符串
	/// @details 将时间戳转换为"多久前"的友好显示字符串
	/// @param[in] date_available 时间戳（秒）
	/// @return 友好显示字符串（如"3 分钟前", "2 小时前"）
	/// @note 支持多种时间单位（秒、分钟、小时、天、月、年）
	string get_ago_string(uint64_t date_available);
	
	/// @brief 生成分类树 UL 列表 HTML
	/// @details 递归生成分类树的 HTML 无序列表
	/// @param[in] lst 分类列表
	/// @param[in] parent_id 父分类 ID
	/// @param[out] categoryUl 输出参数，生成的 UL 列表 HTML
	/// @return 0: 成功; 其他: 错误码
	/// @note 使用递归生成嵌套的 UL/LI 结构
	long get_categoryUl(list<CATEGORY_DTO> lst, string parent_id, string& categoryUl);
	
	/// @brief 生成分类标题
	/// @details 根据分类列表生成分类标题（面包屑导航）
	/// @param[in] lst 分类列表
	/// @param[in] type 标题类型（"full"=完整路径, "short"=短名称）
	/// @param[in] parent_id 父分类 ID
	/// @param[in] cat_id 当前分类 ID
	/// @param[out] titlename 输出参数，生成的标题字符串
	/// @return 0: 成功; 其他: 错误码
	long get_category_title(list<CATEGORY_DTO> lst, string type, string parent_id, string cat_id, string& titlename);
	
	/// @brief 生成分类选项 HTML
	/// @details 生成分类选择下拉框的 OPTION 列表
	/// @param[in] ty 分类类型
	/// @param[in] lst 分类列表
	/// @param[in] parent_id 父分类 ID
	/// @param[in] cat_id 当前分类 ID（用于标记选中状态）
	/// @param[out] options 输出参数，生成的 OPTION 列表 HTML
	/// @return 0: 成功; 其他: 错误码
	long get_category_options(string ty, list<CATEGORY_DTO> lst, string parent_id, string cat_id, string& options);

private:
	/// @brief 生成图片排序选项 HTML
	/// @details 生成图片排序方式选择的下拉框 HTML
	/// @param[in] image_order_tpl 排序选项模板 HTML
	/// @param[in] image_order 当前排序方式
	/// @return 生成的排序选项 HTML 字符串
	/// @note 支持多种排序方式（名称、时间、大小、自定义等）
	string get_image_order_htm(string image_order_tpl, string image_order);
	
	/// @brief 生成排序选择选项
	/// @details 生成排序方式选择的 OPTION 列表（标记选中状态）
	/// @param[in] orderby 当前排序方式
	/// @param[in] val 选项值
	/// @return 生成的 OPTION HTML（包含 selected 属性）
	string get_sel_opt(string orderby, string val);
	
	/// @brief 生成统计标题
	/// @details 根据年、月、日参数生成统计页面的标题
	/// @param[in] year 年份（空表示全部）
	/// @param[in] month 月份（空表示全部）
	/// @param[in] day 日期（空表示全部）
	/// @param[out] title 输出参数，生成的标题字符串
	/// @return 0: 成功; 其他: 错误码
	long get_stats_title(string year, string month, string day, string& title);

	/// @brief 获取星期名称
	/// @details 根据索引获取星期名称（支持多语言）
	/// @param[in] i 星期索引（0=周日, 1=周一, ...）
	/// @param[in] weekstart 一周起始日（0=周日, 1=周一）
	/// @return 星期名称字符串
	/// @note 根据界面语言返回对应翻译
	string get_day_name(int i, int weekstart);
	
	/// @brief 获取月份名称
	/// @details 根据索引获取月份名称（支持多语言）
	/// @param[in] i 月份索引（0=一月, 1=二月, ...）
	/// @return 月份名称字符串
	/// @note 根据界面语言返回对应翻译
	string get_month_name(int i);
	
	/// @brief 计算宽度像素值
	/// @details 根据页面数量和最大页面数计算进度条宽度
	/// @param[in] nb_pages 当前页面数量
	/// @param[in] nb_max 最大页面数
	/// @return 宽度像素值字符串（如"50px"）
	/// @note 用于生成统计页面的进度条
	string get_width_px(uint32_t nb_pages, uint32_t nb_max);
};
