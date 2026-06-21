///////////////////////////////////////////////////////////////////////////////
// 文件名: ImageProc.h
// 模块: scheme/scheme
// 功能描述: 图片处理类
//           使用 FreeImage 和 Media Foundation 处理图片和视频
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-03-15  PhotoNest Team  添加 EXIF 方向处理
//   2024-05-20  PhotoNest Team  添加 MP4 缩略图生成
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"
#include "../../FreeImage/Source/FreeImage.h"
#include <mfapi.h>
#include <mfplay.h>
#include <mfreadwrite.h>
#include <mferror.h>
#include <wmcodecdsp.h>

/// @brief 函数指针类型定义：获取视频状态
/// @param[in] szFile 视频文件路径（宽字符）
/// @param[in] dst 输出文件路径（宽字符）
/// @param[in,out] width 输入/输出 视频宽度
/// @param[in,out] height 输入/输出 视频高度
/// @param[out] pYUVBuffer YUV 缓冲区指针的指针
/// @return 成功返回 0，失败返回非 0
/// @note 此函数从 Media Foundation DLL 中导出
typedef int(_cdecl *pfnGetStatus)(const wchar_t * szFile, const wchar_t * dst, int &width, int &height, unsigned char **pYUVBuffer);

/// @brief 图片处理类
/// @details 使用 FreeImage 库处理静态图片（JPEG、PNG、BMP 等），
///          使用 Media Foundation 处理视频文件（MP4、AVI 等），
///          生成缩略图、提取 EXIF 信息、处理图片旋转等。
///          采用单例模式确保全局唯一实例。
/// @note 此类依赖 FreeImage 库和 Media Foundation API
/// @warning 视频处理需要加载外部 DLL（mfthumb.dll）
class CImageProc : public Singleton<CImageProc>
{
	friend class Singleton<CImageProc>;

protected:
	/// @brief 构造函数
	/// @details 初始化图片处理对象，加载 Media Foundation DLL
	CImageProc(void);
	
	/// @brief 析构函数
	/// @details 清理资源，释放 Media Foundation DLL
	~CImageProc(void);

public:
	/// @brief 测试 JPEG 文件并生成缩略图
	/// @param[in] lpszPathName 原始图片路径（宽字符）
	/// @param[in] lpszImagePathName 缩略图输出路径（宽字符）
	/// @param[in] scale_denom 缩放分母（1: 原图, 2: 1/2, 4: 1/4, 8: 1/8）
	/// @param[in] rotation 旋转角度（0, 90, 180, 270）
	/// @return TRUE: 成功; FALSE: 失败
	/// @note 使用 FreeImage 库处理 JPEG 文件
	/// @see FreeImage 文档
	BOOL jp_test(const wchar_t *lpszPathName, const wchar_t *lpszImagePathName, int scale_denom, long rotation);
	
	/// @brief 保存缩略图（自动检测文件类型）
	/// @param[in] lpszPathName 原始图片路径（宽字符）
	/// @param[in] lpszImagePathName 缩略图输出路径（宽字符）
	/// @param[in] rotation 旋转角度（0, 90, 180, 270）
	/// @param[in] maxWidth 最大宽度
	/// @param[in] maxHeight 最大高度
	/// @param[out] ftype 文件类型（输出）
	/// @return TRUE: 成功; FALSE: 失败
	/// @note 自动检测文件类型（JPEG、PNG、BMP、GIF 等）
	BOOL testSaveThumbnail(const wchar_t *lpszPathName, const wchar_t *lpszImagePathName, long rotation, float maxWidth, float maxHeight, int &ftype);
	
	/// @brief 获取图片 EXIF 值
	/// @param[in] filename 图片文件路径（宽字符）
	/// @param[in] type EXIF 类型（如 "Make", "Model", "DateTime" 等）
	/// @param[out] attr EXIF 属性映射表（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 支持的 EXIF 类型请参考 EXIF 标准
	/// @see CHtmlexif 类
	long get_exif_value(wstring filename, string type, map<string, string> &attr);
	
