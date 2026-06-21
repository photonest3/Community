///////////////////////////////////////////////////////////////////////////////
// 文件名: pngProc.h
// 模块: scheme/scheme
// 功能描述: PNG 图片处理类
//           使用 FreeImage 库处理 PNG 图片的位深转换和缩放
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-03-15  PhotoNest Team  添加 PNG 位深转换功能
//   2024-05-20  PhotoNest Team  添加双线性插值缩放
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "../../FreeImage/Source/FreeImage.h"

/// @brief 位深类型定义
/// @note 定义图片的位深度（Bits Per Pixel）
#define BPP_TYPE_4   4   ///< 4 位（16 色）
#define BPP_TYPE_8   8   ///< 8 位（256 色）
#define BPP_TYPE_16 16   ///< 16 位（高彩色）
#define BPP_TYPE_24 24   ///< 24 位（真彩色）
#define BPP_TYPE_32 32   ///< 32 位（带 Alpha 通道的真彩色）

/// @brief 缩放精度固定小数点
/// @note 用于缩放计算的精度控制（100 = 1.00）
#define SCALE_FIX_PRE 100

/// @brief 每像素字节数定义
/// @note 根据位深度计算每像素占用的字节数
#define BPP_8_BYTE_NUM   1  ///< 8 位 = 1 字节
#define BPP_16_BYTE_NUM 2  ///< 16 位 = 2 字节
#define BPP_24_BYTE_NUM 3  ///< 24 位 = 3 字节
#define BPP_32_BYTE_NUM 4  ///< 32 位 = 4 字节

/// @brief 图片复制回调函数类型定义
/// @param[in] in 输入图片数据指针
/// @param[out] out 输出图片数据指针
/// @param[in,out] offset 偏移量数组（输入/输出）
/// @note 用于自定义图片复制逻辑
typedef void(*copyback)(const BYTE* in, BYTE* out, int* offset);

/// @brief 双线性插值复制回调函数类型定义
/// @param[in] pa 左上角像素指针
/// @param[in] pb 右上角像素指针
/// @param[in] pc 左下角像素指针
/// @param[in] pd 右下角像素指针
/// @param[in] pla 左上角权重
/// @param[in] plb 右上角权重
/// @param[in] plc 左下角权重
/// @param[in] pld 右下角权重
/// @param[out] out 输出像素指针
/// @note 用于双线性插值缩放算法
typedef void(*bilinearcopyback)(const BYTE* pa, const BYTE* pb, const BYTE* pc, const BYTE* pd,
	double pla, double plb, double plc, double pld, BYTE* out);

/// @brief PNG 图片处理类
/// @details 使用 FreeImage 库处理 PNG 图片的位深转换、缩放、复制等操作。
///          支持最近邻插值和双线性插值两种缩放算法。
/// @note 此类主要处理 PNG 图片，也支持其他 FreeImage 支持的格式
/// @warning 确保输入 FIBITMAP 指针有效，否则会导致内存访问错误
class CPngProc
{
public:
	/// @brief 构造函数
	/// @details 初始化 PNG 处理对象
	CPngProc(void);
	
	/// @brief 析构函数
	/// @details 清理资源
	~CPngProc(void);

public:
	/// @brief 获取指定位深度的每像素字节数
	/// @param[in] bpp 位深度（8, 16, 24, 32）
	/// @return 每像素字节数
	/// @note 静态方法，可直接调用
	static int CPngProc::FreeImage_GetBppByteNum(int bpp);

	/// @brief 复制 8 位图片数据
	/// @param[in] in 输入图片数据指针
	/// @param[out] out 输出图片数据指针
	/// @note 用于 8 位（256 色）图片的像素复制
	static void FreeImage_Copy8Bit(const BYTE* in, BYTE* out);
	
	/// @brief 复制 16 位图片数据
	/// @param[in] in 输入图片数据指针
	/// @param[out] out 输出图片数据指针
	/// @note 用于 16 位（高彩色）图片的像素复制
	static void FreeImage_Copy16Bit(const BYTE* in, BYTE* out);
	
	/// @brief 复制 24 位图片数据
	/// @param[in] in 输入图片数据指针
	/// @param[out] out 输出图片数据指针
	/// @note 用于 24 位（真彩色）图片的像素复制
	static void FreeImage_Copy24Bit(const BYTE* in, BYTE* out);
	
