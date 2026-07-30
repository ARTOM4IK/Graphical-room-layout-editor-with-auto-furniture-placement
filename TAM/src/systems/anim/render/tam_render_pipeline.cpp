/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

 /* FILE NAME   : tam_render_pipeline.cpp
  * PURPOSE     : Tough Arrangement Manager project.
  *               Render pipeline implementation module.
  * PROGRAMMER  : CGSG'Sr'2025.
  *               Arsentev Artemy (AA4),
  *               Nechaev Vladimir (VN4),
  *               Tretyakov Andrey (AT7).
  * LAST UPDATE : 01.05.2026
  * NOTE        : Namespace 'tam'.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include "tam.h"

/* Default type constructor.
 * ARGUMENTS:
 *   - pointer to render:
 *       render *NewRnd;
 */
tam::compute_manager::compute_manager( render *NewRnd ) : Rnd(NewRnd), ComputeDSLayout(NewRnd), ComputeDS(NewRnd)
{
} /* End of 'tam::compute_manager::compute_manager' function */

/* Pipeline constructor.
 * ARGUMENTS:
 *   - pointer to render:
 *       render *Rnd;
 */
tam::render_pipeline::render_pipeline( render *Rnd ) : compute_manager(Rnd)
{
} /* End of 'render_pipeline' function */

/* Create compute manager function.
 * ARGUMENTS:
 *   - compute shader pointer for this manager:
 *       const shader ComputeShader;
 *   - input data:
 *       const compute_data &InputData;
 *   - output data:
 *       const compute_data &OutputData;
 *   - descriptor types vector:
 *       const std::vector<VkDescriptorType> &DescriptorTypes;
 * RETURNS:
 *   (BOOL) TRUE if compute manager was initialized successfully, FALSE otherwise.
 */
BOOL tam::compute_manager::Create( const shader ComputeShader, const compute_data &InputData, const compute_data &OutputData,
                                   const std::vector<VkDescriptorType> &DescriptorTypes )
{
  if (ComputeShader == nullptr || ComputeShader->VlkShd.ShaderModuleComp == VK_NULL_HANDLE)
  {
    logger::error << "Missing compute shader for compute pipeline!";
    return FALSE;
  }
  VkPipelineShaderStageCreateInfo CompShdStageInfo =
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_COMPUTE_BIT,
      .module = ComputeShader->VlkShd.ShaderModuleComp,
      .pName = "main",
    };

  std::vector<VkDescriptorSetLayoutBinding> LayoutInfos {};
  UINT BindingNo = 0;
  for (auto &Data : {InputData, OutputData})
  {
    for (auto &Buf : Data.Buffers)
      LayoutInfos.emplace_back(VkDescriptorSetLayoutBinding
        {
          .binding = BindingNo,
          .descriptorType = DescriptorTypes[BindingNo++],// VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
          .descriptorCount = 1,
          .stageFlags = VK_SHADER_STAGE_COMPUTE_BIT,
        });
    UINT TexCount = static_cast<UINT>(Data.Textures.size());
    if (TexCount > 0)
      LayoutInfos.emplace_back(VkDescriptorSetLayoutBinding
        {
          .binding = BindingNo,
          .descriptorType = DescriptorTypes[BindingNo++],
          .descriptorCount = TexCount,
          .stageFlags = VK_SHADER_STAGE_COMPUTE_BIT,
        });
  }
  if (!ComputeDSLayout.Create(LayoutInfos))
  {
    logger::error << "Error creating descriptor set layout for " + ComputeShader->Name + '!';
    return FALSE;
  }

  // Create compute pipeline layout
  VkPipelineLayoutCreateInfo Info
  {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .setLayoutCount = 1U,
    .pSetLayouts = &ComputeDSLayout.DescriptorSetLayout,
    .pushConstantRangeCount = 0,
    .pPushConstantRanges = nullptr,
  };

  if (vkCreatePipelineLayout(Rnd->Device, &Info, nullptr, &ComputePipelineLayout) != VK_SUCCESS)
  {
    logger::error << "Error creating compute pipeline layout for " + ComputeShader->Name + '!';
    ComputeDSLayout.Destroy();
    return FALSE;
  }

  // Create compute pipeline
  VkComputePipelineCreateInfo CreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .stage = CompShdStageInfo,
    .layout = ComputePipelineLayout,
    .basePipelineHandle = VK_NULL_HANDLE,
    .basePipelineIndex = 0,
  };

  if (vkCreateComputePipelines(Rnd->Device, VK_NULL_HANDLE, 1, &CreateInfo, nullptr, &ComputePipeline) != VK_SUCCESS)
  {
    logger::error << "Error creating compute pipeline for " + ComputeShader->Name + '!';
    vkDestroyPipelineLayout(Rnd->Device, ComputePipelineLayout, nullptr);
    ComputeDSLayout.Destroy();
    return FALSE;
  }
  std::vector<vlk::buffer *> ResBufs {};
  ResBufs.reserve(InputData.Buffers.size() + OutputData.Buffers.size());
  for (auto &Buf : InputData.Buffers)
    ResBufs.push_back(&Buf->VlkBuffer);
  for (auto &Buf : OutputData.Buffers)
    ResBufs.push_back(&Buf->VlkBuffer);
  std::vector<VkImageView> ResTexs {};
  ResTexs.reserve(InputData.Textures.size() + OutputData.Textures.size());
  for (auto &Tex : InputData.Textures)
    ResTexs.push_back(Tex->Img.View);
  for (auto &Tex : OutputData.Textures)
    ResTexs.push_back(Tex->Img.View);
  // ComputeDescriptorTypes = DescriptorTypes;
  if (!ComputeDS.Create(ComputeDSLayout, ResBufs, ResTexs, {}))
  {
    logger::error << "Error creating descriptor set for compute shader: " + ComputeShader->Name + ".";
    vkDestroyPipeline(Rnd->Device, ComputePipeline, nullptr);
    vkDestroyPipelineLayout(Rnd->Device, ComputePipelineLayout, nullptr);
    ComputeDSLayout.Destroy();
    return FALSE;
  }

  return TRUE;
} /* End of 'tam::compute_manager::Create' function */

