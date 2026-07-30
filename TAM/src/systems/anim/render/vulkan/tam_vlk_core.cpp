/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_core.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Vulkan core main functions implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 09.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

#pragma comment(lib, "vulkan-1")

/* Init Vulkan core function.
 * ARGUMENTS:
 *   - window handle reference:
 *       HWND &hWndRef;
 *   - start window sizes:
 *       INT FrameW, FrameH;
 * RETURNS:
 *   (BOOL) TRUE if Vulkan system was initialized successfully, FALSE otherwise.
 */
BOOL tam::vlk::core::Create( HWND &hWndRef, INT FrameW, INT FrameH )
{
  if (IsCoreInit)
    return TRUE;
  /* Create crucial Vulkan objects */
  if (!instance::Create())
    return FALSE;
#ifndef NDEBUG
  else if (!debug::Create())
    return FALSE;
#endif // NDEBUG
  else if (!physdevice::Create())
    return FALSE;
  else if (!surface::Create(hWndRef))
    return FALSE;
  else if (!device::Create())
    return FALSE;
  //else if (!renderpass::Create())
  // return FALSE;

  /* Create VMA allocator */
  VmaAllocatorCreateInfo AllocatorCreateInfo
  {
    .physicalDevice = PhysDevice,
    .device = Device,
    .instance = Instance,
    .vulkanApiVersion = PhysicalDeviceProperties.apiVersion,
  };
  if (vmaCreateAllocator(&AllocatorCreateInfo, &Allocator) != VK_SUCCESS)
  {
    logger::error << "Error creating VMA allocator!";
    Allocator = VK_NULL_HANDLE;
    return FALSE;
  }

  //Create renderpasses
  ShadowRenderPass.CreateShadow();
  OpaqueRenderPass.CreateDeferred();
  TransRenderPass.CreateDeferred();
  MergeRenderPass.CreateMerge();
  PostProcRenderPass.CreatePostProc();

  /* Check for swapchain support and create one if it is possible */
  if (IsSwapchain && !swapchain::Create())
    return FALSE;

  /* Create command pool and allocate transfer buffer and command buffers for every frame */
  if (!CreateCommandPool(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) || !CreateCommandBuffer())
    return FALSE;
  /* Create descriptor pool */
  if (!CreateDescriptorPool())
    return FALSE;

  /* Create staging buffer for data transfer */
  StagingBuffer.Create(10000, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_SHARING_MODE_EXCLUSIVE);

  /* Create default sampler */
  DefSampler.Create();

  /* Create transfer queue fence */
  VkFenceCreateInfo TransferFenceInfo
  {
    .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
    .flags = VK_FENCE_CREATE_SIGNALED_BIT
  };
  if ((vkCreateFence(Device, &TransferFenceInfo, nullptr, &TransferFence)) != VK_SUCCESS)
  {
    logger::error << "Error creating transfer queue fence!";
    return FALSE;
  }

  /* Create fences and semaphores for every frame in swapchain */
  Fences.resize(UsedFramesOnTheFly);
  RenderFinishSemaphores.resize(UsedFramesOnTheFly);
  ImageAvailableSemaphores.resize(UsedFramesOnTheFly);
  for (INT i = 0; i < UsedFramesOnTheFly; i++)
  {
    static VkFenceCreateInfo FenceInfo
    {
      .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
      .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };
    static VkSemaphoreCreateInfo SemaforeCreateInfo
    {
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };
    if ((vkCreateFence(Device, &FenceInfo, nullptr, &Fences[i])) != VK_SUCCESS)
    {
      logger::error << "Unable to create frame buffer synchronization fence!";
      return FALSE;
    }
    if ((vkCreateSemaphore(Device, &SemaforeCreateInfo, nullptr, &RenderFinishSemaphores[i])) != VK_SUCCESS ||
        (vkCreateSemaphore(Device, &SemaforeCreateInfo, nullptr, &ImageAvailableSemaphores[i])) != VK_SUCCESS)
    {
      logger::error << "Unable to create semaphores for swapchain frame buffer!";
      return FALSE;
    }
  }

  /* Create global descriptor set and it's layout */
  // 0 - sync buffer
  // 1 - camera buffer
  // 2 - world matrices buffer
  // 3 - WVP and Winv matrices buffer
  // 4 - buffer of opaque models' id and final buffer
  // 5 - buffer of transparent models' id
  std::vector<VkDescriptorSetLayoutBinding> FrameSyncDescriptorSetLayoutBinding {};

  for (UINT i = 0; i < 6; i++)
    FrameSyncDescriptorSetLayoutBinding.push_back(VkDescriptorSetLayoutBinding
    {
      .binding = i,
      .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
      .descriptorCount = 1,
      .stageFlags = VK_SHADER_STAGE_ALL
    });
  if (!FrameSyncDescriptorSetLayout.Create(FrameSyncDescriptorSetLayoutBinding))
  {
    logger::error << "Error creating descriptor set layout for frame, synchronization, matrices and id's buffers.";
    return FALSE;
  }
  if (!FrameSyncDescriptorSet.Create(FrameSyncDescriptorSetLayout, {}, {}, {}))
  {
    logger::error << "Error creating descriptor set for frame, synchronization, matrices and id's buffers.";
    return FALSE;
  }
  ActiveSwapchainImage = 0;
  IsCoreInit = TRUE;
  return TRUE;
} /* End of 'tam::vlk::core::Create' function */

