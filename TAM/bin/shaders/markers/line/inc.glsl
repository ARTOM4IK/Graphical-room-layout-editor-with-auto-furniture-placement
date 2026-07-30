/*************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : inc.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Markers shader group.
 *               Include data for line marker shader.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 24.03.2026.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

/* Line data structure */
struct line_data
{
  // Line data
  vec4 MrkLinePts[2];   // Line endpoints
  vec4 MrkLineColor;    // Line color
}; /* End of 'line_data' structure */

/* Spheres data buffer */
layout(std430, set = 1, binding = 0) buffer LineBuffer
{
  line_data MrkLineLines[];
}; /* End of 'LineBuffer' buffer */

/* Push constants data transfer structure for cylinders */
//layout(std140, push_constant) uniform cylinder_marker
//{
//  // Matrixes
//  mat4 MatrWVP;  // World-View-Project
//  mat4 MatrW;    // World
//  mat4 MatrWInv; // World inverted and transposed
//}; /* End of 'cylinder_marker' structure */

/* END OF 'inc.glsl' FILE */

