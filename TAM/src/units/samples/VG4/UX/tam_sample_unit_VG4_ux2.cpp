/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_sample_unit_VG4_ux2.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX test unit - ImGui panel with vertical tabs,
 *               split bar, menu bar with font options,
 *               and various control examples.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 10.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Project namespace */
namespace tam
{
  /* Units namespace */
  namespace units
  {
    /* UX test unit class */
    class ux_test2 : public unit
    {
    private:

      // UX sample
      ux::tab Tab1;
      ux::tab Tab2;
      ux::tab Tab3;
      ux::flt_type X {Tab1, "X", 0.0f, -1.0f, 1.0f};
      ux::flt_type X1 {Tab2, "X1", 0.0f, -1.0f, 1.0f};
      ux::flt_type X2 {Tab2, "X2", 0.0f, -1.0f, 1.0f};
      ux::flt_type X3 {Tab2, "X3", 0.0f, -1.0f, 1.0f};
      ux::flt_type X4 {Tab2, "X4", 0.0f, -1.0f, 1.0f};
      ux::separator_type _s1 {Tab1, "Color"};
      ux::color_type C1 {Tab1, "CC"};
      ux::button But {Tab1, "Press me"};
      ux::bool_type Bo1 {Tab1, "Is?", TRUE, "Off", "On"};
      ux::select_type Sel1 {Tab1, "Mode", {"A", "B", "C"}, 2};
      ux::select_type Sel2 {Tab3, "Mode1", {"A1", "B2", "C3"}, 0};
      ux::separator_type _s2 {Tab1, "Deals (Triangulation)"};
      ux::button ButClear {Tab1, "Clear Points"};
      ux::label_type PointsSize {Tab1, "Points size:"};
      ux::separator_type _s2m {Tab1, "Model handle"};
      ux::string_type MdlFileName {Tab1, ""};
      ux::select_type MdlAnimList {Tab1, "Animation list", {}};
      ux::button LoadModel {Tab1, "Load model"};
      ux::sameline_type _Sl1 {Tab1};
      ux::button RemoveModel {Tab1, "Remove model"};
      ux::sameline_type _Sl2 {Tab1};
      ux::bool_type RunModelAnim {Tab1, "Run animation", FALSE, "Off", "On"};
      ux::separator_type _s2mt {Tab1, "Model transfrom"};
      ux::flt_type MdlDx {Tab1, "Model Offset X", 0.0f, -30.0f, 30.0f};
      ux::flt_type MdlDy {Tab1, "Model Offset Y", 0.0f, -30.0f, 30.0f};
      ux::flt_type MdlDz {Tab1, "Model Offset Z", 0.0f, -30.0f, 30.0f};

      ux::flt_type MdlSx {Tab1, "Model Scale", 1.0f, 0.01f, 30.0f};
      //ux::flt_type MdlSy {Tab1, "Model Scale Y", 1.0f, 0.01f, 30.0f};
      //ux::flt_type MdlSz {Tab1, "Model Scale Z", 1.0f, 0.01f, 30.0f};

      ux::flt_type MdlRx {Tab1, "Model Rotate X", 0.0f, -90.01f, 90.0f};
      ux::flt_type MdlRy {Tab1, "Model Rotate Y", 0.0f, -90.01f, 90.0f};
      ux::flt_type MdlRz {Tab1, "Model Rotate Z", 0.0f, -90.01f, 90.0f};
      ux::button MdlReset {Tab1, "Reset Transform"};

      ux::select_type UnitsList;// {Tab1, "Units list", {}};
      ux::button AddUnit {Tab1, "Add unit"};
      ux::sameline_type _Sl3 {Tab1};
      ux::button ClearUnits {Tab1, "Remove all units"};


      // Debug view guidelines
      std::optional<prim> Guides;

      // Editor position
      vec3 EditPos;      // Edit coordinates
      BOOL IsEditPos {}; // Editor position access flag

      // Points
      std::vector<vec3> Points;

      // Test model
      model Mdl {};

      // Work tabs
      ux::tab TabParameters, TabWallEditor, TabFurniture, TabPlacement, TabLamps, TabInfo;

    public:

