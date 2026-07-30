/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_walls_editor.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Walls editor unit implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kupchenkov Ivan (IK4),
 *               Novozhilova Maryana (MN7),
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 01.05.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* 'tam' namespace */
namespace tam
{
  /* Units namespace */
  namespace units
  {
    /* Action type enumeration */
    enum class action_type
    {
      eNOTHING, eMOVEPOINT, eMOVESEGMENT, eADDPOINT, eADDSEGMENT, eDELETEPOINT, eDELETESEGMENT, eDELETEBIGSELECTION
    }; /* End of 'action_type' enumeration */

    /* Action structure */
    struct action
    {
      action_type Action = action_type::eNOTHING;                // Action (moving/adding/deleting)
      INT Point = -1;                                            // Changed point for undo its changes after Ctrl + Z
      vec2 PointPos = vec2(INFINITY);                            // Position of last changed point
      std::vector<INT> PointPoints;                              // Points which were connected to last changed point
      std::vector<vec2> PointPointsPos;                          // Points which were connected to last changed point
      std::map<std::string, std::vector<INT>> PointRoomsPoints;  // Map for restoring rooms after Ctrl + Z
      INT Segment = -1;                                          // Changed segment for undo its changes after Ctrl + Z
      INT Selected = -1;                                         // Save last selected point for continue from it after Ctrl + Z
      vec2 SelectedPos = vec2(INFINITY);                         // Position of last selected point
      INT SegmentSavePoint1 = -1, SegmentSavePoint2 = -1;        // Save segment points for create it after deleting after Ctrl + Z
      vec2 MoveStartPos = vec2(INFINITY);                        // Save mouse position when it starts moving (for Ctrl + Z)
      vec2 SaveDPos = vec2(INFINITY);                            // Save point position change to undo it after Ctrl + Z
      std::vector<segment_type> SegmentType;                     // Segment type
      std::vector<vec4> WinParams;                               // Segment window offsets (X - left, Y - right, Z - top, W - bottom)
      std::vector<vec2> DoorParams;                              // Segment door params

      std::map<INT, segment> SaveSegments;                       // Segments from big selection
      std::map<INT, segment_point> SaveSegmentPoints;            // Segment points from big selection
      std::map<INT, segment_wall> SaveSegmentWalls;              // Segment walls from big selection
    }; /* End of 'action' structure */

    /* Wall editor unit representation class */
    class walls_editor : public editor_unit
    {
    private:
      // Hotkey tip texts
      const std::string Hotkeys =
        "LMB  =>  add point or select existing\n"
        "SHIFT + LMB + MOUSE MOVE => move active point\n"
        "ALT  =>  clamp point to grid\n"
        "Y  =>  delete selected points and segments\n"
        "Ctrl + Z => undo (moving/adding/deleting)\n"
        "RMB  =>  delete from selection\n"
        "E or Esc  =>  clear selection\n"
        "Q  =>  change selecting mode (points/segments)\n"
        "G  =>  ON/OFF drawing walls\n";
      ux::tab &Tab;                                                             // Walls editor tab
      ux::user_type DrawSettings {Tab};                                         // User type drawing settings
      ux::group_begin GrOpenH {Tab, "Hotkeys"};
      ux::text_type HotkeysText {Tab, Hotkeys};                                 // Text with hotkeys
      ux::group_end GeEndH {GrOpenH};

      /* Edit points and segments */
      ux::separator_type SelectionSeparator {Tab, "Selections management"};     // Selections management separator
      ux::button ClearSelectionButton {Tab, "Clear selection ('E' or 'Esc')"};  // Button for connecting selected points by segments
      ux::button DeleteButton {Tab, "Delete selected ('Y')"};                   // Button for deleting selected points or segments

      /* Edit mode settings */
      ux::separator_type EditSeparator {Tab, "Select edit mode ('Q'):"};        // Edit management separator
      BOOL SelectTypeBool = TRUE;                                               // Select type bool : TRUE - points, FALSE - segments
      ux::radio_type SelectType {Tab, "", {"Points", "Segments and walls"}, 0}; // Selector for edit modes
      ux::user_type ModeSettings {Tab};                                         // User type for mode settings
      FLT PointFindOffset = 0.30;                                               // Point find offset
      FLT SegmentFindOffset = 0.30;                                             // Segment find offset
      FLT
        WindowLeftOffset = 0.3, WindowRightOffset = 0.3,                        // Window's left and right sides distances from left side
        WindowBottomOffset = 0.5, WindowTopOffset = 0.3;                        // Window distances from top and bottom sides
      FLT DoorWidth = 1.0;                                                      // Door width
      FLT DoorHeight = 2.3;                                                     // Door height
      segment_type CurrentSegmentType = segment_type::eWALL;                    // Currecnt segment type (for selected segments)
      INT SegmentType = 0;                                                      // Selected segment type 

      /* Walls settings */
      ux::separator_type WallsSeparator {Tab, "Walls settings"};                // Walls mode separator
      ux::flt_type WallsWidth {Tab, "Walls width", 0.08, 0.05, 3.0};            // Walls width
      ux::flt_type WallsHeight {Tab, "Walls height", 3.0, 0.5, 8.0};            // Walls height

      /* Scheme settings */
      ux::separator_type SchemesSeparator {Tab, "Schemes settings"};            // Schemes settings separator
      ux::button LoadScheme {Tab, "Load scheme"};                               // Loading wallpaper textures button
      ux::flt_type SchemeScale {Tab, "Scheme scale", 3.0, 0.1, 10.0};           // Schemes scaling coefficient slider
      ux::flt_type SchemeRotation {Tab, "Scheme rotate", 0.0, -180.0, 180.0};   // Schemes rotating coefficient slider
      ux::flt_type SchemeXOffset {Tab, "X offset", 0.0, -30.0, 30.0};           // Schemes x offset coefficient slider
      ux::flt_type SchemeYOffset {Tab, "Y offset", 0.0, -30.0, 30.0};           // Schemes y offset coefficient slider
      ux::bool_type MovingScheme {Tab, "Scheme move mode", FALSE, "No", "Yes"}; // Flag for moving scheme
      BOOL UsingScheme = FALSE;                                                 // Flag for using scheme

      /* Draw mode settings */
      ux::separator_type DrawSeparator {Tab, "Draw mode settings"};             // Draw mode separator
      ux::flt_type DebugLayoutRadius {Tab, "Debug radius", 0.18, 0.01, 1.8};    // Debug radius of spheres and cylinders
      ux::bool_type DrawLayout {Tab, "Draw layout", TRUE, "No", "Yes"};         // Draw points and segments flag
      ux::bool_type DrawPoints {Tab, "Draw points", FALSE, "No", "Yes"};        // Draw only points flag
      ux::bool_type DrawRuler {Tab, "Draw ruler", TRUE, "No", "Yes"};           // Draw ruler flag

      INT LastAccessFrame {};                                                   // Last frame when unit was active
    public:
      std::vector<INT> SelectedPoints {};                                       // Selected points array
      std::vector<INT> SelectedSegments {};                                     // Selected points array
      INT
        ActivePoint = -1,                                                       // Active point index
        ActiveSegment = -1;                                                     // Active segment index
      vec2 MousePoint {};                                                       // point of cursor on plane

      std::vector<action> LastActions;                                          // Last actions

      vec2 SelectionStartPointM;                                                // Selection start point (on screen)
      vec2 SelectionEndPointM;                                                  // Selection end point (on screen)

      vec2 SelectionStartPoint;                                                 // Big selection start point
      vec2 SelectionEndPoint = vec2(INFINITY);                                  // Big selection end point
      vec2 SelectionSub1Point;                                                  // Big selection sub point 1
      vec2 SelectionSub2Point;                                                  // Big selection sub point 2
      std::vector<INT> BigSelection;                                            // Indices of points/segments in selection

      font Fnt, FntPoint;                                                       // Fonts

