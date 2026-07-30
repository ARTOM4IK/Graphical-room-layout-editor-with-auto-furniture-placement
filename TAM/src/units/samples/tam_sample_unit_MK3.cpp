/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_sample_unit_MK3.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               MK3 sample unit implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 19.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* 'tam' namespace */
namespace tam
{
  /* Anonymous namespace */
  namespace
  {
    /* MK3 unit sample representation class */
    class unit_sample : public unit
    {
    private:
      INT NumOfModels {}; // Test primitive
      model Models[10] {}; // Test model
      matr M;
      ux::tab TabEditRoom;
    public:

      /* Class constructor.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       */
      unit_sample( anim *Ani ) : TabEditRoom(Ani, "Room Placing"), M(matr::Identity())
      {
        /*Models[3] = Ani->ModelCreate("bin/models/G3DM/bed.g3dm");
        Models[1] = Ani->ModelCreate("bin/models/G3DM/fridge.g3dm");
        Models[2] = Ani->ModelCreate("bin/models/G3DM/table.g3dm");*/
        Models[0] = Ani->ModelCreate("bin/models/G3DM/table2.g3dm");
        //NumOfModels = 4;
        NumOfModels = 1;
      } /* End of 'unit_sample' function */

      /* Get response from unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
        if ((Ani->Keys[VK_SHIFT] && Ani->KeysClick['S']))
          M = M * matr::Translate(vec3(1, 0, 0));
        else if ((Ani->Keys[VK_SHIFT] && Ani->KeysClick['W']))
          M = M * matr::Translate(vec3(-1, 0, 0));
        else if ((Ani->Keys[VK_SHIFT] && Ani->KeysClick['A']))
          M = M * matr::Translate(vec3(0, 0, 1));
        else if ((Ani->Keys[VK_SHIFT] && Ani->KeysClick['D']))
          M = M * matr::Translate(vec3(0, 0, -1));
        if (Ani->Keys[MK_LBUTTON])
        {
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
        for (INT i = 0; i < NumOfModels; i++)
          Ani->Draw(Models[i], M);
      } /* End of 'Render' function */

      /* Test unit destructor */
      ~unit_sample( VOID )
      {
        for (INT i = 0; i < NumOfModels; i++)
          Models[i]->Free();
      } /* End of type destructor */
    };
    static tam::anim::unit_register<unit_sample> _("Garniture setting test unit");
  } /* end of anonymous namespace */
} /* end of 'tam' namespace */

/* END OF 'tam_sample_unit_MK3.cpp' FILE */
