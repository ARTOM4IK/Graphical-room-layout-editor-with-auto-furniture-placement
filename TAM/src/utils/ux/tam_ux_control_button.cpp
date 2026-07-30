/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_button.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX one-shot button control implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 10.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux_control_button.h"

/* Draw button via ImGui function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::button::Draw( VOID )
{
  if (ImGui::Button(GetUID().c_str()))
    Value = TRUE;
} /* End of 'tam::ux::button::Draw' function */

/* END OF 'tam_ux_control_button.cpp' FILE */