/* Execute compute shader function.
 * ARGUMENTS:
 *   - count of compute iterations:
 *       INT ComputeCount;
 * RETURNS: None.
 */
VOID tam::compute_manager::Compute( INT ComputeSize )
{
  vkCmdBindPipeline(Rnd->CommandBuffers[Rnd->CurrentFrame], VK_PIPELINE_BIND_POINT_COMPUTE, ComputePipeline);
  // Pipeline barriers ...
  vkCmdBindDescriptorSets(Rnd->CommandBuffers[Rnd->CurrentFrame], VK_PIPELINE_BIND_POINT_COMPUTE, ComputePipelineLayout, 0, 1, &ComputeDS.DescriptorSet, 0, nullptr);
  vkCmdDispatch(Rnd->CommandBuffers[Rnd->CurrentFrame], render_config::XGroups, ComputeSize / render_config::ElementsPerSlice + 1, ComputeSize / render_config::ElementsPerDepthSlice + 1);
} /* End of 'tam::compute_manager::Compute' function */

/* Update data descriptor set function.
 * ARGUMENTS:
 *   - new input data:
 *       const compute_data &NewInputData;
 *   - new output data:
 *       const compute_data &NewOutputData;
 * RETURNS: None.
 */
VOID tam::compute_manager::Update( const compute_data &NewInputData, const compute_data &NewOutputData )
{
  std::vector<vlk::buffer *> ResBufs {};
  ResBufs.reserve(NewInputData.Buffers.size() + NewOutputData.Buffers.size());
  for (auto &Buf : NewInputData.Buffers)
    ResBufs.push_back(&Buf->VlkBuffer);
  for (auto &Buf : NewOutputData.Buffers)
    ResBufs.push_back(&Buf->VlkBuffer);
  std::vector<VkImageView> ResTexs {};
  ResTexs.reserve(NewInputData.Textures.size() + NewOutputData.Textures.size());
  for (auto &Tex : NewInputData.Textures)
    ResTexs.push_back(Tex->Img.View);
  for (auto &Tex : NewOutputData.Textures)
    ResTexs.push_back(Tex->Img.View);
  ComputeDS.Update(ResBufs, ResTexs, {});
} /* End of 'tam::compute_manager::Update' function */

/* Free compute manager function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::compute_manager::Free( VOID )
{
  shader ComputeShader = nullptr;
  vkDestroyPipeline(Rnd->Device, ComputePipeline, nullptr);
  vkDestroyPipelineLayout(Rnd->Device, ComputePipelineLayout, nullptr);
  ComputeDSLayout.Destroy();
  ComputeDS.Destroy();
} /* End of 'tam::compute_manager::Free' function */

/* Default type destructor.
 * ARGUMENTS: None.
 */
tam::compute_manager::~compute_manager( VOID )
{
  if (ComputeShader != nullptr)
    Free();
} /* End of 'tam::compute_manager::~compute_manager' function */

/* Create render pipeline function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) TRUE if render pipeline was initialized successfully, FALSE otherwise.
 */
BOOL tam::render_pipeline::Create( VOID )
{
  // Create buffer for world matrices
  GlobalWorldMatricesBuf = Rnd->BufferCreate(sizeof(matr::matr_data) * render_config::InitialMatrixCapacity,
    nullptr,
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT); /// VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  // Create buffer for calculated WVP and WInverse matrices
  GlobalWVPWInvMatricesBuf = Rnd->BufferCreate(sizeof(matr::matr_data) * render_config::InitialMatrixCapacity * 2, nullptr,
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT); /// VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  // Initialize matrix recalculator
  if (!compute_manager::Create(Rnd->ShaderCreate("world_compute"), {{Rnd->CamBuf, GlobalWorldMatricesBuf}, {}}, {{GlobalWVPWInvMatricesBuf}, {}},
    {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER}))
  {
    logger::error << "Error initializing pipeline.";
    compute_manager::Free();
    /// Free();
    return FALSE;
  }
  Rnd->FrameSyncDescriptorSet.Update({&Rnd->SyncBuf->VlkBuffer,
    &Rnd->CamBuf->VlkBuffer,
    &GlobalWorldMatricesBuf->VlkBuffer,
    &GlobalWVPWInvMatricesBuf->VlkBuffer,
    &Rnd->OpaqueIdBuf->VlkBuffer,
    &Rnd->TransparentIdBuf->VlkBuffer}, {}, {});
  return TRUE;
} /* End of 'tam::render_pipeline::Create' function */

/* Destroy render pipeline function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::render_pipeline::Close( VOID )
{
  compute_manager::Free();
  GlobalWorldMatricesBuf = nullptr;
  GlobalWVPWInvMatricesBuf = nullptr;
  LightsBuf = nullptr;
  // Clear pointers to draw objects that are in pipeline
  ClearDrawQueue();
  FirstDraw = nullptr;
  StoreFrameNo = 0;
  CurrentFrameNo = 0;
  IsCamBufUpdate = FALSE;
  IsWorldBufferResize = FALSE;
  IsWorldBufferUpdate = FALSE;
  GlobalMatrixes.clear();
  GlobalMatrixes.shrink_to_fit();
  Lights.clear();
  NumOfGlobalMatrixes = 0;
  OldNumOfGlobalMatrixes = 0;
  MaxNumOfGlobalMatrixes = 0;
} /* End of 'tam::render_pipeline::Close' function */

