/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_sameline.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX same line control implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 13.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux_control_sameline.h"

/* Draw same line via ImGui function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::sameline_type::Draw( VOID )
{
  ImGui::SameLine();
} /* End of 'tam::ux::sameline_type::Draw' function */

/* END OF 'tam_ux_control_sameline.cpp' FILE */
