// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "tests/cefclient/browser/root_window_win.h"
#include "include/base/cef_build.h"
#include "include/base/cef_callback.h"
#include "include/cef_app.h"
#include "tests/cefclient/browser/browser_window_osr_win.h"
#include "tests/cefclient/browser/browser_window_std_win.h"
#include "tests/cefclient/browser/main_context.h"
#include "tests/cefclient/browser/temp_window.h"
#include "tests/cefclient/browser/window_test_runner_win.h"
#include "tests/shared/browser/geometry_util.h"
#include "tests/shared/browser/main_message_loop.h"
#include "tests/shared/browser/util_win.h"
#include "tests/shared/common/client_switches.h"
#include "tests/cefclient/resources/win/resource.h"
#include "tests/cefclient/browser/Versionhelper.h"
#include "include/capi/cef_parser_capi.h"

#include <shellscalingapi.h>
#include <process.h>
#include <shlobj.h>
#include <shellapi.h>
#include "../../scheme/scheme/scheme.h" 

#include "lunasvg.h"
#include "timestamp.h"

#pragma comment( lib, "../../../PhotoNest/tests/cefclient/Release/bin/lunasvg.lib" ) 


#define MAX_URL_LENGTH 255
#define BUTTON_WIDTH 72
#define URLBAR_HEIGHT 24

namespace client {

namespace {

// Message handler for the About box.
INT_PTR CALLBACK AboutWndProc(HWND hDlg,
                              UINT message,
                              WPARAM wParam,
                              LPARAM lParam) {
  UNREFERENCED_PARAMETER(lParam);
  switch (message) {
    case WM_INITDIALOG:
      return TRUE;

    case WM_COMMAND:
      if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
        EndDialog(hDlg, LOWORD(wParam));
        return TRUE;
      }
      break;
  }
  return FALSE;
}

// Returns true if the process is per monitor DPI aware.
bool IsProcessPerMonitorDpiAware() {
  enum class PerMonitorDpiAware {
    UNKNOWN = 0,
    PER_MONITOR_DPI_UNAWARE,
    PER_MONITOR_DPI_AWARE,
  };
  static PerMonitorDpiAware per_monitor_dpi_aware = PerMonitorDpiAware::UNKNOWN;
  if (per_monitor_dpi_aware == PerMonitorDpiAware::UNKNOWN) {
    per_monitor_dpi_aware = PerMonitorDpiAware::PER_MONITOR_DPI_UNAWARE;
    HMODULE shcore_dll = ::LoadLibrary(L"shcore.dll");
    if (shcore_dll) {
      typedef HRESULT(WINAPI * GetProcessDpiAwarenessPtr)(
          HANDLE, PROCESS_DPI_AWARENESS*);
      GetProcessDpiAwarenessPtr func_ptr =
          reinterpret_cast<GetProcessDpiAwarenessPtr>(
              ::GetProcAddress(shcore_dll, "GetProcessDpiAwareness"));
      if (func_ptr) {
        PROCESS_DPI_AWARENESS awareness;
        if (SUCCEEDED(func_ptr(nullptr, &awareness)) &&
            awareness == PROCESS_PER_MONITOR_DPI_AWARE)
          per_monitor_dpi_aware = PerMonitorDpiAware::PER_MONITOR_DPI_AWARE;
      }
    }
  }
  return per_monitor_dpi_aware == PerMonitorDpiAware::PER_MONITOR_DPI_AWARE;
}

// DPI value for 1x scale factor.
#define DPI_1X 96.0f

float GetWindowScaleFactor(HWND hwnd) {
  if (hwnd && IsProcessPerMonitorDpiAware()) {
    typedef UINT(WINAPI * GetDpiForWindowPtr)(HWND);
    static GetDpiForWindowPtr func_ptr = reinterpret_cast<GetDpiForWindowPtr>(
        GetProcAddress(GetModuleHandle(L"user32.dll"), "GetDpiForWindow"));
    if (func_ptr)
      return static_cast<float>(func_ptr(hwnd)) / DPI_1X;
  }

  return client::GetDeviceScaleFactor();
}

int GetButtonWidth(HWND hwnd) {
  //return LogicalToDevice(BUTTON_WIDTH, GetWindowScaleFactor(hwnd));
  return LogicalToDevice(32, GetWindowScaleFactor(hwnd));
}

int GetURLBarHeight(HWND hwnd) {
    int h = LogicalToDevice(URLBAR_HEIGHT, GetWindowScaleFactor(hwnd));
    //LOG(ERROR) << "GetURLBarHeight:" << h;

    return h;
    //return LogicalToDevice(URLBAR_HEIGHT, GetWindowScaleFactor(hwnd));
}

}  // namespace

RootWindowWin::RootWindowWin()
	: with_controls_(false),
	always_on_top_(false),
	with_osr_(false),
	with_extension_(false),
	is_popup_(false),
	start_rect_(),
	initialized_(false),
	hwnd_(nullptr),
	draggable_region_(nullptr),
	font_(nullptr),
	font_height_(0),
	back_hwnd_(nullptr),
	forward_hwnd_(nullptr),
	reload_hwnd_(nullptr),
	stop_hwnd_(nullptr),
	edit_hwnd_(nullptr),
	edit_wndproc_old_(nullptr),
	find_hwnd_(nullptr),
	find_message_id_(0),
	find_wndproc_old_(nullptr),
	find_state_(),
	find_next_(false),
	find_match_case_last_(false),
	window_destroyed_(false),
	browser_destroyed_(false),


	isLoading_(false),
	_hBmpReload(NULL),
	_hBmpTool(NULL),
	_hBmpBack(NULL),
	_hBmpForward(NULL),

	_hBmpReload_over(NULL),
	_hBmpTool_over(NULL),
	_hBmpBack_over(NULL),
	_hBmpForward_over(NULL),

	_hBmpBack_gray(NULL),
	_hBmpForward_gray(NULL),
	_hBmpReload_gray(NULL),

	_tool_hwnd(NULL),
	_pMainMenu(0),
	_hMenuBrush(NULL),
	_hToolBarBrush(NULL),
	_brBtnShadow(NULL),
	_btnEntry(FALSE),


	_painting(false),
	_uploading(0),

	_hwndPaint(NULL),
	_backWndProc(NULL),
	_forwardWndProc(NULL),
	_reloadWndProc(NULL),
	_stopWndProc(NULL),
	_toolWndProc(NULL),

	_currBtnID(0),
	_lasturl(""),
	_CURR_ZOOM_MENUID(ID_PICOFU_ZOOM_100),





	called_enable_non_client_dpi_scaling_(false) {
	find_buff_[0] = 0;

	// Create a HRGN representing the draggable window area.
	draggable_region_ = ::CreateRectRgn(0, 0, 0, 0);
}

RootWindowWin::~RootWindowWin() {
	REQUIRE_MAIN_THREAD();

	::DeleteObject(draggable_region_);
	::DeleteObject(font_);

	// The window and browser should already have been destroyed.
	DCHECK(window_destroyed_);
	DCHECK(browser_destroyed_);



	if (_hBmpReload != NULL)
	{
		DeleteObject(_hBmpReload);
	}
	if (_hBmpTool != NULL)
	{
		DeleteObject(_hBmpTool);
	}
	if (_hBmpBack != NULL)
	{
		DeleteObject(_hBmpBack);
	}
	if (_hBmpForward != NULL)
	{
		DeleteObject(_hBmpForward);
	}


	if (_hBmpReload_over != NULL)
	{
		DeleteObject(_hBmpReload_over);
	}
	if (_hBmpTool_over != NULL)
	{
		DeleteObject(_hBmpTool_over);
	}
	if (_hBmpBack_over != NULL)
	{
		DeleteObject(_hBmpBack_over);
	}
	if (_hBmpForward_over != NULL)
	{
		DeleteObject(_hBmpForward_over);
	}

	if (_hBmpBack_gray != NULL)
	{
		DeleteObject(_hBmpBack_gray);
	}
	if (_hBmpForward_gray != NULL)
	{
		DeleteObject(_hBmpForward_gray);
	}

	if (_hBmpReload_gray != NULL)
	{
		DeleteObject(_hBmpReload_gray);
	}




	if (_hMenuBrush != NULL)
	{
		DeleteObject(_hMenuBrush);
	}

	if (_hToolBarBrush != NULL)
	{
		DeleteObject(_hToolBarBrush);
	}

	if (_brBtnShadow != NULL)
	{
		DeleteObject(_brBtnShadow);
	}
}

void RootWindowWin::Init(RootWindow::Delegate* delegate,
                         std::unique_ptr<RootWindowConfig> config,
                         const CefBrowserSettings& settings) {
  DCHECK(delegate);
  DCHECK(!initialized_);

  command_args_ = config->command_args;

  delegate_ = delegate;
  with_controls_ = config->with_controls;
  always_on_top_ = config->always_on_top;
  with_osr_ = config->with_osr;
  with_extension_ = config->with_extension;

  start_rect_.left = config->bounds.x;
  start_rect_.top = config->bounds.y;
  start_rect_.right = config->bounds.x + config->bounds.width;
  start_rect_.bottom = config->bounds.y + config->bounds.height;

  CreateBrowserWindow(config->url);

  initialized_ = true;

  // Create the native root window on the main thread.
  if (CURRENTLY_ON_MAIN_THREAD()) {
    CreateRootWindow(settings, config->initially_hidden);
  } else {
    MAIN_POST_CLOSURE(base::BindOnce(&RootWindowWin::CreateRootWindow, this,
                                     settings, config->initially_hidden));
  }
}

void RootWindowWin::InitAsPopup(RootWindow::Delegate* delegate,
                                bool with_controls,
                                bool with_osr,
                                const CefPopupFeatures& popupFeatures,
                                CefWindowInfo& windowInfo,
                                CefRefPtr<CefClient>& client,
                                CefBrowserSettings& settings) {
  CEF_REQUIRE_UI_THREAD();

  DCHECK(delegate);
  DCHECK(!initialized_);

  delegate_ = delegate;
  with_controls_ = with_controls;
  with_osr_ = with_osr;
  is_popup_ = true;

  if (popupFeatures.xSet)
    start_rect_.left = popupFeatures.x;
  if (popupFeatures.ySet)
    start_rect_.top = popupFeatures.y;
  if (popupFeatures.widthSet)
    start_rect_.right = start_rect_.left + popupFeatures.width;
  if (popupFeatures.heightSet)
    start_rect_.bottom = start_rect_.top + popupFeatures.height;

  CreateBrowserWindow(std::string());

  initialized_ = true;

  // The new popup is initially parented to a temporary window. The native root
  // window will be created after the browser is created and the popup window
  // will be re-parented to it at that time.
  browser_window_->GetPopupConfig(TempWindow::GetWindowHandle(), windowInfo,
                                  client, settings);
}

void RootWindowWin::Show(ShowMode mode) {
  REQUIRE_MAIN_THREAD();

  if (!hwnd_)
    return;

  int nCmdShow = SW_SHOWNORMAL;
  switch (mode) {
    case ShowMinimized:
      nCmdShow = SW_SHOWMINIMIZED;
      break;
    case ShowMaximized:
      nCmdShow = SW_SHOWMAXIMIZED;
      break;
    case ShowNoActivate:
      nCmdShow = SW_SHOWNOACTIVATE;
      break;
    default:
      break;
  }

  ShowWindow(hwnd_, nCmdShow);
  UpdateWindow(hwnd_);
}

void RootWindowWin::Hide() {
  REQUIRE_MAIN_THREAD();

  if (hwnd_)
    ShowWindow(hwnd_, SW_HIDE);
}

void RootWindowWin::SetBounds(int x, int y, size_t width, size_t height) {
  REQUIRE_MAIN_THREAD();

  if (hwnd_) {
    SetWindowPos(hwnd_, nullptr, x, y, static_cast<int>(width),
                 static_cast<int>(height), SWP_NOZORDER);
  }
}

void RootWindowWin::Close(bool force) {
  REQUIRE_MAIN_THREAD();

  if (hwnd_) {
    if (force)
      DestroyWindow(hwnd_);
    else
      PostMessage(hwnd_, WM_CLOSE, 0, 0);
  }
}

void RootWindowWin::SetDeviceScaleFactor(float device_scale_factor) {
  REQUIRE_MAIN_THREAD();

  if (browser_window_ && with_osr_)
    browser_window_->SetDeviceScaleFactor(device_scale_factor);
}

float RootWindowWin::GetDeviceScaleFactor() const {
  REQUIRE_MAIN_THREAD();

  if (browser_window_ && with_osr_)
    return browser_window_->GetDeviceScaleFactor();

  NOTREACHED();
  return 0.0f;
}

