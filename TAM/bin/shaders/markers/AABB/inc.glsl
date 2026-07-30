/*************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : inc.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Markers shader group.
 *               Include data for AABB marker shader.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 02.04.2026.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

/* AABB data structure */
struct AABB_data
{
  // AABB data
  vec4 MrkAABBP0;      // First corner point
  vec4 MrkAABBP1;      // Second corner point
  vec4 MrkAABBColor;   // AABB color
}; /* End of 'AABB_data' structure */

/* Spheres data buffer */
layout(std430, set = 1, binding = 0) buffer AABBBuffer
{
  AABB_data MrkAABBs[];
}; /* End of 'AABBBuffer' buffer */

/* END OF 'inc.glsl' FILE */
