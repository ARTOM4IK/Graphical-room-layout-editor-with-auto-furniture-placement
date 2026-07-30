/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_teapot_data.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Utah teapot: 306 vertices, 32 patches (16 indices each, 1-based).
 *               From Angel, Interactive Computer Graphics (teapot.c).
 * PROGRAMMER  : CGSG'Sr'2025.
 * LAST UPDATE : 26.02.2026
 * NOTE        : Namespace 'tam::teapot_data'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_teapot_data_h_
#define __tam_teapot_data_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* Utah teapot data namespace */
  namespace teapot_data
  {
    constexpr INT NumVertices = 306;
    constexpr INT NumPatches = 32;
    constexpr INT IndicesPerPatch = 16;

    /* 306 vertices, xyz (from teapot.c) */
    extern const vec3 Vertices[306];
    /* 32 patches, 4x4 indices (1-based) */
    extern const INT Indices[32][4][4];
  } /* end of 'teapot_data' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_teapot_data_h_ */

/* END OF 'tam_teapot_data.h' FILE */
