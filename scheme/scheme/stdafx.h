////////////////////////////////////////////////////////////////////
/// @file stdafx.h
/// @brief 标准系统包含文件（预编译头文件）
/// @details 此文件用于包含系统头文件和项目特定的常用头文件，
///          这些头文件使用频率高但修改频率低，通过预编译头
///          机制可以显著加快编译速度。
/// @note 这是 Visual Studio 自动生成的预编译头文件，不建议手动修改
/// @author PhotoNest Team
/// @date 2024
////////////////////////////////////////////////////////////////////

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>



// TODO: reference additional headers your program requires here
#include <time.h>
#include <tchar.h>

#define TIXML_USE_STL

//#define MP4V2_EXPORTS
