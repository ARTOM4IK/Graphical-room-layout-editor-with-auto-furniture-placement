/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_int.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX integer slider control implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 26.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux_control_int.h"

/* Draw integer slider via ImGui function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::int_type::Draw( VOID )
{
  ImGui::SliderInt(GetUID().c_str(), &Value, Min, Max);
} /* End of 'tam::ux::int_type::Draw' function */

/* END OF 'tam_ux_control_int.cpp' FILE */
