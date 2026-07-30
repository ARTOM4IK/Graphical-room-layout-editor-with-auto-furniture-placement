/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_pipelines.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Vulkan pipeline functions implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 07.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Create pipeline function.
 * ARGUMENTS:
 *   - vulkan shader reference:
 *       const shader &Shader;
 *   - descriptor set layouts:
 *       std::vector<descriptor_set_layout> DSLayouts;
 *   - description of vertex attributes used on shader:
 *       std::vector<VkVertexInputAttributeDescription> AttribDescrs;
 *   - topology type (points, lines, trimesh, strip, patch):
 *       topology NewType;
 *   - transparency mode:
 *       blend_mode BlendMode;
 *   - patch control points (for PATCH/tessellation):
 *       UINT PatchControlPoints;
 * RETURNS:
 *   (BOOL) depending on result.
 */
BOOL tam::vlk::pipeline::Create( const shader &Shader,
                                 const std::vector<descriptor_set_layout> &DSLayouts,
                                 const std::vector<vertex_attribute> &AttribDescrs,
                                 topology NewType, blend_mode BlendMode, UINT PatchControlPoints )
{
  Mode = BlendMode;
  INT NumOfAttachments = ((Mode == blend_mode::ePOSTPROC) || (Mode == blend_mode::eMERGE) || (Shader.ShaderDirName == "light/opaq") || (Shader.ShaderDirName == "light/tran")) ? 1 : 6;
  for (auto &layout : DSLayouts)
  {
    DescriptorSetLayouts.push_back(layout.DescriptorSetLayout);
    for (auto &binding : layout.DescriptorSetLayoutBindings)
      DescriptorSetLayoutBindings.push_back(binding);
  }

  VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutCreateInfo =
  {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    .flags = 0,
    .bindingCount = (UINT32)DescriptorSetLayoutBindings.size(),
    .pBindings = DescriptorSetLayoutBindings.data(),
  };

  VkPushConstantRange PushConstantRange
  {
    .stageFlags = VK_SHADER_STAGE_ALL,
    .offset = 0,
    /// .size = sizeof(BUF_PRIM),
    .size = sizeof(INT) * 2,
  };

  VkPipelineLayoutCreateInfo Info
  {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .setLayoutCount = (UINT)DescriptorSetLayouts.size(),
    .pSetLayouts = DescriptorSetLayouts.data(),
    .pushConstantRangeCount = 1,
    .pPushConstantRanges = &PushConstantRange,
  };

  std::vector<VkVertexInputAttributeDescription> VertexAttribDescrs {};
  UINT Offset = 0;
  for (INT i = 0; i < AttribDescrs.size(); i++)
  {
    VertexAttribDescrs.push_back(VkVertexInputAttributeDescription
    {
      .location = (UINT32)i,
      .binding = 0,
      .format = AttribDescrs[i] == vertex_attribute::eVEC2 ? VK_FORMAT_R32G32_SFLOAT :
        AttribDescrs[i] == vertex_attribute::eVEC3 ? VK_FORMAT_R32G32B32_SFLOAT :
        AttribDescrs[i] == vertex_attribute::eVEC4 ? VK_FORMAT_R32G32B32A32_SFLOAT : VK_FORMAT_R32G32B32A32_SINT,
      .offset = Offset,
    });
    Offset += AttribDescrs[i] == vertex_attribute::eVEC2 ? 8 :
      AttribDescrs[i] == vertex_attribute::eVEC3 ? 12 : 16;
  }

  // Vertices data format.
  VkVertexInputBindingDescription BindingDescr
  {
    .binding = 0,
    .stride = Offset,
    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX, // Basic format of vertices (needed for instancing)
  };

  if (vkCreatePipelineLayout(VlkPtr->Device, &Info, nullptr, &Layout) != VK_SUCCESS)
  {
    logger::error << "Unable to create pipeline layout.";
    return FALSE;
  }
  logger::info << "Pipeline layout successfully created.";

  VkPipelineVertexInputStateCreateInfo VertexInputInfo =
  {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    .vertexBindingDescriptionCount = 1,
    .pVertexBindingDescriptions = &BindingDescr,
    .vertexAttributeDescriptionCount = (UINT)AttribDescrs.size(),
    .pVertexAttributeDescriptions = VertexAttribDescrs.data(),
  };
  BindingDescription = BindingDescr;
  AttribDescriptions = VertexAttribDescrs;

  // Connection with shaders.
  if (Shader.ShaderModuleVert == VK_NULL_HANDLE)
  {
    logger::error << "No vertex shader found.";
    return FALSE;
  }
  std::vector<VkPipelineShaderStageCreateInfo> ShaderStages {};

  VkPipelineShaderStageCreateInfo VertShdStageInfo
  {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .stage = VK_SHADER_STAGE_VERTEX_BIT,
    .module = Shader.ShaderModuleVert,
    .pName = "main",
  };
  ShaderStages.push_back(VertShdStageInfo);

  if (Shader.ShaderModuleFrag != VK_NULL_HANDLE)
  {
    VkPipelineShaderStageCreateInfo FragShdStageInfo
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
      .module = Shader.ShaderModuleFrag,
      .pName = "main",
    };
    ShaderStages.push_back(FragShdStageInfo);
  }

  if (Shader.ShaderModuleCtrl != VK_NULL_HANDLE)
  {
    VkPipelineShaderStageCreateInfo CtrlShdStageInfo
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
      .module = Shader.ShaderModuleCtrl,
      .pName = "main",
    };
    ShaderStages.push_back(CtrlShdStageInfo);
  }

  if (Shader.ShaderModuleEval != VK_NULL_HANDLE)
  {
    VkPipelineShaderStageCreateInfo EvalShdStageInfo
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
      .module = Shader.ShaderModuleEval,
      .pName = "main",
    };
    ShaderStages.push_back(EvalShdStageInfo);
  }

  if (Shader.ShaderModuleGeom != VK_NULL_HANDLE)
  {
    VkPipelineShaderStageCreateInfo GeomShdStageInfo
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_GEOMETRY_BIT,
      .module = Shader.ShaderModuleGeom,
      .pName = "main",
    };
    ShaderStages.push_back(GeomShdStageInfo);
  }

  if (Shader.ShaderModuleComp != VK_NULL_HANDLE)
  {
    VkPipelineShaderStageCreateInfo CompShdStageInfo
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_COMPUTE_BIT,
      .module = Shader.ShaderModuleComp,
      .pName = "main",
    };
    ShaderStages.push_back(CompShdStageInfo);
  }

  // Dynamic state setup.
  DynamicStates =
  {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR,
    //VK_DYNAMIC_STATE_CULL_MODE,
    //VK_DYNAMIC_STATE_POLYGON_MODE_EXT,
  };

  DynamicStatesCreateInfo =
  {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
    .dynamicStateCount = (UINT32)DynamicStates.size(),
    .pDynamicStates = DynamicStates.data(),
  };

  Viewport =
  {
    .x = 0.0f,
    .y = static_cast<FLT>(VlkPtr->SwapchainInfo.imageExtent.height),
    .width = static_cast<FLT>(VlkPtr->SwapchainInfo.imageExtent.width),
    .height = -static_cast<FLT>(VlkPtr->SwapchainInfo.imageExtent.height),
    .minDepth = 0.0f,
    .maxDepth = 1.0f,
  };

  Scissors =
  {
    .offset
    {
      .x = 0,
      .y = 0
    },
    .extent
    {
      .width = (UINT)VlkPtr->SwapchainInfo.imageExtent.width,
      .height = (UINT)VlkPtr->SwapchainInfo.imageExtent.height
    }
  };

  ViewportCreateInfo =
  {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .viewportCount = 1,
    .pViewports = &Viewport,
    .scissorCount = 1,
    .pScissors = &Scissors
  };

  // Depth and stencil tests.
  DepthStencil =
  {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
    .depthTestEnable = VK_TRUE,
    .depthWriteEnable = (Shader.ShaderDirName == "light/opaq") || (Shader.ShaderDirName == "light/tran") ? VK_FALSE : VK_TRUE,
    .depthCompareOp = VK_COMPARE_OP_LESS,
    .depthBoundsTestEnable = VK_FALSE,
    .stencilTestEnable = VK_FALSE,
  };

  // Rasterizations parameters
  RasterizationCreateInfo =
  {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    .rasterizerDiscardEnable = VK_FALSE,
    .polygonMode = VK_POLYGON_MODE_FILL,
    .cullMode = VK_CULL_MODE_NONE,
    .frontFace = VK_FRONT_FACE_CLOCKWISE,
    .lineWidth = 2.0f,
  };

  // Multisampling settings.
  MultiSamplingCreateInfo =
  {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    .sampleShadingEnable = VK_TRUE,
  };

  // Blending parameters.
  if (BlendMode == blend_mode::eOPAQUE)
    BlendAttachmentState =
    {
      .blendEnable = VK_FALSE,
      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                        VK_COLOR_COMPONENT_G_BIT |
                        VK_COLOR_COMPONENT_B_BIT |
                        VK_COLOR_COMPONENT_A_BIT,
    };
  else
    BlendAttachmentState =
    {
      .blendEnable = VK_TRUE,
      .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
      .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
      .colorBlendOp = VK_BLEND_OP_ADD,
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
      .alphaBlendOp = VK_BLEND_OP_ADD,
      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                        VK_COLOR_COMPONENT_G_BIT |
                        VK_COLOR_COMPONENT_B_BIT |
                        VK_COLOR_COMPONENT_A_BIT,
    };
  std::vector<VkPipelineColorBlendAttachmentState> AttStates(NumOfAttachments, BlendAttachmentState);
  BlendCreateInfo =
  {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    .logicOpEnable = VK_FALSE,
    .attachmentCount = (UINT)NumOfAttachments,
    .pAttachments = AttStates.data(),
  };

  // Geometry parameters (PATCH_LIST when tessellation is used)
  BOOL UseTessellation = (NewType == topology::PATCH && PatchControlPoints > 0 &&
    Shader.ShaderModuleCtrl != VK_NULL_HANDLE && Shader.ShaderModuleEval != VK_NULL_HANDLE);
  InputAssemblyCreateInfo = {.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    .topology = UseTessellation ? VK_PRIMITIVE_TOPOLOGY_PATCH_LIST :
      (NewType == topology::POINTS ? VK_PRIMITIVE_TOPOLOGY_POINT_LIST :
      NewType == topology::LINES ? VK_PRIMITIVE_TOPOLOGY_LINE_LIST :
      NewType == topology::STRIP ? VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP :
      VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST),
    .primitiveRestartEnable = VK_FALSE,};

  StoredTopology = NewType;
  StoredPatchControlPoints = PatchControlPoints;
  TessellationStateCreateInfo = {.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
    .patchControlPoints = UseTessellation ? (UINT32)PatchControlPoints : 0,};

  PipelineCreateInfo = {.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    .stageCount = (UINT32)ShaderStages.size(),
    .pStages = ShaderStages.data(),
    .pVertexInputState = &VertexInputInfo,
    .pInputAssemblyState = &InputAssemblyCreateInfo,
    .pTessellationState = UseTessellation ? &TessellationStateCreateInfo : nullptr,
    .pViewportState = &ViewportCreateInfo,
    .pRasterizationState = &RasterizationCreateInfo,
    .pMultisampleState = &MultiSamplingCreateInfo,
    .pDepthStencilState = &DepthStencil,
    .pColorBlendState = &BlendCreateInfo,
    .pDynamicState = &DynamicStatesCreateInfo,
    .layout = Layout,
    .renderPass = (Mode == blend_mode::ePOSTPROC) ? VlkPtr->PostProcRenderPass.RenderPass : (Mode == blend_mode::eOPAQUE) ? VlkPtr->OpaqueRenderPass.RenderPass : (Mode == blend_mode::eTRANSPARENT) ? VlkPtr->TransRenderPass.RenderPass : VlkPtr->MergeRenderPass.RenderPass,
    .subpass = (Shader.ShaderDirName == "light/opaq") || (Shader.ShaderDirName == "light/tran") ? 1U : 0U,
    .basePipelineHandle = VK_NULL_HANDLE,
    .basePipelineIndex = -1,};

  if (vkCreateGraphicsPipelines(VlkPtr->Device, VK_NULL_HANDLE, 1, &PipelineCreateInfo, nullptr, &Pipeline) != VK_SUCCESS)
  {
    logger::error << "Unable to create pipeline.";
    return FALSE;
  }

  logger::info << "Pipeline successfully created.";
  return TRUE;
} /* End of 'tam::vlk::pipeline::Create' function */

