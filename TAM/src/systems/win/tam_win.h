/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_win.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Window class handle module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 01.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_win_h_
#define __tam_win_h_

#include "tam_def.h"
#include "../res/tam_res.h"

/* 'tam' namespace */
namespace tam
{
  /* Window handle class */
  class win
  {
  protected:
    // Class name
    const CHAR *WindowClassName = "CGSG::TAM window class name";

  private:
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
    static LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg,
                                     WPARAM wParam, LPARAM lParam );

    RECT FullScreenSaveRect; // Full screen mode saved rectangle

  public:
    BOOL IsFullScreen;   // Full screen mode flag
    HWND hWnd;           // Window handle
    INT W, H;            // Window size
    HINSTANCE hInstance; // Application handle

    INT MouseWheel;          // Mouse wheel global counter
    BOOL IsActive;           // Active window flag

    /* Class constructor.
     * ARGUMENTS:
     *   - application instance handle:
     *       HINSTANCE hInst;
     */
    win( HINSTANCE hInst = GetModuleHandle(nullptr) );

    /* Class destructor */
    virtual ~win( VOID );

    /* Main window create function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Create( VOID );

    /* Main window handle loop function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Run( VOID );

    /* Application terminate function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID DoExit( VOID );

    /* Toggle full screen mode function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID FlipFullScreen( VOID );

    /* File open dialog handle function.
     * ARGUMENTS:
     *   - animation interface pointer:
     *       anim *Ani;
     *   - dialog title text:
     *       const std::string &Title;
     *   - file mask filter (like: "Text (*.txt)\0*.txt;*.lst\0All files (*.*)\0*.*\0"):
     *       const CHAR *FileFilters;
     * RETURNS:
     *   (std::string) load file path or "" if no file selected.
     */
    std::string OpenFileDialog( const std::string &Title, const CHAR *FileFilters )
    {
      OPENFILENAMEA ofn {};
      CHAR FileName[_MAX_PATH] {};

      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.lpstrTitle = Title.c_str();
      ofn.hwndOwner = hWnd;
      ofn.lpstrFile = FileName;
      ofn.nMaxFile = sizeof(FileName);
      ofn.lpstrFilter = FileFilters;
      ofn.nFilterIndex = 1;
      ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

      if (GetOpenFileNameA(&ofn) == TRUE)
      {
        return ofn.lpstrFile;
      }
      return "";
    } /* End of 'OpenFileDialog' function */

    /* File save dialog handle function.
     * ARGUMENTS:
     *   - animation interface pointer:
     *       anim *Ani;
     *   - dialog title text:
     *       const std::string &Title;
     *   - file mask filter (like: "Text (*.txt)\0*.txt;*.lst\0All files (*.*)\0*.*\0"):
     *       const CHAR *FileFilters;
     *   - old file name:
     *       const std::string &OldFileName;
     * RETURNS:
     *   (std::string) load file path or "" if no file selected.
     */
    std::string SaveFileDialog( const std::string &Title, const CHAR *FileFilters, const std::string &OldFileName = "" )
    {
      OPENFILENAMEA ofn {};
      CHAR FileName[_MAX_PATH] {};

      strncpy(FileName, OldFileName.c_str(), _MAX_PATH - 1);
      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.lpstrTitle = Title.c_str();
      ofn.hwndOwner = hWnd;
      ofn.lpstrFile = FileName;
      ofn.nMaxFile = sizeof(FileName);
      ofn.lpstrFilter = FileFilters;
      ofn.nFilterIndex = 1;
      ofn.Flags = OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

      if (GetSaveFileNameA(&ofn) == TRUE)
      {
        return ofn.lpstrFile;
      }
      return "";
    } /* End of 'SaveFileDialog' function */


  protected:

    ///
    /// Cracked message handle functions
    ///

    /* WM_CREATE window message handle function.
     * ARGUMENTS:
     *   - structure with creation data:
     *       CREATESTRUCT *CS;
     * RETURNS:
     *   (BOOL) TRUE to continue creation window, FALSE to terminate.
     */
    virtual BOOL OnCreate( CREATESTRUCT *CS );

    /* WM_DESTROY window message handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID OnDestroy( VOID );

    /* WM_SIZE window message handle function.
     * ARGUMENTS:
     *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
     *       UINT State;
     *   - new width and height of client area:
     *       INT W, H;
     * RETURNS: None.
     */
    virtual VOID OnSize( UINT State, INT W, INT H );

    /* WM_ERASEBKGND window message handle function.
     * ARGUMENTS:
     *   - device context of client area:
     *       HDC hDC;
     * RETURNS:
     *   (BOOL) TRUE if background is erased, FALSE otherwise.
     */
    virtual BOOL OnEraseBkgnd( HDC hDC );

    /* WM_PAINT window message handle function.
     * ARGUMENTS:
     *   - window device context:
     *       HDC hDC;
     *   - paint message structure pointer:
     *       PAINTSTRUCT *PS;
     * RETURNS: None.
     */
    virtual VOID OnPaint( HDC hDC, PAINTSTRUCT *PS );

