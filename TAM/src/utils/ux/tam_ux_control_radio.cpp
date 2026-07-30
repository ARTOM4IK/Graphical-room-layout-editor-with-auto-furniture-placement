/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_radio.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX dropdown radio control implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 03.04.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux_control_radio.h"

/* Draw radio buttons via ImGui function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::radio_type::Draw( VOID )
{
  if (Label != "")
  {
    ImGui::BeginChild(GetUID().c_str(), ImVec2(0, 0), 
        ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY);
    ImGui::TextUnformatted(Label.c_str());
    ImGui::Separator();
  }
  for (INT i = 0; i < (INT)Items.size(); i++)
  {
    BOOL is_selected = (SelectedIndex == i);
    std::string item_id = Items[i] + "##si" + std::to_string(i);

    ImGui::RadioButton(item_id.c_str(), &SelectedIndex, i);
    if (is_selected)
      ImGui::SetItemDefaultFocus();
  }
  if (Label != "")
    ImGui::EndChild();
} /* End of 'tam::ux::radio_type::Draw' function */

/* END OF 'tam_ux_control_radio.cpp' FILE */
