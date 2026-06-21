///////////////////////////////////////////////////////////////////////
/// @file unitil3.h
/// @brief 安装程序工具函数声明(inscl 模块)
/// @details 提供字符串转换、路径处理、文本处理等工具函数声明,
///          是 inscl 模块专用的工具函数头文件
/// @author PhotoNest Team
/// @date 2024
/// @version 1.0
/// @note 本文件与 photol/ 模块的 unitil3.h 功能类似但独立
///////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <cctype>
#include <algorithm>
#include <vector>

using namespace std;

string _w2u(wstring w);
wstring _u2w(string u);

wstring Replace(wstring &szBody, wstring szOld, wstring szNew);
string trim(string& s, const string drop);

vector<string> tokenize(const string& src, string tok, bool btrim, string null_subst);
wstring get_module_path(HMODULE hModule);
