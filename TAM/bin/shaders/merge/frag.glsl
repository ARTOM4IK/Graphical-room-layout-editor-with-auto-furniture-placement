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
 * LAST UPDATE : 23.04.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

#include "commondf.glsl"

layout(input_attachment_index = 0, set = 1, binding = 0) uniform subpassInput InColorOpaq;
layout(input_attachment_index = 1, set = 1, binding = 1) uniform subpassInput InDepthOpaq;
layout(input_attachment_index = 2, set = 1, binding = 2) uniform subpassInput InColorTran;
layout(input_attachment_index = 3, set = 1, binding = 3) uniform subpassInput InDepthTran;

layout(location = 0) out vec4 OutColor;

void main( void )
{
  //OutColor = vec4(subpassLoad(InColorOpaq).xyz, 1);
  vec4 Opaq = vec4(subpassLoad(InColorOpaq).xyz, 1.0);
  vec4 Tran = subpassLoad(InColorTran);
  vec4 OpaqDepth = subpassLoad(InDepthOpaq);
  vec4 TransDepth = subpassLoad(InDepthTran);

  // Mark selected model
  //int Id = OpaqueId[int(int(gl_FragCoord.x) + int(gl_FragCoord.y) * (FrameW + FrameOffsetX))];
  //int TransId = TransparentId[int(int(gl_FragCoord.x) + int(gl_FragCoord.y) * (FrameW + FrameOffsetX))];

  //if (CurrentObjectId != 0)
  //{
  //  int cnt = 0, cid = -1;
  //  for (int i = int(gl_FragCoord.y - 1); i <= int(gl_FragCoord.y + 1); i++)
  //    for (int j = int(gl_FragCoord.x - 1); j <= int(gl_FragCoord.x + 1); j++)
  //      if (i != 0 || i != 0)
  //     {
  //        if (OpaqueId[j + i * int(FrameW + FrameOffsetX)] == CurrentObjectId)
  //          cnt++;
  //      }
  //      else
  //        cid = OpaqueId[j + i * int(FrameW + FrameOffsetX)];
  //}
   
  //if (cnt > 8 && cid != CurrentObjectId)
  //  Opaq.xyz = vec3(0.8, 0.47, 0.30) * 0.30;
  int ModelIdBufferInd = int(int(gl_FragCoord.x) + int(gl_FragCoord.y) * (FrameW + FrameOffsetX));
  // int Id = OpaqueId[ModelIdBufferInd];
  int TransId = TransparentId[ModelIdBufferInd];
  /*
  if (Id == CurrentObjectId && Id != 0)
  //Opaq.xyz += 0.5 * abs(sin(Time * 5)) * vec3(0.8, 0.47, 0.30);
  Opaq.xyz += 0.30;
  // Opaq.xyz *= 2;
  if ((TransId == CurrentObjectId) && (TransId != 0))
    Tran.xyz += 0.5 * abs(sin(Time * 5)) * vec3(0.8, 0.47, 0.30);
    // Tran.xyz *= 2;
  */
  vec3 ResColor = vec3(0);
  if (OpaqDepth.x < TransDepth.x)
  {
    //Opaq.xyz = Opaq.xyz / (Opaq.xyz + vec3(1.0));
    ResColor = Opaq.xyz;
    //OutColor.rgb *= Brightness;
    //return;
  }
  else
  {
    //atomicExchange(OpaqueId[ModelIdBufferInd], TransId); 
    ResColor = Opaq.rgb * (1 - Tran.a) + Tran.rgb * (Tran.a);
  }
  // HDR 
  //ResColor = ResColor / (ResColor + vec3(1.0));
  OutColor = vec4(ResColor, 1);
  // Brightness multiplier application
  OutColor.rgb *= Brightness;

  // Color selected objects
  if (CurrentObjectId != 0)
  {
    const int nn = 5;
    bool m[nn][nn];
    int cnti = 0;
    for (int i = 0; i < nn; i++)
      for (int j = 0; j < nn; j++)
      {
        m[i][j] = (OpaqueId[int(j - nn / 2 + gl_FragCoord.x) + int(i - nn / 2 + gl_FragCoord.y) * int(FrameW + FrameOffsetX)]) == CurrentObjectId;
        cnti += int(m[i][j]);
      }
  
    if (!m[nn / 2][nn / 2] && cnti > 0)//nn / 2)
    {
        //(m[0][0] || m[2][2]))
      //Opaq.xyz = vec3(0.0000000047, 0.8, 0.0000030) * 11.30;
      float t = cnti / (nn * nn  - 1.0);
      OutColor = mix(vec4(1, 1, 0, 1), vec4(1, 1, 0, 1), 2 * t);
      return;
    }
  }
}

/* END OF 'frag.glsl' FILE */