/* Fill global matrixes buffer function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::render_pipeline::FillGlobalMatrices( VOID )
{
  // Check for case if there is no draw objects so we won't draw anyting
  if (StoreFrameNo != CurrentFrameNo)
  {
    ClearDrawQueue();
    return;
  }
  material_pattern MtlPat;
  material Mtl;
  prim  Pr;

  NumOfGlobalMatrixes = 0;
  MtlPat = FirstDraw;
  while (MtlPat != nullptr)
  {
    Mtl = MtlPat->FirstDraw;
    while (Mtl != nullptr)
    {
      Pr = Mtl->FirstDraw;
      while (Pr != nullptr)
      {
        if (Pr->NumOfInstances + NumOfGlobalMatrixes < MaxNumOfGlobalMatrixes)
        {
          for (INT i = 0; i < Pr->NumOfInstances; i++)
            GlobalMatrixes[NumOfGlobalMatrixes + i] = Pr->WorldMatrixes[i];
          //GlobalMatrixes.insert(GlobalMatrixes.begin() + NumOfGlobalMatrixes, Pr->WorldMatrixes.begin(), Pr->WorldMatrixes.begin() + Pr->NumOfInstances);
        }
        else
        {
          MaxNumOfGlobalMatrixes = static_cast<UINT>(MaxNumOfGlobalMatrixes * 1.5) + (Pr->NumOfInstances / FORWARD_ALLOCATION_MATRS + 1) * FORWARD_ALLOCATION_MATRS;
          GlobalMatrixes.resize(MaxNumOfGlobalMatrixes, matr::Identity());
          for (INT i = 0; i < Pr->NumOfInstances; i++)
            GlobalMatrixes[NumOfGlobalMatrixes + i] = Pr->WorldMatrixes[i];
        }
        Pr->FirstMatrixIndex = NumOfGlobalMatrixes;
        NumOfGlobalMatrixes += Pr->NumOfInstances;
        // Check if count of instances of primitive was changed so matrices are different
        IsWorldBufferUpdate |= Pr->NumOfInstances != Pr->PreviousFrameNumOfInstances;
        Pr->PreviousFrameNumOfInstances = Pr->NumOfInstances;
        Pr = Pr->NextDraw;
      }
      Mtl = Mtl->NextDraw;
    }
    MtlPat = MtlPat->NextDraw;
  }
  if (NumOfGlobalMatrixes != OldNumOfGlobalMatrixes)
    IsWorldBufferUpdate = TRUE;
  OldNumOfGlobalMatrixes = NumOfGlobalMatrixes;

  SystemProfiler.Collect("collect matrixes");

  const static INT MatrSize = sizeof(matr::matr_data);
  /* Don't resize buffer if it's size is bigger then needed */
  if (GlobalWorldMatricesBuf->Size < NumOfGlobalMatrixes * MatrSize)
  {
    GlobalWorldMatricesBuf->Resize(MatrSize * NumOfGlobalMatrixes);
    GlobalWVPWInvMatricesBuf->Resize(2 * MatrSize * NumOfGlobalMatrixes);
    compute_manager::Update({{Rnd->CamBuf, GlobalWorldMatricesBuf}, {}}, {{GlobalWVPWInvMatricesBuf}, {}});
    Rnd->FrameSyncDescriptorSet.Update({&Rnd->SyncBuf->VlkBuffer,
                                        &Rnd->CamBuf->VlkBuffer,
                                        &GlobalWorldMatricesBuf->VlkBuffer, 
                                        &GlobalWVPWInvMatricesBuf->VlkBuffer}, {}, {});
  }
  if (IsWorldBufferUpdate)
    GlobalWorldMatricesBuf->Update(GlobalMatrixes.data(), 0, MatrSize * NumOfGlobalMatrixes);
  SystemProfiler.Collect("Updating matrixes buffer");

  BOOL IsBufferRecalc = IsCamBufUpdate || IsWorldBufferUpdate;
  if (NumOfGlobalMatrixes != 0 && IsBufferRecalc)
  {
    compute_manager::Compute(NumOfGlobalMatrixes);
    IsWorldBufferUpdate = FALSE;
    IsBufferRecalc = FALSE;
  }
  SystemProfiler.Collect("compute matrices");
} /* End of 'tam::render_pipeline::FillGlobalMatrices' function */

/* Clear draw queue function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::render_pipeline::ClearDrawQueue( VOID )
{
  /* Clear pointers that are in render queue */
  material_pattern MtlPat = FirstDraw, OldMtlPat = nullptr;
  material Mtl = nullptr, OldMtl = nullptr;
  prim Pr = nullptr, OldPr = nullptr;
  while (MtlPat != nullptr)
  {
    Mtl = MtlPat->FirstDraw;
    while (Mtl != nullptr)
    {
      Pr = Mtl->FirstDraw;
      while (Pr != nullptr)
      {
        OldPr = Pr;
        Pr = Pr->NextDraw;
        OldPr->NextDraw = nullptr;
      }
      OldMtl = Mtl;
      Mtl = Mtl->NextDraw;
      OldMtl->NextDraw = nullptr;
      OldMtl->FirstDraw = nullptr;
    }
    OldMtlPat = MtlPat;
    MtlPat = MtlPat->NextDraw;
    OldMtlPat->NextDraw = nullptr;
    OldMtlPat->FirstDraw = nullptr;
  }
  OldPr = nullptr;
  OldMtl = nullptr;
  OldMtlPat = nullptr;
  FirstDraw = nullptr;
} /* End of 'tam::render_pipeline::ClearDrawQueue' fucntion */

/* Draw current frame function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::render_pipeline::FrameDraw( VOID )
{
  SystemProfiler.Collect("EndRenderPass");

  //ShadowPass(Rnd->CommandBuffers[Rnd->CurrentFrame], Rnd->FrameBuffers[Rnd->CurrentFrame], { (UINT)Rnd->FrameW, (UINT)Rnd->FrameH });
  OpaquePass(Rnd->CommandBuffers[Rnd->CurrentFrame], Rnd->OpaqueFrameBuf.FrameBuffer, {(UINT)Rnd->FrameW, (UINT)Rnd->FrameH});
  TransparentPass(Rnd->CommandBuffers[Rnd->CurrentFrame], Rnd->TransFrameBuf.FrameBuffer, {(UINT)Rnd->FrameW, (UINT)Rnd->FrameH});
  MergePass(Rnd->CommandBuffers[Rnd->CurrentFrame], /*Rnd->FrameBuffers[Rnd->ActiveSwapchainImage]*/Rnd->MergeFrameBuf.FrameBuffer, {(UINT)Rnd->FrameW, (UINT)Rnd->FrameH});
  PostProcessingPass(Rnd->CommandBuffers[Rnd->CurrentFrame], Rnd->FrameBuffers[Rnd->ActiveSwapchainImage], {(UINT)Rnd->FrameW, (UINT)Rnd->FrameH});

  SystemProfiler.Collect("EndFrameDraw");
} /* End of 'tam::render_pipeline::FrameDraw' function */

