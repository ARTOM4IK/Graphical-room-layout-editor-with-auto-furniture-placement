/*************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : ctrl.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Markers shader group.
 *               AABB fragment shader.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 02.04.2026.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

#include <commondf.glsl>
#include "inc.glsl"

// Output color attribute
layout(location = 0) out vec4 OutColor;
layout(location = 2) out vec4 OutNormal;

// Input data from geometry
layout(location = 0) in GS_IN
{
  vec4 DrawColor;  // Interpolated color
} gs_in;

/* The main shader function */
void main( void )
{
  OutColor = vec4(gs_in.DrawColor.rgb, 1);
  OutNormal = vec4(0, 0, 0, 0);
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */

