///////////////////////////////////////////////////////////////////////////////
// 文件名: util.h
// 模块: scheme/scheme
// 功能描述: 工具函数库
//           提供字符串处理、编码转换、URL 解析等工具函数
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-03-10  PhotoNest Team  添加 URL 编解码功能
//   2024-05-15  PhotoNest Team  添加字符串替换功能
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <cctype>
#include <algorithm>
#include <vector>

using namespace std;

/// @brief 宽字符串转 UTF-8 字符串
/// @param[in] w 宽字符串（UTF-16）
/// @return UTF-8 编码的字符串
/// @note 用于宽字符串到 UTF-8 的转换
string _w2u(wstring w);

/// @brief UTF-8 字符串转宽字符串
/// @param[in] u UTF-8 编码的字符串
/// @return 宽字符串（UTF-16）
/// @note 用于 UTF-8 到宽字符串的转换
wstring _u2w(string u);

/// @brief 十六进制字符转数字
/// @param[in] c 十六进制字符（0-9, A-F, a-f）
/// @return 对应的数字（0-15），失败返回 -1
/// @note 用于 URL 解码等场景
int HexDigit(char c);

/// @brief URL 解码
/// @param[in,out] sURL URL 字符串（输出时被修改）
/// @param[in] type 解码类型
/// @return 解码后的字符串
/// @note 将 %XX 编码转换为原始字符
/// @warning 确保输入字符串有效
string URLDecode(string& sURL, int type);

/// @brief URL 解码（版本 2）
/// @param[in,out] sURL URL 字符串（输出时被修改）
/// @return 解码后的字符串
/// @note 将 %XX 编码转换为原始字符（改进版）
string URLDecode2(string& sURL);

/// @brief 字符串替换（全局）
/// @param[in,out] szBody 待替换字符串（输出时被修改）
/// @param[in] szOld 旧字符串
/// @param[in] szNew 新字符串
/// @return 替换后的字符串
/// @note 替换所有匹配的旧字符串
string Replace(string& szBody, string szOld, string szNew);

/// @brief 宽字符串替换（全局）
/// @param[in,out] szBody 待替换宽字符串（输出时被修改）
/// @param[in] szOld 旧宽字符串
/// @param[in] szNew 新宽字符串
/// @return 替换后的宽字符串
/// @note 替换所有匹配的旧宽字符串
wstring Replace(wstring& szBody, wstring szOld, wstring szNew);

/// @brief 字符串替换（仅首次）
/// @param[in,out] szBody 待替换字符串（输出时被修改）
/// @param[in] szOld 旧字符串
/// @param[in] szNew 新字符串
/// @return 替换后的字符串
/// @note 仅替换第一个匹配的旧字符串
string ReplaceOne(string& szBody, string szOld, string szNew);

/// @brief 宽字符串替换（仅首次）
/// @param[in,out] szBody 待替换宽字符串（输出时被修改）
/// @param[in] szOld 旧宽字符串
/// @param[in] szNew 新宽字符串
/// @return 替换后的宽字符串
/// @note 仅替换第一个匹配的旧宽字符串
wstring ReplaceOne(wstring& szBody, wstring szOld, wstring szNew);

/// @brief 去除字符串两端指定字符
/// @param[in,out] s 待处理字符串（输出时被修改）
/// @param[in] drop 要去除的字符集合
/// @return 去除两端指定字符后的字符串
/// @note 去除字符串两端的指定字符（如空格、换行等）
string trim(string& s, const string drop);

/// @brief 去除字符串左端指定字符
/// @param[in,out] s 待处理字符串（输出时被修改）
/// @param[in] drop 要去除的字符集合
/// @return 去除左端指定字符后的字符串
/// @note 去除字符串左端的指定字符
string ltrim(string& s, const string drop);

/// @brief 去除字符串右端指定字符
/// @param[in,out] s 待处理字符串（输出时被修改）
/// @param[in] drop 要去除的字符集合
/// @return 去除右端指定字符后的字符串
/// @note 去除字符串右端的指定字符
string rtrim(string& s, const string drop);

/// @brief 查找并提取指定名称的值
/// @param[in] sz1 待查找字符串
/// @param[in] szname 要查找的名称
/// @param[out] value 查找到的值（输出）
/// @param[in] nstart 查找起始位置
/// @return 0: 成功; -1: 未找到
/// @note 用于解析 "name=value" 格式的字符串
int lookitem(string sz1, const char* szname, string& value, int nstart = 0);

/// @brief 宽字符串转小写
/// @param[in,out] s 待转换宽字符串（输出时被修改）
/// @return 小写宽字符串
/// @note 将宽字符串中的所有字符转换为小写
wstring my_tolower(wstring& s);

/// @brief 字符串转小写
/// @param[in,out] s 待转换字符串（输出时被修改）
/// @return 小写字符串
/// @note 将字符串中的所有字符转换为小写
string my_tolower(string& s);

