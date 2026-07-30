/*************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : geom.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Markers shader group.
 *               AABB geometry shader.
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

// Geometry parameters
layout(points) in;
layout(line_strip, max_vertices = 30) out;

layout(location = 0) in int VertInstanceIndex[];

// Output data for fragment shader
layout(location = 0) out GS_OUT
{
  vec4 DrawColor;  // Vertex color
} gs_out;

/* The main shader function */
void main( void )
{
  int idx = VertInstanceIndex[0];
  vec3
    P0 = MrkAABBs[idx].MrkAABBP0.xyz,
    P1 = MrkAABBs[idx].MrkAABBP1.xyz;

  vec3 p[8] =
  {
    vec3(-1, -1, -1),
    vec3(-1, -1,  1),
    vec3( 1, -1,  1),
    vec3( 1, -1, -1),

    vec3(-1, 1, -1),
    vec3(-1, 1,  1),
    vec3( 1, 1,  1),
    vec3( 1, 1, -1),
  };
  for (int i = 0; i < p.length(); i++)
  {
    for (int k = 0; k < 3; k++)
      p[i][k] = mix(P0[k], P1[k], (1 + p[i][k]) / 2);
  }
  int e[] =
  {
    0, 1,
    1, 2,
    2, 3,
    3, 0,

    4, 5,
    5, 6,
    6, 7,
    7, 4,

    0, 4,
    1, 5,
    2, 6,
    3, 7,
  };
  for (int i = 0; i < e.length(); i++)
  {
    gs_out.DrawColor = MrkAABBs[idx].MrkAABBColor;
    gl_Position = MatrVP * vec4(p[e[i]], 1);
    EmitVertex();
    i++;
    gs_out.DrawColor = MrkAABBs[idx].MrkAABBColor;
    gl_Position = MatrVP * vec4(p[e[i]], 1);
    EmitVertex();
    EndPrimitive();
  }
} /* End of 'main' function */

/* END OF 'geom.glsl' FILE */
