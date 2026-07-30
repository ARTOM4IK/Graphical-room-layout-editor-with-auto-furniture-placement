/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_color.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX color picker control declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 17.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ux_control_color_h_
#define __tam_ux_control_color_h_

#include "tam_ux_control.h"

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* Color picker control class.
     * Usage: ux::color_type Col {Tab1, "Color", {0.8, 0.5, 0.3}};
     */
    class color_type : public base_control
    {
    private:
      FLT Color[3]; // RGB color components

    public:
      /* Constructor with initial color.
       * ARGUMENTS:
       *   - parent tab reference:
       *       tab &T;
       *   - control display label:
       *       const std::string &L;
       *   - initial color as vec3:
       *       const vec3 &C;
       */
      color_type( tab &T, const std::string &L, const vec3 &C = {0.30, 0.47, 0.8} ) :
        base_control(T, L)
      {
        Color[0] = C.X;
        Color[1] = C.Y;
        Color[2] = C.Z;
      } /* End of 'color_type' function */

      /* Implicit conversion to vec3 operator.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) current color.
       */
      operator vec3( VOID ) const
      {
        return vec3(Color[0], Color[1], Color[2]);
      } /* End of 'operator vec3' function */

      /* Assignment from vec3 operator.
       * ARGUMENTS:
       *   - color to assign:
       *       const vec3 &C;
       * RETURNS:
       *   (color_type &) self reference.
       */
      color_type & operator=( const vec3 &C )
      {
        Color[0] = C.X;
        Color[1] = C.Y;
        Color[2] = C.Z;
        return *this;
      } /* End of 'operator=' function */

      /* Get color component by index function.
       * ARGUMENTS:
       *   - component index (0=R, 1=G, 2=B):
       *       INT I;
       * RETURNS:
       *   (FLT) color component value.
       */
      FLT operator[]( INT I ) const
      {
        return Color[I];
      } /* End of 'operator[]' function */

      /* Draw color picker via ImGui function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Draw( VOID ) override;
    }; /* End of 'color_type' class */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_ux_control_color_h_ */

/* END OF 'tam_ux_control_color.h' FILE */
