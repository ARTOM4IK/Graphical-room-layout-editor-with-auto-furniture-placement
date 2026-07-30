/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX core class implementation file.
 *               Panel layout, splitter, vertical tabs, text rotation.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 30.04.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux.h"

/* Register tab in core function.
 * ARGUMENTS:
 *   - pointer to tab:
 *       tab *Tab;
 * RETURNS: None.
 */
VOID tam::ux::core::RegisterTab( tam::ux::tab *Tab )
{
  if (Tab == nullptr)
    return;
  Tabs.push_back(Tab);
} /* End of 'tam::ux::core::RegisterTab' function */

/* Unregister tab from core function.
 * ARGUMENTS:
 *   - pointer to tab:
 *       tab *Tab;
 * RETURNS: None.
 */
VOID tam::ux::core::UnregisterTab( tam::ux::tab *Tab )
{
  // Release icon if loaded
  vkDeviceWaitIdle(dynamic_cast<render *>(this)->Device);
  auto icon_it = TabIconTexIDs.find(Tab);
  if (icon_it != TabIconTexIDs.end())
  {
    if (!Tab->GetIcon().Name.empty())
      Images.ReleaseIcon(Tab->GetIcon().Name);
    TabIconTexIDs.erase(icon_it);
  }

  auto it = std::find(Tabs.begin(), Tabs.end(), Tab);
  if (it != Tabs.end())
  {
    INT idx = (INT)(it - Tabs.begin());
    Tabs.erase(it);
    if (ActiveTabIndex >= (INT)Tabs.size())
      ActiveTabIndex = Tabs.empty() ? 0 : (INT)Tabs.size() - 1;
    else if (ActiveTabIndex > idx)
      ActiveTabIndex--;
  }
} /* End of 'tam::ux::core::UnregisterTab' function */

/* Initialize UX system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::core::UXInit( VOID )
{
  // Guidelines primitive initialization
  render *RndPtr = dynamic_cast<render *>(this);
  tam::material_pattern MtlPat =
    RndPtr->MaterialPatternCreate(RndPtr->ShaderCreate("ux/guidelines"),
      "UX GildeLines material pattern", 0, 0, {}, {},
      {}, tam::vlk::topology::POINTS, tam::vlk::blend_mode::eTRANSPARENT);
  tam::material Mtl = RndPtr->MaterialCreate("UX GildeLines material", MtlPat);
  vertex_std V = {vec3(0, 0, 0), vec2(0, 0), vec3(0, 0, -1), vec4(1, 0, 1, 1)};

  Guidelines = RndPtr->PrimitiveCreate(Mtl, &V, sizeof(vec3), 1, {});
} /* End of 'tam::ux::core::UXInit' function */

/* Close UX system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::core::UXClose( VOID )
{
  TabIconTexIDs.clear();
  Tabs.clear();

  auto VlkDevice = dynamic_cast<render *>(this)->Device;
  if (VlkDevice != nullptr)
    vkDeviceWaitIdle(VlkDevice);
  if (IsLogoLoaded && LogoTexID != nullptr)
    Images.Release("res/tam_logo.ico");
  LogoTexID = nullptr;
  IsLogoLoaded = FALSE;

  Images.Clear();
} /* End of 'tam::ux::core::UXClose' function */