/* Draw every primitive into one target function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::render_pipeline::DrawQueuedPrims( vlk::blend_mode Mode )
{
  /* Draw every primitive from every material from every material pattern */
  material_pattern MtlPat = FirstDraw;
  material Mtl {};
  prim Pr {};
  while (MtlPat != nullptr)
  {
    if (MtlPat->PatternPipeline.Mode == Mode)
    {
      MtlPat->Apply();
      Mtl = MtlPat->FirstDraw;
      while (Mtl != nullptr)
      {
        Mtl->Apply();
        Pr = Mtl->FirstDraw;
        while (Pr != nullptr)
        {
          DrawInstant(Pr);
          Pr = Pr->NextDraw;
        }
        Mtl = Mtl->NextDraw;
      }
    }
    MtlPat = MtlPat->NextDraw;
  }
} /* End of 'tam::render_pipeline::DrawQueuedPrims' function */

/* Instantly draw primitive to screen function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       prim Pr;
 * RETURNS: None.
 */
VOID tam::render_pipeline::DrawInstant( const prim Pr )
{
  if (Pr->VertexBuf != nullptr)
  {
    INT PC[2] {Pr->FirstMatrixIndex, Pr->PrimitiveObjectId};
    vkCmdPushConstants(Rnd->CommandBuffers[Rnd->CurrentFrame], Pr->Mtl->MtlPat->PatternPipeline.Layout,
      VK_SHADER_STAGE_ALL, 0, sizeof(INT) * 2, PC);
    VkDeviceSize Offsets[] = {0};
    vkCmdBindVertexBuffers(Rnd->CommandBuffers[Rnd->CurrentFrame], 0, 1, &Pr->VertexBuf->VlkBuffer.Buffer, Offsets);
    if (Pr->IndexBuf != nullptr)
    {
      vkCmdBindIndexBuffer(Rnd->CommandBuffers[Rnd->CurrentFrame], Pr->IndexBuf->VlkBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
      vkCmdDrawIndexed(Rnd->CommandBuffers[Rnd->CurrentFrame], Pr->NumOfElements, Pr->NumOfInstances, 0, 0, 0);
    }
    else
      vkCmdDraw(Rnd->CommandBuffers[Rnd->CurrentFrame], Pr->NumOfElements, Pr->NumOfInstances, 0, 0);
  }
} /* End of 'tam::render_pipeline::DrawInstant' function */

/* Primitive draw function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       prim Pr;
 *   - transformation matrix:
 *       matr &World;
 *   - flag if primitive is static:
 *       BOOL IsStaticDraw;
 * RETURNS: None.
 */
VOID tam::render_pipeline::Draw( prim Pr, const matr &World, BOOL IsStaticDraw )
{
  if (Pr == nullptr)
    return;
  material_pattern MtlPat;
  material Mtl;

  IsWorldBufferUpdate |= !IsStaticDraw;
  /// IsWorldBufferUpdate |= Pr->NumOfInstances == 0;
  /* Check for a new frame */
  if (Pr->StoreFrameNo != CurrentFrameNo)
  {
    Pr->NumOfInstances = 0;
    Pr->StoreFrameNo = CurrentFrameNo;
  }
  /* Store instance matrix */
  if (Pr->NumOfInstances < Pr->WorldMatrixesSize)
    Pr->WorldMatrixes[Pr->NumOfInstances] = World;
  else
  {
    Pr->WorldMatrixesSize = (Pr->NumOfInstances / FORWARD_ALLOCATION_MATRS + 1) * FORWARD_ALLOCATION_MATRS;
    Pr->WorldMatrixes.resize(Pr->WorldMatrixesSize, matr::Identity());
    Pr->WorldMatrixes[Pr->NumOfInstances] = World;
  }
  Pr->WorldMatrixes[Pr->NumOfInstances++].M[3][3] = CurrentModelId;

  /* Add primitive to material primitive draw list */
  if (Pr->NumOfInstances == 1)
  {
    Pr->Transform = World;
    Mtl = Pr->Mtl;
    if (Mtl->StoreFrameNo != CurrentFrameNo)
    {
      Pr->NextDraw = nullptr;
      Mtl->StoreFrameNo = CurrentFrameNo;

      /* Add material to material patern material draw list */
      MtlPat = Mtl->MtlPat;
      if (MtlPat->StoreFrameNo != CurrentFrameNo)
      {
        Mtl->NextDraw = nullptr;
        MtlPat->StoreFrameNo = CurrentFrameNo;

        /* Add material pattern to global material pattern draw list */
        if (StoreFrameNo != CurrentFrameNo)
        {
          MtlPat->NextDraw = nullptr;
          StoreFrameNo = CurrentFrameNo;
        }
        else
          MtlPat->NextDraw = FirstDraw;
        FirstDraw = MtlPat;
      }
      else
        Mtl->NextDraw = MtlPat->FirstDraw;
      MtlPat->FirstDraw = Mtl;
    }
    else
      Pr->NextDraw = Mtl->FirstDraw;
    Mtl->FirstDraw = Pr;
  }
} /* End of 'tam::render_pipeline::Draw' function */

/* Model draw function.
 * ARGUMENTS:
 *   - model pointer:
 *       model Mdl;
 *   - transformation matrix:
 *       const matr World;
 *   - flag if model is static:
 *       BOOL IsStaticDraw;
 * RETURNS:
 *   (INT) drawn model id.
 */
INT tam::render_pipeline::Draw( model Mdl, const matr &World, BOOL IsStaticDraw )
{
  if (Mdl == nullptr)
    return -1;

  for (auto pr : Mdl->Prims)
    Draw(pr, World, IsStaticDraw);
  return ++CurrentModelId;
} /* End of 'tam::render_pipeline::Draw' function */

/* Init render pipeline function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
BOOL tam::render_pipeline::InitPipeline( VOID )
{
  /* Lights preperation */

  /* Init light buffer */
  LightsBuf = Rnd->BufferCreate(30, nullptr, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  std::array<vec3, 3> V = render_config::ClipSpaceTriangle;

  /* Prepare light opque */
  material_pattern opaq_mtl_pat_lig = Rnd->MaterialPatternCreate(Rnd->ShaderCreate("light/opaq"), "opaq_light_material_pattern", 0, 1,
    {}, {}, {}, vlk::topology::POINTS, vlk::blend_mode::eOPAQUE, 0, 7);

  for (INT i = 0; i < 6; i++)
    OpaqAttach.push_back(Rnd->TextureCreate("Light opaque attachment #" + std::to_string(i), Rnd->OpaqDefferedImg[i]));
  OpaqAttach.push_back(Rnd->TextureCreate("Light opaque attachment #" + std::to_string(7), Rnd->OpaqDefferedImg[7]));
  material opque_mtl_lig = Rnd->MaterialCreate("light_opaque_material", opaq_mtl_pat_lig, {}, {LightsBuf}, OpaqAttach);
  OpaqueLightScreenPrim = Rnd->PrimitiveCreate(opque_mtl_lig, std::span<vec3>(V.data(), 3), {});
  OpaqueLightScreenPrim->NumOfInstances = 1;

  /* Bind opaque light attachments */
  //std::vector<vlk::image> OpaqImages;
  for (INT i = 0; i < 6; i++)
  {
    OpaqAttach[i]->RewriteImage(Rnd->OpaqDefferedImg[i]);
    //OpaqImages.push_back(Rnd->OpaqDefferedImg[i]);
  }
  OpaqAttach[6]->RewriteImage(Rnd->OpaqDefferedImg[7]);
  //OpaqImages.push_back(Rnd->OpaqDefferedImg[7]);

  std::vector<std::pair<VkImageView, VkImageLayout>> OpaqImages, TransImages;
  for (INT i = 0; i < 6; i++)
  {
    OpaqAttach[i]->RewriteImage(Rnd->OpaqDefferedImg[i]);
    OpaqImages.push_back({Rnd->OpaqDefferedImg[i].View, Rnd->OpaqDefferedImg[i].BindLayout});
  }
  OpaqAttach[6]->RewriteImage(Rnd->OpaqDefferedImg[7]);
  OpaqImages.push_back({Rnd->OpaqDefferedImg[7].View, Rnd->OpaqDefferedImg[7].BindLayout});

  if (OpaqueLightScreenPrim && OpaqueLightScreenPrim->Mtl)
    OpaqueLightScreenPrim->Mtl->MtlDescriptorSet.Update(OpaqImages, {&LightsBuf->VlkBuffer});

  /* Prepare light trans */
  material_pattern trans_mtl_pat_lig = Rnd->MaterialPatternCreate(Rnd->ShaderCreate("light/tran"), "trans_light_material_pattern", 0, 1,
    {}, {}, {}, vlk::topology::POINTS, vlk::blend_mode::eTRANSPARENT, 0, 7);
  for (INT i = 0; i < 6; i++)
    TransAttach.push_back(Rnd->TextureCreate("Light trans attachment #" + std::to_string(i), Rnd->TransDefferedImg[i]));
  TransAttach.push_back(Rnd->TextureCreate("Light trans attachment #" + std::to_string(7), Rnd->TransDefferedImg[7]));
  material trans_mtl_lig = Rnd->MaterialCreate("light_trans_material", trans_mtl_pat_lig, {}, {LightsBuf}, OpaqAttach);
  TransLightScreenPrim = Rnd->PrimitiveCreate(trans_mtl_lig, std::span<vec3>(V.data(), 3), {});
  TransLightScreenPrim->NumOfInstances = 1;

  /* Bind transparent light attachments */
  //std::vector<vlk::image> TransImages;
  for (INT i = 0; i < 6; i++)
  {
    TransAttach[i]->RewriteImage(Rnd->TransDefferedImg[i]);
    TransImages.push_back({Rnd->TransDefferedImg[i].View, Rnd->TransDefferedImg[i].BindLayout});
  }
  TransAttach[6]->RewriteImage(Rnd->TransDefferedImg[7]);
  TransImages.push_back({Rnd->TransDefferedImg[7].View, Rnd->TransDefferedImg[7].BindLayout});

  if (TransLightScreenPrim && TransLightScreenPrim->Mtl)
    TransLightScreenPrim->Mtl->MtlDescriptorSet.Update(TransImages, {&LightsBuf->VlkBuffer});

  /* Merge prepetaion */

  /* Opaque color */
  MergeTex = Rnd->TextureCreate("MergeTex", Rnd->OpaqDefferedImg[6]);
  MergeAttach.push_back(MergeTex);

  /* Trans color */
  TransMergeTex = Rnd->TextureCreate("TransMergeTex", Rnd->TransDefferedImg[6]);
  MergeAttach.push_back(TransMergeTex);

  /* Opaque depth */
  OpaqDepth = Rnd->TextureCreate("OpaqDepth", Rnd->OpaqDefferedImg[7]);
  MergeAttach.push_back(OpaqDepth);

  /* Trans depth */
  TransDepth = Rnd->TextureCreate("TransDepth", Rnd->TransDefferedImg[7]);
  MergeAttach.push_back(TransDepth);

  /* Bind attachments */
  material_pattern MtlPat = Rnd->MaterialPatternCreate(Rnd->ShaderCreate("merge"), "Core mtl_pat", 0, 0, {}, {}, {}, vlk::topology::POINTS, vlk::blend_mode::eMERGE, 0, 4);
  material Mtl = Rnd->MaterialCreate("Core mtl", MtlPat, {}, {}, MergeAttach);
  vertex_std V1 = {vec3(0, 0, 0), vec2(0, 0), vec3(0, 0, -1), vec4(1, 0, 1, 1)};
  FullScreen = Rnd->PrimitiveCreate(Mtl, std::span<vertex_std>(&V1, 1), std::span<INT>({}));
  FullScreen->NumOfInstances = 1;
  if (FullScreen && FullScreen->Mtl)
    FullScreen->Mtl->MtlDescriptorSet.Update(
      {
        {MergeTex->Img.View, MergeTex->Img.BindLayout}, 
        {OpaqDepth->Img.View, OpaqDepth->Img.BindLayout},
        {TransMergeTex->Img.View, TransMergeTex->Img.BindLayout},
        {TransDepth->Img.View, TransDepth->Img.BindLayout}
      });

  /* Post processing preperation */

  /* Opaque color */
  PostProcTex = Rnd->TextureCreate("PostProcTex", Rnd->OpaqDefferedImg[6]);
  MergeAttach.push_back(MergeTex);
  PostProcAttach.push_back(PostProcTex);
  /* Bind attachments */
  material_pattern PostProcMtlPat = Rnd->MaterialPatternCreate(Rnd->ShaderCreate("post_processing"), "Post processing mtl_pat", 1, 0, {}, {}, {}, vlk::topology::POINTS, vlk::blend_mode::ePOSTPROC, 0, 0);
  material PostProcMtl = Rnd->MaterialCreate("Post processing mtl", PostProcMtlPat, PostProcAttach, {});
  vertex_std V2 = {vec3(0, 0, 0), vec2(0, 0), vec3(0, 0, -1), vec4(1, 0, 1, 1)};
  PostProc = Rnd->PrimitiveCreate(PostProcMtl, std::span<vertex_std>(&V2, 1), std::span<INT>({}));
  PostProc->NumOfInstances = 1;

  PostProcView.push_back(PostProcTex->Img.View);
  PostProcSampler.push_back(PostProcTex->Sampler);

  if (PostProc && PostProc->Mtl)
    PostProc->Mtl->MtlDescriptorSet.Update({}, PostProcView, PostProcSampler);

  /* Fill clear values */
  for (UINT i = 0; i < 7; i++)
    ClearValues[i].color = {{0.0f, 0.0f, 0.0f, 0.0f}};
  ClearValues[7].depthStencil = render_config::DepthFar;

  /* Fill merge clear values*/
  MergeClearValues[0].color = render_config::MergeBackground;
  MergeClearValues[1].depthStencil = render_config::DepthFar;
  MergeClearValues[2].color = render_config::MergeBackground;
  MergeClearValues[3].depthStencil = render_config::DepthFar;
  MergeClearValues[4].color = render_config::MergeBackground;

  return TRUE;
} /* End of 'tam::render_pipeline::InitPipeline' function */

/* Close render pipeline function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::render_pipeline::ClosePipeline( VOID )
{
  /* Create default null vulkan image */
  vlk::image NullImg(Rnd);
  NullImg.View = VK_NULL_HANDLE;
  NullImg.Image = VK_NULL_HANDLE;

  /* Clear merge attachments*/
  MergeTex->RewriteImage(NullImg);
  TransMergeTex->RewriteImage(NullImg);
  OpaqDepth->RewriteImage(NullImg);
  TransDepth->RewriteImage(NullImg);
  PostProcTex->RewriteImage(NullImg);

  /* Clear deffered attachments*/
  for (INT i = 0; i < 7; i++)
    OpaqAttach[i]->RewriteImage(NullImg);
  for (INT i = 0; i < 7; i++)
    TransAttach[i]->RewriteImage(NullImg);
} /* End of 'tam::render_pipeline::ClosePipeline' function */

