/********************************14*******************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_placement_editor_unit.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Flat parameters placement_editor unit.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 01.05.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_editor_unit.h"

/* Project namespace */
namespace tam
{
  /* Units namespace */
  namespace units
  {
    /* UX test unit class */
    class placement_editor : public editor_unit
    {
      friend flat;
    private:
      INT NumOfModels {}; // Test primitive
      BOOL IsModelsUsed[5] {};
      vec2 MousePoint{};

      // Editor tab control
      ux::tab &Tab;
      ux::user_type DrawSettings {Tab};                                        // User type drawing settings
      ux::user_type ModelParams {Tab};                                         // Furniture model angle and position
      std::string NameOfRoom {};                                               // Number of selected rooms in flat
      std::string NameOfModel {};                                              // Name of selected furniture
      INT PlaceNumber = -1;                                                    // Order number of selected place
      INT NameOfFurniture {};                                                  // Name of furniture
      INT CurId = -1;                                                          // Current Id
      BOOL IsSelectedByMouse = FALSE;                                          // Flag of selected model
      BOOL IsSomethingSelected = FALSE;                                        // Flag of selected something


    public:
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
      placement_editor( anim *Ani, flat &WorkFlat, ux::tab &WorkTab, const std::function<VOID ( VOID )> &UserUIFunc ) : editor_unit(Ani, WorkFlat), Tab(WorkTab)
      {
        DrawSettings.SetUserFunc(UserUIFunc);

        ModelParams.SetUserFunc([Ani, this](VOID)
        {
          ImGuiWindow *Window = ImGui::GetCurrentWindow();

          static bool FixY45 = FALSE;
          auto AngleUI =
            [this]( const std::string &Label, FLT &AngleValue, bool &Fix45 )
            {
              ImGui::Text(Label.c_str());
              ImGui::SameLine();
              if (ImGui::SliderFloat(std::format("##Model{}", Label).c_str(), &AngleValue, -180, 180, "%.1f\xC2\xB0"))
                (VOID)0;
              ImGui::SameLine();
              if (ImGui::Button(std::format("0\xC2\xB0##set0{}", Label).c_str()))
                AngleValue = 0;
              ImGui::SameLine();
              ImGui::Checkbox(std::format("45\xC2\xB0##set45{}", Label).c_str(), &Fix45);
              if (Fix45)
                AngleValue = roundf(AngleValue / 45.0f) * 45.0f;
              while (AngleValue < -180)
                AngleValue += 360;
              while (AngleValue > 180)
                AngleValue -=  360;
            };

          // Picked model transform
          if (IsSomethingSelected && NameOfModel != "" && NameOfRoom != "")
          {
            if (auto mit = Flat.FlatFurnitureCollection.find(NameOfModel); mit != Flat.FlatFurnitureCollection.end())
            {
              auto &[FurParams, Mdl] = mit->second;
              if (Mdl != nullptr)
              {
                if (auto rit = Flat.Rooms.find(NameOfRoom); rit != Flat.Rooms.end())
                {
                  auto &Room = rit->second;
                  if (auto fit = Room.Furniture.find(NameOfModel); fit != Room.Furniture.end())
                  {
                    auto &FurPlaces = fit->second;
                    if (PlaceNumber >= 0 && PlaceNumber < FurPlaces.size())
                    {
                      auto &Place = FurPlaces[PlaceNumber];
                      AngleUI("RotateY", Place.RotationY, FixY45);
                      ImGui::Text("Position:");
                      ImGui::SameLine();
                      if (ImGui::SliderFloat3("##ModelPosition", &Place.Pos.X, -18, 18))
                       (VOID)0;
                      ImGui::SameLine();
                      if (ImGui::Button("Set to 0##set0offset"))
                        Place.Pos = vec3(0);
                      ImGui::AlignTextToFramePadding();
                      if (ImGui::Button("To floor##placefloor"))
                        Place.Pos.Y = 0;
                    }
                  }
                }
              }
            }
          }
        });
      } /* End of 'placement_editor' function */

      /* Class destructor */
      ~placement_editor( VOID ) override
      {
      } /* End of '~placement_editor' function */

      /* Found furniture by click of mouse function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       *   - room name:
       *       const std::string &RoomName;
       *   - transform matrix:
       *       const matr &Transform;
       * RETURNS: None.
       */
      VOID FoundFurniture( anim *Ani )
      {
        NameOfModel = "";
        NameOfRoom = "";
        PlaceNumber = -1;
        NameOfFurniture = {};
        // All rooms
        for (auto &[RoomName, Room] : Flat.Rooms)
        {
          // All furniture
          for (auto &[FurName, FurPlaces] : Room.Furniture)
          {
            mth::ray<FLT> R = Ani->Cam.FrameRay(Ani->Mx, Ani->My);
            DBL t = R.IntersectPlane();
            MousePoint = vec2(R(t).X, R(t).Z);
            // GetFurByPoint(MousePoint, RoomName);

            auto &[FurParams, Mdl] = Flat.FlatFurnitureCollection[FurName];
            if (Mdl == nullptr)
              continue;
            //matr FurModelMatr = FurParams.GetTransform();
            INT place_no = 0;
            for (auto &Place : FurPlaces)
            {
              //matr FurMatr = FurModelMatr * matr::RotateY(Place.RotationY) * matr::Translate(Place.Pos);
              matr PlaceMatr = Place.GetTransform();
              // Calculate ray
              ray R = Ani->Cam.FrameRay(Ani->Mx, Ani->My);
              matr m1 = PlaceMatr.Inverse();
              ray R1 = ray(m1.TransformPoint(R.Org), m1.TransformVector(R.Dir));

              BOOL IsSelected = FALSE;
              if (R1.IsBoxIntersect(Mdl->MinBB, Mdl->MaxBB))
              {
                //Ani->DrawCylinder(vec3(5, 0, -5), 0.12, vec3(5, 5, -5), 0.12, vec4(0, 1, 0, 1));
                NameOfModel = FurName;
                NameOfRoom = RoomName;
                NameOfFurniture = Place.Id;
                Ani->DrawAABBMatr(Mdl->MinBB, Mdl->MaxBB, vec4(vec3(0, 1, 1), 1), PlaceMatr);
                PlaceNumber = place_no;
                CurId = Place.Id;
                IsSelected = TRUE;
              }

              /*if (DrawAABB)
              Ani->DrawAABBMatr(Mdl->MinBB, Mdl->MaxBB, vec4(vec3(1, 1, 1), 1), PlaceMatr);*/
              place_no++;
            }
          }
        }
      }

