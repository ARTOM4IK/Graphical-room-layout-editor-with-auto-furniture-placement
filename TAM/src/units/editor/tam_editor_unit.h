/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_editor_unit.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Flat main structures declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3),
 *               Arsentev Artemy (AA4),
 *               Nechaev Vladimir (VN4),
 *               Kupchenkov Ivan (IK4),
 *               Novozhilova Maryana (MN7),
 *               Tretyakov Andrey (AT7),
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 23.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_editor_unit_h_
#define __tam_editor_unit_h_

#include "systems/anim/tam_anim.h"
#include "systems/flat/tam_flat.h"


/* Project namespace */
namespace tam
{
  /* Units namespace */
  namespace units
  {
    /* Point for wall editor class */
    class editor_unit : public unit
    {
    public:
      flat &Flat; // Current edit flat data

      /* Class constructor.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       *   - edit flat reference:
       *       flat &WorkFlat;
       */
      editor_unit( anim *Ani, flat &WorkFlat ) :
        Flat(WorkFlat)
      {
      } /* End of 'editor_unit' function */
    }; /* End of 'editor_unit' class */

    /* UX test unit class */
    class editor : public unit
    {
    private:
      /* Subeditors units/tabs store data type */
      class subeditor
      {
      public:
        ux::tab Tab; // Subeditor tab control
        unit *Unit;  // Subeditor unit
      }; /* End of 'subeditor' class */

      // Subeditors conteiner
      std::vector<subeditor> Subeditors;

      // Main flat
      flat MainFlat;

      // Direction light from camera location to camera dir
      light CameraLight;

      // Current subeditor
      INT ActiveSubeditor = -1;

      // Editor position
      vec3 EditPos;      // Edit coordinates
      BOOL IsEditPos {}; // Editor position access flag

    public:
      // Registered unit names storage
      inline static std::map<std::string, unit * (*)( anim *Ani, flat &WorkFlat, ux::tab &WorkTab, const std::function<VOID ( VOID )> &UserUIFunc )> UnitNames {};

      /* Unit register representation class */
    template<typename unit_type>
      class unit_register
      {
      private:
        /* Create new unit function.
         * ARGUMENTS:
         *   - pointer to animation context:
         *       anim *Ani;
         *   - edit flat reference:
         *       flat &WorkFlat;
         *   - UX tab control reference:
         *       ux::tab &WorkTab;
         *   - UX function reference for user_type:
         *       const std::function<VOID ( VOID )> &UserUIFunc;
         * RETURNS:
         *   (*unit) new unit.
         */
        static unit * Create( anim *Ani, flat &WorkFlat, ux::tab &WorkTab, const std::function<VOID ( VOID )> &UserUIFunc )
        {
          return new unit_type(Ani, WorkFlat, WorkTab, UserUIFunc);
        } /* End of 'Create' function */

      public:
        /* Type constructor.
         * ARGUMENTS:
         *   - unit-to-register name:
         *       const std::string &UnitName;
         */
        unit_register( const std::string &UnitName )
        {
          UnitNames[UnitName] = Create;
        } /* End of 'unit_register' function */
      }; /* End of unit_register class template */

      /* Class constructor.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       */
      editor( anim *Ani );

      /* Class destructor */
      ~editor( VOID ) override
      {
        for (auto &e : Subeditors)
          delete e.Unit;
      } /* End of '~editor' function */

      /* Render unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Render( anim *Ani ) override
      {
        if (INT n = Ani->GetActiveTabIndex();
            n >= 0 && n < Subeditors.size() && Subeditors[n].Unit != nullptr)
          Subeditors[n].Unit->Render(Ani);
        MainFlat.DrawFlat();
        if (Ani->DrawGuidelines)
          Ani->Draw(Ani->Guidelines);
      } /* End of 'Render' function */

      /* Response unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
        CameraLight->Pos = Ani->Cam.Loc;
        CameraLight->Dir = Ani->Cam.Dir;
        CameraLight->Update();
        if (INT n = Ani->GetActiveTabIndex();
            n >= 0 && n < Subeditors.size() && Subeditors[n].Unit != nullptr)
          Subeditors[n].Unit->Response(Ani);
      } /* End of 'Response' function */
    }; /* End of 'editor' class */
  } /* end of 'units' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_editor_unit_h_ */

/* END OF 'tam_editor_unit.h' FILE */
