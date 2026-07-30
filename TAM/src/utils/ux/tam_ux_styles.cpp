/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_styles.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX core styles handle file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 12.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux.h"

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* Convert color from UINT32 to ImGui vec4 function.
     * ARGUMENTS:
     *   - source color:
     *       ImU32 InColorU32;
     * RETURNS:
     *   (ImVec4) result float-point color value.
     */
    inline ImVec4 ColorConvertU32ToFloat4( ImU32 InColorU32 )
    {
      FLT scale_factor = 1.0 / 255.0;
      return ImVec4(((InColorU32 >> IM_COL32_R_SHIFT) & 0xFF) * scale_factor,
                    ((InColorU32 >> IM_COL32_G_SHIFT) & 0xFF) * scale_factor,
                    ((InColorU32 >> IM_COL32_B_SHIFT) & 0xFF) * scale_factor,
                    ((InColorU32 >> IM_COL32_A_SHIFT) & 0xFF) * scale_factor);
    } /* End of 'ColorConvertU32ToFloat4' function */

    /* Styles collection namespace */
    namespace styles
    {
      /* Set dark ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      static VOID StyleDark( VOID )
      {
        ImGui::StyleColorsDark();

        ImGuiStyle *style = &ImGui::GetStyle();
        style->WindowRounding = 0.0f;
        style->FrameRounding = 0.0f;
        style->PopupRounding = 0.0f;
        style->ScrollbarRounding = 0.0f;
        style->GrabRounding = 0.0f;
        style->FrameBorderSize = 2.0f;
        style->WindowBorderSize = 2.0f;
      } /* End of 'StyleDark' function */

      /* Set enemy mouse ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      static VOID StyleLight( VOID )
      {
        ImGui::StyleColorsLight();

        ImGuiStyle *style = &ImGui::GetStyle();
        style->WindowRounding = 0.0f;
        style->FrameRounding = 0.0f;
        style->PopupRounding = 0.0f;
        style->ScrollbarRounding = 0.0f;
        style->GrabRounding = 0.0f;
        style->FrameBorderSize = 2.0f;
        style->WindowBorderSize = 2.0f;
      } /* End of 'StyleLight' function */

      /* Set enemy mouse ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      static VOID StyleEnemyMouse( VOID )
      {
        ImGuiStyle &style = ImGui::GetStyle();

        style.Alpha = 1.0;
        //style.WindowFillAlphaDefault = 0.83;
        //style.ChildWindowRounding = 3;
        style.WindowRounding = 3;
        style.GrabRounding = 1;
        style.GrabMinSize = 20;
        style.FrameRounding = 3;


        style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 1.00f, 1.00f, 0.65f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.18f, 0.21f, 0.73f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.27f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.44f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        //style.Colors[ImGuiCol_ComboBg] = ImVec4(0.16f, 0.24f, 0.22f, 0.60f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.36f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.76f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.65f, 0.65f, 0.46f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.01f, 1.00f, 1.00f, 0.43f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.62f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 1.00f, 1.00f, 0.33f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.42f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
        //style.Colors[ImGuiCol_Column] = ImVec4(0.00f, 0.50f, 0.50f, 0.33f);
        //style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.00f, 0.50f, 0.50f, 0.47f);
        //style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.00f, 0.70f, 0.70f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        //style.Colors[ImGuiCol_CloseButton] = ImVec4(0.00f, 0.78f, 0.78f, 0.35f);
        //style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.00f, 0.78f, 0.78f, 0.47f);
        //style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.00f, 0.78f, 0.78f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);
        //style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.00f, 0.13f, 0.13f, 0.90f);
        style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.04f, 0.10f, 0.09f, 0.51f);
        style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.13f, 0.13f, 0.90f);
        style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.00f, 0.13f, 0.13f, 0.90f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.00f, 0.13f, 0.13f, 0.90f);
        style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.13f, 0.13f, 0.90f);
      } /* End of 'StyleEnemyMouse' function */

      /* Set enemy mouse ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      static VOID StyleColorsSpectrum( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        style->GrabRounding = 4.0f;

        ImVec4 *colors = style->Colors;
        colors[ImGuiCol_Text] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY800); // text on hovered controls is gray900
        colors[ImGuiCol_TextDisabled] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY500);
        colors[ImGuiCol_WindowBg] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY100);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY50); // not sure about this. Note: applies to tooltips too.
        colors[ImGuiCol_Border] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY300);
        colors[ImGuiCol_BorderShadow] = ColorConvertU32ToFloat4(ImGui::Spectrum::Static::NONE); // We don't want shadows. Ever.
        colors[ImGuiCol_FrameBg] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY75); // this isnt right, spectrum does not do this, but it's a good fallback
        colors[ImGuiCol_FrameBgHovered] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY50);
        colors[ImGuiCol_FrameBgActive] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY200);
        colors[ImGuiCol_TitleBg] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY300); // those titlebar values are totally made up, spectrum does not have this.
        colors[ImGuiCol_TitleBgActive] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY200);
        colors[ImGuiCol_TitleBgCollapsed] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY400);
        colors[ImGuiCol_MenuBarBg] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY100);
        colors[ImGuiCol_ScrollbarBg] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY100); // same as regular background
        colors[ImGuiCol_ScrollbarGrab] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY400);
        colors[ImGuiCol_ScrollbarGrabHovered] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY600);
        colors[ImGuiCol_ScrollbarGrabActive] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY700);
        colors[ImGuiCol_CheckMark] = ColorConvertU32ToFloat4(ImGui::Spectrum::BLUE500);
        colors[ImGuiCol_SliderGrab] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY700);
        colors[ImGuiCol_SliderGrabActive] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY800);
        colors[ImGuiCol_Button] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY75); // match default button to ImGui::Spectrum's 'Action Button'.
        colors[ImGuiCol_ButtonHovered] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY50);
        colors[ImGuiCol_ButtonActive] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY200);
        colors[ImGuiCol_Header] = ColorConvertU32ToFloat4(ImGui::Spectrum::BLUE400);
        colors[ImGuiCol_HeaderHovered] = ColorConvertU32ToFloat4(ImGui::Spectrum::BLUE500);
        colors[ImGuiCol_HeaderActive] = ColorConvertU32ToFloat4(ImGui::Spectrum::BLUE600);
        colors[ImGuiCol_Separator] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY400);
        colors[ImGuiCol_SeparatorHovered] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY600);
        colors[ImGuiCol_SeparatorActive] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY700);
        colors[ImGuiCol_ResizeGrip] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY400);
        colors[ImGuiCol_ResizeGripHovered] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY600);
        colors[ImGuiCol_ResizeGripActive] = ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY700);
        colors[ImGuiCol_PlotLines] = ColorConvertU32ToFloat4(ImGui::Spectrum::BLUE400);
        colors[ImGuiCol_PlotLinesHovered] = ColorConvertU32ToFloat4(ImGui::Spectrum::BLUE600);
        colors[ImGuiCol_PlotHistogram] = ColorConvertU32ToFloat4(ImGui::Spectrum::BLUE400);
        colors[ImGuiCol_PlotHistogramHovered] = ColorConvertU32ToFloat4(ImGui::Spectrum::BLUE600);
        colors[ImGuiCol_TextSelectedBg] = ColorConvertU32ToFloat4((ImGui::Spectrum::BLUE400 & 0x00FFFFFF) | 0x33000000);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ColorConvertU32ToFloat4((ImGui::Spectrum::GRAY900 & 0x00FFFFFF) | 0x0A000000);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
      } /* End of 'StyleColorsSpectrum' function */

      /* Set DarkSpectrum ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsDarkSpectrum( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 6.0f;
        style->FrameRounding = 4.0f;
        style->PopupRounding = 4.0f;
        style->ScrollbarRounding = 4.0f;
        style->GrabRounding = 3.0f;
        style->FrameBorderSize = 1.0f;
        style->WindowBorderSize = 0.0f;

        colors[ImGuiCol_Text]                   = ImVec4(0.215f, 0.215f, 0.215f, 1.0f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.501f, 0.501f, 0.501f, 1.0f);
        colors[ImGuiCol_WindowBg]                = ImVec4(0.897f, 0.897f, 0.897f, 1.0f);
        colors[ImGuiCol_ChildBg]                 = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]                 = ImVec4(0.931f, 0.931f, 0.931f, 1.0f);
        colors[ImGuiCol_Border]                  = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);
        colors[ImGuiCol_BorderShadow]            = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]                 = ImVec4(0.294f, 0.294f, 0.294f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]          = ImVec4(0.931f, 0.931f, 0.931f, 1.0f);
        colors[ImGuiCol_FrameBgActive]           = ImVec4(0.784f, 0.784f, 0.784f, 1.0f);
        colors[ImGuiCol_TitleBg]                 = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);
        colors[ImGuiCol_TitleBgActive]           = ImVec4(0.784f, 0.784f, 0.784f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]        = ImVec4(0.502f, 0.502f, 0.502f, 1.0f);
        colors[ImGuiCol_MenuBarBg]               = ImVec4(0.897f, 0.897f, 0.897f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]             = ImVec4(0.897f, 0.897f, 0.897f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]           = ImVec4(0.502f, 0.502f, 0.502f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]    = ImVec4(0.373f, 0.373f, 0.373f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]     = ImVec4(0.275f, 0.275f, 0.275f, 1.0f);
        colors[ImGuiCol_CheckMark]               = ImVec4(0.129f, 0.318f, 0.647f, 1.0f);
        colors[ImGuiCol_SliderGrab]              = ImVec4(0.275f, 0.275f, 0.275f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]        = ImVec4(0.215f, 0.215f, 0.215f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.294f, 0.294f, 0.294f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.931f, 0.931f, 0.931f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.784f, 0.784f, 0.784f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.255f, 0.627f, 0.898f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.318f, 0.502f, 0.863f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.369f, 0.596f, 0.949f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.502f, 0.502f, 0.502f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.373f, 0.373f, 0.373f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.275f, 0.275f, 0.275f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.502f, 0.502f, 0.502f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.373f, 0.373f, 0.373f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.275f, 0.275f, 0.275f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.255f, 0.627f, 0.898f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.369f, 0.596f, 0.949f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.255f, 0.627f, 0.898f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.369f, 0.596f, 0.949f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.255f, 0.627f, 0.898f, 0.2f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.10f, 0.10f, 0.10f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.93f, 0.93f, 0.93f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.66f, 0.66f, 0.66f, 0.75f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.50f, 0.50f, 0.50f, 0.55f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.29f, 0.29f, 0.29f, 0.15f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.37f, 0.37f, 0.37f, 0.20f);
      } /* End of 'StyleColorsDarkSpectrum' function */

      /* Set LightSoft ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsLightSoft( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 7.0f;
        style->FrameRounding = 6.0f;
        style->PopupRounding = 6.0f;
        style->ScrollbarRounding = 5.0f;
        style->GrabRounding = 5.0f;
        style->FrameBorderSize = 0.0f;
        style->WindowBorderSize = 1.0f;

        colors[ImGuiCol_Text]                   = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        colors[ImGuiCol_WindowBg]                = ImVec4(0.97f, 0.97f, 0.98f, 1.0f);
        colors[ImGuiCol_ChildBg]                 = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]                 = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
        colors[ImGuiCol_Border]                  = ImVec4(0.85f, 0.85f, 0.85f, 1.0f);
        colors[ImGuiCol_BorderShadow]            = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]                 = ImVec4(0.88f, 0.88f, 0.88f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]          = ImVec4(0.78f, 0.78f, 0.78f, 1.0f);
        colors[ImGuiCol_FrameBgActive]           = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
        colors[ImGuiCol_TitleBg]                 = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
        colors[ImGuiCol_TitleBgActive]           = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]        = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
        colors[ImGuiCol_MenuBarBg]               = ImVec4(0.93f, 0.93f, 0.95f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]             = ImVec4(0.93f, 0.93f, 0.95f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]           = ImVec4(0.55f, 0.55f, 0.55f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]    = ImVec4(0.65f, 0.65f, 0.65f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]     = ImVec4(0.70f, 0.70f, 0.70f, 1.0f);
        colors[ImGuiCol_CheckMark]               = ImVec4(0.2f, 0.4f, 0.8f, 1.0f);
        colors[ImGuiCol_SliderGrab]              = ImVec4(0.64f, 0.64f, 0.64f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]        = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.8f, 0.85f, 0.9f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.65f, 0.7f, 0.75f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.55f, 0.6f, 0.65f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.33f, 0.57f, 0.82f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.29f, 0.42f, 0.7f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.24f, 0.35f, 0.56f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.45f, 0.45f, 0.45f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.37f, 0.37f, 0.37f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.27f, 0.27f, 0.27f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.33f, 0.57f, 0.82f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.37f, 0.6f, 0.95f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.33f, 0.57f, 0.82f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.37f, 0.6f, 0.95f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.33f, 0.57f, 0.82f, 0.2f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.1f, 0.1f, 0.1f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.8f, 0.8f, 0.8f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.75f, 0.70f, 0.85f, 0.9f);
        colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.68f, 0.63f, 0.80f, 0.75f);
        colors[ImGuiCol_TableBorderLight]       = ImVec4(0.7f, 0.66f, 0.81f, 0.55f);
        colors[ImGuiCol_TableRowBg]             = ImVec4(0.9f, 0.9f, 0.95f, 0.10f);
        colors[ImGuiCol_TableRowBgAlt]          = ImVec4(0.85f, 0.85f, 0.9f, 0.15f);
      } /* End of 'StyleColorsLightSoft' function */

      /* Set ClassicSoft ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsClassicSoft( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 5.0f;
        style->FrameRounding = 3.0f;
        style->PopupRounding = 4.0f;
        style->ScrollbarRounding = 6.0f;
        style->GrabRounding = 4.0f;
        style->FrameBorderSize = 1.0f;
        style->WindowBorderSize = 1.0f;

        colors[ImGuiCol_Text]                  = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
        colors[ImGuiCol_Border]                 = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.81f, 0.81f, 0.81f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.68f, 0.68f, 0.68f, 1.00f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.63f, 0.63f, 0.63f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_CheckMark]               = ImVec4(0.13f, 0.32f, 0.64f, 1.00f);
        colors[ImGuiCol_SliderGrab]              = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]        = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_Button]                  = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_Header]                  = ImVec4(0.25f, 0.61f, 0.89f, 1.00f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.20f, 0.53f, 0.82f, 1.00f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.18f, 0.47f, 0.73f, 1.00f);
        colors[ImGuiCol_Separator]               = ImVec4(0.53f, 0.53f, 0.53f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.52f, 0.92f, 0.71f, 1.00f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.47f, 0.53f, 0.73f, 1.00f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.18f, 0.53f, 0.91f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.22f, 0.62f, 0.88f, 1.00f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.16f, 0.48f, 0.85f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.21f, 0.58f, 0.85f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.18f, 0.53f, 0.89f, 0.20f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.05f, 0.05f, 0.05f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.7f, 0.7f, 0.7f, 0.90f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.6f, 0.6f, 0.6f, 0.75f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.5f, 0.5f, 0.5f, 0.55f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.9f, 0.9f, 0.9f, 0.10f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.85f, 0.85f, 0.85f, 0.15f);
      } /* End of 'StyleColorsClassicSoft' function */

      /* Set BlueOcean ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsBlueOcean( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 2.0f;
        style->FrameRounding = 1.0f;
        style->PopupRounding = 2.0f;
        style->ScrollbarRounding = 3.0f;
        style->GrabRounding = 4.0f;
        style->FrameBorderSize = 1.0f;
        style->WindowBorderSize = 0.0f;

        colors[ImGuiCol_Text]                   = ImVec4(0.90f, 0.97f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.66f, 0.66f, 0.66f, 1.00f);
        colors[ImGuiCol_WindowBg]                = ImVec4(0.09f, 0.16f, 0.24f, 1.00f);
        colors[ImGuiCol_ChildBg]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]                 = ImVec4(0.07f, 0.13f, 0.23f, 1.00f);
        colors[ImGuiCol_Border]                  = ImVec4(0.20f, 0.32f, 0.51f, 1.00f);
        colors[ImGuiCol_BorderShadow]            = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]                 = ImVec4(0.04f, 0.20f, 0.44f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]          = ImVec4(0.15f, 0.30f, 0.54f, 1.00f);
        colors[ImGuiCol_FrameBgActive]           = ImVec4(0.20f, 0.35f, 0.60f, 1.00f);
        colors[ImGuiCol_TitleBg]                 = ImVec4(0.03f, 0.22f, 0.40f, 1.00f);
        colors[ImGuiCol_TitleBgActive]           = ImVec4(0.11f, 0.30f, 0.54f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]        = ImVec4(0.09f, 0.25f, 0.46f, 1.00f);
        colors[ImGuiCol_MenuBarBg]               = ImVec4(0.05f, 0.17f, 0.34f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]             = ImVec4(0.05f, 0.15f, 0.29f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]           = ImVec4(0.20f, 0.38f, 0.63f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]    = ImVec4(0.22f, 0.42f, 0.69f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]     = ImVec4(0.25f, 0.46f, 0.74f, 1.00f);
        colors[ImGuiCol_CheckMark]               = ImVec4(0.12f, 0.34f, 0.66f, 1.00f);
        colors[ImGuiCol_SliderGrab]              = ImVec4(0.40f, 0.65f, 0.96f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]        = ImVec4(0.55f, 0.75f, 0.99f, 1.00f);
        colors[ImGuiCol_Button]                  = ImVec4(0.10f, 0.42f, 0.70f, 1.00f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.15f, 0.58f, 0.93f, 1.00f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.20f, 0.49f, 0.80f, 1.00f);
        colors[ImGuiCol_Header]                  = ImVec4(0.15f, 0.48f, 0.80f, 1.00f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.20f, 0.56f, 0.90f, 1.00f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.27f, 0.64f, 0.95f, 1.00f);
        colors[ImGuiCol_Separator]               = ImVec4(0.15f, 0.38f, 0.63f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.20f, 0.56f, 0.90f, 1.00f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.18f, 0.50f, 0.80f, 1.00f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.35f, 0.68f, 0.99f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.47f, 0.74f, 1.00f, 1.00f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.55f, 0.80f, 1.00f, 1.00f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.20f, 0.45f, 0.85f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.25f, 0.52f, 0.90f, 1.00f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.21f, 0.50f, 0.86f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.25f, 0.58f, 0.91f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.20f, 0.42f, 0.80f, 0.20f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.10f, 0.10f, 0.10f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.15f, 0.30f, 0.55f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.20f, 0.42f, 0.75f, 0.90f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.15f, 0.29f, 0.54f, 0.70f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.15f, 0.33f, 0.85f, 0.20f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.10f, 0.25f, 0.67f, 0.10f);
      } /* End of 'StyleColorsBlueOcean' function */

      /* Set NeonPink ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsNeonPink( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 10.0f;
        style->FrameRounding = 8.0f;
        style->PopupRounding = 8.0f;
        style->ScrollbarRounding = 8.0f;
        style->GrabRounding = 8.0f;
        style->FrameBorderSize = 1.5f;
        style->WindowBorderSize = 1.0f;

        colors[ImGuiCol_Text]                   = ImVec4(1.00f, 0.20f, 0.50f, 1.0f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.69f, 0.42f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg]                = ImVec4(0.10f, 0.00f, 0.07f, 1.00f);
        colors[ImGuiCol_ChildBg]                 = ImVec4(0.10f, 0.00f, 0.07f, 1.00f);
        colors[ImGuiCol_PopupBg]                 = ImVec4(0.19f, 0.00f, 0.15f, 1.00f);
        colors[ImGuiCol_Border]                  = ImVec4(1.00f, 0.50f, 0.78f, 1.00f);
        colors[ImGuiCol_BorderShadow]            = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]                 = ImVec4(0.30f, 0.05f, 0.21f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]          = ImVec4(0.45f, 0.00f, 0.32f, 1.00f);
        colors[ImGuiCol_FrameBgActive]           = ImVec4(0.65f, 0.00f, 0.50f, 1.00f);
        colors[ImGuiCol_TitleBg]                 = ImVec4(0.21f, 0.03f, 0.14f, 1.00f);
        colors[ImGuiCol_TitleBgActive]           = ImVec4(0.42f, 0.00f, 0.24f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]        = ImVec4(0.35f, 0.01f, 0.18f, 1.00f);
        colors[ImGuiCol_MenuBarBg]               = ImVec4(0.26f, 0.00f, 0.17f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]             = ImVec4(0.20f, 0.00f, 0.12f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]           = ImVec4(1.00f, 0.59f, 0.89f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]    = ImVec4(1.00f, 0.29f, 0.68f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]     = ImVec4(1.00f, 0.05f, 0.56f, 1.00f);
        colors[ImGuiCol_CheckMark]               = ImVec4(1.00f, 0.29f, 0.68f, 1.00f);
        colors[ImGuiCol_SliderGrab]              = ImVec4(1.00f, 0.18f, 0.59f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]        = ImVec4(1.00f, 0.14f, 0.49f, 1.00f);
        colors[ImGuiCol_Button]                  = ImVec4(0.30f, 0.05f, 0.20f, 1.00f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(1.00f, 0.00f, 0.60f, 1.00f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.68f, 0.00f, 0.40f, 1.00f);
        colors[ImGuiCol_Header]                  = ImVec4(1.00f, 0.39f, 0.77f, 1.00f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(1.00f, 0.29f, 0.68f, 1.00f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(1.00f, 0.14f, 0.49f, 1.00f);
        colors[ImGuiCol_Separator]               = ImVec4(0.78f, 0.37f, 0.71f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(1.00f, 0.19f, 0.52f, 1.00f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(1.00f, 0.09f, 0.40f, 1.00f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.86f, 0.37f, 0.75f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(1.00f, 0.15f, 0.59f, 1.00f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(1.00f, 0.05f, 0.50f, 1.00f);
        colors[ImGuiCol_PlotLines]               = ImVec4(1.00f, 0.18f, 0.58f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(1.00f, 0.22f, 0.62f, 1.00f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(1.00f, 0.18f, 0.58f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(1.00f, 0.22f, 0.62f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(1.00f, 0.22f, 0.62f, 0.20f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.10f, 0.10f, 0.10f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(1.0f,  1.0f,  1.0f,  0.7f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.8f,  0.8f,  0.8f,  0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.19f, 0.00f, 0.15f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(1.00f, 0.50f, 0.78f, 0.81f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.84f, 0.29f, 0.59f, 0.65f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.40f, 0.10f, 0.31f, 0.12f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.20f, 0.05f, 0.25f, 0.18f);
      } /* End of 'StyleColorsNeonPink' function */

      /* Set CleanBlue ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsCleanBlue( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 3.0f;
        style->FrameRounding = 3.0f;
        style->PopupRounding = 5.0f;
        style->ScrollbarRounding = 2.0f;
        style->GrabRounding = 3.0f;
        style->FrameBorderSize = 1.0f;
        style->WindowBorderSize = 1.0f;

        colors[ImGuiCol_Text]                 = ImVec4(0.10f, 0.22f, 0.42f, 1.0f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.45f, 0.52f, 0.60f, 1.0f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.93f, 0.95f, 1.00f, 1.0f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.96f, 0.97f, 1.00f, 1.0f);
        colors[ImGuiCol_Border]                = ImVec4(0.54f, 0.62f, 0.75f, 1.0f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.89f, 0.91f, 0.96f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.80f, 0.83f, 0.92f, 1.0f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.72f, 0.77f, 0.86f, 1.0f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.60f, 0.70f, 0.89f, 1.0f);
        colors[ImGuiCol_TitleBgActive]         = ImVec4(0.46f, 0.55f, 0.77f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.40f, 0.52f, 0.70f, 1.0f);
        colors[ImGuiCol_MenuBarBg]             = ImVec4(0.82f, 0.87f, 0.97f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.83f, 0.87f, 0.96f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.52f, 0.59f, 0.75f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.57f, 0.63f, 0.81f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.63f, 0.68f, 0.86f, 1.0f);
        colors[ImGuiCol_CheckMark]             = ImVec4(0.12f, 0.28f, 0.58f, 1.0f);
        colors[ImGuiCol_SliderGrab]            = ImVec4(0.38f, 0.50f, 0.73f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.28f, 0.42f, 0.63f, 1.0f);
        colors[ImGuiCol_Button]                = ImVec4(0.65f, 0.74f, 0.96f, 1.0f);
        colors[ImGuiCol_ButtonHovered]         = ImVec4(0.54f, 0.63f, 0.89f, 1.0f);
        colors[ImGuiCol_ButtonActive]          = ImVec4(0.39f, 0.50f, 0.75f, 1.0f);
        colors[ImGuiCol_Header]                = ImVec4(0.54f, 0.62f, 0.88f, 1.0f);
        colors[ImGuiCol_HeaderHovered]         = ImVec4(0.47f, 0.55f, 0.79f, 1.0f);
        colors[ImGuiCol_HeaderActive]          = ImVec4(0.43f, 0.50f, 0.74f, 1.0f);
        colors[ImGuiCol_Separator]             = ImVec4(0.48f, 0.54f, 0.74f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.56f, 0.61f, 0.82f, 1.0f);
        colors[ImGuiCol_SeparatorActive]       = ImVec4(0.66f, 0.71f, 0.88f, 1.0f);
        colors[ImGuiCol_ResizeGrip]            = ImVec4(0.52f, 0.59f, 0.78f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.63f, 0.66f, 0.87f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.72f, 0.74f, 0.89f, 1.0f);
        colors[ImGuiCol_PlotLines]             = ImVec4(0.33f, 0.44f, 0.59f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.41f, 0.57f, 0.74f, 1.0f);
        colors[ImGuiCol_PlotHistogram]         = ImVec4(0.32f, 0.41f, 0.57f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.39f, 0.53f, 0.70f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.32f, 0.43f, 0.59f, 0.2f);
        colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.01f, 0.90f);
        colors[ImGuiCol_NavHighlight]          = ImVec4(0.08f, 0.08f, 0.1f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.7f);
        colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
      } /* End of 'StyleColorsCleanBlue' function */

      /* Set VintageRed ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsVintageRed( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 3.0f;
        style->FrameRounding = 2.0f;
        style->PopupRounding = 2.0f;
        style->ScrollbarRounding = 3.0f;
        style->GrabRounding = 3.0f;
        style->FrameBorderSize = 1.5f;
        style->WindowBorderSize = 1.0f;

        colors[ImGuiCol_Text]                 = ImVec4(0.50f, 0.10f, 0.10f, 1.0f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.52f, 0.12f, 0.12f, 1.0f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.98f, 0.89f, 0.92f, 1.0f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.97f, 0.85f, 0.87f, 1.0f);
        colors[ImGuiCol_Border]                = ImVec4(0.83f, 0.45f, 0.49f, 1.0f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.89f, 0.64f, 0.67f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.83f, 0.45f, 0.49f, 1.0f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.77f, 0.40f, 0.45f, 1.0f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.91f, 0.42f, 0.46f, 1.0f);
        colors[ImGuiCol_TitleBgActive]         = ImVec4(0.78f, 0.40f, 0.43f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.66f, 0.30f, 0.34f, 1.0f);
        colors[ImGuiCol_MenuBarBg]             = ImVec4(0.80f, 0.29f, 0.33f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.80f, 0.28f, 0.32f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.60f, 0.20f, 0.24f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.85f, 0.30f, 0.35f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.95f, 0.38f, 0.43f, 1.0f);
        colors[ImGuiCol_CheckMark]             = ImVec4(0.75f, 0.20f, 0.25f, 1.0f);
        colors[ImGuiCol_SliderGrab]            = ImVec4(0.85f, 0.20f, 0.25f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.65f, 0.10f, 0.20f, 1.0f);
        colors[ImGuiCol_Button]                = ImVec4(0.95f, 0.65f, 0.70f, 1.0f);
        colors[ImGuiCol_ButtonHovered]         = ImVec4(0.85f, 0.25f, 0.30f, 1.0f);
        colors[ImGuiCol_ButtonActive]          = ImVec4(0.70f, 0.20f, 0.25f, 1.0f);
        colors[ImGuiCol_Header]                = ImVec4(0.95f, 0.70f, 0.73f, 1.0f);
        colors[ImGuiCol_HeaderHovered]         = ImVec4(0.85f, 0.45f, 0.50f, 1.0f);
        colors[ImGuiCol_HeaderActive]          = ImVec4(0.72f, 0.36f, 0.40f, 1.0f);
        colors[ImGuiCol_Separator]             = ImVec4(0.75f, 0.50f, 0.54f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.95f, 0.65f, 0.68f, 1.0f);
        colors[ImGuiCol_SeparatorActive]       = ImVec4(0.80f, 0.44f, 0.48f, 1.0f);
        colors[ImGuiCol_ResizeGrip]            = ImVec4(0.80f, 0.44f, 0.48f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.95f, 0.65f, 0.68f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.99f, 0.72f, 0.76f, 1.0f);
        colors[ImGuiCol_PlotLines]             = ImVec4(0.72f, 0.30f, 0.35f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.91f, 0.40f, 0.45f, 1.0f);
        colors[ImGuiCol_PlotHistogram]         = ImVec4(0.70f, 0.28f, 0.32f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.94f, 0.44f, 0.48f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.70f, 0.28f, 0.32f, 0.20f);
        colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]          = ImVec4(0.10f, 0.10f, 0.10f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.00f, 1.00f, 1.00f, 0.10f);
        colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.00f, 0.50f, 0.50f, 0.70f);
        colors[ImGuiCol_TableBorderLight]      = ImVec4(0.00f, 0.50f, 0.50f, 0.35f);
        colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 1.00f, 1.00f, 0.05f);
        colors[ImGuiCol_TableRowBgAlt]         = ImVec4(0.00f, 1.00f, 1.00f, 0.10f);

        colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.90f, 0.70f, 0.75f, 0.80f);
        colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.85f, 0.55f, 0.60f, 0.70f);
        colors[ImGuiCol_TableBorderLight]      = ImVec4(0.78f, 0.48f, 0.52f, 0.60f);
        colors[ImGuiCol_TableRowBg]            = ImVec4(0.95f, 0.75f, 0.80f, 0.15f);
        colors[ImGuiCol_TableRowBgAlt]         = ImVec4(0.90f, 0.70f, 0.75f, 0.20f);
      } /* End of 'StyleColorsVintageRed' function */

      /* Set ForestGreen ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsForestGreen( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 4.0f;
        style->FrameRounding = 3.0f;
        style->PopupRounding = 5.0f;
        style->ScrollbarRounding = 5.0f;
        style->GrabRounding = 4.0f;
        style->FrameBorderSize = 1.0f;
        style->WindowBorderSize = 1.0f;

        colors[ImGuiCol_Text]                   = ImVec4(0.14f, 0.27f, 0.13f, 1.0f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.36f, 0.49f, 0.36f, 1.0f);
        colors[ImGuiCol_WindowBg]                = ImVec4(0.88f, 0.93f, 0.87f, 1.0f);
        colors[ImGuiCol_ChildBg]                 = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]                 = ImVec4(0.74f, 0.83f, 0.78f, 1.0f);
        colors[ImGuiCol_Border]                  = ImVec4(0.50f, 0.70f, 0.50f, 1.0f);
        colors[ImGuiCol_BorderShadow]            = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]                 = ImVec4(0.65f, 0.77f, 0.67f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]          = ImVec4(0.57f, 0.69f, 0.58f, 1.0f);
        colors[ImGuiCol_FrameBgActive]           = ImVec4(0.52f, 0.65f, 0.55f, 1.0f);
        colors[ImGuiCol_TitleBg]                 = ImVec4(0.62f, 0.78f, 0.58f, 1.0f);
        colors[ImGuiCol_TitleBgActive]           = ImVec4(0.50f, 0.68f, 0.47f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]        = ImVec4(0.44f, 0.60f, 0.41f, 1.0f);
        colors[ImGuiCol_MenuBarBg]               = ImVec4(0.59f, 0.77f, 0.59f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]             = ImVec4(0.58f, 0.76f, 0.58f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]           = ImVec4(0.43f, 0.58f, 0.44f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]    = ImVec4(0.50f, 0.67f, 0.53f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]     = ImVec4(0.55f, 0.73f, 0.58f, 1.0f);
        colors[ImGuiCol_CheckMark]               = ImVec4(0.26f, 0.52f, 0.23f, 1.0f);
        colors[ImGuiCol_SliderGrab]              = ImVec4(0.48f, 0.75f, 0.40f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]        = ImVec4(0.39f, 0.62f, 0.36f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.71f, 0.90f, 0.68f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.60f, 0.81f, 0.59f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.50f, 0.67f, 0.49f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.60f, 0.81f, 0.62f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.54f, 0.74f, 0.60f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.46f, 0.63f, 0.53f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.56f, 0.83f, 0.72f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.64f, 0.92f, 0.80f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.70f, 0.94f, 0.83f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.45f, 0.70f, 0.52f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.55f, 0.80f, 0.62f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.65f, 0.90f, 0.72f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.30f, 0.40f, 0.25f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.35f, 0.50f, 0.30f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.32f, 0.42f, 0.27f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.38f, 0.52f, 0.33f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.30f, 0.40f, 0.25f, 0.20f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.10f, 0.10f, 0.10f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.6f, 0.75f, 0.65f, 1.0f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.4f, 0.5f, 0.45f, 1.0f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.7f, 0.8f, 0.75f, 1.0f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.8f, 0.9f, 0.85f, 0.07f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.85f, 0.95f, 0.90f, 0.07f);

        //colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.22f, 0.36f, 0.20f, 0.85f);
        //colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.30f, 0.55f, 0.33f, 0.70f);
        //colors[ImGuiCol_TableBorderLight]        = ImVec4(0.25f, 0.45f, 0.25f, 0.50f);
        //colors[ImGuiCol_TableRowBg]              = ImVec4(0.33f, 0.55f, 0.30f, 0.15f);
        //colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.27f, 0.48f, 0.27f, 0.20f);
      } /* End of 'StyleColorsForestGreen' function */

      /* Set MonoChrome ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsMonoChrome( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 0.0f;
        style->FrameRounding = 0.0f;
        style->PopupRounding = 0.0f;
        style->ScrollbarRounding = 0.0f;
        style->GrabRounding = 0.0f;
        style->FrameBorderSize = 1.0f;
        style->WindowBorderSize = 1.0f;

        colors[ImGuiCol_Text]                  = ImVec4(0.88f, 0.88f, 0.88f, 1.0f);
        colors[ImGuiCol_TextDisabled]          = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.13f, 0.13f, 0.13f, 1.0f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);
        colors[ImGuiCol_Border]                 = ImVec4(0.45f, 0.45f, 0.45f, 1.0f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.22f, 0.22f, 0.22f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.32f, 0.32f, 0.32f, 1.0f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.40f, 0.40f, 0.40f, 1.0f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.19f, 0.19f, 0.19f, 1.0f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.33f, 0.33f, 0.33f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.36f, 0.36f, 0.36f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.55f, 0.55f, 0.55f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.90f, 0.90f, 0.90f, 1.0f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.65f, 0.65f, 0.65f, 1.0f);
        colors[ImGuiCol_Button]                 = ImVec4(0.40f, 0.40f, 0.40f, 1.0f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.55f, 0.55f, 0.55f, 1.0f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
        colors[ImGuiCol_Header]                 = ImVec4(0.44f, 0.44f, 0.44f, 1.0f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.70f, 0.70f, 0.70f, 1.0f);
        colors[ImGuiCol_Separator]              = ImVec4(0.45f, 0.45f, 0.45f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.70f, 0.70f, 0.70f, 1.0f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.65f, 0.65f, 0.65f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.80f, 0.80f, 0.80f, 1.0f);
        colors[ImGuiCol_PlotLines]              = ImVec4(0.44f, 0.44f, 0.44f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(0.42f, 0.42f, 0.42f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.58f, 0.58f, 0.58f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.50f, 0.50f, 0.50f, 0.20f);
        colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]           = ImVec4(0.10f, 0.10f, 0.10f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
      } /* End of 'StyleColorsMonoChrome' function */

      /* Set OceanDeep ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsOceanDeep( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 8.0f;
        style->FrameRounding = 5.0f;
        style->PopupRounding = 8.0f;
        style->ScrollbarRounding = 7.0f;
        style->GrabRounding = 6.0f;
        style->FrameBorderSize = 1.0f;
        style->WindowBorderSize = 1.0f;

        colors[ImGuiCol_Text]                 = ImVec4(0.95f, 0.97f, 1.00f, 1.0f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.65f, 0.66f, 0.69f, 1.0f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.05f, 0.10f, 0.15f, 1.0f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.10f, 0.13f, 1.0f);
        colors[ImGuiCol_Border]                = ImVec4(0.18f, 0.23f, 0.30f, 1.0f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.10f, 0.17f, 0.26f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.15f, 0.25f, 0.38f, 1.0f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.25f, 0.35f, 0.48f, 1.0f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.12f, 0.20f, 0.29f, 1.0f);
        colors[ImGuiCol_TitleBgActive]         = ImVec4(0.22f, 0.30f, 0.44f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.18f, 0.25f, 0.36f, 1.0f);
        colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.21f, 0.30f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.12f, 0.18f, 0.25f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.25f, 0.37f, 0.57f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.28f, 0.43f, 0.65f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.32f, 0.48f, 0.69f, 1.0f);
        colors[ImGuiCol_CheckMark]             = ImVec4(0.10f, 0.38f, 0.77f, 1.0f);
        colors[ImGuiCol_SliderGrab]            = ImVec4(0.20f, 0.45f, 0.90f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.30f, 0.55f, 0.95f, 1.0f);
        colors[ImGuiCol_Button]                = ImVec4(0.08f, 0.48f, 0.91f, 1.0f);
        colors[ImGuiCol_ButtonHovered]         = ImVec4(0.12f, 0.56f, 0.98f, 1.0f);
        colors[ImGuiCol_ButtonActive]          = ImVec4(0.15f, 0.65f, 1.00f, 1.0f);
        colors[ImGuiCol_Header]                = ImVec4(0.12f, 0.48f, 0.91f, 1.0f);
        colors[ImGuiCol_HeaderHovered]         = ImVec4(0.15f, 0.62f, 0.95f, 1.0f);
        colors[ImGuiCol_HeaderActive]          = ImVec4(0.15f, 0.36f, 0.90f, 1.0f);
        colors[ImGuiCol_Separator]             = ImVec4(0.20f, 0.35f, 0.55f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.30f, 0.45f, 0.65f, 1.0f);
        colors[ImGuiCol_SeparatorActive]       = ImVec4(0.25f, 0.38f, 0.60f, 1.0f);
        colors[ImGuiCol_ResizeGrip]            = ImVec4(0.20f, 0.40f, 0.75f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.28f, 0.50f, 0.80f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.35f, 0.55f, 0.95f, 1.0f);
        colors[ImGuiCol_PlotLines]             = ImVec4(0.20f, 0.42f, 0.85f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.30f, 0.52f, 0.95f, 1.0f);
        colors[ImGuiCol_PlotHistogram]         = ImVec4(0.20f, 0.42f, 0.85f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.30f, 0.52f, 0.95f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.22f, 0.48f, 0.92f, 0.3f);
        colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]          = ImVec4(0.05f, 0.05f, 0.05f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.07f, 0.15f, 0.23f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.14f, 0.30f, 0.45f, 0.8f);
        colors[ImGuiCol_TableBorderLight]      = ImVec4(0.11f, 0.23f, 0.35f, 0.6f);
        colors[ImGuiCol_TableRowBg]            = ImVec4(0.09f, 0.19f, 0.29f, 0.1f);
        colors[ImGuiCol_TableRowBgAlt]         = ImVec4(0.04f, 0.10f, 0.20f, 0.15f);
      } /* End of 'StyleColorsOceanDeep' function */

      /* Set CandyPink ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsCandyPink( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 8.0f;
        style->FrameRounding = 8.0f;
        style->PopupRounding = 10.0f;
        style->ScrollbarRounding = 8.0f;
        style->GrabRounding = 6.0f;
        style->FrameBorderSize = 0.0f;
        style->WindowBorderSize = 0.0f;

        colors[ImGuiCol_Text]                 = ImVec4(0.40f, 0.13f, 0.24f, 1.00f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.70f, 0.38f, 0.59f, 1.00f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.98f, 0.91f, 0.92f, 1.00f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.99f, 0.89f, 0.92f, 0.90f);
        colors[ImGuiCol_Border]                = ImVec4(0.78f, 0.38f, 0.59f, 1.00f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.96f, 0.73f, 0.80f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.94f, 0.61f, 0.74f, 1.00f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.96f, 0.67f, 0.77f, 1.00f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.96f, 0.55f, 0.74f, 1.00f);
        colors[ImGuiCol_TitleBgActive]         = ImVec4(0.82f, 0.38f, 0.59f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.78f, 0.33f, 0.56f, 1.00f);
        colors[ImGuiCol_MenuBarBg]             = ImVec4(0.91f, 0.55f, 0.73f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.91f, 0.55f, 0.73f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.75f, 0.38f, 0.59f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.89f, 0.48f, 0.68f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.84f, 0.44f, 0.64f, 1.00f);
        colors[ImGuiCol_CheckMark]             = ImVec4(0.96f, 0.47f, 0.67f, 1.00f);
        colors[ImGuiCol_SliderGrab]            = ImVec4(0.82f, 0.45f, 0.66f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.78f, 0.39f, 0.60f, 1.00f);
        colors[ImGuiCol_Button]                = ImVec4(0.78f, 0.38f, 0.59f, 1.00f);
        colors[ImGuiCol_ButtonHovered]         = ImVec4(0.87f, 0.48f, 0.68f, 1.00f);
        colors[ImGuiCol_ButtonActive]          = ImVec4(0.70f, 0.35f, 0.62f, 1.00f);
        colors[ImGuiCol_Header]                = ImVec4(0.80f, 0.46f, 0.58f, 1.00f);
        colors[ImGuiCol_HeaderHovered]         = ImVec4(0.93f, 0.61f, 0.73f, 1.00f);
        colors[ImGuiCol_HeaderActive]          = ImVec4(0.78f, 0.45f, 0.67f, 1.00f);
        colors[ImGuiCol_Separator]             = ImVec4(0.68f, 0.37f, 0.58f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.76f, 0.43f, 0.61f, 1.00f);
        colors[ImGuiCol_SeparatorActive]       = ImVec4(0.72f, 0.41f, 0.60f, 1.00f);
        colors[ImGuiCol_ResizeGrip]            = ImVec4(0.73f, 0.52f, 0.63f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.80f, 0.60f, 0.69f, 1.00f);
        colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.84f, 0.63f, 0.71f, 1.00f);
        colors[ImGuiCol_PlotLines]             = ImVec4(0.70f, 0.34f, 0.48f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.90f, 0.40f, 0.57f, 1.00f);
        colors[ImGuiCol_PlotHistogram]         = ImVec4(0.68f, 0.30f, 0.45f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.85f, 0.36f, 0.54f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.80f, 0.39f, 0.60f, 0.25f);
        colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]          = ImVec4(0.18f, 0.10f, 0.25f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.98f, 0.84f, 0.98f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.58f, 0.45f, 0.60f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.32f, 0.24f, 0.43f, 0.38f);

        colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.86f, 0.34f, 0.59f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.70f, 0.20f, 0.50f, 0.75f);
        colors[ImGuiCol_TableBorderLight]      = ImVec4(0.75f, 0.25f, 0.54f, 0.65f);
        colors[ImGuiCol_TableRowBg]            = ImVec4(0.87f, 0.60f, 0.79f, 0.12f);
        colors[ImGuiCol_TableRowBgAlt]         = ImVec4(0.85f, 0.50f, 0.74f, 0.10f);
      } /* End of 'StyleColorsCandyPink' function */

      /* Set DeepNight ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsDeepNight( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 3.0f;
        style->FrameRounding = 2.0f;
        style->PopupRounding = 3.0f;
        style->ScrollbarRounding = 2.0f;
        style->GrabRounding = 3.0f;
        style->FrameBorderSize = 2.0f;
        style->WindowBorderSize = 1.0f;

        colors[ImGuiCol_Text]                   = ImVec4(0.67f, 0.72f, 0.93f, 1.0f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.40f, 0.46f, 0.57f, 1.0f);
        colors[ImGuiCol_WindowBg]                = ImVec4(0.04f, 0.05f, 0.13f, 1.0f);
        colors[ImGuiCol_ChildBg]                 = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]                 = ImVec4(0.06f, 0.08f, 0.17f, 1.0f);
        colors[ImGuiCol_Border]                  = ImVec4(0.30f, 0.35f, 0.52f, 1.0f);
        colors[ImGuiCol_BorderShadow]            = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]                 = ImVec4(0.09f, 0.14f, 0.23f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]          = ImVec4(0.12f, 0.24f, 0.40f, 1.0f);
        colors[ImGuiCol_FrameBgActive]           = ImVec4(0.18f, 0.30f, 0.45f, 1.0f);
        colors[ImGuiCol_TitleBg]                 = ImVec4(0.09f, 0.14f, 0.23f, 1.0f);
        colors[ImGuiCol_TitleBgActive]           = ImVec4(0.19f, 0.24f, 0.30f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]        = ImVec4(0.08f, 0.11f, 0.17f, 1.0f);
        colors[ImGuiCol_MenuBarBg]               = ImVec4(0.06f, 0.12f, 0.18f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]             = ImVec4(0.09f, 0.18f, 0.26f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]           = ImVec4(0.25f, 0.43f, 0.67f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]    = ImVec4(0.35f, 0.55f, 0.81f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]     = ImVec4(0.40f, 0.62f, 0.88f, 1.0f);
        colors[ImGuiCol_CheckMark]               = ImVec4(0.30f, 0.45f, 0.91f, 1.0f);
        colors[ImGuiCol_SliderGrab]              = ImVec4(0.45f, 0.60f, 0.92f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]        = ImVec4(0.60f, 0.72f, 0.96f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.30f, 0.45f, 0.92f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.45f, 0.60f, 0.92f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.55f, 0.72f, 0.95f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.30f, 0.45f, 0.85f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.45f, 0.60f, 0.92f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.60f, 0.75f, 0.98f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.35f, 0.50f, 0.70f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.50f, 0.65f, 0.85f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.55f, 0.70f, 0.90f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.38f, 0.53f, 0.83f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.45f, 0.60f, 0.90f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.55f, 0.72f, 0.95f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.40f, 0.55f, 0.95f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.50f, 0.65f, 1.00f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.38f, 0.53f, 0.92f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.48f, 0.63f, 0.96f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.42f, 0.60f, 0.98f, 0.20f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.10f, 0.10f, 0.10f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.15f, 0.18f, 0.24f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.30f, 0.32f, 0.42f, 0.75f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.25f, 0.28f, 0.38f, 0.55f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.20f, 0.22f, 0.30f, 0.10f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.12f, 0.15f, 0.25f, 0.15f);
      } /* End of 'StyleColorsDeepNight' function */

      /* Set AdminPanel ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsAdminPanel( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;
  
        style->WindowRounding = 0.0f;
        style->FrameRounding = 0.0f;
        style->PopupRounding = 4.0f;
        style->ScrollbarRounding = 4.0f;
        style->GrabRounding = 3.0f;
        style->FrameBorderSize = 2.0f;
        style->WindowBorderSize = 1.0f;

        colors[ImGuiCol_Text]                = ImVec4(1.00f, 0.98f, 0.98f, 1.00f);
        colors[ImGuiCol_TextDisabled]        = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        colors[ImGuiCol_WindowBg]             = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
        colors[ImGuiCol_ChildBg]              = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]              = ImVec4(0.12f, 0.29f, 0.46f, 1.00f);
        colors[ImGuiCol_Border]               = ImVec4(0.24f, 0.55f, 0.74f, 1.00f);
        colors[ImGuiCol_BorderShadow]         = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]              = ImVec4(0.13f, 0.16f, 0.22f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.20f, 0.25f, 0.34f, 1.00f);
        colors[ImGuiCol_FrameBgActive]        = ImVec4(0.24f, 0.30f, 0.40f, 1.00f);
        colors[ImGuiCol_TitleBg]              = ImVec4(0.12f, 0.29f, 0.46f, 1.00f);
        colors[ImGuiCol_TitleBgActive]        = ImVec4(0.22f, 0.34f, 0.52f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.18f, 0.27f, 0.41f, 1.00f);
        colors[ImGuiCol_MenuBarBg]            = ImVec4(0.11f, 0.25f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.13f, 0.27f, 0.44f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.41f, 0.62f, 0.81f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.47f, 0.69f, 0.90f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.53f, 0.75f, 0.93f, 1.00f);
        colors[ImGuiCol_CheckMark]            = ImVec4(0.12f, 0.42f, 0.72f, 1.00f);
        colors[ImGuiCol_SliderGrab]           = ImVec4(0.34f, 0.65f, 0.90f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.43f, 0.76f, 0.98f, 1.00f);
        colors[ImGuiCol_Button]               = ImVec4(0.17f, 0.55f, 0.85f, 1.00f);
        colors[ImGuiCol_ButtonHovered]        = ImVec4(0.19f, 0.65f, 0.92f, 1.00f);
        colors[ImGuiCol_ButtonActive]         = ImVec4(0.21f, 0.72f, 0.97f, 1.00f);
        colors[ImGuiCol_Header]               = ImVec4(0.15f, 0.60f, 0.92f, 1.00f);
        colors[ImGuiCol_HeaderHovered]        = ImVec4(0.20f, 0.69f, 0.93f, 1.00f);
        colors[ImGuiCol_HeaderActive]         = ImVec4(0.25f, 0.71f, 0.96f, 1.00f);
        colors[ImGuiCol_Separator]            = ImVec4(0.34f, 0.54f, 0.79f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]     = ImVec4(0.40f, 0.62f, 0.88f, 1.00f);
        colors[ImGuiCol_SeparatorActive]      = ImVec4(0.46f, 0.68f, 0.92f, 1.00f);
        colors[ImGuiCol_ResizeGrip]           = ImVec4(0.42f, 0.61f, 0.86f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0.56f, 0.74f, 0.95f, 1.00f);
        colors[ImGuiCol_ResizeGripActive]     = ImVec4(0.70f, 0.85f, 0.99f, 1.00f);
        colors[ImGuiCol_PlotLines]            = ImVec4(0.20f, 0.50f, 0.85f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]     = ImVec4(0.30f, 0.60f, 0.95f, 1.00f);
        colors[ImGuiCol_PlotHistogram]        = ImVec4(0.22f, 0.48f, 0.88f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.32f, 0.60f, 0.92f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.20f, 0.56f, 0.92f, 0.20f);
        colors[ImGuiCol_DragDropTarget]       = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]         = ImVec4(0.10f, 0.10f, 0.10f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]    = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]     = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]        = ImVec4(0.15f, 0.15f, 0.15f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]    = ImVec4(0.22f, 0.22f, 0.22f, 0.75f);
        colors[ImGuiCol_TableBorderLight]     = ImVec4(0.20f, 0.20f, 0.20f, 0.55f);
        colors[ImGuiCol_TableRowBg]           = ImVec4(0.30f, 0.30f, 0.30f, 0.10f);
        colors[ImGuiCol_TableRowBgAlt]        = ImVec4(0.27f, 0.27f, 0.27f, 0.15f);
      } /* End of 'StyleColorsAdminPanel' function */

      /* Set DarkRust ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsDarkRust( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 4.0f;
        style->FrameRounding = 4.0f;
        style->PopupRounding = 4.0f;
        style->ScrollbarRounding = 3.0f;
        style->GrabRounding = 4.0f;
        style->FrameBorderSize = 1.5f;
        style->WindowBorderSize = 1.0f;

        colors[ImGuiCol_Text]                 = ImVec4(0.80f, 0.33f, 0.27f, 1.0f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.56f, 0.25f, 0.20f, 1.0f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.20f, 0.14f, 0.11f, 1.0f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.28f, 0.17f, 0.09f, 1.0f);
        colors[ImGuiCol_Border]                = ImVec4(0.46f, 0.26f, 0.18f, 1.0f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.40f, 0.23f, 0.14f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.53f, 0.30f, 0.17f, 1.0f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.61f, 0.34f, 0.20f, 1.0f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.28f, 0.18f, 0.14f, 1.0f);
        colors[ImGuiCol_TitleBgActive]         = ImVec4(0.38f, 0.25f, 0.18f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.33f, 0.22f, 0.16f, 1.0f);
        colors[ImGuiCol_MenuBarBg]             = ImVec4(0.29f, 0.18f, 0.12f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.24f, 0.14f, 0.09f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.50f, 0.25f, 0.15f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.62f, 0.30f, 0.20f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.72f, 0.36f, 0.24f, 1.0f);
        colors[ImGuiCol_CheckMark]             = ImVec4(0.88f, 0.36f, 0.26f, 1.0f);
        colors[ImGuiCol_SliderGrab]            = ImVec4(0.60f, 0.33f, 0.20f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.86f, 0.40f, 0.26f, 1.0f);
        colors[ImGuiCol_Button]                = ImVec4(0.70f, 0.35f, 0.23f, 1.0f);
        colors[ImGuiCol_ButtonHovered]         = ImVec4(0.80f, 0.42f, 0.28f, 1.0f);
        colors[ImGuiCol_ButtonActive]          = ImVec4(0.90f, 0.48f, 0.32f, 1.0f);
        colors[ImGuiCol_Header]                = ImVec4(0.77f, 0.41f, 0.30f, 1.0f);
        colors[ImGuiCol_HeaderHovered]         = ImVec4(0.86f, 0.48f, 0.34f, 1.0f);
        colors[ImGuiCol_HeaderActive]          = ImVec4(0.92f, 0.52f, 0.36f, 1.0f);
        colors[ImGuiCol_Separator]             = ImVec4(0.70f, 0.40f, 0.30f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.79f, 0.51f, 0.40f, 1.0f);
        colors[ImGuiCol_SeparatorActive]       = ImVec4(0.85f, 0.59f, 0.44f, 1.0f);
        colors[ImGuiCol_ResizeGrip]            = ImVec4(0.68f, 0.46f, 0.32f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.78f, 0.52f, 0.38f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.88f, 0.58f, 0.43f, 1.0f);
        colors[ImGuiCol_PlotLines]             = ImVec4(0.90f, 0.38f, 0.28f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.95f, 0.45f, 0.33f, 1.0f);
        colors[ImGuiCol_PlotHistogram]         = ImVec4(0.88f, 0.38f, 0.27f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.92f, 0.45f, 0.34f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.90f, 0.30f, 0.22f, 0.22f);
        colors[ImGuiCol_DragDropTarget]        = ImVec4(1.0f, 0.9f, 0.0f, 0.9f);
        colors[ImGuiCol_NavHighlight]          = ImVec4(0.25f, 0.11f, 0.04f, 0.06f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.40f, 0.31f, 0.10f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.70f, 0.53f, 0.16f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.60f, 0.40f, 0.10f, 0.45f);

        colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.43f, 0.16f, 0.09f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.56f, 0.23f, 0.12f, 0.75f);
        colors[ImGuiCol_TableBorderLight]      = ImVec4(0.53f, 0.20f, 0.08f, 0.65f);
        colors[ImGuiCol_TableRowBg]            = ImVec4(0.63f, 0.28f, 0.15f, 0.15f);
        colors[ImGuiCol_TableRowBgAlt]         = ImVec4(0.55f, 0.22f, 0.10f, 0.20f);
      } /* End of 'StyleColorsDarkRust' function */

      /* Set SteelGray ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsSteelGray( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 5.0f;
        style->FrameRounding = 5.0f;
        style->PopupRounding = 5.0f;
        style->ScrollbarRounding = 4.0f;
        style->GrabRounding = 5.0f;
        style->FrameBorderSize = 1.0f;
        style->WindowBorderSize = 1.0f;

        colors[ImGuiCol_Text]                 = ImVec4(0.80f, 0.82f, 0.85f, 1.0f);
        colors[ImGuiCol_TextDisabled]          = ImVec4(0.55f, 0.57f, 0.60f, 1.0f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.20f, 0.22f, 0.25f, 1.0f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.27f, 0.29f, 0.32f, 1.0f);
        colors[ImGuiCol_Border]                 = ImVec4(0.42f, 0.45f, 0.48f, 1.0f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.28f, 0.31f, 0.35f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.35f, 0.38f, 0.42f, 1.0f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.38f, 0.41f, 0.45f, 1.0f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.26f, 0.29f, 0.34f, 1.0f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.38f, 0.42f, 0.49f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.30f, 0.34f, 0.39f, 1.0f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.25f, 0.30f, 0.36f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.22f, 0.26f, 0.31f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.44f, 0.49f, 0.54f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.55f, 0.60f, 0.65f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.63f, 0.67f, 0.72f, 1.0f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.75f, 0.78f, 0.82f, 1.0f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.56f, 0.59f, 0.62f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.45f, 0.48f, 0.51f, 1.0f);
        colors[ImGuiCol_Button]                 = ImVec4(0.38f, 0.42f, 0.45f, 1.0f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.52f, 0.56f, 0.59f, 1.0f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.60f, 0.63f, 0.66f, 1.0f);
        colors[ImGuiCol_Header]                 = ImVec4(0.35f, 0.39f, 0.45f, 1.0f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.46f, 0.49f, 0.55f, 1.0f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.53f, 0.57f, 0.60f, 1.0f);
        colors[ImGuiCol_Separator]              = ImVec4(0.50f, 0.53f, 0.57f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.60f, 0.63f, 0.67f, 1.0f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.66f, 0.70f, 0.72f, 1.0f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.58f, 0.61f, 0.65f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.67f, 0.70f, 0.73f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.76f, 0.77f, 0.81f, 1.0f);
        colors[ImGuiCol_PlotLines]              = ImVec4(0.46f, 0.48f, 0.52f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.55f, 0.57f, 0.61f, 1.0f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(0.50f, 0.52f, 0.56f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.60f, 0.62f, 0.66f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.50f, 0.52f, 0.56f, 0.20f);
        colors[ImGuiCol_DragDropTarget]         = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
        colors[ImGuiCol_NavHighlight]           = ImVec4(0.09f, 0.09f, 0.09f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
        colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.8f, 0.8f, 0.8f, 0.22f);
        colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.2f, 0.2f, 0.2f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.55f, 0.55f, 0.55f, 0.9f);
        colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.45f, 0.45f, 0.45f, 0.75f);
        colors[ImGuiCol_TableBorderLight]       = ImVec4(0.40f, 0.40f, 0.40f, 0.55f);
        colors[ImGuiCol_TableRowBg]             = ImVec4(0.60f, 0.60f, 0.60f, 0.1f);
        colors[ImGuiCol_TableRowBgAlt]          = ImVec4(0.65f, 0.65f, 0.65f, 0.15f);
      } /* End of 'StyleColorsSteelGray' function */

      /* Set RetroPurple ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsRetroPurple( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;
  
        style->WindowRounding = 4.0f;
        style->FrameRounding = 0.0f;
        style->PopupRounding = 5.0f;
        style->ScrollbarRounding = 3.0f;
        style->GrabRounding = 4.0f;
        style->FrameBorderSize = 1.5f;
        style->WindowBorderSize = 1.0f;
  
        colors[ImGuiCol_Text]                  = ImVec4(0.85f, 0.80f, 0.95f, 1.0f);
        colors[ImGuiCol_TextDisabled]          = ImVec4(0.55f, 0.49f, 0.67f, 1.0f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.25f, 0.19f, 0.43f, 1.0f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.32f, 0.28f, 0.49f, 1.0f);
        colors[ImGuiCol_Border]                 = ImVec4(0.50f, 0.44f, 0.71f, 1.0f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.29f, 0.23f, 0.48f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.42f, 0.35f, 0.62f, 1.0f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.30f, 0.28f, 0.67f, 1.0f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.27f, 0.21f, 0.58f, 1.0f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.52f, 0.24f, 0.81f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.38f, 0.14f, 0.56f, 1.0f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.31f, 0.24f, 0.49f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.27f, 0.18f, 0.42f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.77f, 0.60f, 0.99f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.84f, 0.52f, 0.99f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.73f, 0.58f, 0.96f, 1.0f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.70f, 0.32f, 0.98f, 1.0f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.64f, 0.44f, 0.98f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.48f, 0.39f, 0.94f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.73f, 0.46f, 0.92f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.84f, 0.59f, 0.98f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.65f, 0.39f, 0.99f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.62f, 0.42f, 0.89f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.77f, 0.53f, 0.91f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.60f, 0.43f, 0.92f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.64f, 0.51f, 0.88f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.75f, 0.59f, 0.94f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.66f, 0.53f, 0.93f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.68f, 0.50f, 0.97f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.73f, 0.53f, 0.99f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.82f, 0.59f, 1.00f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.50f, 0.17f, 0.96f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.59f, 0.27f, 0.98f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.38f, 0.10f, 0.93f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.58f, 0.18f, 0.94f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.70f, 0.44f, 0.91f, 0.20f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 0.00f, 1.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.25f, 0.07f, 0.60f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(0.75f, 0.62f, 0.97f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.90f, 0.88f, 0.99f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.30f, 0.25f, 0.50f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.48f, 0.38f, 0.58f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.50f, 0.30f, 0.70f, 0.82f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.45f, 0.25f, 0.60f, 0.60f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.50f, 0.40f, 0.70f, 0.13f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.45f, 0.35f, 0.65f, 0.18f);
      } /* End of 'StyleColorsRetroPurple' function */

      /* Set Creamy ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsCreamy( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 10.0f;
        style->FrameRounding = 8.0f;
        style->PopupRounding = 12.0f;
        style->ScrollbarRounding = 8.0f;
        style->GrabRounding = 8.0f;
        style->FrameBorderSize = 0.0f;
        style->WindowBorderSize = 0.0f;

        colors[ImGuiCol_Text]                   = ImVec4(0.40f, 0.30f, 0.20f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.65f, 0.55f, 0.45f, 1.00f);
        colors[ImGuiCol_WindowBg]                = ImVec4(0.99f, 0.98f, 0.93f, 1.00f);
        colors[ImGuiCol_ChildBg]                 = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
        colors[ImGuiCol_PopupBg]                 = ImVec4(0.97f, 0.97f, 0.92f, 0.95f);
        colors[ImGuiCol_Border]                  = ImVec4(0.85f, 0.80f, 0.73f, 1.00f);
        colors[ImGuiCol_BorderShadow]            = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]                 = ImVec4(0.98f, 0.95f, 0.92f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]          = ImVec4(0.95f, 0.90f, 0.80f, 1.00f);
        colors[ImGuiCol_FrameBgActive]           = ImVec4(0.90f, 0.85f, 0.70f, 1.00f);
        colors[ImGuiCol_TitleBg]                 = ImVec4(0.85f, 0.78f, 0.65f, 1.00f);
        colors[ImGuiCol_TitleBgActive]           = ImVec4(0.78f, 0.68f, 0.50f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]        = ImVec4(0.72f, 0.60f, 0.48f, 1.00f);
        colors[ImGuiCol_MenuBarBg]               = ImVec4(0.95f, 0.90f, 0.78f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]             = ImVec4(0.95f, 0.90f, 0.78f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]           = ImVec4(0.80f, 0.70f, 0.45f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]    = ImVec4(0.90f, 0.80f, 0.45f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]     = ImVec4(0.95f, 0.90f, 0.70f, 1.00f);
        colors[ImGuiCol_CheckMark]               = ImVec4(0.60f, 0.50f, 0.30f, 1.00f);
        colors[ImGuiCol_SliderGrab]              = ImVec4(0.80f, 0.70f, 0.40f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]        = ImVec4(0.85f, 0.75f, 0.50f, 1.00f);
        colors[ImGuiCol_Button]                  = ImVec4(0.90f, 0.85f, 0.60f, 1.00f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.95f, 0.90f, 0.65f, 1.00f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.85f, 0.75f, 0.50f, 1.00f);
        colors[ImGuiCol_Header]                  = ImVec4(0.82f, 0.70f, 0.48f, 1.00f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.78f, 0.65f, 0.40f, 1.00f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.72f, 0.60f, 0.30f, 1.00f);
        colors[ImGuiCol_Separator]               = ImVec4(0.75f, 0.65f, 0.45f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.80f, 0.75f, 0.55f, 1.00f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.75f, 0.65f, 0.45f, 1.00f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.75f, 0.65f, 0.45f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.80f, 0.75f, 0.55f, 1.00f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.90f, 0.85f, 0.65f, 1.00f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.62f, 0.45f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.65f, 0.55f, 0.25f, 1.00f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.60f, 0.48f, 0.23f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.65f, 0.55f, 0.25f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.65f, 0.55f, 0.25f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.10f, 0.10f, 0.10f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.30f, 0.30f, 0.30f, 0.50f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.88f, 0.82f, 0.70f, 0.90f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.80f, 0.75f, 0.64f, 0.75f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.78f, 0.70f, 0.61f, 0.58f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.96f, 0.90f, 0.72f, 0.15f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.90f, 0.85f, 0.65f, 0.20f);
      } /* End of 'StyleColorsCreamy' function */

      /* Set AcidGreen ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsAcidGreen( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 1.0f;
        style->FrameRounding = 1.0f;
        style->PopupRounding = 2.0f;
        style->ScrollbarRounding = 2.0f;
        style->GrabRounding = 1.0f;
        style->FrameBorderSize = 2.0f;
        style->WindowBorderSize = 2.0f;

        colors[ImGuiCol_Text]                   = ImVec4(0.85f, 1.00f, 0.73f, 1.0f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.45f, 0.75f, 0.45f, 1.0f);
        colors[ImGuiCol_WindowBg]                = ImVec4(0.15f, 0.30f, 0.15f, 1.0f);
        colors[ImGuiCol_ChildBg]                 = ImVec4(0.12f, 0.25f, 0.12f, 1.0f);
        colors[ImGuiCol_PopupBg]                 = ImVec4(0.10f, 0.22f, 0.10f, 1.0f);
        colors[ImGuiCol_Border]                  = ImVec4(0.30f, 0.65f, 0.30f, 1.0f);
        colors[ImGuiCol_BorderShadow]            = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]                 = ImVec4(0.18f, 0.35f, 0.18f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]          = ImVec4(0.30f, 0.55f, 0.30f, 1.0f);
        colors[ImGuiCol_FrameBgActive]           = ImVec4(0.45f, 0.65f, 0.45f, 1.0f);
        colors[ImGuiCol_TitleBg]                 = ImVec4(0.22f, 0.45f, 0.22f, 1.0f);
        colors[ImGuiCol_TitleBgActive]           = ImVec4(0.32f, 0.58f, 0.32f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]        = ImVec4(0.25f, 0.50f, 0.25f, 1.0f);
        colors[ImGuiCol_MenuBarBg]               = ImVec4(0.20f, 0.40f, 0.20f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]             = ImVec4(0.18f, 0.38f, 0.18f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]           = ImVec4(0.35f, 0.55f, 0.35f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]    = ImVec4(0.45f, 0.65f, 0.45f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]     = ImVec4(0.52f, 0.72f, 0.52f, 1.0f);
        colors[ImGuiCol_CheckMark]               = ImVec4(0.20f, 0.80f, 0.20f, 1.0f);
        colors[ImGuiCol_SliderGrab]              = ImVec4(0.55f, 0.95f, 0.55f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]        = ImVec4(0.40f, 0.90f, 0.40f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.25f, 0.70f, 0.25f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.30f, 0.85f, 0.30f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.15f, 0.80f, 0.15f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.22f, 0.70f, 0.22f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.25f, 0.80f, 0.25f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.30f, 0.90f, 0.30f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.45f, 0.75f, 0.45f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.60f, 0.85f, 0.60f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.55f, 0.90f, 0.55f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.55f, 0.85f, 0.55f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.60f, 0.90f, 0.60f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.70f, 0.95f, 0.70f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.30f, 0.65f, 0.30f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.40f, 0.75f, 0.40f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.28f, 0.60f, 0.28f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.38f, 0.70f, 0.38f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.28f, 0.75f, 0.28f, 0.20f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.05f, 0.30f, 0.05f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(0.30f, 1.00f, 0.30f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.60f, 1.00f, 0.60f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.20f, 0.60f, 0.20f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.40f, 0.70f, 0.20f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.60f, 0.80f, 0.32f, 0.75f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.50f, 0.70f, 0.20f, 0.60f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.68f, 0.95f, 0.20f, 0.12f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.58f, 0.80f, 0.20f, 0.17f);
      } /* End of 'StyleColorsAcidGreen' function */

      // 10 стилей приведены, при необходимости дополню остальными.
      /* Set SunnyDay ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsSunnyDay( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 10.0f;
        style->FrameRounding = 10.0f;
        style->PopupRounding = 10.0f;
        style->ScrollbarRounding = 6.0f;
        style->GrabRounding = 6.0f;
        style->FrameBorderSize = 0.0f;
        style->WindowBorderSize = 0.0f;

        colors[ImGuiCol_Text]                   = ImVec4(0.56f, 0.43f, 0.16f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.62f, 0.55f, 0.41f, 1.00f);
        colors[ImGuiCol_WindowBg]                = ImVec4(1.00f, 0.99f, 0.92f, 1.00f);
        colors[ImGuiCol_ChildBg]                 = ImVec4(1.00f, 1.00f, 0.95f, 0.75f);
        colors[ImGuiCol_PopupBg]                 = ImVec4(0.99f, 0.95f, 0.80f, 1.00f);
        colors[ImGuiCol_Border]                  = ImVec4(0.99f, 0.91f, 0.54f, 1.00f);
        colors[ImGuiCol_BorderShadow]            = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]                 = ImVec4(0.99f, 0.96f, 0.70f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]          = ImVec4(0.98f, 0.87f, 0.26f, 1.00f);
        colors[ImGuiCol_FrameBgActive]           = ImVec4(0.98f, 0.78f, 0.12f, 1.00f);
        colors[ImGuiCol_TitleBg]                 = ImVec4(0.98f, 0.86f, 0.35f, 1.00f);
        colors[ImGuiCol_TitleBgActive]           = ImVec4(0.99f, 0.81f, 0.20f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]        = ImVec4(0.99f, 0.76f, 0.35f, 1.00f);
        colors[ImGuiCol_MenuBarBg]               = ImVec4(0.99f, 0.93f, 0.55f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]             = ImVec4(1.00f, 0.88f, 0.38f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]           = ImVec4(0.99f, 0.81f, 0.20f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]    = ImVec4(0.99f, 0.86f, 0.26f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]     = ImVec4(0.99f, 0.91f, 0.35f, 1.00f);
        colors[ImGuiCol_CheckMark]               = ImVec4(0.99f, 0.79f, 0.39f, 1.00f);
        colors[ImGuiCol_SliderGrab]              = ImVec4(0.95f, 0.72f, 0.12f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]        = ImVec4(0.98f, 0.68f, 0.11f, 1.00f);
        colors[ImGuiCol_Button]                  = ImVec4(0.99f, 0.85f, 0.31f, 1.00f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.99f, 0.73f, 0.12f, 1.00f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.99f, 0.74f, 0.18f, 1.00f);
        colors[ImGuiCol_Header]                  = ImVec4(0.99f, 0.78f, 0.19f, 1.00f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.99f, 0.73f, 0.12f, 1.00f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.99f, 0.68f, 0.12f, 1.00f);
        colors[ImGuiCol_Separator]               = ImVec4(0.99f, 0.79f, 0.11f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(1.00f, 0.88f, 0.24f, 1.00f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(1.00f, 0.91f, 0.36f, 1.00f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.99f, 0.89f, 0.53f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(1.00f, 0.92f, 0.75f, 1.00f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(1.00f, 0.90f, 0.55f, 1.00f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.99f, 0.90f, 0.55f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(1.00f, 0.94f, 0.70f, 1.00f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.98f, 0.83f, 0.36f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.99f, 0.88f, 0.44f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.98f, 0.77f, 0.22f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.10f, 0.10f, 0.08f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.75f, 0.61f, 0.15f, 0.25f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(1.00f, 0.84f, 0.45f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.98f, 0.75f, 0.18f, 0.75f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.85f, 0.63f, 0.15f, 0.55f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(1.00f, 0.91f, 0.58f, 0.15f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(1.00f, 0.81f, 0.47f, 0.20f);
      } /* End of 'StyleColorsSunnyDay' function */

      /* Set SunLight ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsSunLight( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 15.0f;
        style->FrameRounding = 15.0f;
        style->PopupRounding = 12.0f;
        style->ScrollbarRounding = 12.0f;
        style->GrabRounding = 15.0f;
        style->FrameBorderSize = 0.0f;
        style->WindowBorderSize = 0.0f;

        colors[ImGuiCol_Text]                   = ImVec4(0.54f, 0.41f, 0.16f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.62f, 0.52f, 0.27f, 1.00f);
        colors[ImGuiCol_WindowBg]                = ImVec4(1.00f, 0.99f, 0.92f, 1.00f);
        colors[ImGuiCol_ChildBg]                 = ImVec4(0.99f, 0.98f, 0.92f, 0.95f);
        colors[ImGuiCol_PopupBg]                 = ImVec4(0.98f, 0.95f, 0.90f, 1.0f);
        colors[ImGuiCol_Border]                  = ImVec4(0.90f, 0.78f, 0.30f, 1.0f);
        colors[ImGuiCol_BorderShadow]            = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]                 = ImVec4(0.98f, 0.93f, 0.59f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]          = ImVec4(0.99f, 0.88f, 0.40f, 1.0f);
        colors[ImGuiCol_FrameBgActive]           = ImVec4(0.98f, 0.83f, 0.20f, 1.0f);
        colors[ImGuiCol_TitleBg]                 = ImVec4(0.98f, 0.85f, 0.30f, 1.0f);
        colors[ImGuiCol_TitleBgActive]           = ImVec4(0.99f, 0.77f, 0.29f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]        = ImVec4(0.94f, 0.70f, 0.25f, 1.0f);
        colors[ImGuiCol_MenuBarBg]               = ImVec4(0.99f, 0.92f, 0.60f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]             = ImVec4(0.96f, 0.88f, 0.48f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]           = ImVec4(0.99f, 0.91f, 0.58f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]    = ImVec4(0.99f, 0.88f, 0.48f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]     = ImVec4(0.99f, 0.94f, 0.70f, 1.0f);
        colors[ImGuiCol_CheckMark]               = ImVec4(0.99f, 0.88f, 0.40f, 1.0f);
        colors[ImGuiCol_SliderGrab]              = ImVec4(0.98f, 0.88f, 0.30f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]        = ImVec4(0.99f, 0.82f, 0.18f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.99f, 0.88f, 0.30f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.99f, 0.83f, 0.20f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.99f, 0.80f, 0.15f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.99f, 0.86f, 0.32f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.99f, 0.82f, 0.20f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.99f, 0.75f, 0.09f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.99f, 0.77f, 0.20f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(1.00f, 0.86f, 0.23f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(1.00f, 0.92f, 0.29f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.99f, 0.91f, 0.38f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(1.00f, 0.96f, 0.58f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(1.00f, 0.94f, 0.52f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.99f, 0.89f, 0.38f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(1.00f, 0.91f, 0.44f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.98f, 0.87f, 0.36f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.99f, 0.90f, 0.42f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.99f, 0.91f, 0.44f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.10f, 0.10f, 0.10f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.80f, 0.65f, 0.22f, 0.35f);
      } /* End of 'StyleColorsSunnyDay' function */

      /* Set DarkViolet ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsDarkViolet( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;
  
        style->WindowRounding = 8.0f;
        style->FrameRounding = 6.0f;
        style->PopupRounding = 9.0f;
        style->ScrollbarRounding = 6.0f;
        style->GrabRounding = 6.0f;
        style->FrameBorderSize = 1.0f;
        style->WindowBorderSize = 1.0f;
  
        colors[ImGuiCol_Text]                = ImVec4(0.75f, 0.54f, 0.94f, 1.0f);
        colors[ImGuiCol_TextDisabled]        = ImVec4(0.47f, 0.37f, 0.62f, 1.0f);
        colors[ImGuiCol_WindowBg]             = ImVec4(0.12f, 0.05f, 0.35f, 1.0f);
        colors[ImGuiCol_ChildBg]              = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]              = ImVec4(0.15f, 0.07f, 0.38f, 1.0f);
        colors[ImGuiCol_Border]               = ImVec4(0.40f, 0.26f, 0.60f, 1.0f);
        colors[ImGuiCol_BorderShadow]         = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]              = ImVec4(0.19f, 0.09f, 0.49f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.29f, 0.15f, 0.65f, 1.0f);
        colors[ImGuiCol_FrameBgActive]        = ImVec4(0.34f, 0.19f, 0.75f, 1.0f);
        colors[ImGuiCol_TitleBg]              = ImVec4(0.25f, 0.10f, 0.55f, 1.0f);
        colors[ImGuiCol_TitleBgActive]        = ImVec4(0.42f, 0.18f, 0.77f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.40f, 0.16f, 0.70f, 1.0f);
        colors[ImGuiCol_MenuBarBg]            = ImVec4(0.27f, 0.10f, 0.58f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.22f, 0.08f, 0.50f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.53f, 0.29f, 0.94f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.62f, 0.33f, 0.99f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.67f, 0.35f, 1.00f, 1.0f);
        colors[ImGuiCol_CheckMark]            = ImVec4(0.52f, 0.20f, 0.88f, 1.0f);
        colors[ImGuiCol_SliderGrab]           = ImVec4(0.60f, 0.40f, 0.90f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.72f, 0.55f, 0.96f, 1.0f);
        colors[ImGuiCol_Button]               = ImVec4(0.58f, 0.42f, 0.89f, 1.0f);
        colors[ImGuiCol_ButtonHovered]        = ImVec4(0.65f, 0.49f, 0.92f, 1.0f);
        colors[ImGuiCol_ButtonActive]         = ImVec4(0.54f, 0.38f, 0.82f, 1.0f);
        colors[ImGuiCol_Header]               = ImVec4(0.67f, 0.45f, 0.96f, 1.0f);
        colors[ImGuiCol_HeaderHovered]        = ImVec4(0.74f, 0.54f, 0.98f, 1.0f);
        colors[ImGuiCol_HeaderActive]         = ImVec4(0.66f, 0.44f, 0.88f, 1.0f);
        colors[ImGuiCol_Separator]            = ImVec4(0.55f, 0.33f, 0.85f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]     = ImVec4(0.62f, 0.39f, 0.87f, 1.0f);
        colors[ImGuiCol_SeparatorActive]      = ImVec4(0.58f, 0.35f, 0.82f, 1.0f);
        colors[ImGuiCol_ResizeGrip]           = ImVec4(0.62f, 0.45f, 0.92f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0.68f, 0.52f, 0.94f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]     = ImVec4(0.72f, 0.54f, 0.96f, 1.0f);
        colors[ImGuiCol_PlotLines]            = ImVec4(0.67f, 0.45f, 0.87f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]     = ImVec4(0.72f, 0.58f, 0.95f, 1.0f);
        colors[ImGuiCol_PlotHistogram]        = ImVec4(0.64f, 0.40f, 0.88f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.70f, 0.48f, 0.94f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.67f, 0.44f, 0.93f, 0.25f);
        colors[ImGuiCol_DragDropTarget]       = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]         = ImVec4(0.20f, 0.16f, 0.27f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.75f, 0.60f, 0.98f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]    = ImVec4(0.58f, 0.44f, 0.94f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]     = ImVec4(0.34f, 0.28f, 0.59f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]        = ImVec4(0.45f, 0.36f, 0.68f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]    = ImVec4(0.33f, 0.22f, 0.55f, 0.70f);
        colors[ImGuiCol_TableBorderLight]     = ImVec4(0.30f, 0.25f, 0.52f, 0.48f);
        colors[ImGuiCol_TableRowBg]           = ImVec4(0.37f, 0.28f, 0.51f, 0.12f);
        colors[ImGuiCol_TableRowBgAlt]        = ImVec4(0.30f, 0.23f, 0.47f, 0.18f);
      } /* End of 'StyleColorsDarkViolet' function */

      /* Set SoftBlue ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsSoftBlue( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding   = 3.0f;
        style->FrameRounding    = 3.0f;
        style->PopupRounding    = 5.0f;
        style->ScrollbarRounding= 2.0f;
        style->GrabRounding     = 3.0f;
        style->FrameBorderSize  = 1.0f;
        style->WindowBorderSize = 1.0f;

        colors[ImGuiCol_Text]                  = ImVec4(0.10f, 0.22f, 0.42f, 1.0f);
        colors[ImGuiCol_TextDisabled]          = ImVec4(0.45f, 0.52f, 0.60f, 1.0f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.93f, 0.95f, 1.00f, 1.0f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.96f, 0.97f, 1.00f, 1.0f);
        colors[ImGuiCol_Border]                 = ImVec4(0.54f, 0.62f, 0.75f, 1.0f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.89f, 0.91f, 0.96f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.80f, 0.83f, 0.92f, 1.0f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.72f, 0.77f, 0.86f, 1.0f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.60f, 0.70f, 0.89f, 1.0f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.46f, 0.55f, 0.77f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.40f, 0.52f, 0.70f, 1.0f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.82f, 0.87f, 0.97f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.83f, 0.87f, 0.96f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.52f, 0.59f, 0.75f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.57f, 0.63f, 0.81f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.63f, 0.68f, 0.86f, 1.0f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.12f, 0.28f, 0.58f, 1.0f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.38f, 0.50f, 0.73f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.28f, 0.42f, 0.63f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.65f, 0.74f, 0.96f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.54f, 0.63f, 0.89f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.39f, 0.50f, 0.75f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.54f, 0.62f, 0.88f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.47f, 0.55f, 0.79f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.46f, 0.52f, 0.74f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.48f, 0.54f, 0.74f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.56f, 0.61f, 0.82f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.58f, 0.66f, 0.84f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.52f, 0.60f, 0.82f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.63f, 0.72f, 0.92f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.70f, 0.77f, 0.96f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.33f, 0.44f, 0.59f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.41f, 0.57f, 0.74f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.37f, 0.50f, 0.66f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.40f, 0.53f, 0.68f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.37f, 0.54f, 0.78f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.07f, 0.07f, 0.07f, 0.05f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.60f, 0.60f, 0.60f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.15f, 0.15f, 0.15f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.60f, 0.74f, 0.88f, 0.90f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.45f, 0.60f, 0.78f, 0.75f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.50f, 0.65f, 0.81f, 0.55f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.77f, 0.85f, 0.95f, 0.10f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.70f, 0.80f, 0.90f, 0.15f);
      } /* End of 'StyleColorsSoftBlue' function */

      /* Set RoundedWarm ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsRoundedWarm( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        // Скругления
        style->WindowRounding    = 12.0f;
        style->FrameRounding     = 10.0f;
        style->PopupRounding     = 10.0f;
        style->ScrollbarRounding = 8.0f;
        style->GrabRounding      = 8.0f;
        style->FrameBorderSize   = 1.5f;
        style->WindowBorderSize  = 1.0f;

        // Тёплые мягкие оттенки
        colors[ImGuiCol_Text]                 = ImVec4(0.45f, 0.35f, 0.20f, 1.0f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.60f, 0.55f, 0.40f, 1.0f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.98f, 0.95f, 0.90f, 1.0f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.99f, 0.97f, 0.93f, 1.0f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.95f, 0.90f, 0.80f, 1.0f);
        colors[ImGuiCol_Border]                = ImVec4(0.85f, 0.70f, 0.50f, 1.0f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.90f, 0.83f, 0.70f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.92f, 0.75f, 0.44f, 1.0f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.95f, 0.65f, 0.30f, 1.0f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.91f, 0.75f, 0.40f, 1.0f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.82f, 0.67f, 0.33f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.77f, 0.62f, 0.30f, 1.0f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.90f, 0.70f, 0.25f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.90f, 0.65f, 0.22f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.82f, 0.58f, 0.16f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.90f, 0.68f, 0.22f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.95f, 0.75f, 0.27f, 1.0f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.90f, 0.62f, 0.15f, 1.0f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.92f, 0.67f, 0.20f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.90f, 0.55f, 0.18f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.86f, 0.68f, 0.30f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.90f, 0.70f, 0.35f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.92f, 0.66f, 0.29f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.85f, 0.64f, 0.26f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.89f, 0.73f, 0.35f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.90f, 0.74f, 0.34f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.70f, 0.48f, 0.19f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.82f, 0.58f, 0.26f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.84f, 0.59f, 0.22f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.85f, 0.60f, 0.25f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.90f, 0.70f, 0.32f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.92f, 0.74f, 0.32f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.88f, 0.58f, 0.25f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.92f, 0.64f, 0.31f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.90f, 0.60f, 0.28f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.92f, 0.66f, 0.32f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.90f, 0.62f, 0.20f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 0.88f, 0.11f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.20f, 0.16f, 0.09f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(0.92f, 0.61f, 0.10f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.65f, 0.43f, 0.12f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.40f, 0.32f, 0.14f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(1.00f, 0.94f, 0.73f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.95f, 0.84f, 0.54f, 0.75f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.90f, 0.79f, 0.45f, 0.60f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(1.00f, 0.90f, 0.62f, 0.15f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(1.00f, 0.82f, 0.51f, 0.20f);
      } /* End of 'StyleColorsRoundedWarm' function */

      /* Set HighContrast ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsHighContrast( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding = 0.0f;
        style->FrameRounding = 0.0f;
        style->PopupRounding = 0.0f;
        style->ScrollbarRounding = 0.0f;
        style->GrabRounding = 0.0f;
        style->FrameBorderSize = 2.0f;
        style->WindowBorderSize = 2.0f;

        colors[ImGuiCol_Text]                  = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        colors[ImGuiCol_TextDisabled]          = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
        colors[ImGuiCol_Border]                 = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.9f, 0.9f, 0.0f, 1.0f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(1.0f, 0.95f, 0.0f, 1.0f);
        colors[ImGuiCol_Button]                 = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.95f, 0.95f, 0.2f, 1.0f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(1.0f, 0.90f, 0.0f, 1.0f);
        colors[ImGuiCol_Header]                 = ImVec4(1.0f, 1.0f, 0.2f, 1.0f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(1.0f, 1.0f, 0.5f, 1.0f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(1.0f, 1.0f, 0.7f, 1.0f);
        colors[ImGuiCol_Separator]              = ImVec4(1.0f, 1.0f, 0.2f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(1.0f, 1.0f, 0.6f, 1.0f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.7f, 0.7f, 0.0f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.9f, 0.9f, 0.1f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        colors[ImGuiCol_PlotLines]              = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.0f, 1.0f, 0.2f, 1.0f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(1.0f, 1.0f, 0.4f, 0.25f);
        colors[ImGuiCol_DragDropTarget]         = ImVec4(1.0f, 1.0f, 0.0f, 0.90f);
        colors[ImGuiCol_NavHighlight]           = ImVec4(1.0f, 1.0f, 0.2f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.0f, 1.0f, 1.0f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.30f, 0.30f, 0.00f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]          = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
        colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TableBorderLight]       = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_TableRowBg]             = ImVec4(0.85f, 0.85f, 0.85f, 0.15f);
        colors[ImGuiCol_TableRowBgAlt]          = ImVec4(0.75f, 0.75f, 0.75f, 0.18f);
      } /* End of 'StyleColorsHighContrast' function */

      /* Set StyleColorsNeonBlue ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsNeonBlue( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding    = 4.0f;
        style->FrameRounding     = 3.0f;
        style->PopupRounding     = 5.0f;
        style->ScrollbarRounding = 6.0f;
        style->GrabRounding      = 5.0f;
        style->FrameBorderSize   = 1.0f;
        style->WindowBorderSize  = 1.0f;

        colors[ImGuiCol_Text]                 = ImVec4(0.48f, 0.82f, 1.00f, 1.0f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.25f, 0.45f, 0.65f, 1.0f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.05f, 0.10f, 0.18f, 1.0f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.03f, 0.10f, 0.16f, 1.0f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.07f, 0.12f, 0.20f, 1.0f);
        colors[ImGuiCol_Border]                = ImVec4(0.25f, 0.70f, 0.92f, 0.8f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.10f, 0.22f, 0.33f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.24f, 0.68f, 1.00f, 1.0f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.40f, 0.85f, 1.00f, 1.0f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.10f, 0.22f, 0.38f, 1.0f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.20f, 0.50f, 0.90f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.12f, 0.28f, 0.43f, 1.0f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.08f, 0.20f, 0.35f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.06f, 0.18f, 0.30f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.22f, 0.60f, 1.00f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.33f, 0.75f, 1.00f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.50f, 0.85f, 1.00f, 1.0f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.30f, 0.70f, 1.00f, 1.0f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.28f, 0.65f, 0.98f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.40f, 0.82f, 1.00f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.15f, 0.50f, 0.90f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.25f, 0.68f, 1.00f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.30f, 0.70f, 1.00f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.20f, 0.60f, 1.00f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.35f, 0.75f, 1.00f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.38f, 0.78f, 1.00f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.26f, 0.57f, 0.85f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.32f, 0.67f, 1.00f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.39f, 0.70f, 1.00f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.35f, 0.75f, 1.00f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.40f, 0.81f, 1.00f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.50f, 0.90f, 1.00f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.30f, 0.60f, 0.90f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.42f, 0.74f, 0.95f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.30f, 0.58f, 0.89f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.40f, 0.70f, 0.95f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.25f, 0.60f, 0.90f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 1.00f, 0.10f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.10f, 0.25f, 0.50f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(0.25f, 0.60f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.20f, 0.50f, 0.90f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.10f, 0.25f, 0.50f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.18f, 0.30f, 0.52f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.22f, 0.42f, 0.75f, 0.90f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.15f, 0.29f, 0.54f, 0.70f);
        colors[ImGuiCol_TableRowBg]               = ImVec4(0.15f, 0.33f, 0.85f, 0.20f);
        colors[ImGuiCol_TableRowBgAlt]            = ImVec4(0.12f, 0.25f, 0.67f, 0.10f);
      } /* End of 'StyleColorsNeonBlue' function */

      /* Set StyleColorsNeonGreen ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsNeonGreen( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding    = 4.0f;
        style->FrameRounding     = 5.0f;
        style->PopupRounding     = 6.0f;
        style->ScrollbarRounding = 7.0f;
        style->GrabRounding      = 6.0f;
        style->FrameBorderSize   = 1.2f;
        style->WindowBorderSize  = 1.0f;

        colors[ImGuiCol_Text]                 = ImVec4(0.50f, 1.00f, 0.50f, 1.0f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.30f, 0.70f, 0.30f, 1.0f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.05f, 0.15f, 0.04f, 1.0f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.04f, 0.12f, 0.03f, 1.0f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.06f, 0.17f, 0.05f, 1.0f);
        colors[ImGuiCol_Border]                = ImVec4(0.20f, 0.75f, 0.20f, 0.9f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.10f, 0.40f, 0.10f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.25f, 0.85f, 0.25f, 1.0f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.38f, 1.00f, 0.38f, 1.0f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.15f, 0.38f, 0.15f, 1.0f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.30f, 0.88f, 0.30f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.18f, 0.47f, 0.18f, 1.0f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.33f, 0.14f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.12f, 0.30f, 0.11f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.25f, 0.70f, 0.25f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.38f, 0.80f, 0.38f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.46f, 0.85f, 0.46f, 1.0f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.30f, 1.00f, 0.30f, 1.0f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.38f, 1.00f, 0.38f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.50f, 1.00f, 0.50f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.25f, 0.80f, 0.25f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.38f, 0.90f, 0.38f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.18f, 0.70f, 0.18f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.30f, 0.90f, 0.30f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.40f, 1.00f, 0.40f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.35f, 0.80f, 0.35f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.30f, 0.75f, 0.30f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.40f, 0.85f, 0.40f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.38f, 0.80f, 0.38f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.42f, 0.85f, 0.42f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.50f, 0.95f, 0.50f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.60f, 1.00f, 0.60f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.30f, 1.00f, 0.30f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.40f, 1.00f, 0.40f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.30f, 0.95f, 0.30f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.40f, 0.98f, 0.40f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.30f, 1.00f, 0.30f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(0.85f, 0.95f, 0.10f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.20f, 0.75f, 0.20f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(0.60f, 1.00f, 0.60f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.30f, 0.70f, 0.30f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.15f, 0.55f, 0.15f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.20f, 0.60f, 0.20f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.15f, 0.45f, 0.15f, 0.75f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.18f, 0.48f, 0.18f, 0.55f);
        colors[ImGuiCol_TableRowBg]               = ImVec4(0.18f, 0.50f, 0.18f, 0.10f);
        colors[ImGuiCol_TableRowBgAlt]            = ImVec4(0.20f, 0.60f, 0.20f, 0.15f);
      } /* End of 'StyleColorsNeonGreen' function */

      /* Set StyleColorsNeonRed ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsNeonRed( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding    = 5.0f;
        style->FrameRounding     = 5.0f;
        style->PopupRounding     = 7.0f;
        style->ScrollbarRounding = 6.0f;
        style->GrabRounding      = 5.0f;
        style->FrameBorderSize   = 1.2f;
        style->WindowBorderSize  = 1.0f;

        colors[ImGuiCol_Text]                 = ImVec4(1.00f, 0.24f, 0.24f, 1.00f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.70f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.12f, 0.03f, 0.03f, 1.00f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.15f, 0.03f, 0.03f, 1.00f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.20f, 0.05f, 0.05f, 1.00f);
        colors[ImGuiCol_Border]                = ImVec4(0.80f, 0.20f, 0.20f, 0.9f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.30f, 0.06f, 0.06f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.80f, 0.12f, 0.12f, 1.00f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.90f, 0.18f, 0.15f, 1.00f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.65f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.80f, 0.25f, 0.25f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.55f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.45f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.40f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.90f, 0.25f, 0.25f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(1.00f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(1.00f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(1.00f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(1.00f, 0.23f, 0.23f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(1.00f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_Button]                  = ImVec4(0.85f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(1.00f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.95f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_Header]                  = ImVec4(0.95f, 0.24f, 0.24f, 1.00f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(1.00f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(1.00f, 0.35f, 0.35f, 1.00f);
        colors[ImGuiCol_Separator]               = ImVec4(0.75f, 0.22f, 0.22f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(1.00f, 0.25f, 0.25f, 1.00f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(1.00f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.80f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.88f, 0.28f, 0.28f, 1.00f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.90f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_PlotLines]               = ImVec4(1.00f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(1.00f, 0.45f, 0.45f, 1.00f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.90f, 0.38f, 0.38f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(1.00f, 0.46f, 0.46f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(1.00f, 0.40f, 0.40f, 0.30f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 0.20f, 0.20f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.60f, 0.22f, 0.22f, 0.10f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(1.00f, 0.28f, 0.28f, 0.80f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.85f, 0.20f, 0.20f, 0.30f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.60f, 0.25f, 0.25f, 0.50f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.75f, 0.10f, 0.10f, 0.90f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.90f, 0.25f, 0.25f, 0.80f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.80f, 0.10f, 0.10f, 0.50f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.85f, 0.28f, 0.28f, 0.15f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.65f, 0.20f, 0.20f, 0.20f);
      } /* End of 'StyleColorsNeonRed' function */

      /* Set StyleColorsNeonTurquoise ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsNeonTurquoise( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding    = 5.0f;
        style->FrameRounding     = 4.0f;
        style->PopupRounding     = 6.0f;
        style->ScrollbarRounding = 7.0f;
        style->GrabRounding      = 5.0f;
        style->FrameBorderSize   = 1.0f;
        style->WindowBorderSize  = 1.0f;

        colors[ImGuiCol_Text]                   = ImVec4(0.00f, 0.80f, 0.80f, 1.0f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.20f, 0.50f, 0.55f, 1.0f);
        colors[ImGuiCol_WindowBg]                = ImVec4(0.02f, 0.20f, 0.22f, 1.0f);
        colors[ImGuiCol_ChildBg]                 = ImVec4(0.01f, 0.13f, 0.15f, 1.0f);
        colors[ImGuiCol_PopupBg]                 = ImVec4(0.03f, 0.18f, 0.20f, 1.0f);
        colors[ImGuiCol_Border]                  = ImVec4(0.00f, 0.75f, 0.72f, 0.85f);
        colors[ImGuiCol_BorderShadow]            = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]                 = ImVec4(0.05f, 0.48f, 0.50f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]          = ImVec4(0.16f, 0.76f, 0.72f, 1.0f);
        colors[ImGuiCol_FrameBgActive]           = ImVec4(0.25f, 0.90f, 0.85f, 1.0f);
        colors[ImGuiCol_TitleBg]                 = ImVec4(0.10f, 0.45f, 0.50f, 1.0f);
        colors[ImGuiCol_TitleBgActive]           = ImVec4(0.33f, 0.83f, 0.80f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]        = ImVec4(0.15f, 0.40f, 0.48f, 1.0f);
        colors[ImGuiCol_MenuBarBg]               = ImVec4(0.18f, 0.58f, 0.60f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]             = ImVec4(0.06f, 0.28f, 0.30f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]           = ImVec4(0.27f, 0.80f, 0.75f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]    = ImVec4(0.40f, 0.90f, 0.85f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]     = ImVec4(0.60f, 0.95f, 0.90f, 1.0f);
        colors[ImGuiCol_CheckMark]               = ImVec4(0.20f, 0.90f, 0.90f, 1.0f);
        colors[ImGuiCol_SliderGrab]              = ImVec4(0.25f, 0.88f, 0.85f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]        = ImVec4(0.38f, 0.93f, 0.90f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.20f, 0.75f, 0.70f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.32f, 0.90f, 0.88f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.22f, 0.80f, 0.75f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.22f, 0.75f, 0.70f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.35f, 0.88f, 0.82f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.30f, 0.85f, 0.80f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.30f, 0.80f, 0.75f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.40f, 0.92f, 0.88f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.35f, 0.90f, 0.85f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.38f, 0.85f, 0.80f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.45f, 0.93f, 0.90f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.55f, 0.98f, 0.95f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.30f, 0.85f, 0.98f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.38f, 0.88f, 1.00f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.25f, 0.80f, 0.83f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.32f, 0.90f, 0.88f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.20f, 0.85f, 0.87f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(0.10f, 1.00f, 0.90f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.10f, 0.80f, 0.75f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(0.32f, 0.98f, 0.90f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.28f, 0.80f, 0.75f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.10f, 0.70f, 0.65f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.10f, 0.75f, 0.75f, 0.90f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.25f, 0.90f, 0.90f, 0.80f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.10f, 0.80f, 0.80f, 0.50f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.25f, 0.85f, 0.85f, 0.15f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.10f, 0.70f, 0.70f, 0.20f);
      } /* End of 'StyleColorsNeonTurquoise' function */

      /* Set StyleColorsNeonYellow ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsNeonYellow( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding    = 5.0f;
        style->FrameRounding     = 5.0f;
        style->PopupRounding     = 7.0f;
        style->ScrollbarRounding = 6.0f;
        style->GrabRounding      = 5.0f;
        style->FrameBorderSize   = 1.2f;
        style->WindowBorderSize  = 1.0f;

        colors[ImGuiCol_Text]                 = ImVec4(1.00f, 1.00f, 0.50f, 1.00f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.70f, 0.70f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.15f, 0.15f, 0.05f, 1.00f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.18f, 0.18f, 0.07f, 1.00f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.22f, 0.21f, 0.10f, 1.00f);
        colors[ImGuiCol_Border]                = ImVec4(0.85f, 0.85f, 0.40f, 0.9f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.45f, 0.45f, 0.24f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.90f, 0.90f, 0.30f, 1.00f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(1.00f, 1.00f, 0.40f, 1.00f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.55f, 0.55f, 0.20f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.80f, 0.80f, 0.25f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.45f, 0.45f, 0.15f, 1.00f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.58f, 0.58f, 0.20f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.50f, 0.50f, 0.18f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(1.00f, 1.00f, 0.40f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(1.00f, 1.00f, 0.60f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(1.00f, 1.00f, 0.70f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(1.00f, 1.00f, 0.45f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(1.00f, 1.00f, 0.55f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(1.00f, 1.00f, 0.65f, 1.00f);
        colors[ImGuiCol_Button]                  = ImVec4(0.90f, 0.90f, 0.30f, 1.00f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(1.00f, 1.00f, 0.50f, 1.00f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(1.00f, 1.00f, 0.40f, 1.00f);
        colors[ImGuiCol_Header]                  = ImVec4(1.00f, 1.00f, 0.45f, 1.00f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(1.00f, 1.00f, 0.55f, 1.00f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(1.00f, 1.00f, 0.65f, 1.00f);
        colors[ImGuiCol_Separator]               = ImVec4(0.80f, 0.80f, 0.40f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.90f, 0.90f, 0.50f, 1.00f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.95f, 0.95f, 0.70f, 1.00f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.90f, 0.90f, 0.60f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(1.00f, 1.00f, 0.80f, 1.00f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(1.00f, 1.00f, 0.90f, 1.00f);
        colors[ImGuiCol_PlotLines]               = ImVec4(1.00f, 0.90f, 0.30f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(1.00f, 0.95f, 0.60f, 1.00f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.95f, 0.85f, 0.30f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(1.00f, 0.95f, 0.50f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(1.00f, 0.90f, 0.30f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 1.00f, 0.10f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.80f, 0.80f, 0.30f, 0.05f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(1.00f, 1.00f, 0.55f, 0.65f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.80f, 0.80f, 0.40f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.70f, 0.65f, 0.25f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.90f, 0.90f, 0.10f, 0.90f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.95f, 0.95f, 0.20f, 0.80f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.80f, 0.80f, 0.08f, 0.50f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.95f, 0.95f, 0.30f, 0.15f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.85f, 0.85f, 0.25f, 0.20f);
      } /* End of 'StyleColorsNeonYellow' function */

      /* Set StyleColorsNeonLime ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsNeonLime( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding    = 6.0f;
        style->FrameRounding     = 5.0f;
        style->PopupRounding     = 6.0f;
        style->ScrollbarRounding = 6.0f;
        style->GrabRounding      = 6.0f;
        style->FrameBorderSize   = 1.0f;
        style->WindowBorderSize  = 1.0f;

        colors[ImGuiCol_Text]                 = ImVec4(0.60f, 1.00f, 0.15f, 1.00f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.35f, 0.70f, 0.20f, 1.00f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.10f, 0.18f, 0.07f, 1.00f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.12f, 0.20f, 0.06f, 1.00f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.14f, 0.25f, 0.05f, 1.00f);
        colors[ImGuiCol_Border]                = ImVec4(0.35f, 0.78f, 0.12f, 0.9f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.30f, 0.72f, 0.20f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.60f, 0.95f, 0.28f, 1.00f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.65f, 1.00f, 0.35f, 1.00f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.25f, 0.78f, 0.15f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.40f, 0.90f, 0.20f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.20f, 0.70f, 0.12f, 1.00f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.28f, 0.85f, 0.21f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.25f, 0.80f, 0.18f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.55f, 0.95f, 0.25f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.65f, 1.00f, 0.30f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.75f, 1.00f, 0.30f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.65f, 1.00f, 0.20f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.52f, 0.95f, 0.20f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.65f, 1.00f, 0.30f, 1.00f);
        colors[ImGuiCol_Button]                  = ImVec4(0.40f, 0.85f, 0.10f, 1.00f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.65f, 1.00f, 0.20f, 1.00f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.55f, 0.85f, 0.15f, 1.00f);
        colors[ImGuiCol_Header]                  = ImVec4(0.55f, 1.00f, 0.15f, 1.00f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.68f, 1.00f, 0.30f, 1.00f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.60f, 0.90f, 0.25f, 1.00f);
        colors[ImGuiCol_Separator]               = ImVec4(0.45f, 0.82f, 0.24f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.55f, 0.90f, 0.25f, 1.00f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.50f, 0.88f, 0.20f, 1.00f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.55f, 1.00f, 0.25f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.65f, 1.00f, 0.30f, 1.00f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.75f, 1.00f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.40f, 1.00f, 0.30f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.50f, 1.00f, 0.40f, 1.00f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.35f, 0.90f, 0.25f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.45f, 0.98f, 0.33f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.40f, 1.00f, 0.35f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(0.75f, 1.00f, 0.15f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.20f, 0.90f, 0.25f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(0.60f, 0.98f, 0.40f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.30f, 0.85f, 0.20f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.15f, 0.75f, 0.15f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.65f, 0.90f, 0.10f, 0.90f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.70f, 1.00f, 0.20f, 0.80f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.50f, 0.85f, 0.10f, 0.50f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.80f, 1.00f, 0.20f, 0.15f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.75f, 0.87f, 0.18f, 0.20f);
      } /* End of 'StyleColorsNeonLime' function */

      /* Set StyleColorsNeonBrown ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsNeonBrown( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding    = 4.0f;
        style->FrameRounding     = 4.0f;
        style->PopupRounding     = 5.0f;
        style->ScrollbarRounding = 5.0f;
        style->GrabRounding      = 5.0f;
        style->FrameBorderSize   = 1.0f;
        style->WindowBorderSize  = 1.0f;

        colors[ImGuiCol_Text]                 = ImVec4(0.65f, 0.45f, 0.25f, 1.00f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.45f, 0.35f, 0.20f, 1.00f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.10f, 0.06f, 0.03f, 1.00f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.12f, 0.08f, 0.04f, 1.00f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.15f, 0.10f, 0.05f, 1.00f);
        colors[ImGuiCol_Border]                = ImVec4(0.60f, 0.40f, 0.20f, 0.9f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.35f, 0.25f, 0.15f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.65f, 0.50f, 0.25f, 1.00f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.75f, 0.60f, 0.30f, 1.00f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.45f, 0.30f, 0.13f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.60f, 0.40f, 0.20f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.42f, 0.28f, 0.12f, 1.00f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.50f, 0.35f, 0.15f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.40f, 0.28f, 0.10f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.80f, 0.56f, 0.25f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.90f, 0.64f, 0.30f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.95f, 0.70f, 0.32f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.88f, 0.50f, 0.20f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.70f, 0.44f, 0.20f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.80f, 0.50f, 0.20f, 1.00f);
        colors[ImGuiCol_Button]                  = ImVec4(0.65f, 0.42f, 0.20f, 1.00f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.80f, 0.50f, 0.20f, 1.00f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.70f, 0.45f, 0.20f, 1.00f);
        colors[ImGuiCol_Header]                  = ImVec4(0.75f, 0.44f, 0.20f, 1.00f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.90f, 0.53f, 0.20f, 1.00f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.80f, 0.50f, 0.20f, 1.00f);
        colors[ImGuiCol_Separator]               = ImVec4(0.65f, 0.40f, 0.18f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.75f, 0.52f, 0.22f, 1.00f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.70f, 0.48f, 0.20f, 1.00f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.70f, 0.40f, 0.20f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.80f, 0.50f, 0.22f, 1.00f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.90f, 0.60f, 0.25f, 1.00f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.75f, 0.45f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.85f, 0.55f, 0.25f, 1.00f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.70f, 0.45f, 0.18f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.80f, 0.55f, 0.25f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.70f, 0.55f, 0.25f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(0.90f, 0.70f, 0.20f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.30f, 0.20f, 0.06f, 0.05f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(0.70f, 0.50f, 0.15f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.50f, 0.40f, 0.12f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.40f, 0.30f, 0.10f, 0.30f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.55f, 0.42f, 0.07f, 0.90f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.66f, 0.52f, 0.09f, 0.75f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.60f, 0.48f, 0.09f, 0.55f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.65f, 0.52f, 0.10f, 0.15f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.55f, 0.42f, 0.07f, 0.20f);
      } /* End of 'StyleColorsNeonBrown' function */

      /* Set StyleColorsNeonWhite ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsNeonWhite( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding    = 6.0f;
        style->FrameRounding     = 6.0f;
        style->PopupRounding     = 6.0f;
        style->ScrollbarRounding = 5.0f;
        style->GrabRounding      = 5.0f;
        style->FrameBorderSize   = 1.0f;
        style->WindowBorderSize  = 1.0f;

        colors[ImGuiCol_Text]                 = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_Border]                = ImVec4(0.85f, 0.85f, 0.85f, 0.9f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
        colors[ImGuiCol_Button]                  = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        colors[ImGuiCol_Header]                  = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
        colors[ImGuiCol_Separator]               = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.80f, 0.80f, 0.80f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.20f, 0.20f, 0.20f, 0.05f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(0.85f, 0.85f, 0.85f, 0.75f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.75f, 0.75f, 0.75f, 0.25f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.55f, 0.55f, 0.55f, 0.45f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.95f, 0.95f, 0.95f, 0.90f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.85f, 0.85f, 0.85f, 0.75f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.75f, 0.75f, 0.75f, 0.55f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(1.00f, 1.00f, 1.00f, 0.15f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.90f, 0.90f, 0.90f, 0.20f);
      } /* End of 'StyleColorsNeonWhite' function */

      /* Set StyleColorsPastel ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsPastel( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding    = 5.0f;
        style->FrameRounding     = 4.0f;
        style->PopupRounding     = 6.0f;
        style->ScrollbarRounding = 5.0f;
        style->GrabRounding      = 4.0f;
        style->FrameBorderSize   = 1.0f;
        style->WindowBorderSize  = 1.0f;

        colors[ImGuiCol_Text]                  = ImVec4(0.43f, 0.39f, 0.57f, 1.0f);
        colors[ImGuiCol_TextDisabled]          = ImVec4(0.65f, 0.60f, 0.72f, 1.0f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.94f, 0.91f, 0.96f, 1.0f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.96f, 0.94f, 0.97f, 1.0f);
        colors[ImGuiCol_Border]                 = ImVec4(0.80f, 0.75f, 0.85f, 1.0f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.78f, 0.72f, 0.82f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.87f, 0.82f, 0.89f, 1.0f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.95f, 0.90f, 0.95f, 1.0f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.61f, 0.57f, 0.69f, 1.0f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.57f, 0.50f, 0.70f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.48f, 0.42f, 0.53f, 1.0f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.70f, 0.66f, 0.79f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.68f, 0.63f, 0.75f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.55f, 0.50f, 0.63f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.63f, 0.58f, 0.70f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.70f, 0.65f, 0.75f, 1.0f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.59f, 0.56f, 0.70f, 1.0f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.58f, 0.54f, 0.70f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.62f, 0.60f, 0.72f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.74f, 0.70f, 0.84f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.85f, 0.80f, 0.92f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.77f, 0.74f, 0.88f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.55f, 0.52f, 0.65f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.68f, 0.65f, 0.78f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.72f, 0.69f, 0.82f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.60f, 0.58f, 0.72f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.70f, 0.65f, 0.80f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.75f, 0.70f, 0.85f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.60f, 0.56f, 0.70f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.70f, 0.65f, 0.80f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.74f, 0.70f, 0.85f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.54f, 0.50f, 0.65f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.65f, 0.60f, 0.78f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.50f, 0.45f, 0.62f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.60f, 0.55f, 0.72f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.55f, 0.52f, 0.69f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 0.85f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.15f, 0.16f, 0.20f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(0.55f, 0.52f, 0.68f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.20f, 0.20f, 0.30f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.10f, 0.10f, 0.15f, 0.25f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.65f, 0.62f, 0.75f, 0.90f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.58f, 0.56f, 0.68f, 0.75f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.70f, 0.68f, 0.78f, 0.55f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.85f, 0.84f, 0.94f, 0.10f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.80f, 0.78f, 0.92f, 0.15f);
      } /* End of 'StyleColorsPastel' function */

      /* Set StyleColorsTerminalGreen ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsTerminalGreen( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding    = 0.0f;
        style->FrameRounding     = 0.0f;
        style->PopupRounding     = 0.0f;
        style->ScrollbarRounding = 0.0f;
        style->GrabRounding      = 0.0f;
        style->FrameBorderSize   = 0.0f;
        style->WindowBorderSize  = 1.0f;

        colors[ImGuiCol_Text]                 = ImVec4(0.70f, 1.00f, 0.20f, 1.0f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.30f, 0.60f, 0.10f, 1.0f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.05f, 0.10f, 0.04f, 1.0f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.05f, 0.12f, 0.06f, 1.0f);
        colors[ImGuiCol_Border]                = ImVec4(0.15f, 0.40f, 0.15f, 0.8f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.12f, 0.30f, 0.10f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.30f, 0.70f, 0.25f, 1.0f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.45f, 0.95f, 0.40f, 1.0f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.10f, 0.35f, 0.10f, 1.0f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.25f, 0.95f, 0.20f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.10f, 0.30f, 0.08f, 1.0f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.30f, 0.12f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.10f, 0.28f, 0.10f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.20f, 0.60f, 0.20f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.30f, 0.85f, 0.30f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.40f, 1.00f, 0.40f, 1.0f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.30f, 1.00f, 0.30f, 1.0f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.30f, 1.00f, 0.30f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.40f, 1.00f, 0.40f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.20f, 0.80f, 0.20f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.30f, 0.90f, 0.30f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.10f, 0.75f, 0.10f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.30f, 1.00f, 0.40f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.35f, 1.00f, 0.45f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.30f, 0.90f, 0.40f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.22f, 0.75f, 0.22f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.30f, 0.91f, 0.30f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.40f, 1.00f, 0.40f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.40f, 1.00f, 0.40f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.50f, 1.00f, 0.50f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.60f, 1.00f, 0.60f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.30f, 1.00f, 0.30f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.45f, 1.00f, 0.45f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.35f, 0.90f, 0.35f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.45f, 0.95f, 0.45f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.25f, 1.00f, 0.25f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(0.80f, 1.00f, 0.30f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.15f, 0.85f, 0.25f, 0.05f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(0.60f, 1.00f, 0.50f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.30f, 0.75f, 0.30f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.20f, 0.85f, 0.20f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.25f, 0.70f, 0.25f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.10f, 0.40f, 0.10f, 0.65f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.12f, 0.45f, 0.12f, 0.40f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.15f, 0.70f, 0.15f, 0.10f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.20f, 0.75f, 0.20f, 0.15f);
      } /* End of 'StyleColorsTerminalGreen' function */

      /* Set StyleColorsGrayDay ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsGrayDay( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding    = 4.0f;
        style->FrameRounding     = 3.0f;
        style->PopupRounding     = 5.0f;
        style->ScrollbarRounding = 5.0f;
        style->GrabRounding      = 4.0f;
        style->FrameBorderSize   = 1.0f;
        style->WindowBorderSize  = 1.0f;

        colors[ImGuiCol_Text]                 = ImVec4(0.33f, 0.33f, 0.33f, 1.0f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.85f, 0.85f, 0.85f, 1.0f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.90f, 0.90f, 0.90f, 1.0f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.88f, 0.88f, 0.88f, 1.0f);
        colors[ImGuiCol_Border]                = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.77f, 0.77f, 0.77f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.87f, 0.87f, 0.87f, 1.0f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.92f, 0.92f, 0.92f, 1.0f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.75f, 0.75f, 0.75f, 1.0f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.55f, 0.55f, 0.55f, 1.0f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.70f, 0.70f, 0.70f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.40f, 0.40f, 0.40f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.33f, 0.33f, 0.33f, 1.0f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.45f, 0.45f, 0.45f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.70f, 0.70f, 0.70f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.70f, 0.70f, 0.70f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.80f, 0.80f, 0.80f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.45f, 0.45f, 0.45f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.55f, 0.55f, 0.55f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.65f, 0.65f, 0.65f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.40f, 0.40f, 0.40f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.55f, 0.55f, 0.55f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.70f, 0.70f, 0.70f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.58f, 0.58f, 0.58f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(0.85f, 0.85f, 0.85f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.33f, 0.55f, 0.85f, 0.10f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(0.44f, 0.65f, 0.95f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.40f, 0.70f, 0.95f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.45f, 0.45f, 0.45f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.70f, 0.70f, 0.70f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.60f, 0.60f, 0.60f, 0.75f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.50f, 0.50f, 0.50f, 0.55f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.90f, 0.90f, 0.90f, 0.15f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.85f, 0.85f, 0.85f, 0.20f);
      } /* End of 'StyleColorsGrayDay' function */

      /* Set StyleColorsEveningRed ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsEveningRed( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding    = 7.0f;
        style->FrameRounding     = 6.0f;
        style->PopupRounding     = 8.0f;
        style->ScrollbarRounding = 7.0f;
        style->GrabRounding      = 5.0f;
        style->FrameBorderSize   = 1.2f;
        style->WindowBorderSize  = 1.0f;

        colors[ImGuiCol_Text]                 = ImVec4(0.85f, 0.25f, 0.25f, 1.0f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.55f, 0.18f, 0.18f, 1.0f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.18f, 0.05f, 0.05f, 1.0f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.22f, 0.06f, 0.06f, 1.0f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.24f, 0.08f, 0.08f, 1.0f);
        colors[ImGuiCol_Border]                = ImVec4(0.70f, 0.25f, 0.25f, 0.90f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.60f, 0.20f, 0.20f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.90f, 0.35f, 0.35f, 1.0f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(1.00f, 0.40f, 0.40f, 1.0f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.55f, 0.15f, 0.15f, 1.0f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.75f, 0.25f, 0.25f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.40f, 0.10f, 0.10f, 1.0f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.65f, 0.25f, 0.25f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.55f, 0.20f, 0.20f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.90f, 0.31f, 0.31f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(1.00f, 0.40f, 0.40f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(1.00f, 0.47f, 0.47f, 1.0f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.99f, 0.20f, 0.20f, 1.0f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(1.00f, 0.25f, 0.25f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(1.00f, 0.35f, 0.35f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.95f, 0.30f, 0.30f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(1.00f, 0.35f, 0.35f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.90f, 0.25f, 0.25f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.90f, 0.35f, 0.35f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(1.00f, 0.45f, 0.45f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.97f, 0.40f, 0.40f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.65f, 0.25f, 0.25f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.75f, 0.35f, 0.35f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.70f, 0.30f, 0.30f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.70f, 0.30f, 0.30f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.80f, 0.40f, 0.40f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.90f, 0.45f, 0.45f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(1.00f, 0.30f, 0.30f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(1.00f, 0.40f, 0.40f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.95f, 0.25f, 0.25f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(1.00f, 0.35f, 0.35f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(1.00f, 0.25f, 0.25f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 0.20f, 0.20f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.90f, 0.35f, 0.35f, 0.15f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(1.00f, 0.40f, 0.40f, 0.75f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(1.00f, 0.35f, 0.35f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.50f, 0.20f, 0.20f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.70f, 0.35f, 0.35f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.55f, 0.25f, 0.25f, 0.75f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.75f, 0.45f, 0.45f, 0.55f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.90f, 0.70f, 0.70f, 0.12f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.85f, 0.65f, 0.65f, 0.20f);
      } /* End of 'StyleColorsEveningRed' function */

      /* Set StyleColorsGoldenNight ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsGoldenNight( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding    = 6.0f;
        style->FrameRounding     = 5.0f;
        style->PopupRounding     = 7.0f;
        style->ScrollbarRounding = 6.0f;
        style->GrabRounding      = 6.0f;
        style->FrameBorderSize   = 1.5f;
        style->WindowBorderSize  = 1.0f;

        colors[ImGuiCol_Text]                 = ImVec4(1.00f, 0.80f, 0.20f, 1.0f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.80f, 0.70f, 0.40f, 1.0f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.12f, 0.10f, 0.03f, 1.0f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.18f, 0.15f, 0.05f, 1.0f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.20f, 0.17f, 0.06f, 1.0f);
        colors[ImGuiCol_Border]                = ImVec4(0.90f, 0.70f, 0.10f, 0.9f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.82f, 0.65f, 0.23f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.95f, 0.85f, 0.35f, 1.0f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(1.00f, 0.95f, 0.40f, 1.0f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.90f, 0.75f, 0.25f, 1.0f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.97f, 0.82f, 0.27f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.75f, 0.65f, 0.23f, 1.0f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.85f, 0.72f, 0.20f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.82f, 0.70f, 0.18f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.90f, 0.80f, 0.20f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.95f, 0.88f, 0.25f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.99f, 0.95f, 0.30f, 1.0f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.95f, 0.80f, 0.10f, 1.0f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.90f, 0.85f, 0.10f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.95f, 0.90f, 0.15f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.90f, 0.82f, 0.20f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.95f, 0.88f, 0.25f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.85f, 0.75f, 0.15f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.90f, 0.82f, 0.20f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.95f, 0.88f, 0.30f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.85f, 0.75f, 0.20f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.85f, 0.78f, 0.20f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.90f, 0.85f, 0.30f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.95f, 0.90f, 0.50f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.90f, 0.82f, 0.20f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.95f, 0.88f, 0.30f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(1.00f, 0.90f, 0.40f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.95f, 0.85f, 0.35f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(1.00f, 0.90f, 0.40f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.85f, 0.75f, 0.15f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.90f, 0.85f, 0.30f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.95f, 0.80f, 0.10f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 0.90f, 0.10f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.80f, 0.70f, 0.15f, 0.05f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(0.90f, 0.85f, 0.40f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.65f, 0.60f, 0.25f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.60f, 0.55f, 0.20f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.90f, 0.80f, 0.30f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.80f, 0.70f, 0.20f, 0.75f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.75f, 0.70f, 0.25f, 0.45f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.88f, 0.78f, 0.28f, 0.10f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.85f, 0.70f, 0.20f, 0.15f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.90f, 0.78f, 0.28f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.85f, 0.69f, 0.22f, 0.75f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.80f, 0.65f, 0.18f, 0.55f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.88f, 0.75f, 0.26f, 0.15f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.80f, 0.70f, 0.22f, 0.20f);
      } /* End of 'StyleColorsGoldenNight' function */

      /* Set StyleColorsBlueRect/StyleColorsMarine ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsMarine( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding    = 0.0f;
        style->FrameRounding     = 0.0f;
        style->PopupRounding     = 0.0f;
        style->ScrollbarRounding = 0.0f;
        style->GrabRounding      = 0.0f;
        style->FrameBorderSize   = 1.5f;
        style->WindowBorderSize  = 1.5f;

        colors[ImGuiCol_Text]                 = ImVec4(0.70f, 0.85f, 1.00f, 1.0f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.40f, 0.55f, 0.70f, 1.0f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.10f, 0.20f, 0.35f, 1.0f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.05f, 0.12f, 0.22f, 1.0f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.07f, 0.14f, 0.27f, 1.0f);
        colors[ImGuiCol_Border]                = ImVec4(0.25f, 0.45f, 0.80f, 1.0f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.15f, 0.30f, 0.55f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.30f, 0.55f, 0.90f, 1.0f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.45f, 0.70f, 0.98f, 1.0f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.12f, 0.25f, 0.52f, 1.0f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.22f, 0.42f, 0.75f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.15f, 0.30f, 0.62f, 1.0f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.20f, 0.35f, 0.65f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.18f, 0.30f, 0.60f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.45f, 0.65f, 0.98f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.60f, 0.80f, 1.00f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.65f, 0.85f, 1.00f, 1.0f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.50f, 0.75f, 1.00f, 1.0f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.55f, 0.80f, 1.00f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.70f, 0.90f, 1.00f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.45f, 0.72f, 1.00f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.60f, 0.82f, 1.00f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.75f, 0.90f, 1.00f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.55f, 0.80f, 1.00f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.70f, 0.90f, 1.00f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.80f, 0.95f, 1.00f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.50f, 0.75f, 1.00f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.65f, 0.85f, 1.00f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.75f, 0.90f, 1.00f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.50f, 0.75f, 1.00f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.65f, 0.85f, 1.00f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.85f, 0.90f, 1.00f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.30f, 0.65f, 0.95f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.50f, 0.80f, 1.00f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.40f, 0.70f, 1.00f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.50f, 0.80f, 1.00f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.35f, 0.75f, 1.00f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(0.90f, 1.00f, 1.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.15f, 0.65f, 1.00f, 0.04f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(0.50f, 0.85f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.30f, 0.70f, 1.00f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.20f, 0.45f, 0.65f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.20f, 0.45f, 0.75f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.18f, 0.35f, 0.65f, 0.70f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.15f, 0.30f, 0.55f, 0.60f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.18f, 0.40f, 0.70f, 0.10f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.15f, 0.35f, 0.65f, 0.15f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.18f, 0.30f, 0.52f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.22f, 0.42f, 0.75f, 0.90f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.15f, 0.29f, 0.54f, 0.70f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.15f, 0.33f, 0.85f, 0.20f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.12f, 0.25f, 0.67f, 0.10f);
      } /* End of 'StyleColorsMarine' function */

      /* Set StyleColorsSunSetOrange ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsSunSetOrange( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding    = 4.0f;
        style->FrameRounding     = 4.0f;
        style->PopupRounding     = 6.0f;
        style->ScrollbarRounding = 5.0f;
        style->GrabRounding      = 5.0f;
        style->FrameBorderSize   = 1.1f;
        style->WindowBorderSize  = 1.0f;

        colors[ImGuiCol_Text]                 = ImVec4(1.00f, 0.60f, 0.30f, 1.0f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.75f, 0.45f, 0.25f, 1.0f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.10f, 0.05f, 0.02f, 1.0f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.15f, 0.07f, 0.03f, 1.0f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.22f, 0.11f, 0.04f, 1.0f);
        colors[ImGuiCol_Border]                = ImVec4(1.00f, 0.62f, 0.26f, 0.8f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.60f, 0.25f, 0.10f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.85f, 0.45f, 0.20f, 1.0f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(1.00f, 0.50f, 0.15f, 1.0f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.70f, 0.34f, 0.14f, 1.0f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.95f, 0.50f, 0.22f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.60f, 0.30f, 0.12f, 1.0f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.75f, 0.38f, 0.20f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.75f, 0.38f, 0.20f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.95f, 0.55f, 0.25f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(1.00f, 0.65f, 0.35f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(1.00f, 0.75f, 0.45f, 1.0f);
        colors[ImGuiCol_CheckMark]              = ImVec4(1.00f, 0.65f, 0.25f, 1.0f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.90f, 0.50f, 0.13f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(1.00f, 0.65f, 0.23f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.85f, 0.45f, 0.20f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(1.00f, 0.60f, 0.30f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.95f, 0.55f, 0.22f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(1.00f, 0.60f, 0.25f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(1.00f, 0.70f, 0.35f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(1.00f, 0.65f, 0.30f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.95f, 0.70f, 0.30f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(1.00f, 0.80f, 0.40f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.97f, 0.75f, 0.35f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(1.00f, 0.70f, 0.30f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(1.00f, 0.85f, 0.40f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(1.00f, 0.90f, 0.50f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(1.00f, 0.60f, 0.25f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(1.00f, 0.70f, 0.35f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(1.00f, 0.60f, 0.25f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(1.00f, 0.80f, 0.35f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(1.00f, 0.70f, 0.30f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(1.00f, 0.90f, 0.20f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.90f, 0.70f, 0.15f, 0.05f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(1.00f, 0.80f, 0.38f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.85f, 0.70f, 0.28f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.80f, 0.70f, 0.20f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(1.00f, 0.60f, 0.25f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.90f, 0.70f, 0.25f, 0.75f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.85f, 0.65f, 0.22f, 0.55f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(1.00f, 0.80f, 0.25f, 0.10f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(1.00f, 0.75f, 0.20f, 0.15f);
      } /* End of 'StyleColorsSunSetOrange' function */

      /* Set StyleColorsVioletSweety ImGui style function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      inline static VOID StyleColorsVioletSweety( VOID )
      {
        ImGuiStyle *style = &ImGui::GetStyle();
        ImVec4 *colors = style->Colors;

        style->WindowRounding    = 7.0f;
        style->FrameRounding     = 7.0f;
        style->PopupRounding     = 8.0f;
        style->ScrollbarRounding = 8.0f;
        style->GrabRounding      = 6.0f;
        style->FrameBorderSize   = 0.0f;
        style->WindowBorderSize  = 1.0f;

        colors[ImGuiCol_Text]                 = ImVec4(0.48f, 0.30f, 0.56f, 1.0f);
        colors[ImGuiCol_TextDisabled]         = ImVec4(0.64f, 0.52f, 0.70f, 1.0f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.95f, 0.90f, 0.97f, 1.0f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.90f, 0.85f, 0.93f, 1.0f);
        colors[ImGuiCol_Border]                = ImVec4(0.70f, 0.60f, 0.85f, 1.0f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.80f, 0.65f, 0.95f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.85f, 0.70f, 0.98f, 1.0f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.88f, 0.75f, 1.00f, 1.0f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.60f, 0.45f, 0.80f, 1.0f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.70f, 0.55f, 0.90f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.58f, 0.40f, 0.78f, 1.0f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.65f, 0.45f, 0.85f, 1.0f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.60f, 0.40f, 0.80f, 1.0f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.52f, 0.35f, 0.70f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.62f, 0.45f, 0.80f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.70f, 0.50f, 0.82f, 1.0f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.70f, 0.55f, 0.95f, 1.0f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.68f, 0.55f, 0.90f, 1.0f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.74f, 0.65f, 0.95f, 1.0f);
        colors[ImGuiCol_Button]                  = ImVec4(0.65f, 0.50f, 0.90f, 1.0f);
        colors[ImGuiCol_ButtonHovered]           = ImVec4(0.75f, 0.55f, 0.98f, 1.0f);
        colors[ImGuiCol_ButtonActive]            = ImVec4(0.70f, 0.55f, 0.90f, 1.0f);
        colors[ImGuiCol_Header]                  = ImVec4(0.62f, 0.50f, 0.90f, 1.0f);
        colors[ImGuiCol_HeaderHovered]           = ImVec4(0.72f, 0.60f, 0.95f, 1.0f);
        colors[ImGuiCol_HeaderActive]            = ImVec4(0.68f, 0.55f, 0.90f, 1.0f);
        colors[ImGuiCol_Separator]               = ImVec4(0.61f, 0.50f, 0.86f, 1.0f);
        colors[ImGuiCol_SeparatorHovered]        = ImVec4(0.68f, 0.58f, 0.90f, 1.0f);
        colors[ImGuiCol_SeparatorActive]         = ImVec4(0.65f, 0.53f, 0.87f, 1.0f);
        colors[ImGuiCol_ResizeGrip]              = ImVec4(0.70f, 0.60f, 0.89f, 1.0f);
        colors[ImGuiCol_ResizeGripHovered]       = ImVec4(0.75f, 0.65f, 0.95f, 1.0f);
        colors[ImGuiCol_ResizeGripActive]        = ImVec4(0.80f, 0.68f, 0.95f, 1.0f);
        colors[ImGuiCol_PlotLines]               = ImVec4(0.62f, 0.50f, 0.90f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered]        = ImVec4(0.72f, 0.60f, 0.95f, 1.0f);
        colors[ImGuiCol_PlotHistogram]           = ImVec4(0.65f, 0.53f, 0.90f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered]    = ImVec4(0.70f, 0.58f, 0.93f, 1.0f);
        colors[ImGuiCol_TextSelectedBg]          = ImVec4(0.70f, 0.55f, 0.90f, 0.25f);
        colors[ImGuiCol_DragDropTarget]          = ImVec4(0.95f, 0.85f, 0.98f, 0.90f);
        colors[ImGuiCol_NavHighlight]            = ImVec4(0.40f, 0.30f, 0.60f, 0.05f);
        colors[ImGuiCol_NavWindowingHighlight]   = ImVec4(0.65f, 0.60f, 0.90f, 0.75f);
        colors[ImGuiCol_NavWindowingDimBg]       = ImVec4(0.40f, 0.35f, 0.70f, 0.25f);
        colors[ImGuiCol_ModalWindowDimBg]        = ImVec4(0.30f, 0.27f, 0.45f, 0.35f);

        colors[ImGuiCol_TableHeaderBg]           = ImVec4(0.55f, 0.40f, 0.85f, 0.85f);
        colors[ImGuiCol_TableBorderStrong]       = ImVec4(0.40f, 0.30f, 0.70f, 0.75f);
        colors[ImGuiCol_TableBorderLight]        = ImVec4(0.50f, 0.35f, 0.80f, 0.60f);
        colors[ImGuiCol_TableRowBg]              = ImVec4(0.60f, 0.45f, 0.90f, 0.10f);
        colors[ImGuiCol_TableRowBgAlt]           = ImVec4(0.50f, 0.40f, 0.85f, 0.15f);
      } /* End of 'StyleColorsVioletSweety' function */

      // Current selected style
      INT CurrentStyle {};

      // Styles array (Russian name, setup function, English name)
      std::vector<std::tuple<std::string, std::function<VOID(VOID)>, std::string>> ColorStyles =
      {
        {"Темный Стандартный", StyleDark, "DarkStandard"},
        {"Светлый Стандартный", StyleLight, "LightStandard"},
        {"Enemy Mouse", StyleEnemyMouse, "EnemyMouse"},
        {"Spectrum", StyleColorsSpectrum, "Spectrum"},
        {"Светлый Мягкий", StyleColorsLightSoft, "LightSoft"},
        {"Классический Мягкий", StyleColorsClassicSoft, "ClassicSoft"},
        {"Монохромный", StyleColorsMonoChrome, "MonoChrome"},
        {"Стальной Серый", StyleColorsSteelGray, "SteelGray"},
        {"Синий Океан", StyleColorsBlueOcean, "BlueOcean"},
        {"Чистый Синий", StyleColorsCleanBlue, "CleanBlue"},
        {"Океан Глубокий", StyleColorsOceanDeep, "OceanDeep"},
        {"Мягкий Голубой", StyleColorsSoftBlue, "SoftBlue"},
        {"Глубокая Ночь", StyleColorsDeepNight, "DeepNight"},
        {"Лесной Зеленый", StyleColorsForestGreen, "ForestGreen"},
        {"Кислотный Зеленый", StyleColorsAcidGreen, "AcidGreen"},
        {"Ретро Фиолетовый", StyleColorsRetroPurple, "RetroPurple"},
        {"Темный Фиолетовый", StyleColorsDarkViolet, "DarkViolet"},
        {"Кремовый", StyleColorsCreamy, "Creamy"},
        {"Винтажный Красный", StyleColorsVintageRed, "VintageRed"},
        {"Неоновый Розовый", StyleColorsNeonPink, "NeonPink"},
        {"Розовая Конфета", StyleColorsCandyPink, "CandyPink"},
        //{"Кондитерский розовый", StyleColorsCandyPink, "CandyPink"},
        {"Солнечный День", StyleColorsSunnyDay, "SunnyDay"},
        {"Админ Панель", StyleColorsAdminPanel, "AdminPanel"},
        {"Темный Ржавый", StyleColorsDarkRust, "DarkRust"},
        {"Темный Spectrum", StyleColorsDarkSpectrum, "DarkSpectrum"},
        {"Теплый Круглый", StyleColorsRoundedWarm, "RoundedWarm"},
        {"Высокий Контраст", StyleColorsHighContrast, "HighContrast"},
        {"Зеленый Неон", StyleColorsNeonGreen, "NeonGreen"},
        {"Синий Неон", StyleColorsNeonBlue, "NeonBlue"},
        {"Красный Неон", StyleColorsNeonRed, "NeonRed"},
        {"Салатовый Неон", StyleColorsNeonTurquoise, "NeonTurquoise"},
        {"Желтый Неон", StyleColorsNeonYellow, "NeonYellow"},
        {"Бирюзовый Неон", StyleColorsNeonLime, "NeonLime"},
        {"Коричнеый Неон", StyleColorsNeonBrown, "NeonBrown"},
        {"Белый Неон", StyleColorsNeonWhite, "NeonWhite"},

        {"Пастель", StyleColorsPastel, "Pastel"},
        {"Зеленый терминал", StyleColorsTerminalGreen, "TerminalGreen"},
        {"Серый день", StyleColorsGrayDay, "GrayDay"},
        {"Вечерний красный", StyleColorsEveningRed, "EveningRed"},
        {"Золотая ночь", StyleColorsGoldenNight, "GoldenNight"},
        //{"Прямоугольный синий", StyleColorsBlueRect, "BlueRect"},
        {"Морской", StyleColorsMarine, "Marine"},
        {"Оранжевый закат", StyleColorsSunSetOrange, "SunSetOrange"},
        {"Фиалковая нежность", StyleColorsVioletSweety, "VioletSweety"},
      };

      /* Set ImGui style by number function.
       * ARGUMENTS:
       *   - style number from style table:
       *       INT StyleNo;
       * RETURNS:
       *   (INT) real set style number.
       */
      INT SetStyle( INT StyleNo )
      {
        if (StyleNo < 0 || StyleNo >= ColorStyles.size())
          StyleNo = 0;
        std::get<1>(ColorStyles[StyleNo])();
        CurrentStyle = StyleNo;
        return StyleNo;
      } /* End of 'SetStyle' function */

      /* Set ImGui style by name function.
       * ARGUMENTS:
       *   - style number from style table:
       *       const std::string &StyleName;
       * RETURNS:
       *   (INT) real set style number.
       */
      INT SetStyle( const std::string &StyleName )
      {
        INT StyleNo = 0;

        for (INT i = 0; i < ColorStyles.size(); i++)
          if (StyleName == std::get<2>(ColorStyles[i]))
          {
            StyleNo = i;
            break;
          }
        std::get<1>(ColorStyles[StyleNo])();
        CurrentStyle = StyleNo;
        return StyleNo;
      } /* End of 'SetStyle' function */
    }; /* end of 'styles' namespace */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

/* END OF 'tam_ux_styles.cpp' FILE */
