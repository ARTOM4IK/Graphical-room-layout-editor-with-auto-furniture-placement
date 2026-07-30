/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_info_editor_unit.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Flat information info_editor unit.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3),
 *               Arsentev Artemy (AA4),
 *               Nechaev Vladimir (VN4),
 *               Kupchenkov Ivan (IK4),
 *               Novozhilova Maryana (MN7),
 *               Tretyakov Andrey (AT7),
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 18.04.2026
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
    /* Info edtitor unit class */
    class info_editor : public editor_unit
    {
    private:
      // Editor tab control
      ux::tab &Tab;
      ux::color_type Color {Tab, "Info", {1, 0, 1}};

      ux::flt_type YShift {Tab, "Y shift", 0, -30, 30};
      ux::user_type DrawSettings {Tab}; // User type drawing settings

      model Models[10] {}; // Test model

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
      info_editor( anim *Ani, flat &WorkFlat, ux::tab &WorkTab, const std::function<VOID ( VOID )> &UserUIFunc ) :
        editor_unit(Ani, WorkFlat), Tab(WorkTab)
      {
        DrawSettings.SetUserFunc(UserUIFunc);
        //Models[0] = Ani->ModelCreate(R"ops(Z:\2025-26\Sr\T51VLK\bin\models\Sunflower.g3dm)ops");
      } /* End of 'info_editor' function */

      /* Class destructor */
      ~info_editor( VOID ) override
      {
      } /* End of '~info_editor' function */

      /* Render unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Render( anim *Ani ) override
      {
        Ani->DrawCylinder({0, 0, 0}, 0.8, {0, 5, 0}, 0.8, {Color, 1});
        Ani->DrawSphere({2, 0.8, 0}, 0.8, {Color, 1});
        Ani->DrawLine({2, 1, 0}, {2, 2, 0}, {Color, 1});
        Ani->DrawLine({2, 2, 0}, {2, 2, -1}, {Color, 1});
        Ani->DrawLine({2, 2, -1}, {3, 2.2, -1.8}, {Color, 1});
        Ani->Draw(Models[0], matr::Translate({0, YShift, 0}));
      } /* End of 'Render' function */

      /* Response unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
      } /* End of 'Response' function */
    }; /* End of 'info_editor' class */
  } /* end of 'units' namespace */
  static units::editor::unit_register<units::info_editor> _("Info editor unit");
} /* end of 'tam' namespace */

/* END OF 'tam_info_editor_unit.cpp' FILE */
