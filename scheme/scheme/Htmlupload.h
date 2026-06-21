///////////////////////////////////////////////////////////////////////////////
// 文件名: Htmlupload.h
// 模块: scheme/scheme
// 功能描述: 文件上传处理类
//           处理图片、视频等文件的上传请求
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-04-10  PhotoNest Team  添加断点续传功能
//   2024-06-01  PhotoNest Team  优化上传性能
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ImBase.h"
#include "stmdef.h"
#include "Singleton.h"

/// @brief 文件上传处理类
/// @details 处理文件上传的 HTTP 请求，包括图片、视频等文件。
///          支持断点续传、缩略图生成、EXIF 提取、分类自动创建等功能。
///          继承自 CImBase 和 Singleton 模式。
/// @note 此类是文件上传功能的核心处理类
/// @warning 确保上传文件经过验证，防止恶意文件上传
class CHtmlupload : public CImBase, public Singleton<CHtmlupload>
{
	friend class Singleton<CHtmlupload>;

protected:
	/// @brief 构造函数
	/// @details 初始化文件上传处理对象，初始化临界区
	CHtmlupload(void);
	
	/// @brief 析构函数
	/// @details 清理资源，删除临界区
	~CHtmlupload(void);

public:
	/// @brief 获取文件上传页面 HTML
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] response_dto 响应数据传输对象
	/// @return 0: 成功; 其他: 错误码
	/// @note 生成文件上传页面的 HTML
	long post_upload_htm(const REQUEST_DTO request_dto, RESPONSE_DTO& response_dto);
	
	/// @brief 设置根目录列表
	/// @return 0: 成功; 其他: 错误码
	/// @note 从数据库读取根目录列表并缓存
	long set_roots();

private:
	/// @brief 创建目录
	/// @param[in] path 目录路径（宽字符）
	/// @param[in] src 源路径（宽字符）
	/// @note 递归创建目录（如果父目录不存在）
	void mk_dir(wstring path, wstring src);
	
	/// @brief 创建目录和文件
	/// @param[in] cat_id 分类 ID
	/// @param[in] type 文件类型
	/// @param[in] szFile 源文件路径（宽字符）
	/// @param[out] fullname 完整文件路径（输出，宽字符）
	/// @param[out] toname 目标文件名（输出，宽字符）
	/// @param[out] upload_url 上传 URL（输出）
	/// @return TRUE: 成功; FALSE: 失败
	/// @note 创建上传文件的存储目录和文件名
	BOOL CreateDirectoryFile(string cat_id, string type, wstring szFile, wstring& fullname, wstring& toname, wstring& upload_url);
	
	/// @brief 处理缩略图生成（线程函数）
	/// @param[in] data 上传数据 DTO
	/// @param[in,out] imageDto 图片 DTO（输出时被修改）
	/// @note 后台线程，生成缩略图和提取 EXIF 信息
	void ProcThumbnailFunc(UPLOAD_DATA data, IMAGE_DTO& imageDto);
	
	/// @brief 分割文件名
	/// @param[in] file_name 完整文件名（宽字符）
	/// @param[out] szDir 目录路径（输出）
	/// @param[out] szName 文件名（不含扩展名，输出）
	/// @param[out] szExt 扩展名（输出）
	/// @note 将完整路径分割为目录、文件名、扩展名
	void splitFile(wstring file_name, wstring& szDir, wstring& szName, wstring& szExt);
	
	/// @brief 创建我的目录
	/// @param[in] path 目录路径（宽字符）
	/// @return TRUE: 成功; FALSE: 失败
	/// @note 创建用户目录（如果不存在）
	BOOL CreateMyDirectory(wstring path);
	
	/// @brief 获取封面图片
	/// @param[in] szFile 文件路径（宽字符）
	/// @param[in] szName 文件名（宽字符）
	/// @param[in] fullname 完整路径（宽字符）
	/// @param[in] upload_url 上传 URL
	/// @param[out] url 封面 URL（输出）
	/// @param[in,out] imageDto 图片 DTO（输出时被修改）
	/// @return 0: 成功; 其他: 错误码
	/// @note 为视频文件生成封面缩略图
	long get_cover(wstring szFile, wstring szName, wstring fullname, wstring upload_url, string& url, IMAGE_DTO& imageDto);
	
	/// @brief 加密文件
	/// @param[in] szFile 源文件路径（宽字符）
	/// @param[in] krf 密钥文件路径（宽字符）
	/// @return 0: 成功; 其他: 错误码
	/// @note 使用 Blowfish 算法加密文件
	/// @see CBlowfish 类
	long Encrypt_szFile(wstring szFile, wstring krf);
	
	/// @brief 设置分类
	/// @param[in] cat_id 分类 ID
	/// @return 0: 成功; 其他: 错误码
	/// @note 确保分类存在，如果不存在则创建
	long set_category(string cat_id);
	
	/// @brief 转换日期时间字符串为时间戳
	/// @param[in] dateTimeOriginal EXIF 原始日期时间字符串
	/// @return 时间戳（秒）
	/// @note 将 EXIF 日期时间转换为时间戳
	uint64_t convertDateTimeOriginalToUint64(const std::string& dateTimeOriginal);

private:
	/// @brief 临界区对象，用于多线程同步
	CRITICAL_SECTION _cs;
	
	/// @brief 文件集合（用于去重）
	list<string> _fileSet;
	
	/// @brief 分类 DTO 列表（二级缓存）
	list<CATEGORY_DTO2> _lstCategory;
	
	/// @brief 根目录 DTO 列表（二级缓存）
	list<ROOT_DTO2> _lstRoot2;
	
	/// @brief 我的目录列表（宽字符）
	list<wstring> _lstMydir;
	
	/// @brief 相册名称（默认 "public"）
	string album_="public";
};