      /* Class constructor.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       */
      ux_test2( anim *Ani ) :
        Tab1(Ani, "Test", ux::icon("test/file_edit.ico")),
        Tab2(Ani, "Test2"),
        Tab3(Ani, "", ux::icon("test/file_edit.ico")),
        UnitsList(Tab1, "Units list", Ani->GetRegUnitNamesList()),
        TabParameters(Ani, "Parameters", ux::icon("editor/EditorParameters1.png")), //
        TabWallEditor(Ani, "Walls", ux::icon("editor/EditorWallEditor.png")),
        TabFurniture(Ani, "Furniture", ux::icon("editor/EditorFurniture.png")),
        TabPlacement(Ani, "Placement", ux::icon("editor/EditorPlacement.png")), //
        TabLamps(Ani, "Lamps", ux::icon("editor/EditorLamps.png")),
        TabInfo(Ani, "Info", ux::icon("editor/EditorInfo.png"))
      {
        std::vector<vec2> P
        {
          {0, 0},
          {0, 1},
          {1, 1},
          {1, 0},
        };

        auto ind = mth::Triangulate(P);
        Guides = Ani->CreateGuideLines();
      } /* End of 'ux_test2' function */

      /* Class destructor */
      ~ux_test2( VOID ) override
      {
      } /* End of '~ux_test2' function */

      /* Render unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Render( anim *Ani ) override
      {
        //if (Guides)
        //  Ani->Draw(Guides.value());
        Ani->Draw(Ani->Guidelines);
        if (IsEditPos)
          Ani->DrawSphere(EditPos, 0.30, vec3(1));
        for (auto p : Points)
          Ani->DrawSphere(p, 0.18, vec3(1, 0, 0));
        for (INT i = 0; i < Points.size(); i++)
          Ani->DrawCylinder(Points[i], 0.08, Points[(i + 1) % Points.size()], 0.08, vec3(1, 1, 0));
        if (Points.size() > 2)
        {
          std::vector<vec2> pnts;
          for (auto &p : Points)
            pnts.push_back({p.X, -p.Z});
          auto ind = mth::Triangulate(pnts);
          for (INT i = 0; i < ind.size(); i += 3)
          {
            Ani->DrawCylinder(Points[ind[i + 0]], 0.05, Points[ind[i + 1]], 0.05, vec3(0, 1, 0));
            Ani->DrawCylinder(Points[ind[i + 1]], 0.05, Points[ind[i + 2]], 0.05, vec3(0, 1, 0));
            Ani->DrawCylinder(Points[ind[i + 2]], 0.05, Points[ind[i + 0]], 0.05, vec3(0, 1, 0));
          }
        }
        if (Mdl != nullptr)
        {
          Mdl->DoAnim("Walk", Ani->Time);
          matr M =
            matr::Scale(vec3{MdlSx, MdlSx, MdlSx}) *
            matr::RotateY(MdlRy) *
            matr::RotateX(MdlRx) *
            matr::RotateZ(MdlRz) *
            matr::Translate(MdlDx, MdlDy, MdlDz);
          Ani->Draw(Mdl, M);
        }
      } /* End of 'Render' function */

      /* Response unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
        ray R = Ani->Cam.FrameRay(Ani->Mx, Ani->My);
        if (R.Dir.Y != 0)
        {
          IsEditPos = TRUE;
          EditPos = R(-R.Org.Y / R.Dir.Y);
        }

        if (IsEditPos && Ani->KeysClick[VK_LBUTTON] && !Ani->Keys[VK_CONTROL] && Ani->Keys[VK_SHIFT])
          Points.push_back(EditPos);

        if (But)
          Sel1.Add("Text addon");
        if (ButClear)
          Points.clear();
        PointsSize = std::format("Points size: {}", Points.size());

        if (AddUnit)
          *Ani << UnitsList.GetSelected();
        if (ClearUnits)
          Ani->CurrentScene.ClearInsteadOf(this);

        if (LoadModel)
        {
          std::string fn = Ani->OpenFileDialog("Load Model", "Models\0*.gltf\0All\0*.*\0");
          if (!fn.empty())
          {
            if (Mdl != nullptr)
              Ani->ModelDelete(Mdl), MdlAnimList.Clear();
            MdlFileName = fn;
            Mdl = Ani->ModelCreate(fn);
            if (Mdl != nullptr)
            for (auto &a : Mdl->GLTFData.Animations)
              MdlAnimList.Add(a.Name);
          }
        }
        if (RunModelAnim && Mdl != nullptr)
          Mdl->DoAnim(MdlAnimList.GetSelected(), Ani->Time);

        if (RemoveModel && Mdl != NULL)
          Ani->ModelDelete(Mdl), MdlFileName = "", MdlAnimList.Clear();
        if (MdlReset)
        {
          MdlSx = 1; // MdlSy = MdlSz = 1,
          MdlRx = 0;
          MdlRy = MdlRz = MdlDx = MdlDy = MdlDz = 0;
        }
      } /* End of 'Response' function */
    }; /* End of 'ux_test2' class */
  } /* end of 'units' namespace */
  static anim::unit_register<units::ux_test2> _("VG4 ux test2");
} /* end of 'tam' namespace */

/* END OF 'tam_sample_unit_VG4_ux2.cpp' FILE */
