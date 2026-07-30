/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_sample_unit_VG4_ux_test.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX test unit - ImGui panel with vertical tabs,
 *               split bar, menu bar with font options,
 *               and various control examples.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 07.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "utils/imgui/imgui_internal.h"

/* Project namespace */
namespace tam
{
  /* Units namespace */
  namespace units
  {
    /* UX test unit class */
    class ux_test : public unit
    {
    private:
      FLT TabStripWidth = 38.0f;

      // Size parameters
      FLT PanelWidth = 400.0f;
      FLT PanelMinWidth = 200.0f;
      FLT PanelMaxWidthRatio = 2.0f / 3.0f;

      // Tab control flags and values
      BOOL IsDraggingSplitter = FALSE;
      BOOL IsAnyPopupOpen = FALSE;
      INT ActiveTab = 0;

      // Panel placing flag
      BOOL IsPanelOnRight = FALSE;

      // Logo texture for menu bar and About dialog
      texture LogoTexture {};
      VkDescriptorSet LogoDescSet {};

      // Font management
      std::vector<ImFont *> LoadedFonts;
      std::vector<std::string> FontNames;
      INT SelectedFontIndex = 1;
      FLT FontSize = 18.0f;
      INT SelectedStyleIndex = 0;
      ImGuiStyle SavedDarkStyle {};
      BOOL IsStyleSaved = FALSE;

      // About modal state
      BOOL ShowAboutModalFlag = false;

      texture IconTextures[5] {};
      VkDescriptorSet IconDescSets[5] {};
      BOOL IsIconsLoaded = FALSE;

      /* Image store data type */
      struct image_entry
      {
        texture Tex {};
        VkDescriptorSet DescSet {};
        std::string Name {};
      }; /* End of 'image_entry' structure */

      std::vector<image_entry> DropdownImages {};
      INT SelectedImageIdx = 0;
      BOOL IsImagesLoaded = FALSE;

      texture FlatTexture {};
      VkDescriptorSet FlatDescSet {};
      BOOL IsFlatLoaded = FALSE;

      // Tab control names
      static constexpr const CHAR *TabNames[] =
      {
        "Sliders", "Radios", "Checks", "Input",
        "Colors", "Dropdown", "Graphics", "Data"
      };

      // Slider controls state
      FLT SliderFloat1 = 0.5f;
      FLT SliderFloat2 = 0.0f;
      INT SliderInt1 = 50;
      FLT SliderAngle = 0.0f;
      FLT RangeMin = 0.2f, RangeMax = 0.8f;
      FLT VSlider1 = 0.5f;
      FLT VSlider2 = 0.3f;
      FLT VSlider3 = 0.7f;

      // Radio controls state
      INT RadioValue = 0;
      INT RadioGroup2 = 1;

      // Checkbox state
      bool CheckValues[6] = {true, false, true, false, true, false};

      // Input controls state
      CHAR TextBuf[256] = "Hello, TAM!";
      CHAR MultilineBuf[1024] = "Line 1\nLine 2\nLine 3\n";
      CHAR HintBuf[128] = "";
      INT IntVal = 42;
      FLT FloatVal = 3.14f;

      // Color controls state
      FLT ColorRGB[3] = {0.2f, 0.5f, 0.9f};
      FLT ColorRGBA[4] = {1.0f, 0.3f, 0.1f, 0.8f};
      FLT ColorHSV[3] = {0.6f, 0.8f, 0.7f};

      /* Tree data types */
      struct tree_node
      {
        std::string Name;
        std::vector<tree_node> Children;
      }; /* End of 'tree_node' structure */

      // Used UI tree data
      std::vector<tree_node> TreeData;

      /* Table row data types */
      struct table_row
      {
        INT Id;
        std::string Name;
        FLT Value;
        std::string Status;
      }; /* End of 'table_row' structure */

      // Table data
      std::vector<table_row> TableData;

      /* Initialize tree test data function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID InitTreeData( VOID )
      {
        TreeData =
        {
          {"Root A",
            {
              {"Child A.1", {{"Leaf A.1.1", {}}, {"Leaf A.1.2", {}}}},
              {"Child A.2", {}},
              {"Child A.3", {{"Leaf A.3.1", {}}}}
            }},
          {"Root B",
            {
              {"Child B.1", {}},
              {"Child B.2", {{"Leaf B.2.1", {}}, {"Leaf B.2.2", {}}, {"Leaf B.2.3", {}}}}
            }},
          {"Root C",
            {
              {"Child C.1", {{"Leaf C.1.1", {{"Deep C.1.1.1", {}}}}}},
              {"Child C.2", {}}
            }}
        };
      } /* End of 'InitTreeData' function */

