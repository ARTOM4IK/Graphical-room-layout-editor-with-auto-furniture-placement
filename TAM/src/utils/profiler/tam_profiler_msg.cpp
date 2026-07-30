/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : profiler_msg.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Profiler support. Profiler window message handle module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 05.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

#include <psapi.h>
#include <cstdio>

#include "tam_profiler_font.h"

/* WM_CREATE window message handle function.
 * ARGUMENTS:
 *   - structure with creation data:
 *       CREATESTRUCT *CS;
 * RETURNS:
 *   (BOOL) TRUE to continue creation window, FALSE to terminate.
 */
BOOL tam::profiler::OnCreate( CREATESTRUCT *CS )
{
  SetTimer(hWnd, 102, 28 * 1 /* 1000 + 102 */, nullptr);

  HDC hDC = GetDC(hWnd);
  hDCFrame = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
  hBmFrame = nullptr;

  return TRUE;
} /* End of 'tam::profiler::OnCreate' function */

/* WM_DESTROY window message handle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::profiler::OnDestroy( VOID )
{
  if (DXGIAdapter != nullptr)
    DXGIAdapter->Release(), DXGIAdapter = nullptr;
  DeleteObject(hBmFrame);
  DeleteDC(hDCFrame);
} /* End of 'tam::profiler::OnDestroy' function */

/* WM_SIZE window message handle function.
 * ARGUMENTS:
 *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
 *       UINT State;
 *   - new width and height of client area:
 *       INT W, H;
 * RETURNS: None.
 */
VOID tam::profiler::OnSize( UINT State, INT W, INT H )
{
  const std::lock_guard<std::recursive_mutex> lock(profiler_mutex_display);
  if (State == SIZE_MINIMIZED)
    // Block window collapsing
    ShowWindow(hWnd, SW_SHOWNORMAL);
  BITMAPINFOHEADER bmih;

  /* Delete old frame */
  DeleteObject(hBmFrame);

  /* Create new frame */
  bmih.biSize = sizeof(BITMAPINFOHEADER);
  bmih.biBitCount = 32;                             // BGRx
  bmih.biPlanes = 1;                                // Always 1
  bmih.biCompression = BI_RGB;                      // 0 - no compression
  bmih.biWidth = win::W;                            // width
  bmih.biHeight = -win::H;                          // height, order from top to bottom
  bmih.biSizeImage = win::W * win::H * 4;           // image size in bytes
  bmih.biClrUsed = 0;                               // no color table
  bmih.biClrImportant = 0;
  bmih.biXPelsPerMeter = bmih.biYPelsPerMeter = 0;   // resolution
  hBmFrame = CreateDIBSection(nullptr, (BITMAPINFO *)&bmih, DIB_RGB_COLORS, (VOID **)&FrameBits, nullptr, 0);

  SelectObject(hDCFrame, hBmFrame);
  SendMessage(hWnd, WM_TIMER, 0, 0);
} /* End of 'tam::profiler::OnSize' function */

/* WM_TIMER window message handle function.
 * ARGUMENTS:
 *   - specified the timer identifier.
 *       INT Id;
 * RETURNS: None.
 */
VOID tam::profiler::OnTimer( INT Id )
{
  static CHAR Buf[102];
  SYSTEMTIME st;

  if (DXGIAdapter == nullptr)
  {
    IDXGIFactory4 *DXGIFactory = nullptr;
    CreateDXGIFactory1(IID_PPV_ARGS(&DXGIFactory));

    if (DXGIFactory != nullptr)
    {
      IDXGIAdapter1 *TempDXGIiAdapter = nullptr;
      UINT AdapterIndex = 0;

      while (DXGIFactory->EnumAdapters1(AdapterIndex, &TempDXGIiAdapter) != DXGI_ERROR_NOT_FOUND)
      {
        DXGI_ADAPTER_DESC1 desc;
        TempDXGIiAdapter->GetDesc1(&desc);
        if (memcmp(&desc.AdapterLuid, DeviceLUID, VK_LUID_SIZE) == 0)
        //if (DXGIAdapter == nullptr && desc.Flags == 0)
        {
          TempDXGIiAdapter->QueryInterface(IID_PPV_ARGS(&DXGIAdapter));
        }
        TempDXGIiAdapter->Release();
        AdapterIndex++;
      }
      DXGIFactory->Release();
    }
  }

  DXGI_QUERY_VIDEO_MEMORY_INFO MemInfo {};
  if (DXGIAdapter != nullptr)
    DXGIAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &MemInfo);

  // Redraw profiler screen
  //memset(FrameBits, 0x08, win::W * win::H * 4);
  DWORD *ptr = FrameBits, cnt = win::W * win::H;
  while (cnt-- > 0)
    *ptr++ = 0x181828;

  INT x;
  GetLocalTime(&st);
  x = PutStr(8, 8, "CGSG-TAM Profiler ", 0xFFFFFFFF);

  PROCESS_MEMORY_COUNTERS mi;
  mi.cb = sizeof(mi);
  GetProcessMemoryInfo(GetCurrentProcess(), &mi, sizeof(mi));
  sprintf(Buf, "CPU Mem: %.3f MiB::%s ", mi.WorkingSetSize / (1024 * 1024.0), CPUName.c_str());
  x = PutStr(x, 8, Buf, 0xFFCCFFCC);

  //sprintf(Buf, "CPU: %s", );
  //PutStr(8 + 102 * 2 + 50 + 18 * 2, 8, Buf, 0xFFAAFFDD);
  sprintf(Buf, "GPU: %s", DeviceName);
  PutStr(x, 8, Buf, 0xFF55FF55);

  sprintf(Buf, "%02d:%02d:%02d %02d.%02d.%04d ",
    st.wHour, st.wMinute, st.wSecond, st.wDay, st.wMonth, st.wYear);
  x = PutStr(8, 27, Buf, 0xFFFFCCCC);
  sprintf(Buf, "FPS: %10.3f ", FPS);
  x = PutStr(x, 27, Buf, 0xFFAAFFFF);
  sprintf(Buf, "GPU: total memory - %.3f GiB", TotalGPUMemory / (1024 * 1024 * 1024.0));
  PutStr(x, 27, Buf, 0xFF55FF55);

  sprintf(Buf, "Frame: %dx%d ", RenderFrameW, RenderFrameH);
  x = PutStr(8, 47, Buf, 0xFFFFFFFF);
  sprintf(Buf, "GPU Mem: budget %.1f MiB, used %.1f MiB, avail %.1f, res %.1f",
    MemInfo.Budget / (1024 * 1024.0),
    MemInfo.CurrentUsage / (1024 * 1024.0),
    MemInfo.AvailableForReservation / (1024 * 1024.0),
    MemInfo.CurrentReservation / (1024 * 1024.0));
  PutStr(x, 47, Buf, 0xFF55FF55);

  Display();

  // Output to window
  HDC hDC = GetDC(hWnd);
  // BitBlt(hDC, 0, 0, window::W, window::H, hDCFrame, 0, 0, SRCCOPY);
  StretchBlt(hDC, 0, 0, win::W * FrameZoom, win::H * FrameZoom, hDCFrame, 0, 0, win::W, win::H, SRCCOPY);
  ReleaseDC(hWnd, hDC);
} /* End of 'tam::profiler::OnTimer' function */

