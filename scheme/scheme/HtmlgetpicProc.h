///////////////////////////////////////////////////////////////////////////////
// 文件名: HtmlgetpicProc.h
// 模块: scheme/scheme
// 功能描述: 图片查看处理类
//           处理图片元数据、幻灯片播放、图片详情等
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-05-20  PhotoNest Team  添加幻灯片播放功能
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

/// @brief 图片查看处理类
/// @details 处理图片查看页面的核心逻辑，包括 EXIF 元数据提取、
///          幻灯片播放、图片详情显示、图片源文件访问等。
///          继承自 CImBase 和 Singleton 模式。
/// @note 此类是图片查看功能的核心处理类
/// @warning 确保图片 ID 经过验证，防止路径遍历攻击
class CHtmlgetpicProc : public CImBase, public Singleton<CHtmlgetpicProc>
{
	friend class Singleton<CHtmlgetpicProc>;

protected:
	/// @brief 构造函数
	/// @details 初始化图片查看处理对象
	CHtmlgetpicProc(void);
	
	/// @brief 析构函数
	/// @details 清理资源
	~CHtmlgetpicProc(void);

public:
	/// @brief 获取图片 EXIF 元数据 JSON 字符串
	/// @param[in] url 图片 URL 或文件路径
	/// @return EXIF 元数据 JSON 字符串
	/// @note 提取图片的 EXIF 信息并转换为 JSON 格式
	/// @see CHtmlexif 类
	string get_meta_exif(string url);
	
	/// @brief 处理图片盒子显示 HTML
	/// @param[in] osm 显示模式（"box", "slide", 等）
	/// @param[in] from 起始索引
	/// @param[in] to 结束索引
	/// @param[out] tpl 生成的 HTML 模板（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 生成图片盒子显示区域的 HTML
	long proc_boxshow_htm(string osm, uint32_t from, uint32_t to, string& tpl);
	
	/// @brief 处理幻灯片播放 HTML
	/// @param[in] plstImageIds 图片 ID 列表
	/// @param[in] from 起始索引
	/// @param[in] to 结束索引
	/// @param[out] slider_nav 幻灯片导航 HTML（输出）
	/// @param[out] picture_content 图片内容 HTML（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 生成幻灯片播放界面的 HTML
	long proc_slide_htm(list<string> plstImageIds, uint32_t from, uint32_t to, string& slider_nav, string& picture_content);
	
	/// @brief 获取图片详情 HTML
	/// @param[in] type 详情类型（"exif", "iptc", "xmp" 等）
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 生成图片详细信息展示界面
	long get_details(string type, const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 获取图片源文件
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 返回图片原始文件（用于下载或查看原图）
	/// @warning 确保文件路径经过验证，防止路径遍历攻击
	long get_src(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 获取图片加载速度信息
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 返回图片加载速度、文件大小等信息
	long get_speed(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 获取图片所有 EXIF 信息
	/// @param[in] curr 当前图片 DTO
	/// @param[in] cat_name 分类名称
	/// @param[out] metadata 元数据 HTML（输出）
	/// @param[out] all 完整 EXIF 信息 HTML（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 生成图片 EXIF 信息的完整展示界面
	long get_all_exif(IMAGE_DTO curr, string cat_name, string& metadata, string& all);
};
