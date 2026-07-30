/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Merge shader.
 *               Fragment shader implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 19.04.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

#include "commondf.glsl"

layout(set = 1, binding = 0) uniform sampler2D InColor1;

layout(location = 0) out vec4 OutColor;

void main( void )
{
  //OutColor = vec4(1, 1, 0, 1);
  vec2 TextureSize = vec2(textureSize(InColor1, 0));
  OutColor = vec4(texture(InColor1, gl_FragCoord.xy / TextureSize));
  //OutColor = vec4(texture(InColor1, gl_FragCoord.xy / vec2(FrameW + FrameOffsetX, FrameH + FrameOffsetY)));
}

/* END OF 'frag.glsl' FILE */