/* Begin deferred render pass and execute subpasses function.
 * ARGUMENTS:
 *   - command buffer:
 *       VkCommandBuffer Cmd;
 *   - framebuffer:
 *       VkFramebuffer Fb;
 *   - render area extent:
 *       VkExtent2D Extent;
 * RETURNS: None.
 */
VOID tam::render_pipeline::OpaquePass( VkCommandBuffer Cmd, VkFramebuffer Fb, VkExtent2D Extent )
{
  VkRenderPassBeginInfo BeginInfo;

  BeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  BeginInfo.pNext = nullptr;

  BeginInfo.renderPass = Rnd->OpaqueRenderPass.RenderPass;

  BeginInfo.framebuffer = Fb;
  BeginInfo.renderArea = { .offset = {0, 0}, .extent = Extent };
  BeginInfo.clearValueCount = static_cast<UINT>(ClearValues.size());
  BeginInfo.pClearValues = ClearValues.data();

  vkCmdBeginRenderPass(Cmd, &BeginInfo, VK_SUBPASS_CONTENTS_INLINE);

  /* Subpass 0: G-Buffer geometry draw */
  DrawQueuedPrims(vlk::blend_mode::eOPAQUE);
  Rnd->DrawAllMarkers();

  vkCmdNextSubpass(Cmd, VK_SUBPASS_CONTENTS_INLINE);

  /* Subpass 1: fullscreen lighting, read input attachments */
  OpaqueLightPassDraw();

  vkCmdEndRenderPass(Cmd);
} /* End of 'tam::render_pipeline::OpaquePass' function */

