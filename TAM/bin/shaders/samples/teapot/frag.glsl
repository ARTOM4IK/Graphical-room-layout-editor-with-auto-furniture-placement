/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Utah teapot. Gold material, directional light (1,1,1).
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 26.02.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460
#define FRAGMENT_STD4 1
#include "commondf.glsl"

layout(location = 0) out vec4 OutColor;

// Light soure material pass data
layout(set = 1, binding = 0) buffer LightMtl
{
  vec4 LightDir;       // Light source direction
  vec4 LightColor;     // Light source color
  vec4 CamPos;         // Camera position
  vec4 Ka, Kd, Ks, Ph; // Phong illumination coefficients
};

/* Main fragment shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  vec3 N = normalize(DrawNormal);
  vec3 L = normalize(vec3(1, -1, 1));//normalize(LightDir.xyz);
  vec3 V = -normalize(CamPos.xyz - DrawPos);
  vec3 H = normalize(L + V);
  float ndl = max(0.0, dot(N, L));
  float ndh = max(0.0, dot(N, H));
  float ph = Ph.x;
  vec3 col = Ka.rgb + ndl * Kd.rgb * LightColor.rgb + pow(ndh, ph) * Ks.rgb * LightColor.rgb;
  OutColor = vec4(col, 1.0);
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
