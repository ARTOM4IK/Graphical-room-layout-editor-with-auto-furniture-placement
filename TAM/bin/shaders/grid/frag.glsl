/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Grid shader.
 *               Fragment shader implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 12.03.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460
#include "commondf.glsl"

layout(location = 0) out vec4 OutColor;

/* The main fragment shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  float Wp = ProjSize, Hp = ProjSize;
  float width = 0.05;
  /* Correct aspect ratio */
  if (FrameW > FrameH)
    Wp *= FrameW / FrameH;
  else
    Hp *= FrameH / FrameW;
 
  float xp = Wp * gl_FragCoord.x / FrameW - Wp / 2;
  float yp = Hp * gl_FragCoord.y / FrameH - Hp / 2;
  vec3 Dir = CamDir * ProjDist + CamRight * xp - CamUp * yp;
  Dir = normalize(Dir);
  float t = -CamLoc.y / Dir.y;
  float t1 = (log(abs(10.0 * CamLoc.y)) / log(10)) - 1.0;
  vec3 P = CamLoc + Dir * t;
  if (t < 0)
    discard;

  int Levels = abs(int(t1));
  if (Levels <= 0)
    Levels = 1;

  float X = mod(P.x, 1.0 / Levels);
  float Z = mod(P.z, 1.0 / Levels);

  width *= t1;
  if (width < 0.007)
    width = 0.007;
  if (!(X <= width || Z <= width))
    discard;
  vec3 color = vec3(0.7, 0.7, 0.7);
  OutColor = vec4(color, clamp(0, 1, 1 - t / (30.0 * 4)));
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
