/*************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : vert.glsl
 * PURPOSE     : Animation pattern on Vulkan API project.
 *               Markers shader group.
 *               AABB vertex shader.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 02.04.2026.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

layout(location = 0) out int VertInstanceIndex;

/* The main shader function */
void main( void )
{
  VertInstanceIndex = gl_InstanceIndex;
  gl_Position = vec4(0, 0, 0, 1);
} /* End of 'main' function */

/* END OF 'vert.glsl' FILE */
