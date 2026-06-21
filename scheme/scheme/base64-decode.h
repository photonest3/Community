///////////////////////////////////////////////////////////////////////////////
// 文件名: base64-decode.h
// 模块: scheme/scheme
// 功能描述: Base64 编码和解码功能
//           提供字符串的 Base64 编码和解码接口
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#ifndef BASE64_DECODE_H
#define BASE64_DECODE_H

/// @brief Base64 编码
/// @details 将输入字符串进行 Base64 编码，结果输出到指定缓冲区。
///          编码后的字符串长度会是 4 的倍数，不足部分用 '=' 填充。
/// @param[in] in 输入字符串指针
/// @param[in] in_len 输入字符串长度
/// @param[out] out 输出缓冲区指针（需预先分配足够空间）
/// @param[in] out_size 输出缓冲区大小（应至少为 (in_len + 2) / 3 * 4 + 1）
/// @return 编码后的字符串长度（不包含终止符）; 失败返回 0
/// @note 输出字符串会自动添加 null 终止符
/// @warning 确保 out 缓冲区足够大，否则可能导致缓冲区溢出
int lws_b64_encode_string(const char *in, int in_len, char *out, int out_size);

/// @brief Base64 解码
/// @details 将 Base64 编码的字符串解码为原始数据。
///          支持标准 Base64 和 URL-safe Base64（'-' 和 '_'）
/// @param[in] in Base64 编码的输入字符串
/// @param[out] out 输出缓冲区指针（需预先分配足够空间）
/// @param[in] out_size 输出缓冲区大小（应至少为 in_len * 3 / 4 + 1）
/// @return 解码后的数据长度; 失败返回 0
/// @note 输出数据会自动添加 null 终止符
/// @warning 确保 out 缓冲区足够大，否则可能导致缓冲区溢出
int lws_b64_decode_string(const char *in, char *out, int out_size);

#endif // BASE64_DECODE_H