CefRefPtr<CefBrowser> RootWindowWin::GetBrowser() const {
  REQUIRE_MAIN_THREAD();

  if (browser_window_)
    return browser_window_->GetBrowser();
  return nullptr;
}

ClientWindowHandle RootWindowWin::GetWindowHandle() const {
  REQUIRE_MAIN_THREAD();
  return hwnd_;
}

bool RootWindowWin::WithWindowlessRendering() const {
  REQUIRE_MAIN_THREAD();
  return with_osr_;
}

bool RootWindowWin::WithExtension() const {
  REQUIRE_MAIN_THREAD();
  return with_extension_;
}



void RootWindowWin::CreateBrowserWindow(const std::string& startup_url) {
  if (with_osr_) {
    OsrRendererSettings settings = {};
    MainContext::Get()->PopulateOsrSettings(&settings);
    browser_window_.reset(
        new BrowserWindowOsrWin(this, with_controls_, startup_url, settings));
  } else {
    browser_window_.reset(
        new BrowserWindowStdWin(this, with_controls_, startup_url));
  }
}

void RootWindowWin::CreateRootWindow(const CefBrowserSettings& settings,
	bool initially_hidden) {
	REQUIRE_MAIN_THREAD();
	DCHECK(!hwnd_);

	HINSTANCE hInstance = GetModuleHandle(nullptr);

	// Load strings from the resource file.
	const std::wstring& window_title = GetResourceString(IDS_APP_TITLE);
	const std::wstring& window_class = GetResourceString(IDC_CEFCLIENT);

	const cef_color_t background_color = MainContext::Get()->GetBackgroundColor();
	const HBRUSH background_brush = CreateSolidBrush(
		RGB(CefColorGetR(background_color), CefColorGetG(background_color),
			CefColorGetB(background_color)));

	// Register the window class.
	RegisterRootClass(hInstance, window_class, background_brush);

	// Register the message used with the find dialog.
	find_message_id_ = RegisterWindowMessage(FINDMSGSTRING);
	CHECK(find_message_id_);

	CefRefPtr<CefCommandLine> command_line =
		CefCommandLine::GetGlobalCommandLine();
	const bool no_activate = command_line->HasSwitch(switches::kNoActivate);

	const DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
	DWORD dwExStyle = always_on_top_ ? WS_EX_TOPMOST : 0;
	if (no_activate) {
		// Don't activate the browser window on creation.
		dwExStyle |= WS_EX_NOACTIVATE;
	}



	//20230218 show
	int zoomed = 0;
	RECT rcWin;
	int pub_default = 1;

    IsoHandler::Instance().get_prof_rcwin(zoomed, pub_default, rcWin);

	if (rcWin.right > 0)
	{
		//int  cy = GetSystemMetrics(SM_CYFULLSCREEN);
		//if (rcWin.bottom > cy)
		//{
		//    rcWin.top = 0;
		//    rcWin.bottom = cy;
		//    nMax = 1;
		//}

		start_rect_.left = rcWin.left;
		start_rect_.top = rcWin.top;
		start_rect_.right = rcWin.right;
		start_rect_.bottom = rcWin.bottom;
	}

	int x, y, width, height;
	if (::IsRectEmpty(&start_rect_)) {
		// Use the default window position/size.
		x = y = width = height = CW_USEDEFAULT;
	}
	else {
		// Adjust the window size to account for window frame and controls.
		RECT window_rect = start_rect_;
        // 20230218
              //::AdjustWindowRectEx(&window_rect, dwStyle, with_controls_, dwExStyle);

		x = start_rect_.left;
		y = start_rect_.top;
		width = window_rect.right - window_rect.left;
		height = window_rect.bottom - window_rect.top;
	}

	browser_settings_ = settings;

	// Create the main window initially hidden.
	CreateWindowEx(dwExStyle, window_class.c_str(), window_title.c_str(), dwStyle,
		x, y, width, height, nullptr, nullptr, hInstance, this);
	CHECK(hwnd_);

	if (!called_enable_non_client_dpi_scaling_ && IsProcessPerMonitorDpiAware()) {
		// This call gets Windows to scale the non-client area when WM_DPICHANGED
		// is fired on Windows versions < 10.0.14393.0.
		// Derived signature; not available in headers.
		typedef LRESULT(WINAPI * EnableChildWindowDpiMessagePtr)(HWND, BOOL);
		static EnableChildWindowDpiMessagePtr func_ptr =
			reinterpret_cast<EnableChildWindowDpiMessagePtr>(GetProcAddress(
				GetModuleHandle(L"user32.dll"), "EnableChildWindowDpiMessage"));
		if (func_ptr)
			func_ptr(hwnd_, TRUE);
	}
	IsoHandler::Instance().root_hwnd_ = hwnd_;

	//20230218
	if (zoomed == 1)
	{
		Show(ShowMaximized);
	}
	else
	{
		if (!initially_hidden) {
			// Show this window.
			Show(no_activate ? ShowNoActivate : ShowNormal);
		}
	}

	IsoHandler::Instance().ShowDialogBox(hwnd_, IDD_DIALOG_LOADING, 0);
	return;



	if (!initially_hidden) {
		// Show this window.
		Show(no_activate ? ShowNoActivate : ShowNormal);
	}
}

// static
void RootWindowWin::RegisterRootClass(HINSTANCE hInstance,
                                      const std::wstring& window_class,
                                      HBRUSH background_brush) {
  // Only register the class one time.
  static bool class_registered = false;
  if (class_registered)
    return;
  class_registered = true;


  WNDCLASSEX wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = RootWndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CEFCLIENT));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = background_brush;

  //20230218 RegisterRootClass
  wcex.lpszMenuName = nullptr;// MAKEINTRESOURCE(IDC_CEFCLIENT);



  wcex.lpszClassName = window_class.c_str();
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  RegisterClassEx(&wcex);
}

// static
LRESULT CALLBACK RootWindowWin::EditWndProc(HWND hWnd,
                                            UINT message,
                                            WPARAM wParam,
                                            LPARAM lParam) {
  REQUIRE_MAIN_THREAD();

  RootWindowWin* self = GetUserDataPtr<RootWindowWin*>(hWnd);
  DCHECK(self);
  DCHECK(hWnd == self->edit_hwnd_);

  switch (message) {
    case WM_CHAR:
      if (wParam == VK_RETURN) {
        // When the user hits the enter key load the URL.
        CefRefPtr<CefBrowser> browser = self->GetBrowser();
        if (browser) {
          wchar_t strPtr[MAX_URL_LENGTH + 1] = {0};
          *((LPWORD)strPtr) = MAX_URL_LENGTH;
          LRESULT strLen = SendMessage(hWnd, EM_GETLINE, 0, (LPARAM)strPtr);
          if (strLen > 0) {
            strPtr[strLen] = 0;
            browser->GetMainFrame()->LoadURL(strPtr);
          }
        }
        return 0;
      }
      break;
    case WM_NCDESTROY:
      // Clear the reference to |self|.
      SetUserDataPtr(hWnd, nullptr);
      self->edit_hwnd_ = nullptr;
      break;
  }

  return CallWindowProc(self->edit_wndproc_old_, hWnd, message, wParam, lParam);
}

// static
LRESULT CALLBACK RootWindowWin::FindWndProc(HWND hWnd,
                                            UINT message,
                                            WPARAM wParam,
                                            LPARAM lParam) {
  REQUIRE_MAIN_THREAD();

  RootWindowWin* self = GetUserDataPtr<RootWindowWin*>(hWnd);
  DCHECK(self);
  DCHECK(hWnd == self->find_hwnd_);

  switch (message) {
    case WM_ACTIVATE:
      // Set this dialog as current when activated.
      MainMessageLoop::Get()->SetCurrentModelessDialog(wParam == 0 ? nullptr
                                                                   : hWnd);
      return FALSE;
    case WM_NCDESTROY:
      // Clear the reference to |self|.
      SetUserDataPtr(hWnd, nullptr);
      self->find_hwnd_ = nullptr;
      break;
  }

  return CallWindowProc(self->find_wndproc_old_, hWnd, message, wParam, lParam);
}

