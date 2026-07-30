/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_pipelines.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Vulkan pipeline declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 24.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_vlk_pipelines_h_
#define __tam_vlk_pipelines_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* 'vlk' namespace */
  namespace vlk
  {
    /* Vulkan core class forward declaration */
    class core;

    /* Descriptor set layout class forward declaration */
    class descriptor_set_layout;

    /* Object shape representation type */
    enum class topology
    {
      POINTS = 0,  // Points collection
      LINES = 1,   // Line list
      STRIP = 2,   // Strip of triangles
      TRIMESH = 3, // Triangle mesh
      PATCH = 4    // Patch list (tessellation)
    }; /* End of 'topology' enum class */

    /* Object depth interaction state representation type */
    enum class depth_state
    {
      DEPTH_NOT_READ_NOT_WRITE = 0, // Flag if objects should not use depth buffer
      DEPTH_READ = 1,               // Flag if objects should only read depth
      DEPTH_WRITE = 2,              // Flag if objects should only write depth
      DEPTH_READ_AND_WRITE = 3      // Flag if objects should read and write depth
    }; /* End of 'depth_state' enum class */

    /* Object alpha usage flags */
    enum class blend_mode
    {
      eTRANSPARENT = 0, // Flag if object should use alpha testing
      eOPAQUE = 1,      // Flag if object are opaque and non-transparent
      eMERGE = 2,
      ePOSTPROC = 3,
    }; /* End of 'blend_mode' enum class */

    /* Vertex attributes representation type */
    enum class vertex_attribute
    {
      eVEC2,  // 2D float vector
      eVEC3,  // 3D float vector
      eVEC4,  // 4D float vector
      eiVEC4  // 4D integer vector
    }; /* End of 'vertex_attribute' enum class */

    /* 'pipeline' handle class */
    class pipeline
    {
    private:
      core *VlkPtr; // Vulkan class instance reference
      topology StoredTopology = topology::TRIMESH; // For pipeline Update (tessellation)
      UINT StoredPatchControlPoints = 0;
    public:
      blend_mode Mode = blend_mode::eOPAQUE; // Blend mode
      VkPipeline Pipeline {}; // Graphics pipeline
      VkPipelineLayout Layout {}; // Pipeline layout
      VkVertexInputBindingDescription BindingDescription {}; // Vertex input bingind description
      std::vector<VkVertexInputAttributeDescription> AttribDescriptions {}; // Vertex input attributes descriptions
      std::vector<VkDescriptorSetLayoutBinding> DescriptorSetLayoutBindings {}; // Descriptor set layout bindings
      std::vector<VkDescriptorSetLayout> DescriptorSetLayouts {}; // Descriptor set layouts
      std::vector<VkDynamicState> DynamicStates {}; // Dynamic states (viewport, scissors, culling and polygon modes)
      VkPipelineDynamicStateCreateInfo DynamicStatesCreateInfo {}; // Dynamic states creation info
      VkViewport Viewport {}; // Used viewport
      VkRect2D Scissors {}; // Used 'scissors'
      VkPipelineViewportStateCreateInfo ViewportCreateInfo {}; // Viewport state creation info
      VkPipelineDepthStencilStateCreateInfo DepthStencil {}; // Depth and stencil usage description
      VkPipelineRasterizationStateCreateInfo RasterizationCreateInfo {}; // Rasterization info
      VkPipelineMultisampleStateCreateInfo MultiSamplingCreateInfo {}; // Multisampling creation info
      VkPipelineColorBlendAttachmentState BlendAttachmentState {}; // Blending attachment state
      VkPipelineColorBlendStateCreateInfo BlendCreateInfo {}; // Blending creation info
      VkPipelineInputAssemblyStateCreateInfo InputAssemblyCreateInfo {}; // Input assembly creation info
      VkPipelineTessellationStateCreateInfo TessellationStateCreateInfo {}; // Tessellation state (when PatchControlPoints > 0)
      VkGraphicsPipelineCreateInfo PipelineCreateInfo {}; // Pipeline creation info

      /* Pipeline handle type constructor.
       * ARGUMENTS:
       *   - global Vulkan handle pointer:
       *       core *Vlk;
       */
      pipeline( core *Vlk )
      {
        VlkPtr = Vlk;
      } /* End of 'pipeline' function */

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
       *   - patch control points (for PATCH/tessellation; use 3 or 16):
       *       UINT PatchControlPoints;
       * RETURNS:
       *   (BOOL) depending on result.
       */
      BOOL Create( const shader &Shader,
                   const std::vector<descriptor_set_layout> &DSLayouts,
                   const std::vector<vertex_attribute> &AttribDescrs,
                   topology NewType, blend_mode BlendMode = blend_mode::eTRANSPARENT, UINT PatchControlPoints = 0 );

      /* Update pipeline function.
       * ARGUMENTS:
       *   - vulkan shader reference:
       *       const shader &NewShader;
       * RETURNS:
       *   (BOOL) depending on result.
       */
      BOOL Update( const shader &NewShader );

      /* Destroy vulkan pipeline function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Destroy( VOID );
    }; /* End of 'pipeline' class */
  } /* end of 'vlk' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_vlk_pipelines_h_ */

/* END OF 'tam_vlk_pipelines.h' FILE */
