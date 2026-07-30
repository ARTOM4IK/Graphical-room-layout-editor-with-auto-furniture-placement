/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_anim.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Animation declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 09.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_anim_h_
#define __tam_anim_h_

#include "tam_def.h"
#include "systems/win/tam_win.h"
#include "input/tam_input.h"
#include "input/tam_timer.h"
#include "render/tam_render.h"
#include "tam_units.h"
#include "utils/imgui/tam_ui.h"
#include "utils/ux/tam_ux.h"
#include "systems/flat/tam_flat.h"

/* 'tam' namespace */
namespace tam
{
  /* 'anim' handle class */
  class anim : public win, public input, public timer, public render, public ux::core
  {
  private:
    std::atomic_bool
      IsActive {},       // flag if window is active (not hidden)
      IsRenderActive {}; // flag if frame was started
  public:

    /* Class default constructor */
    anim( HINSTANCE hInst = GetModuleHandle(nullptr) ) : win(hInst), input(win::hWnd, win::MouseWheel),
      render(static_cast<timer &>(*this), static_cast<input &>(*this))
    {
    } /* End of 'anim' function */

    inline static std::map<std::string, unit * (*)( anim *Ani )> UnitNames {}; // registered unit names storage
    scene CurrentScene {};                                                     // current set of units

    /* Unit register representation class */
    template<typename unit_type>
      class unit_register
      {
      private:
        /* Create new unit function.
         * ARGUMENTS:
         *   - pointer to animation context:
         *       anim *Ani;
         * RETURNS:
         *   (*unit) new unit.
         */
        static unit * Create( anim *Ani )
        {
          return new unit_type(Ani);
        } /* End of 'Create' function */
      public:
        /* Type constructor.
         * ARGUMENTS:
         *   - unit-to-register name:
         *       const std::string &UnitName;
         */
        unit_register( const std::string &UnitName )
        {
          UnitNames[UnitName] = Create;
        } /* End of 'unit_register' function */
      }; /* End of unit_register class template */

    /* Add unit into current scene function.
     * ARGUMENTS:
     *   - pointer to a new unit:
     *       unit *Uni;
     * RETURNS:
     *   (anim &) reference to itself.
     */
    anim & operator<<( unit *Uni );

    /* Add unit into animation by name function.
     * ARGUMENTS:
     *   - unit to add name:
     *       const std::string &UnitName;
     * RETURNS:
     *   (anim &) reference to itself.
     */
    anim & operator<<( const std::string &UnitName );

    /* Get registered units names list function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (std::vector<std::string>) registered unit names list.
     */
    std::vector<std::string> GetRegUnitNamesList( VOID )
    {
      std::vector<std::string> names;

      for (auto &ru : UnitNames)
        names.push_back(ru.first);
      return names;
    } /* End of 'GetUnitsCount' function */

    /* Class destructor */
    ~anim( VOID )
    {
    } /* End of '~anim' function */

    /* Render animtaion function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Render( VOID );

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

    /* WM_TIMER window message handle function.
     * ARGUMENTS:
     *   - specified the timer identifier.
     *       INT Id;
     * RETURNS: None.
     */
    VOID OnTimer( INT Id ) override;

    /* WM_EXITSIZEMOVE window message handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID OnExitSizeMove( VOID ) override;

    /* WM_ENTERSIZEMOVE window message handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID OnEnterSizeMove( VOID ) override;

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
    VOID OnMouseWheel( INT X, INT Y, INT Z, UINT Keys ) override;

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
    VOID OnButtonDown( BOOL IsDoubleClick, INT X, INT Y, UINT Keys ) override;

    /* WM_*BUTTONUP window message handle function.
     * ARGUMENTS:
     *   - mouse window position:
     *       INT X, Y;
     *   - mouse keys bits (see MK_*** bits constants):
     *       UINT Keys;
     * RETURNS: None.
     */
    VOID OnButtonUp( INT X, INT Y, UINT Keys ) override;

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

    /* Main window handle loop function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Run( VOID ) override;
  }; /* End of 'anim' class */
} /* end of 'tam' namespace */

#endif /* __tam_anim_h_ */

/* END OF 'tam_anim.h' FILE */
