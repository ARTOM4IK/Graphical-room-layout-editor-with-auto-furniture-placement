/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_bool.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX boolean checkbox control implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 10.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux_control_bool.h"

/* Draw checkbox via ImGui function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::bool_type::Draw( VOID )
{
  bool Val = (Value != FALSE);
  ImGui::Checkbox(GetUID().c_str(), &Val);
  Value = Val ? TRUE : FALSE;

  if (!TrueLabel.empty() || !FalseLabel.empty())
  {
    ImGui::SameLine();
    ImGui::TextDisabled("(%s)", Value ? TrueLabel.c_str() : FalseLabel.c_str());
  }
} /* End of 'tam::ux::bool_type::Draw' function */

/* END OF 'tam_ux_control_bool.cpp' FILE */