	/// @brief 复制 32 位图片数据
	/// @param[in] in 输入图片数据指针
	/// @param[out] out 输出图片数据指针
	/// @note 用于 32 位（带 Alpha 通道的真彩色）图片的像素复制
	static void FreeImage_Copy32Bit(const BYTE* in, BYTE* out);
	
	/// @brief 复制图片数据（指定位深度）
	/// @param[in] src 源图片数据指针
	/// @param[in] srcBpp 源图片位深度
	/// @param[out] dst 目标图片数据指针
	/// @param[in] dstBpp 目标图片位深度
	/// @param[in] paste 粘贴模式
	/// @note 支持不同位深度之间的转换复制
	static void FreeImage_CopyBit(const BYTE* src, int srcBpp, BYTE* dst, int dstBpp, int paste);
	
	/// @brief 复制图片 Alpha 通道数据
	/// @param[in] pst 源图片数据指针（二级指针）
	/// @param[in] pstBpp 源图片位深度
	/// @param[out] dst 目标图片数据指针（二级指针）
	/// @param[in] dstBpp 目标图片位深度
	/// @note 专门用于 Alpha 通道的复制
	static void FreeImage_AlphaCopyBit(const BYTE** pst, int pstBpp, BYTE** dst, int dstBpp);

	/// @brief 缩放复制 8 位图片数据
	/// @param[in] in 输入图片数据指针
	/// @param[out] out 输出图片数据指针
	/// @param[in,out] offset 偏移量数组（输入/输出）
	/// @note 用于 8 位图片的缩放复制（最近邻插值）
	static void FreeImage_ZoomCopy8Bit(const BYTE* in, BYTE* out, int* offset);
	
	/// @brief 缩放复制 16 位图片数据
	/// @param[in] in 输入图片数据指针
	/// @param[out] out 输出图片数据指针
	/// @param[in,out] offset 偏移量数组（输入/输出）
	/// @note 用于 16 位图片的缩放复制（最近邻插值）
	static void FreeImage_ZoomCopy16Bit(const BYTE* in, BYTE* out, int* offset);
	
	/// @brief 缩放复制 24 位图片数据
	/// @param[in] in 输入图片数据指针
	/// @param[out] out 输出图片数据指针
	/// @param[in,out] offset 偏移量数组（输入/输出）
	/// @note 用于 24 位图片的缩放复制（最近邻插值）
	static void FreeImage_ZoomCopy24Bit(const BYTE* in, BYTE* out, int* offset);
	
	/// @brief 缩放复制 32 位图片数据
	/// @param[in] in 输入图片数据指针
	/// @param[out] out 输出图片数据指针
	/// @param[in,out] offset 偏移量数组（输入/输出）
	/// @note 用于 32 位图片的缩放复制（最近邻插值）
	static void FreeImage_ZoomCopy32Bit(const BYTE* in, BYTE* out, int* offset);
	
	/// @brief 计算缩放参数
	/// @param[in] oldWidth 原始宽度
	/// @param[in] oldHeight 原始高度
	/// @param[in] width 目标宽度
	/// @param[in] height 目标高度
	/// @param[out] wScale 宽度缩放因子（输出）
	/// @param[out] wScaleFix 宽度缩放因子（定点数，输出）
	/// @param[out] hScale 高度缩放因子（输出）
	/// @param[out] hScaleFix 高度缩放因子（定点数，输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 计算缩放所需的各种参数
	static int FreeImage_ZoomCopyScale(int oldWidth, int oldHeight,
		int width, int height, int* wScale, int* wScaleFix, int* hScale, int* hScaleFix);
	
	/// @brief 计算缩放后的索引位置
	/// @param[in] index 原始索引位置
	/// @param[in] scale 缩放因子
	/// @param[in] scaleFix 缩放因子（定点数）
	/// @param[out] pos 目标位置（输出）
	/// @param[out] posFix 目标位置（定点数，输出）
	/// @param[in] srcLength 原始长度
	/// @param[in] length 目标长度
	/// @return 0: 成功; 其他: 错误码
	/// @note 用于计算缩放后像素的位置映射
	static int FreeImage_ZoomGetIndex(int index,
		int scale, int scaleFix, int* pos, int* posFix, int srcLength, int length);
	
