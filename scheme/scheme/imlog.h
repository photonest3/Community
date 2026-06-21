///////////////////////////////////////////////////////////////////////////////
// 文件名: imlog.h
// 模块: scheme/scheme
// 功能描述: 日志记录类
//           提供文件日志记录功能，支持多级别日志
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include "Singleton.h"

using namespace std;

/// @brief 日志记录类
/// @details 提供简单的文件日志记录功能，支持多级别日志（1: 信息, 2: 警告, 3: 错误）。
///          采用单例模式确保全局唯一实例。
/// @note 日志文件使用宽字符路径，支持中文文件名
/// @warning 确保日志文件路径可写，否则日志记录会失败
class Cimlog : public Singleton<Cimlog> 
{
	friend class Singleton<Cimlog>;

protected:
	/// @brief 构造函数
	/// @details 初始化日志对象
	Cimlog();
	
	/// @brief 析构函数
	/// @details 清理资源
	~Cimlog(void);
		
public:
	/// @brief 设置日志文件名
	/// @param[in] szLog 日志文件路径（宽字符）
	/// @note 设置日志输出文件路径
	/// @warning 确保在调用 LogToFile 之前设置日志文件名
	void setLogName(wstring szLog);
	
	/// @brief 记录日志到文件
	/// @param[in] text 日志内容字符串
	/// @param[in] lv 日志级别（1: 信息, 2: 警告, 3: 错误）
	/// @note 日志格式：[时间戳] [级别] 日志内容
	/// @warning 如果未设置日志文件名，此函数不会记录任何内容
	void LogToFile(string text, int lv = 1);
	
private:
	/// @brief 日志文件名（宽字符）
	wstring _szFilename;
};
