/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_sample_unit_AT7.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Flat test unit implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 17.03.2026
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
    /* Flat test representation class */
    class flat_test : public unit
    {
    public:

      /* Class constructor.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       */
      flat_test( anim *Ani )
      {
      } /* End of 'unit_sample' function */

      /* Get response from unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
      } /* End of 'Response' function */

      /* Render unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Render( anim *Ani ) override
      {
      } /* End of 'Render' function */

      /* Test unit destructor */
      ~flat_test( VOID )
      {
        logger::info << "Flat unit deinitialization.";
      } /* End of type destructor */
    };
    static tam::anim::unit_register<flat_test> _("Flat test unit");
  } /* end of anonymous namespace */
} /* end of 'tam' namespace */

/* END OF 'tam_sample_unit_AT7.cpp' FILE */
