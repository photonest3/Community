///////////////////////////////////////////////////////////////////////////////
// 文件名: HtmlgetPic.h
// 模块: scheme/scheme
// 功能描述: 图片查看页面请求处理类
//           处理图片查看、导航、收藏等请求
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-05-10  PhotoNest Team  添加收藏功能
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

/// @brief 图片查看页面请求处理类
/// @details 处理图片查看页面的 HTTP 请求，生成图片查看器的 HTML 响应内容。
///          支持图片导航、收藏、元数据显示等功能。
///          继承自 CImBase 和 Singleton 模式。
/// @note 此类处理单张图片的查看和导航逻辑
/// @warning 确保图片 ID 经过验证，防止路径遍历攻击
class CHtmlgetPic : public CImBase, public Singleton<CHtmlgetPic>
{
	friend class Singleton<CHtmlgetPic>;

protected:
	/// @brief 构造函数
	/// @details 初始化图片查看请求处理对象
	CHtmlgetPic(void);
	
	/// @brief 析构函数
	/// @details 清理资源
	~CHtmlgetPic(void);

public:
	/// @brief 获取图片查看页面 HTML
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 根据请求参数生成图片查看页面的完整 HTML
	long get_picture_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);

private:
	/// @brief 获取图片查看页面模板
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] tpl 页面模板 HTML
	/// @return 0: 成功; 其他: 错误码
	long get_picture_template(const REQUEST_DTO request_dto, string& tpl);
	
	/// @brief 获取当前图片 ID
	/// @param[in] request_dto 请求数据传输对象
	/// @return 图片 ID 字符串
	/// @note 从请求参数中解析当前图片 ID
	string get_picid(const REQUEST_DTO request_dto);
	
	/// @brief 获取当前图片在列表中的位置
	/// @param[in] plstImageIds 图片 ID 列表
	/// @param[in] idx0 当前索引
	/// @param[out] picid 当前图片 ID
	/// @param[out] idx 当前索引（输出）
	/// @param[out] bfav 是否收藏
	/// @return 0: 成功; 其他: 错误码
	/// @note 用于图片导航（上一张/下一张）
	long get_curr_images(list<string> plstImageIds, string idx0, string& picid, int& idx, BOOL& bfav);

};