	/// @brief 缩放复制图片数据（最近邻插值）
	/// @param[in] in 输入 FIBITMAP 指针
	/// @param[in] width 目标宽度
	/// @param[in] height 目标高度
	/// @param[out] out 输出 FIBITMAP 指针
	/// @param[in] copy 复制回调函数指针
	/// @return 0: 成功; 其他: 错误码
	/// @note 使用最近邻插值算法缩放图片
	static int FreeImage_ZoomCopy(FIBITMAP* in, int width, int height, FIBITMAP* out, copyback copy);
	
	/// @brief 双线性插值复制 32 位图片数据
	/// @param[in] pa 左上角像素指针
	/// @param[in] pb 右上角像素指针
	/// @param[in] pc 左下角像素指针
	/// @param[in] pd 右下角像素指针
	/// @param[in] pla 左上角权重
	/// @param[in] plb 右上角权重
	/// @param[in] plc 左下角权重
	/// @param[in] pld 右下角权重
	/// @param[out] out 输出像素指针
	/// @note 用于 32 位图片的双线性插值缩放
	static void FreeImage_BillinearCopy32Bit(const BYTE* pa, const BYTE* pb, const BYTE* pc, const BYTE* pd,
		const double pla, const double plb, const double plc, const double pld, BYTE* out);
	
	/// @brief 缩放复制图片数据（双线性插值）
	/// @param[in] in 输入 FIBITMAP 指针
	/// @param[in] width 目标宽度
	/// @param[in] height 目标高度
	/// @param[out] out 输出 FIBITMAP 指针
	/// @param[in] copy 双线性复制回调函数指针
	/// @return 0: 成功; 其他: 错误码
	/// @note 使用双线性插值算法缩放图片（质量更好，速度较慢）
	static int FreeImage_Bilinear_ZoomCopy(FIBITMAP* in, int width, int height, FIBITMAP* out, bilinearcopyback copy);
	
	/// @brief 创建 8 位图片
	/// @param[in] dib 参考 FIBITMAP 指针
	/// @param[in] width 目标宽度
	/// @param[in] height 目标高度
	/// @return 新创建的 FIBITMAP 指针，失败返回 NULL
	/// @note 创建指定尺寸的 8 位（256 色）图片
	FIBITMAP* FreeImage_New8BppImage(FIBITMAP* dib, int width, int height);
	
	/// @brief 创建 16 位图片
	/// @param[in] dib 参考 FIBITMAP 指针
	/// @param[in] width 目标宽度
	/// @param[in] height 目标高度
	/// @return 新创建的 FIBITMAP 指针，失败返回 NULL
	/// @note 创建指定尺寸的 16 位（高彩色）图片
	FIBITMAP* FreeImage_New16BppImage(FIBITMAP* dib, int width, int height);
	
	/// @brief 创建 24 位图片
	/// @param[in] dib 参考 FIBITMAP 指针
	/// @param[in] width 目标宽度
	/// @param[in] height 目标高度
	/// @return 新创建的 FIBITMAP 指针，失败返回 NULL
	/// @note 创建指定尺寸的 24 位（真彩色）图片
	FIBITMAP* FreeImage_New24BppImage(FIBITMAP* dib, int width, int height);
	
	/// @brief 创建 32 位图片
	/// @param[in] dib 参考 FIBITMAP 指针
	/// @param[in] width 目标宽度
	/// @param[in] height 目标高度
	/// @param[in] bilinear 是否使用双线性插值
	/// @return 新创建的 FIBITMAP 指针，失败返回 NULL
	/// @note 创建指定尺寸的 32 位（带 Alpha 通道的真彩色）图片
	FIBITMAP* FreeImage_New32BppImage(FIBITMAP* dib, int width, int height, int bilinear);
	
	/// @brief Alpha 通道粘贴
	/// @param[in] dib 目标 FIBITMAP 指针
	/// @param[in] pst 源 FIBITMAP 指针
	/// @param[in] pitx 粘贴位置 X 坐标
	/// @param[in] pity 粘贴位置 Y 坐标
	/// @return 0: 成功; 其他: 错误码
	/// @note 将带 Alpha 通道的图片粘贴到目标图片
	int FreeImage_AlphaPaste(FIBITMAP* dib, FIBITMAP* pst, int pitx, int pity);
	
	/// @brief 主函数（测试用）
	/// @return 0: 成功; 其他: 错误码
	/// @note 用于测试和调试
	int main0();

};
