/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : eval.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Sphereflake. Tessellation evaluation (triangle to sphere).
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 26.02.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

layout(triangles, equal_spacing, ccw) in;

/* Match sphereflake ctrl locations 4-5 */
layout(location = 4) in vec3 InPosModel[];
layout(location = 5) in vec3 InNormal[];

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

/* Main tessellation evaluation shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  vec3 p = gl_TessCoord.x * InPosModel[0] + gl_TessCoord.y * InPosModel[1] + gl_TessCoord.z * InPosModel[2];
  p = normalize(p);
  vec3 n = p;
  vec4 worldPos = vec4(p, 1.0);
  gl_Position = MatrWVP * worldPos;
  DrawColor = vec4(1.0);
  DrawNormal = normalize(mat3(MatrWInvTrans) * n);
  DrawPos = (MatrW * worldPos).xyz;
  DrawTexCoord = vec2(0.0);
} /* End of 'main' function */

/* END OF 'eval.glsl' FILE */
