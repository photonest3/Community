///////////////////////////////////////////////////////////////////////////////
// 文件名: HtmlgetIndex.h
// 模块: scheme/scheme
// 功能描述: 首页和导航页面请求处理类
//           处理首页、注册页、登录页、皮肤切换等请求
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-03-20  PhotoNest Team  添加皮肤切换功能
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"

/// @brief 首页和导航页面请求处理类
/// @details 处理首页（索引页）、注册页、登录页、皮肤切换等 HTTP 请求，
///          生成对应的 HTML 响应内容。继承自 CImBase 和 Singleton 模式。
/// @note 此类主要处理页面框架和导航相关的请求
/// @warning 确保请求参数经过验证，防止 XSS 攻击
class CHtmlgetIndex : public CImBase, public Singleton<CHtmlgetIndex>
{
	friend class Singleton<CHtmlgetIndex>;

protected:
	/// @brief 构造函数
	/// @details 初始化首页请求处理对象
	CHtmlgetIndex(void);
	
	/// @brief 析构函数
	/// @details 清理资源
	~CHtmlgetIndex(void);

public:
	/// @brief 获取首页 HTML
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 根据请求参数生成首页 HTML 内容
	long get_index_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 获取后台管理首页 HTML
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	long get_b_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 获取注册页面 HTML
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	long get_register_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 获取登录页面 HTML
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	long get_login_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 获取通用页面 HTML
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	long get_page_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 获取皮肤列表 HTML
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 返回可用的皮肤主题列表
	long get_skins_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 获取 JavaScript 文件内容
	/// @param[in] id JS 文件 ID
	/// @param[in] js JS 文件路径（宽字符）
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 用于动态加载 JavaScript 文件
	long get_js(UINT id, wstring js, RESPONSE_DTO& response_dto);
	
	/// @brief 获取 CSS 文件内容
	/// @param[in] id CSS 文件 ID
	/// @param[in] css CSS 文件路径（宽字符）
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 用于动态加载 CSS 文件
	long get_css(UINT id, wstring css, RESPONSE_DTO& response_dto);
	
	/// @brief 获取最后访问页面 HTML
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	long get_last_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);

public:
	/// @brief 处理首次访问首页
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @note 处理首次访问的初始化逻辑
	void proc_first_index(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 处理删除盒子操作
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	long proc_delbox(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);

private:
	/// @brief 获取首页模板
	/// @param[in] lst 分类列表
	/// @param[in] req 查询请求对象
	/// @param[in] num_tags 标签数量
	/// @param[out] menubar 菜单栏 HTML
	/// @param[out] tpl 首页模板 HTML
	/// @return 0: 成功; 其他: 错误码
	long get_index_template(list<CATEGORY_DTO> lst, const QUERY_REQ req, string num_tags, string& menubar, string& tpl);
	
	/// @brief 创建目录
	/// @param[in] path 目录路径（宽字符）
	/// @return TRUE: 成功; FALSE: 失败
	/// @note 递归创建目录（如果父目录不存在）
	BOOL CreateMyDirectory(wstring path);
	
	/// @brief 获取日期类型
	/// @param[in] req 查询请求对象
	/// @param[out] orderDto 排序数据传输对象
	/// @return 0: 成功; 其他: 错误码
	long get_dateType(QUERY_REQ req, ORDER_DTO& orderDto);
	
	/// @brief 侧边栏数字显示线程函数
	/// @param[in] pParam 线程参数
	/// @return 线程退出码
	/// @note 后台线程，用于更新侧边栏的数字统计
	static UINT __stdcall sb_nb(LPVOID pParam);

private:
	/// @brief 分类列表缓存
	list<CATEGORY_DTO> _lstCategory;
};
