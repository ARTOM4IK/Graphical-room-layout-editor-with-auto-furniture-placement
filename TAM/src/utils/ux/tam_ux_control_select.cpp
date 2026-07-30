/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_select.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX dropdown select control implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 10.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux_control_select.h"

/* Draw dropdown via ImGui function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::select_type::Draw( VOID )
{
  const CHAR *preview =
    (SelectedIndex >= 0 && SelectedIndex < (INT)Items.size()) ?
    Items[SelectedIndex].c_str() : "";

  if (ImGui::BeginCombo(GetUID().c_str(), preview))
  {
    for (INT i = 0; i < (INT)Items.size(); i++)
    {
      BOOL is_selected = (SelectedIndex == i);
      std::string item_id = Items[i] + "##si" + std::to_string(i);
      if (ImGui::Selectable(item_id.c_str(), is_selected))
        SelectedIndex = i;
      if (is_selected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
} /* End of 'tam::ux::select_type::Draw' function */

/* END OF 'tam_ux_control_select.cpp' FILE */
