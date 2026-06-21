///////////////////////////////////////////////////////////////////////////////
// 文件名: HtmlgetBatch.h
// 模块: scheme/scheme
// 功能描述: 批量操作界面 HTML 生成类
//           生成批量操作界面（批量标签、批量移动、批量删除等）
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

/// @brief 批量操作界面 HTML 生成类
/// @details 生成批量操作界面的 HTML 内容，包括：
///          - 批量标签添加/删除
///          - 批量移动到分类
///          - 批量删除
///          - 批量修改属性
///          采用单例模式确保全局唯一实例。
/// @note 批量操作会涉及多张图片，需注意性能
/// @warning 批量删除操作不可恢复，需确认提示
class CHtmlgetBatch : public CImBase, public Singleton<CHtmlgetBatch>
{
	friend class Singleton<CHtmlgetBatch>;
	
protected:
	/// @brief 构造函数
	/// @details 初始化批量操作界面处理对象
	CHtmlgetBatch(void);
	
	/// @brief 析构函数
	/// @details 释放资源
	~CHtmlgetBatch(void);

public:
	/// @brief 生成批量操作页面 HTML
	/// @details 生成批量操作界面的 HTML 内容
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[in] batch_infos 批量操作信息（JSON 字符串）
	/// @param[out] tpl 输出参数，生成的 HTML 模板字符串
	/// @return 0: 成功; 其他: 错误码
	long proc_batch_htm(REQUEST_DTO request_dto, string batch_infos, string& tpl);

private:
	/// @brief 获取图片标签字符串
	/// @details 生成指定图片的所有标签的 HTML 字符串
	/// @param[in] image_id 图片 ID
	/// @param[in] lstImageTag 图片标签列表
	/// @return 标签 HTML 字符串
	/// @note 用于批量标签操作的界面展示
	string get_tags_string(string image_id, list<IMAGE_TAG_DTO> lstImageTag);
	
	/// @brief 生成全局导航栏
	/// @details 生成批量操作界面的全局导航栏 HTML
	/// @param[in] ty 导航类型
	/// @param[in] per 每页显示数量
	/// @param[in] navigationBar 导航栏模板
	/// @param[out] navigationBar_global 输出参数，生成的导航栏 HTML
	/// @return 生成的导航栏 HTML 字符串
	string get_navigationBar_global(string ty, int per, string navigationBar, string& navigationBar_global);
	
	/// @brief 生成筛选请求 HTML
	/// @details 生成批量操作界面的筛选条件展示 HTML
	/// @param[in] lst 分类列表
	/// @param[in] filter 筛选条件字符串
	/// @param[out] tpl 输出参数，生成的 HTML 模板字符串
	/// @return 0: 成功; 其他: 错误码
	long proc_filter_req_htm(list<CATEGORY_DTO> lst, string filter, string& tpl);

public:
	/// @brief URL 参数字符串
	/// @details 存储当前请求的 URL 参数部分
	string _szArgs = "";
	
	/// @brief 图片数据 JSON 字符串
	/// @details 存储当前批量操作的图片数据（JSON 格式）
	string _picture_data = "";
	
	/// @brief 元素 ID 列表
	/// @details 存储当前批量操作的元素 ID 列表（逗号分隔）
	string _element_ids = "";
	
	/// @brief 筛选请求对象
	/// @details 存储当前批量操作的筛选条件
	SEARCH_REQ _f_req;
};