/* Close Vulkan system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::core::Destroy( VOID )
{
  if (IsCoreDestroy)
    return;
  if (Device != VK_NULL_HANDLE)
  {
    vkDeviceWaitIdle(Device);

    for (INT i = 0; i < UsedFramesOnTheFly; i++)
    {
      if (ImageAvailableSemaphores[i] != VK_NULL_HANDLE)
        vkDestroySemaphore(Device, ImageAvailableSemaphores[i], nullptr);
      if (RenderFinishSemaphores[i] != VK_NULL_HANDLE)
        vkDestroySemaphore(Device, RenderFinishSemaphores[i], nullptr);
      if (Fences[i] != VK_NULL_HANDLE)
        vkDestroyFence(Device, Fences[i], nullptr);
      RenderFinishSemaphores[i] = ImageAvailableSemaphores[i] = VK_NULL_HANDLE;
      Fences[i] = VK_NULL_HANDLE;
    }

    vkDestroyFence(Device, TransferFence, nullptr);
    DefSampler.Destroy();
    StagingBuffer.Destroy();
    DestroyDescriptorPool();
    DestroyCommandPool();
    //renderpass::Destroy();
    DepthImage.Destroy();
    swapchain::Destroy();
    ShadowRenderPass.Destroy();
    OpaqueRenderPass.Destroy();
    TransRenderPass.Destroy();
    MergeRenderPass.Destroy();
    PostProcRenderPass.Destroy();
  }
  if (Allocator != VK_NULL_HANDLE)
  {
    vmaDestroyAllocator(Allocator);
    Allocator = VK_NULL_HANDLE;
  }
  device::Destroy();
  surface::Destroy();
#ifndef NDEBUG
  debug::Destroy();
#endif // NDEBUG
  instance::Destroy();
  IsCoreDestroy = TRUE;
  IsCoreInit = FALSE;
} /* End of 'tam::vlk::core::Destroy' function */

/* Class destructor */
tam::vlk::core::~core( VOID )
{
  Destroy();
} /* End of 'tam::vlk::core::~core' function */

/* Prepare Vulkan system for new frame function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::core::FramePreStart( VOID )
{
  if (!IsCoreInit)
    return;
  // Queue sync
  vkWaitForFences(Device, 1, &Fences[CurrentFrame], VK_TRUE, UINT64_MAX);
  vkResetFences(Device, 1, &Fences[CurrentFrame]);
 
  vkAcquireNextImageKHR(Device, Swapchain, UINT64_MAX,
                        ImageAvailableSemaphores[CurrentFrame], VK_NULL_HANDLE, &ActiveSwapchainImage);
  SystemProfiler.Collect("Waiting for fences");

  vkResetCommandBuffer(CommandBuffers[CurrentFrame], 0);
  VkCommandBufferBeginInfo BeginInfo
  {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .flags = 0,
    .pInheritanceInfo = nullptr,
  };
  if (vkBeginCommandBuffer(CommandBuffers[CurrentFrame], &BeginInfo) != VK_SUCCESS)
  {
    logger::error << "Error begining command buffer work!";
    return;
  }
} /* End of 'tam::vlk::core::FramePreStart' function */

/* Start Vulkan frame function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::core::FrameStart( VOID )
{
  if (!IsCoreInit)
    return;
  static std::array<VkClearValue, 2> ClearValues 
  {
    VkClearValue {.depthStencil {.depth = 1.0f}},
    VkClearValue {.color {.float32 {0.30f, 0.47f, 0.8f, 1.0f}}},
  };
  //VkRenderPassBeginInfo Info
  //{
  //  .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
  //  .renderPass = CoreRenderPass.RenderPass,
  //  .framebuffer = FrameBuffers[ActiveSwapchainImage],
  //  .renderArea =
  //  {
  //    .offset =
  //    {
  //      .x = 0,
  //      .y = 0
  //    },
  //    .extent =
  //    {
  //      .width = SwapchainInfo.imageExtent.width,
  //      .height = SwapchainInfo.imageExtent.height,
  //    }
  //  },
  //  .clearValueCount = static_cast<UINT32>(ClearValues.size()),
  //  .pClearValues = ClearValues.data()
  //};

  //vkCmdBeginRenderPass(CommandBuffers[CurrentFrame], &Info, VK_SUBPASS_CONTENTS_INLINE);
  SystemProfiler.Collect("Begin core render pass");

  VkViewport Viewport
  {
    .x = 0.0f,
    .y = (FLT)SwapchainInfo.imageExtent.height,
    .width = (FLT)SwapchainInfo.imageExtent.width,
    .height = -(FLT)SwapchainInfo.imageExtent.height,
    .minDepth = 0.0f,
    .maxDepth = 1.0f,
  };
  vkCmdSetViewport(CommandBuffers[CurrentFrame], 0, 1, &Viewport);
  VkRect2D Scissor
  {
    .offset = {0, 0},
    .extent = SwapchainInfo.imageExtent,
  };
  vkCmdSetScissor(CommandBuffers[CurrentFrame], 0, 1, &Scissor);
} /* End of 'tam::vlk::core::FrameStart' function */