      /* Class constructor.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       *   - edit flat reference:
       *       flat &WorkFlat;
       *   - UX tab control reference:
       *       ux::tab &WorkTab;
       *   - UX function reference for user_type:
       *       const std::function<VOID ( VOID )> &UserUIFunc;
       */
      walls_editor( anim *Ani, flat &WorkFlat, ux::tab &WorkTab, const std::function<VOID ( VOID )> &UserUIFunc ) :
        editor_unit(Ani, WorkFlat), Tab(WorkTab)
      {
        Fnt = Ani->FontCreate("consolas", vec4(1, 0, 0, 1.0));
        FntPoint = Ani->FontCreate("consolas", vec4(1, 0, 0, 1.0));
        UsingScheme = FALSE;
        SchemeXOffset = Flat.SchemeTranslate.X;
        SchemeYOffset = Flat.SchemeTranslate.Z;
        SchemeRotation = Flat.SchemeRotate;
        SchemeScale = Flat.SchemeScale;

        DrawSettings.SetUserFunc(UserUIFunc);
        ModeSettings.SetUserFunc([Ani, this](VOID)
        {
          ImGuiWindow *Window = ImGui::GetCurrentWindow();

          ImGui::AlignTextToFramePadding();
          if (SelectTypeBool)
          {
            ImGui::SeparatorText("Points editing");
            ImGui::Text("Points find offset:");
            ImGui::SliderFloat("##Points find offset", &PointFindOffset, 0.08, 1.8);
            ImGui::SameLine();
            if (ImGui::Button(std::format("Default##setDef{}", "Points find offset").c_str()))
              PointFindOffset = 0.30;
            ImGui::Text("Segments find offset:");
            ImGui::SliderFloat("##Segments find offset", &SegmentFindOffset, 0.08, 1.8);
            ImGui::SameLine();
            if (ImGui::Button(std::format("Default##setDef{}", "Segments find offset").c_str()))
              SegmentFindOffset = 0.30;
          }
          else
          {
            ImGui::SeparatorText("Segments editing");
            ImGui::Text("Segments find offset:");
            ImGui::SliderFloat("##Segments find offset", &SegmentFindOffset, 0.08, 1.8);
            ImGui::SameLine();
            if (ImGui::Button(std::format("Default##setDef{}", "Segments find offset").c_str()))
              SegmentFindOffset = 0.30;

            if (SelectedSegments.size() < 1)
              return;

            ImGui::Text("Select segment type ('1', '2', '3'):");

            std::string item_id = std::string("Wall") + "##si" + std::to_string(0);
            ImGui::RadioButton(item_id.c_str(), &SegmentType, 0);
            if ((SegmentType == 0))
              ImGui::SetItemDefaultFocus();
            ImGui::SameLine();

            item_id = std::string("Window") + "##si" + std::to_string(1);
            ImGui::RadioButton(item_id.c_str(), &SegmentType, 1);
            if ((SegmentType == 1))
              ImGui::SetItemDefaultFocus();
            ImGui::SameLine();

            item_id = std::string("Door") + "##si" + std::to_string(2);
            ImGui::RadioButton(item_id.c_str(), &SegmentType, 2);
            if ((SegmentType == 2))
              ImGui::SetItemDefaultFocus();

            if ((SegmentType == 1))
            {
              ImGui::Text("Window left offset");
              ImGui::SameLine();
              ImGui::SliderFloat("##Window left offset", &WindowLeftOffset, 0.0, 30.0);

              ImGui::Text("Window right offset");
              ImGui::SameLine();
              ImGui::SliderFloat("##Window right offset", &WindowRightOffset, 0.0, 30.0);

              ImGui::Text("Window bottom offset");
              ImGui::SameLine();
              ImGui::SliderFloat("##Window bottom offset", &WindowBottomOffset, 0.0, WallsHeight);

              ImGui::Text("Window top offset");
              ImGui::SameLine();
              ImGui::SliderFloat("##Window top offset", &WindowTopOffset, 0.0, WallsHeight);
            }

            else if ((SegmentType == 2))
            {
              ImGui::Text("Door width");
              ImGui::SameLine();
              ImGui::SliderFloat("##Door width", &DoorWidth, 0.1, 8.0);

              ImGui::Text("Door height");
              ImGui::SameLine();
              ImGui::SliderFloat("##Door height", &DoorHeight, 0.30, WallsHeight);
            }
          }
        });
      } /* End of 'furniture_editor' function */

