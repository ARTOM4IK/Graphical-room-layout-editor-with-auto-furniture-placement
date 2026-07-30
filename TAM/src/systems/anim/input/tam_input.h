/***************************************************************
 * Copyright (C) 2026
 4
 4*    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_input.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Input declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 08.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_input_h_
#define __tam_input_h_

#include "tam_def.h"
#include "tam_timer.h"
#include "tam_keyboard.h"
#include "tam_joystick.h"
#include "tam_mouse.h"

/* 'tam' namespace */
namespace tam
{
  /* 'input' handle class */
  class input : public keyboard, public mouse, public joystick
  {
  public:
    /* Input class constructor.
     * ARGUMENTS:
     *    - window handle reference:
     *        HWND &hWndRef;
     *    - mouse wheel reference:
     *        INT &WheelRef;
     */
    input( HWND &hWndRef, INT &WheelRef ) : mouse(hWndRef, WheelRef), keyboard(), joystick()
    {
    } /* End of 'input' function */

    /* Class destructor */
    ~input( VOID )
    {
    } /* End of '~input' function */

    /* Reset timer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Reset( VOID )
    {
      mouse::Reset();
      keyboard::Reset();
      joystick::Reset();
    } /* End of 'Reset' function */

    /* Response timer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      static BOOL NeedReset = FALSE;

      if (ImGui::GetCurrentContext() != nullptr)
      {
        ImGuiIO &io = ImGui::GetIO();
      
        if (!io.WantCaptureKeyboard && !io.WantCaptureMouse)
        {
          if (NeedReset)
          {
            NeedReset = FALSE;
            keyboard::Reset();
            mouse::Reset();
          }
          keyboard::Response();
          mouse::Response();
        }
        else
          NeedReset = TRUE;
      }
      else
      {
        keyboard::Response();
        mouse::Response();
      }
      joystick::Response();
      // mouse::Response();
      // keyboard::Response();
      // joystick::Response();
    } /* End of 'Response' function */
  }; /* End of 'input' class */
} /* end of 'tam' namespace */

#endif /* __tam_input_h_ */

/* END OF 'tam_input.h' FILE */