/* Update pipeline function.
 * ARGUMENTS:
 *   - vulkan shader reference:
 *       const shader &NewShader;
 * RETURNS:
 *   (BOOL) depending on result.
 */
BOOL tam::vlk::pipeline::Update( const shader &NewShader )
{
  vkDeviceWaitIdle(VlkPtr->Device);
  INT NumOfAttachments = ((Mode == blend_mode::ePOSTPROC) || (Mode == blend_mode::eMERGE) || (NewShader.ShaderDirName == "light/opaq") || (NewShader.ShaderDirName == "light/tran")) ? 1 : 6;

  VkPipelineVertexInputStateCreateInfo VertexInputInfo =
  {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    .vertexBindingDescriptionCount = 1,
    .pVertexBindingDescriptions = &BindingDescription,
    .vertexAttributeDescriptionCount = (UINT)AttribDescriptions.size(),
    .pVertexAttributeDescriptions = AttribDescriptions.data(),
  };

  // Connection with shaders.
  if (NewShader.ShaderModuleVert == VK_NULL_HANDLE)
  {
    logger::error << "No vertex shader found.";
    return FALSE;
  }
  std::vector<VkPipelineShaderStageCreateInfo> ShaderStages {};

  VkPipelineShaderStageCreateInfo VertShdStageInfo
  {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .stage = VK_SHADER_STAGE_VERTEX_BIT,
    .module = NewShader.ShaderModuleVert,
    .pName = "main",
  };
  ShaderStages.push_back(VertShdStageInfo);

  if (NewShader.ShaderModuleFrag != VK_NULL_HANDLE)
  {
    VkPipelineShaderStageCreateInfo FragShdStageInfo
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
      .module = NewShader.ShaderModuleFrag,
      .pName = "main",
    };
    ShaderStages.push_back(FragShdStageInfo);
  }

  if (NewShader.ShaderModuleCtrl != VK_NULL_HANDLE)
  {
    VkPipelineShaderStageCreateInfo CtrlShdStageInfo
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
      .module = NewShader.ShaderModuleCtrl,
      .pName = "main",
    };
    ShaderStages.push_back(CtrlShdStageInfo);
  }

  if (NewShader.ShaderModuleEval != VK_NULL_HANDLE)
  {
    VkPipelineShaderStageCreateInfo EvalShdStageInfo
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
      .module = NewShader.ShaderModuleEval,
      .pName = "main",
    };
    ShaderStages.push_back(EvalShdStageInfo);
  }

  if (NewShader.ShaderModuleGeom != VK_NULL_HANDLE)
  {
    VkPipelineShaderStageCreateInfo GeomShdStageInfo
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_GEOMETRY_BIT,
      .module = NewShader.ShaderModuleGeom,
      .pName = "main",
    };
    ShaderStages.push_back(GeomShdStageInfo);
  }

  if (NewShader.ShaderModuleComp != VK_NULL_HANDLE)
  {
    VkPipelineShaderStageCreateInfo CompShdStageInfo
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_COMPUTE_BIT,
      .module = NewShader.ShaderModuleComp,
      .pName = "main",
    };
    ShaderStages.push_back(CompShdStageInfo);
  }

  /* Correct pointers to constant variables */
  DynamicStatesCreateInfo.pDynamicStates = DynamicStates.data();
  BlendCreateInfo.pAttachments = &BlendAttachmentState;
  BOOL UseTessellationUpdate = (StoredTopology == topology::PATCH && StoredPatchControlPoints > 0 &&
    NewShader.ShaderModuleCtrl != VK_NULL_HANDLE && NewShader.ShaderModuleEval != VK_NULL_HANDLE);
  if (UseTessellationUpdate)
  {
    InputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
    TessellationStateCreateInfo.patchControlPoints = (UINT32)StoredPatchControlPoints;
  }
  else
    InputAssemblyCreateInfo.topology =
      StoredTopology == topology::POINTS ? VK_PRIMITIVE_TOPOLOGY_POINT_LIST :
      StoredTopology == topology::LINES ? VK_PRIMITIVE_TOPOLOGY_LINE_LIST :
      StoredTopology == topology::STRIP ? VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP :
      VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  // Blending parameters.
  if (Mode == blend_mode::eOPAQUE)
    BlendAttachmentState =
    {
      .blendEnable = VK_FALSE,
      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                        VK_COLOR_COMPONENT_G_BIT |
                        VK_COLOR_COMPONENT_B_BIT |
                        VK_COLOR_COMPONENT_A_BIT,
    };
  else
    BlendAttachmentState =
    {
      .blendEnable = VK_FALSE,
      .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                        VK_COLOR_COMPONENT_G_BIT |
                        VK_COLOR_COMPONENT_B_BIT |
                        VK_COLOR_COMPONENT_A_BIT,
    };
  std::vector<VkPipelineColorBlendAttachmentState> AttStates(NumOfAttachments, BlendAttachmentState);
  BlendCreateInfo =
  {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    .logicOpEnable = VK_FALSE,
    .attachmentCount = (UINT)NumOfAttachments,
    .pAttachments = AttStates.data(),
  };


  PipelineCreateInfo =
  {
    .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    .stageCount = (UINT32)ShaderStages.size(),
    .pStages = ShaderStages.data(),
    .pVertexInputState = &VertexInputInfo,
    .pInputAssemblyState = &InputAssemblyCreateInfo,
    .pTessellationState = UseTessellationUpdate ? &TessellationStateCreateInfo : nullptr,
    .pViewportState = &ViewportCreateInfo,
    .pRasterizationState = &RasterizationCreateInfo,
    .pMultisampleState = &MultiSamplingCreateInfo,
    .pDepthStencilState = &DepthStencil,
    .pColorBlendState = &BlendCreateInfo,
    .pDynamicState = &DynamicStatesCreateInfo,
    .layout = Layout,
    .renderPass = (Mode == blend_mode::ePOSTPROC) ? VlkPtr->PostProcRenderPass.RenderPass : (Mode == blend_mode::eOPAQUE) ? VlkPtr->OpaqueRenderPass.RenderPass : (Mode == blend_mode::eTRANSPARENT) ? VlkPtr->TransRenderPass.RenderPass : VlkPtr->MergeRenderPass.RenderPass,
    .subpass = (NewShader.ShaderDirName == "light/opaq") || (NewShader.ShaderDirName == "light/tran") ? 1U : 0U,
    .basePipelineHandle = Pipeline,
    .basePipelineIndex = -1,
  };

  VkPipeline OldPipeline = Pipeline;

  if (vkCreateGraphicsPipelines(VlkPtr->Device, VK_NULL_HANDLE, 1, &PipelineCreateInfo, nullptr, &Pipeline) != VK_SUCCESS)
  {
    logger::error << "Unable to update pipeline.";
    return FALSE;
  }
  if (OldPipeline != VK_NULL_HANDLE)
    vkDestroyPipeline(VlkPtr->Device, OldPipeline, nullptr);

  logger::info << "Pipeline successfully updated.";
  return TRUE;
} /* End of 'tam::vlk::pipeline::Update' function */

/* Destroy vulkan pipeline function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::pipeline::Destroy( VOID )
{
  if (Layout != VK_NULL_HANDLE)
  {
    vkDestroyPipelineLayout(VlkPtr->Device, Layout, nullptr);
    Layout = VK_NULL_HANDLE;
  }
  if (Pipeline != VK_NULL_HANDLE)
  {
    vkDestroyPipeline(VlkPtr->Device, Pipeline, nullptr);
    Pipeline = VK_NULL_HANDLE;
  }
  BindingDescription = {};
  AttribDescriptions.clear();
  DescriptorSetLayoutBindings.clear();
  DescriptorSetLayouts.clear();
  DynamicStates.clear();
  DynamicStatesCreateInfo = {};
  Viewport = {};
  Scissors = {};
  ViewportCreateInfo = {};
  DepthStencil = {};
  RasterizationCreateInfo = {};
  MultiSamplingCreateInfo = {};
  BlendAttachmentState = {};
  BlendCreateInfo = {};
  InputAssemblyCreateInfo = {};
  PipelineCreateInfo = {};
} /* End of 'tam::vlk::pipeline::Destroy' function */

/* END OF 'tam_vlk_pipelines.cpp' FILE */