      /* Get response from unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
        static BOOL IsBigSelection = FALSE;

        // Max for saved actions
        if (LastActions.size() > 47)
        {
          std::vector<action> Save = LastActions;
          LastActions.clear();
          for (INT i = 8; i < Save.size(); i++)
            LastActions.push_back(Save[i]);
          Save.clear();
        }

        // Check if unit was unactive to clear selected points / segments
        if (LastAccessFrame != Ani->CurrentFrameNo - 1)
        {
          SelectedPoints.clear();
          SelectedSegments.clear();
          ActivePoint = -1;
          ActiveSegment = -1;
        }

        LastAccessFrame = Ani->CurrentFrameNo;

        if (LoadScheme)
        {
          std::string p = Ani->OpenFileDialog("Load scheme", "Schemes\0*.*\0");
          if (!p.empty())
          {
            texture tex = Ani->TextureCreate(p);
            if (tex != nullptr)
            {
              Flat.SchemeMaterial->Update({tex}, Flat.SchemeMaterial->Buffers);

              FLT Sc = (FLT)tex->W / tex->H;

              std::vector<vertex_std> Verts
              {
                { vec3(-Sc * 3.0, -0.047, -3.0), {0, 0}, {0, 1, 0}, vec4(1, 1, 1, 1) },
                { vec3(-Sc * 3.0, -0.047, 3.0), {0, 1}, {0, 1, 0}, vec4(1, 1, 1, 1) },
                { vec3(Sc * 3.0, -0.047, 3.0), {1, 1}, {0, 1, 0}, vec4(1, 1, 1, 1) },
                { vec3(Sc * 3.0, -0.047, -3.0), {1, 0}, {0, 1, 0}, vec4(1, 1, 1, 1) }
              };

              std::vector<INT> Inds
              {
                0, 1, 2,
                0, 3, 2
              };

              Flat.SchemePrim = Ani->PrimitiveCreate<vertex_std>(Flat.SchemeMaterial, std::span(Verts), std::span(Inds));
              Flat.SchemeTex = p;
              Flat.SchemeScale = 3.0;
              Flat.DrawingScheme = TRUE;
            }
          }
        }

        if (!UsingScheme && Flat.SchemeScale > 0)
        {
          SchemeXOffset = Flat.SchemeTranslate.X;
          SchemeYOffset = Flat.SchemeTranslate.Z;
          SchemeRotation = Flat.SchemeRotate;
          SchemeScale = Flat.SchemeScale;
          UsingScheme = TRUE;
        }

        if (UsingScheme && Flat.SchemeScale == 0)
          UsingScheme = FALSE;

        static vec2 PrevMousePoint = vec2(INFINITY);
        if (MovingScheme && Ani->Keys[VK_LBUTTON] && !Ani->Keys[VK_LCONTROL])
        {
          // Get cursor point on plane
          mth::ray<FLT> Rs = Ani->Cam.FrameRay(Ani->Mx, Ani->My);
          DBL ts = Rs.IntersectPlane();
          if (!(vec2(Rs(ts).X, Rs(ts).Z) - vec2(Ani->Cam.Loc.X, Ani->Cam.Loc.Z)) >= 300 || ts <= 0)
            return;
          MousePoint = vec2(Rs(ts).X, Rs(ts).Z);

          // Clamp point to integer coordinates
          if (Ani->Keys[VK_MENU])
          {
            MousePoint = vec2(std::roundf(MousePoint.X), std::roundf(MousePoint.Y));
            PrevMousePoint = vec2(std::roundf(PrevMousePoint.X), std::roundf(PrevMousePoint.Y));
          }

          if (PrevMousePoint.X == INFINITY)
            PrevMousePoint = MousePoint;
          else
          {
            SchemeXOffset = MousePoint.X - PrevMousePoint.X;
            SchemeYOffset = MousePoint.Y - PrevMousePoint.Y;
          }
          return;
        }
        else if (PrevMousePoint.X != INFINITY)
          PrevMousePoint = vec2(INFINITY);

        // Drawing for big selection
        static BOOL IsSelectionStarted = FALSE;
        static BOOL IsSelection = FALSE;

        if (Ani->Keys['W'] || Ani->Keys['S'] || Ani->Keys['A'] || Ani->Keys['D'] || Ani->Keys['K'] || Ani->Keys['L'])
        {
          IsSelectionStarted = FALSE;
          IsSelection = FALSE;
          SelectionEndPoint = vec2(INFINITY);
          IsBigSelection = FALSE;
        }

        if (IsSelectionStarted && !Ani->Keys[VK_LCONTROL])
          Ani->DrawSphere(vec3(SelectionStartPoint.X, 0, SelectionStartPoint.Y), !(vec3(SelectionStartPoint.X, 0, SelectionStartPoint.Y) - Ani->Cam.Loc) / 180, vec4(1, 0.6, 0, 1));

        if (IsSelection && IsSelectionStarted && SelectionEndPoint.X != INFINITY && !Ani->Keys[VK_LCONTROL])
        {
          Ani->DrawCylinder(vec3(SelectionStartPoint.X, 0, SelectionStartPoint.Y), !(vec3(SelectionStartPoint.X, 0, SelectionStartPoint.Y) - Ani->Cam.Loc) / 300, vec3(SelectionSub1Point.X, 0, SelectionSub1Point.Y), !(vec3(SelectionSub1Point.X, 0, SelectionSub1Point.Y) - Ani->Cam.Loc) / 300, vec4(1, 0.74, 0, 1));
          Ani->DrawCylinder(vec3(SelectionEndPoint.X, 0, SelectionEndPoint.Y), !(vec3(SelectionEndPoint.X, 0, SelectionEndPoint.Y) - Ani->Cam.Loc) / 300, vec3(SelectionSub1Point.X, 0, SelectionSub1Point.Y), !(vec3(SelectionSub1Point.X, 0, SelectionSub1Point.Y) - Ani->Cam.Loc) / 300, vec4(1, 0.74, 0, 1));
          Ani->DrawCylinder(vec3(SelectionEndPoint.X, 0, SelectionEndPoint.Y), !(vec3(SelectionEndPoint.X, 0, SelectionEndPoint.Y) - Ani->Cam.Loc) / 300, vec3(SelectionSub2Point.X, 0, SelectionSub2Point.Y), !(vec3(SelectionSub2Point.X, 0, SelectionSub2Point.Y) - Ani->Cam.Loc) / 300, vec4(1, 0.74, 0, 1));
          Ani->DrawCylinder(vec3(SelectionStartPoint.X, 0, SelectionStartPoint.Y), !(vec3(SelectionStartPoint.X, 0, SelectionStartPoint.Y) - Ani->Cam.Loc) / 300, vec3(SelectionSub2Point.X, 0, SelectionSub2Point.Y), !(vec3(SelectionSub2Point.X, 0, SelectionSub2Point.Y) - Ani->Cam.Loc) / 300, vec4(1, 0.74, 0, 1));
        }

        // Save old params
        static vec2 OldPos = vec2(INFINITY);

        static FLT OldWallsHeight = 3.0;
        static FLT OldWallsWidth = 0.8;

        // Walls width and height params
        if (WallsHeight != OldWallsHeight || WallsWidth != OldWallsWidth)
        {
          Flat.WallsHeight = WallsHeight;
          Flat.WallsWidth = WallsWidth;
          Flat.UpdateSelectedSegments({}, INFINITY, INFINITY);
          for (auto &ind : SelectedSegments)
            Flat.ChangeSegmentType(ind, Flat.Segments[ind].Type);
          OldWallsHeight = WallsHeight;
          OldWallsWidth = WallsWidth;
        }

        // Ctrl + Z part
        if (Ani->Keys[VK_LCONTROL] && Ani->KeysClick['Z'] && LastActions.size() > 0)
        {
          auto Last = LastActions[LastActions.size() - 1];
          if (Last.Action == action_type::eNOTHING)
          {
            LastActions.pop_back();
            return;
          }
          else if (Last.Action == action_type::eMOVEPOINT && Last.SaveDPos != vec2(INFINITY) && Last.SaveDPos != vec2(-INFINITY) && Last.Point != -1)
          {
            Flat.UpdateSelectedPoints({Last.Point}, -Last.SaveDPos.X, -Last.SaveDPos.Y);
            Last.Action = action_type::eNOTHING;
            Last.Point = -1;
            Last.SaveDPos = vec2(INFINITY);
            LastActions.pop_back();
          }
          else if (Last.Action == action_type::eMOVESEGMENT && Last.SaveDPos != vec2(INFINITY) && Last.SaveDPos != vec2(-INFINITY) && Last.Segment != -1)
          {
            Flat.UpdateSelectedSegments({Last.Segment}, -Last.SaveDPos.X, -Last.SaveDPos.Y);
            Last.Action = action_type::eNOTHING;
            Last.Segment = -1;
            Last.SaveDPos = vec2(INFINITY);
            LastActions.pop_back();
          }
          else if (Last.Action == action_type::eDELETESEGMENT && Last.SegmentSavePoint1 != -1 && Last.SegmentSavePoint2 != -1)
          {
            Last.SegmentSavePoint1 = Flat.AddPoint(Last.PointPointsPos[0].X, Last.PointPointsPos[0].Y);
            Last.SegmentSavePoint2 = Flat.AddPoint(Last.PointPointsPos[1].X, Last.PointPointsPos[1].Y);
            INT SaveS = Flat.AddSegment(Last.SegmentSavePoint1, Last.SegmentSavePoint2);
            if (Last.SegmentType[0] != segment_type::eWALL)
            {
              Flat.Segments[SaveS].DoorWidth = Last.DoorParams[0].X;
              Flat.Segments[SaveS].DoorHeight = Last.DoorParams[0].Y;
              Flat.Segments[SaveS].WindowLeftDist = Last.WinParams[0].X;
              Flat.Segments[SaveS].WindowRightDist = Last.WinParams[0].Y;
              Flat.Segments[SaveS].WindowTopDist = Last.WinParams[0].Z;
              Flat.Segments[SaveS].WindowBottomDist = Last.WinParams[0].W;
              Flat.ChangeSegmentType(SaveS, Last.SegmentType[0]);
            }
            Last.SegmentType.clear();
            Last.WinParams.clear();
            Last.DoorParams.clear();
            Last.SegmentSavePoint1 = -1;
            Last.SegmentSavePoint2 = -1;
            Last.Action = action_type::eNOTHING;
            LastActions.pop_back();
          }
          else if (Last.Action == action_type::eDELETEPOINT && Last.Point != -1)
          {
            INT SavePoint = Flat.AddPoint(Last.PointPos.X, Last.PointPos.Y);
            INT i = 0;
            for (auto &Point : Last.PointPoints)
            {
              INT PPoint = Flat.AddPoint(Last.PointPointsPos[i].X, Last.PointPointsPos[i].Y);
              INT SaveS = Flat.AddSegment(SavePoint, PPoint);
              if (Last.SegmentType[i] != segment_type::eWALL)
              {
                Flat.Segments[SaveS].DoorWidth = Last.DoorParams[i].X;
                Flat.Segments[SaveS].DoorHeight = Last.DoorParams[i].Y;
                Flat.Segments[SaveS].WindowLeftDist = Last.WinParams[i].X;
                Flat.Segments[SaveS].WindowRightDist = Last.WinParams[i].Y;
                Flat.Segments[SaveS].WindowTopDist = Last.WinParams[i].Z;
                Flat.Segments[SaveS].WindowBottomDist = Last.WinParams[i].W;
                Flat.ChangeSegmentType(SaveS, Last.SegmentType[i]);
              }
              i++;
            }
            for (auto &[Name, Points] : Last.PointRoomsPoints)
            {
              Flat.SegmentPoints[SavePoint].RoomNames.push_back(Name);
              Flat.Rooms[Name].RoomPoints.clear();
              for (auto &Point : Points)
              {
                if (Point == Last.Point)
                {
                  Flat.Rooms[Name].RoomPoints.push_back(SavePoint);
                  continue;
                }
                else
                  Flat.Rooms[Name].RoomPoints.push_back(Point);
              }
            }
            for (auto &[Name, Points] : Last.PointRoomsPoints)
              Flat.Rooms[Name].RoomFloor.UpdateFloorPrimitive();
            SelectedPoints.clear();
            SelectedPoints.push_back(SavePoint);
            Last.Selected = SavePoint;
            Last.Point = -1;
            Last.PointPoints.clear();
            Last.PointRoomsPoints.clear();
            Last.PointPos = vec2(INFINITY);
            Last.Action = action_type::eNOTHING;
            LastActions.pop_back();
          }
          else if (Last.Action == action_type::eADDPOINT && Last.Point != -1 && Last.SegmentSavePoint1 == -1 && Last.SegmentSavePoint2 == -1)
          {
            INT LastPoint = Flat.AddPoint(Last.PointPos.X, Last.PointPos.Y);
            Flat.DeletePoint(LastPoint);
            Last.Point = -1;
            Last.PointPoints.clear();
            Last.PointPos = vec2(INFINITY);
            Last.Action = action_type::eNOTHING;
            SelectedPoints.clear();
            if (Last.Selected != -1)
              SelectedPoints.push_back(Flat.AddPoint(Last.SelectedPos.X, Last.SelectedPos.Y));
            LastActions.pop_back();
          }
          else if (Last.Action == action_type::eADDPOINT && Last.Point != -1 && Last.SegmentSavePoint1 != -1 && Last.SegmentSavePoint2 != -1)
          {
            INT LastPoint = Flat.AddPoint(Last.PointPos.X, Last.PointPos.Y);
            Flat.DeletePoint(LastPoint);
            Last.Point = -1;
            Last.PointPoints.clear();
            Last.PointPos = vec2(INFINITY);
            INT SaveS = Flat.AddSegment(Last.SegmentSavePoint1, Last.SegmentSavePoint2);
            if (Last.SegmentType[0] != segment_type::eWALL)
            {
              Flat.Segments[SaveS].DoorWidth = Last.DoorParams[0].X;
              Flat.Segments[SaveS].DoorHeight = Last.DoorParams[0].Y;
              Flat.Segments[SaveS].WindowLeftDist = Last.WinParams[0].X;
              Flat.Segments[SaveS].WindowRightDist = Last.WinParams[0].Y;
              Flat.Segments[SaveS].WindowTopDist = Last.WinParams[0].Z;
              Flat.Segments[SaveS].WindowBottomDist = Last.WinParams[0].W;
              Flat.ChangeSegmentType(SaveS, Last.SegmentType[0]);
            }
            Last.Action = action_type::eNOTHING;
            Last.SegmentType.clear();
            Last.DoorParams.clear();
            Last.WinParams.clear();
            Last.SegmentSavePoint1 = -1;
            Last.SegmentSavePoint2 = -1;
            SelectedPoints.clear();
            if (Last.Selected != -1)
              SelectedPoints.push_back(Flat.AddPoint(Last.SelectedPos.X, Last.SelectedPos.Y));
            LastActions.pop_back();
          }
          else if (Last.Action == action_type::eADDSEGMENT && Last.Segment != -1)
          {
            Flat.DeleteSegment(Last.Segment);
            Last.Segment = -1;
            Last.Action = action_type::eNOTHING;
            SelectedPoints.clear();
            if (Last.Selected != -1)
              SelectedPoints.push_back(Flat.AddPoint(Last.SelectedPos.X, Last.SelectedPos.Y));
            LastActions.pop_back();
          }
          else if (Last.Action == action_type::eDELETEBIGSELECTION)
          {
            Flat.SegmentPoints = Last.SaveSegmentPoints;
            Flat.Segments = Last.SaveSegments;
            Flat.SegmentWalls = Last.SaveSegmentWalls;
            LastActions.pop_back();
          }
        }

        // Clear selection
        if (Ani->KeysClick[VK_ESCAPE] || Ani->KeysClick['E'] || ClearSelectionButton)
        {
          SelectedPoints.clear();
          SelectedSegments.clear();
          ActivePoint = -1;
          ActiveSegment = -1;
        }

        // Segment type
        if (Ani->KeysClick['Q'])
          SelectType = (SelectType == "Points" ? 1 : 0);
        if (SelectType == "Points")
          SelectTypeBool = TRUE, SelectedSegments.clear();
        else
          SelectTypeBool = FALSE;

        if (Ani->KeysClick['G'])
          Flat.DrawWalls = !Flat.DrawWalls;

        // Handle windows and doors creation (selected segment type change)
        if ((UINT)SelectedSegments.size() > 0)
        {
          if (Ani->KeysClick['1'])
            SegmentType = 0;
          else if (Ani->KeysClick['2'])
            SegmentType = 1;
          else if (Ani->KeysClick['3'])
            SegmentType = 2;
        }

        // Handle segment type selection
        if (SegmentType == 0)
          CurrentSegmentType = segment_type::eWALL;
        else if (SegmentType == 1)
          CurrentSegmentType = segment_type::eWIN;
        else
          CurrentSegmentType = segment_type::eDOOR;

        for (auto &ind : SelectedSegments)
        {
          if (ind == SelectedSegments[SelectedSegments.size() - 1])
          {
            if (CurrentSegmentType == segment_type::eWIN && Flat.Segments[ind].Type == CurrentSegmentType)
            {
              Flat.Segments[ind].DoorWidth = Flat.Segments[ind].DoorHeight = 0;
              Flat.Segments[ind].WindowLeftDist = WindowLeftOffset;
              Flat.Segments[ind].WindowRightDist = WindowRightOffset;
              Flat.Segments[ind].WindowBottomDist = WindowBottomOffset;
              Flat.Segments[ind].WindowTopDist = WindowTopOffset;
            }
            else if (CurrentSegmentType == segment_type::eWIN)
            {
              DoorWidth = 1.0;
              DoorHeight = 2.3;
              if (Flat.Segments[ind].WindowLeftDist != 0)
                WindowLeftOffset = Flat.Segments[ind].WindowLeftDist;
              if (Flat.Segments[ind].WindowRightDist != 0)
                WindowRightOffset = Flat.Segments[ind].WindowRightDist;
              if (Flat.Segments[ind].WindowBottomDist != 0)
                WindowBottomOffset = Flat.Segments[ind].WindowBottomDist;
              if (Flat.Segments[ind].WindowTopDist != 0)
                WindowTopOffset = Flat.Segments[ind].WindowTopDist;
            }
            else if (CurrentSegmentType == segment_type::eDOOR && Flat.Segments[ind].Type == CurrentSegmentType)
            {
              Flat.Segments[ind].WindowLeftDist = Flat.Segments[ind].WindowRightDist = 0;
              Flat.Segments[ind].WindowBottomDist = Flat.Segments[ind].WindowTopDist = 0;
              Flat.Segments[ind].DoorWidth = DoorWidth;
              Flat.Segments[ind].DoorHeight = DoorHeight;
            }
            else if (CurrentSegmentType == segment_type::eDOOR)
            {
              WindowLeftOffset = 0.3, WindowRightOffset = 0.3;
              WindowBottomOffset = 0.5, WindowTopOffset = 0.3;
              if (Flat.Segments[ind].DoorWidth != 0)
                DoorWidth = Flat.Segments[ind].DoorWidth;
              if (Flat.Segments[ind].DoorHeight != 0)
                DoorHeight = Flat.Segments[ind].DoorHeight;
            }
            Flat.ChangeSegmentType(ind, CurrentSegmentType);
          }
        }

        // Get cursor point on plane
        mth::ray<FLT> R = Ani->Cam.FrameRay(Ani->Mx, Ani->My);
        DBL t = R.IntersectPlane();
        if (!(vec2(R(t).X, R(t).Z) - vec2(Ani->Cam.Loc.X, Ani->Cam.Loc.Z)) >= 300 || t <= 0)
          return;
        MousePoint = vec2(R(t).X, R(t).Z);

        ActivePoint = Flat.FindNearestPoint(MousePoint.X, MousePoint.Y, PointFindOffset);
        ActiveSegment = Flat.FindNearestSegment(MousePoint.X, MousePoint.Y, SegmentFindOffset);

        // Clamp point to integer coordinates
        if (Ani->Keys[VK_MENU] && ActivePoint == -1)
        {
          if (abs(MousePoint.X - std::roundf(MousePoint.X)) > abs(MousePoint.Y - std::roundf(MousePoint.Y)) + 0.1)
            MousePoint = vec2(MousePoint.X, std::roundf(MousePoint.Y));
          else if (abs(MousePoint.X - std::roundf(MousePoint.X)) + 0.1 < abs(MousePoint.Y - std::roundf(MousePoint.Y)))
            MousePoint = vec2(std::roundf(MousePoint.X), MousePoint.Y);
          else
            MousePoint = vec2(std::roundf(MousePoint.X), std::roundf(MousePoint.Y));
        }

        if (Ani->KeysClick[VK_RBUTTON] && (!Ani->Keys[VK_LCONTROL]))
        {
          // Delete point from selected
          if (ActivePoint != -1 && SelectedPoints.size() > 0 && SelectTypeBool)
            if (auto DelPoint = std::find(SelectedPoints.begin(), SelectedPoints.end(), ActivePoint); DelPoint != SelectedPoints.end())
              SelectedPoints.erase(DelPoint);
          // Delete segment from selected
          if (ActiveSegment != -1  && SelectedSegments.size() > 0 && !SelectTypeBool)
            if (auto DelSegment = std::find(SelectedSegments.begin(), SelectedSegments.end(), ActiveSegment); DelSegment != SelectedSegments.end())
              SelectedSegments.erase(DelSegment);
        }

        static BOOL WasShift = FALSE;
        static BOOL WasLButton = FALSE;
        static BOOL WasMove = FALSE;

        // Ruler
        if (DrawRuler && SelectedPoints.size() > 0 && SelectTypeBool && !Ani->Keys[VK_SPACE] && !IsBigSelection && ActivePoint == -1)
        {
          Fnt->DrawBB(std::format("{:.3f}m", !(MousePoint - Flat.SegmentPoints[SelectedPoints[0]].Pos)), vec3((MousePoint + Flat.SegmentPoints[SelectedPoints[0]].Pos).X / 2, 0.18 + 0.08 * !(Ani->Cam.Loc - vec3((MousePoint + Flat.SegmentPoints[SelectedPoints[0]].Pos).X / 2, 0.18, (MousePoint + Flat.SegmentPoints[SelectedPoints[0]].Pos).Y / 2)), (MousePoint + Flat.SegmentPoints[SelectedPoints[0]].Pos).Y / 2), 0.047 * !(Ani->Cam.Loc - vec3((MousePoint + Flat.SegmentPoints[SelectedPoints[0]].Pos).X / 2, 0.18, (MousePoint + Flat.SegmentPoints[SelectedPoints[0]].Pos).Y / 2)));
          Ani->DrawLine(vec3(Flat.SegmentPoints[SelectedPoints[0]].Pos.X, 0, Flat.SegmentPoints[SelectedPoints[0]].Pos.Y), vec3(MousePoint.X, 0, MousePoint.Y), vec4(1, 0, 0, 1));
          for (INT i = 0; i <= !(MousePoint - Flat.SegmentPoints[SelectedPoints[0]].Pos); i++)
          {
            vec2 Pos = vec2(Flat.SegmentPoints[SelectedPoints[0]].Pos + (MousePoint - Flat.SegmentPoints[SelectedPoints[0]].Pos).Normalize() * i);
            vec3 Perp = (vec3((MousePoint - Flat.SegmentPoints[SelectedPoints[0]].Pos).Normalize().X, 0, (MousePoint - Flat.SegmentPoints[SelectedPoints[0]].Pos).Normalize().Y) % vec3(0, 1, 0)).Normalize() * 0.30;
            Ani->DrawLine(vec3(Pos.X - Perp.X, 0, Pos.Y - Perp.Z), vec3(Pos.X + Perp.X, 0, Pos.Y + Perp.Z), vec4(1, 0, 0, 1));
            if (i > 3000)
              break;
          }
        }
        else if (DrawRuler && SelectedPoints.size() > 0 && SelectTypeBool && !Ani->Keys[VK_SPACE] && !IsBigSelection)
        {
          Fnt->DrawBB(std::format("{:.3f}m", !(Flat.SegmentPoints[ActivePoint].Pos - Flat.SegmentPoints[SelectedPoints[0]].Pos)), vec3((Flat.SegmentPoints[ActivePoint].Pos + Flat.SegmentPoints[SelectedPoints[0]].Pos).X / 2, 0.18 + 0.08 * !(Ani->Cam.Loc - vec3((Flat.SegmentPoints[ActivePoint].Pos + Flat.SegmentPoints[SelectedPoints[0]].Pos).X / 2, 0.18, (Flat.SegmentPoints[ActivePoint].Pos + Flat.SegmentPoints[SelectedPoints[0]].Pos).Y / 2)), (Flat.SegmentPoints[ActivePoint].Pos + Flat.SegmentPoints[SelectedPoints[0]].Pos).Y / 2), 0.047 * !(Ani->Cam.Loc - vec3((Flat.SegmentPoints[ActivePoint].Pos + Flat.SegmentPoints[SelectedPoints[0]].Pos).X / 2, 0.18, (Flat.SegmentPoints[ActivePoint].Pos + Flat.SegmentPoints[SelectedPoints[0]].Pos).Y / 2)));
          Ani->DrawLine(vec3(Flat.SegmentPoints[SelectedPoints[0]].Pos.X, 0, Flat.SegmentPoints[SelectedPoints[0]].Pos.Y), vec3(Flat.SegmentPoints[ActivePoint].Pos.X, 0, Flat.SegmentPoints[ActivePoint].Pos.Y), vec4(1, 0, 0, 1));
          for (INT i = 0; i <= !(Flat.SegmentPoints[ActivePoint].Pos - Flat.SegmentPoints[SelectedPoints[0]].Pos); i++)
          {
            vec2 Pos = vec2(Flat.SegmentPoints[SelectedPoints[0]].Pos + (Flat.SegmentPoints[ActivePoint].Pos - Flat.SegmentPoints[SelectedPoints[0]].Pos).Normalize() * i);
            vec3 Perp = (vec3((Flat.SegmentPoints[ActivePoint].Pos - Flat.SegmentPoints[SelectedPoints[0]].Pos).Normalize().X, 0, (Flat.SegmentPoints[ActivePoint].Pos - Flat.SegmentPoints[SelectedPoints[0]].Pos).Normalize().Y) % vec3(0, 1, 0)).Normalize() * 0.30;
            Ani->DrawLine(vec3(Pos.X - Perp.X, 0, Pos.Y - Perp.Z), vec3(Pos.X + Perp.X, 0, Pos.Y + Perp.Z), vec4(1, 0, 0, 1));
            if (i > 3000)
              break;
          }
        }

        // Editor part
        if (!Ani->Keys[VK_LCONTROL] && !Ani->Keys[VK_SHIFT] && !Ani->Keys[VK_SPACE] && !IsBigSelection)
        {
          if (WasShift && WasMove)
            SelectedPoints.clear(), SelectedSegments.clear();

          // Add new point or select it or select segment
          if (Ani->KeysClick[VK_LBUTTON] && (!Ani->Keys[VK_LCONTROL]))
          {
            // Handle points
            if (SelectTypeBool)
            {
              // Add point and select it
              if (ActivePoint == -1 && ActiveSegment == -1)
              {
                action Last;
                Last.PointPos = vec2(MousePoint.X, MousePoint.Y);
                INT Point = Flat.AddPoint(MousePoint.X, MousePoint.Y);
                Last.Action = action_type::eADDPOINT;
                Last.Point = Point;
                if (SelectedPoints.size() > 0)
                {
                  Last.Selected = SelectedPoints[SelectedPoints.size() - 1];
                  Last.SelectedPos = Flat.SegmentPoints[Last.Selected].Pos;
                  Flat.AddSegment(SelectedPoints[SelectedPoints.size() - 1], Point);
                  SelectedPoints.clear();
                }
                else
                  Last.Selected = -1;
                SelectedPoints.push_back(Point);
                LastActions.push_back(Last);
              }
              // Select active point
              else if (ActivePoint != -1)
              {
                action Last;
                Last.Action = action_type::eADDSEGMENT;
                if (SelectedPoints.size() > 0 && SelectedPoints[SelectedPoints.size() - 1] != ActivePoint)
                {
                  Last.Selected = SelectedPoints[SelectedPoints.size() - 1];
                  Last.SelectedPos = Flat.SegmentPoints[Last.Selected].Pos;
                  INT SaveSize = (INT)Flat.Segments.size();
                  Last.Segment = Flat.AddSegment(SelectedPoints[SelectedPoints.size() - 1], ActivePoint);
                  SelectedPoints.clear();
                  if (SaveSize == Flat.Segments.size() || Last.Segment == -1)
                    SelectedPoints.push_back(ActivePoint);
                  else
                    LastActions.push_back(Last);
                }
                else
                {
                  SelectedPoints.clear();
                  SelectedPoints.push_back(ActivePoint);
                }
              }
              // Add clamped point to segment
              else if (ActiveSegment != -1)
              {
                action Last;
                vec2
                  P1 = Flat.SegmentPoints[Flat.Segments[ActiveSegment].Point1].Pos,
                  P2 = Flat.SegmentPoints[Flat.Segments[ActiveSegment].Point2].Pos;
                vec2 ResultPoint = mth::vec2<FLT>::GetNearestPointOnSegment(MousePoint, P1, P2);
                if ((ResultPoint.X != P1.X || ResultPoint.Y != P1.Y) && (ResultPoint.X != P2.X || ResultPoint.Y != P2.Y))
                {
                  INT ClampedPoint = Flat.AddPoint(ResultPoint.X, ResultPoint.Y);
                  INT
                    PointInd1 = Flat.Segments[ActiveSegment].Point1,
                    PointInd2 = Flat.Segments[ActiveSegment].Point2;
                  segment_type SaveType = Flat.Segments[ActiveSegment].Type;
                  vec2 SaveDoor = vec2(Flat.Segments[ActiveSegment].DoorWidth, Flat.Segments[ActiveSegment].DoorHeight);
                  vec4 SaveWindow = vec4(Flat.Segments[ActiveSegment].WindowLeftDist, Flat.Segments[ActiveSegment].WindowRightDist, Flat.Segments[ActiveSegment].WindowTopDist, Flat.Segments[ActiveSegment].WindowBottomDist);
                  INT NewSegment1 = Flat.AddSegment(PointInd1, ClampedPoint);
                  INT NewSegment2 = Flat.AddSegment(PointInd2, ClampedPoint);
                  if (NewSegment1 == NewSegment2)
                    NewSegment2 = Flat.AddSegment(PointInd1, ClampedPoint);
                  if ((Flat.SegmentPoints[Flat.Segments[NewSegment1].Point1].Pos - Flat.SegmentPoints[Flat.Segments[NewSegment1].Point2].Pos).Length2() >= (Flat.SegmentPoints[Flat.Segments[NewSegment2].Point1].Pos - Flat.SegmentPoints[Flat.Segments[NewSegment2].Point2].Pos).Length2())
                  {
                    Flat.Segments[NewSegment1].DoorWidth = SaveDoor.X;
                    Flat.Segments[NewSegment1].DoorHeight = SaveDoor.Y;
                    Flat.Segments[NewSegment1].WindowLeftDist = SaveWindow.X;
                    Flat.Segments[NewSegment1].WindowRightDist = SaveWindow.Y;
                    Flat.Segments[NewSegment1].WindowTopDist = SaveWindow.Z;
                    Flat.Segments[NewSegment1].WindowBottomDist = SaveWindow.W;
                    Flat.ChangeSegmentType(NewSegment1, SaveType);
                  }
                  else
                  {
                    Flat.Segments[NewSegment2].DoorWidth = SaveDoor.X;
                    Flat.Segments[NewSegment2].DoorHeight = SaveDoor.Y;
                    Flat.Segments[NewSegment2].WindowLeftDist = SaveWindow.X;
                    Flat.Segments[NewSegment2].WindowRightDist = SaveWindow.Y;
                    Flat.Segments[NewSegment2].WindowTopDist = SaveWindow.Z;
                    Flat.Segments[NewSegment2].WindowBottomDist = SaveWindow.W;
                    Flat.ChangeSegmentType(NewSegment2, SaveType);
                  }
                  ActiveSegment = -1;
                  if (SelectedPoints.size() > 0)
                  {
                    Last.Selected = SelectedPoints[SelectedPoints.size() - 1];
                    Last.SelectedPos = Flat.SegmentPoints[Last.Selected].Pos;
                    Flat.AddSegment(SelectedPoints[SelectedPoints.size() - 1], ClampedPoint);
                    SelectedPoints.clear();
                  }
                  else
                    Last.Selected = -1;
                  SelectedPoints.push_back(ClampedPoint);
                  Last.Action = action_type::eADDPOINT;
                  Last.PointPos = Flat.SegmentPoints[ClampedPoint].Pos;
                  Last.Point = ClampedPoint;
                  Last.SegmentSavePoint1 = PointInd1;
                  Last.SegmentSavePoint2 = PointInd2;
                  Last.SegmentType.clear();
                  Last.WinParams.clear();
                  Last.DoorParams.clear();
                  Last.SegmentType.push_back(SaveType);
                  Last.WinParams.push_back(SaveWindow);
                  Last.DoorParams.push_back(SaveDoor);
                  LastActions.push_back(Last);
                }
              }
            }
            // Handle segments
            else
            { 
              if (ActiveSegment != -1)
              {
                SelectedSegments.clear();
                SelectedSegments.push_back(ActiveSegment);
                CurrentSegmentType = Flat.Segments[ActiveSegment].Type;
                DoorWidth = Flat.Segments[ActiveSegment].DoorWidth;
                DoorHeight = Flat.Segments[ActiveSegment].DoorHeight;
                WindowLeftOffset = Flat.Segments[ActiveSegment].WindowLeftDist;
                WindowRightOffset = Flat.Segments[ActiveSegment].WindowRightDist;
                WindowBottomOffset = Flat.Segments[ActiveSegment].WindowBottomDist;
                WindowTopOffset = Flat.Segments[ActiveSegment].WindowTopDist;
                // Handle segment type selection
                if (CurrentSegmentType == segment_type::eWALL)
                  SegmentType = 0;
                else if (CurrentSegmentType == segment_type::eWIN)
                  SegmentType = 1;
                else
                  SegmentType = 2;
              }
            }
          }
          // Delete selected objects
          else if (DeleteButton || ((Ani->KeysClick['Y'] || Ani->KeysClick[VK_BACK]) && (!Ani->Keys[VK_LCONTROL])))
          {
            if (!SelectTypeBool && SelectedSegments.size() > 0)
            {
              action Last;
              Last.Action = action_type::eDELETESEGMENT;
              INT SegmNum = SelectedSegments[SelectedSegments.size() - 1];
              Last.SegmentType.clear();
              Last.WinParams.clear();
              Last.DoorParams.clear();
              Last.SegmentType.push_back(Flat.Segments[SegmNum].Type);
              Last.DoorParams.push_back(vec2(Flat.Segments[SegmNum].DoorWidth, Flat.Segments[SegmNum].DoorHeight));
              Last.WinParams.push_back(vec4(Flat.Segments[SegmNum].WindowLeftDist, Flat.Segments[SegmNum].WindowRightDist, Flat.Segments[SegmNum].WindowTopDist, Flat.Segments[SegmNum].WindowBottomDist));
              Last.SegmentSavePoint1 = Flat.Segments[SegmNum].Point1;
              Last.PointPointsPos.push_back(Flat.SegmentPoints[Flat.Segments[SegmNum].Point1].Pos);
              Last.SegmentSavePoint2 = Flat.Segments[SegmNum].Point2;
              Last.PointPointsPos.push_back(Flat.SegmentPoints[Flat.Segments[SegmNum].Point2].Pos);
              Flat.ChangeSegmentType(SegmNum, segment_type::eWALL);
              for (auto &SelectedSegment : SelectedSegments)
                Flat.DeleteSegment(SelectedSegment);
              SelectedSegments.clear();
              LastActions.push_back(Last);
            }
            else if (SelectedPoints.size() > 0)
            {
              action Last;
              Last.Action = action_type::eDELETEPOINT;
              INT SavePoint = -1;
              if (LastActions.size() > 0)
                SavePoint = LastActions[LastActions.size() - 1].Selected;
              Last.Point = SelectedPoints[SelectedPoints.size() - 1];
              for (auto &Name : Flat.SegmentPoints[Last.Point].RoomNames)
                Last.PointRoomsPoints[Name] = Flat.Rooms[Name].RoomPoints;
              Last.PointPos = Flat.SegmentPoints[Last.Point].Pos;
              INT i = 0;
              for (auto &SegmentS : Flat.SegmentPoints[Last.Point].SegmentIndices)
              {
                INT Segment = Flat.SegmentPoints[Last.Point].SegmentIndices[i];
                if (Flat.Segments[Segment].Point1 == Last.Point)
                  Last.PointPoints.push_back(Flat.Segments[Segment].Point2), Last.PointPointsPos.push_back(Flat.SegmentPoints[Flat.Segments[Segment].Point2].Pos);
                else if (Flat.Segments[Segment].Point2 == Last.Point)
                  Last.PointPoints.push_back(Flat.Segments[Segment].Point1), Last.PointPointsPos.push_back(Flat.SegmentPoints[Flat.Segments[Segment].Point1].Pos);
                Last.SegmentType.push_back(Flat.Segments[Segment].Type);
                Last.DoorParams.push_back(vec2(Flat.Segments[Segment].DoorWidth, Flat.Segments[Segment].DoorHeight));
                Last.WinParams.push_back(vec4(Flat.Segments[Segment].WindowLeftDist, Flat.Segments[Segment].WindowRightDist, Flat.Segments[Segment].WindowTopDist, Flat.Segments[Segment].WindowBottomDist));
                i++;
              }
              for (auto &SelectedPoint : SelectedPoints)
                Flat.DeletePoint(SelectedPoint);
              SelectedPoints.clear();
              SelectedPoints.push_back(SavePoint);
              LastActions.push_back(Last);
            }
          }
          OldPos = vec2(INFINITY);
          WasMove = FALSE;
          WasShift = FALSE;
        }
        else if (!Ani->Keys[VK_LCONTROL] && !Ani->Keys[VK_SPACE] && !IsBigSelection && !(Ani->Keys['W'] || Ani->Keys['S'] || Ani->Keys['A'] || Ani->Keys['D'] || Ani->Keys['K'] || Ani->Keys['L']))
        {
          if (!WasShift && !WasMove)
            SelectedPoints.clear(), SelectedSegments.clear();
          // Move point or segment
          if (Ani->Keys[VK_LBUTTON] && (!Ani->Keys[VK_LCONTROL]))
          {
            if (!WasMove && ((SelectTypeBool && ActivePoint != -1) || ActiveSegment != -1))
            {
              action Last;
              BOOL IsPrev = FALSE;
              if (OldPos.X == INFINITY && OldPos.Y == INFINITY)
              {
                OldPos = MousePoint;
                if (SelectTypeBool && ActivePoint != -1)
                  Last.MoveStartPos = Flat.SegmentPoints[ActivePoint].Pos;
                else if (!SelectTypeBool && ActiveSegment != -1)
                  Last.MoveStartPos = Flat.SegmentPoints[Flat.Segments[ActiveSegment].Point1].Pos;
              }
              if (Ani->Keys[VK_MENU])
                MousePoint = vec2(std::roundf(MousePoint.X), std::roundf(MousePoint.Y));
              vec2 DPos = MousePoint - OldPos;
              if (SelectTypeBool)
              {
                if (LastActions.size() > 0 && LastActions[LastActions.size() - 1].Action == action_type::eMOVEPOINT && LastActions[LastActions.size() - 1].Point != ActivePoint && !(!WasShift && WasMove) && WasLButton)
                  ActivePoint = LastActions[LastActions.size() - 1].Point, IsPrev = TRUE, Last.MoveStartPos = LastActions[LastActions.size() - 1].MoveStartPos;
                else if (LastActions.size() > 0 && LastActions[LastActions.size() - 1].Action == action_type::eMOVEPOINT && LastActions[LastActions.size() - 1].Point == ActivePoint)
                  IsPrev = TRUE, Last.MoveStartPos = LastActions[LastActions.size() - 1].MoveStartPos;
                Last.Action = action_type::eMOVEPOINT;
                Last.Point = ActivePoint;
                if (Ani->Keys[VK_MENU])
                  Flat.UpdateSelectedPoints({ActivePoint}, DPos.X, DPos.Y, TRUE);
                else
                  Flat.UpdateSelectedPoints({ActivePoint}, DPos.X, DPos.Y);
                Last.SaveDPos = Flat.SegmentPoints[ActivePoint].Pos - Last.MoveStartPos;
                if (IsPrev)
                  LastActions[LastActions.size() - 1] = Last;
                else
                  LastActions.push_back(Last);
                SelectedPoints.clear();
                SelectedSegments.clear();
                SelectedPoints.push_back(ActivePoint);
                WasMove = TRUE;
              }
              else if (!SelectTypeBool && ActiveSegment != -1)
              {
                if (LastActions.size() > 0 && LastActions[LastActions.size() - 1].Action == action_type::eMOVESEGMENT && LastActions.size() > 0 && LastActions[LastActions.size() - 1].Segment != ActiveSegment && !(!WasShift && WasMove) && WasLButton)
                  ActiveSegment = LastActions[LastActions.size() - 1].Segment, IsPrev = TRUE, Last.MoveStartPos = LastActions[LastActions.size() - 1].MoveStartPos;
                else if (LastActions.size() > 0 && LastActions[LastActions.size() - 1].Action == action_type::eMOVESEGMENT && LastActions.size() > 0 && LastActions[LastActions.size() - 1].Segment == ActiveSegment)
                  IsPrev = TRUE, Last.MoveStartPos = LastActions[LastActions.size() - 1].MoveStartPos;
                INT SegmNum = ActiveSegment;
                Last.SegmentType.clear();
                Last.WinParams.clear();
                Last.DoorParams.clear();
                Last.SegmentType.push_back(Flat.Segments[SegmNum].Type);
                Last.DoorParams.push_back(vec2(Flat.Segments[SegmNum].DoorWidth, Flat.Segments[SegmNum].DoorHeight));
                Last.WinParams.push_back(vec4(Flat.Segments[SegmNum].WindowLeftDist, Flat.Segments[SegmNum].WindowRightDist, Flat.Segments[SegmNum].WindowTopDist, Flat.Segments[SegmNum].WindowBottomDist));
                Last.Action = action_type::eMOVESEGMENT;
                if (Ani->Keys[VK_MENU])
                  Flat.UpdateSelectedSegments({ActiveSegment}, DPos.X, DPos.Y, TRUE);
                else
                  Flat.UpdateSelectedSegments({ActiveSegment}, DPos.X, DPos.Y);
                Flat.Segments[ActiveSegment].DoorWidth = Last.DoorParams[0].X;
                Flat.Segments[ActiveSegment].DoorHeight = Last.DoorParams[0].Y;
                Flat.Segments[ActiveSegment].WindowLeftDist = Last.WinParams[0].X;
                Flat.Segments[ActiveSegment].WindowRightDist = Last.WinParams[0].Y;
                Flat.Segments[ActiveSegment].WindowTopDist = Last.WinParams[0].Z;
                Flat.Segments[ActiveSegment].WindowBottomDist = Last.WinParams[0].W;
                Flat.ChangeSegmentType(ActiveSegment, Last.SegmentType[0]);
                Last.SaveDPos = Flat.SegmentPoints[Flat.Segments[ActiveSegment].Point1].Pos - Last.MoveStartPos;
                CurrentSegmentType = Flat.Segments[ActiveSegment].Type;
                // Handle segment type selection
                if (CurrentSegmentType == segment_type::eWALL)
                  SegmentType = 0;
                else if (CurrentSegmentType == segment_type::eWIN)
                  SegmentType = 1;
                else
                  SegmentType = 2;
                Last.Segment = ActiveSegment;
                if (IsPrev)
                  LastActions[LastActions.size() - 1] = Last;
                else
                  LastActions.push_back(Last);
                SelectedPoints.clear();
                SelectedSegments.clear();
                SelectedSegments.push_back(ActiveSegment);
                WasMove = TRUE;
              }
              else
                WasMove = FALSE;
              if (!DPos > Threshold)
                WasMove = FALSE;
              OldPos = MousePoint;
            }
            else
            {
              action Last;
              if (OldPos.X == INFINITY && OldPos.Y == INFINITY)
              {
                OldPos = MousePoint;
                if (SelectTypeBool && ActivePoint != -1)
                  Last.MoveStartPos = Flat.SegmentPoints[ActivePoint].Pos;
                else if (!SelectTypeBool && ActiveSegment != -1)
                  Last.MoveStartPos = Flat.SegmentPoints[Flat.Segments[ActiveSegment].Point1].Pos;
              }
              if (Ani->Keys[VK_MENU])
                MousePoint = vec2(std::roundf(MousePoint.X), std::roundf(MousePoint.Y));
              vec2 DPos = MousePoint - OldPos;
              if (SelectTypeBool && SelectedPoints.size() > 0)
              {
                Last.Action = action_type::eMOVEPOINT;
                Last.Point = SelectedPoints[SelectedPoints.size() - 1];
                if (Ani->Keys[VK_MENU])
                  Flat.UpdateSelectedPoints(SelectedPoints, DPos.X, DPos.Y, TRUE);
                else
                  Flat.UpdateSelectedPoints(SelectedPoints, DPos.X, DPos.Y);
                Last.SaveDPos = Flat.SegmentPoints[SelectedPoints[SelectedPoints.size() - 1]].Pos - Last.MoveStartPos;
                SelectedSegments.clear();
                WasMove = TRUE;
              }
              else if (!SelectTypeBool && SelectedSegments.size() > 0)
              {
                Last.Action = action_type::eMOVESEGMENT;
                Last.Segment = SelectedSegments[SelectedSegments.size() - 1];
                if (Ani->Keys[VK_MENU])
                  Flat.UpdateSelectedSegments(SelectedSegments, DPos.X, DPos.Y, TRUE);
                else
                  Flat.UpdateSelectedSegments(SelectedSegments, DPos.X, DPos.Y);
                Last.SaveDPos = Flat.SegmentPoints[Flat.Segments[SelectedSegments[SelectedSegments.size() - 1]].Point1].Pos - Last.MoveStartPos;
                CurrentSegmentType = Flat.Segments[Last.Segment].Type;
                // Handle segment type selection
                if (CurrentSegmentType == segment_type::eWALL)
                  SegmentType = 0;
                else if (CurrentSegmentType == segment_type::eWIN)
                  SegmentType = 1;
                else
                  SegmentType = 2;
                SelectedPoints.clear();
                WasMove = TRUE;
              }
              else
                WasMove = FALSE;
              if (!DPos > Threshold)
                WasMove = FALSE;
              OldPos = MousePoint;
            }
          }
          if ((Ani->Keys[VK_LCONTROL] || !Ani->Keys[VK_LBUTTON]) && (OldPos.X != INFINITY || OldPos.Y != INFINITY))
            OldPos = vec2(INFINITY), WasMove = FALSE;
          WasShift = TRUE;
        }

        if (!Ani->Keys[VK_LBUTTON] && (WasShift || WasMove))
          WasMove = FALSE, SelectedPoints.clear(), SelectedSegments.clear();
        if (!Ani->Keys[VK_SHIFT] && (WasShift || WasMove))
          WasShift = FALSE, SelectedPoints.clear(), SelectedSegments.clear();

        if (Ani->Keys[VK_LBUTTON])
          WasLButton = TRUE;
        else
          WasLButton = FALSE;

        // Big selection part
        if (!Ani->Keys[VK_SHIFT] && !Ani->Keys[VK_LCONTROL] && Ani->Keys[VK_SPACE] && Ani->KeysClick[VK_LBUTTON] && !IsSelectionStarted)
        {
          IsSelectionStarted = TRUE;
          IsSelection = TRUE;
          SelectionStartPointM = vec2(Ani->Mx, Ani->My);

          mth::ray<FLT> RStart = Ani->Cam.FrameRay(SelectionStartPointM.X, SelectionStartPointM.Y);
          DBL ts = RStart.IntersectPlane();
          SelectionStartPoint = vec2(RStart(ts).X, RStart(ts).Z);
          
          BigSelection.clear();
        }
        else if (!Ani->Keys[VK_SHIFT] && !Ani->Keys[VK_LCONTROL] && Ani->Keys[VK_SPACE] && Ani->KeysClick[VK_LBUTTON])
        {
          SelectionEndPointM = vec2(Ani->Mx, Ani->My);

          mth::ray<FLT> REnd = Ani->Cam.FrameRay(SelectionEndPointM.X, SelectionEndPointM.Y);
          DBL ts = REnd.IntersectPlane();
          SelectionEndPoint = vec2(REnd(ts).X, REnd(ts).Z);

          mth::ray<FLT> RSub1 = Ani->Cam.FrameRay(SelectionStartPointM.X, SelectionEndPointM.Y);
          ts = RSub1.IntersectPlane();
          SelectionSub1Point = vec2(RSub1(ts).X, RSub1(ts).Z);
          mth::ray<FLT> RSub2 = Ani->Cam.FrameRay(SelectionEndPointM.X, SelectionStartPointM.Y);
          ts = RSub2.IntersectPlane();
          SelectionSub2Point = vec2(RSub2(ts).X, RSub2(ts).Z);

          IsSelection = TRUE;
          IsBigSelection = TRUE;
          SelectedPoints.clear();
          SelectedSegments.clear();
        }

        if (Ani->Keys[VK_SHIFT] || Ani->Keys[VK_LCONTROL] || !Ani->Keys[VK_SPACE])
          IsSelection = FALSE;

        if (!IsSelection && IsSelectionStarted)
        {
          IsSelectionStarted = FALSE;
          IsSelection = FALSE;
          for (auto &[Num, P] : Flat.SegmentPoints)
          {
            if (IsPointInsideTriangle(SelectionStartPoint, SelectionSub1Point, SelectionEndPoint, P.Pos) || IsPointInsideTriangle(SelectionStartPoint, SelectionSub2Point, SelectionEndPoint, P.Pos))
              BigSelection.push_back(Num);
          }
          SelectionEndPoint = vec2(INFINITY);
        }

        if (IsBigSelection)
        {
          if ((Ani->KeysClick['E'] || Ani->KeysClick[VK_ESCAPE]) && !Ani->Keys[VK_LCONTROL])
          {
            BigSelection.clear();
            IsBigSelection = FALSE;
          }
          else if (Ani->KeysClick['Y'] && !Ani->Keys[VK_LCONTROL])
          {
            action Last;
            Last.Action = action_type::eDELETEBIGSELECTION;
            Last.SaveSegments = Flat.Segments;
            Last.SaveSegmentPoints = Flat.SegmentPoints;
            Last.SaveSegmentWalls = Flat.SegmentWalls;
            for (auto &N : BigSelection)
              Flat.DeletePoint(N);
            BigSelection.clear();
            IsBigSelection = FALSE;
            LastActions.push_back(Last);
          }
        }
      } /* End of 'Response' function */