/* Begin deferred render pass and execute subpasses function.
 * ARGUMENTS:
 *   - command buffer:
 *       VkCommandBuffer Cmd;
 *   - framebuffer:
 *       VkFramebuffer Fb;
 *   - render area extent:
 *       VkExtent2D Extent;
 * RETURNS: None.
 */
VOID tam::render_pipeline::TransparentPass( VkCommandBuffer Cmd, VkFramebuffer Fb, VkExtent2D Extent )
{
  VkRenderPassBeginInfo BeginInfo;

  BeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  BeginInfo.pNext = nullptr;

  BeginInfo.renderPass = Rnd->TransRenderPass.RenderPass;

  BeginInfo.framebuffer = Fb;
  BeginInfo.renderArea = { .offset = {0, 0}, .extent = Extent };
  BeginInfo.clearValueCount = static_cast<UINT>(ClearValues.size());
  BeginInfo.pClearValues = ClearValues.data();

  vkCmdBeginRenderPass(Cmd, &BeginInfo, VK_SUBPASS_CONTENTS_INLINE);

  /* Subpass 0: G-Buffer geometry draw */
  DrawQueuedPrims(vlk::blend_mode::eTRANSPARENT);

  vkCmdNextSubpass(Cmd, VK_SUBPASS_CONTENTS_INLINE);

  /* Subpass 1: fullscreen lighting, read input attachments */
  TransLightPassDraw();

  vkCmdEndRenderPass(Cmd);
} /* End of 'tam::render_pipeline::TransparentPass' function */

