/*************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : ctrl.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Markers shader group.
 *               Sphere tessellation evaluation shader.
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

// Tessellation parameters
layout(quads, fractional_even_spacing) in;

layout(location = 0) in int TessCtrlInstanceIndex[];

// Output data for fragment shader
layout(location = 0) out ES_OUT
{
  vec3 DrawNormal; // Vertex normal
  vec4 DrawColor;  // Vertex color
} es_out;

/* The main shader function */
void main( void )
{
  int idx = TessCtrlInstanceIndex[0];
  vec4 CenterR = MrkSphSpheres[idx].MrkSphCenterR;
  float
    t = gl_TessCoord.y * acos(-1),
    p = gl_TessCoord.x * 2 * acos(-1);
  vec3
    pnt =
      vec3(sin(t) * sin(p),
           cos(t),
           sin(t) * cos(p));

  //pnt = vec3(gl_TessCoord.xy, 0);
  gl_Position = MatrVP * vec4(CenterR.xyz + pnt * CenterR.w, 1);
  //gl_Position = vec4(pnt, 1);
  //gl_Position = MatrWVP * vec4(pnt, 1);
  es_out.DrawNormal = /* mat3(MatrWInvTrans) * */ normalize(pnt / CenterR.w);
  es_out.DrawColor = MrkSphSpheres[idx].MrkSphColor;
  //es_out.DrawNormal = vec3(1, 1, 1);
} /* End of 'main' function */

/* END OF 'eval.glsl' FILE */
