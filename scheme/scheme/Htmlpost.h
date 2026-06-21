///////////////////////////////////////////////////////////////////////////////
// 文件名: Htmlpost.h
// 模块: scheme/scheme
// 功能描述: POST 请求处理基类
//           处理 HTTP POST 请求，是所有 POST 处理类的基类
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"

/// @brief POST 请求处理基类
/// @details 处理 HTTP POST 请求，解析 POST 数据并生成响应。
///          是所有 POST 请求处理类的基类。
///          采用单例模式确保全局唯一实例。
/// @note 此类主要处理 POST 请求，GET 请求由 CHtmlget 类处理
/// @warning 确保 POST 数据经过验证，防止 SQL 注入和 XSS 攻击
class CHtmlpost : public Singleton<CHtmlpost> 
{
	friend class Singleton<CHtmlpost>;

protected:
	/// @brief 构造函数
	/// @details 初始化 POST 请求处理对象
	CHtmlpost(void);
	
	/// @brief 析构函数
	/// @details 清理资源
	~CHtmlpost(void);

public:
	/// @brief POST 请求处理主函数
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 解析 POST 数据，调用对应的处理函数，生成响应
	/// @warning 确保 POST 数据经过验证和转义
	long mypostfunc(REQUEST_DTO request_dto, RESPONSE_DTO &response_dto);
};