/* End Vulkan frame function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::core::FrameEnd( VOID )
{
  if (!IsCoreInit)
    return;
  //vkCmdEndRenderPass(CommandBuffers[CurrentFrame]);
  //SystemProfiler.Collect("EndRenderPass");


  //vkCmdDraw(CommandBuffers[CurrentFrame], 1, 1, 0, 0);
  //vkCmdEndRenderPass(CommandBuffers[CurrentFrame]);
  //vkCmdPipelineBarrier(CommandBuffers[CurrentFrame], );

  vkEndCommandBuffer(CommandBuffers[CurrentFrame]);
  SystemProfiler.Collect("EndCommandBuffer");

  VkPipelineStageFlags WaitPipelineStageFlags[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  VkSemaphore WaitSemaphores[] = {ImageAvailableSemaphores[CurrentFrame]};
  VkSemaphore SignalSemaphores[] = {RenderFinishSemaphores[ActiveSwapchainImage]};
  VkSubmitInfo SubmitInfo
  {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .waitSemaphoreCount = 1,
    .pWaitSemaphores = WaitSemaphores,
    .pWaitDstStageMask = WaitPipelineStageFlags,
    .commandBufferCount = 1,
    .pCommandBuffers = &CommandBuffers[CurrentFrame],
    .signalSemaphoreCount = 1,
    .pSignalSemaphores = SignalSemaphores,
  };
  vkQueueSubmit(GraphicsQueue, 1, &SubmitInfo, Fences[CurrentFrame]);
  SystemProfiler.Collect("GraphicsQueueSubmit");
 
  VkPresentInfoKHR PresentInfo
  {
    .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    .waitSemaphoreCount = 1,
    .pWaitSemaphores = SignalSemaphores,
    .swapchainCount = 1,
    .pSwapchains = &Swapchain,
    .pImageIndices = &ActiveSwapchainImage,
    .pResults = nullptr
  };

  vkQueuePresentKHR(PresentQueue, &PresentInfo);
  SystemProfiler.Collect("QueuePresent");
  CurrentFrame = (CurrentFrame + 1) % UsedFramesOnTheFly;
  SystemProfiler.Collect("End frame");
} /* End of 'tam::vlk::core::FrameEnd' function */

/* Create descriptor pool function.
 * ARGUMENTS:
 *   - numbers of different types of descriptors:
 *       UINT StorageBuffersCount, UniformBuffersCount, ImageSamplersCount;
 * RETURNS:
 *   (BOOL) TRUE if descriptor pool was successfully created, FALSE otherwise.
 */
BOOL tam::vlk::core::CreateDescriptorPool( UINT StorageBuffersCount, UINT UniformBuffersCount, UINT ImageSamplersCount, UINT InputAttachmentsCount )
{
  std::array<VkDescriptorPoolSize, 4> PoolSizes {};

  PoolSizes[0].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  PoolSizes[0].descriptorCount = StorageBuffersCount;

  PoolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  PoolSizes[1].descriptorCount = UniformBuffersCount;

  PoolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  PoolSizes[2].descriptorCount = ImageSamplersCount;

  PoolSizes[3].type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
  PoolSizes[3].descriptorCount = InputAttachmentsCount;

  VkDescriptorPoolCreateInfo DescriptorPoolCreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
    .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
    .maxSets = 10000,
    .poolSizeCount = (UINT)PoolSizes.size(),
    .pPoolSizes = PoolSizes.data(),
  };

  if (vkCreateDescriptorPool(Device, &DescriptorPoolCreateInfo, nullptr, &DescriptorPool) != VK_SUCCESS)
  {
    logger::error << "Unable to create descriptor pool.";
    return FALSE;
  }
  logger::info << std::format("Descriptor pool successfully created:");
  logger::info << std::format("Number of storage buffers: {}", StorageBuffersCount);
  logger::info << std::format("Number of uniform buffers: {}", UniformBuffersCount);
  logger::info << std::format("Number of image samplers: {}", ImageSamplersCount);
  logger::info << std::format("Number of image samplers: {}", InputAttachmentsCount);
  return TRUE;
} /* End of 'tam::vlk::core::CreateDescriptorPool' function */

/* Destroy descriptor pool function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::core::DestroyDescriptorPool( VOID )
{
  vkDestroyDescriptorPool(Device, DescriptorPool, nullptr);
  DescriptorPool = VK_NULL_HANDLE;
} /* End of 'tam::vlk::core::DestroyDescriptorPool' function */

/* END OF 'tam_vlk_core.cpp' FILE */
