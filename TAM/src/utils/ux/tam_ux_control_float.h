/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_float.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX float slider control declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 12.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ux_control_float_h_
#define __tam_ux_control_float_h_

#include "tam_ux_control.h"

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* Float slider control class.
     * Usage: ux::flt_type DeltaX {Tab1, "Delta X", 0, -30, 30};
     */
    class flt_type : public base_type<FLT>
    {
    private:
      FLT Min; // Slider minimum value
      FLT Max; // Slider maximum value

    public:
      /* Constructor with range.
       * ARGUMENTS:
       *   - parent tab reference:
       *       tab &T;
       *   - control display label:
       *       const std::string &L;
       *   - default value:
       *       FLT Default;
       *   - minimum value:
       *       FLT MinVal;
       *   - maximum value:
       *       FLT MaxVal;
       */
      flt_type( tab &T, const std::string &L, FLT Default, FLT MinVal, FLT MaxVal ) :
        base_type<FLT>(T, L, Default), Min(MinVal), Max(MaxVal)
      {
      } /* End of 'flt_type' function */

      /* Assignment operator.
       * ARGUMENTS:
       *   - value to assign:
       *       FLT V;
       * RETURNS:
       *   (flt_type &) self reference.
       */
      flt_type & operator=( FLT V )
      {
        Value = V;
        return *this;
      } /* End of 'operator=' function */

      /* Assignment operator.
       * ARGUMENTS:
       *   - value to assign:
       *       const flt_type &V;
       * RETURNS:
       *   (flt_type &) self reference.
       */
      flt_type & operator=( const flt_type &V )
      {
        Value = V.Value;
        return *this;
      } /* End of 'operator=' function */

      /* Add-assign operator.
       * ARGUMENTS:
       *   - value to add:
       *       FLT V;
       * RETURNS:
       *   (flt_type &) self reference.
       */
      flt_type & operator+=( FLT V )
      {
        Value += V;
        return *this;
      } /* End of 'operator+=' function */

      /* Subtract-assign operator.
       * ARGUMENTS:
       *   - value to subtract:
       *       FLT V;
       * RETURNS:
       *   (flt_type &) self reference.
       */
      flt_type & operator-=( FLT V )
      {
        Value -= V;
        return *this;
      } /* End of 'operator-=' function */

      /* Multiply-assign operator.
       * ARGUMENTS:
       *   - value to multiply:
       *       FLT V;
       * RETURNS:
       *   (flt_type &) self reference.
       */
      flt_type & operator*=( FLT V )
      {
        Value *= V;
        return *this;
      } /* End of 'operator*=' function */

      /* Divide-assign operator.
       * ARGUMENTS:
       *   - value to divide:
       *       FLT V;
       * RETURNS:
       *   (flt_type &) self reference.
       */
      flt_type & operator/=( FLT V )
      {
        Value /= V;
        return *this;
      } /* End of 'operator/=' function */

      /* Draw float slider via ImGui function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Draw( VOID ) override;
    }; /* End of 'flt_type' class */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_ux_control_float_h_ */

/* END OF 'tam_ux_control_float.h' FILE */
