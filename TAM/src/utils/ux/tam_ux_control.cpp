/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX control base classes implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 02.04.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux_tab.h"
//#include "tam_ux_control.h"

/* Constructor with tab and label.
 * ARGUMENTS:
 *   - parent tab reference:
 *       tab &T;
 *   - control display label:
 *       const std::string &L;
 */
tam::ux::base_control::base_control( tab &T, const std::string &L ) :
  Tab(T), Label(L), ControlUID(NextControlUID++)
{
  // Auto-add to current group if exists
  group_begin *CurrentGroup = group_manager::CurrentGroup();
  if (CurrentGroup != nullptr)
    CurrentGroup->AddChild(this);
  else
    Tab.RegisterControl(this);
} /* End of 'tam::ux::base_control' function */

/* END OF 'tam_ux_control.cpp' FILE */
