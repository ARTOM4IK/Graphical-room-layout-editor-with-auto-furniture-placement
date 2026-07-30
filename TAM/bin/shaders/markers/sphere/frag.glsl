/*************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : ctrl.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Markers shader group.
 *               Sphere fragment shader.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 03.03.2026.
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

// Input data from tessellation
layout(location = 0) in ES_IN
{
  vec3 DrawNormal; // Interpolated normal
  vec4 DrawColor;  // Interpolated color
} es_in;

/* The main shader function */
void main( void )
{
  float nl = max(0.2, dot(normalize(es_in.DrawNormal), normalize(-vec3(CamDir))));
  OutColor = vec4(es_in.DrawColor.rgb * nl, 1);
  OutNormal = vec4(0, 0, 0, 0);
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */

