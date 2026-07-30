/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : vert.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Render test shader.
 *               Vertex shader implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 07.02.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

#define VERTEX_STD4 1
#include "commondf.glsl"

/* Main vertex shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{                   
  gl_Position = vec4(InPosition, 1.0);
  DrawColor = vec4(1, 0, 0, 1);
  //DrawPos = gl_Position.xyz;
  //DrawColor = InColor;
  //DrawNormal = normalize(mat3(MatrWInvTrans) * InNormal);
  //DrawNormal = normalize(InNormal);
  //DrawTexCoord = InTexCoord;
} /* End of 'main' function */

/* END OF 'vert.glsl' FILE */
