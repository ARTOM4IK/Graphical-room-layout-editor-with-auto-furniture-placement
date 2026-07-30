/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_int.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX integer slider control declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 26.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ux_control_int_h_
#define __tam_ux_control_int_h_

#include "tam_ux_control.h"

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* ÿnteger slider control class.
     * Usage: ux::int_type DeltaX {Tab1, "Delta X", 0, -30, 30};
     */
    class int_type : public base_type<INT>
    {
    private:
      INT Min; // Slider minimum value
      INT Max; // Slider maximum value

    public:
      /* Constructor with range.
       * ARGUMENTS:
       *   - parent tab reference:
       *       tab &T;
       *   - control display label:
       *       const std::string &L;
       *   - default value:
       *       INT Default;
       *   - minimum value:
       *       INT MinVal;
       *   - maximum value:
       *       ÿ“≈ MaxVal;
       */
      int_type( tab &T, const std::string &L, INT Default, INT MinVal, INT MaxVal ) :
        base_type<INT>(T, L, Default), Min(MinVal), Max(MaxVal)
      {
      } /* End of 'int_type' function */

      /* Assignment operator.
       * ARGUMENTS:
       *   - value to assign:
       *       INT V;
       * RETURNS:
       *   (int_type &) self reference.
       */
      int_type & operator=( INT V )
      {
        Value = V;
        return *this;
      } /* End of 'operator=' function */

      /* Assignment operator.
       * ARGUMENTS:
       *   - value to assign:
       *       const int_type &V;
       * RETURNS:
       *   (int_type &) self reference.
       */
      int_type & operator=( const int_type &V )
      {
        Value = V.Value;
        return *this;
      } /* End of 'operator=' function */

      /* Add-assign operator.
       * ARGUMENTS:
       *   - value to add:
       *       INT V;
       * RETURNS:
       *   (int_type &) self reference.
       */
      int_type & operator+=( INT V )
      {
        Value += V;
        return *this;
      } /* End of 'operator+=' function */

      /* Subtract-assign operator.
       * ARGUMENTS:
       *   - value to subtract:
       *       ÿ“≈ V;
       * RETURNS:
       *   (int_type &) self reference.
       */
      int_type & operator-=( INT V )
      {
        Value -= V;
        return *this;
      } /* End of 'operator-=' function */

      /* Multiply-assign operator.
       * ARGUMENTS:
       *   - value to multiply:
       *       INT V;
       * RETURNS:
       *   (int_type &) self reference.
       */
      int_type & operator*=( INT V )
      {
        Value *= V;
        return *this;
      } /* End of 'operator*=' function */

      /* Divide-assign operator.
       * ARGUMENTS:
       *   - value to divide:
       *       INT V;
       * RETURNS:
       *   (flt_type &) self reference.
       */
      int_type & operator/=( INT V )
      {
        Value /= V;
        return *this;
      } /* End of 'operator/=' function */

      /* Draw integer slider via ImGui function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Draw( VOID ) override;
    }; /* End of 'int_type' class */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_ux_control_int_h_ */

/* END OF 'tam_ux_control_int.h' FILE */
