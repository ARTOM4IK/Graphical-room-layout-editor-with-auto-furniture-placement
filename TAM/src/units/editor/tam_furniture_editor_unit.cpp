/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_furniture_editor_unit.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Flat information furniture_editor unit.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3),
 *               Arsentev Artemy (AA4),
 *               Nechaev Vladimir (VN4),
 *               Kupchenkov Ivan (IK4),
 *               Novozhilova Maryana (MN7),
 *               Tretyakov Andrey (AT7),
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 01.05.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "systems/flat/tam_furniture.h"

/* Project namespace */
namespace tam
{
  /* Units namespace */
  namespace units
  {
    /* Furniture edtitor unit class */
    class furniture_editor : public editor_unit
    {
    private:
      // Editor tab control
      ux::tab &Tab;
      // User type drawing settings
      ux::user_type DrawSettings {Tab};

      ux::string_type ObjIdView {Tab, ""};
      ux::group_begin GrOpen {Tab, "Collection"};
      // ux::separator_type _SepCol {Tab, "Collection"};
      ux::button CollectionLoad {Tab, "Load"};
      ux::sameline_type _SL1 {Tab};
      ux::button CollectionSave{Tab, "Save"};
      ux::sameline_type _SL2 {Tab};
      ux::text_type FileName {Tab, ""};
      ux::user_type FurTree {Tab};
      /*
      ux::group_begin GrOpen1 {Tab, "Mds"};
      ux::radio_type Radio {Tab, "Modes", {"Select1", "Select2", "Select3"}, 2};
      ux::radio_type Radio1 {Tab, "", {"Select1", "Select2", "Select3"}, 1};
      ux::group_end GeEnd1 {GrOpen1};
      */

      ux::group_end GrEnd {GrOpen};

      ux::separator_type _SepAddMdl {Tab, "Place to room"};
      ux::select_type RoomSelector {Tab, "Select room", {"Select room"}, 0};
      ux::button AddToRoomButton{Tab, "Add furniture to room"};

      ux::group_begin GrOpenM {Tab, "Model"};
      ux::separator_type _SepMdl {Tab, "Model Setup"};
      ux::user_type ModelParams {Tab};

      ux::bool_type ModelDrawBB {Tab, "AABB", FALSE, "Draw", "Hide"};
      ux::sameline_type _SLM1 {Tab};
      ux::color_type ModelDrawBBColor {Tab, "", vec3(1, 1, 0)};
      ux::bool_type ModelDrawBBIn {Tab, "AABBIn", FALSE, "Draw", "Hide"};
      ux::sameline_type _SLM2 {Tab};
      ux::color_type ModelDrawBBColorIn {Tab, "", vec3(1, 0, 0)};
      ux::group_end GeEndM {GrOpenM};

      ux::user_type PackHandleType {Tab}; // Model tree editor user type

      INT ModelType = 0;

      // Current type number
      INT CurrentType = -1;
      // Current model number
      INT CurrentModel = -1;
      // Selected type number
      INT SelectedType = -1;
      // Selected model number
      INT SelectedModel = -1;

      // Current model
      model Mdl {};

      // Collection
      furniture_collection Collection;

