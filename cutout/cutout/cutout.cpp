// 
//

#include <tchar.h>
#include <windows.h>
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

#include "../../photol/photol/photol.h"


//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#pragma comment(linker, "/subsystem:windows /ENTRY:wmainCRTStartup")

#pragma comment( lib, "../../../PhotoNest/tests/cefclient/Release/bin/photol.lib" ) 

int _tmain(int argc, _TCHAR* argv[])
{
	wchar_t buf[1024] = { 0 };

	if (argc > 1)
	{
		wcscpy_s(buf, argv[1]);
	}


	HDC hDC = CreateCompatibleDC(0);
	int pixelsy = GetDeviceCaps(hDC, LOGPIXELSY);
	DeleteDC(hDC);
	int ret = paint_show(0, 0, pixelsy, 1152, 768, buf, L"", L"", L"", 0);
	return 0;
}

