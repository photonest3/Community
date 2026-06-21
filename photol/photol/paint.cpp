// ============================================================
// paint.cpp : PhotoNest 绘图工具主程序入口
// 功能：通过命令行参数启动 paint 绘图窗口，并使用互斥量防止多开
// ============================================================

#include <tchar.h>
#include <windows.h>

typedef unsigned int       uint32_t;   // 32位无符号整数类型别名
typedef unsigned long long uint64_t;   // 64位无符号整数类型别名

#include "../../photol/photol/photol.h" // 引入 photol.dll 导出函数

// 使用 Windows 子系统入口，避免弹出控制台窗口
#pragma comment(linker, "/subsystem:windows /ENTRY:wmainCRTStartup")
// 链接 photol 导入库
#pragma comment( lib, "../../../PhotosVO/tests/cefclient/Release/bin/photol.lib" )

// ------------------------------------------------------------
// 主函数：解析命令行参数并启动 paint 窗口
// 命令行参数顺序：
// argv[1]=父窗口句柄, argv[2]=类型, argv[3]=DPI,
// argv[4]=文件路径, argv[5]=语言, argv[6]=用户,
// argv[7]=加密密钥, argv[8]=授权标记
// ------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	if (argc == 9)
	{
		// 获取当前程序所在目录，用于构建唯一互斥量名称
		HMODULE hModule = ::GetModuleHandle(NULL);
		wchar_t buf[MAX_PATH] = { 0 };
		GetModuleFileNameW(hModule, buf, MAX_PATH);
		wchar_t* p = wcsrchr(buf, L'\\');
		if (p != NULL)
		{
			p[0] = 0;
		}

		// 创建基于用户和路径的唯一互斥量名称，防止同一用户多开
		wchar_t buf2[MAX_PATH] = { 0 };
		swprintf_s(buf2, MAX_PATH, L"mypaint_%s_%d", argv[6], (int)wcslen(buf));

		HANDLE paint_mgr = NULL;
		paint_mgr = CreateMutex(
			(LPSECURITY_ATTRIBUTES)NULL,
			(BOOL)TRUE,
			buf2
		);

		DWORD dwLastError = GetLastError();

		if (paint_mgr == NULL || dwLastError == ERROR_ALREADY_EXISTS)
		{
			// 互斥量已存在，说明已有实例在运行（此处省略激活已有窗口的代码）
		}
		else
		{
			// 解析命令行参数并调用 photol.dll 的 paint_show 打开绘图窗口
			uint64_t hParent = (uint64_t)_wtoi64(argv[1]);
			int type = (int)_wtoi(argv[2]);
			int pixelsy = (int)_wtoi(argv[3]);
			int ov = (int)_wtoi(argv[8]);
			int ret = paint_show(type, hParent, pixelsy, 1152, 768, argv[4], argv[5], argv[6], argv[7], ov);
		}

		if (paint_mgr != NULL)
		{
			CloseHandle(paint_mgr);
		}
	}
	return 0;
}

