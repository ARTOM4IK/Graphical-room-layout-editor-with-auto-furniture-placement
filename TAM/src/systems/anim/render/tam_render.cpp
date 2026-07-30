/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_render.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Render functions implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 21.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Initialize render function.
 * ARGUMENTS:
 *   - reference to window handle:
 *       HWND &hWndRef;
 *   - initial frame sizes:
 *       INT Width, Height;
 * RETURNS:
 *   (BOOL) TRUE if render system was successfully initialized, FALSE otherwise.
 */
BOOL tam::render::Init( HWND &hWndRef, INT Width, INT Height )
{
  if (IsRenderInit)
    return TRUE;
  if (!core::Create(hWndRef, Width, Height))
    return FALSE;

  render_pipeline::InitPipeline();
  // Create buffers for camera and synchronization (HOST_COHERENT so GPU sees updates without explicit flush)
  CamBuf = BufferCreate<BUF_CAM>(nullptr,
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT); // VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  SyncBuf = BufferCreate<BUF_SYNC>(nullptr,
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT); // VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  OpaqueIdBuf = BufferCreate(Width * Height * sizeof(INT), nullptr,
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);// VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  TransparentIdBuf = BufferCreate(Width * Height * sizeof(INT), nullptr,
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT); // VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  Cam.SetLocAtUp(vec3(10), vec3(0), vec3(0, 1, 0));
  Cam.Resize(Width, Height);

  if (!render_pipeline::Create())
    return FALSE;
  IsRenderInit = TRUE;
  marker_manager::Init();
  CreateUI(hWndRef);
  return TRUE;
} /* End of 'tam::render::Init' function */

/* Close render system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::render::Close( VOID )
{
  if (!IsRenderInit)
    return;
  CloseUI();
  marker_manager::Close();
  CamBuf = nullptr;
  SyncBuf = nullptr;
  OpaqueIdBuf = nullptr;
  TransparentIdBuf = nullptr;
  vkDeviceWaitIdle(Device);
  render_pipeline::Close();
  render_pipeline::ClosePipeline();
  FrameSyncDescriptorSetLayout.Destroy();
  FrameSyncDescriptorSet.Destroy();
  light_manager::Clear();
  model_manager::Clear();
  font_manager::Clear();
  primitive_manager::Clear();
  material_manager::Clear();
  material_pattern_manager::Clear();
  texture_manager::Clear();
  buffer_manager::Clear();
  shader_manager::Clear();
  core::Destroy();
  IsRenderInit = FALSE;
} /* End of 'tam::render::Close' function */

/* Resize render frame function.
 * ARGUMENTS:
 *   - new width and height of window:
 *       INT NewW, NewH;
 * RETURNS: None.
 */
VOID tam::render::Resize( INT NewW, INT NewH )
{
  if (!IsRenderInit)
    return;
  FrameW = NewW, FrameH = NewH;
  Cam.Resize(FrameW, FrameH);

  // Resize id's buffer and update global descriptor set
  OpaqueIdBuf->Resize(NewW * NewH * sizeof(INT));
  TransparentIdBuf->Resize(NewW * NewH * sizeof(INT));
  VlkRef->FrameSyncDescriptorSet.Update({&SyncBuf->VlkBuffer,
    &CamBuf->VlkBuffer,
    &GlobalWorldMatricesBuf->VlkBuffer,
    &GlobalWVPWInvMatricesBuf->VlkBuffer,
    &OpaqueIdBuf->VlkBuffer,
    &TransparentIdBuf->VlkBuffer}, {}, {});

  core::surface::Resize();
  core::swapchain::Resize();
  render_pipeline::Resize();
  // Cam.Resize(NewW, NewH);
} /* End of 'tam::render::Resize' function */