      /* Initialize table test data function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID InitTableData( VOID )
      {
        TableData =
        {
          {1, "Alpha",   0.95f, "Active"},
          {2, "Beta",    0.72f, "Paused"},
          {3, "Gamma",   0.31f, "Active"},
          {4, "Delta",   0.88f, "Inactive"},
          {5, "Epsilon", 0.56f, "Active"},
          {6, "Zeta",    0.14f, "Error"},
          {7, "Eta",     0.67f, "Active"},
          {8, "Theta",   0.43f, "Paused"},
        };
      } /* End of 'InitTableData' function */

      /* Draw tree node recursively function.
       * ARGUMENTS:
       *   - node reference:
       *       const tree_node &Node;
       * RETURNS: None.
       */
      VOID DrawTreeNode( const tree_node &Node )
      {
        if (Node.Children.empty())
        {
          ImGui::TreeNodeEx(Node.Name.c_str(),
            ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
          return;
        }
        if (ImGui::TreeNode(Node.Name.c_str()))
        {
          for (auto &child : Node.Children)
            DrawTreeNode(child);
          ImGui::TreePop();
        }
      } /* End of 'DrawTreeNode' function */

      /* Draw rotated text glyph-by-glyph via ImDrawList primitives function.
       * ARGUMENTS:
       *   - draw list pointer:
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
      VOID DrawTextRotated( ImDrawList *DrawList, const CHAR *Text, ImVec2 Pos, FLT Angle, ImU32 Color )
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
          unsigned int codepoint = 0;
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
      } /* End of 'DrawTextRotated' function */

      /* Draw vertical tab bar (side-aware) function.
       * ARGUMENTS:
       *   - available height:
       *       FLT Height;
       * RETURNS: None.
       */
      VOID DrawVerticalTabs( FLT Height )
      {
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

        FLT tab_tops[8], tab_h[8];
        FLT y_cursor = 0;
        for (INT i = 0; i < 8; i++)
        {
          BOOL has_icon = (i < 5 && IsIconsLoaded && IconDescSets[i] != VK_NULL_HANDLE);
          if (has_icon)
            tab_h[i] = icon_tab_h;
          else
          {
            ImVec2 text_size = ImGui::CalcTextSize(TabNames[i]);
            tab_h[i] = text_size.x + 16.0f;
          }
          tab_tops[i] = y_cursor;
          y_cursor += tab_h[i];
        }

        for (INT i = 0; i < 8; i++)
        {
          ImVec2 tab_min(win_pos.x, win_pos.y + tab_tops[i]);
          ImVec2 tab_max(win_pos.x + TabStripWidth, win_pos.y + tab_tops[i] + tab_h[i]);

          BOOL is_active = (ActiveTab == i);
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

          BOOL has_icon = (i < 5 && IsIconsLoaded && IconDescSets[i] != VK_NULL_HANDLE);
          if (has_icon)
          {
            FLT pad = (TabStripWidth - icon_size) * 0.5f;
            ImVec2 icon_min(tab_min.x + pad, tab_min.y + pad);
            draw_list->AddImage((ImTextureID)IconDescSets[i], icon_min,
              ImVec2(icon_min.x + icon_size, icon_min.y + icon_size));
          }
          else
          {
            const CHAR *label = TabNames[i];
            ImVec2 text_size = ImGui::CalcTextSize(label);
            FLT font_sz = ImGui::GetFontSize();

            FLT tab_cx = tab_min.x + TabStripWidth * 0.5f;
            FLT tab_cy = tab_min.y + tab_h[i] * 0.5f;

            ImVec2 text_pos;
            FLT angle;

            if (is_tabs_on_left)
              angle = PI * 0.5f, text_pos = ImVec2(tab_cx + font_sz * 0.5f, tab_cy - text_size.x * 0.5f);
            else
              angle = -PI * 0.5f, text_pos = ImVec2(tab_cx - font_sz * 0.5f, tab_cy + text_size.x * 0.5f);

            ImU32 text_color = is_active ? col_text : col_text_dim;
            DrawTextRotated(draw_list, label, text_pos, angle, text_color);
          }

          if (is_hovered && ImGui::IsMouseClicked(0))
            ActiveTab = i;
        }

        /* Border polyline: vertical separator with active-tab cutout */
        ImVec2 a_min(win_pos.x, win_pos.y + tab_tops[ActiveTab]);
        ImVec2 a_max(win_pos.x + TabStripWidth,
          win_pos.y + tab_tops[ActiveTab] + tab_h[ActiveTab]);
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
        draw_list->AddPolyline(pts, 6, border_color, ImDrawFlags_None,
          border_thick);

        ImGui::EndChild();
      } /* End of 'DrawVerticalTabs' function */

      /* Draw slider controls tab content function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DrawTabSliders( VOID )
      {
        ImGui::SeparatorText("Slider Controls");

        ImGui::SliderFloat("Float Slider", &SliderFloat1, 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Logarithmic", &SliderFloat2, 0.001f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
        ImGui::SliderInt("Integer Slider", &SliderInt1, 0, 100);
        ImGui::SliderAngle("Angle", &SliderAngle, -360.0f, 360.0f);
        ImGui::DragFloatRange2("Range", &RangeMin, &RangeMax, 0.005f, 0.0f, 1.0f);
        ImGui::Spacing();

        ImGui::Text("Float value: %.4f", SliderFloat1);
        ImGui::Text("Int value: %d", SliderInt1);
        ImGui::Text("Angle: %.1f deg", SliderAngle * 180.0f / 3.14159265f);

        ImGui::Spacing();
        ImGui::Separator();

        FLT progress = SliderFloat1;
        ImGui::ProgressBar(progress, ImVec2(-1.0f, 0.0f));

        auto overlay = std::format("{}/{}", (INT)(progress * 100), 100);
        ImGui::ProgressBar(progress, ImVec2(-1.0f, 0.0f), overlay.c_str());

        ImGui::Spacing();
        ImGui::VSliderFloat("##vs1", ImVec2(24, 120), &VSlider1, 0.0f, 1.0f);
        ImGui::SameLine();
        ImGui::VSliderFloat("##vs2", ImVec2(24, 120), &VSlider2, 0.0f, 1.0f);
        ImGui::SameLine();
        ImGui::VSliderFloat("##vs3", ImVec2(24, 120), &VSlider3, 0.0f, 1.0f);
      } /* End of 'DrawTabSliders' function */

      /* Draw radio button controls tab content function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DrawTabRadios( VOID )
      {
        ImGui::SeparatorText("Radio Button Controls");

        ImGui::Text("Render Mode:");
        ImGui::RadioButton("Wireframe", &RadioValue, 0);
        ImGui::RadioButton("Solid", &RadioValue, 1);
        ImGui::RadioButton("Textured", &RadioValue, 2);
        ImGui::RadioButton("Shaded", &RadioValue, 3);
        ImGui::RadioButton("PBR", &RadioValue, 4);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Projection:");
        ImGui::RadioButton("Perspective##proj", &RadioGroup2, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Orthographic##proj", &RadioGroup2, 1);
        ImGui::SameLine();
        ImGui::RadioButton("Isometric##proj", &RadioGroup2, 2);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        const CHAR *modes[] = {"Wireframe", "Solid", "Textured", "Shaded", "PBR"};
        const CHAR *projs[] = {"Perspective", "Orthographic", "Isometric"};
        ImGui::Text("Current: %s / %s", modes[RadioValue], projs[RadioGroup2]);
      } /* End of 'DrawTabRadios' function */

      /* Draw checkbox controls tab content function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DrawTabChecks( VOID )
      {
        ImGui::SeparatorText("Checkbox Controls");

        const CHAR *labels[] =
        {
          "Show Grid", "Show Axes", "Enable Shadows",
          "Anti-Aliasing", "V-Sync", "Show FPS"
        };

        for (INT i = 0; i < 6; i++)
          ImGui::Checkbox(labels[i], &CheckValues[i]);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Enabled features:");
        for (INT i = 0; i < 6; i++)
          if (CheckValues[i])
          {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "[%s]", labels[i]);
          }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Button("Select All"))
          for (INT i = 0; i < 6; i++)
            CheckValues[i] = true;
        ImGui::SameLine();
        if (ImGui::Button("Deselect All"))
          for (INT i = 0; i < 6; i++)
            CheckValues[i] = false;
        ImGui::SameLine();
        if (ImGui::Button("Toggle All"))
          for (INT i = 0; i < 6; i++)
            CheckValues[i] = !CheckValues[i];
      } /* End of 'DrawTabChecks' function */

      /* Draw text input controls tab content function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DrawTabInput( VOID )
      {
        ImGui::SeparatorText("Input Controls");

        ImGui::InputText("Text", TextBuf, IM_ARRAYSIZE(TextBuf));
        ImGui::InputTextWithHint("With Hint", "type here...", HintBuf, IM_ARRAYSIZE(HintBuf));
        ImGui::InputTextMultiline("##multiline", MultilineBuf, IM_ARRAYSIZE(MultilineBuf),
          ImVec2(-1.0f, ImGui::GetTextLineHeight() * 6));
        ImGui::InputInt("Integer", &IntVal);
        ImGui::InputFloat("Float", &FloatVal, 0.1f, 1.0f, "%.3f");

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Text value: %s", TextBuf);
        ImGui::Text("Hint value: %s", HintBuf);
        ImGui::Text("Int: %d  Float: %.3f", IntVal, FloatVal);
      } /* End of 'DrawTabInput' function */

      /* Draw color controls tab content function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DrawTabColors( VOID )
      {
        ImGui::SeparatorText("Color Controls");

        ImGui::ColorEdit3("RGB Color", ColorRGB);
        ImGui::ColorEdit4("RGBA Color", ColorRGBA);

        ImGui::Spacing();
        ImGui::ColorPicker3("Color Picker", ColorHSV,
          ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::ColorButton("Preview##rgb",
          ImVec4(ColorRGB[0], ColorRGB[1], ColorRGB[2], 1.0f), 0, ImVec2(60, 30));
        ImGui::SameLine();
        ImGui::ColorButton("Preview##rgba",
          ImVec4(ColorRGBA[0], ColorRGBA[1], ColorRGBA[2], ColorRGBA[3]),
          ImGuiColorEditFlags_AlphaPreview, ImVec2(60, 30));
      } /* End of 'DrawTabColors' function */

      /* Draw dropdown with images tab content function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DrawTabDropdown( VOID )
      {
        ImGui::SeparatorText("Dropdown with Images");

        if (!IsImagesLoaded || DropdownImages.empty())
        {
          ImGui::TextWrapped("Images not loaded from bin/textures/ui/images/");
          return;
        }

        const CHAR *preview = DropdownImages[SelectedImageIdx].Name.c_str();
        if (ImGui::BeginCombo("Texture", preview))
        {
          for (INT i = 0; i < (INT)DropdownImages.size(); i++)
          {
            BOOL is_selected = (SelectedImageIdx == i);
            if (DropdownImages[i].DescSet != VK_NULL_HANDLE)
            {
              ImGui::Image((ImTextureID)DropdownImages[i].DescSet, ImVec2(32, 32));
              ImGui::SameLine();
            }
            if (ImGui::Selectable(DropdownImages[i].Name.c_str(), is_selected))
              SelectedImageIdx = i;
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
          ImGui::EndCombo();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Selected: %s", DropdownImages[SelectedImageIdx].Name.c_str());

        if (DropdownImages[SelectedImageIdx].DescSet != VK_NULL_HANDLE)
        {
          FLT avail = ImGui::GetContentRegionAvail().x;
          FLT img_size = avail < 256.0f ? avail : 256.0f;
          ImGui::Image((ImTextureID)DropdownImages[SelectedImageIdx].DescSet,
            ImVec2(img_size, img_size));
        }
      } /* End of 'DrawTabDropdown' function */

      /* Draw vector and raster graphics tab content function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DrawTabGraphics( VOID )
      {
        ImGui::SeparatorText("Vector & Raster Graphics");

        ImGui::Text("Vector Drawing (ImDrawList):");
        ImGui::Spacing();

        const FLT canvas_size = 180.0f;
        ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
        ImVec2 canvas_end(canvas_pos.x + canvas_size, canvas_pos.y + canvas_size);
        ImDrawList *draw_list = ImGui::GetWindowDrawList();

        draw_list->AddRectFilled(canvas_pos, canvas_end, IM_COL32(15, 30, 50, 255));
        draw_list->AddRect(canvas_pos, canvas_end, IM_COL32(60, 120, 200, 255), 0.0f, 0, 2.0f);

        FLT cx = canvas_pos.x + canvas_size * 0.5f;
        FLT cy = canvas_pos.y + canvas_size * 0.5f;
        FLT r = canvas_size * 0.35f;

        draw_list->AddCircle(ImVec2(cx, cy), r, IM_COL32(100, 200, 255, 255), 32, 2.0f);
        draw_list->AddCircleFilled(ImVec2(cx, cy), r * 0.15f, IM_COL32(255, 200, 80, 255));

        for (INT i = 0; i < 6; i++)
        {
          FLT a1 = (FLT)i / 6.0f * 2.0f * 3.14159265f - 3.14159265f / 2.0f;
          FLT a2 = (FLT)((i + 2) % 6) / 6.0f * 2.0f * 3.14159265f - 3.14159265f / 2.0f;
          draw_list->AddLine(ImVec2(cx + cosf(a1) * r, cy + sinf(a1) * r),
            ImVec2(cx + cosf(a2) * r, cy + sinf(a2) * r),
            IM_COL32(60, 160, 220, 180), 1.5f);
        }

        for (INT i = 0; i < 6; i++)
        {
          FLT a = (FLT)i / 6.0f * 2.0f * 3.14159265f - 3.14159265f / 2.0f;
          FLT px = cx + cosf(a) * r;
          FLT py = cy + sinf(a) * r;
          draw_list->AddCircleFilled(ImVec2(px, py), 4.0f, IM_COL32(255, 120, 50, 255));
        }

        draw_list->AddTriangle(ImVec2(cx, cy - r * 0.6f),
          ImVec2(cx - r * 0.5f, cy + r * 0.4f),
          ImVec2(cx + r * 0.5f, cy + r * 0.4f),
          IM_COL32(100, 255, 100, 200), 2.0f);

        ImGui::Dummy(ImVec2(canvas_size, canvas_size));

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Raster Image (flat1.png):");
        ImGui::Spacing();

        if (IsFlatLoaded && FlatDescSet != VK_NULL_HANDLE)
        {
          FLT avail = ImGui::GetContentRegionAvail().x;
          FLT img_w = avail < 256.0f ? avail : 256.0f;
          FLT aspect = FlatTexture ? (FLT)FlatTexture->H / (FLT)FlatTexture->W : 1.0f;
          ImGui::Image((ImTextureID)FlatDescSet, ImVec2(img_w, img_w * aspect));
        }
        else
          ImGui::TextWrapped("Raster image not loaded from bin/textures/ui/flats/flat1.png");
      } /* End of 'DrawTabGraphics' function */

      /* Draw tree view and table tab content function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DrawTabData( VOID )
      {
        ImGui::SeparatorText("Tree View & Table");

        ImGui::Text("Tree View:");
        ImGui::BeginChild("##treeview", ImVec2(0, 200), ImGuiChildFlags_Borders);
        for (auto &node : TreeData)
          DrawTreeNode(node);
        ImGui::EndChild();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Data Table:");
        if (ImGui::BeginTable("##datatable", 4,
              ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
              ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY,
              ImVec2(0, 200)))
        {
          ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 40.0f);
          ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
          ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, 80.0f);
          ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, 80.0f);
          ImGui::TableSetupScrollFreeze(0, 1);
          ImGui::TableHeadersRow();

          for (auto &row : TableData)
          {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", row.Id);

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", row.Name.c_str());

            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%.2f", row.Value);

            ImGui::TableSetColumnIndex(3);
            ImVec4 status_color(0.8f, 0.8f, 0.8f, 1.0f);
            if (row.Status == "Active")
              status_color = ImVec4(0.3f, 1.0f, 0.3f, 1.0f);
            else if (row.Status == "Paused")
              status_color = ImVec4(1.0f, 1.0f, 0.3f, 1.0f);
            else if (row.Status == "Error")
              status_color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
            else if (row.Status == "Inactive")
              status_color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
            ImGui::TextColored(status_color, "%s", row.Status.c_str());
          }
          ImGui::EndTable();
        }
      } /* End of 'DrawTabData' function */

      /* Draw active tab content inside a child window function.
       * ARGUMENTS:
       *   - content width (0 = fill remaining):
       *       FLT Width;
       *   - content height:
       *       FLT Height;
       * RETURNS: None.
       */
      VOID DrawTabContent( FLT Width, FLT Height )
      {
        ImGui::BeginChild("##tabcontent", ImVec2(Width, Height), ImGuiChildFlags_Borders,
          ImGuiWindowFlags_HorizontalScrollbar);
        switch (ActiveTab)
        {
        case 0:
          DrawTabSliders();
          break;
        case 1:
          DrawTabRadios();
          break;
        case 2:
          DrawTabChecks();
          break;
        case 3:
          DrawTabInput();
          break;
        case 4:
          DrawTabColors();
          break;
        case 5:
          DrawTabDropdown();
          break;
        case 6:
          DrawTabGraphics();
          break;
        case 7:
          DrawTabData();
          break;
        }
        ImGui::EndChild();
      } /* End of 'DrawTabContent' function */

      /* Draw About modal dialog function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DrawAboutModal( VOID )
      {
        bool is_modal_open = true;
        if (ImGui::BeginPopupModal("About TAM", &is_modal_open, ImGuiWindowFlags_AlwaysAutoResize))
        {
          if (LogoDescSet != VK_NULL_HANDLE)
          {
            ImGui::Image((ImTextureID)LogoDescSet, ImVec2(48, 48));
            ImGui::SameLine();
          }
          ImGui::BeginGroup();
          ImGui::TextColored(ImVec4(0.5f, 0.9f, 1.0f, 1.0f), "TAM - Tough Arrangement Manager");
          ImGui::Text("CGSG Seniors 2025 Project");
          ImGui::Text("Computer Graphics Support Group");
          ImGui::Text("30 Phys-Math Lyceum");
          ImGui::EndGroup();

          ImGui::Spacing();
          ImGui::Separator();
          ImGui::Spacing();

          ImGui::Text("TAM UX Test Unit");
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
      } /* End of 'DrawAboutModal' function */

      /* Try to load icon textures function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID TryLoadIcons( anim *Ani )
      {
        if (IsIconsLoaded)
          return;

        const CHAR *icon_files[] =
        {
          "bin/textures/ui/icons/tam.ico",
          "bin/textures/ui/icons/file_edit.ico",
          "bin/textures/ui/icons/skybox.ico",
          "bin/textures/ui/icons/icons8-c-programming-480.png",
          "bin/textures/ui/icons/YRotPlus.ico",
        };

        for (INT i = 0; i < 5; i++)
        {
          IconTextures[i] = Ani->TextureCreate(icon_files[i]);
          if (IconTextures[i] &&
              IconTextures[i]->Img.View != VK_NULL_HANDLE &&
              IconTextures[i]->Sampler.Sampler != VK_NULL_HANDLE)
            IconDescSets[i] = ImGui_ImplVulkan_AddTexture(IconTextures[i]->Sampler.Sampler,
              IconTextures[i]->Img.View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
          else
            IconDescSets[i] = VK_NULL_HANDLE;
        }
        IsIconsLoaded = TRUE;
      } /* End of 'TryLoadIcons' function */

      /* Try to load dropdown images function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID TryLoadImages( anim *Ani )
      {
        if (IsImagesLoaded)
          return;

        const CHAR *image_files[] =
        {
          "bin/textures/ui/images/Blue_cotton_tweed_pxr128.jpg",
          "bin/textures/ui/images/CadNav_Textures_087.JPG",
          "bin/textures/ui/images/tablecloth.jpg",
          "bin/textures/ui/images/TexturesCom_FabricPatterns0091_S.jpg",
          "bin/textures/ui/images/TexturesCom_FabricPatterns0116_3_seamless_S.jpg",
        };

        const CHAR *image_names[] =
        {
          "Blue Cotton Tweed",
          "CadNav Texture",
          "Tablecloth",
          "Fabric Pattern 1",
          "Fabric Pattern 2",
        };

        for (INT i = 0; i < 5; i++)
        {
          image_entry entry;
          entry.Name = image_names[i];
          entry.Tex = Ani->TextureCreate(image_files[i]);
          if (entry.Tex &&
              entry.Tex->Img.View != VK_NULL_HANDLE &&
              entry.Tex->Sampler.Sampler != VK_NULL_HANDLE)
            entry.DescSet = ImGui_ImplVulkan_AddTexture(entry.Tex->Sampler.Sampler,
              entry.Tex->Img.View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
          else
            entry.DescSet = VK_NULL_HANDLE;
          DropdownImages.push_back(entry);
        }
        IsImagesLoaded = TRUE;
      } /* End of 'TryLoadImages' function */

      /* Try to load flat raster texture function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID TryLoadFlat( anim *Ani )
      {
        if (IsFlatLoaded)
          return;

        FlatTexture = Ani->TextureCreate("bin/textures/ui/flats/flat1.png");
        if (FlatTexture &&
            FlatTexture->Img.View != VK_NULL_HANDLE &&
            FlatTexture->Sampler.Sampler != VK_NULL_HANDLE)
          FlatDescSet = ImGui_ImplVulkan_AddTexture(FlatTexture->Sampler.Sampler,
            FlatTexture->Img.View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        else
          FlatDescSet = VK_NULL_HANDLE;
        IsFlatLoaded = TRUE;
      } /* End of 'TryLoadFlat' function */

      /* Try to load project logo texture function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID TryLoadLogo( anim *Ani )
      {
        LogoTexture = Ani->TextureCreate("res/tam_logo.ico");
        if (LogoTexture &&
            LogoTexture->Img.View != VK_NULL_HANDLE &&
            LogoTexture->Sampler.Sampler != VK_NULL_HANDLE)
          LogoDescSet = ImGui_ImplVulkan_AddTexture(LogoTexture->Sampler.Sampler,
            LogoTexture->Img.View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        else
          LogoDescSet = VK_NULL_HANDLE;
      } /* End of 'TryLoadLogo' function */

      /* Load a single system font by file name (without extension) function.
       * ARGUMENTS:
       *   - font file name without extension (e.g. "calibri"):
       *       const CHAR *FontName;
       *   - registry display name map (filename_lc -> display name):
       *       const std::map<std::string, std::string> &DisplayNames;
       * RETURNS: None.
       */
      VOID LoadSystemFont( const CHAR *FontName,
        const std::map<std::string, std::string> &DisplayNames )
      {
        ImGuiIO &io = ImGui::GetIO();
        std::string path = std::string("C:\\Windows\\Fonts\\") + FontName + ".ttf";

        if (ImFont *f = io.Fonts->AddFontFromFileTTF(path.c_str(), 16.0f); f != nullptr)
        {
          LoadedFonts.push_back(f);

          std::string key = FontName;
          for (auto &c : key)
            c = (CHAR)tolower(c);
          auto it = DisplayNames.find(key);
          FontNames.push_back(it != DisplayNames.end() ? it->second : std::string(FontName));
        }
      } /* End of 'LoadSystemFont' function */

      /* Read font display names from Windows registry function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (std::map<std::string, std::string>) map: lowercase filename (no ext) -> display name.
       */
      std::map<std::string, std::string> ReadFontDisplayNames( VOID )
      {
        std::map<std::string, std::string> result;
        HKEY hKey;

        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
            "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts",
            0, KEY_READ, &hKey) != ERROR_SUCCESS)
          return result;

