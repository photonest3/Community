///////////////////////////////////////////////////////////////////////////////
// 文件名: Htmlget.h
// 模块: scheme/scheme
// 功能描述: 网页请求处理基类
//           处理 HTTP GET 请求，生成网页内容
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"

/// @brief 网页请求处理类
/// @details 处理 HTTP GET 请求，解析请求参数并生成对应的 HTML 响应。
///          采用单例模式确保全局唯一实例。
///          是所有网页请求处理类的基类。
/// @note 此类主要处理 GET 请求，POST 请求由子类处理
/// @warning 确保请求参数经过验证，防止 XSS 攻击
class CHtmlget : public Singleton<CHtmlget>
{
	friend class Singleton<CHtmlget>;
	
protected:
	/// @brief 构造函数
	/// @details 初始化请求处理对象
	CHtmlget(void);
	
	/// @brief 析构函数
	/// @details 释放资源
	~CHtmlget(void);

public:
	/// @brief 处理网页请求
	/// @details 解析 HTTP 请求，根据请求类型调用对应的处理方法，
	///          生成 HTML 响应内容。
	/// @param[in] request_dto HTTP 请求数据传输对象
	/// @param[out] response_dto HTTP 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 此方法是请求处理的入口点
	long mygetfunc(REQUEST_DTO &request_dto, RESPONSE_DTO &response_dto);

private:
	/// @brief 验证 URL 合法性
	/// @details 检查 URL 是否合法，防止路径遍历攻击。
	/// @param[in] val 要验证的 URL 字符串
	/// @param[out] szUrl 输出参数，验证后的 URL
	/// @param[out] szArgs 输出参数，URL 参数部分
	/// @return true: URL 合法; false: URL 不合法
	/// @note 会检查 URL 是否包含非法字符或路径遍历序列
	bool isurlok(string val, string& szUrl, string& szArgs);
	
	/// @brief 是否使用主 URL
	/// @details 判断当前请求是否应该使用主 URL（而非别名或短链接）
	/// @return true: 使用主 URL; false: 使用别名
	bool use_main_url();
	
	/// @brief 是否首次运行标记
	/// @details 标记应用是否首次运行，用于显示初始化向导
	bool _is_firstrun = false;
};
