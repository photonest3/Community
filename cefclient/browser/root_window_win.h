// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_BROWSER_ROOT_WINDOW_WIN_H_
#define CEF_TESTS_CEFCLIENT_BROWSER_ROOT_WINDOW_WIN_H_
#pragma once

#include <windows.h>

#include <commdlg.h>

#include <memory>
#include <string>

#include "tests/cefclient/browser/browser_window.h"
#include "tests/cefclient/browser/root_window.h"
#include "IsoHandler.h"

namespace client {
#define WM_RUNINFO WM_USER + 100
#define WM_PAINT_CLOSE0 WM_USER + 102
#define WM_PAINT_CLOSE WM_USER + 105
#define WM_PAINT_RUN WM_USER + 106
#define WM_PAINT_SAVE WM_USER + 107
#define WM_PAINT_SIZE WM_USER + 108
#define WM_PAINT_MOVE WM_USER + 109
#define WM_CEF_CLOSE WM_USER + 110
#define WM_PAINT_MASK WM_USER + 111
#define WM_PAINT_INIT WM_USER + 112
#define WM_PAINT_DPICHANGED WM_USER + 115
#define WM_UPLOADING WM_USER + 150
#define WM_CLOSE_UPLOADING WM_USER + 160

// Windows implementation of a top-level native window in the browser process.
// The methods of this class must be called on the main thread unless otherwise
// indicated.
class RootWindowWin : public RootWindow, public BrowserWindow::Delegate {
 public:
  // Constructor may be called on any thread.
  RootWindowWin();
  ~RootWindowWin();

  // RootWindow methods.
  void Init(RootWindow::Delegate* delegate,
            std::unique_ptr<RootWindowConfig> config,
            const CefBrowserSettings& settings) override;
  void InitAsPopup(RootWindow::Delegate* delegate,
                   bool with_controls,
                   bool with_osr,
                   const CefPopupFeatures& popupFeatures,
                   CefWindowInfo& windowInfo,
                   CefRefPtr<CefClient>& client,
                   CefBrowserSettings& settings) override;
  void Show(ShowMode mode) override;
  void Hide() override;
  void SetBounds(int x, int y, size_t width, size_t height) override;
  void Close(bool force) override;
  void SetDeviceScaleFactor(float device_scale_factor) override;
  float GetDeviceScaleFactor() const override;
  CefRefPtr<CefBrowser> GetBrowser() const override;
  ClientWindowHandle GetWindowHandle() const override;
  bool WithWindowlessRendering() const override;
  bool WithExtension() const override;

 private:
  void CreateBrowserWindow(const std::string& startup_url);
  void CreateRootWindow(const CefBrowserSettings& settings,
                        bool initially_hidden);

  // Register the root window class.
  static void RegisterRootClass(HINSTANCE hInstance,
                                const std::wstring& window_class,
                                HBRUSH background_brush);












  // Window procedure for the edit field.
  static LRESULT CALLBACK EditWndProc(HWND hWnd,
                                      UINT message,
                                      WPARAM wParam,
                                      LPARAM lParam);

  // Window procedure for the find dialog.
  static LRESULT CALLBACK FindWndProc(HWND hWnd,
                                      UINT message,
                                      WPARAM wParam,
                                      LPARAM lParam);

  // Window procedure for the root window.
  static LRESULT CALLBACK RootWndProc(HWND hWnd,
                                      UINT message,
                                      WPARAM wParam,
                                      LPARAM lParam);


  //20230218
  static HICON HICONFromHBITMAP(HBITMAP hBitmap);

  static LRESULT CALLBACK BtnWndProc(HWND hWnd,
      UINT message,
      WPARAM wParam,
      LPARAM lParam);


  static void transl1(HMENU hMenu, UINT id, wchar_t* buf);

  bool OnSysCommand(HWND hWnd, UINT id);




  // Event handlers.
  void OnPaint();
  void OnFocus();
  void OnActivate(bool active);
  void OnSize(bool minimized);
  void OnMove();
  void OnDpiChanged(WPARAM wParam, LPARAM lParam);
  bool OnEraseBkgnd();
  bool OnCommand(UINT id, LPARAM lParam);
  void OnFind();
  void OnFindEvent();
  void OnAbout();
  void OnNCCreate(LPCREATESTRUCT lpCreateStruct);
  void OnCreate(LPCREATESTRUCT lpCreateStruct);
  bool OnClose(HWND hWnd);
  void OnDestroyed();

