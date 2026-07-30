/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_user.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX static text control implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 24.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux_control_user.h"

/* Draw static text via ImGui function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::user_type::Draw( VOID )
{
  if (UserRenderFunc != nullptr)
    UserRenderFunc();
} /* End of 'tam::ux::user_type::Draw' function */

/* END OF 'tam_ux_control_user.cpp' FILE */

