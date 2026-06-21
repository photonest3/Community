///////////////////////////////////////////////////////////////////////////////
// 文件名: HtmlgetFolder.h
// 模块: scheme/scheme
// 功能描述: 文件夹浏览和处理类
//           浏览文件系统文件夹，获取图片文件列表
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "ImBase.h"
#include <shlobj.h>
//#include <regex>
#include <queue>

/// @brief 导入 scrrun.dll 中的文件系统对象
/// @details 使用 #import 导入 Windows Scripting Runtime 类型库，
///          提供文件夹和文件操作接口。
///          重命名了部分方法以避免命名冲突。
# import "scrrun.dll" raw_interfaces_only \
 rename("CopyFile", "ExclCopyFile") \
 rename("DeleteFile", "ExclDeleteFile") \
 rename("MoveFile", "ExclMoveFile") 

/// @brief 文件夹浏览和处理类
/// @details 浏览文件系统文件夹，获取图片文件列表，
///          支持递归遍历子文件夹。
///          提供文件夹大小计算、文件 CRC 校验等功能。
///          采用单例模式确保全局唯一实例。
/// @note 此类直接操作文件系统，需注意权限和性能
/// @warning 递归遍历深层文件夹可能导致性能问题
class CHtmlgetFolder : public CImBase, public Singleton<CHtmlgetFolder>
{
	friend class Singleton<CHtmlgetFolder>;
	
protected:
	/// @brief 构造函数
	/// @details 初始化文件夹浏览对象，设置支持的图片扩展名
	CHtmlgetFolder(void);
	
	/// @brief 析构函数
	/// @details 释放资源
	~CHtmlgetFolder(void);

public:
	/// @brief 获取子文件夹列表
	/// @details 获取指定路径下的所有子文件夹
	/// @param[in] ty 文件夹类型（0=普通, 1=系统等）
	/// @param[in] strPath 父文件夹路径
	/// @param[out] lstSubs 输出参数，子文件夹路径列表
	/// @return 0: 成功; 其他: 错误码
	/// @note 使用 Windows API 枚举文件夹
	long GetSubFolders(int ty, wstring strPath, list<wstring >& lstSubs);
	
	/// @brief 获取文件夹中的图片文件
	/// @details 获取指定文件夹中的所有图片文件（根据扩展名筛选）
	/// @param[in] cat_id 分类 ID（用于关联分类）
	/// @param[in] strPath 文件夹路径
	/// @param[in] crc CRC8 校验值（用于去重）
	/// @param[out] lstFile 输出参数，图片文件列表
	/// @return 0: 成功; 其他: 错误码
	/// @note 只获取支持的图片格式（jpg, png, gif, etc.）
	long GetFolderPics(string cat_id, wstring strPath, uint8_t crc, list<FILE_DTO >& lstFile);
	
	/// @brief 获取文件夹中的图片文件（重载）
	/// @details 使用 Windows API 枚举文件夹中的图片文件
	/// @param[in] ty 文件夹类型
	/// @param[in] strPath 文件夹路径
	/// @param[out] vecffd 输出参数，文件数据列表（WIN32_FIND_DATA）
	/// @return 0: 成功; 其他: 错误码
	long GetFolderPics(int ty, wstring strPath, list<WIN32_FIND_DATA >& vecffd);
	
	/// @brief 获取文件夹大小
	/// @details 计算指定根分类下所有文件夹的总大小
	/// @param[in] lstRoot 根分类列表
	/// @param[in] album 相册名称
	/// @param[out] fsize 输出参数，文件夹大小（字节）
	/// @return 0: 成功; 其他: 错误码
	long GetFolderSize(list<ROOT_DTO> lstRoot, string album, uint64_t& fsize);

	/// @brief 删除文件夹中的文件
	/// @details 删除指定文件夹中的所有文件（不包括子文件夹）
	/// @param[in] strPath 文件夹路径
	/// @return 0: 成功; 其他: 错误码
	/// @warning 此操作不可恢复
	long DelFolderFiles(wstring strPath);
	
	/// @brief 获取文件夹中的图片文件（队列版本）
	/// @details 将指定文件夹中的图片文件路径添加到队列
	/// @param[in] strPath 文件夹路径
	/// @param[out] lstFile 输出参数，图片文件路径队列
	/// @return 0: 成功; 其他: 错误码
	long GetFolderPics(wstring strPath, queue<wstring>& lstFile);
	
	/// @brief 获取子文件夹数量
	/// @details 递归计算指定文件夹下的所有子文件夹数量
	/// @param[in] dDir 根目录路径
	/// @param[out] lstSubs 输出参数，子文件夹路径到数量的映射表
	/// @return 0: 成功; 其他: 错误码
	long GetSubFolderNb(wstring dDir, map<wstring, uint32_t>& lstSubs);
	
