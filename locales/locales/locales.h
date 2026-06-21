///////////////////////////////////////////////////////////////////////
/// @file locales.h
/// @brief 多语言资源 DLL 导出函数声明
/// @details 定义多语言资源 DLL 的导出宏和 Read_xml 函数声明,
///          用于从资源中读取 XML 格式的语言文件
/// @author PhotoNest Team
/// @date 2024
/// @version 1.0
/// @note 本文件是 locales DLL 的头文件
///////////////////////////////////////////////////////////////////////

#ifndef LOCALES_H
#define LOCALES_H

#define LOCALES_EXPORTS

#ifdef LOCALES_EXPORTS
#define LOCALES_API __declspec(dllexport)
#else
#define LOCALES_API __declspec(dllimport)
#endif


LOCALES_API long Read_xml(unsigned int cr_id, const wchar_t* filename, char** szhtml);
