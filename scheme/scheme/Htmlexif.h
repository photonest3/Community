///////////////////////////////////////////////////////////////////////////////
// 文件名: Htmlexif.h
// 模块: scheme/scheme
// 功能描述: EXIF 信息提取类
//           从图像文件中提取 EXIF 元数据信息
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ImBase.h"
#include "stmdef.h"
#include "Singleton.h"
#include "exif.h"

/// @brief EXIF 信息提取类
/// @details 从图像文件（JPEG、TIFF 等）中提取 EXIF 元数据，
///          包括拍摄时间、相机型号、曝光参数、GPS 信息等。
///          继承自 CImBase 和 Singleton 模式。
/// @note 使用开源 exif.h 库解析 EXIF 数据
/// @warning 确保输入文件是有效的图像文件，避免解析错误
class CHtmlexif : public CImBase, public Singleton<CHtmlexif>
{
	friend class Singleton<CHtmlexif>;

protected:
	/// @brief 构造函数
	/// @details 初始化 EXIF 提取对象
	CHtmlexif(void);
	
	/// @brief 析构函数
	/// @details 清理资源
	~CHtmlexif(void);

public:
	/// @brief 获取指定类型的 EXIF 值
	/// @param[in] filename 图像文件路径（宽字符）
	/// @param[in] type EXIF 类型（如 "Make", "Model", "DateTime" 等）
	/// @param[out] attr 输出的 EXIF 属性映射表
	/// @return 0: 成功; 其他: 错误码
	/// @note 支持的 EXIF 类型请参考 EXIF 标准
	long get_exif_value(wstring filename, string type, map<string, string> &attr);
	
	/// @brief 获取图像文件的完整 EXIF 信息
	/// @param[in] szSrc 图像文件路径（宽字符）
	/// @param[out] result EXIF 信息结构体
	/// @return 0: 成功; 其他: 错误码
	/// @note 返回的 EXIFInfo 结构体包含完整的 EXIF 信息
	/// @see EXIFInfo 结构体定义
	long get_exif(wstring szSrc, EXIFInfo &result);

};
