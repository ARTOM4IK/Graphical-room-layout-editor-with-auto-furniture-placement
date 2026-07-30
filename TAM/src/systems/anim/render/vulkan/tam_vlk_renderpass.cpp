/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

 /* FILE NAME   : tam_vlk_renderpass.cpp
  * PURPOSE     : Tough Arrangement Manager project.
  *               Vulkan system module.
  *               Renderpass function implementation file.
  * PROGRAMMER  : CGSG'Sr'2025.
  *               Tretyakov Andrey (AT7).
  * LAST UPDATE : 21.03.2026
  * NOTE        : Namespace 'tam'.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include "tam.h"

/* Class constructor for render pipeline only */
tam::vlk::renderpass::renderpass( core* Ptr ) : VlkRef(Ptr)
{
} /* End of 'renderpass' function */

/* Create renderpass function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) TRUE if renderpass was created, FALSE otherwise.
 */
BOOL tam::vlk::renderpass::Create( VOID )
{
  /* Create render pass with one attachment and depth */
  VkAttachmentDescription DepthAttachmentDescription
  {
    .format = VK_FORMAT_D32_SFLOAT,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
    .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
  };
  VkAttachmentDescription AttachmentDescription
  {
    .flags = 0,
    .format = VlkRef->SwapchainImageFormat,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
  };

  VkAttachmentReference DepthStencilAttachmentReference
  {
    .attachment = 0,
    .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
  };
  VkAttachmentReference ColorAttachmentReference
  {
    .attachment = 1,
    .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
  };

  SubpassDescriptions =
  {
    VkSubpassDescription
    {
      .flags = 0,
      .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
      .inputAttachmentCount = 0,
      .pInputAttachments = nullptr,
      .colorAttachmentCount = 1,
      .pColorAttachments = &ColorAttachmentReference,
      .pResolveAttachments = nullptr,
      .pDepthStencilAttachment = &DepthStencilAttachmentReference,
      .preserveAttachmentCount = 0,
      .pPreserveAttachments = nullptr,
    }
  };

  SubpassDependencies =
  {
    VkSubpassDependency
    {
      .srcSubpass = VK_SUBPASS_EXTERNAL,
      .dstSubpass = 0,
      .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      .srcAccessMask = 0,
      .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      .dependencyFlags = 0,
    }
  };

  AttachmentDescriptions = { DepthAttachmentDescription, AttachmentDescription };
  VkRenderPassCreateInfo CreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .attachmentCount = static_cast<uint32_t>(AttachmentDescriptions.size()),
    .pAttachments = AttachmentDescriptions.data(),
    .subpassCount = static_cast<UINT>(SubpassDescriptions.size()),
    .pSubpasses = SubpassDescriptions.data(),
    .dependencyCount = static_cast<UINT>(SubpassDependencies.size()),
    .pDependencies = SubpassDependencies.data(),
  };

  if (vkCreateRenderPass(VlkRef->Device, &CreateInfo, nullptr, &RenderPass) != VK_SUCCESS)
  {
    logger::error << "Error creating render pass!";
    return FALSE;
  }
  logger::info << "Render pass successfully created.";

  return TRUE;
} /* End of 'tam::vlk::renderpass::Create' function */

/* Create shadow renderpass with one subpass function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) TRUE if renderpass was created, FALSE otherwise.
 */
BOOL tam::vlk::renderpass::CreateShadow( VOID )
{
  constexpr UINT DepthIdx = 0;

  AttachmentDescriptions.push_back(VkAttachmentDescription
  {
    .flags = 0,
    .format = VK_FORMAT_D32_SFLOAT,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
    .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
  });

  /* Output depth (to shader) */
  VkAttachmentReference DepthRef
  {
    .attachment = DepthIdx,
    .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
  };

  /* Subpass descriptions */
  SubpassDescriptions =
  {
    VkSubpassDescription
    {
      .flags = 0,
      .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
      .inputAttachmentCount = 0,
      .pInputAttachments = nullptr,
      .colorAttachmentCount = 0,
      .pColorAttachments = nullptr,
      .pResolveAttachments = nullptr,
      .pDepthStencilAttachment = &DepthRef,
      .preserveAttachmentCount = 0,
      .pPreserveAttachments = nullptr,
    }
  };

  /* Subpass dependencies */
  SubpassDependencies =
  {
    /* External -> Subpass 0 */
    VkSubpassDependency
    {
      .srcSubpass = VK_SUBPASS_EXTERNAL,
      .dstSubpass = 0,
      .srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      .srcAccessMask = VK_ACCESS_MEMORY_READ_BIT,
      .dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
    },
    /* Subpass 0 -> External */
    VkSubpassDependency
    {
      .srcSubpass = 0,
      .dstSubpass = VK_SUBPASS_EXTERNAL,
      .srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      .srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
      .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
    },
  };

  /* Create render pass */
  VkRenderPassCreateInfo CreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .attachmentCount = static_cast<UINT>(AttachmentDescriptions.size()),
    .pAttachments = AttachmentDescriptions.data(),
    .subpassCount = static_cast<UINT>(SubpassDescriptions.size()),
    .pSubpasses = SubpassDescriptions.data(),
    .dependencyCount = static_cast<UINT>(SubpassDependencies.size()),
    .pDependencies = SubpassDependencies.data(),
  };

  if (vkCreateRenderPass(VlkRef->Device, &CreateInfo, nullptr, &RenderPass) != VK_SUCCESS)
  {
    logger::error << "Error creating shadow render pass!";
    return FALSE;
  }
  logger::info << "Shadow render pass successfully created.";

  return TRUE;
} /* End of 'tam::vlk::renderpass::CreateShadow' function */

