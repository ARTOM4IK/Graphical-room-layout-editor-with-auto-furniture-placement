/*************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : inc.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Markers shader group.
 *               Include data for sphere marker shader.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 03.03.2026.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

/* Sphere data structure */
struct sphere_data
{
  // Sphere data
  vec4 MrkSphCenterR;  // Sphere center position and radius
  vec4 MrkSphColor;    // Sphere color
}; /* End of 'sphere_data' structure */

/* Spheres data buffer */
layout(std430, set = 1, binding = 0) buffer SphereBuffer
{
  sphere_data MrkSphSpheres[];
}; /* End of 'SphereBuffer' buffer */

/* END OF 'inc.glsl' FILE */
