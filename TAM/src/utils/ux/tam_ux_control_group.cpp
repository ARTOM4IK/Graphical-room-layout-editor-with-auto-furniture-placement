/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_group.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX read-only label control implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 02.04.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux_control_group.h"

/* Draw group begin via ImGui function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::group_begin::Draw( VOID )
{
  IsActive = TRUE;
  
  // Temporarily remove from stack while drawing children
  group_manager::PopGroup();
  
  // Draw the tree node
  IsOpen = ImGui::TreeNodeEx(Header.c_str(),
    IsOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
  
  // Draw child controls if open
  if (IsOpen)
  {
    for (auto *Ctrl : ChildControls)
      Ctrl->Draw();
    ImGui::TreePop();
  }
  
  // Push back after drawing
  group_manager::PushGroup(this);
  IsActive = FALSE;
} /* End of 'tam::ux::group_begin::Draw' function */

/* Draw group end via ImGui function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::group_end::Draw( VOID )
{
} /* End of 'tam::ux::group_begin::Draw' function */

/* END OF 'tam_ux_control_group.cpp' FILE */
