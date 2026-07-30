/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : vert.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Model (.G3DM) shader.
 *               Vertex shader implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 22.02.2026
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
  DrawPosId = MatrW * vec4(InPosition, 1);
  DrawPosId.w = MatrWInvTrans[3][3];
  DrawColor = InColor;
  DrawNormal = normalize(mat3(MatrWInvTrans) * InNormal);
  DrawTexCoord = InTexCoord;
  ModelId = int(MatrWInvTrans[3][3]);
} /* End of 'main' function */

/* END OF 'vert.glsl' FILE */
