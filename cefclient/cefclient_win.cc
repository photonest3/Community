// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <windows.h>

#include <memory>

#include "include/cef_command_line.h"
#include "include/cef_sandbox_win.h"
#include "tests/cefclient/browser/main_context_impl.h"
#include "tests/cefclient/browser/main_message_loop_multithreaded_win.h"
#include "tests/cefclient/browser/root_window_manager.h"
#include "tests/cefclient/browser/test_runner.h"
#include "tests/shared/browser/client_app_browser.h"
#include "tests/shared/browser/main_message_loop_external_pump.h"
#include "tests/shared/browser/main_message_loop_std.h"
#include "tests/shared/common/client_app_other.h"
#include "tests/shared/common/client_switches.h"
#include "tests/shared/renderer/client_app_renderer.h"
#include "../scheme/scheme/scheme.h" 

//20230218
#include "tests/shared/browser/util_win.h"
#include "tests/cefclient/resources/win/resource.h"
#include <shellapi.h>
#include <shlobj.h>
#include "tests/cefclient/browser/timestamp.h"
#include "tests/cefclient/browser/IsoHandler.h"


// When generating projects with CMake the CEF_USE_SANDBOX value will be defined
// automatically if using the required compiler version. Pass -DUSE_SANDBOX=OFF
// to the CMake command-line to disable use of the sandbox.
// Uncomment this line to manually enable sandbox support.
 //#define CEF_USE_SANDBOX 1

#if defined(CEF_USE_SANDBOX)
// The cef_sandbox.lib static library may not link successfully with all VS
// versions.
#pragma comment( lib, "cef_sandbox.lib")
#endif

namespace client {
	namespace {
		HANDLE		g_hApp_Mutex_mgr = NULL;

