/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_menu.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX core menu bar and modal dialogs implementation.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 20.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux.h"

/* Draw main application menu bar function.
 * ARGUMENTS:
 *   - output menu bar height:
 *       FLT &MenuBarHeight;
 * RETURNS: None.
 */
VOID tam::ux::core::DrawMainMenu( FLT &MenuBarHeight )
{
  MenuBarHeight = 0;

  if (ImGui::BeginMainMenuBar())
  {
    ImGuiStyle &style = ImGui::GetStyle();

    // Logo in menu bar
    if (LogoTexID != nullptr)
    {
      FLT logo_size = ImGui::GetFrameHeight() - 4.0f;
      ImGui::Image((ImTextureID)LogoTexID, ImVec2(logo_size, logo_size));
    }
    ImGui::TextColored(style.Colors[ImGuiCol_TextDisabled], "CGSG'Sr'2025 TAM");
    ImGui::Separator();

    // File menu
    if (ImGui::BeginMenu("File"))
    {
      ImGui::MenuItem("New");
      ImGui::MenuItem("Open");
      ImGui::MenuItem("Save");
      ImGui::Separator();
      if (ImGui::MenuItem("Exit", "Alt+F4"))
        PostMessage(UXHosthWnd, WM_CLOSE, 0, 0);
      ImGui::EndMenu();
    }

    // View menu
    if (ImGui::BeginMenu("View"))
    {
      if (ImGui::MenuItem(IsPanelOnRight ? "Panel Left" : "Panel Right"))
      {
        IsPanelOnRight = !IsPanelOnRight;
        IsDraggingSplitter = FALSE;
      }
      if (ImGui::MenuItem(UXHostIsFullScreen ? "Go from fullscreen" : "Go to fullscreen", "F11"))
      {
        if (UXOnFlipFullScreen)
          UXOnFlipFullScreen();
      }
      ImGui::EndMenu();
    }

    // Options menu
    if (ImGui::BeginMenu("Options"))
    {
      if (ImGui::DragFloat("##mf1", &FontSize, 0.1f, 8.0f, 47.0f, "Font Size: %.0f"))
        RegStoreAll();
      if (!FontNames.empty() && SelectedFontIndex < (INT)FontNames.size())
      {
        if (ImGui::BeginCombo("##mf2", FontNames[SelectedFontIndex].c_str()))
        {
          for (INT i = 0; i < (INT)FontNames.size(); i++)
          {
            if (ImGui::Selectable(FontNames[i].c_str(), SelectedFontIndex == i))
            {
              SelectedFontIndex = i;
              RegStoreAll();
            }
            if (SelectedFontIndex == i)
              ImGui::SetItemDefaultFocus();
          }
          ImGui::EndCombo();
        }
      }
      ImGui::Separator();
      // if (ImGui::BeginCombo("##ms1", SelectedStyleIndex == 0 ? "Dark Style" : "Light Style"))
      // {
      //   if (ImGui::Selectable("Dark Style", SelectedStyleIndex == 0))
      //     SelectedStyleIndex = 0, ImGui::GetStyle() = SavedDarkStyle;
      //   if (SelectedStyleIndex == 0)
      //     ImGui::SetItemDefaultFocus();
      //   if (ImGui::Selectable("Light Style", SelectedStyleIndex == 1))
      //     SelectedStyleIndex = 1, ImGui::StyleColorsLight();
      //   if (SelectedStyleIndex == 1)
      //     ImGui::SetItemDefaultFocus();
      //   ImGui::EndCombo();
      // }
      if (ImGui::BeginCombo("##21", (std::get<2>(styles::ColorStyles[styles::CurrentStyle]) + " Style").c_str()))
      {
        for (INT i = 0; i < styles::ColorStyles.size(); i++)
        {
          ImGui::PushID(i);
          bool selected = i == styles::CurrentStyle;
          BOOL is_chk = ImGui::Checkbox(("##chk" + std::to_string(i)).c_str(), &selected); // hidden id for checkbox
          ImGui::SameLine();
          if (is_chk || ImGui::Selectable((std::get<2>(styles::ColorStyles[i]) + " Style").c_str()))
          {
            styles::CurrentStyle = i;
            std::get<1>(styles::ColorStyles[i])();
            RegStoreAll();
          }
          if (i == styles::CurrentStyle)
            ImGui::SetItemDefaultFocus();
          ImGui::PopID();
        }
        ImGui::EndCombo();
      }

      ImGui::EndMenu();
    }

    // Help menu
    if (ImGui::BeginMenu("Help"))
    {
      if (ImGui::MenuItem("About TAM"))
        ShowAboutModalFlag = TRUE;
      ImGui::EndMenu();
    }
    ImGui::Text("%s", std::format(" :: {:.3f}", SystemProfiler.FPS).c_str());

    MenuBarHeight = ImGui::GetWindowSize().y;
    ImGui::EndMainMenuBar();
  }
} /* End of 'tam::ux::core::DrawMainMenu' function */

/* Draw About TAM modal dialog function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::core::DrawAboutModal( VOID )
{
  bool is_modal_open = true;
  if (ImGui::BeginPopupModal("About TAM", &is_modal_open, ImGuiWindowFlags_AlwaysAutoResize))
  {
    if (LogoTexID != nullptr)
    {
      ImGui::Image((ImTextureID)LogoTexID, ImVec2(48, 48));
      ImGui::SameLine();
    }
    ImGui::BeginGroup();
    ImGui::TextColored(ImVec4(0.5f, 0.9f, 1.0f, 1.0f), "TAM - Tough Arrangement Manager");
    ImGui::Text("\xC2\xA9 CGSG 2025-2026");
    ImGui::Text("Seniors 2025 Project");
    ImGui::Text("Computer Graphics Support Group");
    ImGui::Text("of 30 Phys-Math Lyceum");
    ImGui::Separator();
    ImGui::Text((CHAR *)u8"Санкт-Петербургский губернаторский");
    ImGui::Text((CHAR *)u8"Физико-математический лицей № 30");
    ImGui::EndGroup();

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("TAM Command: MK3, AA4, VN4, IK4, MN7, AT7, VG4");

    ImGui::Spacing();

    FLT btn_w = 120.0f;
    FLT avail = ImGui::GetContentRegionAvail().x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (avail - btn_w) * 0.5f);
    if (ImGui::Button("OK", ImVec2(btn_w, 0)))
      ImGui::CloseCurrentPopup();

    if (!is_modal_open)
      ImGui::CloseCurrentPopup();

    ImGui::EndPopup();
  }
} /* End of 'tam::ux::core::DrawAboutModal' function */

/* END OF 'tam_ux_menu.cpp' FILE */
