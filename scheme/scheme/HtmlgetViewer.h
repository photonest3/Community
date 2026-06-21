///////////////////////////////////////////////////////////////////////////////
// 文件名: HtmlgetViewer.h
// 模块: scheme/scheme
// 功能描述: 查看器页面请求处理类
//           处理查看器、删除盒子、缩略图文件等请求
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-05-10  PhotoNest Team  添加查看器功能
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

/// @brief 查看器页面请求处理类
/// @details 处理查看器页面的 HTTP 请求，生成查看器的 HTML 响应内容。
///          支持查看器数据管理、删除操作、缩略图文件访问等。
///          继承自 CImBase 和 Singleton 模式。
/// @note 此类处理查看器相关的所有请求
/// @warning 确保文件路径经过验证，防止路径遍历攻击
class CHtmlgetViewer : public CImBase, public Singleton<CHtmlgetViewer>
{
	friend class Singleton<CHtmlgetViewer>;

protected:
	/// @brief 构造函数
	/// @details 初始化查看器请求处理对象
	CHtmlgetViewer(void);
	
	/// @brief 析构函数
	/// @details 清理资源
	~CHtmlgetViewer(void);

public:
	/// @brief 获取查看器页面 HTML
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 生成查看器页面的完整 HTML
	long get_viewer_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 处理删除盒子操作
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 删除指定的查看器盒子
	long get_delbox(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 删除查看器
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @warning 此操作不可恢复
	long del_viewer(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 获取缩略图文件
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 返回缩略图文件内容（用于 AJAX 请求）
	long get_thumb_file(REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);

private:
	/// @brief 设置查看器数据
	/// @param[in] items 查看器项目列表字符串
	/// @param[out] history_file 历史文件路径（输出）
	/// @param[out] tpl 模板 HTML（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 从文件读取查看器数据并生成模板
	long set_viewer_data(string items, string& history_file, string& tpl);
	
	/// @brief 获取查看器项目列表
	/// @param[in] path0 查看器路径（宽字符）
	/// @param[out] items 项目列表字符串（输出）
	/// @param[out] idx 当前索引（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 读取查看器目录中的文件列表
	long get_viewer_items(wstring path0, string& items, int& idx);
	
	/// @brief 分割文件名
	/// @param[in] file_name 完整文件名（宽字符）
	/// @param[out] szDir 目录路径（输出）
	/// @param[out] szName 文件名（不含扩展名，输出）
	/// @param[out] szExt 扩展名（输出）
	/// @note 将完整路径分割为目录、文件名、扩展名
	void splitFile(wstring file_name, wstring& szDir, wstring& szName, wstring& szExt);

private:
	/// @brief 文件查找数据列表
	list<WIN32_FIND_DATA > _lstffd;
	
	/// @brief 查看器路径（宽字符）
	wstring _strPath = L"";
	
	/// @brief 目标路径（宽字符）
	wstring _dstP = L"";

};