/* UX system response - layout panel, draw menu, tabs and controls function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::core::UXResponse( VOID )
{
  if (Tabs.empty())
  {
    UXSceneViewport = {0, 0, (FLT)UXHostW, (FLT)UXHostH};
    if (anim *Ani = dynamic_cast<anim *>(this); Ani != nullptr)
    {
      Ani->Cam.Resize((INT)UXSceneViewport.W, (INT)UXSceneViewport.H);
      Ani->Cam.OffsetX = (INT)UXSceneViewport.X;
      Ani->Cam.OffsetY = (INT)UXSceneViewport.Y;
    }
    return;
  }

  // Load fonts on first frame
  if (!IsFontsLoaded)
  {
    LoadFonts();
    IsFontsLoaded = TRUE;
  }

  // Load logo on first frame
  if (!IsLogoLoaded && Images.LoadFn)
  {
    LogoTexID = Images.Acquire("res/tam_logo.ico");
    IsLogoLoaded = TRUE;
  }

  // Save initial dark style
  if (!IsStyleSaved)
  {
    SavedDarkStyle = ImGui::GetStyle();
    IsStyleSaved = TRUE;
  }

  // Apply selected font and size
  ImGuiIO &io = ImGui::GetIO();
  ImGuiStyle &style = ImGui::GetStyle();
  if (SelectedFontIndex >= 0 && SelectedFontIndex < (INT)LoadedFonts.size())
    io.FontDefault = LoadedFonts[SelectedFontIndex];
  style._NextFrameFontSizeBase = FontSize;
  style.Alpha = 1.0f;

  // Recalculate tab strip width based on font size
  TabStripWidth = ImGui::GetFontSize() * 2.0f;

  FLT wnd_w = (FLT)UXHostW;
  FLT wnd_h = (FLT)UXHostH;
  if (wnd_w < 100 || wnd_h < 100)
    return;

  // Reset one-shot controls from previous frame
  for (auto *T : Tabs)
    T->ResetControls();

  ShowConfirmDialog();

  // --- Main menu bar ---
  IsAnyPopupOpen = ImGui::IsPopupOpen("", ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel);
  FLT menu_bar_h = 0;
  DrawMainMenu(menu_bar_h);

  FLT panel_y = menu_bar_h;
  wnd_h -= menu_bar_h;

  // --- Panel / splitter layout ---
  FLT max_panel_w = wnd_w * PanelMaxWidthRatio;
  if (PanelWidth < PanelMinWidth)
    PanelWidth = PanelMinWidth;
  if (PanelWidth > max_panel_w)
    PanelWidth = max_panel_w;

  const FLT splitter_w = 6.0f;
  FLT panel_x, splitter_x;

  if (IsPanelOnRight)
  {
    panel_x = wnd_w - PanelWidth;
    splitter_x = panel_x - splitter_w;
  }
  else
  {
    panel_x = 0;
    splitter_x = PanelWidth;
  }

  // Compute scene viewport
  if (IsPanelOnRight)
  {
    UXSceneViewport = {0, menu_bar_h, panel_x - splitter_w, wnd_h};
  }
  else
  {
    FLT left = PanelWidth + splitter_w;
    UXSceneViewport = {left, menu_bar_h, wnd_w - left, wnd_h};
  }
  // Splitter interaction
  BOOL splitter_hovered =
    !IsAnyPopupOpen &&
    io.MousePos.x >= splitter_x &&
    io.MousePos.x <= splitter_x + splitter_w &&
    io.MousePos.y >= panel_y &&
    io.MousePos.y <= panel_y + wnd_h;

  if (splitter_hovered && ImGui::IsMouseClicked(0))
    IsDraggingSplitter = TRUE;

  if (IsDraggingSplitter)
  {
    if (ImGui::IsMouseDown(0))
    {
      if (IsPanelOnRight)
        PanelWidth = wnd_w - io.MousePos.x - splitter_w * 0.5f;
      else
        PanelWidth = io.MousePos.x - splitter_w * 0.5f;
      if (PanelWidth < PanelMinWidth)
        PanelWidth = PanelMinWidth;
      if (PanelWidth > max_panel_w)
        PanelWidth = max_panel_w;
      ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
      RegStoreAll();
    }
    else
      IsDraggingSplitter = FALSE;
  }

  if (splitter_hovered || IsDraggingSplitter)
    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);

  // Recalculate positions after drag
  if (IsPanelOnRight)
  {
    panel_x = wnd_w - PanelWidth;
    splitter_x = panel_x - splitter_w;
  }
  else
  {
    panel_x = 0;
    splitter_x = PanelWidth;
  }

  // Recompute viewport after drag
  if (IsPanelOnRight)
    UXSceneViewport = {0, menu_bar_h, panel_x - splitter_w, wnd_h};
  else
  {
    FLT left = PanelWidth + splitter_w;
    UXSceneViewport = {left, menu_bar_h, wnd_w - left, wnd_h};
  }

  if (anim *Ani = dynamic_cast<anim *>(this); Ani != nullptr)
  {
    // Camera viewport data must match the final panel layout, not the preliminary one.
    Ani->Cam.Resize((INT)UXSceneViewport.W, (INT)UXSceneViewport.H);
    Ani->Cam.OffsetX = (INT)UXSceneViewport.X;
    Ani->Cam.OffsetY = (INT)UXSceneViewport.Y;
  }

  // Draw splitter bar
  ImDrawList *bg_draw = ImGui::GetBackgroundDrawList();
  ImU32 splitter_color = (splitter_hovered || IsDraggingSplitter) ?
    IM_COL32(80, 160, 240, 255) :
    IM_COL32(40, 80, 140, 200);
  bg_draw->AddRectFilled(
    ImVec2(splitter_x, panel_y),
    ImVec2(splitter_x + splitter_w, panel_y + wnd_h),
    splitter_color);
  FLT mid_y = panel_y + wnd_h * 0.5f;
  bg_draw->AddLine(
    ImVec2(splitter_x + splitter_w * 0.5f, mid_y - wnd_h * 0.05f),
    ImVec2(splitter_x + splitter_w * 0.5f, mid_y + wnd_h * 0.05f),
    IM_COL32(160, 200, 255, 200), 2.0f);

  // --- Main panel window ---
  ImGui::SetNextWindowPos(ImVec2(panel_x, panel_y));
  ImGui::SetNextWindowSize(ImVec2(PanelWidth, wnd_h));

  ImGuiWindowFlags panel_flags =
    ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoCollapse |
    ImGuiWindowFlags_NoBringToFrontOnFocus;

  if (ImGui::Begin("##ux_panel", nullptr, panel_flags))
  {
    if (SelectedFontIndex >= (INT)LoadedFonts.size())
      SelectedFontIndex = 0;

    FLT content_h = ImGui::GetContentRegionAvail().y;

    if (IsPanelOnRight)
    {
      DrawVerticalTabs(content_h);
      ImGui::SameLine();
      DrawTabContent(0, content_h);
    }
    else
    {
      FLT avail_w = ImGui::GetContentRegionAvail().x;
      FLT content_w = avail_w - TabStripWidth - ImGui::GetStyle().ItemSpacing.x;
      DrawTabContent(content_w, content_h);
      ImGui::SameLine();
      DrawVerticalTabs(content_h);
    }

    // About modal dialog
    if (ShowAboutModalFlag)
    {
      ImGui::OpenPopup("About TAM");
      ShowAboutModalFlag = FALSE;
    }
    DrawAboutModal();
  }
  ImGui::End();
} /* End of 'tam::ux::core::UXResponse' function */

