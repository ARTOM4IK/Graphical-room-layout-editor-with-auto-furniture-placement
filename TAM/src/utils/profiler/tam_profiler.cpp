/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_profiler.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Profiler support. Main implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 05.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

#include "tam_profiler_font.h"
#include <dwmapi.h>

#pragma comment(lib, "dwmapi")

/* Profiler font reference */
static tam::profiler_font ProfilerFont;

UINT64
  /* Start profiler time */
  tam::profiler::StartTime =
    []( VOID ) -> UINT64
    {
      LARGE_INTEGER ts;
      QueryPerformanceCounter(&ts);
      return ts.QuadPart;
    }(),
  /* Timer resolution */
  tam::profiler::TimePerSec =
    []( VOID ) -> UINT64
    {
      LARGE_INTEGER ts;
      QueryPerformanceFrequency(&ts);
      return ts.QuadPart;
    }();

/* Class constructor */
tam::profiler::profiler( VOID )
{
  WindowClassName = "CGSG-TAM Vulkan Animation profiler window class name";
  Create();
  FrameZoom = 1;
  SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_STATICEDGE);
  OnSize(0, win::W, win::H);
  SetWindowText(hWnd, "CGSG :: TAM Project :: Profiler");

  /// HICON hIcon;
  /// hIcon = (HICON)LoadImage(hInstance, (CHAR *)IDI_LOGO, IMAGE_ICON,
  ///   GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
  /// SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
  /// hIcon = (HICON)LoadImage(hInstance, (CHAR *)IDI_LOGO, IMAGE_ICON,
  ///   GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
  /// SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
  /// 
  /// hIcon = (HICON)LoadImage(hInstance, (CHAR *)IDI_LOGO, IMAGE_ICON,
  ///   GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
  /// SendMessage(hWnd, WM_SETICON, ICON_SMALL2, (LPARAM)hIcon);


  // Create and populate the Blur Behind structure
  DWM_BLURBEHIND bb = {0};

  // Enable Blur Behind and apply to the entire client area
  bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION | DWM_BB_TRANSITIONONMAXIMIZED;
  bb.fEnable = TRUE;
  bb.hRgnBlur = nullptr;
  bb.fTransitionOnMaximized = TRUE;

  // Apply Blur Behind
  // DwmEnableBlurBehindWindow(hWnd, &bb);

  // Setup alpha layer for profiler
  SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);

  /* Show win */
  UpdateWindow(hWnd);
  ShowWindow(hWnd, SW_HIDE);
  ShowWindow(hWnd, SW_SHOWNORMAL);

  //SetActiveWindow(TMP->GetWindowHandle());
  //SetFocus(TMP->GetWindowHandle());
} /* End of 'tam::profiler::profiler' function */

/* Profiling total init function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::profiler::Init( VOID )
{
  std::thread::id tid = std::this_thread::get_id();
  if (Threads.find(tid) == Threads.end())
  {
    PROFILER_THREAD pti {};
    Threads[tid] = pti;
  }
} /* End of 'tam::profiler::Init' function */

/* Profiling restart function.
 * ARGUMENTS:
 *   - current frame size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID tam::profiler::Reset( INT W, INT H )
{
  // Lock access
  const std::lock_guard<std::recursive_mutex> lock(profiler_mutex_display);
  Init();
  Threads[std::this_thread::get_id()].StampsCount = 0;
  Threads[std::this_thread::get_id()].CountersCount = 0;
  if (W > 0)
    RenderFrameW = W;
  if (H > 0)
    RenderFrameH = H;
} /* End of 'tam::profiler::Reset' function */

/* Collect profiling information function.
 * ARGUMENTS:
 *   - profile sample name:
 *       const CHAR *ProfStampName;
 * RETURNS: None.
 */
VOID tam::profiler::Collect( const CHAR *ProfStampName )
{
  // Lock access
  const std::lock_guard<std::recursive_mutex> lock(profiler_mutex_display);
  // Get current time in seconds
  LARGE_INTEGER ts;
  QueryPerformanceCounter(&ts);
  DBL t = (DBL)(ts.QuadPart - StartTime) / TimePerSec;

  Init();
  auto &Tr = Threads[std::this_thread::get_id()];

  if (Tr.StampsCount < 0 || !Tr.IsCollection)
  {
    Tr.LastTimeStamp = t;
    return;
  }

  if (Tr.StampsCount >= Tr.Stamps.size())
    return;

  PROFILER_STAMP &St = Tr.Stamps[Tr.StampsCount];
  St.Name = ProfStampName;
  if (Tr.StampsCount == 0)
    St.Accum += t - St.LastStamp;
  else
    St.Accum += t - Tr.LastTimeStamp;
  St.Count++;
  St.LastStamp = t;
  if (St.Count > 1 && t - St.LastMeasure > 3)
  {
    St.Average = St.Accum / St.Count;
    St.Accum = 0;
    St.Count = 0;
    St.LastMeasure = t;
  }
  Tr.LastTimeStamp = t;
  Tr.StampsCount++;
  if (Tr.StampsCount > Tr.StampsFilled)
    Tr.StampsFilled = Tr.StampsCount;
} /* End of 'tam::profiler::Collect' function */