/* Begin shadow render pass and execute subpasses function.
 * ARGUMENTS:
 *   - command buffer:
 *       VkCommandBuffer Cmd;
 *   - framebuffer:
 *       VkFramebuffer Fb;
 *   - render area extent:
 *       VkExtent2D Extent;
 * RETURNS: None.
 */
VOID tam::render_pipeline::ShadowPass( VkCommandBuffer Cmd, VkFramebuffer Fb, VkExtent2D Extent )
{
  VkRenderPassBeginInfo BeginInfo
  {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    .pNext = nullptr,
    .renderPass = Rnd->ShadowRenderPass.RenderPass,
    .framebuffer = Fb,
    .renderArea = {.offset = {0, 0}, .extent = Extent },
    .clearValueCount = 1U,
    .pClearValues = &ClearValues[7],
  };

  vkCmdBeginRenderPass(Cmd, &BeginInfo, VK_SUBPASS_CONTENTS_INLINE);

  /* Subpass 0: Depth write */
  /*
  for (light in stock)
  {
    if (light.IsShadowEnable)
    {
      vkCmdBeginRenderPass(ShadowPass[light]);
      vkCmdBindPipeline(...shadow_depth...);
      // set view/projection from light
      draw_scene_for_shadow(light);
      vkCmdEndRenderPass(ShadowPass[light]);
    }
  }
  Barrier: all shadow depth -> VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
  VKPipelineBarrier
  */
  vkCmdEndRenderPass(Cmd);
} /* End of 'tam::render_pipeline::ShadowPass' function */

/* Begin merge render pass and execute subpasses function.
 * ARGUMENTS:
 *   - command buffer:
 *       VkCommandBuffer Cmd;
 *   - framebuffer:
 *       VkFramebuffer Fb;
 *   - render area extent:
 *       VkExtent2D Extent;
 * RETURNS: None.
 */
VOID tam::render_pipeline::MergePass( VkCommandBuffer Cmd, VkFramebuffer Fb, VkExtent2D Extent )
{
  VkRenderPassBeginInfo BeginInfo
  {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    .pNext = nullptr,
    .renderPass = Rnd->MergeRenderPass.RenderPass,
    .framebuffer = Fb,
    .renderArea = {.offset = {0, 0}, .extent = Extent },
    .clearValueCount = 5,
    .pClearValues = MergeClearValues.data(),
  };

  vkCmdBeginRenderPass(Cmd, &BeginInfo, VK_SUBPASS_CONTENTS_INLINE);

  /* Draw fullscreen primitive of final attachment */
  FullScreen->Mtl->MtlPat->Apply();
  FullScreen->Mtl->Apply();
  DrawInstant(FullScreen);

  vkCmdEndRenderPass(Cmd);
} /* End of 'tam::render_pipeline::MergePass' function */

/* Begin post processing render pass and execute subpasses function.
 * ARGUMENTS:
 *   - command buffer:
 *       VkCommandBuffer Cmd;
 *   - framebuffer:
 *       VkFramebuffer Fb;
 *   - render area extent:
 *       VkExtent2D Extent;
 * RETURNS: None.
 */
VOID tam::render_pipeline::PostProcessingPass( VkCommandBuffer Cmd, VkFramebuffer Fb, VkExtent2D Extent )
{
  VkRenderPassBeginInfo BeginInfo
  {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    .pNext = nullptr,
    .renderPass = Rnd->PostProcRenderPass.RenderPass,
    .framebuffer = Fb,
    .renderArea = {.offset = {0, 0}, .extent = Extent },
    .clearValueCount = 2,
    .pClearValues = ClearValues.data(),
  };

  //vkDeviceWaitIdle(Rnd->Device);

  vkCmdBeginRenderPass(Cmd, &BeginInfo, VK_SUBPASS_CONTENTS_INLINE);

  /* Draw fullscreen primitive of final attachment */
  PostProc->Mtl->MtlPat->Apply();
  PostProc->Mtl->Apply();
  DrawInstant(PostProc);

  /* Render UI */
  Rnd->RenderUI();
  Rnd->DrawUI();

  vkCmdEndRenderPass(Cmd);
} /* End of 'tam::render_pipeline::PostProcessingPass' function */