        DWORD index = 0;
        CHAR value_name[256];
        BYTE value_data[MAX_PATH];
        DWORD name_len, data_len, type;

        while (TRUE)
        {
          name_len = sizeof(value_name);
          data_len = sizeof(value_data);
          if (RegEnumValueA(hKey, index++, value_name, &name_len,
              nullptr, &type, value_data, &data_len) != ERROR_SUCCESS)
            break;
          if (type != REG_SZ)
            continue;

          std::string fileName = (const CHAR *)value_data;
          auto dot = fileName.rfind('.');
          if (dot != std::string::npos)
            fileName = fileName.substr(0, dot);
          for (auto &c : fileName)
            c = (CHAR)tolower(c);

          std::string display = value_name;
          auto pos = display.find(" (TrueType)");
          if (pos != std::string::npos)
            display = display.substr(0, pos);

          result[fileName] = display;
        }
        RegCloseKey(hKey);
        return result;
      } /* End of 'ReadFontDisplayNames' function */

      /* Load Windows fonts into ImGui atlas function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID LoadFonts( VOID )
      {
        ImGuiIO &io = ImGui::GetIO();

        LoadedFonts.push_back(io.Fonts->AddFontDefault());
        FontNames.push_back("Default");

        auto DisplayNames = ReadFontDisplayNames();

        const CHAR *font_names[] =
        {
          "calibri", "calibrib", "calibril", "calibrili",
          "consola", "consolab", "consolai", "consolaz",
          "arial", "arialbd", "ariali", "arialbi",
          "times", "timesbd", "timesi", "timesbi",
          "cour", "courbd", "couri", "courbi",
          "segoeui", "segoeuib", "segoeuii", "segoeuiz",
          "verdana", "verdanab", "verdanai", "verdanaz",
          "tahoma", "tahomabd",
          "trebuc", "trebucbd", "trebucbi", "trebucit",
          "georgia", "georgiab", "georgiai", "georgiaz",
          "comic", "comicbd",
        };

        for (auto &name : font_names)
          LoadSystemFont(name, DisplayNames);

        io.Fonts->Build();
      } /* End of 'LoadFonts' function */

