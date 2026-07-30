/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : eval.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Utah teapot. Bezier bicubic evaluation.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 26.02.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

layout(quads, equal_spacing, ccw) in;

/* Match teapot ctrl location 4 */
layout(location = 4) in vec3 InPosModel[];

/* Match commondf.glsl fragment inputs order: 0 DrawColor, 1 DrawNormal, 2 DrawPos, 3 DrawTexCoord */
layout(location = 0) out vec4 DrawColor;
layout(location = 1) out vec3 DrawNormal;
layout(location = 2) out vec3 DrawPos;
layout(location = 3) out vec2 DrawTexCoord;

layout( push_constant ) uniform PrimMatrs
{
  mat4 MatrWVP;
  mat4 MatrW;
  mat4 MatrWInvTrans;
};

/* Bezier basis B_i(t) = C(3,i) * t^i * (1-t)^(3-i) evaluation function.
 * ARGUMENTS:
 *   - parameter:
 *       float t;
 * RETURNS:
 *   (vec4) basis vector.
 */
vec4 BezierBasis(float t)
{
  float u = 1.0 - t;
  float u2 = u * u, u3 = u2 * u;
  float t2 = t * t, t3 = t2 * t;
  return vec4(u3, 3.0*u2*t, 3.0*u*t2, t3);
} /* End of 'BezierBasis' function */

/* Main tessellation evaluation shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  float u = gl_TessCoord.x;
  float v = gl_TessCoord.y;
  vec4 Bu = BezierBasis(u);
  vec4 Bv = BezierBasis(v);
  /* Control points: patch[i][j] = gl_in[i + j*4].gl_Position.xyz */
  vec3 P = vec3(0.0);
  for (int j = 0; j < 4; j++)
    for (int i = 0; i < 4; i++)
      P += Bu[i] * Bv[j] * InPosModel[i + j*4];
  /* Tangent for normal: d/du and d/dv of Bezier */
  vec4 dBu = vec4(-3.0*(1.0-u)*(1.0-u), 3.0*(1.0-u)*(1.0-u) - 6.0*u*(1.0-u), 6.0*u*(1.0-u) - 3.0*u*u, 3.0*u*u);
  vec4 dBv = vec4(-3.0*(1.0-v)*(1.0-v), 3.0*(1.0-v)*(1.0-v) - 6.0*v*(1.0-v), 6.0*v*(1.0-v) - 3.0*v*v, 3.0*v*v);
  vec3 dPdu = vec3(0.0), dPdv = vec3(0.0);
  for (int j = 0; j < 4; j++)
    for (int i = 0; i < 4; i++)
    {
      dPdu += dBu[i] * Bv[j] * InPosModel[i + j*4];
      dPdv += Bu[i] * dBv[j] * InPosModel[i + j*4];
    }
  vec3 N = normalize(cross(dPdu, dPdv));
  vec4 worldPos = vec4(P, 1.0);
  gl_Position = MatrWVP * worldPos;
  DrawColor = vec4(1.0);
  DrawNormal = normalize(mat3(MatrWInvTrans) * N);
  DrawPos = (MatrW * worldPos).xyz;
  DrawTexCoord = vec2(u, v);
} /* End of 'main' function */

/* END OF 'eval.glsl' FILE */