      /* Render unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Render( anim *Ani ) override
      {
        // Debug draw
        vec4 ActiveColor = vec4(1, 0.84, 0.22, 1);
        vec4 SelectedColor = vec4(0.81, 1, 0.47, 1);
        if (DrawPoints && !DrawLayout)
        {
          for (auto &[Key, Pnt] : Flat.SegmentPoints)
          {
            vec4 PointColor = vec4(0.65, 0.65, 0.65, 1); // Unselected point color
            if (Key == ActivePoint && std::find(SelectedPoints.begin(), SelectedPoints.end(), Key) != SelectedPoints.end())
              PointColor = PointColor * 0.8 + ActiveColor * 0.30 + SelectedColor * 0.30;
            else if (Key == ActivePoint)
              PointColor = PointColor * 0.8 + ActiveColor * 0.47;
            else if (std::find(SelectedPoints.begin(), SelectedPoints.end(), Key) != SelectedPoints.end())
              PointColor = PointColor * 0.8 + SelectedColor * 0.47;
            else if (std::find(BigSelection.begin(), BigSelection.end(), Key) != BigSelection.end())
              PointColor = PointColor * 0.47 + vec4(1, 0.74, 0 , 1) * 0.8;
            Ani->DrawSphere(vec3(Pnt.Pos.X,
                                 0,
                                 Pnt.Pos.Y), DebugLayoutRadius, PointColor);
          }
          if (ActivePoint == -1 && ActiveSegment == -1)
            Ani->DrawSphere(vec3(MousePoint.X,
                                 0,
                                 MousePoint.Y), DebugLayoutRadius, vec4(1));
        }
        else if (DrawLayout)
        {
          for (auto &[Key, Pnt] : Flat.SegmentPoints)
          {
            vec4 PointColor = vec4(0.65, 0.65, 0.65, 1); // Unselected point color
            if (Key == ActivePoint && std::find(SelectedPoints.begin(), SelectedPoints.end(), Key) != SelectedPoints.end())
              PointColor = PointColor * 0.8 + ActiveColor * 0.30 + SelectedColor * 0.30;
            else if (Key == ActivePoint)
              PointColor = PointColor * 0.8 + ActiveColor * 0.47;
            else if (std::find(SelectedPoints.begin(), SelectedPoints.end(), Key) != SelectedPoints.end())
              PointColor = PointColor * 0.8 + SelectedColor * 0.47;
            else if (std::find(BigSelection.begin(), BigSelection.end(), Key) != BigSelection.end())
              PointColor = PointColor * 0.47 + vec4(1, 0.74, 0 , 1) * 0.8;
            Ani->DrawSphere(vec3(Pnt.Pos.X,
                                 0,
                                 Pnt.Pos.Y), DebugLayoutRadius, PointColor);
          }
          if (ActivePoint == -1 && ActiveSegment == -1)
            Ani->DrawSphere(vec3(MousePoint.X,
                                 0,
                                 MousePoint.Y), DebugLayoutRadius, vec4(1));
          for (auto &[Key, Sgm] : Flat.Segments)
          {
            vec4 SegmentColor = vec4(0.6, 0.6, 0.6, 1); // Unselected segment color
            if (Key == ActiveSegment && std::find(SelectedSegments.begin(), SelectedSegments.end(), Key) != SelectedSegments.end())
              SegmentColor = SegmentColor * 0.8 + ActiveColor * 0.30 + SelectedColor * 0.30;
            else if (Key == ActiveSegment)
              SegmentColor = SegmentColor * 0.8 + ActiveColor * 0.47;
            else if (std::find(SelectedSegments.begin(), SelectedSegments.end(), Key) != SelectedSegments.end())
              SegmentColor = SegmentColor * 0.8 + SelectedColor * 0.47;
            vec3
              P1 = vec3(Flat.SegmentPoints[Sgm.Point1].Pos.X, 0, Flat.SegmentPoints[Sgm.Point1].Pos.Y),
              P2 = vec3(Flat.SegmentPoints[Sgm.Point2].Pos.X, 0, Flat.SegmentPoints[Sgm.Point2].Pos.Y);
            Ani->DrawCylinder(P1, DebugLayoutRadius * 0.9, P2, DebugLayoutRadius * 0.9, SegmentColor);
          }
        }

        if (UsingScheme)
        {
          Flat.SchemeTranslate = vec3(SchemeXOffset, 0, SchemeYOffset);
          Flat.SchemeRotate = INT(SchemeRotation / 45) * 45;
          Flat.SchemeScale = SchemeScale;
        }
        SystemProfiler.Collect("Walls Draw");
      } /* End of 'Render' function */
    };  /* End of 'walls_editor' class */
    static units::editor::unit_register<units::walls_editor> _("Walls editor unit");
  } /* end of 'units' namespace */
} /* end of 'tam' namespace */

/* END OF 'tam_walls_editor.cpp' FILE */
