///////////////////////////////////////////////////////////////////////////////
// 文件名: HtmlgetIndexAction.h
// 模块: scheme/scheme
// 功能描述: 首页操作按钮和导航生成类
//           生成分类操作按钮、排序框、缩略图显示等 HTML
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-04-10  PhotoNest Team  添加缩略图分类显示功能
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

/// @brief 首页操作按钮和导航生成类
/// @details 生成首页的各种操作按钮（显示模式切换、排序、分类导航等），
///          以及缩略图显示区域的 HTML 内容。
///          继承自 CImBase 和 Singleton 模式。
/// @note 此类主要负责生成首页的操作界面元素
/// @warning 确保生成的 HTML 经过转义，防止 XSS 攻击
class CHtmlgetIndexAction : public CImBase, public Singleton<CHtmlgetIndexAction>
{
	friend class Singleton<CHtmlgetIndexAction>;

protected:
	/// @brief 构造函数
	/// @details 初始化首页操作生成对象
	CHtmlgetIndexAction(void);
	
	/// @brief 析构函数
	/// @details 清理资源
	~CHtmlgetIndexAction(void);

public:
	/// @brief 设置显示模式
	/// @param[in] display 显示模式（"thumb", "list", "detail" 等）
	/// @return 0: 成功; 其他: 错误码
	/// @note 显示模式影响缩略图的显示方式
	long set_display(string display);

	/// @brief 获取分类操作按钮 HTML
	/// @param[in] ty 类型（0: 顶部按钮, 1: 侧边栏按钮）
	/// @param[in] lst 分类列表
	/// @param[in] nb_images 图片总数
	/// @param[in] req 查询请求对象
	/// @param[out] switchBoxSizeUrl 尺寸切换 URL
	/// @param[out] switchBoxSortUrl 排序切换 URL
	/// @return 操作按钮 HTML 字符串
	/// @note 生成分类页面的操作按钮（上传、删除、排序等）
	string get_categoryActions(int ty, list<CATEGORY_DTO> lst, uint32_t nb_images, const QUERY_REQ req, string& switchBoxSizeUrl, string& switchBoxSortUrl);

	/// @brief 获取排序选择框 HTML
	/// @param[in] lst 分类列表
	/// @param[in] req 查询请求对象
	/// @param[in] args 额外参数
	/// @return 排序选择框 HTML 字符串
	/// @note 生成排序方式选择下拉框
	string get_sortOrderBox(list<CATEGORY_DTO> lst, QUERY_REQ req, string args);

	/// @brief 获取缩略图分类导航 HTML
	/// @param[in] type 类型（"thumb", "list" 等）
	/// @param[in] cat_id 分类 ID
	/// @param[in] lstCat 分类列表
	/// @return 分类导航 HTML 字符串
	/// @note 生成缩略图上方的小分类导航
	string get_thumbnailCategories(string type, string cat_id, list<CATEGORY_DTO> lstCat);

	/// @brief 获取缩略图 HTML
	/// @param[in] req 查询请求对象
	/// @param[in] lstImageDto 图片 DTO 列表
	/// @param[in] orderDto 排序 DTO
	/// @param[in] lst 分类列表
	/// @return 缩略图 HTML 字符串
	/// @note 生成图片缩略图网格显示
	string get_thumbnails(QUERY_REQ req, list<IMAGE_DTO> lstImageDto, ORDER_DTO orderDto, list<CATEGORY_DTO> lst);

	/// @brief 获取最大高度
	/// @return 最大高度（像素）
	/// @note 用于缩略图显示尺寸计算
	uint32_t get_maxHeight();

	/// @brief 获取最大宽度
	/// @return 最大宽度（像素）
	/// @note 用于缩略图显示尺寸计算
	uint32_t get_maxWidth();

	/// @brief 获取操作 URL
	/// @param[in] req 查询请求对象
	/// @return 操作 URL 字符串
	/// @note 生成各种操作的 URL 参数
	string get_cation_url(const QUERY_REQ req);

private:
	/// @brief 获取显示模式切换框 HTML
	/// @param[in] display 当前显示模式
	/// @return 显示模式切换框 HTML 字符串
	/// @note 生成缩略图/列表/详情等显示模式切换按钮
	string get_derivativeSwitchBox(string display);

	/// @brief 获取完整分类名称
	/// @param[in] full_id 完整分类 ID（包含父分类路径）
	/// @param[in] lstCat 分类列表
	/// @param[out] fullname 完整分类名称（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 根据分类 ID 获取包含父分类的完整名称
	long get_fullname(string full_id, list<CATEGORY_DTO> lstCat, string& fullname);

private:
	/// @brief 缩略图最大宽度
	uint32_t _maxWidth = 144;

	/// @brief 缩略图最大高度
	uint32_t _maxHeight = 144;
};