// static
LRESULT CALLBACK RootWindowWin::RootWndProc(HWND hWnd,
                                            UINT message,
                                            WPARAM wParam,
                                            LPARAM lParam) {
  REQUIRE_MAIN_THREAD();

  RootWindowWin* self = nullptr;
  if (message != WM_NCCREATE) {
    self = GetUserDataPtr<RootWindowWin*>(hWnd);
    if (!self)
      return DefWindowProc(hWnd, message, wParam, lParam);
    DCHECK_EQ(hWnd, self->hwnd_);
  }

  if (self && message == self->find_message_id_) {
    // Message targeting the find dialog.
    LPFINDREPLACE lpfr = reinterpret_cast<LPFINDREPLACE>(lParam);
    CHECK(lpfr == &self->find_state_);
    self->OnFindEvent();
    return 0;
  }

  // Callback for the main window
  switch (message) {
    case WM_COMMAND:
      if (self->OnCommand(LOWORD(wParam), lParam))
        return 0;
      break;
      //20230218
    case WM_RUNINFO:
    {
        wchar_t lpBuffer[1024] = { 0 };

        ATOM hData = lParam;
        GlobalGetAtomName(hData, lpBuffer, 1024);
        DeleteAtom(hData);

        wstring s1 = lpBuffer;
		
		if(s1 == L"0")
		{
            string url = string("c.html?act=c.nb");
            char* szResponse = 0;
            char* szMimeType = 0;
            mygetfunc(url.c_str(), &szResponse, &szMimeType);
		}
		else
		{
			string::size_type pos = s1.find(L"path=");
			if (pos != string::npos)
			{
				s1 = s1.substr(pos + 5);
			}
			CefString tmp;
			tmp.FromWString(s1);

			if (CefRefPtr<CefBrowser> browser = self->GetBrowser())
			{
				string s2 = tmp;
				Replace(s2, "\\", "/");
				string js = "if(typeof selecthis === \"function\") selecthis(\"his=2&fname=" + s2 + "\");";

				browser->GetMainFrame()->ExecuteJavaScript(js, "", 0);
			}
		}

    }
    break;
    case WM_PAINT_INIT:
    {
        if (CefRefPtr<CefBrowser> browser = self->GetBrowser())
        {
            self->_painting = true;
            self->_hwndPaint = (HWND)lParam;
        }
    }
    break;
    case WM_CLOSE_UPLOADING:
    {
        self->_uploading = 0;
        PostMessage(hWnd, WM_CLOSE, 0, 0);
    }
    break;

    case WM_UPLOADING:
    {
        if (CefRefPtr<CefBrowser> browser = self->GetBrowser())
        {
            self->_uploading = (int)lParam;

            /*
                        if (self->_uploading == 0)
                        {
                            EnableWindow(self->back_hwnd_, true);
                            EnableWindow(self->forward_hwnd_, true);
                            EnableWindow(self->reload_hwnd_, true);
                        }
                        else
                        {
                            EnableWindow(self->back_hwnd_, false);
                            EnableWindow(self->forward_hwnd_, false);
                            EnableWindow(self->reload_hwnd_, false);
                        }
                        InvalidateRect(self->back_hwnd_, NULL, TRUE);
                        InvalidateRect(self->forward_hwnd_, NULL, TRUE);
                        InvalidateRect(self->reload_hwnd_, NULL, TRUE);
            */
        }
    }
    break;

    case WM_PAINT_CLOSE:
    {
        IsoHandler::Instance()._jsclose = 2;

        if (CefRefPtr<CefBrowser> browser = self->GetBrowser())
        {
            self->_painting = false;
            string js = "jshow_close();";
            browser->GetMainFrame()->ExecuteJavaScript(js, "", 0);
        }
    }
    break;
    case WM_PAINT_CLOSE0:
    {
		if (self->_painting)
		{
			PostMessage(self->_hwndPaint, WM_CEF_CLOSE, 0, 0);
		}
	}
    break;
    case WM_PAINT_MASK:
    {
        if (CefRefPtr<CefBrowser> browser = self->GetBrowser())
        {
            self->_painting = true;
            self->_hwndPaint = (HWND)lParam;
            string js = "jshow_mask();";
            browser->GetMainFrame()->ExecuteJavaScript(js, "", 0);
        }
    }
    break;
    case WM_TIMER:
    {
        if (CefRefPtr<CefBrowser> browser = self->GetBrowser())
        {
            if (self->_painting)
            {
                wstring  usr = L"0";
                if (IsoHandler::Instance().viewer_ == 3)
                {
                    usr = L"1";
                }

				HMODULE hModule = ::GetModuleHandle(NULL);
				wchar_t buf[MAX_PATH] = { 0 };
				GetModuleFileNameW(hModule, buf, MAX_PATH);// L"mypaint_mutex"
                wchar_t* p = wcsrchr(buf, L'\\');
                if (p != NULL)
                {
                    p[0] = 0;
                }

                wchar_t buf2[MAX_PATH] = { 0 };
                swprintf_s(buf2, MAX_PATH, L"mypaint_%s_%d", usr.c_str(), (int)wcslen(buf));

                HANDLE paint_mgr = NULL;
                paint_mgr = CreateMutex(
                    (LPSECURITY_ATTRIBUTES)NULL,
                    (BOOL)TRUE,
                    buf2
                );

                DWORD dwLastError = GetLastError();
                if (dwLastError != ERROR_ALREADY_EXISTS)
                {
                    string js = "jshow_close();";
                    browser->GetMainFrame()->ExecuteJavaScript(js, "", 0);
                    self->_painting = false;
                }

                if (paint_mgr != NULL)
                {
                    CloseHandle(paint_mgr);
                }
            }
        }
    }
    break;

    case WM_ACTIVATED:
    {
        if (CefRefPtr<CefBrowser> browser = self->GetBrowser())
        {
            browser->Reload();
        }
    }
    break;

    case WM_PAINT_RUN:
    {
        if (CefRefPtr<CefBrowser> browser = self->GetBrowser())
        {
            int id = (int)wParam;

            //self->_hwndPaint = (HWND)lParam;

            char buf[100] = { 0 };
            sprintf_s(buf, 100, "%d", id);

            string url = string("c.html?act=c.nav&id=") + buf;
            char* szResponse = 0;
            char* szMimeType = 0;
            mygetfunc(url.c_str(), &szResponse, &szMimeType);

            if (id != -1)
            {
                size_t len = strlen(szResponse) + 1;

                //HANDLE hMapFile = CreateFileMapping(
                //	INVALID_HANDLE_VALUE,    // ÎïÀíÎÄ¼þ¾ä±ú
                //	NULL,                    // Ä¬ÈÏ°²È«¼¶±ð
                //	PAGE_READWRITE/*|SEC_RESERVE*/,          // ¿É¶Á¿ÉÐ´
                //	0,                       // ¸ßÎ»ÎÄ¼þ´óÐ¡
                //	1024 * 64,                // µØÎ»ÎÄ¼þ´óÐ¡
                //	L"Paint_Mapping_Object"                   // ¹²ÏíÄÚ´æÃû³Æ
                //);

                //char *pBuf = (char *)MapViewOfFile(
                //	hMapFile,            // ¹²ÏíÄÚ´æµÄ¾ä±ú
                //	FILE_MAP_WRITE, // ¿É¶ÁÐ´Ðí¿É
                //	0,
                //	0,
                //	1024 * 64
                //);

                //UnmapViewOfFile(pBuf);
                //CloseHandle(hMapFile);
                COPYDATASTRUCT data;
                data.dwData = 1;
                data.cbData = (DWORD)len;
                data.lpData = szResponse;

                SendMessage(self->_hwndPaint, WM_COPYDATA, (WPARAM)self->hwnd_, reinterpret_cast<LPARAM>(&data));
            }

            LocalFree(szResponse);
        }
    }
    break;
    case WM_PAINT_SAVE:
    {
        if (CefRefPtr<CefBrowser> browser = self->GetBrowser())
        {
            int id = (int)wParam;

            char lpBuffer[100] = { 0 };

            ATOM hData = lParam;
            GlobalGetAtomNameA(hData, lpBuffer, 100);
            DeleteAtom(hData);

            //LOG(ERROR) << "WM_PAINT_SAVE:" << id << "," << lpBuffer;

            char buf[1024] = { 0 };
            sprintf_s(buf, "%d&%s", id, lpBuffer);

            string url = string("c.html?act=c.reload_img&id=") + buf;
            if (id != -1)
            {
                char* szResponse = 0;
                char* szMimeType = 0;
                mygetfunc(url.c_str(), &szResponse, &szMimeType);
            }


            memset(buf, 0, 1024);
            sprintf_s(buf, "reload_img(%d);", id);

            browser->GetMainFrame()->ExecuteJavaScript(buf, "", 0);
            //_painting = FALSE;
        }
    }
    break;
    case WM_SYSCOMMAND:
        if (self->OnSysCommand(hWnd, LOWORD(wParam)))
            return 0;
        break;


    case WM_GETOBJECT: {
      // Only the lower 32 bits of lParam are valid when checking the object id
      // because it sometimes gets sign-extended incorrectly (but not always).
      DWORD obj_id = static_cast<DWORD>(static_cast<DWORD_PTR>(lParam));

      // Accessibility readers will send an OBJID_CLIENT message.
      if (static_cast<DWORD>(OBJID_CLIENT) == obj_id) {
        if (self->GetBrowser() && self->GetBrowser()->GetHost())
          self->GetBrowser()->GetHost()->SetAccessibilityState(STATE_ENABLED);
      }
    } break;

    case WM_PAINT:
      self->OnPaint();
      return 0;

    case WM_ACTIVATE:
      self->OnActivate(LOWORD(wParam) != WA_INACTIVE);
      // Allow DefWindowProc to set keyboard focus.
      break;

    case WM_SETFOCUS:
      self->OnFocus();
      return 0;

    case WM_SIZE:
      self->OnSize(wParam == SIZE_MINIMIZED);
      break;

    case WM_MOVING:
    case WM_MOVE:
      self->OnMove();
      return 0;
    case WM_DPICHANGED:
      self->OnDpiChanged(wParam, lParam);
      break;

    case WM_ERASEBKGND:
      if (self->OnEraseBkgnd())
        break;
      // Don't erase the background.
      return 0;

    case WM_ENTERMENULOOP:
      if (!wParam) {
        // Entering the menu loop for the application menu.
        CefSetOSModalLoop(true);
      }
      break;

    case WM_EXITMENULOOP:
      if (!wParam) {
        // Exiting the menu loop for the application menu.
        CefSetOSModalLoop(false);
      }
      break;

    case WM_CLOSE:
        if (self->_painting)
        {
            //HANDLE hMapFile = CreateFileMapping(
            //	INVALID_HANDLE_VALUE,    // ÎïÀíÎÄ¼þ¾ä±ú
            //	NULL,                    // Ä¬ÈÏ°²È«¼¶±ð
            //	PAGE_READWRITE /*| SEC_RESERVE*/,          // ¿É¶Á¿ÉÐ´
            //	0,                       // ¸ßÎ»ÎÄ¼þ´óÐ¡
            //	1024 * 64,                // µØÎ»ÎÄ¼þ´óÐ¡
            //	L"Paint_Mapping_Object"                   // ¹²ÏíÄÚ´æÃû³Æ
            //);

            //char *pBuf = (char *)MapViewOfFile(
            //	hMapFile,            // ¹²ÏíÄÚ´æµÄ¾ä±ú
            //	FILE_MAP_WRITE, // ¿É¶ÁÐ´Ðí¿É
            //	0,
            //	0,
            //	1024 * 64
            //);

            //strncpy(pBuf, "close", 5);
            //pBuf[5] = 0;

            //UnmapViewOfFile(pBuf);
            //CloseHandle(hMapFile);
            PostMessage(self->_hwndPaint, WM_CEF_CLOSE, 0, 0);

        }
        
        if (self->OnClose(hWnd))
        return 0;  // Cancel the close.
      break;

    case WM_NCHITTEST: {
      LRESULT hit = DefWindowProc(hWnd, message, wParam, lParam);
      if (hit == HTCLIENT) {
        POINTS points = MAKEPOINTS(lParam);
        POINT point = {points.x, points.y};
        ::ScreenToClient(hWnd, &point);
        if (::PtInRegion(self->draggable_region_, point.x, point.y)) {
          // If cursor is inside a draggable region return HTCAPTION to allow
          // dragging.
          return HTCAPTION;
        }
      }
      return hit;
    }

    case WM_NCCREATE: {
      CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
      self = reinterpret_cast<RootWindowWin*>(cs->lpCreateParams);
      DCHECK(self);
      // Associate |self| with the main window.
      SetUserDataPtr(hWnd, self);
      self->hwnd_ = hWnd;

      self->OnNCCreate(cs);
    } break;

    case WM_CREATE:
    {
        HANDLE hData = (HANDLE)GlobalAddAtomA(self->command_args_.c_str());
        SetPropA(hWnd, "oo", hData);

        ::SetTimer(hWnd, 997, 300, NULL);

        self->OnCreate(reinterpret_cast<CREATESTRUCT*>(lParam));
    }
      break;

    case WM_NCDESTROY:
        RemovePropA(hWnd, "oo");
        
        // Clear the reference to |self|.
      SetUserDataPtr(hWnd, nullptr);
      self->hwnd_ = nullptr;
      self->OnDestroyed();
      break;
    case WM_DRAWITEM:
    {
        //20230218 WM_DRAWITEM
        LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;

        if (lpDIS->CtlID != IDC_NAV_BACK && lpDIS->CtlID != IDC_NAV_FORWARD &&
            lpDIS->CtlID != IDC_NAV_RELOAD && lpDIS->CtlID != IDC_NAV_STOP &&
            lpDIS->CtlID != IDS_MENU_ICON_1X_PNG)
        {
            return (0);
        }


        HDC dc = lpDIS->hDC;
        BOOL bIsPressed = (lpDIS->itemState & ODS_SELECTED);
        BOOL bIsFocused = false;
        BOOL bIsDisabled = (lpDIS->itemState & ODS_DISABLED);
        //BOOL bDrawFocusRect = !(lpDIS->itemState & ODS_NOFOCUSRECT);

        if (lpDIS->CtlID == IDC_NAV_BACK)
        {
            string::size_type pos1 = self->_lasturl.find("b.html");
            if (pos1 != string::npos)
            {
                bIsDisabled = TRUE;
                bIsPressed = FALSE;
            }

            pos1 = self->_lasturl.find("a.html");
            if (pos1 != string::npos)
            {
                bIsDisabled = TRUE;
                bIsPressed = FALSE;
            }
        }

        RECT itemRect = lpDIS->rcItem;

        SetBkMode(dc, TRANSPARENT);
        //if (bIsFocused)
        //{
        //	HBRUSH br = CreateSolidBrush(RGB(0, 0, 0));
        //	FrameRect(dc, &itemRect, br);
        //	InflateRect(&itemRect, -1, -1);
        //	DeleteObject(br);
        //}
        itemRect.left = itemRect.left - 4;
        itemRect.top = itemRect.top - 4;
        itemRect.right = itemRect.right + 4;
        itemRect.bottom = itemRect.bottom + 4;

        if (self->_btnEntry && !bIsDisabled && self->_currBtnID == lpDIS->CtlID)
        {
            FillRect(dc, &itemRect, self->_brBtnShadow);
            bIsFocused = true;
            //SelectObject(dc, self->_brBtnShadow);
            //Ellipse(dc, itemRect.left, itemRect.top, itemRect.right, itemRect.bottom);
        }
        else
        {
            FillRect(dc, &itemRect, self->_hToolBarBrush);

            //SelectObject(dc, self->_hToolBarBrush);
            //Ellipse(dc, itemRect.left, itemRect.top, itemRect.right, itemRect.bottom);
        }


        if (bIsPressed)
        {
	            //HBRUSH brBtnShadow = CreateSolidBrush(GetSysColor(COLOR_BTNSHADOW));
		
			COLORREF color = GetSysColor(COLOR_MENUBAR);//CTLCOLOR_DLG//COLOR_BTNFACE//COLOR_MENU
			
			BYTE r = GetRValue(color) - 12;
			BYTE g = GetGValue(color) - 12;
			BYTE b = GetBValue(color) - 12;
			color = ((uint32_t)r << 16) | ((uint32_t)g << 8) | ((uint32_t)b );
			
            HBRUSH brBtnShadow = CreateSolidBrush(color);
            FrameRect(dc, &itemRect, brBtnShadow);

            //SelectObject(dc, brBtnShadow);
            //Ellipse(dc, itemRect.left, itemRect.top, itemRect.right, itemRect.bottom);

            DeleteObject(brBtnShadow);
        }

        else
        {
            //UINT uState = DFCS_BUTTONPUSH |
            //	((bIsPressed) ? DFCS_PUSHED : 0);

            //DrawFrameControl(dc, &itemRect, DFC_BUTTON, uState);
        }


        self->DrawTheIcon(hWnd, &dc, lpDIS->CtlID, &lpDIS->rcItem, bIsPressed, bIsDisabled, bIsFocused);


        //if (bIsFocused && bDrawFocusRect)
        //{
        //	RECT focusRect = itemRect;
        //	InflateRect(&focusRect, -3, -3);
        //	DrawFocusRect(dc, &focusRect);
        //} 

        return (TRUE);
    }
    break;
}

  return DefWindowProc(hWnd, message, wParam, lParam);
}


