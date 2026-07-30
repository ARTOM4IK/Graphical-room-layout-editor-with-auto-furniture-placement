/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ui.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               user interface handle declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 20.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ui_h_
#define __tam_ui_h_

#include "tam_def.h"

/* ImGui handle */
#include "utils/imgui/imgui.h"
#include "utils/imgui/imgui_internal.h"
#include "utils/imgui/imgui_impl_win32.h"
#include "utils/imgui/imgui_impl_vulkan.h"
#include "utils/imgui/imgui_spectrum.h"

/* 'tam' namespace */
namespace tam
{
  /* Forward declaration of render class */
  class render;

  /* user interface handle class */
  class ui_manager
  {
    std::vector<BOOL> Buttons {};  // UI buttons
    std::vector<FLT> Sliders {};   // UI sliders

    render &Rnd;                   // Reference to render
  public:

    /* Class constructor.
     * ARGUMENTS:
     *   - render reference:
     *       render &RndRef;
     */
    tam::ui_manager( render &RndRef ) : Rnd(RndRef)
    {
    } /* End of 'ui_manager' function */

    /* Create user interface dependencies function.
     * ARGUMENTS: 
     *   - window context:
     *       HWND &hWnd.
     * RETURNS: None.
     */
    VOID CreateUI( HWND &hWnd );

    /* Class destructor */
    ~ui_manager( VOID )
    {
    } /* End of '~ui_manager' function */

    /* Response input keys for user interface function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID ResponseUI( VOID );
      
    /* Render user interface in target function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID RenderUI( VOID );

    /* Draw user interface on GPU function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID DrawUI( VOID );

    /* Close user interface handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID CloseUI( VOID );

  }; /* End of 'ui_manager' class */
} /* end of 'tam' namespace */

#endif /* __tam_ui_h_ */

/* END OF 'tam_ui.h' FILE */
