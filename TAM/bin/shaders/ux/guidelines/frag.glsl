/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Guidelines shader.
 *               Fragment shader implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 01.05.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460
#include "commondf.glsl"

#define FrameOffsetX ProjSizeOffsetXY.y
#define FrameOffsetY ProjSizeOffsetXY.z

layout(location = 0) out vec4 OutColor;

float GridScale = 1.0;      // Grid scale
float LineThickness = 2.0;  // Grid lline width
vec3 GridColor = vec3(0.2, .2, 0.2);  // Grid color
vec3 AxisXColor = vec3(0.8, 0.1, 0.1);  // Grid X axis color
vec3 AxisZColor = vec3(0.1, 0.5, 0.8);  // Grid Z axis color

/* Grid color scale evaluate function.
 * ARGUMENTS:
 *   - space position (X-Z plane -> .xy swizzle):
 *       vec2 Pos;
 *   - line thickness:
 *       float Thickness;
 * RETURNS:
 *   (float) collor coefficient.
 */
float Grid( vec2 Pos, float Thickness )
{
  vec2 grid = abs(fract(Pos - 0.5) - 0.5) / fwidth(1 * Pos);
  float line = min(grid.x, grid.y);
  return 1.0 - min(line, Thickness);
} /* End of 'Grid' function */

/* The main fragment shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  float Wp = ProjSize, Hp = ProjSize;
  // Correct aspect ratio
  if (FrameW > FrameH)
    Wp *= FrameW / FrameH;
  else
    Hp *= FrameH / FrameW;

  // Project plane coordinates
  float xp = Wp * (gl_FragCoord.x - FrameOffsetX) / FrameW - Wp / 2;
  float yp = Hp * (gl_FragCoord.y - FrameOffsetY) / FrameH - Hp / 2;
  vec3 Dir = CamDir * ProjDist + CamRight * xp - CamUp * yp;
  Dir = normalize(Dir);
  float ray_t = -CamLoc.y / Dir.y;

  if (ray_t < 0.0)
    discard;  // Frame off
  // World space plane coordinates
  vec3 plane_p = CamLoc + ray_t * Dir;

  OutColor = vec4(plane_p, 1);

  vec2 GridPos = plane_p.xz / GridScale;
  // Main grid
  float g0 = Grid(GridPos * 0.1, LineThickness * 0.8);
  float g1 = Grid(GridPos * 1.0, LineThickness * 0.5);
  float g2 = Grid(GridPos * 10.0, LineThickness);
  float GridFactor = mix(max(g1, g2 * 0.5), max(g0 * 1.8, g1 * 0.8), clamp(ray_t / 100, 0, 1));

  // Axes (thicker and brightly)
  float AxisX = Grid(vec2(GridPos.x, 0.0), LineThickness * 2.0);
  float AxisZ = Grid(vec2(0.0, GridPos.y), LineThickness * 2.0);

  // Depth decay
  float dist = length(plane_p - CamLoc);
  float inter = smoothstep(100, 150, dist);
  float fade = 1.0 - inter * inter;

  vec3 color = GridColor * GridFactor;
  ///  color = mix(color, AxisXColor, AxisX * fade);
  ///  color = mix(color, AxisZColor, AxisZ * fade);

  // Transparncy
  float alpha = fade * GridFactor * 0.98;
  ///color *= fade;

  //if (fade < 0.821)
  //  discard;
  float s = 1;
  //  if (alpha < 0.8 &&
  //      ((int(gl_FragCoord.x * s) ^ int(gl_FragCoord.y * s)) & 1) == 1)
  //    discard;

  // Mouse current position draw
  float
    mxp = (MouseX - FrameOffsetX - FrameW / 2) * Wp / FrameW,
    myp = (-MouseY + FrameOffsetY + FrameH / 2) * Hp / FrameH;
  vec3 Dm = normalize(CamRight * mxp + CamUp * myp + CamDir * ProjDist);
  float ray_tm = -CamLoc.y / Dm.y;
  vec3 ms_p = CamLoc + Dm * ray_tm;
  float mds = distance(plane_p, ms_p);
  float md = clamp(pow(abs(sin(mds * 47 - Time * 18)), 0.08), 0, 1);
  float t = 1 - md;
  t = t * (1 - smoothstep(0, 1, mds));
  color = mix(color, vec3(1), t);
  alpha = mix(alpha, 1, t);
  /*
  float mds = distance(plane_p, ms_p);
  float md = clamp(pow(abs(sin(mds * 47 - Time * 8)), 0.08), 0, 1);
  float t = 1 - md;
  t = t * (1 - smoothstep(0, 1, mds));
  color = mix(color, vec3(8, 18, 0), t);
  alpha = mix(alpha, 1, t);
  */


  //if (alpha < 0.30)
  //  discard;
  //OutColor = vec4(AxisXColor, 1);
  if (abs(plane_p.x) < 0.1)
    color *= vec3(8, 1, 1);
  if (abs(plane_p.z) < 0.1)
    color *= vec3(1, 8, 8);
  OutColor = vec4(color, clamp(alpha, 0, 1));
  vec4 ndc_p = MatrVP * vec4(plane_p + vec3(0, -0.01, 0), 1);
  gl_FragDepth = ndc_p.z / ndc_p.w;
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
