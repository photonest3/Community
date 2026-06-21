///////////////////////////////////////////////////////////////////////
/// @file templ.h
/// @brief 模板 DLL 导出函数声明
/// @details 定义模板 DLL 的导出宏和 Read_htm 函数声明,
///          用于从资源中读取 HTML 模板文件
/// @author PhotoNest Team
/// @date 2024
/// @version 1.0
/// @note 本文件是 templ DLL 的头文件
///////////////////////////////////////////////////////////////////////

#ifndef TEMPL_H
#define TEMPL_H

#define TEMPL_EXPORTS

#ifdef TEMPL_EXPORTS
#define TEMPL_API __declspec(dllexport)
#else
#define TEMPL_API __declspec(dllimport)
#endif


TEMPL_API long Read_htm(unsigned int cr_id, const wchar_t* filename, char** szhtml);