/* Resize render pass frame buffers function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::render_pipeline::Resize( VOID )
{
  /* Resize merge attachments */

  /* Resize opaque color */
  MergeTex->RewriteImage(Rnd->OpaqDefferedImg[6]);
  MergeTex = Rnd->TextureCreate("MergeTex", Rnd->OpaqDefferedImg[6]);

  /* Resize trans color */
  TransMergeTex->RewriteImage(Rnd->TransDefferedImg[6]);
  TransMergeTex = Rnd->TextureCreate("TransMergeTex", Rnd->TransDefferedImg[6]);

  /* Resize opaque depth */
  OpaqDepth->RewriteImage(Rnd->OpaqDefferedImg[7]);
  OpaqDepth = Rnd->TextureCreate("OpaqDepth", Rnd->OpaqDefferedImg[7]);

  /* Resize trans depth */
  TransDepth->RewriteImage(Rnd->TransDefferedImg[7]);
  TransDepth = Rnd->TextureCreate("TransDepth", Rnd->TransDefferedImg[7]);

  /* Resize trans depth */
  PostProcTex->RewriteImage(Rnd->MergeImg[4]);
  PostProcTex = Rnd->TextureCreate("PostProcTex", Rnd->MergeImg[4]);

  /* Bind attachments */
  if (FullScreen && FullScreen->Mtl)
    FullScreen->Mtl->MtlDescriptorSet.Update(
      {
        {MergeTex->Img.View, MergeTex->Img.BindLayout},
        {OpaqDepth->Img.View, OpaqDepth->Img.BindLayout},
        {TransMergeTex->Img.View, TransMergeTex->Img.BindLayout},
        {TransDepth->Img.View, TransDepth->Img.BindLayout}
      });

  PostProcView[0] = PostProcTex->Img.View;
  PostProcSampler[0] = PostProcTex->Sampler;

  /* Bind attachments */
  if (PostProc && PostProc->Mtl)
    PostProc->Mtl->MtlDescriptorSet.Update({}, PostProcView, PostProcSampler);

  /* Resize deffered attachments */

  /* Resize opaque color */
  std::vector<std::pair<VkImageView, VkImageLayout>> OpaqImages;
  for (INT i = 0; i < 6; i++)
  {
    OpaqAttach[i]->RewriteImage(Rnd->OpaqDefferedImg[i]);
    OpaqImages.push_back({Rnd->OpaqDefferedImg[i].View, Rnd->OpaqDefferedImg[i].BindLayout});
  }

  /* Resize opaque depth */
  OpaqAttach[6]->RewriteImage(Rnd->OpaqDefferedImg[7]);
  OpaqImages.push_back({Rnd->OpaqDefferedImg[7].View, Rnd->OpaqDefferedImg[7].BindLayout});

  /* Bind attachments */
  if (OpaqueLightScreenPrim && OpaqueLightScreenPrim->Mtl)
    OpaqueLightScreenPrim->Mtl->MtlDescriptorSet.Update(OpaqImages, {&LightsBuf->VlkBuffer});

  /* Resize trans color */
  std::vector<std::pair<VkImageView, VkImageLayout>> TransImages;
  for (INT i = 0; i < 6; i++)
  {
    TransAttach[i]->RewriteImage(Rnd->TransDefferedImg[i]);
    TransImages.push_back({Rnd->TransDefferedImg[i].View, Rnd->TransDefferedImg[i].BindLayout});
  }

  /* Resize opaque depth */
  TransAttach[6]->RewriteImage(Rnd->TransDefferedImg[7]);
  TransImages.push_back({Rnd->TransDefferedImg[7].View, Rnd->TransDefferedImg[7].BindLayout});

  /* Bind attachments */
  if (TransLightScreenPrim && TransLightScreenPrim->Mtl)
    TransLightScreenPrim->Mtl->MtlDescriptorSet.Update(TransImages, {&LightsBuf->VlkBuffer});
} /* End of 'tam::render_pipeline::Resize' function */

/* Fill lights buffer function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::render_pipeline::FillLights( VOID )
{
  const auto &Stock = Rnd->GetLights();
  if (Lights.size() != Stock.size())
    Lights.resize(Stock.size(), {}), IsLightsUpdated = FALSE;

  std::time_t t = std::time(nullptr);
  std::tm *now = std::localtime(&t);

  /*std::fmod(Rnd->Timer.Time, 24.0) * 5,*/
   //vec4 Size = vec4(Lights.size(), EnvTime, now->tm_yday + 1, IsSunEnabled);
   lighting_params_data LightParamsData
   {
     .SizeDayTimeDayOfYearPresentFlag = vec4(Lights.size(), EnvTime, now->tm_yday + 1, static_cast<INT>(Rnd->LightingMode)),
     .LightColorIntensity = vec4(Rnd->PresentLightsColor, Rnd->PresentIntensity)
   };
  LightsBuf->Update(&LightParamsData, 0, sizeof(lighting_params_data));

  if (IsLightsUpdated)
    return;
  INT i = 0;
  for (const auto &L : Stock)
  {
    Lights[i] = 
    {
        vec4(L.second->Pos, L.second->Dir.X),
        vec4(L.second->Dir.Y, L.second->Dir.Z, L.second->Color.X, L.second->Color.Y),
        vec4(L.second->Color.Z, L.second->Intensity, (FLT)L.second->Type, L.second->IsEnabled),
        vec4(L.second->Exp, L.second->CutOfAngle, 0, 0),
    };
    i++;
  }
  if (Lights.size() > 0 && LightsBuf->Size < Lights.size() * sizeof(lig_buf) + sizeof(vec4))
  {
    LightsBuf->Resize((UINT)Lights.size() * sizeof(lig_buf) + sizeof(vec4));
    OpaqueLightScreenPrim->Mtl->MtlDescriptorSet.Update({&LightsBuf->VlkBuffer}, {}, {});
    TransLightScreenPrim->Mtl->MtlDescriptorSet.Update({&LightsBuf->VlkBuffer}, {}, {});
  }

  LightsBuf->Update(Lights.data(), sizeof(lighting_params_data));
  IsLightsUpdated = TRUE;
} /* End of 'tam::render_pipeline::FillLights' fucntion */

/* Illuminate scene function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::render_pipeline::OpaqueLightPassDraw( VOID )          
{
  OpaqueLightScreenPrim->Mtl->MtlPat->Apply();
  OpaqueLightScreenPrim->Mtl->Apply();
  DrawInstant(OpaqueLightScreenPrim);
} /* End of 'tam::render_pipeline::LightPassDraw' function */
 
/* Illuminate scene function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::render_pipeline::TransLightPassDraw( VOID )          
{
  TransLightScreenPrim->Mtl->MtlPat->Apply();
  TransLightScreenPrim->Mtl->Apply();
  DrawInstant(TransLightScreenPrim);
} /* End of 'tam::render_pipeline::LightPassDraw' function */

/* END OF 'tam_render_pipeline.cpp' FILE */
