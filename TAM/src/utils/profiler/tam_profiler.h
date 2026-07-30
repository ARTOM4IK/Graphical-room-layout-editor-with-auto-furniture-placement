/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_profiler.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               System profiler declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 26.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_profiler_h_
#define __tam_profiler_h_

#include <array>
#include <map>
#include <mutex>
#include <thread>

#include <intrin.h>

#include <dxgi1_4.h>
#pragma comment(lib, "dxgi")

#include "systems/win/tam_win.h"

/* Project namespace */
namespace tam
{
  /* Profiler class */
  class profiler : public win
  {
  private:
    // Stamps and counters maximum counts
    static const INT MaxStamps = 47, MaxCounters = 47;
    static UINT64
      StartTime,    // Start profiler time
      TimePerSec;   // Timer resolution

    // Profiler display mutex
    std::recursive_mutex profiler_mutex_display;

    // Frame zoom 
    DBL FrameZoom = 1.0;

    // Render frame size
    INT RenderFrameW {}, RenderFrameH {};

    // DirectX12 DXGI adaptor
    IDXGIAdapter3 *DXGIAdapter {};

    /* CPU name obtain function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (std::string) CPU name.
     */
    inline static std::string GetCPUBrand( VOID )
    {
      INT cpuInfo[4] = {-1};
      CHAR brand[0x40] = {0};
      __cpuid(cpuInfo, 0x80000000);
      UINT nExIds = cpuInfo[0];
      for (UINT i = 0x80000000; i <= nExIds; ++i)
      {
        __cpuid(cpuInfo, i);
        if (i == 0x80000002)
          memcpy(brand, cpuInfo, sizeof(cpuInfo));
        else if (i == 0x80000003)
          memcpy(brand + 16, cpuInfo, sizeof(cpuInfo));
        else if (i == 0x80000004)
          memcpy(brand + 48, cpuInfo, sizeof(cpuInfo));
      }
      // std::cout << brand << std::endl;  // Выводит название CPU
      return brand;
    } /* End of 'GetCPUBrand' function */


  public:
    // FPS value
    DBL FPS;

    // Store device GUID/LUID
    UCHAR DeviceLUID[8] {};
    // Store device name
    CHAR DeviceName[256] {};
    // Store CPU name
    std::string CPUName = GetCPUBrand();
    // Installed GPU memory size in bytes
    UINT64 TotalGPUMemory {};

    /* Profile info collect data type */
    struct PROFILER_STAMP
    {
      const CHAR *Name = nullptr;  // Profile stamp name
      DBL Average = 0;             // Profile last average value
      INT Count = 0;               // Profile average stored counter
      DBL Accum = 0;               // Profile delta time stamp accumulator
      DBL LastMeasure = 0;         // Last average measure time stamp
      DBL LastStamp = 0;           // Last time stamp value
    }; /* End of 'PROFILER_STAMP' structure */

    /* Profile tread info data type */
    struct PROFILER_THREAD
    {
      // Profile name
      const CHAR *Name = "";

      // Last profile time stamp value
      DBL LastTimeStamp = 0;

      // Last profile average time stamp
      DBL LastAverageTime = 0;

      // Profile registered timestamps values array
      std::array<PROFILER_STAMP, MaxStamps> Stamps;

      // Profile registered counters values array
      std::array<PROFILER_STAMP, MaxCounters> Counters;

      // Profile per frame updated stamps
      INT StampsCount = 0;

      // Profile per frame filled stamps
      INT StampsFilled = 0;

      // Profile per frame updated stamps
      INT CountersCount = 0;

      // Profile per frame filled stamps
      INT CountersFilled = 0;

      // Profile visibility flag
      BOOL IsVisible = FALSE;

      // Profile collection flag
      BOOL IsCollection = TRUE;

      // Thread wainting flag
      BOOL IsWait = FALSE;
    }; /* End of 'PROFILER_THREAD' structure */

    /* Threads profiles stores */
    std::map<std::thread::id, PROFILER_THREAD> Threads;

    /* Class constructor */
    profiler( VOID );