//20230218
HICON RootWindowWin::HICONFromHBITMAP(HBITMAP hBitmap)
{
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    HBITMAP hbmMask = ::CreateCompatibleBitmap(::GetDC(NULL),
        bmp.bmWidth, bmp.bmHeight);

    ICONINFO ii = { 0 };
    ii.fIcon = TRUE;
    ii.hbmColor = hBitmap;
    ii.hbmMask = hbmMask;

    HICON hIcon = ::CreateIconIndirect(&ii);//一旦不再需要，注意用DestroyIcon函数释放占用的内存及资源
    ::DeleteObject(hbmMask);

    return hIcon;
}

void RootWindowWin::DrawTheIcon(HWND hButtonWnd, HDC* dc, UINT uID, RECT* rpItem, BOOL bIsPressed, BOOL bIsDisabled, BOOL bIsFocused)
{
    RECT    rImage;
    CopyRect(&rImage, rpItem);

    if (bIsPressed)
    {
        OffsetRect(&rImage, 1, 1);
    }

    HBITMAP hicon = _hBmpBack;
    if (uID == IDC_NAV_BACK)
    {
        if (bIsDisabled)
        {
            hicon = _hBmpBack_gray;
        }
        else if (bIsFocused)
        {
            hicon = _hBmpBack_over;
        }
    }
    else if (uID == IDC_NAV_FORWARD)
    {
        if (bIsDisabled)
        {
            hicon = _hBmpForward_gray;
        }
        else if (bIsFocused)
        {
            hicon = _hBmpForward_over;
        }
        else {
            hicon = _hBmpForward;
        }
    }
    else if (uID == IDC_NAV_RELOAD)
    {
        if (bIsDisabled)
        {
            hicon = _hBmpReload_gray;
        }
        else if (bIsFocused)
        {
            hicon = _hBmpReload_over;
        }
        else
        {
            hicon = _hBmpReload;
        }
    }
    else if (uID == IDC_NAV_STOP)
    {
        //hicon = _hBmpReload;
    }
    else if (uID == IDS_MENU_ICON_1X_PNG)
    {
        if (bIsFocused)
        {
            hicon = _hBmpTool_over;
        }
        else
        {
            hicon = _hBmpTool;
        }
    }
    //else if (uID == IDC_NAV_RELOAD && bIsDisabled == DSS_DISABLED)
    //{
    //	bIsDisabled = DSS_NORMAL;
    //}

    BITMAP bmp;
    GetObject(hicon, sizeof(BITMAP), &bmp);

    int w = rImage.right - rImage.left;
    int h = rImage.bottom - rImage.top;

    int xoff = (w - bmp.bmWidth) / 2;
    int yoff = (h - bmp.bmHeight) / 2;

    rImage.left += xoff;
    rImage.top += yoff;

    DrawState(*dc,
        NULL,
        NULL,
        (LPARAM)hicon,
        0,
        rImage.left,
        rImage.top,
        0,//(rImage.right - rImage.left),
        0,//(rImage.bottom - rImage.top),
        DSS_NORMAL | //(bIsDisabled ? DSS_DISABLED : DSS_NORMAL) | 
        DST_BITMAP
        //DST_ICON
    );
}



void RootWindowWin::OnPaint() {
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(hwnd_, &ps);


  //20230218 OnPaint
  if (IsoHandler::Instance().viewer_ == 1)
  {

  }
  else
  {
      const int urlbar_height = GetURLBarHeight(hwnd_) + 4;

      RECT itemRect;
      GetClientRect(hwnd_, &itemRect);

      itemRect.bottom = urlbar_height;
      FillRect(hdc, &itemRect, _hToolBarBrush);

      //if win7
      itemRect.bottom = urlbar_height;
      if (!IsWindows8OrGreater())
      {
          //HPEN hpen = CreatePen(PS_SOLID, 1, RGB(130, 130, 130));
          HPEN hpen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
          HPEN hpenOld = (HPEN)::SelectObject(hdc, hpen);
          ::MoveToEx(hdc, itemRect.left, itemRect.top, NULL);
          ::LineTo(hdc, itemRect.left, itemRect.bottom);
          DeleteObject(hpen);


          hpen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
          //hpen = CreatePen(PS_SOLID, 1, RGB(116, 116, 116));
          SelectObject(hdc, hpen);
          ::MoveToEx(hdc, itemRect.left, itemRect.top, NULL);
          ::LineTo(hdc, itemRect.right, itemRect.top);
          DeleteObject(hpen);

          //add_noerase_rgn
          //ws_cliochildren

          //hpen = CreatePen(PS_SOLID, 1, RGB(213, 213, 213));
          ////hpen = CreatePen(PS_SOLID, 1, RGB(125, 125, 125));
          //SelectObject(hdc, hpen);
          //::MoveToEx(hdc, itemRect.right - 1, itemRect.top, NULL);
          //::LineTo(hdc, itemRect.right - 1, itemRect.bottom);
          //DeleteObject(hpen);

          hpen = CreatePen(PS_SOLID, 1, RGB(170, 170, 170));
          SelectObject(hdc, hpen);
          ::MoveToEx(hdc, itemRect.left, itemRect.bottom, NULL);
          ::LineTo(hdc, itemRect.right, itemRect.bottom);
          DeleteObject(hpen);
          SelectObject(hdc, hpenOld);

      }
      else
      {
          HPEN hpen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
          HPEN hpenOld = (HPEN)SelectObject(hdc, hpen);

          ::MoveToEx(hdc, itemRect.left, itemRect.bottom, NULL);
          ::LineTo(hdc, itemRect.right, itemRect.bottom);
          DeleteObject(hpen);
          SelectObject(hdc, hpenOld);
      }
  }







  EndPaint(hwnd_, &ps);
}

void RootWindowWin::OnFocus() {
  // Selecting "Close window" from the task bar menu may send a focus
  // notification even though the window is currently disabled (e.g. while a
  // modal JS dialog is displayed).
  if (browser_window_ && ::IsWindowEnabled(hwnd_))
    browser_window_->SetFocus(true);
}

void RootWindowWin::OnActivate(bool active) {

    //LOG(ERROR) << "active:" << active << ", _jsclose:" << IsoHandler::Instance()._jsclose;
  if (IsoHandler::Instance()._jsclose == 1)
  {
	  return;
  }
  if (IsoHandler::Instance()._jsclose == 2)
  {
      IsoHandler::Instance()._jsclose = 0;
	  return;
  }

  if (active)
    delegate_->OnRootWindowActivated(this);

  if (CefRefPtr<CefBrowser> browser = GetBrowser())
  {
	  string arg = active ? "1" : "0";
	  string js = "if(typeof myactivate === \"function\") myactivate(" + arg + ");";
	  browser->GetMainFrame()->ExecuteJavaScript(js, "", 0);
  }


}

