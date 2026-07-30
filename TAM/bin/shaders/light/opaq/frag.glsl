/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Default shader.
 *               Fragment shader implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 01.05.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

#include "commondf.glsl"
#include "lights_def.glsl"

/* The main fragment shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  vec3 color = vec3(0, 0, 0);
  
  float id = subpassLoad(InPos).w;
  int Id = int(id);
  if (id - int(id) > 0.5)
    Id += 1;
  // Write model id to buffer
  atomicExchange(OpaqueId[int(int(gl_FragCoord.x) + int(gl_FragCoord.y) * (FrameW + FrameOffsetX))], Id);
  if (!bool(subpassLoad(InNormal).w))
  {
    if (subpassLoad(InDepth).x < 1)
      OutColor = vec4(subpassLoad(InBaseColor).xyz, 0);
    else
      OutColor = vec4(Skybox(), 0);
    return;
  }
  // Presentation lighting mode hanlde
  if (abs(SizeDayTimeDayOfYearPresentFlag.w - 1) < 0.1) // IsPresentLighting = 1
  {
     float t = 0.5;
     color += subpassLoad(InBaseColor).rgb * t + PresentLighting() * (1 - t);
  }
  // Default lighting handle
  else
  {
    color = ComputeSun();
    int i = 0;
    while (i < int(SizeDayTimeDayOfYearPresentFlag.x + 0.1))
    {
      color += ComputeLight(i);
      i++;
    }
  }

  float LWhite = PresentLightColorIntensity.w;
  // HDR for pbr models
  color = color * (1 + color / (LWhite * LWhite)) / (1 + color);
  OutColor = vec4(clamp(color, 0, 1), 1);
  //OutColor.rgb = OutColor.rgb / (OutColor.rgb + vec3(1.0));
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
