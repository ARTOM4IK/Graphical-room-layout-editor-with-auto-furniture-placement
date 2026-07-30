/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_button.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX one-shot button control declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 10.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ux_control_button_h_
#define __tam_ux_control_button_h_

#include "tam_ux_control.h"

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* One-shot button control class.
     * Returns TRUE only on the frame when clicked, resets next frame.
     * Usage: ux::button DoEval {Tab1, "Do evaluation"};
     */
    class button : public base_type<BOOL>
    {
    public:
      /* Constructor.
       * ARGUMENTS:
       *   - parent tab reference:
       *       tab &T;
       *   - button display label:
       *       const std::string &L;
       */
      button( tab &T, const std::string &L ) : base_type<BOOL>(T, L, FALSE)
      {
      } /* End of 'button' function */

      /* Reset button state for new frame function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID ResetFrame( VOID ) override
      {
        Value = FALSE;
      } /* End of 'ResetFrame' function */

      /* Draw button via ImGui function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Draw( VOID ) override;
    }; /* End of 'button' class */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_ux_control_button_h_ */

/* END OF 'tam_ux_control_button.h' FILE */