    /* WM_ACTIVATE window message handle function.
     * ARGUMENTS:
     *   - reason (WA_CLICKACTIVE, WA_ACTIVE or WA_INACTIVE):
     *       UINT Reason;
     *   - handle of active window:
     *       HWND hWndActDeact;
     *   - minimized flag:
     *       BOOL IsMinimized;
     * RETURNS: None.
     */
    virtual VOID OnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized );

    /* WM_TIMER window message handle function.
     * ARGUMENTS:
     *   - specified the timer identifier.
     *       INT Id;
     * RETURNS: None.
     */
    virtual VOID OnTimer( INT Id );

    /* WM_DROPFILES window message handle function.
     * ARGUMENTS:
     *   - handle to an internal structure describing the dropped files:
     *       HDROP hDrop;
     * RETURNS: None.
     */
    virtual VOID OnDropFiles( HDROP hDrop );

    /* WM_NOTIFY window message handle function.
     * ARGUMENTS:
     *   - control identifier:
     *       INT Id;
     *   - notification header depended to control type:
     *       NMHDR *NoteHead;
     * RETURNS:
     *   (LRESULT) return value depended to notification.
     */
    virtual LRESULT OnNotify( INT Id, NMHDR* NoteHead );

    /* WM_COMMAND window message handle function.
     * ARGUMENTS:
     *   - item (menu or accelerator) or control window identifier:
     *       INT Id;
     *   - control window handle:
     *       HWND hWndCtl;
     *   - notification: 0 - menu, 1 - accelerator, otherwise -
     *     depended to control (like BM_CLICKED):
     *       UINT CodeNotify;
     * RETURNS: None.
     */
    virtual VOID OnCommand( INT Id, HWND hWndCtl, UINT CodeNotify );

    /* WM_DRAWITEM window message handle function.
     * ARGUMENTS:
     *   - control identifier (for menu - 0):
     *       INT Id;
     *   - draw item information structure pointer:
     *       DRAWITEMSTRUCT *DrawItem;
     * RETURNS: None.
     */
    virtual VOID OnDrawItem( INT Id, DRAWITEMSTRUCT* DrawItem );

    /* WM_HSCROLL message handle function.
     * ARGUMENTS:
     *   - handle of scroll window or nullptr for window scroll bars:
     *       HWND hWndCtl;
     *   - scrool bar request code (see SB_***):
     *       UINT Code;
     *   - schroll position for 'Code' is SB_THUMBPOSITION or SB_THUMBTRACK:
     *       INT Pos;
     * RETURNS: None.
     */
    virtual VOID OnHScroll( HWND hWndCtl, UINT Code, INT Pos );

    /* WM_VSCROLL message handle function.
     * ARGUMENTS:
     *   - handle of scroll window or nullptr for window scroll bars:
     *       HWND hWndCtl;
     *   - scrool bar request code (see SB_***):
     *       UINT Code;
     *   - schroll position for 'Code' is SB_THUMBPOSITION or SB_THUMBTRACK:
     *       INT Pos;
     * RETURNS: None.
     */
    virtual VOID OnVScroll( HWND hWndCtl, UINT Code, INT Pos );

    /* WM_MENUSELECT window message handle function.
     * ARGUMENTS:
     *   - handle of menu:
     *       HMENU hMenu;
     *   - submenu item ID or submenu index
     *       INT Item;
     *   - handle of popup menu if selected popup submenu:
     *       HMENU hMenuPopup;
     *   - menu item flags (see MF_***):
     *       UINT Flags;
     * RETURNS: None.
     */
    virtual VOID OnMenuSelect( HMENU hMenu, INT Item, HMENU hMenuPopup, UINT Flags );

    /* WM_INITMENUPOPUP window message handle function.
     * ARGUMENTS:
     *   - handle of popup menu:
     *       HMENU hMenu;
     *   - submenu item position:
     *       UINT Item;
     *   - window system menu flag:
     *       BOOL IsSystemMenu;
     * RETURNS: None.
     */
    virtual VOID OnInitMenuPopup( HMENU hMenu, UINT Item, BOOL IsSystemMenu );

    /* WM_EXITSIZEMOVE window message handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID OnExitSizeMove( VOID );

    /* WM_ENTERSIZEMOVE window message handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID OnEnterSizeMove( VOID );

    /* WM_MOUSEWHEEL window message handle function.
     * ARGUMENTS:
     *   - mouse window position:
     *       INT X, Y;
     *   - mouse wheel delta value (120 points per rotate):
     *       INT Z;
     *   - mouse keys bits (see MK_*** bits constants):
     *       UINT Keys;
     * RETURNS: None.
     */
    virtual VOID OnMouseWheel( INT X, INT Y, INT Z, UINT Keys );

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
    virtual VOID OnKey( UINT Key, BOOL IsPress, INT RepeatCount, UINT ShiftKeysFlags );

    /* WM_*BUTTONDOWN window message handle function.
     * ARGUMENTS:
     *   - double click flag:
     *       BOOL IsDoubleClick;
     *   - mouse window position:
     *       INT X, Y;
     *   - mouse keys bits (see MK_*** bits constants):
     *       UINT Keys;
     * RETURNS: None.
     */
    virtual VOID OnButtonDown( BOOL IsDoubleClick, INT X, INT Y, UINT Keys );

    /* WM_*BUTTONUP window message handle function.
     * ARGUMENTS:
     *   - mouse window position:
     *       INT X, Y;
     *   - mouse keys bits (see MK_*** bits constants):
     *       UINT Keys;
     * RETURNS: None.
     */
    virtual VOID OnButtonUp( INT X, INT Y, UINT Keys );

    /* WM_MOUSEMOVE window message handle function.
     * ARGUMENTS:
     *   - mouse window position:
     *       INT X, Y;
     *   - mouse keys bits (see MK_*** bits constants):
     *       UINT Keys;
     * RETURNS: None.
     */
    virtual VOID OnMouseMove( INT X, INT Y, UINT Keys );

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
    virtual LRESULT OnMessage( UINT Msg, WPARAM wParam, LPARAM lParam );
  }; /* End of 'win' class */
} /* end of 'tam' namespace */

#endif /* __tam_win_h_ */

/* END OF 'tam_win.h' FILE */
