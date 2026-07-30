/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : vert.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Default PBR shader.
 *               Vertex shader implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 22.04.2026
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
  gl_Position = MatrWVP * vec4(InPosition, 1.0);
  DrawPosId = vec4((MatrW * vec4(InPosition, 1.0)).xyz, MatrWInvTrans[3][3]);
  DrawColor = InColor;
  DrawNormal = normalize(mat3(MatrWInvTrans) * InNormal);
  DrawTexCoord = InTexCoord;
} /* End of 'main' function */

/* END OF 'vert.glsl' FILE */
