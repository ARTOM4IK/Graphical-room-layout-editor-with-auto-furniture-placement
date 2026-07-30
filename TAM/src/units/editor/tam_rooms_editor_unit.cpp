/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_rooms_editor.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Rooms editor unit implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
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
    /* Room editor unit representation class */
    class rooms_editor : public editor_unit
    {
    private:
      // Hotkey tip texts
      const std::string Hotkeys1 =
        "Rooms editor:\n"
        "LMB  =>  select room\n"
        "SHIFT + LMB  =>  select points\n"
        "N  =>  add room of selected points\n"
        "ESC  =>  clear selection\n"
        "RMB => unselect room\n"
        "SHIFT + RMB => unselect point\n"
        "Y  =>  delete room\n";

      // Rooms editor tab
      ux::tab &Tab;
      ux::user_type DrawSettings {Tab};                                                // User type drawing settings
      ux::separator_type EditInfoSeparator {Tab, "Editor info"};                       // Edit mode info separator
      ux::text_type Hotkeys {Tab, Hotkeys1};                                           // Text with hotkeys
      ux::flt_type PointFindOffset {Tab, "Points find offset", 0.8f, 0.05f, 10.0f};    // Points find offset

      /* Rooms management */
      ux::separator_type RoomsSeparator {Tab, "Rooms management"};                     // Rooms management separator
      ux::string_type RoomNameString {Tab, ""};                                        // Room name string type
      ux::sameline_type AddRoomSameLine {Tab};                                         // Add room button same line
      ux::button AddRoomButton {Tab, "Add new room"};                                  // Add new room button
      ux::button DeleteRoomButton {Tab, "Delete selected room"};                       // Delete selected room button
      ux::select_type RoomsList {Tab, "Selected room", {"Select room"}};               // List of selected rooms
      ux::text_type RoomArea {Tab, ""};                                                // Room area
      std::string PreviousSelectedRoom = "";                                           // Previously selected room name
      ux::text_type ModelId {Tab, ""};                                                 // Model id under mouse variable

      /* Image store data type */
      struct image_entry
      {
        texture Tex {};
        VkDescriptorSet DescSet {};
        std::string Name {};
      }; /* End of 'image_entry' structure */

      /* Wallpapers loading */
      ux::separator_type WallpapersSeparator {Tab, "Load wallpapers"};                 // Wallpapers loading separator
      ux::button LoadTexture {Tab, "Load texture"};                                    // Loading textures button
      std::vector<image_entry> DropdownImages {};                                      // Dropdown images
      BOOL ApplyWallpapers = FALSE;                                                    // Wallpapers application flag
      INT SelectedImageIdx = 0;                                                        // Index of selected image
      BOOL AreWallpapersLoaded = 0;                                                    // Wallpapers loading flag
      std::string SelectedWallpaper = "";                                              // Selected wallpaper (texture) name
      BOOL DeleteWallpaper = FALSE;                                                    // Wallpaper deletion flag
      FLT
        WallpaperScale = 1.0,                                                          // Wallpaper scaling coefficient
        PrevWallpaperScale = 1.0;                                                      // Previous wallpaper scaling coefficient

      /* Floor textures loading */
      BOOL ApplyFloor = FALSE;                                                         // Floor application flag
      INT SelectedImageIdxFloor = 0;                                                   // Index of selected image
      BOOL IsFloorLoaded = 0;                                                          // Wallpapers loading flag
      std::string SelectedFloor = "";                                                  // Selected floor (texture) name
      BOOL DeleteFloorTex = FALSE;                                                     // Floor texture deletion flag
      FLT
        FloorScale = 1.0,                                                              // Floor texture scaling coefficient
        PrevFloorScale = 1.0;                                                          // Previous loor texture scaling coefficient

      ux::user_type WallsColorpicker {Tab};                                            // User type for rooms' walls' colors
      ux::user_type FloorFunc {Tab};                                                   // User type for rooms' floor editing
      vec3
        CurrentColor {1, 1, 1},                                                        // Current selected color
        PrevColor {1, 1, 1};                                                           // Previous selected color

      /* Draw mode settings */
      ux::separator_type DrawSeparator {Tab, "Draw mode settings"};                    // Draw mode separator
      ux::flt_type DebugLayoutRadius {Tab, "Debug radius", 0.08f, 0.03f, 3.0f};        // Debug radius of spheres and cylinders
      ux::bool_type DrawMarkersBool {Tab, "Draw markers", TRUE, "No", "Yes"};          // Draw markers flag

      INT LastAccessFrame {};                                                          // Last frame when unit was active
    public:
      std::vector<INT> SelectedPoints {}; // Selected points array
      std::string SelectedRoom {};        // Selected room
      INT ActivePoint = -1;               // Active point index
      vec2 MousePoint {};                 // Position of cursor on plane

      /* Draw dropdown with images tab content function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DrawTabDropdown( VOID )
      {
        if (!AreWallpapersLoaded || Flat.Wallpapers.empty())
        {
          ImGui::TextWrapped("Images not loaded.");
          return;
        }

        const CHAR *preview = DropdownImages[SelectedImageIdx].Name.c_str();
        if (ImGui::BeginCombo("Wallpapers", preview))
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
        SelectedWallpaper = DropdownImages[SelectedImageIdx].Name;
      } /* End of 'DrawTabDropdown' function */

      /* Draw dropdown with images tab content function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DrawTabDropdownFloor( VOID )
      {
        if (!IsFloorLoaded || Flat.Wallpapers.empty())
        {
          ImGui::TextWrapped("Images not loaded.");
          return;
        }

        const CHAR *preview = DropdownImages[SelectedImageIdx].Name.c_str();
        if (ImGui::BeginCombo("Floor textures", preview))
        {
          for (INT i = 0; i < (INT)DropdownImages.size(); i++)
          {
            BOOL is_selected = (SelectedImageIdxFloor == i);
            if (DropdownImages[i].DescSet != VK_NULL_HANDLE)
            {
              ImGui::Image((ImTextureID)DropdownImages[i].DescSet, ImVec2(32, 32));
              ImGui::SameLine();
            }
            if (ImGui::Selectable(DropdownImages[i].Name.c_str(), is_selected))
              SelectedImageIdxFloor = i;
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
          ImGui::EndCombo();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Selected: %s", DropdownImages[SelectedImageIdxFloor].Name.c_str());

        if (DropdownImages[SelectedImageIdxFloor].DescSet != VK_NULL_HANDLE)
        {
          FLT avail = ImGui::GetContentRegionAvail().x;
          FLT img_size = avail < 256.0f ? avail : 256.0f;
          ImGui::Image((ImTextureID)DropdownImages[SelectedImageIdxFloor].DescSet,
            ImVec2(img_size, img_size));
        }
        SelectedFloor = DropdownImages[SelectedImageIdxFloor].Name;
      } /* End of 'DrawTabDropdownFloor' function */

      /* Try to load dropdown images function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       *   - texture name to be written in dropdown:
       *       std::string TexName;
       *   - texture:
       *       texture Tex;
       * RETURNS: None.
       */
      VOID TryLoadImage( anim *Ani, std::string TexName, texture Tex )
      {
        image_entry entry;
        entry.Name = TexName;
        entry.Tex = Ani->TextureCreate(Tex->Name);
        if (entry.Tex &&
            entry.Tex->Img.View != VK_NULL_HANDLE &&
            entry.Tex->Sampler.Sampler != VK_NULL_HANDLE)
          entry.DescSet = ImGui_ImplVulkan_AddTexture(entry.Tex->Sampler.Sampler,
            entry.Tex->Img.View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        else
          entry.DescSet = VK_NULL_HANDLE;
        DropdownImages.push_back(entry);
        AreWallpapersLoaded = TRUE;
        IsFloorLoaded = TRUE;
      } /* End of 'TryLoadImage' function */

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
      rooms_editor( anim *Ani, flat &WorkFlat, ux::tab &WorkTab, const std::function<VOID ( VOID )> &UserUIFunc ) :
        editor_unit(Ani, WorkFlat), Tab(WorkTab)
      {
        DrawSettings.SetUserFunc(UserUIFunc);
        WallsColorpicker.SetUserFunc([Ani, this](VOID)
        {
          ImGuiWindow *Window = ImGui::GetCurrentWindow();

          ImGui::AlignTextToFramePadding();
          if (RoomsList.GetItems().size() > 0 && SelectedRoom != "Out of room space" &&
              SelectedRoom != "Select room" && SelectedRoom != "")
          {
            static BOOL IsOpen = TRUE;
            IsOpen = ImGui::TreeNodeEx("Walls editing ##SHOWORNOT", IsOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);

            if (IsOpen)
            {
              ImGui::ColorEdit3("Current room color", CurrentColor);
              DrawTabDropdown();

              if (ImGui::Button("Apply selected wallpaper"))
                ApplyWallpapers = TRUE;
              ImGui::SliderFloat("Wallpaper scale", &WallpaperScale, 0.001f, 8.0f);
              if (ImGui::Button("Delete wallpaper"))
                DeleteWallpaper = TRUE;

              ImGui::TreePop();
            }
          }
        });
        FloorFunc.SetUserFunc([Ani, this](VOID)
        {
          ImGui::AlignTextToFramePadding();
          if (RoomsList.GetItems().size() > 0 && SelectedRoom != "Out of room space" &&
              SelectedRoom != "Select room" && SelectedRoom != "")
          {
            static BOOL IsOpen = TRUE;
            IsOpen = ImGui::TreeNodeEx("Floor editing ##SHOWORNOT", IsOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);

            if (IsOpen)
            {
              DrawTabDropdownFloor();

              if (ImGui::Button("Apply selected floor"))
                ApplyFloor = TRUE;
              ImGui::SliderFloat("Floor scale", &FloorScale, 0.001f, 8.0f);
              if (ImGui::Button("Delete floor texture"))
                DeleteFloorTex = TRUE;

              ImGui::TreePop();
            }
          }
        });
      } /* End of 'rooms_editor' function */

      /* Get response from unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
        // Check if unit was unactive to clear selected points / segments or 'ESC' button was pressed
        if (LastAccessFrame != Ani->CurrentFrameNo - 1 || Ani->KeysClick[VK_ESCAPE])
        {
          SelectedPoints.clear();
          SelectedRoom = "";
          ActivePoint = -1;
        }
        LastAccessFrame = Ani->CurrentFrameNo;

        if (LoadTexture)
        {
          std::string p = Ani->OpenFileDialog("Load texture", "Textures\0*.jpg;*.jpeg;*.png;*.bmp;*.g24;*.g32;*.jfif\0All\0*.*\0");
          if (!p.empty())
          {
            if (!Flat.Wallpapers.contains(p))
            {
              texture tex = Ani->TextureCreate(p);
              if (tex != nullptr)
              {
                Flat.Wallpapers.insert({std::filesystem::path(p).stem().string(), tex});
                TryLoadImage(Ani, std::filesystem::path(p).stem().string(), tex);
              }
            }
          }
        }

        if (PrevWallpaperScale != WallpaperScale || (SelectedRoom != "" && SelectedRoom != "Select room" && SelectedRoom != "Out of room space" &&
           (Flat.Rooms[SelectedRoom].WallsColor.X != CurrentColor.X || Flat.Rooms[SelectedRoom].WallsColor.Y != CurrentColor.Y ||
            Flat.Rooms[SelectedRoom].WallsColor.Z != CurrentColor.Z)))
        {
          PrevWallpaperScale = WallpaperScale;
          Flat.UpdateRoomMtlBuf(SelectedRoom, CurrentColor, WallpaperScale);
        }

        if (ApplyWallpapers && SelectedRoom != "Select room" && SelectedRoom != "Out of room space" &&
          SelectedWallpaper != "" && SelectedWallpaper != "Apply wallpapers")
        {
          vec2 WH = vec2(Flat.Wallpapers[SelectedWallpaper]->Img.W, Flat.Wallpapers[SelectedWallpaper]->Img.H);
          Flat.Rooms[SelectedRoom].WallpaperInitial = WH;
          FLT a;
          if (WH.X >= WH.Y)
          {
            a = WH.Y / WH.X;
            Flat.Rooms[SelectedRoom].WallpaperTransformed = vec2(WallpaperScale, WallpaperScale * a);
          }
          else
          {
            a = WH.X / WH.Y;
            Flat.Rooms[SelectedRoom].WallpaperTransformed = vec2(WallpaperScale * a, WallpaperScale);
          }

          Flat.UpdateRoomWalls(SelectedRoom, Flat.Rooms[SelectedRoom].WallpaperTransformed);
          Flat.UpdateRoomTexs(SelectedRoom, {SelectedWallpaper});
          Flat.UpdateRoomMtlBuf(SelectedRoom, CurrentColor, WallpaperScale);
          ApplyWallpapers = FALSE;
        }

        if (DeleteWallpaper && SelectedRoom != "Select room" && SelectedRoom != "Out of room space" &&
          SelectedWallpaper != "" && SelectedWallpaper != "Apply wallpapers")
        {
          Flat.UpdateRoomTexs(SelectedRoom, {});
          DeleteWallpaper = FALSE;
        }

        if (DeleteFloorTex && SelectedRoom != "Select room" && SelectedRoom != "Out of room space" &&
          SelectedWallpaper != "" && SelectedWallpaper != "Apply wallpapers")
        {
          BUF_MTL_PBR BufMtlPBR
          {
            .BaseColor = vec4(1),
            .MetallicRoughnessScale = vec3(0.18, 0.8, FloorScale),
            .TextureFlags = {0, 0, 0, 0, 0, 0, 0, 0},
          };
          Flat.Rooms[SelectedRoom].RoomFloor.FloorTex = "Default wallpaper";
          Flat.Rooms[SelectedRoom].RoomFloor.FloorMtl->Update({Flat.Wallpapers["Default wallpaper"]},
            Flat.Rooms[SelectedRoom].RoomFloor.FloorMtl->Buffers);
          Flat.Rooms[SelectedRoom].RoomFloor.FloorMtl->Buffers[0]->Update(&BufMtlPBR);
          Flat.Rooms[SelectedRoom].RoomFloor.FloorPrim->Mtl = Flat.Rooms[SelectedRoom].RoomFloor.FloorMtl;
          DeleteFloorTex = FALSE;
        }

        if (PrevFloorScale != FloorScale || (ApplyFloor && SelectedRoom != "Select room" && SelectedRoom != "Out of room space" &&
           SelectedFloor != "" && SelectedFloor != "Apply floor" && SelectedFloor != Flat.Rooms[SelectedRoom].RoomFloor.FloorTex))
        {
          PrevFloorScale = FloorScale;
          Flat.Rooms[SelectedRoom].RoomFloor.FloorScale = FloorScale;
          BUF_MTL_PBR BufMtlPBR
          {
            .BaseColor = vec4(1),
            .MetallicRoughnessScale = vec3(0.18, 0.8, FloorScale),
            .TextureFlags = {1, 0, 0, 0, 0, 0, 0, 0},
          };
          Flat.Rooms[SelectedRoom].RoomFloor.FloorTex = SelectedFloor;
          Flat.Rooms[SelectedRoom].RoomFloor.FloorMtl->Update({Flat.Wallpapers[SelectedFloor]},
            Flat.Rooms[SelectedRoom].RoomFloor.FloorMtl->Buffers);
          Flat.Rooms[SelectedRoom].RoomFloor.FloorMtl->Buffers[0]->Update(&BufMtlPBR);
          Flat.Rooms[SelectedRoom].RoomFloor.FloorPrim->Mtl = Flat.Rooms[SelectedRoom].RoomFloor.FloorMtl;
          ApplyFloor = FALSE;
        }

        // Get mouse position on the plane
        mth::ray<FLT> R = Ani->Cam.FrameRay(Ani->Mx, Ani->My);
        DBL t = R.IntersectPlane();
        MousePoint = vec2(R(t).X, R(t).Z);

        /*** Handle points interaction ***/
        ActivePoint = Flat.FindNearestPoint(MousePoint.X, MousePoint.Y, PointFindOffset);
        // Select points
        if (Ani->Keys[VK_SHIFT] && Ani->KeysClick[VK_LBUTTON] && (!Ani->Keys[VK_LCONTROL]))
        {
          // Add point and select it
          if (ActivePoint != -1)
          {
            //if (std::find(SelectedPoints.begin(), SelectedPoints.end(), ActivePoint) == SelectedPoints.end())
            SelectedPoints.push_back(ActivePoint);
          }
        }
        // Unselect point
        else if (Ani->Keys[VK_SHIFT] && Ani->KeysClick[VK_RBUTTON] && (!Ani->Keys[VK_LCONTROL]))
        {
          // Delete point from selected
          if (ActivePoint != -1 && SelectedPoints.size() > 0)
          {
            for (INT i = static_cast<INT>(SelectedPoints.size()) - 1; i >= 0; i--)
              if (SelectedPoints[i] == ActivePoint)
                SelectedPoints.erase(SelectedPoints.begin() + i);
          }
            /*if (auto DelPoint = std::find(SelectedPoints.begin(), SelectedPoints.end(), ActivePoint); DelPoint != SelectedPoints.end())
              SelectedPoints.erase(DelPoint);*/
        }

        if (SelectedRoom != "" && SelectedRoom != "Out of room space" && SelectedRoom != "Select room")
          RoomArea = std::format("Room area: {:.1f}", Flat.Rooms[SelectedRoom].Area);

        /*** Rooms management part ***/
        BOOL IsSelectedRoom = FALSE;
        if ((!Ani->Keys[VK_SHIFT]) && Ani->KeysClick[VK_LBUTTON] && (!Ani->Keys[VK_LCONTROL]))
        {
          SelectedRoom = Flat.GetRoomByPoint(MousePoint);
          IsSelectedRoom = TRUE;
          if (SelectedRoom != "" && SelectedRoom != "Out of room space")
            CurrentColor = Flat.Rooms[SelectedRoom].WallsColor;
          SelectedPoints.clear();
        }
        else if ((!Ani->Keys[VK_SHIFT]) && Ani->KeysClick[VK_RBUTTON] && (!Ani->Keys[VK_LCONTROL]))
        {
          SelectedRoom = "";
          SelectedPoints.clear();
        }

        // Check if room was selected room by selector
        if (RoomsList.GetSelected() != PreviousSelectedRoom && !IsSelectedRoom)
          SelectedRoom = RoomsList.GetSelected();
        PreviousSelectedRoom = RoomsList.GetSelected();

        // Handle adding new room
        if (AddRoomButton && RoomNameString != "" && SelectedPoints.size() >= 3)
        {
          auto Rooms = RoomsList.GetItems();
          if (std::find(Rooms.begin(), Rooms.end(), RoomNameString.operator std::string()) == Rooms.end())
          {
            if (Flat.AddRoom(RoomNameString, SelectedPoints))
              SelectedPoints.clear();
          }
        }
        // Handle deletion of room
        if (DeleteRoomButton && RoomsList.GetSelected() != "Select room")
        {
          Flat.DeleteRoom(RoomsList.GetSelected());
          if (SelectedRoom == RoomsList.GetSelected())
            SelectedRoom = "";
        }
        if (Flat.IsRoomsUpdate || SelectedRoom != RoomsList.GetSelected())
        {
          // Syncronize UX with current rooms list
          std::string CurrentSelected = SelectedRoom;
          RoomsList.Clear();
          RoomsList.Add("Out of room space");
          for (auto &[Name, Room] : Flat.Rooms)
            RoomsList.Add(Name);
          for (INT i = 0; i < RoomsList.GetCount(); i++)
            if (RoomsList.GetItems()[i] == CurrentSelected)
            {
              RoomsList = i;
              break;
            }
          Flat.IsRoomsUpdate = FALSE;
        }
        // Example of selected object handling
        if (Ani->My >= 0 && Ani->My < Ani->FrameH && Ani->Mx >= 0 && Ani->Mx < Ani->FrameW)
        {
          INT *Id = new INT;
          Ani->OpaqueIdBuf->VlkBuffer.Read(Id, sizeof(INT), (Ani->Mx + Ani->FrameW * Ani->My) * sizeof(INT));
          if (Ani->MButClick[0])
            Ani->CurrentObjectId = *Id;
          ModelId.SetText(std::format("X: {}, Y: {}, Id: {}", Ani->Mx, Ani->My, *Id));
          delete Id;
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
        if (Flat.Rooms.contains(SelectedRoom))
        {
          Ani->Draw(Flat.Rooms[SelectedRoom].RoomFloor.FloorPrim);
          if (Flat.DrawCeiling)
            Ani->Draw(Flat.Rooms[SelectedRoom].RoomCeiling.FloorPrim);
        }

        // Debug draw
        if (DrawMarkersBool)
        {
          for (auto &[Key, Pnt] : Flat.SegmentPoints)
          {
            vec4 PointColor = vec4(0.74, 0.74, 0.74, 1); // Unselected point color
            if (Key == ActivePoint)
              PointColor = vec4(0.81, 0.6, 0.18, 1);  // Active point color
            else if (std::find(SelectedPoints.begin(), SelectedPoints.end(), Key) != SelectedPoints.end())
              PointColor = vec4(0.81, 0.98, 0.47, 1);  // Selected point color
            Ani->DrawSphere(vec3(Pnt.Pos.X,
                                 0,
                                 Pnt.Pos.Y), DebugLayoutRadius, PointColor);
          }
          if (ActivePoint == -1)
            Ani->DrawSphere(vec3(MousePoint.X,
                                 0,
                                 MousePoint.Y), DebugLayoutRadius, vec4(1));
          // Draw polyline
          if (SelectedPoints.size() > 1)
          {
            for (INT PointInd = 0; PointInd < SelectedPoints.size() - 1; PointInd++)
            {
              vec4 SegmentColor = vec4(0.81, 0.98, 0.47, 1);
              vec3
                P1 = vec3(Flat.SegmentPoints[SelectedPoints[PointInd]].Pos.X, 0, Flat.SegmentPoints[SelectedPoints[PointInd]].Pos.Y),
                P2 = vec3(Flat.SegmentPoints[SelectedPoints[PointInd + 1]].Pos.X, 0, Flat.SegmentPoints[SelectedPoints[PointInd + 1]].Pos.Y);
              Ani->DrawCylinder(P1, DebugLayoutRadius * 1.1, P2, DebugLayoutRadius * 1.1, SegmentColor);
            }
            // connect polyline edges
            if (SelectedPoints.size() > 2)
            {
              vec4 SegmentColor = vec4(0.81, 0.98, 0.47, 1);
              INT LastPoint = SelectedPoints[SelectedPoints.size() - 1];
              vec3
                P1 = vec3(Flat.SegmentPoints[SelectedPoints[0]].Pos.X, 0, Flat.SegmentPoints[SelectedPoints[0]].Pos.Y),
                P2 = vec3(Flat.SegmentPoints[LastPoint].Pos.X, 0, Flat.SegmentPoints[LastPoint].Pos.Y);
              Ani->DrawCylinder(P1, DebugLayoutRadius * 1.1, P2, DebugLayoutRadius * 1.1, SegmentColor);
            }
          }
          for (auto &[Key, Sgm] : Flat.Segments)
          {
            vec4 SegmentColor = vec4(0.6, 0.6, 0.6, 1); // Default segment color
            /*if (std::find(SelectedPoints.begin(), SelectedPoints.end(), Sgm.Point1) != SelectedPoints.end() &&
                std::find(SelectedPoints.begin(), SelectedPoints.end(), Sgm.Point2) != SelectedPoints.end())
              SegmentColor = vec4(0.81, 0.98, 0.47, 1);*/
            vec3
              P1 = vec3(Flat.SegmentPoints[Sgm.Point1].Pos.X, 0, Flat.SegmentPoints[Sgm.Point1].Pos.Y),
              P2 = vec3(Flat.SegmentPoints[Sgm.Point2].Pos.X, 0, Flat.SegmentPoints[Sgm.Point2].Pos.Y);
            Ani->DrawCylinder(P1, DebugLayoutRadius * 0.9, P2, DebugLayoutRadius * 0.9, SegmentColor);
          }
        }
        SystemProfiler.Collect("Rooms Draw");
      } /* End of 'Render' function */
    }; /* End of 'rooms_editor' class */
    static units::editor::unit_register<units::rooms_editor> _("Rooms editor unit");
  } /* end of 'units' namespace */
} /* end of 'tam' namespace */

/* END OF 'tam_rooms_editor.cpp' FILE */