/* Prepare render for new frame function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::render::FrameStart( VOID )
{
  if (!IsRenderInit)
    return;
  SystemProfiler.CollectCounter("Shader stock:", shader_manager::Stock.size());
  //for (auto s : shader_manager::Stock)
  //  SystemProfiler.CollectCounter(s.second->Name.c_str(), s.second.use_count());
  SystemProfiler.CollectCounter("Buffer stock:", buffer_manager::Stock.size());
  SystemProfiler.CollectCounter("Texture stock:", texture_manager::Stock.size());
  //for (auto s : texture_manager::Stock)
  //  SystemProfiler.CollectCounter(s.second->Name.c_str(), s.second.use_count());
  SystemProfiler.CollectCounter("MPattern stock:", material_pattern_manager::Stock.size());
  //for (auto s : material_pattern_manager::Stock)
  //  SystemProfiler.CollectCounter(s.second->Name.c_str(), s.second.use_count());
  SystemProfiler.CollectCounter("Material stock:", material_manager::Stock.size());
  //for (auto s : material_manager::Stock)
  //  SystemProfiler.CollectCounter(s.second->Name.c_str(), s.second.use_count());
  SystemProfiler.CollectCounter("Primitive stock:", primitive_manager::Stock.size()); 
  SystemProfiler.CollectCounter("Model stock:", model_manager::Stock.size()); 
  SystemProfiler.CollectCounter("Font stock:", font_manager::Stock.size());
  //for (auto s : font_manager::Stock)
  //  SystemProfiler.CollectCounter(s.second->Name.c_str(), s.second.use_count());

  core::FramePreStart();

  font_manager::Freeze();
  model_manager::Freeze(); 
  primitive_manager::Freeze(); 
  //primitive_manager::Freeze();
  material_manager::Freeze();
  material_pattern_manager::Freeze();
  texture_manager::Freeze();
  buffer_manager::Freeze();
  shader_manager::Freeze();
  light_manager::Freeze();

  BUF_SYNC SyncData =
  {
    vec4(Timer.GlobalTime, Timer.GlobalDeltaTime, Timer.Time, Timer.DeltaTime),
    vec4(FALSE, Timer.IsPause, FALSE, Brightness),     // wireframe / pause / debug / fps
  };
  SyncBuf->Update(&SyncData, 0, sizeof(BUF_SYNC));
  SystemProfiler.Collect("Updating sync buffer");

  // Set camera to user viewport
  if (SceneViewport.W != 0 && SceneViewport.H != 0 &&
    (SceneViewport.W != Cam.FrameW || SceneViewport.H != Cam.FrameH))
    Cam.Resize((INT)SceneViewport.W, (INT)SceneViewport.H);

  BUF_CAM CamData =
  {
    Cam.View,
    Cam.Proj,
    Cam.VP,
    vec4(Cam.Loc, Cam.FrameW),
    vec4(Cam.Dir, Cam.FrameH),
    vec4(Cam.Right, Cam.Wp),
    vec4(Cam.Up, Cam.Hp),
    vec4(Cam.At, Cam.ProjDist),
    vec4(Cam.Size, Cam.OffsetX, Cam.OffsetY, 0),
    vec4(Input.Mx, Input.My, Input.MBut[0], Input.MBut[1]),
    mth::vec4<INT>(CurrentObjectId, 0, 0, 0),
  };
  CamBuf->Update(&CamData, 0, sizeof(BUF_CAM));
  SystemProfiler.Collect("Updating camera buffer");

  FillGlobalMatrices();
  SystemProfiler.Collect("Filling world matrices buffer");
  FillLights();
  SystemProfiler.Collect("Filling lights buffer");

  if (INT(Timer.GlobalTime) % 3 == 0)
  {
    ShadersUpdate();
    if (IsShaderChange)
      MaterialPatternsUpdate();
  }
  core::FrameStart();

  // Set user viewport
  if (SceneViewport.W != 0 && SceneViewport.H != 0)
  {
    VkViewport Viewport
    {
      .x = SceneViewport.X,
      .y = SceneViewport.Y + SceneViewport.H, // Y-flip для Vulkan
      .width = SceneViewport.W,
      .height = -SceneViewport.H,
      .minDepth = 0.0f,
      .maxDepth = 1.0f,
    };
    vkCmdSetViewport(CommandBuffers[CurrentFrame], 0, 1, &Viewport);
    VkRect2D Scissor
    {
      .offset = {(INT)SceneViewport.X, (INT)SceneViewport.Y},
      .extent = {(UINT)SceneViewport.W, (UINT)SceneViewport.H},
    };
    vkCmdSetScissor(CommandBuffers[CurrentFrame], 0, 1, &Scissor);
  }
} /* End of 'tam::render::FrameStart' function */

/* End frame function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::render::FrameEnd( VOID )
{
  if (!IsRenderInit)
    return;
  
  SystemProfiler.CollectCounter("Markers: spheres", marker_manager::GetMarkersCount(0));
  SystemProfiler.CollectCounter("Markers: cylinders", marker_manager::GetMarkersCount(1));
  SystemProfiler.CollectCounter("Markers: total", marker_manager::GetMarkersCount(-1));
  SystemProfiler.Collect("Before Marker Draw");
  //DrawAllMarkers();
  SystemProfiler.Collect("Markers draw");

  FrameDraw();
  SystemProfiler.Collect("Draw scene");

  // Restore full viewport for UI
  if (SceneViewport.W != 0 && SceneViewport.H != 0)
  {
    VkViewport FullViewport
    {
      .x = 0.0f,
      .y = (FLT)SwapchainInfo.imageExtent.height,
      .width = (FLT)SwapchainInfo.imageExtent.width,
      .height = -(FLT)SwapchainInfo.imageExtent.height,
      .minDepth = 0.0f,
      .maxDepth = 1.0f,
    };
    vkCmdSetViewport(CommandBuffers[CurrentFrame], 0, 1, &FullViewport);

    VkRect2D FullScissor
    {
      .offset = {0, 0},
      .extent = SwapchainInfo.imageExtent,
    };
    vkCmdSetScissor(CommandBuffers[CurrentFrame], 0, 1, &FullScissor);
  }

  SystemProfiler.Collect("Before Frame End");

  core::FrameEnd();
  SystemProfiler.Collect("After Frame End");
  ShadersChange();
} /* End of 'tam::render::FrameEnd' function */

/* END OF 'tam_render.cpp' FILE */