		int RunMain(HINSTANCE hInstance, int nCmdShow) {
			// Enable High-DPI support on Windows 7 or newer.
			CefEnableHighDPISupport();

			CefMainArgs main_args(hInstance);

			void* sandbox_info = nullptr;

#if defined(CEF_USE_SANDBOX)
			// Manage the life span of the sandbox information object. This is necessary
			// for sandbox support on Windows. See cef_sandbox_win.h for complete details.
			CefScopedSandboxInfo scoped_sandbox;
			sandbox_info = scoped_sandbox.sandbox_info();
#endif

			// Parse command-line arguments.
			CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
			command_line->InitFromString(::GetCommandLineW());

			// Create a ClientApp of the correct type.
			CefRefPtr<CefApp> app;
			ClientApp::ProcessType process_type = ClientApp::GetProcessType(command_line);
			if (process_type == ClientApp::BrowserProcess)
				app = new ClientAppBrowser();
			else if (process_type == ClientApp::RendererProcess)
				app = new ClientAppRenderer();
			else if (process_type == ClientApp::OtherProcess)
				app = new ClientAppOther();

			// Execute the secondary process, if any.
			int exit_code = CefExecuteProcess(main_args, app, sandbox_info);
			if (exit_code >= 0)
				return exit_code;

			const std::wstring& window_no = GetResourceString(IDS_APP_NO);
			std::string command_args = "";
			std::wstring window_mutex = L"PHOTONESTMUTEX_";


			//20230218 CreateMutex
			std::wstring title = L"PhotoNest";
			wstring album = L"public";
			int type = 2;

			if (window_no == L"pvt")
			{
				command_args = "enc";
				type = 3;
				album = L"private";
				window_mutex += L"3";
				title = L"PhotoNest Encryption";
			}

			//Replace(window_mutex, L"\\", L"");
			//Replace(window_mutex, L":", L"");
			if (command_line->HasArguments())
			{
				string args = command_line->GetSwitchValue(switches::kUrl);
				string::size_type pos = args.find("jump=1");
				if (pos == string::npos)
				{
					command_args += "viewer";
					type = 1;
					window_mutex += L"1";
					title = L"PhotoNest Viewer";
					if (window_no == L"pvt") {
						title = L"PhotoNest Viewer2";
					}
				}
			}

			std::wstring dst = L"";
			wchar_t* szDocument;
			if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &szDocument)))
			{
				dst = szDocument;
				wstring p = get_module_path();
				wstring::size_type pos = p.find(L"\\windowsapps\\photonest");
				if (pos != wstring::npos)
				{
					dst += L"\\PhotoNest_App";
					window_mutex = L"O_" + window_mutex;
					command_args = "O_" + command_args;
				}
				else
				{
					dst += L"\\PhotoNest";
				}
				CreateDirectory(dst.c_str(), NULL);

				if (pos != wstring::npos)
				{
					if (type == 1)
					{
						dst += L"\\Viewer";
					}
					else
					{
						dst += L"\\Nest1";
					}
				}
				else
				{
					if (type == 1)
					{
						dst += L"\\Viewer";
					}
					else if (type == 2)
					{
						dst += L"\\Nest1";
					}
					else
					{
						dst += L"\\Nest2";
					}
				}

				CreateDirectory(dst.c_str(), NULL);
				CoTaskMemFree(szDocument);
			}

			/*
			HMODULE hModule = ::GetModuleHandle(NULL);

			wchar_t buf[MAX_PATH] = { 0 };
			GetModuleFileNameW(hModule, buf, MAX_PATH);

			DWORD dwAttr = GetFileAttributes(buf);
			if (dwAttr != -1)
			{
				// found something; better not be a folder....
				if (dwAttr & FILE_ATTRIBUTE_READONLY)
				{
					window_mutex += L"O";
					command_args = "O_";
				}
			}

			  wchar_t* p = wcsrchr(buf, L'\\');
			  if (p != NULL)
			  {
				  p[0] = 0;
			  }

			  p = wcsrchr(buf, L'\\');
			  if (p != NULL)
			  {
				  p[0] = 0;
			  }

			  //wstring pName = buf + wstring(L"\\data1\\_db\\archive.ini");
			  //if (window_no == L"pvt")
			  //{
			  //	pName = buf + wstring(L"\\data2\\_db\\archive.ini");
			  //}
		   //
			  //dwAttr = GetFileAttributes(pName.c_str());
			  //if (dwAttr != -1)
			  //{
			  //	window_mutex += L"2";
			  //	command_args += "2_";
			  //}
		  */

			if (command_args.size() < 3)
			{
				command_args += "index";
			}

			wchar_t buf_mutex[MAX_PATH] = { 0 };
			wcscpy_s(buf_mutex, MAX_PATH, window_mutex.c_str());

			IsoHandler::Instance().set_app_path(type, album);

			g_hApp_Mutex_mgr = CreateMutex(
				(LPSECURITY_ATTRIBUTES)NULL, // pointer to security attributes 
				(BOOL)TRUE, // flag for initial ownership 
				buf_mutex // pointer to mutex-object name 
			);

			DWORD dwLastError = GetLastError();

			// If cannot create Mutex, exit.
			if (g_hApp_Mutex_mgr == NULL)
			{
				return 0;
			}
			//LOG(ERROR) << "dwLastError" << dwLastError;

			// If Mutex already existed, exit.
			if (dwLastError == ERROR_ALREADY_EXISTS)
			{
				//LOG(ERROR) << "ERROR_ALREADY_EXISTS";
				HWND hWnd = NULL;

				HWND child = NULL;
				do {
					child = FindWindowEx(NULL, child, NULL, NULL);

					wchar_t buf[MAX_PATH] = { 0 };
					::GetWindowText(child, buf, MAX_PATH);

					if (wcsstr(buf, title.c_str()) != NULL)
					{
						char lpBuffer[1024] = { 0 };

						ATOM htemp = (ATOM)GetPropA(child, "oo");
						if (htemp != 0)
						{
							GlobalGetAtomNameA(htemp, lpBuffer, 1024);
							DeleteAtom(htemp);
						}

						if (command_args == lpBuffer)
						{
							hWnd = child;
							break;
						}
					}

				} while (child);

				if (hWnd == NULL) {
					return 0;
				}

				if (command_line->HasArguments())
				{
					UINT WM_RUNINFO = WM_USER + 100;
					if (type == 1)
					{
						std::string url = "http://nest1.pc/view.html?path=";
						if (window_no == L"pvt")
						{
							url = "http://nest2.pc/view.html?path=";
						}

						vector<CefString> arguments;
						command_line->GetArguments(arguments);
						url += arguments[0];

						CefString tmp;
						tmp.FromString(url);

						ATOM atom = GlobalAddAtom(tmp.ToWString().c_str());

						PostMessageW(hWnd, WM_RUNINFO, 0, (LPARAM)atom);
					}
					else if (type == 2)
					{
						string args = command_line->GetSwitchValue(switches::kUrl);
						string::size_type pos = args.find("jump=1");
						if (pos != string::npos)
						{
							ATOM atom = GlobalAddAtom(L"0");
							PostMessageW(hWnd, WM_RUNINFO, 0, (LPARAM)atom);
						}
					}
				}

				if (IsIconic(hWnd))
				{
					ShowWindow(hWnd, SW_RESTORE);
				}
				SetForegroundWindow(hWnd);
				return 0;
			}

			// Create the main context object.
			auto context = std::make_unique<MainContextImpl>(command_line, true);

			CefSettings settings;

