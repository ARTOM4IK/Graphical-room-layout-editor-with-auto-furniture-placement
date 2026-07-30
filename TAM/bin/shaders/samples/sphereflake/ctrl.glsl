/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : ctrl.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Sphereflake. Tessellation control (adaptive levels by distance).
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 26.02.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

layout(vertices = 3) out;

/* Match sphereflake vert locations 4-5 (commondf uses 0-3 for vert->frag) */
layout(location = 4) in vec3 InPosModel[];
layout(location = 5) in vec3 InNormal[];

layout(location = 4) out vec3 OutPosModel[];
layout(location = 5) out vec3 OutNormal[];

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
  OutPosModel[gl_InvocationID] = InPosModel[gl_InvocationID];
  OutNormal[gl_InvocationID] = InNormal[gl_InvocationID];
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
  if (gl_InvocationID == 0)
  {
    vec3 center = (InPosModel[0] + InPosModel[1] + InPosModel[2]) / 3.0;
    float d = length(center - CamLoc);
    float scale = 4.0 / max(d, 1.0);
    float lev = clamp(4.0 + scale * 12.0, 4.0, 32.0);
    gl_TessLevelOuter[0] = lev;
    gl_TessLevelOuter[1] = lev;
    gl_TessLevelOuter[2] = lev;
    gl_TessLevelInner[0] = lev;
  }
} /* End of 'main' function */

/* END OF 'ctrl.glsl' FILE */