	/// @brief 复制应用到 AppData
	/// @details 将相册配置复制到系统应用数据目录
	/// @param[in] alb 相册名称
	/// @param[in] msix 菜单索引
	/// @param[out] bAttr 输出参数，属性标记
	/// @note 用于多用户环境下的配置同步
	void CopyAppData(string alb, int msix, int& bAttr);

private:
	/// @brief 判断是否为图片文件
	/// @details 根据文件扩展名判断是否为支持的图片格式
	/// @param[in] cFileName 文件名（宽字符）
	/// @param[in] crc CRC8 校验值（用于去重）
	/// @return true: 是图片文件; false: 不是图片文件
	/// @note 内联函数，提升性能
	bool inline is_picture(wchar_t* cFileName, uint8_t crc);
	
	/// @brief 获取子文件夹列表（使用 Scripting.Folder）
	/// @details 使用 Windows Scripting 接口枚举子文件夹
	/// @param[in] ty 文件夹类型
	/// @param[in] folder Folder 对象指针
	/// @param[out] lstSubs 输出参数，子文件夹路径列表
	/// @return 0: 成功; 其他: 错误码
	int zz(int ty, Scripting::IFolderPtr folder, list<wstring >& lstSubs);
	
	/// @brief 获取文件夹图片（递归版本）
	/// @details 递归遍历文件夹，获取所有图片文件
	/// @param[in] strPath 文件夹路径
	/// @param[out] lstFile 输出参数，图片文件路径列表
	/// @param[out] fsize 输出参数，文件夹大小（字节）
	/// @return 0: 成功; 其他: 错误码
	long zz0(wstring strPath, list<wstring>& lstFile, uint32_t& fsize);
	
	/// @brief 获取 Variant 类型的大小
	/// @details 将 Variant 类型的大小转换为 uint64_t
	/// @param[in] vsize2 Variant 大小对象
	/// @return 文件大小（字节）
	/// @note 用于处理 Windows API 返回的文件大小
	uint64_t variantSize(_variant_t vsize2);
	
	/// @brief 获取文件 CRC8 校验值
	/// @details 计算指定文件的 CRC8 校验值（用于去重）
	/// @param[in] src 文件路径（宽字符）
	/// @param[out] crc 输出参数，CRC8 校验值
	/// @return 0: 成功; 其他: 错误码
	long get_file_crc(wchar_t* src, uint8_t& crc);
	
	/// @brief 获取子文件夹数量（使用 Scripting.Folder）
	/// @details 使用 Windows Scripting 接口递归计算子文件夹数量
	/// @param[in] folder Folder 对象指针
	/// @param[out] lstSubs 输出参数，子文件夹路径到数量的映射表
	/// @return 0: 成功; 其他: 错误码
	int zz5(Scripting::IFolderPtr folder, map<wstring, uint32_t>& lstSubs);

	/// @brief 支持的图片文件扩展名列表
	/// @details 以 null 结尾的宽字符字符串数组，
	///          包含所有支持的图片和视频格式
	const wchar_t* _exts[12] = { L".jpg", L".jpeg", L".png", L".webp" ,L".webm", L".gif", L".bmp", L".dib", L".mp3", L".mp4", L".ogg", NULL };
	
	// TODO: 正则表达式对象（当前已注释）
	// regex _re;
	
	/// @brief 同步复制文件夹
	/// @details 递归复制文件夹到目标位置
	/// @param[in] path 源路径
	/// @param[in] appDataDir 目标应用数据目录
	/// @param[in] subfolder 子文件夹名称
	void sc(wstring path, wstring appDataDir, wstring subfolder);

	/// @brief 创建快捷方式
	/// @details 在指定位置创建指向目标路径的快捷方式
	/// @param[in] lpszPathObj 目标路径
	/// @param[in] lpszPathLink 快捷方式路径
	/// @param[in] lpszDesc 快捷方式描述
	/// @param[in] lpszArgs 启动参数（可选）
	/// @param[in] lpszIcon 图标路径（可选）
	/// @return S_OK: 成功; 其他: 错误码
	/// @note 使用 Windows Shell API 创建快捷方式
	HRESULT CreateLink(LPCWSTR lpszPathObj, LPCWSTR lpszPathLink, LPCWSTR lpszDesc, LPCWSTR lpszArgs, LPCWSTR lpszIcon);
	
	/// @brief 获取 AppData 路径（重载版本）
	/// @details 根据相册名称和类型获取应用数据目录路径
	/// @param[in] alb 相册名称
	/// @param[in] ty 路径类型
	/// @return 应用数据目录路径（宽字符串）
	wstring get_appdata_path2(string alb, int ty);
};