/* Create merge renderpass with one subpass function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) TRUE if renderpass was created, FALSE otherwise.
 */
BOOL tam::vlk::renderpass::CreateMerge( VOID )
{
  constexpr UINT ColorCount = 1;
  constexpr UINT InputColorCount = 1;

  // Input
  AttachmentDescriptions.push_back(VkAttachmentDescription
  {
    .flags = 0,
    .format = VK_FORMAT_R16G16B16A16_SFLOAT,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,  
    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, /// VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
  });

  // Input
  AttachmentDescriptions.push_back(VkAttachmentDescription
  {
    .flags = 0,
    .format = VK_FORMAT_R16G16B16A16_SFLOAT,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,  
    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
  });

  // Input
  AttachmentDescriptions.push_back(VkAttachmentDescription
  {
    .flags = 0,
    .format = VK_FORMAT_D32_SFLOAT,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,  
    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL/*VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL*/,
    .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
  });

  // Input
  AttachmentDescriptions.push_back(VkAttachmentDescription
  {
    .flags = 0,
    .format = VK_FORMAT_D32_SFLOAT,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,  
    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL/*VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL*/,
    .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
  });

  // Output
  AttachmentDescriptions.push_back(VkAttachmentDescription
  {
    .flags = 0,
    .format = VK_FORMAT_R16G16B16A16_SFLOAT,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,  
    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .finalLayout = /*VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL*/VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL/*VK_IMAGE_LAYOUT_PRESENT_SRC_KHR*/,
  });

  AttachmentReferences.push_back(VkAttachmentReference
  {
    .attachment = 0,
    .layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
  });

  AttachmentReferences.push_back(VkAttachmentReference
  {
    .attachment = 1,
    .layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
  });

  AttachmentReferences.push_back(VkAttachmentReference
  {
    .attachment = 2,
    .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
  });

  AttachmentReferences.push_back(VkAttachmentReference
  {
    .attachment = 3,
    .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
  });

  VkAttachmentReference ColorRef
  {
    .attachment = 4,
    .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
  };

  /* Subpass descriptions */
  SubpassDescriptions =
  {
    /* Subpass 0 */
    VkSubpassDescription
    {
      .flags = 0,
      .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
      .inputAttachmentCount = 4,
      .pInputAttachments = AttachmentReferences.data(),
      .colorAttachmentCount = ColorCount,
      .pColorAttachments = &ColorRef,
      .pResolveAttachments = nullptr,
      .pDepthStencilAttachment = nullptr,
      .preserveAttachmentCount = 0,
      .pPreserveAttachments = nullptr,
    }
  };

  /* Subpass dependencies */
  SubpassDependencies =
  {
    /* External -> Subpass 0 */
    VkSubpassDependency
    {
      .srcSubpass = VK_SUBPASS_EXTERNAL,
      .dstSubpass = 0,
      .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      .dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,
      .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
    },
    /* Subpass 0 -> External: results available for sampling */
    VkSubpassDependency
    {
      .srcSubpass = 0,
      .dstSubpass = VK_SUBPASS_EXTERNAL,
      .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
      .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
    }
  };

  /* Create render pass */
  VkRenderPassCreateInfo CreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .attachmentCount = static_cast<UINT>(AttachmentDescriptions.size()),
    .pAttachments = AttachmentDescriptions.data(),
    .subpassCount = static_cast<UINT>(SubpassDescriptions.size()),
    .pSubpasses = SubpassDescriptions.data(),
    .dependencyCount = static_cast<UINT>(SubpassDependencies.size()),
    .pDependencies = SubpassDependencies.data(),
  };

  if (vkCreateRenderPass(VlkRef->Device, &CreateInfo, nullptr, &RenderPass) != VK_SUCCESS)
  {
    logger::error << "Error creating merge render pass!";
    return FALSE;
  }
  logger::info << "Merge render pass successfully created.";

  return TRUE;
} /* End of 'tam::vlk::renderpass::CreateMerge' function */