      // Last frame when unit was active
      INT LastAccessFrame {};

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
      furniture_editor( anim *Ani, flat &WorkFlat, ux::tab &WorkTab, const std::function<VOID ( VOID )> &UserUIFunc ) :
        editor_unit(Ani, WorkFlat), Tab(WorkTab)
      {
        Collection.Load();
        DrawSettings.SetUserFunc(UserUIFunc);
        FurTree.SetUserFunc([Ani, this](VOID)
        {
          ImGuiWindow *Window = ImGui::GetCurrentWindow();

          ImGui::AlignTextToFramePadding();
          static CHAR CollectionNameBuf[300];
          ImGui::Text("Name:");
          ImGui::SameLine();
          strncpy(CollectionNameBuf, Collection.Name.c_str(), sizeof(CollectionNameBuf) - 1);
          if (ImGui::InputText("##CollectionNameEditLine", CollectionNameBuf, sizeof(CollectionNameBuf),
                ImGuiInputTextFlags_None /* | ImGuiInputTextFlags_EnterReturnsTrue */))
            Collection.Name = CollectionNameBuf;

          static CHAR Buf[100];
          if (ImGui::Button("+##add_new_type"))
            if (Trim(Buf) != "")
              Collection.Types.push_back({Buf, {}}), Buf[0] = 0;
          if (ImGui::BeginItemTooltip())
          {
            ImGui::TextUnformatted("Add a new furniture type with name on the right");
            ImGui::EndTooltip();
          }
          ImGui::SameLine();
          BOOL root_open =
            ImGui::TreeNodeEx("Furniture Collection",
              ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen);
          if (!ImGui::IsItemHovered())
            ImGui::SameLine(), ImGui::InputText("##new_type_input", Buf, sizeof(Buf));
          if (root_open)
          {
            INT tno = 0;

            for (auto &t : Collection.Types)
            {
              if (ImGui::SmallButton(std::format("X##deltype{}", tno).c_str()))
                Ani->UXConfirm(std::format("Remove type: '{}'", t.TypeName).c_str(), "\nAre you sure\nto delete this type?\n\n",
                  [tno, this]( INT ButtonNo )
                  {
                    if (ButtonNo == 0)
                    {
                      Collection.Types.erase(Collection.Types.begin() + tno);
                      SelectedType = -1, SelectedModel = -1;
                    }
                  }, "Yes", "No");
              ImGui::SameLine();
              if (ImGui::SmallButton(std::format("+##addmodel{}", tno).c_str()))
              {
                std::string p = Ani->OpenFileDialog("Load model", "Models\0*.g3dm;*.gltf;*.glb\0All\0*.*\0");
                if (!p.empty())
                {
                  t.ModelList.
                    push_back({.Name = std::filesystem::path(p).stem().string(),
                               .FileName = p,
                               .Rotate = {0, 0, 0},
                               .Scale = 1,
                               .Size = {0, 0, 0},
                               .Offset = {0, 0, 0},
                               .MinBB = {0, 0, 0},
                               .MaxBB = {0, 0, 0}});
                  SelectedType = tno;
                  SelectedModel = (INT)t.ModelList.size() - 1;
                  CurrentType = -1;
                  CurrentModel = -1;
                }
              }
              if (ImGui::BeginItemTooltip())
              {
                ImGui::TextUnformatted(std::format("Add model to type: {}", t.TypeName).c_str());
                ImGui::EndTooltip();
              }
              ImGui::SameLine();
              //ImGui::PushID(std::hash<std::string>{}(std::format("##type{}", tno)));
              bool node_open = ImGui::TreeNodeEx(std::format("{}##type{}", t.TypeName, tno).c_str(), ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen);
              // Quick context menu
              auto type_menu_id = std::format("model_menu##{}", tno);
              if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                ImGui::OpenPopup(type_menu_id.c_str());

              if (ImGui::BeginPopupContextItem(type_menu_id.c_str()))
              {
                if (ImGui::MenuItem("Rename"))
                {
                  static std::string EditBuf;
                  EditBuf = t.TypeName;
                  Ani->UXEdit(std::format("Rename type: '{}'", t.TypeName).c_str(), "", EditBuf,
                    [tno, this]( INT ButtonNo )
                    {
                      if (ButtonNo == 0)
                        Collection.Types[tno].TypeName = EditBuf;
                    }, "Set new name", "Cancel");
                }
                if (ImGui::MenuItem("Delete type"))
                  Ani->UXConfirm(std::format("Remove type: '{}'", t.TypeName).c_str(), "\nAre you sure\nto delete this type?\n\n",
                    [tno, this]( INT ButtonNo )
                    {
                      if (ButtonNo == 0)
                      {
                        Collection.Types.erase(Collection.Types.begin() + tno);
                        if (SelectedType == tno)
                          SelectedType = -1, SelectedModel = -1;
                      }
                    }, "Yes", "No");
                ImGui::EndPopup();
              }
              if (node_open)
              {
                INT mno = 0;
                for (auto &mdl_param : t.ModelList)
                {
                  if (ImGui::SmallButton(std::format("X##delmodel{}-{}", tno, mno).c_str()))
                    Ani->UXConfirm(std::format("Remove model: '{}'", mdl_param.Name).c_str(), "\nAre you sure\nto delete this model?\n\n",
                      [tno, mno, this]( INT ButtonNo )
                      {
                        if (ButtonNo == 0)
                        {
                          auto tyi = Collection.Types.begin() + tno;
                          tyi->ModelList.erase(tyi->ModelList.begin() + mno);
                          if (SelectedType == tno && SelectedModel == mno)
                            SelectedType = -1, SelectedModel = -1;
                        }
                      }, "Yes", "No");
                  ImGui::SameLine();
                  if (ImGui::Selectable(std::format("{}##modelname{}-{}", mdl_param.Name, tno, mno).c_str(), SelectedType == tno && SelectedModel == mno))
                  {
                    SelectedType = tno;
                    SelectedModel = mno;
                  }
                  if (ImGui::BeginItemTooltip())
                  {
                    ImGui::TextUnformatted(std::format("File: {}", mdl_param.FileName).c_str());
                    ImGui::EndTooltip();
                  }
                  // Quick context menu
                  auto model_menu_id = std::format("model_menu##{}-{}", tno, mno);
                  if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                    ImGui::OpenPopup(model_menu_id.c_str());

                  if (ImGui::BeginPopupContextItem(model_menu_id.c_str()))
                  {
                    if (ImGui::MenuItem("Rename"))
                    {
                      static std::string EditBuf;
                      EditBuf = mdl_param.Name;
                      Ani->UXEdit(std::format("Rename model: '{}'", mdl_param.Name).c_str(), "", EditBuf,
                        [tno, mno, this]( INT ButtonNo )
                        {
                          if (ButtonNo == 0)
                            Collection.Types[tno].ModelList[mno].Name = EditBuf;
                        }, "Set new name", "Cancel");
                    }
                    if (ImGui::MenuItem("Delete model"))
                      Ani->UXConfirm(std::format("Remove model: '{}'", mdl_param.Name).c_str(), "\nAre you sure\nto delete this model?\n\n",
                        [tno, mno, this]( INT ButtonNo )
                        {
                          if (ButtonNo == 0)
                          {
                            auto tyi = Collection.Types.begin() + tno;
                            tyi->ModelList.erase(tyi->ModelList.begin() + mno);
                            if (SelectedType == tno && SelectedModel == mno)
                              SelectedType = -1, SelectedModel = -1;
                          }
                        }, "Yes", "No");
                    ImGui::EndPopup();
                  }
                  mno++;
                }
                ImGui::TreePop();
              }
              //ImGui::PopID();
              tno++;
            }
            ImGui::TreePop();
          }
        });
        ModelParams.SetUserFunc([Ani, this](VOID)
        {
          ImGuiWindow *Window = ImGui::GetCurrentWindow();

          if (SelectedType >= Collection.Types.size() || SelectedModel >= Collection.Types[SelectedType].ModelList.size())
            SelectedType = -1, SelectedModel = -1;

          if (SelectedType != -1 && SelectedModel != -1)
          {
            if (CurrentType != SelectedType || CurrentModel != SelectedModel)
            {
              CurrentType = SelectedType;
              CurrentModel = SelectedModel;
              auto &CurModel = Collection.Types[SelectedType].ModelList[SelectedModel];
              Mdl = Ani->ModelCreate(CurModel.FileName); // , CurModel.GetTransform());
              // std::tie(Mdl->MinBB, Mdl->MaxBB) = Mdl->RecalculateAABB(matr::Identity());
              Collection.Types[SelectedType].ModelList[SelectedModel].RecalcBB(Mdl);
            }
            auto &CurModel = Collection.Types[SelectedType].ModelList[SelectedModel];
            ImGui::AlignTextToFramePadding();
            static CHAR TypeNameBuf[300];
            ImGui::Text("Type:");
            ImGui::SameLine();
            strncpy(TypeNameBuf, Collection.Types[SelectedType].TypeName.c_str(), sizeof(TypeNameBuf) - 1);
            if (ImGui::InputText("##TypeNameEditLine", TypeNameBuf, sizeof(TypeNameBuf),
                  ImGuiInputTextFlags_None /* | ImGuiInputTextFlags_EnterReturnsTrue */))
              Collection.Types[SelectedType].TypeName = TypeNameBuf;

            std::string item_id = std::string("Furniture") + "##si" + std::to_string(0);
            ImGui::RadioButton(item_id.c_str(), &ModelType, 0);
            if ((ModelType == 0))
              ImGui::SetItemDefaultFocus();
            ImGui::SameLine();

            item_id = std::string("Window") + "##si" + std::to_string(1);
            ImGui::RadioButton(item_id.c_str(), &ModelType, 1);
            if ((ModelType == 1))
              ImGui::SetItemDefaultFocus();
            ImGui::SameLine();

            item_id = std::string("Door") + "##si" + std::to_string(2);
            ImGui::RadioButton(item_id.c_str(), &ModelType, 2);
            if ((ModelType == 2))
              ImGui::SetItemDefaultFocus();
            ImGui::SameLine();

            item_id = std::string("Lamp") + "##si" + std::to_string(3);
            ImGui::RadioButton(item_id.c_str(), &ModelType, 3);
            if ((ModelType == 3))
              ImGui::SetItemDefaultFocus();

            ImGui::AlignTextToFramePadding();
            static CHAR ModelNameBuf[300];
            ImGui::Text("Model name:");
            ImGui::SameLine();
            strncpy(ModelNameBuf, CurModel.Name.c_str(), sizeof(TypeNameBuf) - 1);
            if (ImGui::InputText("##ModelNameEditLine", ModelNameBuf, sizeof(ModelNameBuf),
                  ImGuiInputTextFlags_None /* | ImGuiInputTextFlags_EnterReturnsTrue */))
              CurModel.Name = ModelNameBuf;

            static bool FixX45 = FALSE;
            static bool FixY45 = FALSE;
            static bool FixZ45 = FALSE;
            auto AngleUI =
              [&CurModel, this]( const std::string &Label, FLT &AngleValue, bool &Fix45 )
              {
                ImGui::Text(Label.c_str());
                ImGui::SameLine();
                if (ImGui::SliderFloat(std::format("##Model{}", Label).c_str(), &AngleValue, -180, 180, "%.1f\xC2\xB0"))
                  CurModel.RecalcBB(Mdl);
                ImGui::SameLine();
                if (ImGui::Button(std::format("0\xC2\xB0##set0{}", Label).c_str()))
                  AngleValue = 0, CurModel.RecalcBB(Mdl);;
                ImGui::SameLine();
                ImGui::Checkbox(std::format("45\xC2\xB0##set45{}", Label).c_str(), &Fix45);
                if (Fix45)
                  AngleValue = roundf(AngleValue / 45.0f) * 45.0f, CurModel.RecalcBB(Mdl);
              };

            ImGui::Text("Scale:");
            ImGui::SameLine();
            if (ImGui::SliderFloat("##ModelScale", &CurModel.Scale, 0.001, 8))
              CurModel.RecalcBB(Mdl);

            AngleUI("RotateX", CurModel.Rotate.X, FixX45);
            AngleUI("RotateY", CurModel.Rotate.Y, FixY45);
            AngleUI("RotateZ", CurModel.Rotate.Z, FixZ45);

            ImGui::Text("Offset:");
            ImGui::SameLine();
            if (ImGui::SliderFloat3("##ModelOffset", &CurModel.Offset.X, -8, 8))
              CurModel.RecalcBB(Mdl);
            ImGui::SameLine();
            if (ImGui::Button("Set to 0##set0offset"))
              CurModel.Offset = vec3(0), CurModel.RecalcBB(Mdl);
            ImGui::Text(std::format("Size (W/x, H/y, D/z): {:.3f}m x {:.3f}m x {:.3f}m",
              CurModel.Size.X, CurModel.Size.Y, CurModel.Size.Z).c_str());

            /// for (INT i = 0; i < Ani->OpaqAttach.size(); i++)
            /// {
            ///   vec4 cid = Ani->OpaqAttach[i]->GetColor(vec2(Ani->Mx, Ani->My));
            ///   ImGui::Text(std::format("OA[{}]-Id: {:.5f} {:.5f} {:.5f} {:.5f}",
            ///     i, cid.X, cid.Y, cid.Z, cid.W).c_str());
            /// }
            /// for (INT i = 0; i < Ani->TransAttach.size(); i++)
            /// {
            ///   vec4 cid = Ani->TransAttach[i]->GetColor(vec2(Ani->Mx, Ani->My));
            ///   ImGui::Text(std::format("TA[{}]-Id: {:.5f} {:.5f} {:.5f} {:.5f}",
            ///     i, cid.X, cid.Y, cid.Z, cid.W).c_str());
            /// }
            /// 
            /// vec4 cid = Ani->MergeTex->GetColor(vec2(Ani->Mx, Ani->My));
            /// ImGui::Text(std::format("Id: {:.5f} {:.5f} {:.5f} {:.5f}",
            ///   cid.X, cid.Y, cid.Z, cid.W).c_str());


            ImGui::AlignTextToFramePadding();
            if (ImGui::Button("To floor##placefloor"))
              CurModel.Offset.Y += -CurModel.MinBB.Y, CurModel.RecalcBB(Mdl);
            ImGui::SameLine();
            if (ImGui::Button("Center##center"))
              CurModel.Offset += -CurModel.MinBB - (CurModel.MaxBB - CurModel.MinBB) / 2, CurModel.RecalcBB(Mdl);
            ImGui::SameLine();
            static INT FitAxis = 0;
            static FLT FitSize = 1;
            if (ImGui::Button("Fit to:##fittounitbox"))
            {
              // Prepare transform matrix
              vec3 B = CurModel.MaxBB - CurModel.MinBB;
              FLT scv = CurModel.Scale;
              if (FitAxis == 0)
                scv = FitSize / B.X;
              else if (FitAxis == 1)
                scv = FitSize / B.Y;
              else if (FitAxis == 2)
                scv = FitSize / B.Z;
              CurModel.Scale *= scv;
              CurModel.RecalcBB(Mdl);
              CurModel.Offset.Y += -CurModel.MinBB.Y;
              CurModel.Offset.X += -CurModel.MinBB.X - (CurModel.MaxBB.X - CurModel.MinBB.X) / 2;
              CurModel.Offset.Z += -CurModel.MinBB.Z - (CurModel.MaxBB.Z - CurModel.MinBB.Z) / 2;
              CurModel.RecalcBB(Mdl);
              //CurModel.Offset.Y = CurModel.MinBB.Y * scv;
            }
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::CalcTextSize("8888888888").x);
            ImGui::DragFloat("##sizefit", &FitSize, 0.01f);
            //ImGui::InputFloat("Size", &FitSize, 0.0001f, 100.0f, "%.3f");
            ImGui::SameLine();
            ImGui::RadioButton("X##fx1", &FitAxis, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Y##fx1", &FitAxis, 1);
            ImGui::SameLine();
            ImGui::RadioButton("Z##fx1", &FitAxis, 2);
          }
          else
            if (Mdl != nullptr)
              Ani->ModelDelete(Mdl);
        });

        PackHandleType.SetUserFunc([Ani, this]( VOID )
        {
          if (Mdl != nullptr && (Mdl->ModelType == "GLTF" || Mdl->ModelType == "GLB"))
          {
            // Set tree lines style
            ImGuiStyle& style = ImGui::GetStyle();
            style.TreeLinesSize = 3.0f;
            style.TreeLinesRounding = 5.0f;
            ImGuiTreeNodeFlags BaseFlags = ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_DefaultOpen;
            std::function<VOID( INT )> DrawNode = [this, &BaseFlags, &DrawNode]( INT NodeIndex )
            {
              ImGuiTreeNodeFlags TreeDrawFlags = BaseFlags;
              std::string NodeName = Mdl->GLTFData.Nodes[NodeIndex].Name.size() > 0 ? Mdl->GLTFData.Nodes[NodeIndex].Name : "No name";
              if (Mdl->GLTFData.Nodes[NodeIndex].Subnodes.size() > 0)
              {
                if (ImGui::TreeNodeEx(NodeName.c_str(), TreeDrawFlags))
                {
                  for (INT &ChildNode : Mdl->GLTFData.Nodes[NodeIndex].Subnodes)
                    DrawNode(ChildNode);
                  ImGui::TreePop();
                }
              }
              else
              {
                TreeDrawFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
                ImGui::TreeNodeEx(NodeName.c_str(), TreeDrawFlags);
              }
            };
            for (auto &Scene : Mdl->GLTFData.Scenes)
              for (auto &RootNode : Scene.RootNodes)
                DrawNode(RootNode);
            // if there is no scenes - draw first node (!!! draw every node)
            if (Mdl->GLTFData.Scenes.size() == 0 && Mdl->GLTFData.Nodes.size() > 0)
              DrawNode(0);
          }
        });
      } /* End of 'furniture_editor' function */

