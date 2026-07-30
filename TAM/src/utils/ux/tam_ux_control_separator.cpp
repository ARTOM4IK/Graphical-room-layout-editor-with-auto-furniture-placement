/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_separator.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX separator control implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 10.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux_control_separator.h"

/* Draw separator via ImGui function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::separator_type::Draw( VOID )
{
  ImGui::SeparatorText(Label.c_str());
} /* End of 'tam::ux::separator_type::Draw' function */

/* END OF 'tam_ux_control_separator.cpp' FILE */
