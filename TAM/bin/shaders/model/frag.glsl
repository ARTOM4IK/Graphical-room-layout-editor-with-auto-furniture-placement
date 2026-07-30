/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Model (.G3DM) shader.
 *               Fragment shader implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 02.04.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

#define FRAGMENT_STD4 1
#include "commondf.glsl"

layout(location = 0) out vec4 OutBaseColor;
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
#define MtlKa Ka.xyz
#define MtlKd KdTrans.xyz
#define MtlKs KsPh.xyz
#define MtlPh KsPh.w
#define MtlTrans KdTrans.w
#define IsTexture0 bool(TextureFlags[0])
#define IsTexture1 bool(TextureFlags[1])
#define IsTexture2 bool(TextureFlags[2])
#define IsTexture3 bool(TextureFlags[3])
#define IsTexture4 bool(TextureFlags[4])
#define IsTexture5 bool(TextureFlags[5])
#define IsTexture6 bool(TextureFlags[6])
#define IsTexture7 bool(TextureFlags[7])

layout(set = 1, binding = 1) uniform sampler2D Texs[5];

/* The main fragment shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  vec4 ResKd, ResKs;
  if (IsTexture0)
    ResKd = texture(Texs[0], DrawTexCoord);
  else
    ResKd = vec4(MtlKd, MtlTrans);
  if (IsTexture1)
    ResKs = texture(Texs[1], DrawTexCoord);
  else
    ResKs = vec4(MtlKs, MtlPh);
  OutBaseColor = vec4(MtlKa, 1);//vec4(ShadePhong(CamLoc, DrawPos, DrawNormal, ResKd.rgb, ResKs.rgb, ResKs.a, normalize(vec3(1)), vec3(1)), ResKd.a);
  //if (KdTrans.a < 1)
  //  OpaqueId[int(gl_FragCoord.x + gl_FragCoord.y * FrameW)] = int(DrawPosId.w);
  //else
  //  TransparentId[int(gl_FragCoord.x + gl_FragCoord.y * FrameW)] = int(DrawPosId.w);
  OutPos = vec4(DrawPosId.xyz, ModelId);
  OutNormal = vec4(DrawNormal.xyz, 1);
  OutMetallRough = vec4(ResKd.rgb, 1);
  // OutColor = texture(Texs[0], DrawTexCoord);
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
