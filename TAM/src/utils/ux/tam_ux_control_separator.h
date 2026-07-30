/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_separator.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX separator control declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 10.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ux_control_separator_h_
#define __tam_ux_control_separator_h_

#include "tam_ux_control.h"

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* Separator with text control class.
     * Usage: ux::separator_type _1 {Tab1, "Main"};
     */
    class separator_type : public base_control
    {
    public:
      /* Constructor.
       * ARGUMENTS:
       *   - parent tab reference:
       *       tab &T;
       *   - separator text:
       *       const std::string &Text;
       */
      separator_type( tab &T, const std::string &Text ) : base_control(T, Text)
      {
      } /* End of 'separator_type' function */

      /* Draw separator via ImGui function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Draw( VOID ) override;
    }; /* End of 'separator_type' class */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_ux_control_separator_h_ */

/* END OF 'tam_ux_control_separator.h' FILE */