void RootWindowWin::OnSize(bool minimized) {
	if (minimized) {
		// Notify the browser window that it was hidden and do nothing further.
		if (browser_window_)
			browser_window_->Hide();
		return;
	}

	if (browser_window_)
		browser_window_->Show();

	RECT rect;
	GetClientRect(hwnd_, &rect);

	if (with_controls_ && edit_hwnd_) {
		const int button_width = GetButtonWidth(hwnd_);
		const int urlbar_height = GetURLBarHeight(hwnd_);
		const int font_height = LogicalToDevice(14, GetWindowScaleFactor(hwnd_));

		if (font_height != font_height_) {
			font_height_ = font_height;
			if (font_) {
				DeleteObject(font_);
			}

			// Create a scaled font.
			font_ =
				::CreateFont(-font_height, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");

			SendMessage(back_hwnd_, WM_SETFONT, reinterpret_cast<WPARAM>(font_),
				TRUE);
			SendMessage(forward_hwnd_, WM_SETFONT, reinterpret_cast<WPARAM>(font_),
				TRUE);
			SendMessage(reload_hwnd_, WM_SETFONT, reinterpret_cast<WPARAM>(font_),
				TRUE);
			SendMessage(stop_hwnd_, WM_SETFONT, reinterpret_cast<WPARAM>(font_),
				TRUE);
			//SendMessage(edit_hwnd_, WM_SETFONT, reinterpret_cast<WPARAM>(font_),
			//            TRUE);
		}

		// Resize the window and address bar to match the new frame size.

		if (IsoHandler::Instance().viewer_ == 1)
		{
			//rect.top += urlbar_height;

		}
		else
		{
			rect.top += urlbar_height + 4;
		}

		int x_top = 2;


		int x_offset = rect.left;

		// |browser_hwnd| may be nullptr if the browser has not yet been created.
		HWND browser_hwnd = nullptr;
		if (browser_window_)
			browser_hwnd = browser_window_->GetWindowHandle();

		/*
		// Resize all controls.
		HDWP hdwp = BeginDeferWindowPos(browser_hwnd ? 6 : 5);
		hdwp = DeferWindowPos(hdwp, back_hwnd_, nullptr, x_offset, 0, button_width,
							  urlbar_height, SWP_NOZORDER);
		x_offset += button_width;
		hdwp = DeferWindowPos(hdwp, forward_hwnd_, nullptr, x_offset, 0,
							  button_width, urlbar_height, SWP_NOZORDER);
		x_offset += button_width;
		hdwp = DeferWindowPos(hdwp, reload_hwnd_, nullptr, x_offset, 0,
							  button_width, urlbar_height, SWP_NOZORDER);
		x_offset += button_width;
		hdwp = DeferWindowPos(hdwp, stop_hwnd_, nullptr, x_offset, 0, button_width,
							  urlbar_height, SWP_NOZORDER);
		x_offset += button_width;
		hdwp = DeferWindowPos(hdwp, edit_hwnd_, nullptr, x_offset, 0,
							  rect.right - x_offset, urlbar_height, SWP_NOZORDER);

		if (browser_hwnd) {
		  hdwp = DeferWindowPos(hdwp, browser_hwnd, nullptr, rect.left, rect.top,
								rect.right - rect.left, rect.bottom - rect.top,
								SWP_NOZORDER);
		}
		*/

		//20230218 OnSize
		HDWP hdwp = BeginDeferWindowPos(5);

		hdwp = DeferWindowPos(hdwp, back_hwnd_, NULL, x_offset, x_top, button_width,
			urlbar_height, SWP_NOZORDER);
		x_offset += button_width + 6;
		hdwp = DeferWindowPos(hdwp, forward_hwnd_, NULL, x_offset, x_top, button_width,
			urlbar_height, SWP_NOZORDER);
		x_offset += button_width + 6;
		hdwp = DeferWindowPos(hdwp, reload_hwnd_, NULL, x_offset, x_top, button_width,
			urlbar_height, SWP_NOZORDER);

		hdwp = DeferWindowPos(hdwp, _tool_hwnd, NULL, rect.right - button_width - 5, x_top,
			button_width, urlbar_height, SWP_NOZORDER);

		//if (browser_hwnd) {
		//    hdwp = DeferWindowPos(hdwp, browser_hwnd, NULL, rect.left, rect.top,
		//        rect.right - rect.left, rect.bottom - rect.top,
		//        SWP_NOZORDER);
		//}

		if (browser_hwnd) {
			hdwp = DeferWindowPos(hdwp, browser_hwnd, nullptr, rect.left, rect.top,
				rect.right - rect.left, rect.bottom - rect.top,
				SWP_NOZORDER);
		}


		BOOL result = EndDeferWindowPos(hdwp);
		ALLOW_UNUSED_LOCAL(result);
		DCHECK(result);
	}
	else if (browser_window_) {
		// Size the browser window to the whole client area.
		browser_window_->SetBounds(0, 0, rect.right, rect.bottom);
	}


	//20230218
	if (::IsWindowVisible(IsoHandler::Instance().hwndLoading_))
	{
		RECT rcWin;
		::GetWindowRect(hwnd_, &rcWin);

		int w = rcWin.right - rcWin.left;
		int h = rcWin.bottom - rcWin.top;

		RECT rc;
		::GetWindowRect(IsoHandler::Instance().hwndLoading_, &rc);

		int left = (w - (rc.right - rc.left)) / 2 + rcWin.left;
		int top = (h - (rc.bottom - rc.top)) / 2 + rcWin.top;

		::MoveWindow(IsoHandler::Instance().hwndLoading_,
			left,
			top,
			rc.right - rc.left,
			rc.bottom - rc.top,
			FALSE
		);
	}

	if (_hwndPaint != NULL)
	{
		PostMessage(_hwndPaint, WM_PAINT_SIZE, 0, 0);

	}

}

void RootWindowWin::OnMove() {
	// Notify the browser of move events so that popup windows are displayed
	// in the correct location and dismissed when the window moves.
	CefRefPtr<CefBrowser> browser = GetBrowser();
	if (browser)
		browser->GetHost()->NotifyMoveOrResizeStarted();



	if (::IsWindowVisible(IsoHandler::Instance().hwndLoading_))
	{
		RECT rcWin;
		::GetWindowRect(hwnd_, &rcWin);

		int w = rcWin.right - rcWin.left;
		int h = rcWin.bottom - rcWin.top;

		RECT rc;
		::GetWindowRect(IsoHandler::Instance().hwndLoading_, &rc);

		int left = (w - (rc.right - rc.left)) / 2 + rcWin.left;
		int top = (h - (rc.bottom - rc.top)) / 2 + rcWin.top;

		::MoveWindow(IsoHandler::Instance().hwndLoading_,
			left,
			top,
			rc.right - rc.left,
			rc.bottom - rc.top,
			FALSE
		);
	}

	if (_hwndPaint != NULL)
	{
		PostMessage(_hwndPaint, WM_PAINT_MOVE, 0, 0);

	}

}

void RootWindowWin::OnDpiChanged(WPARAM wParam, LPARAM lParam) {
  if (LOWORD(wParam) != HIWORD(wParam)) {
    NOTIMPLEMENTED() << "Received non-square scaling factors";
    return;
  }

     const float display_scale_factor =
        static_cast<float>(LOWORD(wParam)) / DPI_1X;
 if (browser_window_ && with_osr_) {
    // Scale factor for the new display.
    browser_window_->SetDeviceScaleFactor(display_scale_factor);
  }

  // Suggested size and position of the current window scaled for the new DPI.
  const RECT* rect = reinterpret_cast<RECT*>(lParam);
  SetBounds(rect->left, rect->top, rect->right - rect->left,
            rect->bottom - rect->top);



  //20230218 dpi
  //LOG(ERROR) << "OnDpiChanged:" << display_scale_factor;
  bool b = IsWindows8OrGreater();
  int cx = 32 * display_scale_factor;
  make_bitmap(cx, b);

  if (_hwndPaint != NULL)
  {
      PostMessage(_hwndPaint, WM_PAINT_DPICHANGED, cx, 0);

  }




}

bool RootWindowWin::OnEraseBkgnd() {
  // Erase the background when the browser does not exist.
  return (GetBrowser() == nullptr);
}

bool RootWindowWin::OnCommand(UINT id, LPARAM lParam) {
	/*
  if (id >= ID_TESTS_FIRST && id <= ID_TESTS_LAST) {
	delegate_->OnTest(this, id);
	return true;
  }
  */

	if ((id >= ID_PICOFU_ZOOM && id < ID_PICOFU_CHECK))
	{
		if (id >= ID_PICOFU_ZOOM_400 && id <= ID_PICOFU_ZOOM_50)
		{
			_CURR_ZOOM_MENUID = id;
		}

		delegate_->OnTest(this, id);
		return true;
	}

	switch (id) {
	case IDM_ABOUT:
		OnAbout();
		return true;
	case IDM_EXIT:
		delegate_->OnExit(this);
		return true;
	case ID_PICOFU_FULLSCREEN:
		OnSetFullscreen(true);
		::SetFocus(_tool_hwnd);
		OnFocus();

		return true;
		//20230218 RunTest
	case ID_PICOFYOU_ISO:
	{
		IsoHandler::Instance().ShowDialogBox(hwnd_, IDD_DIALOG_ISO, lParam);
	}
	return true;


	case ID_FIND:
		OnFind();
		return true;

		/*
	  case IDC_NAV_BACK:  // Back button
		if (CefRefPtr<CefBrowser> browser = GetBrowser())
		  browser->GoBack();
		return true;
	  case IDC_NAV_FORWARD:  // Forward button
		if (CefRefPtr<CefBrowser> browser = GetBrowser())
		  browser->GoForward();
		return true;
	  case IDC_NAV_RELOAD:  // Reload button
		if (CefRefPtr<CefBrowser> browser = GetBrowser())
		  browser->Reload();
		return true;
		*/

	case IDC_NAV_BACK:  // Back button
	{
		if (CefRefPtr < CefBrowser > browser = GetBrowser())
		{
			if (_uploading == 1)
			{
				string js = "close_uploading('back');";
				browser->GetMainFrame()->ExecuteJavaScript(js, "", 0);
				return true;
			}

			string::size_type pos = _lasturl.find("/index.html");
            string::size_type pos1 = _lasturl.find("/admin.html?page=admin.batch");
            if (pos != string::npos || pos1 != string::npos)
			{
				string js = "if(typeof mygoback === \"function\") mygoback();";
				browser->GetMainFrame()->ExecuteJavaScript(js, "", 0);
			}
			else
			{
				browser->GoBack();
			}
		}

		//if (CefRefPtr<CefBrowser> browser = GetBrowser())
		//	browser->GoBack();
		OnFocus();
	}
	return true;


	case IDC_NAV_FORWARD:  // Forward button
	{
		if (CefRefPtr<CefBrowser> browser = GetBrowser())
		{
			if (_uploading == 1)
			{
				string js = "close_uploading('forward');";
				browser->GetMainFrame()->ExecuteJavaScript(js, "", 0);
				return true;
			}

			string::size_type pos = _lasturl.find("/index.html");
            string::size_type pos1 = _lasturl.find("/admin.html?page=admin.batch");
            if (pos != string::npos || pos1 != string::npos)
            {
				string js = "if(typeof mygoforward === \"function\") mygoforward();";
				browser->GetMainFrame()->ExecuteJavaScript(js, "", 0);
			}
			else
			{
				browser->GoForward();
			}
		}
		OnFocus();
	}
	return true;
	case IDC_NAV_RELOAD:  // Reload button
		if (CefRefPtr<CefBrowser> browser = GetBrowser())
		{
			if (_uploading == 1)
			{
				string js = "close_uploading('reload');";
				browser->GetMainFrame()->ExecuteJavaScript(js, "", 0);
				return true;
			}

			string::size_type pos = _lasturl.find("/index.html");
			string::size_type pos1 = _lasturl.find("/admin.html?page=admin.batch");
			if (pos != string::npos || pos1 != string::npos)
			{
				string js = "if(typeof myreload === \"function\") myreload();";
				browser->GetMainFrame()->ExecuteJavaScript(js, "", 0);

			}
			else
			{
				if (isLoading_)
				{
					browser->StopLoad();
				}
				else
				{
					browser->Reload();
				}
			}
		}
		OnFocus();
		return true;



	case IDC_NAV_STOP:  // Stop button
		if (CefRefPtr<CefBrowser> browser = GetBrowser())
			browser->StopLoad();
		return true;
	case IDS_MENU_ICON_1X_PNG:
    {
        //LOG(ERROR) << "IDS_MENU_ICON_1X_PNG begin";
//20230218 toolbutton
        double lev = 0.0;
        if (CefRefPtr<CefBrowser> browser = GetBrowser())
        {
            lev = browser->GetHost()->GetZoomLevel();
        }

        wstring zoom_nb = L"";
        if (lev == -0.5)
        {
            _CURR_ZOOM_MENUID = ID_PICOFU_ZOOM_50;
            zoom_nb = L"(50%)";
        }
        else if (lev == -0.25)
        {
            _CURR_ZOOM_MENUID = ID_PICOFU_ZOOM_75;
            zoom_nb = L"(75%)";
        }
        else if (lev == 0.0)
        {
            _CURR_ZOOM_MENUID = ID_PICOFU_ZOOM_100;
        }
        else if (lev == 0.25)
        {
            _CURR_ZOOM_MENUID = ID_PICOFU_ZOOM_125;
            zoom_nb = L"(125%)";
        }
        else if (lev == 0.5)
        {
            _CURR_ZOOM_MENUID = ID_PICOFU_ZOOM_150;
            zoom_nb = L"(150%)";
        }
        else if (lev == 0.75)
        {
            _CURR_ZOOM_MENUID = ID_PICOFU_ZOOM_175;
            zoom_nb = L"(175%)";
        }
        else if (lev == 1.0)
        {
            _CURR_ZOOM_MENUID = ID_PICOFU_ZOOM_200;
            zoom_nb = L"(200%)";
        }
        else if (lev == 1.5)
        {
            _CURR_ZOOM_MENUID = ID_PICOFU_ZOOM_250;
            zoom_nb = L"(250%)";
        }
        else if (lev == 2.0)
        {
            _CURR_ZOOM_MENUID = ID_PICOFU_ZOOM_300;
            zoom_nb = L"(300%)";
        }
        else if (lev == 3.0)
        {
            _CURR_ZOOM_MENUID = ID_PICOFU_ZOOM_400;
            zoom_nb = L"(400%)";
        }

        if (_pMainMenu)
        {
            //LOG(ERROR) << "IDS_MENU_ICON_1X_PNG _pMainMenu";
            HMENU hTestMenu = ::GetSubMenu(_pMainMenu, 2);

            wstring zoom_caption = L"";
            transl("Zoom", zoom_caption);

            transl1(hTestMenu, ID_PICOFYOU_ISO, L"Dump Image Files (ISO)");
            transl1(hTestMenu, ID_PICOFU_PRINT, L"Print");
            transl1(hTestMenu, ID_PICOFU_PRINT_TO_PDF, L"Print to PDF");
            zoom_caption += zoom_nb;
            ModifyMenu(hTestMenu, 5, MF_BYPOSITION | MF_STRING, 0, zoom_caption.c_str());

            transl1(hTestMenu, ID_PICOFU_ZOOM_ADD, L"Zoom In");
            transl1(hTestMenu, ID_PICOFU_ZOOM_SUB, L"Zoom Out");
            transl1(hTestMenu, ID_PICOFU_ZOOM_100, L"100%");
            transl1(hTestMenu, ID_PICOFU_FULLSCREEN, L"Full Screen");
            transl1(hTestMenu, ID_PICOFU_CHECK, L"Check For Updates...");
			
			if (IsoHandler::Instance().album_ == L"private") {
				transl1(hTestMenu, ID_PICOFU_REGISTR, L"Registration...");
			}
			else{
				transl1(hTestMenu, ID_PICOFU_REGISTR, L"PhotoNest encrypted version");
			}
            //transl1(hTestMenu, ID_TESTS_ORDERNOW, L"Order Now...");
            transl1(hTestMenu, IDM_ABOUT, L"About...");

            if (IsoHandler::Instance().username_ == "ok")
            {
                EnableMenuItem(hTestMenu, ID_PICOFYOU_ISO, MF_ENABLED);
            }
            else
            {
                EnableMenuItem(hTestMenu, ID_PICOFYOU_ISO, MF_DISABLED);
            }

            int cx = 25;
            //MENUBARINFO minfo;
            //memset(&minfo, 0, sizeof(MENUBARINFO));
            //minfo.cbSize = sizeof(MENUBARINFO);
            //HWND myhWnd = FindWindowW(L"#32768", nullptr);
            //GetMenuBarInfo(myhWnd, OBJID_CLIENT, 0, &minfo);
            //int w = minfo.rcBar.right - minfo.rcBar.left;
            //cx = w;
            //LOG(ERROR) << "cx: " << cx;

            CheckMenuRadioItem(hTestMenu, ID_PICOFU_ZOOM_400, ID_PICOFU_ZOOM_50, _CURR_ZOOM_MENUID, MF_BYCOMMAND);

            RECT rBtn;
            GetClientRect(_tool_hwnd, &rBtn);

            POINT pt;
            pt.x = rBtn.right - cx - 75;
            pt.y = rBtn.bottom;
            ClientToScreen(_tool_hwnd, &pt);

            TrackPopupMenu(hTestMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, 0, hwnd_, NULL);
        }
    }
	return true;
	case ID_PICOFU_DIR:
		OnDir((int)lParam);
		return true;
	case IDD_DIALOG_ALERRT:
		
		IsoHandler::Instance().ShowDialogBox(hwnd_, IDD_DIALOG_ALERRT, lParam);
		return true;
	case IDD_DIALOG_CONFIRM:
	{
		int result = IsoHandler::Instance().ShowDialogBox(hwnd_, IDD_DIALOG_CONFIRM, lParam);
		if (result == IDOK)
		{
			if (CefRefPtr<CefBrowser> browser = GetBrowser())
			{
				string js = "confirmOK();";
				browser->GetMainFrame()->ExecuteJavaScript(js, "", 0);
			}
		}
	}
	return true;

	case ID_PICOFU_REGISTR:
    {
		if (IsoHandler::Instance().album_ == L"private") {
			IsoHandler::Instance().ShowDialogBox(hwnd_, IDD_DIALOG_REGISTER, lParam);
		}
		else
		{
			ShellExecute(NULL, L"open", L"https://www.photonest.io/index.html#subscribe", L"", NULL, SW_SHOWNORMAL);
		}
    }
	return true;

	//case ID_TESTS_ORDERNOW:
	//{
	//    wstring buyURL = L"http://www.photonest.io/index.html#subscribe";
	//    ShellExecuteW(NULL, L"open", buyURL.c_str(), NULL, NULL, SW_SHOWNORMAL);
	//}
	//return true;

	case ID_PICOFU_CHECK:
        wstring url = L"https://www.photonest.io/index.html";
        if (IsoHandler::Instance().album_ == L"private") {
            url += L"#subscribe";
        }
        else {
            url += L"#download";
        }
        ShellExecuteW(NULL, L"open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
		return true;
	}

	return false;
}

//20230218 OnSysCommand
bool RootWindowWin::OnSysCommand(HWND hWnd, UINT id) {
    switch (id) {
    case SC_MAXIMIZE:
    {
        if (!IsIconic(hWnd))
        {
            RECT rcWin;
            ::GetWindowRect(hWnd, &rcWin);
            IsoHandler::Instance().WritePrivate(rcWin, -1);
        }
    }
    break;
    case SC_RESTORE:
    {
    }
    break;
    case SC_MINIMIZE:
    {
        if (!IsZoomed(hWnd))
        {
            RECT rcWin;
            ::GetWindowRect(hWnd, &rcWin);
            IsoHandler::Instance().WritePrivate(rcWin, -2);
        }
    }
    break;
    }
    return false;
}

void RootWindowWin::OnFind() {
  if (find_hwnd_) {
    // Give focus to the existing find dialog.
    ::SetFocus(find_hwnd_);
    return;
  }

  // Configure dialog state.
  ZeroMemory(&find_state_, sizeof(find_state_));
  find_state_.lStructSize = sizeof(find_state_);
  find_state_.hwndOwner = hwnd_;
  find_state_.lpstrFindWhat = find_buff_;
  find_state_.wFindWhatLen = sizeof(find_buff_);
  find_state_.Flags = FR_HIDEWHOLEWORD | FR_DOWN;

  // Create the dialog.
  find_hwnd_ = FindText(&find_state_);

  // Override the dialog's window procedure.
  find_wndproc_old_ = SetWndProcPtr(find_hwnd_, FindWndProc);

  // Associate |self| with the dialog.
  SetUserDataPtr(find_hwnd_, this);
}

void RootWindowWin::OnFindEvent() {
  CefRefPtr<CefBrowser> browser = GetBrowser();

  if (find_state_.Flags & FR_DIALOGTERM) {
    // The find dialog box has been dismissed so invalidate the handle and
    // reset the search results.
    if (browser) {
      browser->GetHost()->StopFinding(true);
      find_what_last_.clear();
      find_next_ = false;
    }
  } else if ((find_state_.Flags & FR_FINDNEXT) && browser) {
    // Search for the requested string.
    bool match_case = ((find_state_.Flags & FR_MATCHCASE) ? true : false);
    const std::wstring& find_what = find_buff_;
    if (match_case != find_match_case_last_ || find_what != find_what_last_) {
      // The search string has changed, so reset the search results.
      if (!find_what.empty()) {
        browser->GetHost()->StopFinding(true);
        find_next_ = false;
      }
      find_match_case_last_ = match_case;
      find_what_last_ = find_buff_;
    }

    browser->GetHost()->Find(find_what,
                             (find_state_.Flags & FR_DOWN) ? true : false,
                             match_case, find_next_);
    if (!find_next_)
      find_next_ = true;
  }
}

void RootWindowWin::OnAbout() {
	// Show the about box.
	//DialogBox(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd_,
	//          AboutWndProc);
	IsoHandler::Instance().ShowDialogBox(hwnd_, IDD_DIALOG_ABOUT, 0);
}

void RootWindowWin::OnNCCreate(LPCREATESTRUCT lpCreateStruct) {
  if (IsProcessPerMonitorDpiAware()) {
    // This call gets Windows to scale the non-client area when WM_DPICHANGED
    // is fired on Windows versions >= 10.0.14393.0.
    typedef BOOL(WINAPI * EnableNonClientDpiScalingPtr)(HWND);
    static EnableNonClientDpiScalingPtr func_ptr =
        reinterpret_cast<EnableNonClientDpiScalingPtr>(GetProcAddress(
            GetModuleHandle(L"user32.dll"), "EnableNonClientDpiScaling"));
    called_enable_non_client_dpi_scaling_ = !!(func_ptr && func_ptr(hwnd_));
  }
}

void RootWindowWin::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    const HINSTANCE hInstance = lpCreateStruct->hInstance;

    RECT rect;
    GetClientRect(hwnd_, &rect);

    if (with_controls_) {
        // Create the child controls.
        int x_offset = 0;

        const int button_width = GetButtonWidth(hwnd_);
        const int urlbar_height = GetURLBarHeight(hwnd_);

        int x_top = 0;

        //20230218 OnCreate
        int cx = GetSystemMetrics(SM_CXICON);
        x_offset = 7;
        _pMainMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDC_CEFCLIENT));

        int off = 0;
        //if (cx == 32)
        {
            off = 24;
        }

        //COLORREF color = RGB(255, 255, 255);
        //COLORREF crColor = RGB(223, 223, 223);//GetSysColor(COLOR_BTNSHADOW);  //RGB(242, 242, 242);
        COLORREF color = GetSysColor(COLOR_MENUBAR);//CTLCOLOR_DLG//COLOR_BTNFACE//COLOR_MENU
        //COLORREF crColor = GetSysColor(COLOR_BTNSHADOW);  //RGB(242, 242, 242);RGB(223, 223, 223);//COLOR_BTNSHADOW

        BYTE r = GetRValue(color) - 12;
        BYTE g = GetGValue(color) - 12;
        BYTE b1 = GetBValue(color) - 12;
        COLORREF  crColor = ((uint32_t)r << 16) | ((uint32_t)g << 8) | ((uint32_t)b1);

        bool b = IsWindows8OrGreater();
        if (!b)
        {
            //color = RGB(240, 240, 240);
            //crColor = RGB(223, 223, 223);
        }

        make_bitmap(cx, b);

        _hToolBarBrush = CreateSolidBrush(color);
        _brBtnShadow = CreateSolidBrush(crColor);

        x_top = 2;

        back_hwnd_ = CreateWindow(
            L"BUTTON", L"Back", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW | BS_ICON | BS_BITMAP,
            x_offset, x_top, button_width, urlbar_height, hwnd_,
            reinterpret_cast<HMENU>(IDC_NAV_BACK), hInstance, 0);
        CHECK(back_hwnd_);

        _backWndProc = SetWndProcPtr(back_hwnd_, BtnWndProc);
        SetUserDataPtr(back_hwnd_, this);

        x_offset += button_width + 6;

        forward_hwnd_ =
            CreateWindow(L"BUTTON", L"Forward",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW | BS_ICON | BS_BITMAP,
                x_offset, x_top, button_width, urlbar_height, hwnd_,
                reinterpret_cast<HMENU>(IDC_NAV_FORWARD), hInstance, 0);
        CHECK(forward_hwnd_);
        _forwardWndProc = SetWndProcPtr(forward_hwnd_, BtnWndProc);
        SetUserDataPtr(forward_hwnd_, this);

        x_offset += button_width + 6;

        reload_hwnd_ =
            CreateWindow(L"BUTTON", L"Reload",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW | BS_ICON | BS_BITMAP,
                x_offset, x_top, button_width, urlbar_height, hwnd_,
                reinterpret_cast<HMENU>(IDC_NAV_RELOAD), hInstance, 0);
        CHECK(reload_hwnd_);
        _reloadWndProc = SetWndProcPtr(reload_hwnd_, BtnWndProc);
        SetUserDataPtr(reload_hwnd_, this);

        x_offset += button_width + 6;

        stop_hwnd_ = CreateWindow(
            L"BUTTON", L"Stop", WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW | BS_ICON | BS_BITMAP,
            x_offset, x_top, button_width, urlbar_height, hwnd_,
            reinterpret_cast<HMENU>(IDC_NAV_STOP), hInstance, 0);
        CHECK(stop_hwnd_);

        _stopWndProc = SetWndProcPtr(stop_hwnd_, BtnWndProc);
        SetUserDataPtr(stop_hwnd_, this);

        x_offset += button_width + 6;

        //| WS_VISIBLE
        edit_hwnd_ = CreateWindow(L"EDIT", 0,
            WS_CHILD | WS_BORDER | ES_LEFT |
            ES_AUTOVSCROLL | ES_AUTOHSCROLL | /*WS_VISIBLE |*/ WS_DISABLED,
            x_offset, 5, rect.right - button_width * 10,
            //rect.right, x_top, button_width,
            urlbar_height, hwnd_, 0, hInstance, 0);
        CHECK(edit_hwnd_);


        _tool_hwnd = CreateWindow(
            L"BUTTON", L"tool", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW | BS_ICON | BS_BITMAP,
            rect.right - button_width - 5, x_top, button_width, urlbar_height, hwnd_,
            reinterpret_cast<HMENU>(IDS_MENU_ICON_1X_PNG), hInstance, 0);

        CHECK(stop_hwnd_);
        _toolWndProc = SetWndProcPtr(_tool_hwnd, BtnWndProc);
        SetUserDataPtr(_tool_hwnd, this);

        x_offset += button_width;

        CHECK(_tool_hwnd);
        EnableWindow(_tool_hwnd, FALSE);


        // Override the edit control's window procedure.
        edit_wndproc_old_ = SetWndProcPtr(edit_hwnd_, EditWndProc);

        // Associate |this| with the edit window.
        SetUserDataPtr(edit_hwnd_, this);


        if (IsoHandler::Instance().viewer_ == 1)
        {
            ShowHideToolBar(SW_HIDE);

            const std::wstring& window_no = GetResourceString(IDS_APP_NO);
            if (window_no == L"pvt")
            {
                SetWindowText(hwnd_, L"PhotoNest Viewer2");
            }
            else {

                SetWindowText(hwnd_, L"PhotoNest Viewer");
            }
        }
        else
        {
            rect.top += urlbar_height + 4;
        }

        if (!with_osr_) {
            // Remove the menu items that are only used with OSR.
            HMENU hMenu = ::GetMenu(hwnd_);
            if (hMenu) {
                HMENU hTestMenu = ::GetSubMenu(hMenu, 2);
                if (hTestMenu) {
                    //::RemoveMenu(hTestMenu, ID_TESTS_OSR_FPS, MF_BYCOMMAND);
                    //::RemoveMenu(hTestMenu, ID_TESTS_OSR_DSF, MF_BYCOMMAND);
                }
            }
        }
    }
    else {
        // No controls so also remove the default menu.
        ::SetMenu(hwnd_, NULL);
    }



    /*
    back_hwnd_ = CreateWindow(
        L"BUTTON", L"Back", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_DISABLED,
        x_offset, 0, button_width, urlbar_height, hwnd_,
        reinterpret_cast<HMENU>(IDC_NAV_BACK), hInstance, 0);
    CHECK(back_hwnd_);
    x_offset += button_width;

    forward_hwnd_ =
        CreateWindow(L"BUTTON", L"Forward",
                     WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_DISABLED,
                     x_offset, 0, button_width, urlbar_height, hwnd_,
                     reinterpret_cast<HMENU>(IDC_NAV_FORWARD), hInstance, 0);
    CHECK(forward_hwnd_);
    x_offset += button_width;

    reload_hwnd_ =
        CreateWindow(L"BUTTON", L"Reload",
                     WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_DISABLED,
                     x_offset, 0, button_width, urlbar_height, hwnd_,
                     reinterpret_cast<HMENU>(IDC_NAV_RELOAD), hInstance, 0);
    CHECK(reload_hwnd_);
    x_offset += button_width;

    stop_hwnd_ = CreateWindow(
        L"BUTTON", L"Stop", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_DISABLED,
        x_offset, 0, button_width, urlbar_height, hwnd_,
        reinterpret_cast<HMENU>(IDC_NAV_STOP), hInstance, 0);
    CHECK(stop_hwnd_);
    x_offset += button_width;

    edit_hwnd_ = CreateWindow(L"EDIT", 0,
                              WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT |
                                  ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_DISABLED,
                              x_offset, 0, rect.right - button_width * 4,
                              urlbar_height, hwnd_, 0, hInstance, 0);
    CHECK(edit_hwnd_);

    // Override the edit control's window procedure.
    edit_wndproc_old_ = SetWndProcPtr(edit_hwnd_, EditWndProc);

    // Associate |this| with the edit window.
    SetUserDataPtr(edit_hwnd_, this);

    rect.top += urlbar_height;

    if (!with_osr_) {
      // Remove the menu items that are only used with OSR.
      HMENU hMenu = ::GetMenu(hwnd_);
      if (hMenu) {
        HMENU hTestMenu = ::GetSubMenu(hMenu, 2);
        if (hTestMenu) {
          ::RemoveMenu(hTestMenu, ID_TESTS_OSR_FPS, MF_BYCOMMAND);
          ::RemoveMenu(hTestMenu, ID_TESTS_OSR_DSF, MF_BYCOMMAND);
        }
      }
    }
  } else {
    // No controls so also remove the default menu.
    ::SetMenu(hwnd_, nullptr);
  }
  */

    const float device_scale_factor = GetWindowScaleFactor(hwnd_);

    if (with_osr_) {
        browser_window_->SetDeviceScaleFactor(device_scale_factor);
    }

    if (!is_popup_) {
        // Create the browser window.
        CefRect cef_rect(rect.left, rect.top, rect.right - rect.left,
            rect.bottom - rect.top);
        browser_window_->CreateBrowser(hwnd_, cef_rect, browser_settings_, nullptr,
            delegate_->GetRequestContext(this));
    }
    else {
        // With popups we already have a browser window. Parent the browser window
        // to the root window and show it in the correct location.
        browser_window_->ShowPopup(hwnd_, rect.left, rect.top,
            rect.right - rect.left, rect.bottom - rect.top);
    }
}