#if !defined(CEF_USE_SANDBOX)
			settings.no_sandbox = true;
#endif

			// Populate the settings based on command line arguments.
			context->PopulateSettings(&settings);

			//IsoHandler::Instance().debug_ = true;
			//20230218 log_severity
			if (IsoHandler::Instance().debug_)
			{
				settings.log_severity = LOGSEVERITY_ERROR;
			}
			else
			{
				settings.log_severity = LOGSEVERITY_DISABLE;
			}


			dst += L"\\debug.log";
			CefString(&settings.log_file) = dst;

			// Create the main message loop object.
			std::unique_ptr<MainMessageLoop> message_loop;
			if (settings.multi_threaded_message_loop)
				message_loop.reset(new MainMessageLoopMultithreadedWin);
			else if (settings.external_message_pump)
				message_loop = MainMessageLoopExternalPump::Create();
			else
				message_loop.reset(new MainMessageLoopStd);

			// Initialize CEF.
			context->Initialize(main_args, settings, app, sandbox_info);

			// Register scheme handlers.
			test_runner::RegisterSchemeHandlers();

			auto window_config = std::make_unique<RootWindowConfig>();
			window_config->always_on_top =
				command_line->HasSwitch(switches::kAlwaysOnTop);
			window_config->with_controls =
				!command_line->HasSwitch(switches::kHideControls);
			window_config->with_osr =
				settings.windowless_rendering_enabled ? true : false;

			//20230218
			window_config->command_args = command_args;

			// Create the first window.
			context->GetRootWindowManager()->CreateRootWindow(std::move(window_config));

			// Run the message loop. This will block until Quit() is called by the
			// RootWindowManager after all windows have been destroyed.
			int result = message_loop->Run();

			// Shut down CEF.
			context->Shutdown();

			// Release objects in reverse order of creation.
			message_loop.reset();
			context.reset();

			return result;
		}

	}  // namespace
}  // namespace client


BOOL WINAPI HandlerRoutine(DWORD dwCtrlType) {
	switch (dwCtrlType) {
	case CTRL_C_EVENT:
		// 处理Ctrl+C事件
		break;
	case CTRL_BREAK_EVENT:
		// 处理Ctrl+Break事件
		break;
	case CTRL_CLOSE_EVENT:
		// 处理关闭控制台窗口事件
		break;
	default:
		break;
	}
	ShowWindow(::FindWindow(L"Shell_TrayWnd", NULL), SW_SHOW);

	// 执行一些清理工作或保存数据等操作

	return FALSE;  // 返回FALSE表示继续执行默认的处理方式
}

// Program entry point function.
int APIENTRY wWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine,
	int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	SetConsoleCtrlHandler(HandlerRoutine, TRUE);

	return client::RunMain(hInstance, nCmdShow);
}
