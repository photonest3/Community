///////////////////////////////////////////////////////////////////////////////
// 文件名: ImBase.h
// 模块: scheme/scheme
// 功能描述: 图片处理基类
//           提供图片处理、HTML 生成、请求解析等基础功能
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-03-10  PhotoNest Team  添加图片尺寸计算功能
//   2024-05-15  PhotoNest Team  添加多语言支持
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stmdef.h"
#include "Singleton.h"

/// @brief 图片处理基类
/// @details 提供图片处理、HTML 生成、请求解析、导航栏生成等
///          基础功能。所有图片处理类的基类。
/// @note 此类包含大量静态工具方法，可被所有子类复用
/// @warning 此类不是线程安全的，子类需要自行处理线程同步
class CImBase
{
public:
	/// @brief 构造函数
	/// @details 初始化基类对象
	CImBase(void);
	
	/// @brief 析构函数
	/// @details 清理资源
	~CImBase(void);

protected:
	/// @brief 生成页面标题 HTML
	/// @param[in] lst 分类列表
	/// @param[in] szUrl 当前 URL
	/// @param[in] req 查询请求对象
	/// @param[in] picid 图片 ID
	/// @param[in] nb_images 图片总数
	/// @return 页面标题 HTML 字符串
	/// @note 生成页面标题区域（包含分类名称、图片数量等）
	string get_title(list<CATEGORY_DTO> lst, string szUrl, const QUERY_REQ req, string picid, uint32_t nb_images);
	
	/// @brief 解析请求参数
	/// @param[in] type 请求类型（GET/POST）
	/// @param[in] request_dto 请求数据传输对象
	/// @param[out] req 查询请求对象（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 从 HTTP 请求中解析查询参数
	long get_req(int type, const REQUEST_DTO request_dto, QUERY_REQ& req);
	
	/// @brief 设置请求参数
	/// @param[in] posted POST 数据字符串
	/// @param[out] req 查询请求对象（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 从 POST 数据中解析请求参数
	long set_req(string posted, QUERY_REQ& req);
	
	/// @brief 获取指定月份的天数
	/// @param[in] year 年份
	/// @param[in] month 月份（1-12）
	/// @return 该月的天数
	/// @note 考虑闰年情况
	int getdays(int year, int month);
	
	/// @brief 获取图片 URL
	/// @param[in] req 查询请求对象
	/// @return 图片 URL 字符串
	/// @note 根据请求参数生成图片访问 URL
	string get_picurl(const QUERY_REQ req);
	
	/// @brief 获取上传 URL
	/// @param[in] req 查询请求对象
	/// @return 上传 URL 字符串
	/// @note 生成文件上传的 URL
	string get_up_url(const QUERY_REQ req);
	
	/// @brief 计算图片显示尺寸
	/// @param[in] dto 图片 DTO
	/// @param[in] maxWidth 最大宽度
	/// @param[in] maxHeight 最大高度
	/// @param[out] outW 输出宽度（输出）
	/// @param[out] outH 输出高度（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 等比例缩放图片到指定范围内
	long get_imgsize(IMAGE_DTO dto, uint32_t maxWidth, uint32_t maxHeight, uint32_t& outW, uint32_t& outH);
	
	/// @brief 计算图片灵活显示尺寸
	/// @param[in] dto 图片 DTO
	/// @param[in] maxWidth 最大宽度
	/// @param[in] maxHeight 最大高度
	/// @param[out] outW 输出宽度（输出）
	/// @param[out] outH 输出高度（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 智能计算图片显示尺寸（考虑旋转、裁剪等）
	long get_flex_size(IMAGE_DTO dto, uint32_t maxWidth, uint32_t maxHeight, uint32_t& outW, uint32_t& outH);
	
	/// @brief 格式化时间字符串
	/// @param[in] tt 时间戳（秒）
	/// @param[in] fmt 时间格式（如 "%Y-%m-%d %H:%M:%S"）
	/// @return 格式化后的时间字符串
	/// @note 将时间戳转换为指定格式的字符串
	string get_time_string(uint64_t tt, string fmt);
	
