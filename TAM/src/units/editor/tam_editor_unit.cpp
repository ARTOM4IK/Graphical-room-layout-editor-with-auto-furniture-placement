/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_editor_unit.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Main editor dispatcher unit.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3),
 *               Arsentev Artemy (AA4),
 *               Nechaev Vladimir (VN4),
 *               Kupchenkov Ivan (IK4),
 *               Novozhilova Maryana (MN7),
 *               Tretyakov Andrey (AT7),
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 30.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Class constructor.
 * ARGUMENTS:
 *   - animation context pointer:
 *       anim *Ani;
 */
tam::units::editor::editor( anim *Ani )
{
  std::vector<std::tuple<std::string, std::string, std::string>> Subs
  {
    // {"Info", "Info editor unit", "editor/EditorInfo.png"},
    {"Params", "Parameter editor unit", "editor/icon_info.png"}, // "editor/EditorParameters.png"},
    {"Walls", "Walls editor unit", "editor/icon_walls.png"}, // "editor/EditorWallEditor.png"},
    {"Rooms", "Rooms editor unit", "editor/icon_room.png"}, // "editor/EditorRoom.png"},
    {"Furniture", "Furniture editor unit", "editor/icon_furniture.png"}, // "editor/EditorFurniture.png"},
    {"Placement", "Placement editor unit", "editor/icon_placing.png"}, // "editor/EditorPlacement.png"},
    {"AI", "Bot unit", "editor/icon_bot.png"}, // "editor/EditorBot.png"},
    /// {"Lamps", "Parameter editor unit", "editor/EditorLamps.png"},
  };

  MainFlat.Create(Ani);
  CameraLight = Ani->LightCreate(light_type::ePOINT, "Camera point light", Ani->Cam.Loc, 5.0f, -Ani->Cam.Dir, vec3(1, 1, 0.98), 2, 47.0f);
  Subeditors.reserve(Subs.size());
  for (auto &s : Subs)
  {
    Subeditors.push_back({});
    subeditor &NewSub = Subeditors.back();
    NewSub.Tab = std::move(ux::tab(Ani, std::get<0>(s), ux::icon(std::get<2>(s))));
    auto func = 
      [Ani, this]( VOID )
      {
        static BOOL IsOpen = TRUE;
        IsOpen = ImGui::TreeNodeEx("Drawing settings ##GLOBAL", IsOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);

        if (IsOpen)
        {
          bool Val = (MainFlat.DrawWalls != FALSE);
          ImGui::Checkbox("Draw walls ##GLOBAL", &Val);
          MainFlat.DrawWalls = Val ? TRUE : FALSE;
      
          bool Val1 = (MainFlat.DrawFurniture != FALSE);
          ImGui::Checkbox("Draw furniture ##GLOBAL", &Val1);
          MainFlat.DrawFurniture = Val1 ? TRUE : FALSE;

          bool Val2 = (MainFlat.DrawFloor != FALSE);
          ImGui::Checkbox("Draw floors ##GLOBAL", &Val2);
          MainFlat.DrawFloor = Val2 ? TRUE : FALSE;

          bool Val3 = (MainFlat.DrawCeiling != FALSE);
          ImGui::Checkbox("Draw ceilings ##GLOBAL", &Val3);
          MainFlat.DrawCeiling = Val3 ? TRUE : FALSE;

          bool Val4 = (MainFlat.DrawingScheme != FALSE);
          ImGui::Checkbox("Draw scheme ##GLOBAL", &Val4);
          MainFlat.DrawingScheme = Val4 ? TRUE : FALSE;

          bool Val5 = (Ani->DrawGuidelines != FALSE);
          ImGui::Checkbox("Draw guidelines ##GLOBAL", &Val5);
          Ani->DrawGuidelines = Val5 ? TRUE : FALSE;

          ImGui::TreePop();
        }
      };
    if (UnitNames.find(std::get<1>(s)) != UnitNames.end())
      NewSub.Unit = UnitNames[std::get<1>(s)](Ani, MainFlat, NewSub.Tab, func);
  }
  Ani->SetActiveTabIndex(2);
} /* End of 'tam::units::editor::editor' function */

// Register editor unit
static tam::anim::unit_register<tam::units::editor> _("Editor unit");

/* END OF 'tam_editor_unit.cpp' FILE */
