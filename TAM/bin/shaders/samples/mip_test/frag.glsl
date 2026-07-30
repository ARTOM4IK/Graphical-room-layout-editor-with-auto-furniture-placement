/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Mip-map visual test shader.
 *               Fragment shader: samples one mip level via textureLod.
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

layout(set = 1, binding = 0) buffer MipLevelBuf
{
  float MipLevel; // Mip-map level
};

// Used texture
layout(set = 1, binding = 1) uniform sampler2D Tex;

/* Main fragment shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  OutColor = textureLod(Tex, DrawTexCoord, MipLevel);
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