	/// @brief 生成导航栏 HTML
	/// @param[in] url 基础 URL
	/// @param[in] total 总记录数
	/// @param[in] curr 当前页码
	/// @param[in] per 每页记录数
	/// @param[in] css CSS 样式选项（0: 默认, 1: 简洁）
	/// @return 导航栏 HTML 字符串
	/// @note 生成分页导航栏
	string get_navigationBar(string url, int total, int curr, int per, int css = 0);
	
	/// @brief 生成图片数量字符串
	/// @param[in] nb 图片数量
	/// @return 图片数量字符串（如 "123 张图片"）
	/// @note 根据数量生成国际化字符串
	string get_photo_string(uint32_t nb);
	
	/// @brief MurmurHash3 32位哈希算法
	/// @param[in] key 待哈希数据指针
	/// @param[in] len 数据长度（字节）
	/// @param[in] seed 哈希种子
	/// @return 32位哈希值
	/// @note 用于生成短链接、缓存键等
	uint32_t murmur3_32(const uint8_t* key, int len, uint32_t seed);
	
	/// @brief 格式化时长字符串
	/// @param[in] duration 时长（秒）
	/// @return 格式化后的时长字符串（如 "1:23"）
	/// @note 将秒数转换为 MM:SS 格式
	string get_dur_string(uint32_t duration);
	
	/// @brief 获取长日期字符串
	/// @return 长日期字符串（如 "2024年6月21日 星期五"）
	/// @note 生成当前日期的完整中文表示
	string get_sLongDate();
	
	/// @brief 获取年月字符串
	/// @return 年月字符串（如 "2024-06"）
	/// @note 生成当前年月
	string get_sYearMonth();
	
	/// @brief 删除文件
	/// @param[in] dst 文件路径（宽字符）
	/// @param[in] ty 删除类型（0: 直接删除, 1: 移动到回收站）
	/// @return TRUE: 成功; FALSE: 失败
	/// @note 封装文件删除操作，支持移动到回收站
	/// @warning 删除操作不可恢复，请谨慎使用
	BOOL my_DeleteFile(wstring dst, int ty = 0);
	
	/// @brief 转义字符串（防止 XSS 攻击）
	/// @param[in,out] name 待转义字符串（输出时将被修改）
	/// @return 0: 成功; 其他: 错误码
	/// @note 转义 HTML 特殊字符（<, >, ", ', & 等）
	long esc_string(string& name);
	
	/// @brief 翻译字符串（多语言支持）
	/// @param[in,out] strText 待翻译字符串（输出时将被修改）
	/// @param[in] intVal 整数值参数
	/// @param[in] szVal 字符串参数
	/// @return 翻译是否成功
	/// @note 根据当前语言设置翻译字符串
	/// @see lang.h 语言文件
	bool translate(string& strText, string intVal, string szVal);
	
	/// @brief 获取图片旋转角度 CSS 字符串
	/// @param[in] rotation 旋转角度（0, 90, 180, 270）
	/// @return 旋转角度 CSS 字符串（如 "transform: rotate(90deg);"）
	/// @note 生成图片旋转显示的 CSS
	string get_angle(uint32_t rotation);
	
	/// @brief 原始路径转换为 Web 访问路径
	/// @param[in] lstRoot 根目录列表
	/// @param[in] type 路径类型
	/// @param[in] ori 原始路径
	/// @return Web 访问路径字符串
	/// @note 将文件系统路径转换为 HTTP URL 路径
	string ori2path(list<ROOT_DTO> lstRoot, int type, string ori);
	
	/// @brief 读取 HTML 模板文件
	/// @param[in] cr_id 内容请求 ID
	/// @param[in] filename 模板文件路径（宽字符）
	/// @param[out] szhtml HTML 内容字符串（输出）
	/// @return 0: 成功; 其他: 错误码
	/// @note 读取 HTML 模板文件内容
	long Htmlread(UINT cr_id, wstring filename, string& szhtml);
};