  // BrowserWindow::Delegate methods.
  void OnBrowserCreated(CefRefPtr<CefBrowser> browser) override;
  void OnBrowserWindowDestroyed() override;
  void OnSetAddress(const std::string& url) override;
  void OnSetTitle(const std::string& title) override;
  void OnSetFullscreen(bool fullscreen) override;
  void OnAutoResize(const CefSize& new_size) override;
  void OnSetLoadingState(bool isLoading,
                         bool canGoBack,
                         bool canGoForward) override;
  void OnSetDraggableRegions(
      const std::vector<CefDraggableRegion>& regions) override;

  void NotifyDestroyedIfDone();

  // After initialization all members are only accessed on the main thread.
  // Members set during initialization.
  bool with_controls_;
  bool always_on_top_;
  bool with_osr_;
  bool with_extension_;
  bool is_popup_;
  RECT start_rect_;
  std::unique_ptr<BrowserWindow> browser_window_;
  CefBrowserSettings browser_settings_;
  bool initialized_;

  // Main window.
  HWND hwnd_;

  // Draggable region.
  HRGN draggable_region_;

  // Font for buttons and text fields.
  HFONT font_;
  int font_height_;

  // Buttons.
  HWND back_hwnd_;
  HWND forward_hwnd_;
  HWND reload_hwnd_;
  HWND stop_hwnd_;

  // URL text field.
  HWND edit_hwnd_;
  WNDPROC edit_wndproc_old_;

  // Find dialog.
  HWND find_hwnd_;
  UINT find_message_id_;
  WNDPROC find_wndproc_old_;

  // Find dialog state.
  FINDREPLACE find_state_;
  WCHAR find_buff_[80];
  std::wstring find_what_last_;
  bool find_next_;
  bool find_match_case_last_;

  bool window_destroyed_;
  bool browser_destroyed_;

  bool called_enable_non_client_dpi_scaling_;

  //20230218 OnDir
  void ShowHideToolBar(UINT nCmdShow);
  void OnDir(int dirType);
  std::string command_args_;

  //20230218
  void DrawTheIcon(HWND hButtonWnd, HDC* dc, UINT uID, RECT* rpItem, BOOL bIsPressed, BOOL bIsDisabled, BOOL bIsFocused);
  static string Replace(string& szBody, string szOld, string szNew);
  long read_svg(HINSTANCE hrcInst, UINT cr_id, string& xml);
  HBITMAP set_svg(int w, int h, UINT cr_id, uint32_t bgcolor);
  void make_bitmap(int cx, bool isWin8or);

  bool isLoading_;

  HBITMAP _hBmpReload;
  HBITMAP _hBmpTool;
  HBITMAP _hBmpBack;
  HBITMAP _hBmpForward;


  HBITMAP _hBmpReload_over;
  HBITMAP _hBmpTool_over;
  HBITMAP _hBmpBack_over;
  HBITMAP _hBmpForward_over;


  HBITMAP _hBmpBack_gray;
  HBITMAP _hBmpForward_gray;
  HBITMAP _hBmpReload_gray;


  HWND _tool_hwnd;

  HBRUSH _hMenuBrush;
  HBRUSH _hToolBarBrush;
  HBRUSH _brBtnShadow;

  HMENU _pMainMenu;
  BOOL _btnEntry;
  WNDPROC _backWndProc;
  WNDPROC _forwardWndProc;
  WNDPROC _reloadWndProc;
  WNDPROC _stopWndProc;
  WNDPROC _toolWndProc;
  UINT _currBtnID;

  string _lasturl;
  //UINT _CURR_GALLERY_MENUID;
  UINT _CURR_ZOOM_MENUID;

  HWND _hwndPaint;

  bool _painting;
  int _uploading;

  DISALLOW_COPY_AND_ASSIGN(RootWindowWin);
};

}  // namespace client

#endif  // CEF_TESTS_CEFCLIENT_BROWSER_ROOT_WINDOW_WIN_H_
