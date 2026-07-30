/*************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : inc.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Markers shader group.
 *               Include data for cylinder marker shader.
 * PROGRAMMER  : CGSG'Jr'2024.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 03.03.2026.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

/* Cylinder data structure */
struct cylinder_data
{
  // Cylinder data
  vec4 MrkCylPR0;      // First base center point and radius
  vec4 MrkCylPR1;      // Second base center point and radius
  vec4 MrkCylColor;    // Cylinder color
}; /* End of 'cylinder_data' structure */

/* Spheres data buffer */
layout(std430, set = 1, binding = 0) buffer CylinderBuffer
{
  cylinder_data MrkCylCylinders[];
}; /* End of 'SphereBuffer' buffer */

/* Push constants data transfer structure for cylinders */
//layout(std140, push_constant) uniform cylinder_marker
//{
//  // Matrixes
//  mat4 MatrWVP;  // World-View-Project
//  mat4 MatrW;    // World
//  mat4 MatrWInv; // World inverted and transposed
//}; /* End of 'cylinder_marker' structure */

/* END OF 'inc.glsl' FILE */