/// @brief 字符串分割
/// @param[in] src 待分割字符串
/// @param[in] tok 分隔符
/// @param[in] btrim 是否去除两端空白
/// @param[in] null_subst 空字符串替换值
/// @return 分割后的字符串向量
/// @note 根据分隔符分割字符串
vector<string> tokenize(const string& src, string tok, bool btrim, string null_subst);

/// @brief 十六进制字符串转二进制
/// @param[out] dest 输出缓冲区
/// @param[in] src 十六进制字符串
/// @return 转换后的字节数
/// @note 将 "0123456789ABCDEF" 格式的字符串转换为二进制数据
int hexchange(unsigned char* dest, const char* src);

/// @brief 计算校验和
/// @param[in] buffer 数据缓冲区指针
/// @param[in] size 数据大小（字节）
/// @return 校验和值
/// @note 计算数据的简单校验和
unsigned short checksum(USHORT* buffer, int size);

/// @brief 获取模块路径（版本 0）
/// @param[in] hModule 模块句柄
/// @return 模块路径（宽字符串）
/// @note 获取指定模块的完整路径
wstring get_module_path0(HMODULE hModule);

/// @brief 获取模块路径
/// @param[in] hModule 模块句柄
/// @return 模块路径（宽字符串）
/// @note 获取指定模块的完整路径
wstring get_module_path(HMODULE hModule);

/// @brief 获取应用数据路径
/// @param[in] alb 相册名称
/// @return 应用数据路径（宽字符串）
/// @note 获取应用数据目录路径（如 "%APPDATA%\\PhotoNest\\alb"）
wstring get_appdata_path(string alb);

/// @brief 浮点数转字符串
/// @param[in] num 浮点数
/// @param[in] fmt 格式字符串（如 "%.2f"）
/// @return 格式化后的字符串
/// @note 将浮点数格式化为字符串
string float2Str(float num, char* fmt);

/// @brief 整数转字符串
/// @param[in] num 整数
/// @return 整数字符串
/// @note 将整数转换为字符串
string Int2Str(int num);

/// @brief 无符号 32 位整数转字符串
/// @param[in] num 无符号 32 位整数
/// @return 整数字符串
/// @note 将无符号 32 位整数转换为字符串
string uint2str(uint32_t num);

/// @brief 无符号 64 位整数转字符串
/// @param[in] num 无符号 64 位整数
/// @return 整数字符串
/// @note 将无符号 64 位整数转换为字符串
string uint64str(uint64_t num);

/// @brief 从指针读取无符号 32 位整数
/// @param[in] ptr 指针
/// @return 无符号 32 位整数
/// @note 从指定内存地址读取 32 位无符号整数
uint32_t get_uint32(char* ptr);

/// @brief 从指针读取无符号 32 位整数（const 版本）
/// @param[in] ptr 指针
/// @return 无符号 32 位整数
/// @note 从指定内存地址读取 32 位无符号整数
uint32_t get_uint32(const char* ptr);

/// @brief 从指针读取无符号 64 位整数
/// @param[in] ptr 指针
/// @return 无符号 64 位整数
/// @note 从指定内存地址读取 64 位无符号整数
uint64_t get_uint64(char* ptr);

/// @brief 解析 HTTP 请求 URL
/// @param[in] szUrl 完整 URL
/// @param[out] strhost 主机名（输出）
/// @param[out] strUrl 路径部分（输出）
/// @param[out] strArgs 查询参数部分（输出）
/// @note 解析 HTTP 请求 URL，分离主机名、路径和参数
void get_request_url(const char* szUrl, string& strhost, string& strUrl, string& strArgs);

/// @brief 查找并提取指定名称的值（数组版本）
/// @param[in] szBody 待查找字符串
/// @param[in] szname 要查找的名称
/// @param[out] value 查找到的值（输出）
/// @return 0: 成功; -1: 未找到
/// @note 用于解析 POST 请求体中的 "name=value" 格式数据
long lookitem_arr(string szBody, const char* szname, string& value);

/// @brief HTML 实体编码
/// @param[in,out] s 待编码字符串（输出时被修改）
/// @return 编码后的字符串
/// @note 将特殊字符转换为 HTML 实体（如 "<" 转换为 "&lt;"）
/// @warning 用于防止 XSS 攻击
string myEntity(string& s);

/// @brief 数据 URI 转缩略图
/// @param[in,out] src 数据 URI（输出时被修改）
/// @return 缩略图文件名
/// @note 将数据 URI（如 "data:image/jpeg;base64,..."）转换为图片文件
string _data2_thumb(string& src);

/// @brief 从指针读取字符值
/// @param[in] ptr 指针
/// @return 字符值
/// @note 从指定内存地址读取字符值
char* get_value(char* ptr);

/// @brief 从指针读取浮点数值
/// @param[in] ptr 指针
/// @return 浮点数值
/// @note 从指定内存地址读取浮点数值
float get_float(char* ptr);

/// @brief 获取今天的日期字符串
/// @param[in] tt 时间戳（秒），如果为 0 则使用当前时间
/// @return 日期字符串（如 "2024-06-21"）
/// @note 将时间戳转换为日期字符串
string get_today(time_t tt);