    public:

      /* Class constructor.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       */
      ux_test( anim *Ani )
      {
        InitTreeData();
        InitTableData();
        TryLoadIcons(Ani);
        TryLoadImages(Ani);
        TryLoadFlat(Ani);
        TryLoadLogo(Ani);
      } /* End of 'ux_test' function */

      /* Class destructor */
      ~ux_test( VOID ) override
      {
        for (INT i = 0; i < 5; i++)
          if (IconDescSets[i] != VK_NULL_HANDLE)
            ImGui_ImplVulkan_RemoveTexture(IconDescSets[i]);

        for (auto &img : DropdownImages)
          if (img.DescSet != VK_NULL_HANDLE)
            ImGui_ImplVulkan_RemoveTexture(img.DescSet);

        if (FlatDescSet != VK_NULL_HANDLE)
          ImGui_ImplVulkan_RemoveTexture(FlatDescSet);

        if (LogoDescSet != VK_NULL_HANDLE)
          ImGui_ImplVulkan_RemoveTexture(LogoDescSet);
      } /* End of '~ux_test' function */

      /* Render unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Render( anim *Ani ) override
      {
      } /* End of 'Render' function */

      /* Response unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
        if (LoadedFonts.empty())
          LoadFonts();

        if (!IsStyleSaved)
        {
          SavedDarkStyle = ImGui::GetStyle();
          IsStyleSaved = TRUE;
        }

        // Apply selected font and size globally
        ImGuiIO &io = ImGui::GetIO();
        ImGuiStyle &style = ImGui::GetStyle();
        if (SelectedFontIndex < (INT)LoadedFonts.size())
          io.FontDefault = LoadedFonts[SelectedFontIndex];
        style._NextFrameFontSizeBase = FontSize;
        style.Alpha = 1.0f;

        // Recalc sizes
        TabStripWidth = ImGui::GetFontSize() * 2.0f;
        FLT wnd_w = (FLT)Ani->W;
        FLT wnd_h = (FLT)Ani->H;
        if (wnd_w < 100 || wnd_h < 100)
          return;

        // Main menu
        IsAnyPopupOpen = ImGui::IsPopupOpen("", ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel);
        FLT menu_bar_h = 0;
        if (ImGui::BeginMainMenuBar())
        {
          if (LogoDescSet != VK_NULL_HANDLE)
          {
            FLT logo_size = ImGui::GetFrameHeight() - 4.0f;
            ImGui::Image((ImTextureID)LogoDescSet, ImVec2(logo_size, logo_size));
          }
          ImGui::TextColored(style.Colors[ImGuiCol_TextDisabled], "CGSG'Sr'2025 TAM");
          ImGui::Separator();
          if (ImGui::BeginMenu("File"))
          {
            ImGui::MenuItem("New");
            ImGui::MenuItem("Open");
            ImGui::MenuItem("Save");
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4"))
              PostMessage(Ani->hWnd, WM_CLOSE, 0, 0);
            ImGui::EndMenu();
          }
          if (ImGui::BeginMenu("View"))
          {
            if (ImGui::MenuItem(IsPanelOnRight ? "Panel Left" : "Panel Right"))
            {
              IsPanelOnRight = !IsPanelOnRight;
              IsDraggingSplitter = FALSE;
            }
            if (ImGui::MenuItem(Ani->IsFullScreen ? "Go from fullscreen" : "Go to fullscreen", "F11"))
              Ani->FlipFullScreen();
            ImGui::EndMenu();
          }
          if (ImGui::BeginMenu("Options"))
          {
            ImGui::DragFloat("##mf1", &FontSize, 0.1f, 8.0f, 47.0f, "Font Size: %.0f");
            if (ImGui::BeginCombo("##mf2", FontNames[SelectedFontIndex].c_str()))
            {
              for (INT i = 0; i < (INT)FontNames.size(); i++)
              {
                if (ImGui::Selectable(FontNames[i].c_str(), SelectedFontIndex == i))
                  SelectedFontIndex = i;
                if (SelectedFontIndex == i)
                  ImGui::SetItemDefaultFocus();
              }
              ImGui::EndCombo();
            }
            ImGui::Separator();
            if (ImGui::BeginCombo("##ms1", SelectedStyleIndex == 0 ? "Dark Style" : "Light Style"))
            {
              if (ImGui::Selectable("Dark Style", SelectedStyleIndex == 0))
                SelectedStyleIndex = 0, ImGui::GetStyle() = SavedDarkStyle;
              if (SelectedStyleIndex == 0)
                ImGui::SetItemDefaultFocus();
              if (ImGui::Selectable("Light Style", SelectedStyleIndex == 1))
                SelectedStyleIndex = 1, ImGui::StyleColorsLight();
              if (SelectedStyleIndex == 1)
                ImGui::SetItemDefaultFocus();
              ImGui::EndCombo();
            }
            ImGui::EndMenu();
          }
          if (ImGui::BeginMenu("Help"))
          {
            if (ImGui::MenuItem("About TAM"))
              ShowAboutModalFlag = true;
            ImGui::EndMenu();
          }
          menu_bar_h = ImGui::GetWindowSize().y;
          ImGui::EndMainMenuBar();
        }

        FLT panel_y = menu_bar_h;
        wnd_h -= menu_bar_h;

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
        if (IsPanelOnRight)
          Ani->SceneViewport = {0, menu_bar_h, panel_x - splitter_w, wnd_h};
        else
        {
          FLT left = PanelWidth + splitter_w;
          Ani->SceneViewport = {left, menu_bar_h, wnd_w - left, wnd_h};
        }
        //ImGuiStyle &style = ImGui::GetStyle();
        //ImGuiIO &io = ImGui::GetIO();
        BOOL
          splitter_hovered =
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
          }
          else
            IsDraggingSplitter = FALSE;
        }

        if (splitter_hovered || IsDraggingSplitter)
          ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);

        /* Recalculate positions after drag */
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

