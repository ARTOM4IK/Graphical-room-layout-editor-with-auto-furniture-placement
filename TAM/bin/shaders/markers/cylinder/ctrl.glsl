/*************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : ctrl.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Markers shader group.
 *               Cylinder tessellation control shader.
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
  TessCtrlInstanceIndex[gl_InvocationID] = VertInstanceIndex[0];
  int n1 = 5, n2 = 64;

  gl_TessLevelOuter[0] = n1;
  gl_TessLevelOuter[1] = n2;
  gl_TessLevelOuter[2] = n1;
  gl_TessLevelOuter[3] = n2;

  gl_TessLevelInner[0] = n2;
  gl_TessLevelInner[1] = n1;
} /* End of 'main' function */

/* END OF 'ctrl.glsl' FILE */
