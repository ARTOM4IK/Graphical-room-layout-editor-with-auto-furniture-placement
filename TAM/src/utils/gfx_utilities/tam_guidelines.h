/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_guidelines.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               guidelines declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 12.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_guidelines_h_
#define __tam_guidelines_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* Forward declaration of render */
  class render;
  /* Forward declaraiton of 'material_pattern_res' class for 'material_pattern' type usage */
  class material_pattern_res;
  /* Forward declaraiton of 'material_res' class for 'material' type usage */
  class material_res;
  /* Define material pattern interface type */
  using material_pattern = std::shared_ptr<material_pattern_res>;
  /* Define material interface type */
  using material = std::shared_ptr<material_res>;
  /* Forward declaraiton of 'prim_res' class for 'prim' type usage */
  class prim_res;
  /* Define primitive interface type */
  using prim = std::shared_ptr<prim_res>;

  /* 'guidelines' handle class */
  class guidelines
  {
  private:
    render *RndPtr;
    prim Grid;

  public:
    /* Guide lines constructor.
     * ARGUMENTS:
     *   - pointer to render:
     *       render *RndPtr;
     */
    tam::guidelines( render *Rnd ) : RndPtr(Rnd)
    {
    } /* End of 'guidelines' function */

    /* Create guide lines function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    prim CreateGuideLines( VOID );

  }; /* End of 'guidelines' class */
} /* end of 'tam' namespace */

#endif /* __tam_guidelines_h_ */

/* END OF 'tam_guidelines.h' FILE */
