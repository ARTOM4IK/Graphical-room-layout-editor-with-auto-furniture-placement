/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               PBR shader.
 *               Fragment shader implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 16.04.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

#define FRAGMENT_STD7 1
#include "commondf.glsl"

layout(set = 2, binding = 0) uniform sampler2D Texs[5];

layout(location = 0) out vec4 OutBaseColor;
layout(location = 1) out vec4 OutPos;
layout(location = 2) out vec4 OutNormal;
layout(location = 3) out vec4 OutMetallRough;

/* The main fragment shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  vec3 V = normalize(CamLoc - DrawPosId.xyz);
  vec3 normalMap = texture(Texs[2], DrawTexCoord).rgb * 2.0 - 1.0;
  vec3 N;
  vec4 Factors_tan = texture(Texs[4], DrawTexCoord).rgba;
  if (Factors_tan.a == 1)
  {
    vec3 T = normalize(DrawTangent.xyz);
    N = normalize(DrawNormal);
    vec3 B = normalize(cross(N, T)) * DrawTangent.w;

    N = faceforward(N, -V, N);
 
    mat3 TBN = mat3(T, B, N);
    N = normalize(TBN * normalMap);
//    N = normalize(normalMap);
  }
  else
    N = vec3(DrawNormal);
    //N = normalize(DrawNormal * 2.0 - 1.0);
    // N = normalize(DrawNormal);

  OutBaseColor = texture(Texs[0], DrawTexCoord).rgba;// * DrawColor.rgb;
  //if (OutBaseColor.a < 1)
  //  OpaqueId[int(gl_FragCoord.x + gl_FragCoord.y * FrameW)] = int(DrawPosId.w);
  //else
  //  TransparentId[int(gl_FragCoord.x + gl_FragCoord.y * FrameW)] = int(DrawPosId.w);
  OutPos = vec4(DrawPosId.xyz, ModelId);
  OutNormal = vec4(N, 1);
  OutMetallRough = texture(Texs[1], DrawTexCoord).rgba;
  OutMetallRough.b *= Factors_tan.x;
  OutMetallRough.g *= Factors_tan.y;
  //if (OutBaseColor.a < -0.30)
  //  discard;
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
