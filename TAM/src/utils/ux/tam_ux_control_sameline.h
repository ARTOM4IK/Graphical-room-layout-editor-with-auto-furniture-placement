/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_sameline.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX same line control declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 13.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ux_control_sameline_h_
#define __tam_ux_control_sameline_h_

#include "tam_ux_control.h"

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* Same line with text control class.
     * Usage: ux::sameline_type _1 {Tab1};
     */
    class sameline_type : public base_control
    {
    public:
      /* Constructor.
       * ARGUMENTS:
       *   - parent tab reference:
       *       tab &T;
       */
      sameline_type( tab &T ) : base_control(T)
      {
      } /* End of 'sameline_type' function */

      /* Draw same line via ImGui function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Draw( VOID ) override;
    }; /* End of 'separator_type' class */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_ux_control_sameline_h_ */

/* END OF 'tam_ux_control_sameline.h' FILE */