	/// @brief 保存 MP4 视频缩略图
	/// @param[in] cx 缩略图宽度
	/// @param[in] szFile 视频文件路径（宽字符）
	/// @param[in] dst 缩略图输出路径（宽字符）
	/// @return 0: 成功; 其他: 错误码
	/// @note 使用 Media Foundation 从视频中提取帧作为缩略图
	HRESULT save_mp4_thumbnail(UINT cx, wstring szFile, wstring dst);
	
	/// @brief 获取图片 EXIF 方向值
	/// @param[in] filename 图片文件路径（宽字符）
	/// @return EXIF 方向值（1-8），失败返回 0
	/// @note EXIF 方向值定义：1: 正常, 3: 旋转 180°, 6: 旋转 90° CW, 8: 旋转 90° CCW
	long get_Orientation_value(wstring filename);
	
	/// @brief 处理 MP4 视频缩略图生成
	/// @param[in] szFile 视频文件路径（宽字符）
	/// @param[in] dst 缩略图输出路径（宽字符）
	/// @param[in] rotate 旋转角度
	/// @return 0: 成功; 其他: 错误码
	/// @note 调用 Media Foundation DLL 处理视频缩略图
	int proc_mp4_thumbnail(wstring szFile, wstring dst, long rotate);

private:
	/// @brief 读取 JPEG 文件（使用 FreeImage）
	/// @param[in] filename JPEG 文件路径（宽字符）
	/// @param[in] scale_denom 缩放分母
	/// @param[out] outwidth 输出宽度（输出）
	/// @param[out] outheight 输出高度（输出）
	/// @return 图像数据缓冲区指针，失败返回 NULL
	/// @note 使用 FreeImage 库读取 JPEG 文件
	unsigned char * read_jpeg_file(const wchar_t *filename, int scale_denom, unsigned int &outwidth, unsigned int &outheight);
	
	/// @brief 写入 JPEG 文件（使用 FreeImage）
	/// @param[in] filename JPEG 输出文件路径（宽字符）
	/// @param[in] image_buffer 图像数据缓冲区指针
	/// @param[in] quality JPEG 质量（1-100）
	/// @param[in] image_width 图像宽度
	/// @param[in] image_height 图像高度
	/// @return 0: 成功; 其他: 错误码
	/// @note 使用 FreeImage 库写入 JPEG 文件
	int write_jpeg_file(const wchar_t * filename, unsigned char* image_buffer, int quality, unsigned int image_width, unsigned int image_height);
	
	/// @brief 处理图片（内部方法）
	/// @param[in] lpszPathName 原始图片路径（宽字符）
	/// @param[in] lpszImagePathName 输出图片路径（宽字符）
	/// @param[in] rotation 旋转角度
	/// @return 0: 成功; 其他: 错误码
	/// @note 内部图片处理方法（可能是旧版本或备用方法）
	int xxxx(const wchar_t *lpszPathName, const wchar_t *lpszImagePathName, long rotation);
	
	/// @brief 处理位图（使用 GDI+ 或 WIC）
	/// @param[in] hbmp 位图句柄
	/// @param[in] dst 输出文件路径（宽字符）
	/// @return HRESULT 成功返回 S_OK，失败返回错误码
	/// @note 使用 GDI+ 或 Windows Imaging Component 处理位图
	HRESULT proc_bimap(HBITMAP hbmp, wstring dst);
	
	/// @brief 将 YUV420SP 格式转换为 JPEG
	/// @param[in] filename 输出 JPEG 文件路径（宽字符）
	/// @param[in] width 视频宽度
	/// @param[in] height 视频高度
	/// @param[in] pYUVBuffer YUV 缓冲区指针
	/// @return 0: 成功; 其他: 错误码
	/// @note 将视频帧（YUV420SP 格式）转换为 JPEG 图片
	int yuv420sp_to_jpg(const wchar_t *filename, int width, int height, unsigned char *pYUVBuffer);

private:
	/// @brief 序列 ID（未使用）
	int _seq_id;
	
	/// @brief Media Foundation DLL 实例句柄
	HINSTANCE _mfthumb_dll;
	
	/// @brief Media Foundation 函数指针
	pfnGetStatus _func;
	
};
