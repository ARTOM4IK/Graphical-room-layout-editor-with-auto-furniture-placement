/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               AT7 sample shader.
 *               Fragment shader implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 21.02.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

#define FRAGMENT_STD4 1
#include "commondf.glsl"

layout(location = 0) out vec4 OutColor;

layout(set = 1, binding = 0) uniform sampler2D TestTex;

/* The main fragment shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  OutColor = texture(TestTex, DrawTexCoord);
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
