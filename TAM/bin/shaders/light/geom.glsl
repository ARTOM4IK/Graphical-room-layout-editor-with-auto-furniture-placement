/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : geom.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Grid shader.
 *               Geometry shader implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 12.03.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;
 
/* The main program function */
void main( void )
{
  gl_Position = vec4(-1, 3, 0, 1);
  EmitVertex();
  gl_Position = vec4(-1, -1, 0, 1);
  EmitVertex();
  gl_Position = vec4(3, -1, 0, 1);
  EmitVertex();
  EndPrimitive();
} /* End of 'main' function */
 
/* END OF 'vert.glsl' FILE */