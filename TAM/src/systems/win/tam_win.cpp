/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_win.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Window class handle implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 24.01.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

/* Class constructor.
 * ARGUMENTS:
 *   - application instance handle:
 *       HINSTANCE hInst;
 */
tam::win::win( HINSTANCE hInst ) :
  hWnd(nullptr), W(0), H(0), hInstance(hInst),
  IsFullScreen(FALSE), FullScreenSaveRect({}),
  MouseWheel(0), IsActive(FALSE)
{
} /* End of 'tam::win::win' function */

/* Class constructor */
tam::win::~win( VOID )
{
} /* End of 'tam::win::~win' function */

/* Main window create function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::win::Create( VOID )
{
  static std::recursive_mutex win_mutex;
  const std::lock_guard<std::recursive_mutex> lock(win_mutex);

  // Register window class
  WNDCLASS wc;

  if (!GetClassInfo(hInstance, WindowClassName, &wc))
  {
    wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof(win *);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(nullptr, (CHAR *)IDC_HAND);
    wc.hIcon = LoadIcon(hInstance, (CHAR *)TAM_IDI_LOGO);
    // wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
    wc.lpszMenuName = nullptr;
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WinFunc;
    wc.lpszClassName = WindowClassName;

    if (!RegisterClass(&wc))
    {
      MessageBox(nullptr, "Error register window class", "ERROR", MB_OK);
      return;
    }
  }
  /* Create window */
  hWnd =
    CreateWindow(WindowClassName,
      "CGSG::TAM Project",
      WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      //2560, 0, 1000, 900,
      nullptr, nullptr, hInstance, reinterpret_cast<VOID *>(this));

  /* Show window */
  ShowWindow(hWnd, SW_SHOWNORMAL);
  /// UpdateWindow(hWnd);
} /* End of 'tam::win::Create' function */

/* Main window handle loop function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::win::Run( VOID )
{
  MSG msg;

  while (TRUE)
  {
    /* Check message at window message queue */
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
      if (msg.message == WM_QUIT)
        break;
      else
      {
        /* Displatch message to window */
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    else
    {
      /* Idle... */
    }
  }
} /* End of 'tam::win::Run' function */

/* Application terminate function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::win::DoExit( VOID )
{
  DestroyWindow(hWnd);
} /* End of 'tam::win::DoExit' function */

/* Window message handle function (CALLBACK version).
 *   - window handle:
 *       HWND hWnd;
 *   - message identifier (see WM_***):
 *       UINT Msg;
 *   - message first parameter ('word' param):
 *       WPARAM wParam;
 *   - message second parameter ('long' param):
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message return code (depended to Msg type).
 */
