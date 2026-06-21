///////////////////////////////////////////////////////////////////////////////
// 文件名: letter.h
// 模块: scheme/scheme
// 功能描述: 获取中文字符首字母
//           用于中文排序和索引功能
// 作者: PhotoNest Team
// 创建日期: 2024-01-10
// 修改记录:
//   2024-06-21  PhotoNest Team  添加完整注释
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <windows.h>
#include <string>
#include <cctype>
#include <tchar.h>
#include "unitil.h"

using namespace std;

/// @brief 获取中文字符串的首字母
/// @param[in] strName 中文字符串（宽字符）
/// @return 首字母字符串（大写）
/// @details 提取中文字符串中各字的首字母，用于中文排序和索引。
///          对于非中文，返回原字符的大写形式。
/// @note 使用 GB2312 编码判断中文
/// @warning 仅支持 GB2312 编码的中文字符（约 6700 个）
string GetFirstLetter(wstring strName);
