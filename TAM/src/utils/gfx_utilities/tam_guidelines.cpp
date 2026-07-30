/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_guidelines.cpp
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

#include "tam.h"

/* Create guide lines function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
tam::prim tam::guidelines::CreateGuideLines( VOID )
{
  if (Grid)
    return Grid;
  tam::material_pattern MtlPat = RndPtr->MaterialPatternCreate(RndPtr->ShaderCreate("grid"), "MtlPat grid __ &@(#", 0, 0, {}, {}, {tam::vlk::vertex_attribute::eVEC3, tam::vlk::vertex_attribute::eVEC2, tam::vlk::vertex_attribute::eVEC3, tam::vlk::vertex_attribute::eVEC4,}, tam::vlk::topology::POINTS, tam::vlk::blend_mode::eTRANSPARENT);
  tam::material Mtl = RndPtr->MaterialCreate("material grid !@#&$", MtlPat);
  vertex_std V = {vec3(0, 0, 0), vec2(0, 0), vec3(0, 0, -1), vec4(1, 0, 1, 1)};

  return Grid = RndPtr->PrimitiveCreate(Mtl, std::span<vertex_std>(&V, 1), {});
} /* End of 'tam::guidelines::CreateGuideLines' function */

/* END OF 'tam_guidelines.cpp' FILE */
