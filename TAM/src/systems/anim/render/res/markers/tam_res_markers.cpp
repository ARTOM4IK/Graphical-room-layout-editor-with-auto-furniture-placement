/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_markers.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Marker system implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 23.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Marker system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::marker_manager::Init( VOID )
{
  ShaderMarkers[0] = RndRef.ShaderCreate("markers/sphere");
  ShaderMarkers[1] = RndRef.ShaderCreate("markers/cylinder");
  ShaderMarkers[2] = RndRef.ShaderCreate("markers/AABB");
  ShaderMarkers[3] = RndRef.ShaderCreate("markers/AABBMatr");
  ShaderMarkers[4] = RndRef.ShaderCreate("markers/line");

  for (INT i = 0; i < vlk::core::UsedFramesOnTheFly; i++)
  {
    BufferMarkers[i][0] = RndRef.BufferCreate((INT)(INITIAL_MARKER_CAPACITY * sizeof(BUF_MARKER_SPHERE)),
      nullptr,
      VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    BufferMarkerSize[i][0] = INITIAL_MARKER_CAPACITY;

    BufferMarkers[i][1] = RndRef.BufferCreate((INT)(INITIAL_MARKER_CAPACITY * sizeof(BUF_MARKER_CYLINDER)),
      nullptr,
      VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    BufferMarkerSize[i][1] = INITIAL_MARKER_CAPACITY;

    BufferMarkers[i][2] = RndRef.BufferCreate((INT)(INITIAL_MARKER_CAPACITY * sizeof(BUF_MARKER_AABB)),
      nullptr,
      VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    BufferMarkerSize[i][2] = INITIAL_MARKER_CAPACITY;

    BufferMarkers[i][3] = RndRef.BufferCreate((INT)(INITIAL_MARKER_CAPACITY * sizeof(BUF_MARKER_AABB_MATR)),
      nullptr,
      VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    BufferMarkerSize[i][3] = INITIAL_MARKER_CAPACITY;

    BufferMarkers[i][4] = RndRef.BufferCreate((INT)(INITIAL_MARKER_CAPACITY * sizeof(BUF_MARKER_LINE)),
      nullptr,
      VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    BufferMarkerSize[i][4] = INITIAL_MARKER_CAPACITY;

    MtlPatMarkers[i][0] =
      RndRef.MaterialPatternCreate(ShaderMarkers[0], "Markers Sphere Pattern", 1, 0, {BufferMarkers[i][0]}, {},
        {}, vlk::topology::PATCH, vlk::blend_mode::eOPAQUE, 1); //!!! Temporary opaque, when transparent deffered pass will be ready change to eTRANSPARENT
    MtlPatMarkers[i][1] =
      RndRef.MaterialPatternCreate(ShaderMarkers[1], "Markers Cylinder Pattern", 1, 0, {BufferMarkers[i][1]}, {},
        {}, vlk::topology::PATCH, vlk::blend_mode::eOPAQUE, 1); //!!! Temporary opaque, when transparent deffered pass will be ready change to eTRANSPARENT
    MtlPatMarkers[i][2] =
      RndRef.MaterialPatternCreate(ShaderMarkers[2], "Markers AABB Pattern", 1, 0, {BufferMarkers[i][2]}, {},
        {}, vlk::topology::POINTS, vlk::blend_mode::eOPAQUE, 1); //!!! Temporary opaque, when transparent deffered pass will be ready change to eTRANSPARENT
    MtlPatMarkers[i][3] =
      RndRef.MaterialPatternCreate(ShaderMarkers[3], "Markers AABBMatr Pattern", 1, 0, {BufferMarkers[i][3]}, {},
        {}, vlk::topology::POINTS, vlk::blend_mode::eOPAQUE, 1); //!!! Temporary opaque, when transparent deffered pass will be ready change to eTRANSPARENT
    MtlPatMarkers[i][4] =
      RndRef.MaterialPatternCreate(ShaderMarkers[4], "Markers Line Pattern", 1, 0, {BufferMarkers[i][4]}, {},
        {}, vlk::topology::LINES, vlk::blend_mode::eOPAQUE, 1); //!!! Temporary opaque, when transparent deffered pass will be ready change to eTRANSPARENT

    MtlMarkers[i][0] =
      RndRef.MaterialCreate("Markers Sphere Material", MtlPatMarkers[i][0], {}, {});
    MtlMarkers[i][1] =
      RndRef.MaterialCreate("Markers Cylinder Material:", MtlPatMarkers[i][1], {}, {});
    MtlMarkers[i][2] =
      RndRef.MaterialCreate("Markers AABB Material:", MtlPatMarkers[i][2], {}, {});
    MtlMarkers[i][3] =
      RndRef.MaterialCreate("Markers AABB Matr Material:", MtlPatMarkers[i][3], {}, {});
    MtlMarkers[i][4] =
      RndRef.MaterialCreate("Markers Line Material:", MtlPatMarkers[i][4], {}, {});
  }
} /* End of 'tam::marker_manager::Init' function */

/* Draw all markers function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::marker_manager::DrawAllMarkers( VOID )
{
  auto &Vlk = RndRef;
  INT UsedNo = 0; // Vlk.CurrentFrame; // Vlk.ActiveSwapchainImage; // Vlk.CurrentFrame;
  VkCommandBuffer CmdBuf = Vlk.CommandBuffers[Vlk.CurrentFrame];

  // Collect and draw spheres
  if (!Spheres.empty())
  {
    INT size = (INT)Spheres.size();

    // Resize buffer
    if (size > BufferMarkerSize[UsedNo][0])
    {
      INT new_size = size * 2;
      BufferMarkers[UsedNo][0]->Resize(new_size * sizeof(BUF_MARKER_SPHERE));
      BufferMarkerSize[UsedNo][0] = new_size;
      MtlPatMarkers[UsedNo][0]->PatternDS.Update({&BufferMarkers[UsedNo][0]->VlkBuffer}, {}, {});
    }

    // Fill buffer
    BufferMarkers[UsedNo][0]->Update(Spheres.data(), 0, (INT)(size * sizeof(BUF_MARKER_SPHERE)));

    // Bind pipeline & descriptor sets
    MtlPatMarkers[UsedNo][0]->Apply();

    // Setup push constants
    /// matr w = matr::Identity(); /// ???
    /// BUF_PRIM MatrData
    /// {
    ///   .MatrWVP = w * RndRef.Cam.VP,
    ///   .MatrW = w,
    ///   .MatrWInv = w.Inverse(),
    /// };
    INT PC[2] {0, 0};
    vkCmdPushConstants(CmdBuf, MtlPatMarkers[UsedNo][0]->PatternPipeline.Layout,
      VK_SHADER_STAGE_ALL, 0, sizeof(INT) * 2, PC);

    // Draw primitives with instances
    vkCmdDraw(CmdBuf, 1, size, 0, 0);
  }
  // Collect and draw cylinders
  if (!Cylinders.empty())
  {
    INT size = (INT)Cylinders.size();

    // Resize buffer
    if (size > BufferMarkerSize[UsedNo][1])
    {
      INT new_size = size * 2;
      BufferMarkers[UsedNo][1]->Resize(new_size * sizeof(BUF_MARKER_CYLINDER));
      BufferMarkerSize[UsedNo][1] = new_size;
      MtlPatMarkers[UsedNo][1]->PatternDS.Update({&BufferMarkers[UsedNo][1]->VlkBuffer}, {}, {});
    }

    // Fill buffer
    BufferMarkers[UsedNo][1]->Update(Cylinders.data(), 0, (INT)(size * sizeof(BUF_MARKER_CYLINDER)));

    // Bind pipeline & descriptor sets
    MtlPatMarkers[UsedNo][1]->Apply();

    // Setup push constants
    /// matr w = matr::Identity(); /// ???
    /// BUF_PRIM MatrData
    /// {
    ///   .MatrWVP = w * RndRef.Cam.VP,
    ///   .MatrW = w,
    ///   .MatrWInv = w.Inverse(),
    /// };
    INT PC[2] {0, 0};
    vkCmdPushConstants(CmdBuf, MtlPatMarkers[UsedNo][1]->PatternPipeline.Layout,
      VK_SHADER_STAGE_ALL, 0, sizeof(INT) * 2, PC);

    // Draw primitives with instances
    vkCmdDraw(CmdBuf, 1, size, 0, 0);
  }
  // Collect and draw AABBs
  if (!AABBs.empty())
  {
    INT size = (INT)AABBs.size();

    // Resize buffer
    if (size > BufferMarkerSize[UsedNo][2])
    {
      INT new_size = size * 2;
      BufferMarkers[UsedNo][2]->Resize(new_size * sizeof(BUF_MARKER_AABB));
      BufferMarkerSize[UsedNo][2] = new_size;
      MtlPatMarkers[UsedNo][2]->PatternDS.Update({&BufferMarkers[UsedNo][2]->VlkBuffer}, {}, {});
    }

    // Fill buffer
    BufferMarkers[UsedNo][2]->Update(AABBs.data(), 0, (INT)(size * sizeof(BUF_MARKER_AABB)));

    // Bind pipeline & descriptor sets
    MtlPatMarkers[UsedNo][2]->Apply();

    // Setup push constants
    /// matr w = matr::Identity(); /// ???
    /// BUF_PRIM MatrData
    /// {
    ///   .MatrWVP = w * RndRef.Cam.VP,
    ///   .MatrW = w,
    ///   .MatrWInv = w.Inverse(),
    /// };
    INT PC[2] {0, 0};
    vkCmdPushConstants(CmdBuf, MtlPatMarkers[UsedNo][2]->PatternPipeline.Layout,
      VK_SHADER_STAGE_ALL, 0, sizeof(INT) * 2, PC);

    // Draw primitives with instances
    vkCmdDraw(CmdBuf, 1, size, 0, 0);
  }
  // Collect and draw AABBMatrs
  if (!AABBMatrs.empty())
  {
    INT size = (INT)AABBMatrs.size();

    // Resize buffer
    if (size > BufferMarkerSize[UsedNo][3])
    {
      INT new_size = size * 2;
      BufferMarkers[UsedNo][3]->Resize(new_size * sizeof(BUF_MARKER_AABB_MATR));
      BufferMarkerSize[UsedNo][3] = new_size;
      MtlPatMarkers[UsedNo][3]->PatternDS.Update({&BufferMarkers[UsedNo][3]->VlkBuffer}, {}, {});
    }

    // Fill buffer
    BufferMarkers[UsedNo][3]->Update(AABBMatrs.data(), 0, (INT)(size * sizeof(BUF_MARKER_AABB_MATR)));

    // Bind pipeline & descriptor sets
    MtlPatMarkers[UsedNo][3]->Apply();

    // Setup push constants
    /// matr w = matr::Identity(); /// ???
    /// BUF_PRIM MatrData
    /// {
    ///   .MatrWVP = w * RndRef.Cam.VP,
    ///   .MatrW = w,
    ///   .MatrWInv = w.Inverse(),
    /// };
    INT PC[2] {0, 0};
    vkCmdPushConstants(CmdBuf, MtlPatMarkers[UsedNo][3]->PatternPipeline.Layout,
      VK_SHADER_STAGE_ALL, 0, sizeof(INT) * 2, PC);

    // Draw primitives with instances
    vkCmdDraw(CmdBuf, 1, size, 0, 0);
  }

  // Collect and draw lines
  if (!Lines.empty())
  {
    INT size = (INT)Lines.size();

    // Resize buffer
    if (size > BufferMarkerSize[UsedNo][4])
    {
      INT new_size = size * 2;
      BufferMarkers[UsedNo][4]->Resize(new_size * sizeof(BUF_MARKER_LINE));
      BufferMarkerSize[UsedNo][4] = new_size;
      MtlPatMarkers[UsedNo][4]->PatternDS.Update({&BufferMarkers[UsedNo][4]->VlkBuffer}, {}, {});
    }

    // Fill buffer
    BufferMarkers[UsedNo][4]->Update(Lines.data(), 0, (INT)(size * sizeof(BUF_MARKER_LINE)));

    // Bind pipeline & descriptor sets
    MtlPatMarkers[UsedNo][4]->Apply();

    // Setup push constants
    /// matr w = matr::Identity(); /// ???
    /// BUF_PRIM MatrData
    /// {
    ///   .MatrWVP = w * RndRef.Cam.VP,
    ///   .MatrW = w,
    ///   .MatrWInv = w.Inverse(),
    /// };
    INT PC[2] {0, 0};
    vkCmdPushConstants(CmdBuf, MtlPatMarkers[UsedNo][4]->PatternPipeline.Layout,
      VK_SHADER_STAGE_ALL, 0, sizeof(INT) * 2, PC);

    // Draw primitives with instances
    vkCmdDraw(CmdBuf, 2, size, 0, 0);
  }
  Spheres.clear();
  Cylinders.clear();
  AABBs.clear();
  AABBMatrs.clear();
  Lines.clear();
} /* End of 'tam::marker_manager::DrawAllMarkers' function */

/* END OF 'tam_res_markers.cpp' FILE */
