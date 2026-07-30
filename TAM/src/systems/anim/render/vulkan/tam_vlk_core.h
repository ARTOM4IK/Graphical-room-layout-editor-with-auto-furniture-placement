/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_core.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Vulkan core declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 03.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_vlk_core_h_
#define __tam_vlk_core_h_

#include "vma\vk_mem_alloc.h"

#include <vulkan.h>
#include <vulkan_win32.h>

#include "tam_def.h"
#include "tam_vlk_def.h"

#include "tam_vlk_instance.h"
#include "vulkan_debug\tam_vulkan_debug.h"
#include "tam_vlk_physdevice.h"
#include "tam_vlk_device.h"
#include "tam_vlk_surface.h"
#include "tam_vlk_renderpass.h"
#include "tam_vlk_swapchain.h"
#include "vulkan_res\framebuffers\tam_vlk_framebuffers.h"
#include "vulkan_res\images\tam_vlk_images.h"
#include "vulkan_res\shaders\tam_vlk_shaders.h"
#include "vulkan_res\pipelines\tam_vlk_pipelines.h"
#include "vulkan_res\descriptors\tam_vlk_descriptors.h"
#include "vulkan_res\buffers\tam_vlk_buffers.h"

/* 'tam' namespace */
namespace tam
{
  /* 'vlk' namespace */
  namespace vlk
  {
    /* 'core' handle class */
    class core : public project_kernel, public instance, public debug, public physdevice, public device, public surface, public swapchain
    {
    public:
      VmaAllocator Allocator {}; // Vulkan memory allocator instance

      static const UINT UsedBufferCount = 3;    // Used number of back buffers
      static const UINT UsedFramesOnTheFly = 3; // Used command buffers

      VkQueue GraphicsQueue {}; // Vulkan base graphics queue
      VkQueue PresentQueue {};  // Vulkan base present queue
      VkQueue TransferQueue {}; // Vulkan base transfer queue
      VkQueue ComputeQueue {};  // Vulkan base compute queue

      UINT GraphicsQueueFamilyIndex = 0; // Vulkan graphics queue family index
      UINT PresentQueueFamilyIndex = 0;  // Vulkan present queue family index
      UINT TransferQueueFamilyIndex = 0; // Vulkan transfer queue family index
      UINT ComputeQueueFamilyIndex = 0;  // Vulkan compute queue family index

      VkFence TransferFence {};       // Vulkan transfer queue sync fence
      std::vector<VkFence> Fences {}; // Vulkan command buffer queue sync fences (frame finalize)
      UINT ActiveSwapchainImage {};   // Active swap chain image number : 0 .. UsedBufferCount-1
      UINT CurrentFrame {};           // Current frame number: 0..UsedFramesOnTheFly-1

      renderpass ShadowRenderPass, MergeRenderPass, OpaqueRenderPass, TransRenderPass, PostProcRenderPass; // Renderpasses for render pipeline
      framebuffer OpaqueFrameBuf, TransFrameBuf, MergeFrameBuf; // Frame buffers for render pipeline
      std::vector<image> OpaqDefferedImg, TransDefferedImg, MergeImg; // Attachments for deffered frame buffers

      VkCommandPool GraphicsCommandPool {};                 // Vulkan base command pool
      VkCommandPool TransferCommandPool {};                 // Vulkan base command pool
      std::vector<VkCommandBuffer> CommandBuffers {};       // Vulkan command buffers (UsedFramesOnTheFly)
      VkCommandBuffer TransferCommandBuffer {};             // Vulkan command buffer
      BOOL IsTransferBufferActive = FALSE;                  // Transfer command buffer current usage flag
      std::vector<VkSemaphore> RenderFinishSemaphores {};   // Rendering wait semaphores (used before present)
                                                            // (UsedFramesOnTheFly)
      std::vector<VkSemaphore> ImageAvailableSemaphores {}; // Swapchain ready image waiting semaphores
                                                            // (UsedFramesOnTheFly)

      image DepthImage;     // Vulkan depth image
      buffer StagingBuffer; // Vulkan work stage buffer handle

      sampler DefSampler;   // Default sampler

      VkDescriptorPool DescriptorPool {};                 // Vulkan descriptor pool
      descriptor_set_layout FrameSyncDescriptorSetLayout; // Frame and synchronization descriptor set layout
      descriptor_set FrameSyncDescriptorSet;              // Frame and synchronization descriptor set

      VkQueryPool
        StatisticsQueryPool = VK_NULL_HANDLE, // Statistics query pool
        TimestampQueryPool = VK_NULL_HANDLE;  // Timestamps query pool
      UINT64 TimestampPeriod {};              // Timestamp period in nanoseconds

      BOOL
        IsCoreInit = FALSE,    // Vulkan core initialzation flag
        IsCoreDestroy = FALSE; // Destroy Vulkan system flag

      /* Class default constructor */
      core( VOID ) :
        project_kernel(), StagingBuffer(*this), DepthImage(this), DefSampler(this),
        FrameSyncDescriptorSetLayout(this), FrameSyncDescriptorSet(this), ShadowRenderPass(this), MergeRenderPass(this), OpaqueRenderPass(this),
        TransRenderPass(this), PostProcRenderPass(this), OpaqueFrameBuf(this), TransFrameBuf(this), MergeFrameBuf(this)
      {
        for (INT i = 0; i < 8; i++)
        {
          OpaqDefferedImg.push_back(this);
          TransDefferedImg.push_back(this);
        }

        for (INT i = 0; i < 5; i++)
          MergeImg.push_back(this);

        /* Initialize Vulkan core references */
        static_cast<vlk_ref*>(this)->Init(*this);
      } /* End of 'core' function */

      /* Class destructor */
      ~core( VOID );

      /* Init Vulkan core function.
       * ARGUMENTS:
       *   - window handle reference:
       *       HWND &hWndRef;
       *   - start window sizes:
       *       INT FrameW, FrameH;
       * RETURNS:
       *   (BOOL) TRUE if Vulkan system was initialized successfully, FALSE otherwise.
       */
      BOOL Create( HWND &hWndRef, INT FrameW, INT FrameH );

      /* Close Vulkan system function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Destroy( VOID );

      /* Prepare Vulkan system for new frame function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID FramePreStart( VOID );

      /* Start Vulkan frame function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID FrameStart( VOID );

      /* End Vulkan frame function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID FrameEnd( VOID );

      /* Create command pool function.
       * ARGUMENTS:
       *   - command buffer pool create flag bits:
       *       VkCommandPoolCreateFlags CommandPoolFlag;
       * RETURNS:
       *   (BOOL) depending on result.
       */
      BOOL CreateCommandPool( VkCommandPoolCreateFlags CommandPoolFlag );

      /* Delete command pool function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DestroyCommandPool( VOID );

      /* Create command buffer function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) depending on result.
       */
      BOOL CreateCommandBuffer( VOID );

      /* Create descriptor pool function.
       * ARGUMENTS:
       *   - numbers of different types of descriptors:
       *       UINT StorageBuffersCount, UniformBuffersCount, ImageSamplersCount;
       * RETURNS:
       *   (BOOL) TRUE if descriptor pool was successfully created, FALSE otherwise.
       */
      BOOL CreateDescriptorPool( UINT StorageBuffersCount = 1000, UINT UniformBuffersCount = 1000, UINT ImageSamplersCount = 1000, UINT InputAttachmentsCount = 100 );

      /* Destroy descriptor pool function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DestroyDescriptorPool( VOID );
    }; /* End of 'core' class */
  } /* end of 'vlk' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_vlk_core_h_ */

/* END OF 'tam_vlk_core.h' FILE */