bool RootWindowWin::OnClose(HWND hWnd) {

	if (_uploading == 1)
	{
		if (CefRefPtr<CefBrowser> browser = GetBrowser())
		{
			string js = "close_uploading('close');";
			browser->GetMainFrame()->ExecuteJavaScript(js, "", 0);
			return true;
		}
	}

	if (browser_window_ && !browser_window_->IsClosing()) {
		CefRefPtr<CefBrowser> browser = GetBrowser();
		if (browser) {
			// Notify the browser window that we would like to close it. This
			// will result in a call to ClientHandler::DoClose() if the
			// JavaScript 'onbeforeunload' event handler allows it.
			browser->GetHost()->CloseBrowser(false);

			// Cancel the close.
			return true;
		}
	}

	//20230218 OnClose
	if (!IsIconic(hWnd))
	{
		int zm = IsZoomed(hWnd) ? 1 : 0;

		RECT rcWin, rc;
		::GetWindowRect(hWnd, &rcWin);
        ::GetClientRect(hWnd, &rc);
        //rcWin.right = rcWin.left + (rc.right - rc.left);
        //rcWin.bottom = rcWin.top + (rc.bottom - rc.top);
        IsoHandler::Instance().WritePrivate(rcWin, zm);
	}

	bool b = isurlok(_lasturl);
	if (b)
	{
		IsoHandler::Instance().WritePrivate("last", _lasturl.c_str());
	}

	char* szResponse = 0;
	char* szMimeType = 0;
	mygetfunc("c.html?act=c.uinit", &szResponse, &szMimeType);

	// Allow the close.
	return false;
}