/* Collect profiling counter information function.
 * ARGUMENTS:
 *   - profile counter name:
 *       const CHAR *ProfStampName;
 *   - counter value:
 *       DBL CounterValue;
 * RETURNS: None.
 */
VOID tam::profiler::CollectCounter( const CHAR *ProfCounterName, DBL CounterValue )
{
  // Lock access
  const std::lock_guard<std::recursive_mutex> lock(profiler_mutex_display);
  // Get current time in seconds
  LARGE_INTEGER ts;
  QueryPerformanceCounter(&ts);
  DBL t = (DBL)(ts.QuadPart - StartTime) / TimePerSec;

  Init();
  auto &Tr = Threads[std::this_thread::get_id()];

  if (Tr.CountersCount < 0 || !Tr.IsCollection)
  {
    Tr.LastTimeStamp = t;
    return;
  }

  if (Tr.CountersCount >= Tr.Counters.size())
    return;

  PROFILER_STAMP &St = Tr.Counters[Tr.CountersCount];
  St.Name = ProfCounterName;
  // if (Tr.CountersCount == 0)
  //   St.Accum += CounterValue;
  // else
  St.Accum += CounterValue;
  St.Count++;
  St.LastStamp = t;
  if (St.Count > 1 && t - St.LastMeasure > 3)
  {
    St.Average = St.Accum / St.Count;
    St.Accum = 0;
    St.Count = 0;
    St.LastMeasure = t;
  }
  Tr.LastTimeStamp = t;
  Tr.CountersCount++;
  if (Tr.CountersCount > Tr.CountersFilled)
    Tr.CountersFilled = Tr.CountersCount;
} /* End of 'tam::profiler::CollectCounter' function */

/* Set current profile name function.
 * ARGUMENTS:
 *   - profile name:
 *       const CHAR *ProfName;
 * RETURNS: None.
 */
VOID tam::profiler::SetName( const CHAR *ProfName )
{
  const std::lock_guard<std::recursive_mutex> lock(profiler_mutex_display);
  Init();
  Threads[std::this_thread::get_id()].Name = ProfName;
} /* End of 'tam::profiler::SetName' function */

/* Set current profile wait flag function.
 * ARGUMENTS:
 *   - wait flag:
 *       BOOL IsWait;
 * RETURNS: None.
 */
VOID tam::profiler::SetWaitFlag( BOOL IsWait )
{
  const std::lock_guard<std::recursive_mutex> lock(profiler_mutex_display);
  Init();
  Threads[std::this_thread::get_id()].IsWait = IsWait;
} /* End of 'tam::profiler::SetName' function */

