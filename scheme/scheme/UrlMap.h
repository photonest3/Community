///////////////////////////////////////////////////////////////////////////////
// 文件名: UrlMap.h
// 模块: scheme/scheme
// 功能描述: URL 映射类
//           将 HTTP 请求 URL 映射到对应的处理函数 ID
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-03-15  PhotoNest Team  添加 URL 映射功能
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"

/// @brief URL 映射类
/// @details 将 HTTP 请求 URL 映射到对应的处理函数 ID。
///          使用 map 存储 URL 到处理 ID 的映射关系。
///          采用单例模式确保全局唯一实例。
/// @note 此类是 HTTP 请求分发的核心
/// @warning 确保 URL 映射完整，避免请求无法处理
class CUrlMap : public Singleton<CUrlMap>
{
	friend class Singleton<CUrlMap>;

protected:
	/// @brief 构造函数
	/// @details 初始化 URL 映射对象，分配映射表内存
	CUrlMap(void);
	
	/// @brief 析构函数
	/// @details 清理资源，删除映射表
	~CUrlMap(void);

public:
	/// @brief 将 URL 映射到处理函数 ID
	/// @param[in] url URL 字符串
	/// @return 处理函数 ID（UINT），失败返回 0
	/// @note 根据 URL 查找对应的处理函数 ID
	/// @warning 如果 URL 未注册，返回 0
	UINT mapUrl2Int(string url);

	/// @brief 初始化 URL 映射表
	/// @return 0: 成功; 其他: 错误码
	/// @note 初始化所有 URL 到处理函数的映射关系
	/// @warning 确保在处理请求前调用此函数
	long Init_url_map();

private:
	/// @brief URL 到处理函数 ID 的映射表指针
	/// @note 使用 map<string, UINT> 存储映射关系
	REQ_URL_MAP * _pUrl_map;
	
};
