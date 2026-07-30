/*************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : vert.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Markers shader group.
 *               Line vertex shader.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 24.03.2026.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460
#include <commondf.glsl>
#include "inc.glsl"

// Output data for fragment shader
layout(location = 0) out VS_OUT
{
  vec4 DrawColor;  // Vertex color
} vs_out;

/* The main shader function */
void main( void )
{
  vs_out.DrawColor = MrkLineLines[gl_InstanceIndex].MrkLineColor;
  gl_Position = MatrVP * vec4(MrkLineLines[gl_InstanceIndex].MrkLinePts[gl_VertexIndex].xyz, 1);
} /* End of 'main' function */

/* END OF 'vert.glsl' FILE */
