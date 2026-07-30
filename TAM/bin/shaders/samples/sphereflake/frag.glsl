/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Sphereflake. Chrome material, directional light (1,1,1).
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

/* DrawPos, DrawNormal, DrawTexCoord, DrawColor come from commondf (locations 0-3) */

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
  vec3 L = normalize(LightDir.xyz);
  vec3 V = normalize(CamPos.xyz - DrawPos);
  vec3 H = normalize(L + V);
  float ndl = max(0.0, dot(N, L));
  float ndh = max(0.0, dot(N, H));
  float ph = Ph.x;
  /* Chrome: visible silver base + strong specular highlight */
  vec3 spec = pow(ndh, ph) * Ks.rgb * LightColor.rgb;
  vec3 diff = ndl * Kd.rgb * LightColor.rgb;
  vec3 col = Ka.rgb + diff + spec;
  /* Fresnel rim: edges catch more reflection */
  float fresnel = pow(1.0 - max(0.0, dot(N, V)), 2.0);
  col += fresnel * 0.5 * (Ks.rgb + 0.4);
  /* Never pure black - keep chrome visible in shadow */
  col = max(col, vec3(0.12, 0.12, 0.15));
  OutColor = vec4(col, 1.0);
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