void RootWindowWin::OnDestroyed() {
    window_destroyed_ = true;
    NotifyDestroyedIfDone();
}

void RootWindowWin::OnBrowserCreated(CefRefPtr<CefBrowser> browser) {
  REQUIRE_MAIN_THREAD();

  if (is_popup_) {
    // For popup browsers create the root window once the browser has been
    // created.
    CreateRootWindow(CefBrowserSettings(), false);
  } else {
    // Make sure the browser is sized correctly.
    OnSize(false);
  }

  delegate_->OnBrowserCreated(this, browser);
}

void RootWindowWin::OnBrowserWindowDestroyed() {
  REQUIRE_MAIN_THREAD();

  browser_window_.reset();

  if (!window_destroyed_) {
    // The browser was destroyed first. This could be due to the use of
    // off-screen rendering or execution of JavaScript window.close().
    // Close the RootWindow.
    Close(true);
  }

  browser_destroyed_ = true;
  NotifyDestroyedIfDone();
}

void RootWindowWin::OnSetAddress(const std::string& url) {
  REQUIRE_MAIN_THREAD();

  if (edit_hwnd_)
    SetWindowText(edit_hwnd_, CefString(url).ToWString().c_str());



  //20230218 OnSetAddress

  _lasturl = url;
  //LOG(ERROR) << "_lasturl " << _lasturl;

  HMENU hTestMenu = ::GetSubMenu(_pMainMenu, 2);
  wstring::size_type pos = _lasturl.find("admin.html");
  if (pos != wstring::npos)
  {
      EnableMenuItem(hTestMenu, ID_PICOFU_PRINT, MF_GRAYED);
      EnableMenuItem(hTestMenu, ID_PICOFU_PRINT_TO_PDF, MF_GRAYED);
  }
  else
  {
      EnableMenuItem(hTestMenu, ID_PICOFU_PRINT, MF_ENABLED);
      EnableMenuItem(hTestMenu, ID_PICOFU_PRINT_TO_PDF, MF_ENABLED);
  }

  if (back_hwnd_)
  {
      //EnableWindow(back_hwnd_, canGoBack);
      InvalidateRect(back_hwnd_, NULL, TRUE);
      //UpdateWindow(back_hwnd_);
  }




}

void RootWindowWin::OnSetTitle(const std::string& title) {
	REQUIRE_MAIN_THREAD();

	//if (hwnd_)
	//  SetWindowText(hwnd_, CefString(title).ToWString().c_str());

	if (title == "loading")
	{
		::ShowWindow(IsoHandler::Instance().hwndLoading_, SW_HIDE);
		EnableWindow(_tool_hwnd, TRUE);
	}
}

void RootWindowWin::OnDir(int dirType)
{
	wstring szDir = IsoHandler::Instance().OnDir(hwnd_, dirType);

    if (CefRefPtr<CefBrowser> browser = GetBrowser())
    {
        CefString temp = CefString(szDir);
        //if (temp != "")
        {
            string s2 = temp.ToString();
            Replace(s2, "\\", "/");
			s2.erase(s2.find_last_not_of("/") + 1);

            string js = string("cefQuerySuccess(\"") + s2 + string("\");");
            browser->GetMainFrame()->ExecuteJavaScript(js, "", 0);
        }
    }
}


void RootWindowWin::OnSetFullscreen(bool fullscreen) {
  REQUIRE_MAIN_THREAD();

  CefRefPtr<CefBrowser> browser = GetBrowser();
  if (browser) {
    std::unique_ptr<window_test::WindowTestRunnerWin> test_runner(
        new window_test::WindowTestRunnerWin());
    if (fullscreen)
      test_runner->Maximize(browser);
    else
      test_runner->Restore(browser);
  }
}

void RootWindowWin::OnAutoResize(const CefSize& new_size) {
  REQUIRE_MAIN_THREAD();

  if (!hwnd_)
    return;

  int new_width = new_size.width;

  // Make the window wide enough to drag by the top menu bar.
  if (new_width < 200)
    new_width = 200;

  const float device_scale_factor = GetWindowScaleFactor(hwnd_);
  RECT rect = {0, 0, LogicalToDevice(new_width, device_scale_factor),
               LogicalToDevice(new_size.height, device_scale_factor)};
  DWORD style = GetWindowLong(hwnd_, GWL_STYLE);
  DWORD ex_style = GetWindowLong(hwnd_, GWL_EXSTYLE);
  bool has_menu = !(style & WS_CHILD) && (GetMenu(hwnd_) != nullptr);

  // The size value is for the client area. Calculate the whole window size
  // based on the current style.
  // 20230218
  //AdjustWindowRectEx(&rect, style, has_menu, ex_style);

  // Size the window. The left/top values may be negative.
  // Also show the window if it's not currently visible.
  SetWindowPos(hwnd_, nullptr, 0, 0, rect.right - rect.left,
               rect.bottom - rect.top,
               SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
}

void RootWindowWin::OnSetLoadingState(bool isLoading,
                                      bool canGoBack,
                                      bool canGoForward) {
  REQUIRE_MAIN_THREAD();

  //20230218
  if (with_controls_) {
      string::size_type pos1 = _lasturl.find("b.html");
      if (pos1 != string::npos)
      {
          canGoBack = false;
      }
      pos1 = _lasturl.find("a.html");
      if (pos1 != string::npos)
      {
          canGoBack = false;
      }
      /*
              if (_uploading == 1)
              {
                  canGoBack = false;
                  canGoForward = false;
                  EnableWindow(reload_hwnd_, false);
              }
              else
              {
                  EnableWindow(reload_hwnd_, true);
              }
      */
      EnableWindow(back_hwnd_, canGoBack);
      EnableWindow(forward_hwnd_, canGoForward);

      isLoading_ = isLoading;
      //EnableWindow(reload_hwnd_, true);
      InvalidateRect(reload_hwnd_, NULL, TRUE);
      //UpdateWindow(reload_hwnd_);


      //EnableWindow(reload_hwnd_, !isLoading);
      //EnableWindow(stop_hwnd_, isLoading);
      //EnableWindow(edit_hwnd_, TRUE);
  }


  /*
  if (with_controls_) {
    EnableWindow(back_hwnd_, canGoBack);
    EnableWindow(forward_hwnd_, canGoForward);
    EnableWindow(reload_hwnd_, !isLoading);
    EnableWindow(stop_hwnd_, isLoading);
    EnableWindow(edit_hwnd_, TRUE);
  }
  */

  if (!isLoading && GetWindowLongPtr(hwnd_, GWL_EXSTYLE) & WS_EX_NOACTIVATE) {
    // Done with the initial navigation. Remove the WS_EX_NOACTIVATE style so
    // that future mouse clicks inside the browser correctly activate and focus
    // the window. For the top-level window removing this style causes Windows
    // to display the task bar button.
    SetWindowLongPtr(hwnd_, GWL_EXSTYLE,
                     GetWindowLongPtr(hwnd_, GWL_EXSTYLE) & ~WS_EX_NOACTIVATE);

    if (browser_window_) {
      HWND browser_hwnd = browser_window_->GetWindowHandle();
      SetWindowLongPtr(
          browser_hwnd, GWL_EXSTYLE,
          GetWindowLongPtr(browser_hwnd, GWL_EXSTYLE) & ~WS_EX_NOACTIVATE);
    }
  }
}

namespace {

LPCWSTR kParentWndProc = L"CefParentWndProc";
LPCWSTR kDraggableRegion = L"CefDraggableRegion";

LRESULT CALLBACK SubclassedWindowProc(HWND hWnd,
                                      UINT message,
                                      WPARAM wParam,
                                      LPARAM lParam) {
  WNDPROC hParentWndProc =
      reinterpret_cast<WNDPROC>(::GetPropW(hWnd, kParentWndProc));
  HRGN hRegion = reinterpret_cast<HRGN>(::GetPropW(hWnd, kDraggableRegion));

  if (message == WM_NCHITTEST) {
    LRESULT hit = CallWindowProc(hParentWndProc, hWnd, message, wParam, lParam);
    if (hit == HTCLIENT) {
      POINTS points = MAKEPOINTS(lParam);
      POINT point = {points.x, points.y};
      ::ScreenToClient(hWnd, &point);
      if (::PtInRegion(hRegion, point.x, point.y)) {
        // Let the parent window handle WM_NCHITTEST by returning HTTRANSPARENT
        // in child windows.
        return HTTRANSPARENT;
      }
    }
    return hit;
  }

  return CallWindowProc(hParentWndProc, hWnd, message, wParam, lParam);
}

void SubclassWindow(HWND hWnd, HRGN hRegion) {
  HANDLE hParentWndProc = ::GetPropW(hWnd, kParentWndProc);
  if (hParentWndProc) {
    return;
  }

  SetLastError(0);
  LONG_PTR hOldWndProc = SetWindowLongPtr(
      hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(SubclassedWindowProc));
  if (hOldWndProc == 0 && GetLastError() != ERROR_SUCCESS) {
    return;
  }

  ::SetPropW(hWnd, kParentWndProc, reinterpret_cast<HANDLE>(hOldWndProc));
  ::SetPropW(hWnd, kDraggableRegion, reinterpret_cast<HANDLE>(hRegion));
}

void UnSubclassWindow(HWND hWnd) {
  LONG_PTR hParentWndProc =
      reinterpret_cast<LONG_PTR>(::GetPropW(hWnd, kParentWndProc));
  if (hParentWndProc) {
    LONG_PTR hPreviousWndProc =
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, hParentWndProc);
    ALLOW_UNUSED_LOCAL(hPreviousWndProc);
    DCHECK_EQ(hPreviousWndProc,
              reinterpret_cast<LONG_PTR>(SubclassedWindowProc));
  }

  ::RemovePropW(hWnd, kParentWndProc);
  ::RemovePropW(hWnd, kDraggableRegion);
}

BOOL CALLBACK SubclassWindowsProc(HWND hwnd, LPARAM lParam) {
  SubclassWindow(hwnd, reinterpret_cast<HRGN>(lParam));
  return TRUE;
}

BOOL CALLBACK UnSubclassWindowsProc(HWND hwnd, LPARAM lParam) {
  UnSubclassWindow(hwnd);
  return TRUE;
}

}  // namespace