/* Draw profiling information function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::profiler::Display( VOID )
{
  // Lock access
  const std::lock_guard<std::recursive_mutex> lock(profiler_mutex_display);
  INT cnt = 0, hn = 1, tn = 0, x = 0, y = 47 + 18, tr_w = 370;
  static CHAR Buf[100000];
  for (auto &pi : Threads)
  {
    INT len = 0;
    static DBL LastRedrawTime;

    auto &Tr = pi.second;
    if (hn < Tr.StampsFilled)
      hn = (INT)Tr.StampsFilled;

    //if (!Tr.IsVisible)
    //  continue;

    // Update texture
    len += sprintf(Buf + len, "Thread (#%d/%d): %s\n\n", tn, *(DWORD *)&pi.first, Tr.Name);
    PutStr(x + 8, y + 8, Buf, 0xFFCCFFCC);
    len = 0;
    for (INT i = 0; i < Tr.StampsFilled; i++)
      if (Tr.Stamps[i].Name != nullptr)
      {
        DBL pc = Tr.Stamps[0].Average != 0 ? 100 * Tr.Stamps[i].Average / Tr.Stamps[0].Average : i == 0 ? 100 : 0;

        len += sprintf(Buf + len, "%22.22s:%9.5fms [%6.2f%%]\n",
          Tr.Stamps[i].Name, Tr.Stamps[i].Average * 1000, pc);
        if (i == 0)
          FPS = 1.0 / Tr.Stamps[i].Average;
      }
    PutStr(x + 8, y + 8 + ProfilerFont.H, Buf, 0xFFFFFFAA);

    PutBar(x + 8 - 8, y + 8 + 4, 8, 8, Tr.IsWait ? 0xFFFF0000 : 0xFF00FF00);

    len = 0;
    Buf[0] = 0;
    PutStr(x + tr_w + 8, y + 8 + 1 * ProfilerFont.H, "Counters:", 0xFFAAFFAA);
    for (INT i = 0; i < Tr.CountersFilled; i++)
      if (Tr.Counters[i].Name != nullptr)
        len += sprintf(Buf + len, "%18.18s:%18.3f\n", Tr.Counters[i].Name, Tr.Counters[i].Average);
    PutStr(x + tr_w + 8, y + 8 + 2 * ProfilerFont.H, Buf, 0xFFAAFFFF);

    PutBar(x + tr_w * 2, y + 8 + ProfilerFont.H, 18, 18, 0xFFFFAA88);

    x += tr_w * 2;
    if (x + tr_w * 2 >= win::W)
    {
      x = 0;
      y += (hn + 3) * ProfilerFont.H;
      hn = 1;
    }
    cnt++;
    tn++;
  }
} /* End of 'tam::profiler::Display' function */

/* Draw one character function.
 * ARGUMENTS:
 *   - frame position:
 *       INT X, Y;
 *   - character code:
 *       CHAR Ch;
 *   - character color:
 *       DWORD Color;
 * RETURNS: None.
 */
VOID tam::profiler::PutChar( INT X, INT Y, CHAR Ch, DWORD Color )
{
  INT i, j;
  BYTE *ptr = ProfilerFont.Data + (BYTE)Ch * ProfilerFont.H;

  for (i = 0; i < ProfilerFont.H; i++)
    for (j = 0; j < ProfilerFont.W; j++)
    {
      INT x0 = X + j, y0 = Y + i;

      if (x0 >= 0 && x0 < win::W && y0 >= 0 && y0 < win::H && 
          (ptr[i] & (0x80 >> j)) != 0)
        FrameBits[y0 * win::W + x0] = Color;
    }
} /* End of 'tam::profiler::PutChar' function */

/* Draw string function.
 * ARGUMENTS:
 *   - frame position:
 *       INT X, Y;
 *   - string pointer:
 *       const CHAR *Str;
 *   - character color:
 *       DWORD Color;
 * RETURNS:
 *  (INT) X coordinate afster last output character.
 */
INT tam::profiler::PutStr( INT X, INT Y, const CHAR *Str, DWORD Color )
{
  INT SaveX = X;

  if (X >= win::W || Y >= win::H)
    return X;

  while (*Str != 0)
    if (*Str == '\n')
    {
      X = SaveX;
      Y += ProfilerFont.H;
      if (Y >= win::H)
        return X;
      Str++;
    }
    else
      PutChar(X, Y, *Str++, Color), X += ProfilerFont.W;
  return X;
} /* End of 'tam::profiler::PutStr' function */

/* Draw filled bar function.
 * ARGUMENTS:
 *   - frame position:
 *       INT X, Y;
 *   - bar size:
 *       INT W, H;
 *   - bar color:
 *       DWORD Color;
 * RETURNS: None.
 */
VOID tam::profiler::PutBar( INT X, INT Y, INT W, INT H, DWORD Color )
{
  // Trivial reject
  if (W <= 0 || H <= 0 || X >= win::W || Y >= win::H)
    return;
  INT X2 = X + W - 1, Y2 = Y + H - 1;
  if (X2 < 0 || Y2 < 0)
    return;

  // Clipping
  if (X < 0)
    X = 0;
  if (Y < 0)
    Y = 0;
  if (X2 >= win::W)
    X2 = win::W - 1;
  if (Y2 >= win::H)
    Y2 = win::H - 1;

  W = X2 - X + 1;
  H = Y2 - Y + 1;

  DWORD addr = Y * win::W + X;
  INT delta = win::W - W;
  while (H-- > 0)
  {
    INT cnt = W;
    while (cnt-- > 0)
      FrameBits[addr++] = Color;
    addr += delta;
  }
} /* End of 'tam::profiler::PutBar' function */

// Global profiler
tam::profiler tam::SystemProfiler;

/* END OF 'tam_profiler.cpp' FILE */
