/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : vert.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Utah teapot. Vertex shader (pass-through control points).
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 26.02.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460
#define VERTEX_STD4 1
#include "commondf.glsl"

/* Tessellation pass-through: use location 4 (0-3 are used by commondf vertex outs) */
layout(location = 4) out vec3 OutPosModel;

/* Main vertex shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  OutPosModel = InPosition;
  gl_Position = MatrWVP * vec4(InPosition, 1.0);
} /* End of 'main' function */

/* END OF 'vert.glsl' FILE */
