/*************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : ctrl.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Markers shader group.
 *               Sphere tessellation control shader.
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

// Number of stream points in patch
layout(vertices = 1) out;

layout(location = 0) in int VertInstanceIndex[];
layout(location = 0) out int TessCtrlInstanceIndex[];

/* The main shader function */
void main( void )
{
  int idx = VertInstanceIndex[0];
  TessCtrlInstanceIndex[gl_InvocationID] = idx;
  vec4 CenterR = MrkSphSpheres[idx].MrkSphCenterR;
  float
    d = distance(CamLoc, vec3(MatrW * vec4(CenterR.xyz, 1))) - CenterR.w,
    a = 1, b = 18,
    minn = 5, maxn = 30,
    n = d > b ? minn : d < a ? maxn : (d - a) / (b - a) * (minn - maxn) + maxn;

  n = 30;

  gl_TessLevelOuter[0] = n;
  gl_TessLevelOuter[1] = n;
  gl_TessLevelOuter[2] = n;
  gl_TessLevelOuter[3] = n;
  gl_TessLevelInner[0] = n;
  gl_TessLevelInner[1] = n;
} /* End of 'main' function */

/* END OF 'ctrl.glsl' FILE */