    /* Class destructor */
    ~profiler( VOID )
    {
      if (DXGIAdapter != nullptr)
        DXGIAdapter->Release(), DXGIAdapter = nullptr;
    } /* End of '~profiler' function */

    /* Profiling total init function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Init( VOID );

    /* Profiling restart function.
     * ARGUMENTS:
     *   - current frame size:
     *       INT W, H;
     * RETURNS: None.
     */
    VOID Reset( INT W = 0, INT H = 0 );

    /* Collect profiling timestamp information function.
     * ARGUMENTS:
     *   - profile sample name:
     *       const CHAR *ProfStampName;
     * RETURNS: None.
     */
    VOID Collect( const CHAR *ProfStampName );

    /* Collect profiling counter information function.
     * ARGUMENTS:
     *   - profile counter name:
     *       const CHAR *ProfStampName;
     *   - counter value:
     *       DBL CounterValue;
     * RETURNS: None.
     */
    VOID CollectCounter( const CHAR *ProfCounterName, DBL CounterValue );

    /* Set current profile name function.
     * ARGUMENTS:
     *   - profile name:
     *       const CHAR *ProfName;
     * RETURNS: None.
     */
    VOID SetName( const CHAR *ProfName );

    /* Set current profile wait flag function.
     * ARGUMENTS:
     *   - wait flag:
     *       BOOL IsWait;
     * RETURNS: None.
     */
    VOID SetWaitFlag( BOOL IsWait );

    /* Draw profiling information function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Display( VOID );

  private:

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
    VOID PutBar( INT X, INT Y, INT W, INT H, DWORD Color );

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
    VOID PutChar( INT X, INT Y, CHAR Ch, DWORD Color );

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
    INT PutStr( INT X, INT Y, const CHAR *Str, DWORD Color );

  private:
    // Profile window class name
    const CHAR *ClassName = "TAM Profile Window Class Name";

    /***
     * Double buffering data
     ***/

    // Memory device context
    HDC hDCFrame = nullptr;

    // Frame buffer bitmap
    HBITMAP hBmFrame = nullptr;
    // Frame buffer pixel color data
    DWORD *FrameBits = nullptr;

    /***
     * Cracked message handle functions
     ***/

    /* WM_CREATE window message handle function.
     * ARGUMENTS:
     *   - structure with creation data:
     *       CREATESTRUCT *CS;
     * RETURNS:
     *   (BOOL) TRUE to continue creation window, FALSE to terminate.
     */
    BOOL OnCreate( CREATESTRUCT *CS ) override;

    /* WM_DESTROY window message handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID OnDestroy( VOID ) override;

    /* WM_SIZE window message handle function.
     * ARGUMENTS:
     *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
     *       UINT State;
     *   - new width and height of client area:
     *       INT W, H;
     * RETURNS: None.
     */
    VOID OnSize( UINT State, INT W, INT H ) override;

    /* WM_TIMER window message handle function.
     * ARGUMENTS:
     *   - specified the timer identifier.
     *       INT Id;
     * RETURNS: None.
     */
    VOID OnTimer( INT Id ) override;

    /* WM_ERASEBKGND window message handle function.
     * ARGUMENTS:
     *   - device context of client area:
     *       HDC hDC;
     * RETURNS:
     *   (BOOL) TRUE if background is erased, FALSE otherwise.
     */
    BOOL OnEraseBkgnd( HDC hDC ) override;

    /* WM_PAINT window message handle function.
     * ARGUMENTS:
     *   - window device context:
     *       HDC hDC;
     *   - paint message structure pointer:
     *       PAINTSTRUCT *PS;
     * RETURNS: None.
     */
    VOID OnPaint( HDC hDC, PAINTSTRUCT *PS ) override;

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
    VOID OnKey( UINT Key, BOOL IsPress, INT RepeatCount, UINT ShiftKeysFlags ) override;

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
    LRESULT OnMessage( UINT Msg, WPARAM wParam, LPARAM lParam ) override;
  }; /* End of 'profiler' class */

  // Global profiler
  extern profiler SystemProfiler;
} /* end of 'tam' namespace */

#endif /* __tam_profiler_h_ */

/* END OF 'tam_profiler.h' FILE */
