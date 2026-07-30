/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ctrl.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Utah teapot. Tessellation control (adaptive quad patch).
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 26.02.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

layout(vertices = 16) out;

/* Match teapot vert location 4 (commondf uses 0-3 for vert->frag) */
layout(location = 4) in vec3 InPosModel[];
layout(location = 4) out vec3 OutPosModel[];

layout(set = 0, binding = 1) buffer Camera
{
  mat4 MatrV;
  mat4 MatrP;
  mat4 MatrVP;
  vec4 CamLocFrameW;
  vec4 CamDirFrameH;
  vec4 CamRightWp;
  vec4 CamUpHp;
  vec4 CamAtProjDist;
  vec4 ProjSize;
};

#define CamLoc CamLocFrameW.xyz

/* Main tessellation control shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  if (gl_InvocationID == 0)
  {
    vec3 center = vec3(0);
    for (int i = 0; i < 16; i++)
      center += gl_in[i].gl_Position.xyz;
    center /= 16.0;
    float d = length(center - CamLoc);
    float lev = clamp(4.0 + 8.0 / max(d, 1.0), 4.0, 24.0);
    gl_TessLevelOuter[0] = lev;
    gl_TessLevelOuter[1] = lev;
    gl_TessLevelOuter[2] = lev;
    gl_TessLevelOuter[3] = lev;
    gl_TessLevelInner[0] = lev;
    gl_TessLevelInner[1] = lev;
  }
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
  OutPosModel[gl_InvocationID] = InPosModel[gl_InvocationID];
} /* End of 'main' function */

/* END OF 'ctrl.glsl' FILE */
