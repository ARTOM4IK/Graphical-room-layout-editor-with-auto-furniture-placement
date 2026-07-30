/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Font direct 3D/billboard drawing shader.
 *               Fragment shader implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 19.02.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

#define FRAGMENT_STD4 1
#include "commondf.glsl"

layout(location = 0) out vec4 OutColor;

layout(set = 1, binding = 0) uniform sampler2D FontTexture;


/* Main fragment shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  vec4 TexColor = texture(FontTexture, DrawTexCoord);
  if (TexColor.a < 0.30)
    discard;
  OutColor = DrawColor * TexColor;
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