/* WM_ERASEBKGND window message handle function.
 * ARGUMENTS:
 *   - device context of client area:
 *       HDC hDC;
 * RETURNS:
 *   (BOOL) TRUE if background is erased, FALSE otherwise.
 */
BOOL tam::profiler::OnEraseBkgnd( HDC hDC )
{
  return 1;
} /* End of 'tam::profiler::OnEraseBkgnd' function */

/* WM_PAINT window message handle function.
 * ARGUMENTS:
 *   - window device context:
 *       HDC hDC;
 *   - paint message structure pointer:
 *       PAINTSTRUCT *PS;
 * RETURNS: None.
 */
VOID tam::profiler::OnPaint( HDC hDC, PAINTSTRUCT *PS )
{
  // BitBlt(hDC, 0, 0, window::W, window::H, hDCFrame, 0, 0, SRCCOPY);
  StretchBlt(hDC, 0, 0, win::W * FrameZoom, win::H * FrameZoom, hDCFrame, 0, 0, win::W, win::H, SRCCOPY);
} /* End of 'tam::profiler::OnPaint' function */

/* WM_KEYDOWN/WM_KEYUP window message handle function.
 * ARGUMENTS:
 *   - virtual key number (see VK_***, 'A', ..., '0', ...):
 *       UINT Key;
 *   - press flag:
 *       BOOL IsPress;
 *   - autorepeat counter:
 *       INT RepeatCount;
 *   - shift/control etc key states bits (see MK_***):
 *       UINT ShiftKeysFlags;
 * RETURNS: None.
 */
VOID tam::profiler::OnKey( UINT Key, BOOL IsPress, INT RepeatCount, UINT ShiftKeysFlags )
{
  if (IsPress)
    switch (Key)
    {
    case 'P':
      if (GetAsyncKeyState(VK_SHIFT) & 0x8000 &&
          GetAsyncKeyState(VK_MENU) & 0x8000 &&
          GetAsyncKeyState(VK_CONTROL) & 0x8000)
        if (GetWindowLong(hWnd, GWL_STYLE) & WS_VISIBLE)
          ShowWindow(hWnd, SW_HIDE);
        else
          ShowWindow(hWnd, SW_SHOW);
      return;
    case '1':
      SetLayeredWindowAttributes(hWnd, 0, 90, LWA_ALPHA);
      return;
    case '2':
      SetLayeredWindowAttributes(hWnd, 0, 130, LWA_ALPHA);
      return;
    case '3':
      SetLayeredWindowAttributes(hWnd, 0, 190, LWA_ALPHA);
      return;
    case '4':
      SetLayeredWindowAttributes(hWnd, 0, 220, LWA_ALPHA);
      return;
    case '5':
      SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
      return;
    case VK_ADD:
      if (FrameZoom < 8)
        FrameZoom *= 1.25;
      break;
    case VK_SUBTRACT:
      if (FrameZoom > 0.5)
        FrameZoom /= 1.25;
      break;
    }
} /* End of 'tam::profiler::OnKey' function */

/* Window message universal handle function.
 * Should be returned 'DefWindowProc' call result.
 * ARGUMENTS:
 *   - message type (see WM_***):
 *      UINT Msg;
 *   - message 'word' parameter:
 *      WPARAM wParam;
 *   - message 'long' parameter:
 *      LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message depende return value.
 */
LRESULT tam::profiler::OnMessage( UINT Msg, WPARAM wParam, LPARAM lParam )
{
  switch (Msg)
  {
  case WM_CLOSE:
    // Block window closing
    ShowWindow(hWnd, SW_HIDE);
    return 0;
  // case WM_SETFOCUS:
  //   SetFocus(hWndMain);
  //   return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'tam::profiler::OnMessage' function */

/* END OF 'tam_profiler_msg.cpp' FILE */