        /* Draw splitter bar */
        ImDrawList *bg_draw = ImGui::GetBackgroundDrawList();
        ImU32 splitter_color = (splitter_hovered || IsDraggingSplitter) ?
          IM_COL32(80, 160, 240, 255) :
          IM_COL32(40, 80, 140, 200);
        bg_draw->AddRectFilled(ImVec2(splitter_x, panel_y),
          ImVec2(splitter_x + splitter_w, panel_y + wnd_h),
          splitter_color);
        FLT mid_y = panel_y + wnd_h * 0.5f;
        bg_draw->AddLine(ImVec2(splitter_x + splitter_w * 0.5f, mid_y - wnd_h * 0.05f),
          ImVec2(splitter_x + splitter_w * 0.5f, mid_y + wnd_h * 0.05f),
          IM_COL32(160, 200, 255, 200), 2.0f);

        /* Main panel window */
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
          /* Clamp font index */
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

          /* About modal (at window ID scope) */
          if (ShowAboutModalFlag)
          {
            ImGui::OpenPopup("About TAM");
            ShowAboutModalFlag = false;
          }
          DrawAboutModal();
        }
        ImGui::End();
      } /* End of 'Response' function */
    }; /* End of 'ux_test' class */
  } /* end of 'units' namespace */
  static anim::unit_register<units::ux_test> _("VG4 ux test");
} /* end of 'tam' namespace */

/* END OF 'tam_sample_unit_VG4_ux_test.cpp' FILE */
