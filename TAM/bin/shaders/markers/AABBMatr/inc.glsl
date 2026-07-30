/*************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : inc.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Markers shader group.
 *               Include data for AABBMatr marker shader.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 14.04.2026.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

/* AABB data structure */
struct AABBMatr_data
{
  // AABB data
  vec4 MrkAABBMatrP0;        // First corner point
  vec4 MrkAABBMatrP1;        // Second corner point
  vec4 MrkAABBMatrColor;     // AABB color
  mat4 MrkAABBMatrTransform; // AABB transofrm matrix
}; /* End of 'AABBMatr_data' structure */

/* Spheres data buffer */
layout(std430, set = 1, binding = 0) buffer AABBMatrBuffer
{
  AABBMatr_data MrkAABBMatrs[];
}; /* End of 'AABBBuffer' buffer */

/* END OF 'inc.glsl' FILE */
