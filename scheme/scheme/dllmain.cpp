///////////////////////////////////////////////////////////////////////
/// @file dllmain.cpp
/// @brief DLL 模块入口点实现文件
/// @details 实现 DLL 的入口函数，处理进程/线程的附加和分离事件
/// @author PhotoNest Team
/// @date 2024
/// @version 1.0
/// @note 本文件是 Windows DLL 的标准入口点
///////////////////////////////////////////////////////////////////////

// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