/* Create deferred renderpass with two subpasses function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) TRUE if renderpass was created, FALSE otherwise.
 */
BOOL tam::vlk::renderpass::CreateDeferred( VOID )
{
  constexpr UINT Sp0ColorCount = 6;
  constexpr UINT Sp1ColorCount = 1;
  constexpr UINT DepthIdx = Sp0ColorCount + Sp1ColorCount; /* 7 */
  constexpr UINT TotalAttachments = DepthIdx + 1;          /* 8 */

  /* Subpass 0 color attachments: output 0..5 G-Bufs to shaders */
  for (UINT i = 0; i < Sp0ColorCount; i++)
    AttachmentDescriptions.push_back(VkAttachmentDescription
    {
      .flags = 0,
      .format = VK_FORMAT_R16G16B16A16_SFLOAT,
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
      .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    });

  /* Subpass 1 color attachment [6]: output color texture */
  for (UINT i = 0; i < Sp1ColorCount; i++)
    AttachmentDescriptions.push_back(VkAttachmentDescription
    {
      .flags = 0,
      .format = VK_FORMAT_R16G16B16A16_SFLOAT,
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR, /// VK_ATTACHMENT_LOAD_OP_LOAD,
      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
      .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED, /// VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
      .finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    });

  /* Depth attachment [7]: shared, preserved on output */
  AttachmentDescriptions.push_back(VkAttachmentDescription
  {
    .flags = 0,
    .format = VK_FORMAT_D32_SFLOAT,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR, //VK_ATTACHMENT_LOAD_OP_LOAD,
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
  });

  /* Subpass 0 attachment references: 6 color outputs */
  std::array<VkAttachmentReference, Sp0ColorCount> Sp0ColorRefs{};
  for (UINT i = 0; i < Sp0ColorCount; i++)
    Sp0ColorRefs[i] = VkAttachmentReference
    {
      .attachment = i,
      .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

  /* Subpass 0 depth output */
  VkAttachmentReference Sp0DepthRef
  {
    .attachment = DepthIdx,
    .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
  };

  /* Subpass 1 input references (6 color + 1 depth): read G-Buffer from subpass 0 */
  std::array<VkAttachmentReference, Sp0ColorCount + 1> Sp1InputRefs{};
  for (UINT i = 0; i < Sp0ColorCount; i++)
    Sp1InputRefs[i] = VkAttachmentReference
  {
    .attachment = i,
    .layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
  };

  Sp1InputRefs[6] = VkAttachmentReference
  {
    .attachment = 7,
    .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
  };

  /* Subpass 1 color references: 1 color output */
  std::array<VkAttachmentReference, Sp1ColorCount> Sp1ColorRefs{};
  for (UINT i = 0; i < Sp1ColorCount; i++)
    Sp1ColorRefs[i] = VkAttachmentReference
    {
      .attachment = Sp0ColorCount + i,
      .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

  /* Subpass 1 depth reference: read-only */
  VkAttachmentReference Sp1DepthRef
  {
    .attachment = DepthIdx,
    .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
  };

  /* Subpass descriptions */
  SubpassDescriptions =
  {
    /* Subpass 0: G-Buffer fill */
    VkSubpassDescription
    {
      .flags = 0,
      .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
      .inputAttachmentCount = 0,
      .pInputAttachments = nullptr,
      .colorAttachmentCount = Sp0ColorCount,
      .pColorAttachments = Sp0ColorRefs.data(),
      .pResolveAttachments = nullptr,
      .pDepthStencilAttachment = &Sp0DepthRef,
      .preserveAttachmentCount = 0,
      .pPreserveAttachments = nullptr,
    },
    /* Subpass 1: lighting / compose (reads G-Buffer as input) */
    VkSubpassDescription
    {
      .flags = 0,
      .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
      .inputAttachmentCount = static_cast<UINT>(Sp1InputRefs.size()),
      .pInputAttachments = Sp1InputRefs.data(),
      .colorAttachmentCount = 1,
      .pColorAttachments = Sp1ColorRefs.data(),
      .pResolveAttachments = nullptr,
      .pDepthStencilAttachment = &Sp1DepthRef,
      .preserveAttachmentCount = 0,
      .pPreserveAttachments = nullptr,
    },
  };

  /* Subpass dependencies */
  SubpassDependencies =
  {
    /* External -> Subpass 0 */
    VkSubpassDependency
    {
      .srcSubpass = VK_SUBPASS_EXTERNAL,
      .dstSubpass = 0,
      .srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      .srcAccessMask = VK_ACCESS_MEMORY_READ_BIT,
      .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
    },
    /* Subpass 0 -> Subpass 1: G-Buffer color/depth available as input */
    VkSubpassDependency
    {
      .srcSubpass = 0,
      .dstSubpass = 1,
      .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                        VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT |
                        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      .dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT |
                        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
      .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
    },
    ///* Subpass 0 -> External:
    //   deferred G-Buffer outputs are available for input attachment reads in next renderpass (merge). */
    //VkSubpassDependency
    //{
    //  .srcSubpass = 0,
    //  .dstSubpass = VK_SUBPASS_EXTERNAL,
    //  .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
    //  .dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
    //  .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    //  .dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,
    //  .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
    //},
    ///* Subpass 1 -> External: results available for sampling */
    VkSubpassDependency
    {
      .srcSubpass = 1,
      .dstSubpass = VK_SUBPASS_EXTERNAL,
      .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      .dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,
      .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
    },
  };

  /* Create render pass */
  VkRenderPassCreateInfo CreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .attachmentCount = static_cast<UINT>(AttachmentDescriptions.size()),
    .pAttachments = AttachmentDescriptions.data(),
    .subpassCount = static_cast<UINT>(SubpassDescriptions.size()),
    .pSubpasses = SubpassDescriptions.data(),
    .dependencyCount = static_cast<UINT>(SubpassDependencies.size()),
    .pDependencies = SubpassDependencies.data(),
  };

  if (vkCreateRenderPass(VlkRef->Device, &CreateInfo, nullptr, &RenderPass) != VK_SUCCESS)
  {
    logger::error << "Error creating deferred render pass!";
    return FALSE;
  }
  logger::info << "Deferred render pass successfully created.";

  return TRUE;
} /* End of 'tam::vlk::renderpass::Create' function */

/* Create post processing renderpass with one subpass function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) TRUE if renderpass was created, FALSE otherwise.
 */
BOOL tam::vlk::renderpass::CreatePostProc( VOID )
{
  constexpr UINT ColorCount = 1;
  constexpr UINT InputColorCount = 1;

  // Input
  AttachmentDescriptions.push_back(VkAttachmentDescription
  {
    .flags = 0,
    .format = VK_FORMAT_R16G16B16A16_SFLOAT,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,  
    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, /// VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
  });

  // Output
  AttachmentDescriptions.push_back(VkAttachmentDescription
  {
    .flags = 0,
    .format = VlkRef->SwapchainImageFormat,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,  
    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .finalLayout = /*VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL*/VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
  });

  AttachmentReferences.push_back(VkAttachmentReference
  {
    .attachment = 0,
    .layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
  });

  VkAttachmentReference ColorRef
  {
    .attachment = 1,
    .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
  };

  /* Subpass descriptions */
  SubpassDescriptions =
  {
    /* Subpass 0 */
    VkSubpassDescription
    {
      .flags = 0,
      .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
      .inputAttachmentCount = 1,
      .pInputAttachments = AttachmentReferences.data(),
      .colorAttachmentCount = ColorCount,
      .pColorAttachments = &ColorRef,
      .pResolveAttachments = nullptr,
      .pDepthStencilAttachment = nullptr,
      .preserveAttachmentCount = 0,
      .pPreserveAttachments = nullptr,
    }
  };

  /* Subpass dependencies */
  SubpassDependencies =
  {
    /* External -> Subpass 0 */
    VkSubpassDependency
    {
      .srcSubpass = VK_SUBPASS_EXTERNAL,
      .dstSubpass = 0,
      .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      .dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,
      .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
    },
    /* Subpass 0 -> External: results available for sampling */
    VkSubpassDependency
    {
      .srcSubpass = 0,
      .dstSubpass = VK_SUBPASS_EXTERNAL,
      .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
      .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
    }
  };

  /* Create render pass */
  VkRenderPassCreateInfo CreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .attachmentCount = static_cast<UINT>(AttachmentDescriptions.size()),
    .pAttachments = AttachmentDescriptions.data(),
    .subpassCount = static_cast<UINT>(SubpassDescriptions.size()),
    .pSubpasses = SubpassDescriptions.data(),
    .dependencyCount = static_cast<UINT>(SubpassDependencies.size()),
    .pDependencies = SubpassDependencies.data(),
  };

  if (vkCreateRenderPass(VlkRef->Device, &CreateInfo, nullptr, &RenderPass) != VK_SUCCESS)
  {
    logger::error << "Error creating merge render pass!";
    return FALSE;
  }
  logger::info << "Merge render pass successfully created.";

  return TRUE;
} /* End of 'CreatePostProc' function */

/* Destroy renderpass function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::renderpass::Destroy( VOID )
{
  if (RenderPass != VK_NULL_HANDLE && VlkRef->Device != VK_NULL_HANDLE)
    vkDestroyRenderPass(VlkRef->Device, RenderPass, nullptr);
} /* End of 'tam::vlk::renderpass::Destroy' function */

/* END OF 'tam_vlk_renderpass.cpp' FILE */