void RootWindowWin::OnSetDraggableRegions(
    const std::vector<CefDraggableRegion>& regions) {
  REQUIRE_MAIN_THREAD();

  // Reset draggable region.
  ::SetRectRgn(draggable_region_, 0, 0, 0, 0);

  // Determine new draggable region.
  std::vector<CefDraggableRegion>::const_iterator it = regions.begin();
  for (; it != regions.end(); ++it) {
    HRGN region = ::CreateRectRgn(it->bounds.x, it->bounds.y,
                                  it->bounds.x + it->bounds.width,
                                  it->bounds.y + it->bounds.height);
    ::CombineRgn(draggable_region_, draggable_region_, region,
                 it->draggable ? RGN_OR : RGN_DIFF);
    ::DeleteObject(region);
  }

  // Subclass child window procedures in order to do hit-testing.
  // This will be a no-op, if it is already subclassed.
  if (hwnd_) {
    WNDENUMPROC proc =
        !regions.empty() ? SubclassWindowsProc : UnSubclassWindowsProc;
    ::EnumChildWindows(hwnd_, proc,
                       reinterpret_cast<LPARAM>(draggable_region_));
  }
}

void RootWindowWin::NotifyDestroyedIfDone() {
  // Notify once both the window and the browser have been destroyed.
  if (window_destroyed_ && browser_destroyed_)
    delegate_->OnRootWindowDestroyed(this);
}




//20230218 BtnWndProc
LRESULT CALLBACK RootWindowWin::BtnWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    REQUIRE_MAIN_THREAD();
    RootWindowWin* self = GetUserDataPtr<RootWindowWin*>(hWnd);
    DCHECK(self);

    // Callback for the main window
    switch (message) {
    case WM_KEYDOWN:
        break;
    case WM_ERASEBKGND:
    {
        return TRUE;
    }
    break;
    case WM_MOUSEMOVE:
    {
        if (!self->_btnEntry)
        {
            TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(tme);
            tme.hwndTrack = hWnd;// self->_tool_hwnd;
            tme.dwFlags = TME_LEAVE | TME_HOVER;
            tme.dwHoverTime = 1;
            self->_btnEntry = ::TrackMouseEvent(&tme);
        }
    }

    break;
    case WM_MOUSELEAVE:
    {
        self->_btnEntry = FALSE;
        InvalidateRect(hWnd, NULL, TRUE);
        //UpdateWindow(hWnd);
    }
    break;
    case WM_MOUSEHOVER:
    {
        self->_currBtnID = 0;

        if (hWnd == self->back_hwnd_)
        {
            self->_currBtnID = IDC_NAV_BACK;
        }
        else if (hWnd == self->forward_hwnd_)
        {
            self->_currBtnID = IDC_NAV_FORWARD;
        }
        else if (hWnd == self->reload_hwnd_)
        {
            self->_currBtnID = IDC_NAV_RELOAD;
        }
        else if (hWnd == self->stop_hwnd_)
        {
            //self->_currBtnID = IDC_NAV_STOP;
        }
        else
        {
            self->_currBtnID = IDS_MENU_ICON_1X_PNG;
        }

        if (hWnd != self->stop_hwnd_)
        {
            self->_btnEntry = TRUE;
            InvalidateRect(hWnd, NULL, TRUE);
            //UpdateWindow(hWnd);
        }

    }
    break;
    }

    if (hWnd == self->back_hwnd_)
    {
        return CallWindowProc(self->_backWndProc, hWnd, message, wParam, lParam);
    }
    else if (hWnd == self->forward_hwnd_)
    {
        return CallWindowProc(self->_forwardWndProc, hWnd, message, wParam, lParam);
    }
    else if (hWnd == self->reload_hwnd_)
    {
        return CallWindowProc(self->_reloadWndProc, hWnd, message, wParam, lParam);
    }
    //else if (hWnd == self->stop_hwnd_)
    //{
    //	return CallWindowProc(self->_stopWndProc, hWnd, message, wParam, lParam);
    //}
    else
    {
        return CallWindowProc(self->_toolWndProc, hWnd, message, wParam, lParam);
    }
}


void RootWindowWin::transl1(HMENU hMenu, UINT id, wchar_t* buf)
{
    //wchar_t buf[200];
    //memset(buf, 0, 200 * sizeof(wchar_t));
 //	GetMenuString(hMenu, id, buf, 200, MF_BYCOMMAND);

    wstring szDst = L"";
    string s = CefString(buf);
    transl(s.c_str(), szDst);

    if (id == ID_PICOFYOU_ISO)
    {
        ModifyMenu(hMenu, 0, MF_BYPOSITION | MF_STRING, id, szDst.c_str());
    }
    else
    {
        ModifyMenu(hMenu, id, MF_BYCOMMAND | MF_STRING, id, szDst.c_str());
    }
}


void RootWindowWin::ShowHideToolBar(UINT nCmdShow)
{
    ShowWindow(back_hwnd_, nCmdShow);
    ShowWindow(reload_hwnd_, nCmdShow);
    //ShowWindow(stop_hwnd_, nCmdShow);
    ShowWindow(forward_hwnd_, nCmdShow);
    //ShowWindow(edit_hwnd_, nCmdShow);
    ShowWindow(_tool_hwnd, nCmdShow);

}

string RootWindowWin::Replace(string& szBody, string szOld, string szNew)
{
    if (szOld == szNew)
    {
        return szBody;
    }

    size_t nLen = szNew.length();

    string::size_type nFound = string::npos;
    do
    {
        nFound = szBody.find(szOld.c_str(), nFound != string::npos ? nFound + nLen : 0);
        if (nFound != string::npos)
        {
            szBody.replace(nFound, szOld.length(), szNew);
        }
    } while (nFound != string::npos);

    return szBody;
}

long RootWindowWin::read_svg(HINSTANCE hrcInst, UINT cr_id, string& xml)
{
    xml = "";

    HRSRC hrs = FindResource(hrcInst, MAKEINTRESOURCE(cr_id), RT_HTML);
    if (hrs != NULL)
    {
        DWORD dw = SizeofResource(hrcInst, hrs);
        HGLOBAL hg = LoadResource(hrcInst, hrs);
        if (hg != NULL)
        {
            void* p = LockResource(hg);
            if (p != NULL)
            {
                xml = string((LPSTR)p, dw);
            }
            FreeResource(hg);
        }
    }
    return 0;
}



HBITMAP RootWindowWin::set_svg(int w, int h, UINT cr_id, uint32_t bgcolor)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    string xml = "";
    read_svg(hInstance, cr_id, xml);
    if (!xml.empty())
    {
        std::uint32_t width = w, height = h;
        //std::uint32_t bgColor = 0x2B2B2BFF;
        //if (color == Scalar::all(218))
        //{
        //	bgColor = 0xDADADAFF;
        //}

        auto document = lunasvg::Document::loadFromData(xml);
        if (!document)
        {
            return NULL;
        }

        auto bitmap = document->renderToBitmap(width, height, bgcolor);
        if (!bitmap.valid())
        {
            return NULL;
        }

        width = bitmap.width();
        height = bitmap.height();

        int nchannels = 4;
        //imgTmp.depth()=0
        int depth = 0;

        unsigned char buffer[sizeof(BITMAPINFOHEADER) + 1024];
        BITMAPINFO* bmi = (BITMAPINFO*)buffer;
        {
            int bpp = (depth + 1) * nchannels;

            BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

            memset(bmih, 0, sizeof(*bmih));
            bmih->biSize = sizeof(BITMAPINFOHEADER);

            bmih->biWidth = width;
            bmih->biHeight = 0 - height;// origin ? abs(height) : -abs(height);
            bmih->biPlanes = 1;
            bmih->biBitCount = (unsigned short)bpp * 8;
            bmih->biCompression = BI_RGB;
        }

        HDC hDC = ::CreateCompatibleDC(0);


        char* pBits = NULL;
        HBITMAP hBitmap = CreateDIBSection(hDC, bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
        int pixelBytes = nchannels * (depth + 1);

        memcpy(pBits, bitmap.data(), pixelBytes * width * height);

        DeleteDC(hDC);

        return hBitmap;

        /*
            auto stride = bitmap.stride();
        auto rowData = bitmap.data();

        int x0 = (w0 - width) / 2;
        int y0 = (h0 - height) / 2;

        for (std::uint32_t y = 0; y < height; y++)
        {
            auto data = rowData;
            for (std::uint32_t x = 0; x < width; x++)
            {
                data += 4;
            }
            rowData += stride;
        }
        */
    }
    return NULL;
}

void RootWindowWin::make_bitmap(int cx, bool isWin8or)
{
    uint32_t	bg = 0xFFFFFFFF;
    uint32_t	bg_over = 0xDFDFDFFF;




    bg = GetSysColor(COLOR_MENUBAR);
    BYTE r = GetRValue(bg);
    BYTE g = GetGValue(bg);
    BYTE b = GetBValue(bg);
    uint32_t a = 255;

    bg = ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | (uint32_t)a;

    //bg_over = GetSysColor(COLOR_BTNSHADOW);
    r -= 12;
    g -= 12;
    b -= 12;
    a = 255;

    bg_over = ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | (uint32_t)a;



    if (!isWin8or)
    {
        bg = 0xF0F0F0FF;
        bg_over = 0xDFDFDFFF;
    }

    int e = 32;
    int e1 = 24;
    if (cx <= 32)
    {
        e = 24;
        e1 = 18;
    }
    else if (cx <= 40)
    {
        e = 28;
        e1 = 20;
    }
    else if (cx <= 48)
    {
        e = 32;
        e1 = 24;
    }
    else
    {
        e = 36;
        e1 = 28;
    }
    //LOG(ERROR) << "cx:" << cx;

    if (_hBmpReload != NULL)
    {
        DeleteObject(_hBmpReload);
        _hBmpReload = NULL;
    }
    if (_hBmpTool != NULL)
    {
        DeleteObject(_hBmpTool);
        _hBmpTool = NULL;
    }
    if (_hBmpBack != NULL)
    {
        DeleteObject(_hBmpBack);
        _hBmpBack = NULL;
    }
    if (_hBmpForward != NULL)
    {
        DeleteObject(_hBmpForward);
        _hBmpForward = NULL;
    }


    if (_hBmpReload_over != NULL)
    {
        DeleteObject(_hBmpReload_over);
        _hBmpReload_over = NULL;
    }
    if (_hBmpTool_over != NULL)
    {
        DeleteObject(_hBmpTool_over);
        _hBmpTool_over = NULL;
    }
    if (_hBmpBack_over != NULL)
    {
        DeleteObject(_hBmpBack_over);
        _hBmpBack_over = NULL;
    }
    if (_hBmpForward_over != NULL)
    {
        DeleteObject(_hBmpForward_over);
        _hBmpForward_over = NULL;
    }

    if (_hBmpBack_gray != NULL)
    {
        DeleteObject(_hBmpBack_gray);
        _hBmpBack_gray = NULL;
    }
    if (_hBmpForward_gray != NULL)
    {
        DeleteObject(_hBmpForward_gray);
        _hBmpForward_gray = NULL;
    }

    if (_hBmpReload_gray != NULL)
    {
        DeleteObject(_hBmpReload_gray);
        _hBmpReload_gray = NULL;
    }

    //0xRRGGBBAA
    _hBmpReload = set_svg(e1, e1, IDR_SVG_RELOAD, bg);
    _hBmpTool = set_svg(e1, e1, IDR_SVG_TOOL, bg);
    _hBmpBack = set_svg(e, e, IDR_SVG_BACK, bg);
    _hBmpForward = set_svg(e, e, IDR_SVG_FORWARD, bg);

    _hBmpBack_gray = set_svg(e, e, IDR_SVG_BACK_GRAY, bg);
    _hBmpForward_gray = set_svg(e, e, IDR_SVG_FORWARD_GRAY, bg);
    _hBmpReload_gray = set_svg(e1, e1, IDR_SVG_RELOAD_GRAY, bg);

    _hBmpReload_over = set_svg(e1, e1, IDR_SVG_RELOAD, bg_over);
    _hBmpTool_over = set_svg(e1, e1, IDR_SVG_TOOL, bg_over);
    _hBmpBack_over = set_svg(e, e, IDR_SVG_BACK, bg_over);
    _hBmpForward_over = set_svg(e, e, IDR_SVG_FORWARD, bg_over);
}







}  // namespace client