      /* Class destructor */
      ~furniture_editor( VOID ) override
      {
      } /* End of '~furniture_editor' function */

      /* Response unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
        // Check if unit was unactive to clear selected points / segments or 'ESC' button was pressed
        if (LastAccessFrame != Ani->CurrentFrameNo - 1)
        {
          // Syncronize UX with current rooms list
          RoomSelector.Clear();
          RoomSelector.Add("Out of room space");
          for (auto &[Name, Room] : Flat.Rooms)
            RoomSelector.Add(Name);
        }
        LastAccessFrame = Ani->CurrentFrameNo;
        if (CollectionSave)
        {
          std::string p = Ani->SaveFileDialog("Save collection", "Collections\0*.json\0All\0*.*\0", FileName);
          if (!p.empty())
          {
            Collection.Save(p);
            FileName = p;
          }
        }
        if (CollectionLoad)
        {
          std::string p = Ani->OpenFileDialog("Save collection", "Collections\0*.json\0All\0*.*\0");
          if (!p.empty())
          {
            Collection.Load(p);
            FileName = p;
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
        if (SelectedType >= Collection.Types.size() || SelectedModel >= Collection.Types[SelectedType].ModelList.size())
          SelectedType = -1, SelectedModel = -1;
        if (SelectedType != -1 && SelectedModel != -1)
        {
          if (CurrentType != SelectedType || CurrentModel != SelectedModel)
          {
            CurrentType = SelectedType;
            CurrentModel = SelectedModel;
            auto &CurrentModel = Collection.Types[SelectedType].ModelList[SelectedModel];
            Mdl = Ani->ModelCreate(CurrentModel.FileName); // , CurrentModel.GetTransform());
          }
          auto &CurModel = Collection.Types[SelectedType].ModelList[SelectedModel];
          matr m = CurModel.GetTransform();
          //CurModel.RecalcBB(Mdl, &m);
          if (AddToRoomButton && Flat.Rooms.contains(RoomSelector.GetSelected()))
          {
            vec2 RoomCenter = Flat.Rooms[RoomSelector.GetSelected()].CenterPoint;
            Flat.AddFurnitureToRoom(RoomSelector.GetSelected(), CurModel, vec3(RoomCenter.X, 0, RoomCenter.Y));
          }
          ray R = Ani->Cam.FrameRay(Ani->Mx, Ani->My);
          if (ModelDrawBB)
          {
            Ani->DrawAABB(CurModel.MinBB, CurModel.MaxBB, vec4(ModelDrawBBColor, 1));
            vec3
              B = CurModel.MaxBB - CurModel.MinBB,
              p = CurModel.MinBB + vec3(B.X / 2, 0, B.Z), p1;
            Ani->DrawCylinder(p, 0.05, p1 = p + vec3(0, 0, 1), 0.05, vec4(ModelDrawBBColor, 1));
            Ani->DrawCylinder(p1, 0.12, p1 + vec3(0, 0, 0.30), 0, vec4(ModelDrawBBColor, 1));
            if (R.IsBoxIntersect(CurModel.MinBB, CurModel.MaxBB))
              Ani->DrawCylinder(vec3(5, 0, 0), 0.12, vec3(5, 5, 0), 0.12, vec4(ModelDrawBBColor, 1));
          }
          if (ModelDrawBBIn)
          {
            Ani->DrawAABBMatr(Mdl->MinBB, Mdl->MaxBB, vec4(ModelDrawBBColorIn, 1), m);
            matr m1 = m.Inverse();
            ray R1 = ray(m1.TransformPoint(R.Org), m1.TransformVector(R.Dir));
            if (R1.IsBoxIntersect(Mdl->MinBB, Mdl->MaxBB))
              Ani->DrawCylinder(vec3(5, 0, -5), 0.12, vec3(5, 5, -5), 0.12, vec4(ModelDrawBBColorIn, 1));
          }
          Ani->CurrentObjectId = -1;
          ObjIdView = "";
          INT Id = -1;
          if (Ani->My >= 0 && Ani->My < Ani->FrameH && Ani->Mx >= 0 && Ani->Mx < Ani->FrameW)
          {
            Ani->OpaqueIdBuf->VlkBuffer.Read(&Id, sizeof(INT), (Ani->Mx + Ani->FrameW * Ani->My) * sizeof(INT));
            // ModelId.SetText(std::format("X: {}, Y: {}, Id: {}", Ani->Mx, Ani->My, *Id));
            ObjIdView = ObjIdView.operator std::string() + std::format("Read Id: {:03}", Id);
          }
          if (Mdl != nullptr)
          {
            INT IdRef = Ani->Draw(Mdl, m);
            ObjIdView = ObjIdView.operator std::string() + std::format("Model Id: {:03}", IdRef);
            if (IdRef == Id) // Ani->MButClick[0])
              Ani->CurrentObjectId = Id;
            // Draw mesh (!!! points are transformed with setup matrix)
            /// for (INT i = 0; i < Mdl->Geometry.Triangles.size(); i++)
            /// {
            ///   vec3
            ///     P1 = m.TransformPoint(Mdl->Geometry.Vertices[Mdl->Geometry.Triangles[i].V0]),
            ///     P2 = m.TransformPoint(Mdl->Geometry.Vertices[Mdl->Geometry.Triangles[i].V1]),
            ///     P3 = m.TransformPoint(Mdl->Geometry.Vertices[Mdl->Geometry.Triangles[i].V2]);
            /// 
            ///   Ani->DrawCylinder(P1, 0.005, P2, 0.005, vec4(1));
            ///   Ani->DrawCylinder(P1, 0.005, P3, 0.005, vec4(1));
            ///   Ani->DrawCylinder(P3, 0.005, P2, 0.005, vec4(1));
            /// }
          }
        }
      } /* End of 'Render' function */
    }; /* End of 'furniture_editor' class */
  } /* end of 'units' namespace */
  static units::editor::unit_register<units::furniture_editor> _("Furniture editor unit");
} /* end of 'tam' namespace */

/* END OF 'tam_furniture_editor_unit.cpp' FILE */