      /* Response unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
        static INT DragMouseX = 0, DragMouseZ = 0;
        static vec3 DragStorePos;
        static BOOL IsDragging = FALSE;

        // Model pick
        if (Ani->KeysClick[MK_LBUTTON])// && !IsSomethingSelected)
        {
          FoundFurniture(Ani);
          if (PlaceNumber != -1)
          {
            IsSomethingSelected = TRUE;
            IsSelectedByMouse = TRUE;
          }
          else
          {
            IsSomethingSelected = FALSE;
            IsSelectedByMouse = FALSE;
          }
        }
        // Mouse drag
        if (PlaceNumber != -1 && Ani->Keys[MK_LBUTTON] && (Ani->Mdx != 0 || Ani->Mdy != 0))
          IsDragging = TRUE;
        else
          IsDragging = FALSE;

        // Picked model transform
        if (!Ani->Keys[VK_CONTROL] && IsSomethingSelected && NameOfModel != "" && NameOfRoom != "")
        {
          if (auto mit = Flat.FlatFurnitureCollection.find(NameOfModel); mit != Flat.FlatFurnitureCollection.end())
          {
            auto &[FurParams, Mdl] = mit->second;
            if (Mdl != nullptr)
            {
              if (auto rit = Flat.Rooms.find(NameOfRoom); rit != Flat.Rooms.end())
              {
                auto &Room = rit->second;
                if (auto fit = Room.Furniture.find(NameOfModel); fit != Room.Furniture.end())
                {
                  auto &FurPlaces = fit->second;
                  if (PlaceNumber >= 0 && PlaceNumber < FurPlaces.size())
                  {
                    // Rotate
                    auto &Place = FurPlaces[PlaceNumber];
                    if (Ani->Keys['Y'])
                      Place.Pos.Y += Ani->Mdz / 12000.0;
                    else if (Ani->Keys['X'])
                      Place.Pos.X += Ani->Mdz / 12000.0;
                    else if (Ani->Keys['Z'])
                      Place.Pos.Z += Ani->Mdz / 12000.0;
                    else
                      Place.RotationY += Ani->Mdz / 120.0;

                    // Position dragging
                    mth::ray<FLT> Rs = Ani->Cam.FrameRay(Ani->Mx, Ani->My);
                    if (DBL ts = Rs.IntersectPlane(); ts > 0)
                    {
                      vec3 p = vec3(Rs(ts).X, Place.Pos.Y, Rs(ts).Z);

                      if (Ani->Keys[MK_LBUTTON])
                      {
                        if (!IsDragging)
                          DragStorePos = p - Place.Pos;
                        if (Ani->Mdx != 0 || Ani->Mdy != 0)
                          if (!IsDragging)
                            IsDragging = TRUE;
                        if (IsDragging)
                          Place.Pos = p - DragStorePos;
                      }
                      else
                        IsDragging = FALSE;
                    }

                  }
                }
              }
            }
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
        if (NameOfModel != "" && NameOfRoom != "")
        {
          if (auto mit = Flat.FlatFurnitureCollection.find(NameOfModel);
              mit != Flat.FlatFurnitureCollection.end())
          {
            auto &[FurParams, Mdl] = mit->second;
            if (Mdl != nullptr)
            {
              if (auto rit = Flat.Rooms.find(NameOfRoom);
                  rit != Flat.Rooms.end())
              {
                auto &Room = rit->second;
                if (auto fit = Room.Furniture.find(NameOfModel);
                    fit != Room.Furniture.end())
                {
                  auto &FurPlaces = fit->second;
                  if (PlaceNumber >= 0 && PlaceNumber < FurPlaces.size())
                  {
                    auto &Place = FurPlaces[PlaceNumber];
                    matr PlaceMatr = Place.GetTransform();
                    Ani->DrawAABBMatr(Mdl->MinBB, Mdl->MaxBB, vec4(vec3(0, 1, 1), 1), PlaceMatr);
                  }
                }
              }
            }
          }
        }
      } /* End of 'Render' function */
    }; /* End of 'placement_editor' class */
  } /* end of 'units' namespace */
  static units::editor::unit_register<units::placement_editor> _("Placement editor unit");
} /* end of 'tam' namespace */

/* END OF 'tam_placement_editor_unit.cpp' FILE */
