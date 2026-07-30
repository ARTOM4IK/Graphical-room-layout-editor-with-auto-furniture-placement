/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : vert.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Font screen space drawing shader.
 *               Vertex shader implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 21.02.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

#define VERTEX_STD4 1
#include "commondf.glsl"

/* Orthographics projection matrix setup function.
 * ARGUMENTS:
 *   - orthographic box side facets coordinates:
 *       FLT Left, Right, Bottom, Top, Near, Far;
 * RETURNS:
 *   (mat4) result matrix.
 */
mat4 MatrOrtho( float Left, float Right, float Bottom, float Top, float Near, float Far )
{
  return
    mat4(vec4(2 / (Right - Left), 0, 0, 0),
         vec4(0, 2 / (Top - Bottom), 0, 0),
         vec4(0, 0, -2 / (Far - Near), 0),
         vec4(-(Right + Left) / (Right - Left), -(Top + Bottom) / (Top - Bottom), -(Far + Near) / (Far - Near), 1));
} /* End of 'MatrOrtho' function */

/* Main vertex shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{                   
  gl_Position = MatrOrtho(0, FrameW, -FrameH, 1, -100, 100) * MatrW * vec4(InPosition, 1);
  //gl_Position = vec4(InPosition, 1);
  //gl_Position = MatrWVP * vec4(InPosition, 1.0);
  DrawTexCoord = InTexCoord;
  DrawColor = InColor;
} /* End of 'main' function */

/* END OF 'vert.glsl' FILE */
