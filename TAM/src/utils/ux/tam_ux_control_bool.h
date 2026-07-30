/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_bool.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX boolean checkbox control declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 10.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ux_control_bool_h_
#define __tam_ux_control_bool_h_

#include "tam_ux_control.h"

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* Boolean checkbox control class.
     * Usage: ux::bool_type IsTest {Tab1, "Test", TRUE, "Off", "On"};
     */
    class bool_type : public base_type<BOOL>
    {
    private:
      std::string FalseLabel; // Label displayed when value is FALSE
      std::string TrueLabel;  // Label displayed when value is TRUE

    public:
      /* Constructor with labels.
       * ARGUMENTS:
       *   - parent tab reference:
       *       tab &T;
       *   - control display label:
       *       const std::string &L;
       *   - default value:
       *       BOOL Default;
       *   - label for FALSE state:
       *       const std::string &FLabel;
       *   - label for TRUE state:
       *       const std::string &TLabel;
       */
      bool_type( tab &T, const std::string &L, BOOL Default,
        const std::string &FLabel = "", const std::string &TLabel = "" ) :
        base_type<BOOL>(T, L, Default), FalseLabel(FLabel), TrueLabel(TLabel)
      {
      } /* End of 'bool_type' function */

      /* Assignment operator.
       * ARGUMENTS:
       *   - value to assign:
       *       BOOL V;
       * RETURNS:
       *   (bool_type &) self reference.
       */
      bool_type & operator=( BOOL V )
      {
        Value = V;
        return *this;
      } /* End of 'operator=' function */

      /* Draw checkbox via ImGui function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Draw( VOID ) override;
    }; /* End of 'bool_type' class */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_ux_control_bool_h_ */

/* END OF 'tam_ux_control_bool.h' FILE */