/* UX system render function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::core::UXRender( VOID )
{
} /* End of 'tam::ux::core::UXRender' function */

/* Draw vertical tab strip function.
 * ARGUMENTS:
 *   - available height:
 *       FLT Height;
 * RETURNS: None.
 */
VOID tam::ux::core::DrawVerticalTabs( FLT Height )
{
  const INT tab_count = (INT)Tabs.size();
  if (tab_count == 0)
    return;

  const FLT icon_size = TabStripWidth * 0.9f;
  const FLT icon_tab_h = TabStripWidth;
  BOOL is_tabs_on_left = IsPanelOnRight;

  const ImU32 border_color = ImGui::GetColorU32(ImGuiCol_Border);
  const FLT border_thick = 5.0f;

  const ImU32 col_bg       = ImGui::GetColorU32(ImGuiCol_WindowBg);
  const ImU32 col_hovered  = ImGui::GetColorU32(ImGuiCol_TabHovered);
  const ImU32 col_text     = ImGui::GetColorU32(ImGuiCol_Text);
  const ImU32 col_text_dim = ImGui::GetColorU32(ImGuiCol_TextDisabled);

  ImGui::BeginChild("##vtabs", ImVec2(TabStripWidth, Height), ImGuiChildFlags_None,
    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  ImVec2 win_pos = ImGui::GetCursorScreenPos();

  /* Compute tab heights */
  std::vector<FLT> tab_tops(tab_count);
  std::vector<FLT> tab_h(tab_count);
  FLT y_cursor = 0;

  for (INT i = 0; i < tab_count; i++)
  {
    // Resolve icon texture (lazy load)
    VOID *icon_tex = nullptr;
    const std::string &icon_name = Tabs[i]->GetIcon().Name;
    if (!icon_name.empty())
    {
      auto it = TabIconTexIDs.find(Tabs[i]);
      if (it != TabIconTexIDs.end())
        icon_tex = it->second;
      else
      {
        icon_tex = Images.AcquireIcon(icon_name);
        TabIconTexIDs[Tabs[i]] = icon_tex;
      }
    }

    BOOL has_icon = (icon_tex != nullptr);
    BOOL has_name = !Tabs[i]->GetName().empty();

    if (has_icon && has_name)
    {
      ImVec2 text_size = ImGui::CalcTextSize(Tabs[i]->GetName().c_str());
      tab_h[i] = icon_tab_h + text_size.x + 12.0f;
    }
    else if (has_icon)
      tab_h[i] = icon_tab_h;
    else if (has_name)
    {
      ImVec2 text_size = ImGui::CalcTextSize(Tabs[i]->GetName().c_str());
      tab_h[i] = text_size.x + 16.0f;
    }
    else
      tab_h[i] = icon_tab_h;

    tab_tops[i] = y_cursor;
    y_cursor += tab_h[i];
  }

  // Clamp active tab index
  if (ActiveTabIndex >= tab_count)
    ActiveTabIndex = 0;

  // Draw tabs
  for (INT i = 0; i < tab_count; i++)
  {
    ImVec2 tab_min(win_pos.x, win_pos.y + tab_tops[i]);
    ImVec2 tab_max(win_pos.x + TabStripWidth, win_pos.y + tab_tops[i] + tab_h[i]);

    BOOL is_active = (ActiveTabIndex == i);
    BOOL is_hovered = !IsAnyPopupOpen && ImGui::IsMouseHoveringRect(tab_min, tab_max);

    ImU32 bg_color = is_hovered && !is_active ? col_hovered : col_bg;

    if (is_active)
    {
      ImDrawFlags round = is_tabs_on_left ?
        ImDrawFlags_RoundCornersLeft : ImDrawFlags_RoundCornersRight;
      draw_list->AddRectFilled(tab_min, tab_max, bg_color, 4.0f, round);
    }
    else
      draw_list->AddRectFilled(tab_min, tab_max, bg_color, 4.0f);

    // Resolve icon texture
    VOID *icon_tex = nullptr;
    auto icon_it = TabIconTexIDs.find(Tabs[i]);
    if (icon_it != TabIconTexIDs.end())
      icon_tex = icon_it->second;

    BOOL has_icon = (icon_tex != nullptr);
    BOOL has_name = !Tabs[i]->GetName().empty();

    if (has_icon && has_name)
    {
      // Icon at top, rotated text below
      FLT pad = (TabStripWidth - icon_size) * 0.5f;
      ImVec2 icon_min(tab_min.x + pad, tab_min.y + pad);
      draw_list->AddImage((ImTextureID)icon_tex, icon_min,
        ImVec2(icon_min.x + icon_size, icon_min.y + icon_size));

      const CHAR *label = Tabs[i]->GetName().c_str();
      ImVec2 text_size = ImGui::CalcTextSize(label);
      FLT font_sz = ImGui::GetFontSize();

      FLT text_region_top = tab_min.y + icon_tab_h;
      FLT text_region_h = tab_h[i] - icon_tab_h;
      FLT tab_cx = tab_min.x + TabStripWidth * 0.5f;
      FLT tab_cy = text_region_top + text_region_h * 0.5f;

      ImVec2 text_pos;
      FLT angle;
      if (is_tabs_on_left)
        angle = (FLT)PI * 0.5f, text_pos = ImVec2(tab_cx + font_sz * 0.5f, tab_cy - text_size.x * 0.5f);
      else
        angle = -(FLT)PI * 0.5f, text_pos = ImVec2(tab_cx - font_sz * 0.5f, tab_cy + text_size.x * 0.5f);
      // Mandatory left-top orientation
      angle = -(FLT)PI * 0.5f, text_pos = ImVec2(tab_cx - font_sz * 0.5f, tab_cy + text_size.x * 0.5f);

      ImU32 text_color = is_active ? col_text : col_text_dim;
      DrawTextRotated(draw_list, label, text_pos, angle, text_color);
    }
    else if (has_icon)
    {
      // Icon only, centered
      FLT pad = (TabStripWidth - icon_size) * 0.5f;
      ImVec2 icon_min(tab_min.x + pad, tab_min.y + pad);
      draw_list->AddImage((ImTextureID)icon_tex, icon_min,
        ImVec2(icon_min.x + icon_size, icon_min.y + icon_size));
    }
    else if (has_name)
    {
      // Rotated text only
      const CHAR *label = Tabs[i]->GetName().c_str();
      ImVec2 text_size = ImGui::CalcTextSize(label);
      FLT font_sz = ImGui::GetFontSize();

      FLT tab_cx = tab_min.x + TabStripWidth * 0.5f;
      FLT tab_cy = tab_min.y + tab_h[i] * 0.5f;

      ImVec2 text_pos;
      FLT angle;
      if (is_tabs_on_left)
        angle = (FLT)PI * 0.5f, text_pos = ImVec2(tab_cx + font_sz * 0.5f, tab_cy - text_size.x * 0.5f);
      else
        angle = -(FLT)PI * 0.5f, text_pos = ImVec2(tab_cx - font_sz * 0.5f, tab_cy + text_size.x * 0.5f);
      // Mandatory left-top orientation
      angle = -(FLT)PI * 0.5f, text_pos = ImVec2(tab_cx - font_sz * 0.5f, tab_cy + text_size.x * 0.5f);

      ImU32 text_color = is_active ? col_text : col_text_dim;
      DrawTextRotated(draw_list, label, text_pos, angle, text_color);
    }
    else
    {
      // Fallback: draw ellipsis character
      const CHAR *fallback = "...";
      ImVec2 text_size = ImGui::CalcTextSize(fallback);
      FLT font_sz = ImGui::GetFontSize();

      FLT tab_cx = tab_min.x + TabStripWidth * 0.5f;
      FLT tab_cy = tab_min.y + tab_h[i] * 0.5f;

      ImVec2 text_pos;
      FLT angle;
      if (is_tabs_on_left)
        angle = (FLT)PI * 0.5f, text_pos = ImVec2(tab_cx + font_sz * 0.5f, tab_cy - text_size.x * 0.5f);
      else
        angle = -(FLT)PI * 0.5f, text_pos = ImVec2(tab_cx - font_sz * 0.5f, tab_cy + text_size.x * 0.5f);
      // Mandatory left-top orientation
      angle = -(FLT)PI * 0.5f, text_pos = ImVec2(tab_cx - font_sz * 0.5f, tab_cy + text_size.x * 0.5f);

      DrawTextRotated(draw_list, fallback, text_pos, angle, col_text_dim);
    }

    if (is_hovered && ImGui::IsMouseClicked(0))
      ActiveTabIndex = i;
  }

  // Border polyline: vertical separator with active-tab cutout
  ImVec2 a_min(win_pos.x, win_pos.y + tab_tops[ActiveTabIndex]);
  ImVec2 a_max(win_pos.x + TabStripWidth,
    win_pos.y + tab_tops[ActiveTabIndex] + tab_h[ActiveTabIndex]);
  FLT y_top = win_pos.y;
  FLT y_bot = win_pos.y + Height;

  ImVec2 pts[6];
  if (is_tabs_on_left)
  {
    FLT bx = win_pos.x + TabStripWidth;
    pts[0] = ImVec2(bx, y_top);
    pts[1] = ImVec2(bx, a_min.y);
    pts[2] = ImVec2(a_min.x, a_min.y);
    pts[3] = ImVec2(a_min.x, a_max.y);
    pts[4] = ImVec2(bx, a_max.y);
    pts[5] = ImVec2(bx, y_bot);
  }
  else
  {
    FLT bx = win_pos.x;
    pts[0] = ImVec2(bx, y_top);
    pts[1] = ImVec2(bx, a_min.y);
    pts[2] = ImVec2(a_max.x, a_min.y);
    pts[3] = ImVec2(a_max.x, a_max.y);
    pts[4] = ImVec2(bx, a_max.y);
    pts[5] = ImVec2(bx, y_bot);
  }
  draw_list->AddPolyline(pts, 6, border_color, ImDrawFlags_None, border_thick);

  ImGui::EndChild();
} /* End of 'tam::ux::core::DrawVerticalTabs' function */

/* Draw currently active tab content region function.
 * ARGUMENTS:
 *   - content width (0 = fill remaining):
 *       FLT Width;
 *   - content height:
 *       FLT Height;
 * RETURNS: None.
 */
VOID tam::ux::core::DrawTabContent( FLT Width, FLT Height )
{
  ImGui::BeginChild("##tabcontent", ImVec2(Width, Height), ImGuiChildFlags_Borders,
    ImGuiWindowFlags_HorizontalScrollbar);

  if (ActiveTabIndex >= 0 && ActiveTabIndex < (INT)Tabs.size())
    Tabs[ActiveTabIndex]->DrawControls();

  ImGui::EndChild();
} /* End of 'tam::ux::core::DrawTabContent' function */

/* Draw rotated text glyph-by-glyph via ImDrawList function.
 * ARGUMENTS:
 *   - ImGui draw list pointer:
 *       ImDrawList *DrawList;
 *   - text to draw:
 *       const CHAR *Text;
 *   - pivot position:
 *       ImVec2 Pos;
 *   - rotation angle in radians:
 *       FLT Angle;
 *   - text color:
 *       ImU32 Color;
 * RETURNS: None.
 */
VOID tam::ux::core::DrawTextRotated( ImDrawList *DrawList, const CHAR *Text,
                                     ImVec2 Pos, FLT Angle, ImU32 Color )
{
  ImFont *font = ImGui::GetFont();
  ImFontBaked *baked = ImGui::GetFontBaked();
  if (font == nullptr || baked == nullptr || Text == nullptr || *Text == 0)
    return;

  FLT font_size = ImGui::GetFontSize();
  FLT scale = font_size / baked->Size;
  FLT cos_a = cosf(Angle);
  FLT sin_a = sinf(Angle);

  DrawList->PushTextureID(font->ContainerAtlas->TexRef);

  FLT cursor = 0;
  const CHAR *p = Text;
  while (*p != 0)
  {
    UINT codepoint = 0;
    INT bytes = ImTextCharFromUtf8(&codepoint, p, nullptr);
    if (bytes <= 0)
      break;
    p += bytes;

    const ImFontGlyph *glyph = baked->FindGlyph((ImWchar)codepoint);
    if (glyph == nullptr)
      continue;

    if (glyph->Visible)
    {
      FLT x0 = cursor + glyph->X0 * scale;
      FLT y0 = glyph->Y0 * scale;
      FLT x1 = cursor + glyph->X1 * scale;
      FLT y1 = glyph->Y1 * scale;

      ImVec2 c0(Pos.x + x0 * cos_a - y0 * sin_a, Pos.y + x0 * sin_a + y0 * cos_a);
      ImVec2 c1(Pos.x + x1 * cos_a - y0 * sin_a, Pos.y + x1 * sin_a + y0 * cos_a);
      ImVec2 c2(Pos.x + x1 * cos_a - y1 * sin_a, Pos.y + x1 * sin_a + y1 * cos_a);
      ImVec2 c3(Pos.x + x0 * cos_a - y1 * sin_a, Pos.y + x0 * sin_a + y1 * cos_a);

      DrawList->PrimReserve(6, 4);
      DrawList->PrimQuadUV(c0, c1, c2, c3,
        ImVec2(glyph->U0, glyph->V0),
        ImVec2(glyph->U1, glyph->V0),
        ImVec2(glyph->U1, glyph->V1),
        ImVec2(glyph->U0, glyph->V1),
        Color);
    }
    cursor += glyph->AdvanceX * scale;
  }
  DrawList->PopTextureID();
} /* End of 'tam::ux::core::DrawTextRotated' function */

/* END OF 'tam_ux.cpp' FILE */
