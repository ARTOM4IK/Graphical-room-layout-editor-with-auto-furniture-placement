/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_string.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX string input control implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 10.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux_control_string.h"

/* Draw text input via ImGui function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::string_type::Draw( VOID )
{
  ImGui::InputText(GetUID().c_str(), Buf, sizeof(Buf));
  IsActiveFlag = ImGui::IsItemActive();
} /* End of 'tam::ux::string_type::Draw' function */

/* END OF 'tam_ux_control_string.cpp' FILE */