LRESULT CALLBACK tam::win::WinFunc( HWND hWnd, UINT Msg,
                                     WPARAM wParam, LPARAM lParam )
{
  if (ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam))
    return true;

  win *Win;
  HDC hDC;
  PAINTSTRUCT ps;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    ((MINMAXINFO *)lParam)->ptMinTrackSize.y =
      GetSystemMetrics(SM_CYCAPTION) * 8 +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    ((MINMAXINFO *)lParam)->ptMinTrackSize.x += 300;
    return 0;
  case WM_CREATE:
    /* Attach 'this' pointer to window class to window */
    SetWindowLongPtr(hWnd, 0, (UINT_PTR)((CREATESTRUCT *)lParam)->lpCreateParams);
  default:
    Win = reinterpret_cast<win *>(GetWindowLongPtr(hWnd, 0));
    if (Win != NULL)
      switch (Msg)
      {
      case WM_CREATE:
        Win->hWnd = hWnd;
        return Win->OnCreate((CREATESTRUCT *)lParam) ? 0 : -1;
      case WM_SIZE:
        Win->W = (INT)(SHORT)LOWORD(lParam);
        Win->H = (INT)(SHORT)HIWORD(lParam);
        Win->OnSize(wParam, LOWORD(lParam), HIWORD(lParam));
        return 0;
      case WM_ACTIVATE:
        Win->IsActive = LOWORD(wParam) != WA_INACTIVE;
        Win->OnActivate((UINT)LOWORD(wParam),(HWND)(lParam),(BOOL)HIWORD(wParam));
        return 0;
      case WM_ERASEBKGND:
        return (LRESULT)Win->OnEraseBkgnd((HDC)wParam);
      case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        Win->OnPaint(hDC, &ps);
        EndPaint(hWnd, &ps);
        return 0;
      case WM_DRAWITEM:
        Win->OnDrawItem((INT)wParam, (DRAWITEMSTRUCT *)lParam);
        return 0;
      case WM_TIMER:
        Win->OnTimer((UINT)wParam);
        return 0;
      case WM_MOUSEWHEEL:
        Win->OnMouseWheel((INT)(SHORT)LOWORD(lParam),
                          (INT)(SHORT)HIWORD(lParam),
                          (INT)(SHORT)HIWORD(wParam),
                          (UINT)(SHORT)LOWORD(wParam));
        return 0;
      case WM_LBUTTONDBLCLK:
        Win->OnButtonDown(TRUE, (INT)(SHORT)LOWORD(lParam),
                          (INT)(SHORT)HIWORD(lParam),(UINT)(SHORT)LOWORD(wParam));
        return 0;
      case WM_LBUTTONDOWN:
      case WM_RBUTTONDOWN:
      case WM_MBUTTONDOWN:
        Win->OnButtonDown(FALSE, (INT)(SHORT)LOWORD(lParam),
                          (INT)(SHORT)HIWORD(lParam), (UINT)(SHORT)LOWORD(wParam));
        return 0;
      case WM_LBUTTONUP:
      case WM_RBUTTONUP:
      case WM_MBUTTONUP:
        Win->OnButtonUp((INT)(SHORT)LOWORD(lParam), (INT)(SHORT)HIWORD(lParam), 
                        (UINT)(SHORT)LOWORD(wParam));
        return 0;
      case WM_MOUSEMOVE:
        Win->OnMouseMove((INT)(SHORT)LOWORD(lParam), (INT)(SHORT)HIWORD(lParam), (UINT)(SHORT)LOWORD(wParam));
        return 0;
      case WM_KEYDOWN:
        Win->OnKey((UINT)(wParam), TRUE, (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam));
        return 0;
      case WM_KEYUP:
        Win->OnKey((UINT)(wParam), FALSE, (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam));
        return 0;
      case WM_DROPFILES:
        Win->OnDropFiles((HDROP)wParam);
        return 0;
      case WM_NOTIFY:
        return Win->OnNotify((INT)wParam, (NMHDR *)lParam);
      case WM_COMMAND:
        Win->OnCommand((INT)LOWORD(wParam), (HWND)lParam, (UINT)HIWORD(wParam));
        return 0;
      case WM_MENUSELECT:
        Win->OnMenuSelect((HMENU)lParam,
          (HIWORD(wParam) & MF_POPUP) ? 0L : (INT)LOWORD(wParam),
          (HIWORD(wParam) & MF_POPUP) ?
            GetSubMenu((HMENU)lParam, LOWORD(wParam)) : 0L,
          (UINT)(((SHORT)HIWORD(wParam) == -1) ? 0xFFFFFFFF : HIWORD(wParam)));
        return 0;
      case WM_INITMENUPOPUP:
        Win->OnInitMenuPopup((HMENU)wParam, (UINT)
          LOWORD(lParam), (BOOL)HIWORD(lParam));
        return 0;
      case WM_HSCROLL:
        Win->OnHScroll((HWND)lParam, LOWORD(wParam), HIWORD(wParam));
        return 0;
      case WM_VSCROLL:
        Win->OnVScroll((HWND)lParam, LOWORD(wParam), HIWORD(wParam));
        return 0;
      case WM_ENTERSIZEMOVE:
        Win->OnEnterSizeMove();
        return 0;
      case WM_EXITSIZEMOVE:
        Win->OnExitSizeMove();
        return 0;
      case WM_DESTROY:
        Win->OnDestroy();
        return 0;
      default:
        return Win->OnMessage(Msg, wParam, lParam);
      }
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'tam::win::WinFunc' function */

/* Toggle full screen function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::win::FlipFullScreen( VOID )
{
  if (IsFullScreen)
  {
    /* Restore window size */
    SetWindowPos(hWnd, HWND_TOP,
      FullScreenSaveRect.left, FullScreenSaveRect.top,
      FullScreenSaveRect.right - FullScreenSaveRect.left,
      FullScreenSaveRect.bottom - FullScreenSaveRect.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    /* Set full screen size to window */
    HMONITOR hmon;
    MONITORINFOEX moninfo;
    RECT rc;

    /* Store window old size */
    GetWindowRect(hWnd, &FullScreenSaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    /* Obtain monitor info */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* Set window new size */
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
} /* End of 'tam::win::FlipFullScreen' function */

/* END OF 'tam_win.cpp' FILE */
