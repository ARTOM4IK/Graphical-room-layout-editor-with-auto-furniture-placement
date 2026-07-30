/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Default shader.
 *               Fragment shader implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 18.04.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

#define FRAGMENT_STD4 1
#include "commondf.glsl"

layout(location = 0) out vec4 OutColor;
layout(location = 1) out vec4 OutPos;
layout(location = 2) out vec4 OutNormal;
layout(location = 3) out vec4 OutMetallRough;

layout(set = 1, binding = 0) buffer MaterialBuffer
{
  vec4 Ka;
  vec4 KdTrans;
  vec4 KsPh;
  int TextureFlags[8];
};
#define IsTexture0 bool(TextureFlags[0])

layout(set = 1, binding = 1) uniform sampler2D Texs[1];

/* The main fragment shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  vec3 V = normalize(DrawPosId.xyz - CamLoc);
  vec3 N = DrawNormal;
  N = faceforward(N, V, N);
  // OutColor = vec4(normalize(N), 1);
  OutPos = vec4(DrawPosId.xyz, 0);
  vec4 ResKd;
  if (IsTexture0)
    ResKd = texture(Texs[0], DrawTexCoord * Ka.a);
  else
    ResKd = KdTrans;
  //if (KdTrans.a < 1)
  //  OpaqueId[int(gl_FragCoord.x + gl_FragCoord.y * FrameW)] = int(DrawPosId.w);
  //else
  //  TransparentId[int(gl_FragCoord.x + gl_FragCoord.y * FrameW)] = int(DrawPosId.w);
  OutPos = vec4(DrawPosId.xyz, ModelId);

  OutNormal = vec4(N * 0.5 + 0.5, 0);
  if (KdTrans.a < 1)
    OpaqueId[int(gl_FragCoord.x + gl_FragCoord.y * FrameW)] = int(DrawPosId.w);
  else
    TransparentId[int(gl_FragCoord.x + gl_FragCoord.y * FrameW)] = int(DrawPosId.w);
  OutColor = vec4(ShadePhong(CamLoc, DrawPosId.xyz, N, ResKd.xyz, KsPh.xyz, KsPh.a, -CamDir, vec3(1)), ResKd.a);
  // OutColor = vec4(DrawColor.rgb * nl, 1);;
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
