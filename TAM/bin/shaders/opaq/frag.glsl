/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Merge shader.
 *               Fragment shader implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 17.03.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

layout(input_attachment_index = 0, set = 1, binding = 0) uniform subpassInput InColorOpaq;
layout(input_attachment_index = 1, set = 1, binding = 1) uniform subpassInput InColorTran;

layout(location = 0) out vec4 OutBaseColor;
layout(location = 1) out vec4 OutPos;
layout(location = 2) out vec4 OutNormal;
layout(location = 3) out vec4 OutMetallRough;

void main( void )
{
  OutBaseColor = vec4(subpassLoad(InColorOpaq).xyz, 1);
  
//  OutColor = vec4(gl_FragCoord.www * 10000, 1);
}

/* END OF 'frag.glsl' FILE */
