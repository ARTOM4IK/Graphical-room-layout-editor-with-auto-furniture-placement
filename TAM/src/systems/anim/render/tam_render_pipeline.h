/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

 /* FILE NAME   : tam_render_pipeline.h
  * PURPOSE     : Tough Arrangement Manager project.
  *               Render pipeline declaration module.
  * PROGRAMMER  : CGSG'Sr'2025.
  *               Arsentev Artemy (AA4),
  *               Tretyakov Andrey (AT7).
  * LAST UPDATE : 21.04.2026
  * NOTE        : Namespace 'tam'.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __tam_render_pipeline_h_
#define __tam_render_pipeline_h_

#define FORWARD_ALLOCATION_MATRS 128

/* 'tam' namespace */
namespace tam
{
  //enum class object_type
  //{
  //  eOPAQUE,
  //  eTRANSPARENT,
  //}; /* End of 'vertex_type' class */

  /* Compute pipeline data representation type */
  struct compute_data
  {
    std::vector<buffer> Buffers;
    std::vector<texture> Textures;
  }; /* End of 'compute_data' struct */

  /* Render config representation type */
  struct render_config
  {
    // Matrix buffer policy
    static constexpr uint32_t InitialMatrixCapacity = 8192;
    static constexpr uint32_t MatrixChunkSize = 128;
    static constexpr float MatrixGrowthFactor = 1.5f;
    
    // Compute
    static constexpr uint32_t XGroups = 1024;
    static constexpr uint32_t ElementsPerSlice = 1024;
    static constexpr uint32_t ElementsPerDepthSlice = 1048576;
    
    // Scene limits
    static constexpr uint32_t MaxLights = 128;
    static constexpr uint32_t MaxInstances = 1024;
    
    // Clear colors
    static inline const VkClearColorValue MergeBackground = {{0.30f, 0.47f, 0.8f, 1.0f}};
    static inline const VkClearDepthStencilValue DepthFar = {1.0f, 0};
    
    // Fullscreen triangle
    static inline const std::array<vec3, 3> ClipSpaceTriangle = 
    {
      vec3(-1.0, 3.0, 0.01), 
      vec3(-1.0, -1.0, 0.01), 
      vec3(3.0, -1.0, 0.01)
    };
  }; /* End of 'render_config' struct */

  /* Compute handler representation class */
  class compute_manager
  {
  protected:
    render *Rnd; // Pointer to render instance
  public:

    shader ComputeShader {};                                 // Compute shader
    vlk::descriptor_set_layout ComputeDSLayout;              // Descriptor sets layout handle
    VkPipelineLayout ComputePipelineLayout = VK_NULL_HANDLE; // Compute pipeline layout
    VkPipeline ComputePipeline = VK_NULL_HANDLE;             // Compute pipline
    vlk::descriptor_set ComputeDS;                           // Descriptor set with attached resources

    /* Default type constructor.
     * ARGUMENTS:
     *   - pointer to render:
     *       render *NewRnd;
     */
    compute_manager( render *NewRnd );

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
    BOOL Create( const shader ComputeShader, const compute_data &InputData, const compute_data &OutputData,
                 const std::vector<VkDescriptorType> &DescriptorTypes );

    /* Execute compute shader function.
     * ARGUMENTS:
     *   - count of compute iterations:
     *       INT ComputeCount;
     * RETURNS: None.
     */
    VOID Compute( INT ComputeSize );

    /* Update data descriptor set function.
     * ARGUMENTS:
     *   - new input data:
     *       const compute_data &NewInputData;
     *   - new output data:
     *       const compute_data &NewOutputData;
     * RETURNS: None.
     */
    VOID Update( const compute_data &NewInputData, const compute_data &NewOutputData );

    /* Free compute manager function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );

    /* Default type destructor.
     * ARGUMENTS: None.
     */
    ~compute_manager( VOID );
  }; /* End of 'compute_manager' class */

  /* Logic pipeline class declaration */
  class render_pipeline : public compute_manager
  {
  public:
    std::vector<texture> OpaqAttach, TransAttach, MergeAttach, PostProcAttach;
    std::vector<VkImageView> PostProcView;
    std::vector<vlk::sampler> PostProcSampler;
    std::array<VkClearValue, 8> ClearValues{};
    std::array<VkClearValue, 5> MergeClearValues{};

    texture MergeTex, TransMergeTex, OpaqDepth, TransDepth, PostProcTex;        // Fullscreen texture for merge
    prim FullScreen, PostProc;              // Fullscreen primitive for merge
    buffer
      GlobalWorldMatricesBuf {},   // Global buffer of world transformation matrices
      GlobalWVPWInvMatricesBuf {}; // Global buffer of calculated WVP and inversed world matrices

    material_pattern FirstDraw {}; // First material pattern to draw
    UINT64 StoreFrameNo {};        // Material pattern store frame number
    UINT64 CurrentFrameNo {};      // Current frame number

    BOOL
      IsCamBufUpdate = FALSE,      // Flag if camera buffer was updated so it's necessary to recalculate matrices
      IsWorldBufferResize = FALSE, // Flag if matrix buffers were resized and should be binded to descriptorsets again
      IsWorldBufferUpdate = FALSE; // Flag if matrix buffers should be updated

    std::vector<matr::matr_data> GlobalMatrixes {}; // global world matrices vector
    UINT
      NumOfGlobalMatrixes = 0,    // number of global world matrices
      OldNumOfGlobalMatrixes = 0, // number of global world matrices
      MaxNumOfGlobalMatrixes = 0, // max number of global world matrices
      CurrentModelId = 0;         // Current id of model

    /* Representation type of global lighting params in buffer */
    struct lighting_params_data
    {
      vec4 SizeDayTimeDayOfYearPresentFlag {}; // x - count of light sources
                                               // y - day time
                                               // z - day of year
                                               // w - present lighting mode flag
      vec4 LightColorIntensity {};             // xyz - color of present light sources
                                               // w - intensity of present lights
    }; /* End of 'lighting_params_data' struct */

    struct lig_buf
    {
      vec4 PosXYZ_DirX;                // position (XYZ), direction (X)
      vec4 DirYZ_ColorXY;              // direction (YZ), color (XY)
      vec4 ColorZ_Int_Type_IsEnabled;  // color (Z), intensity, type, is_enabled
      vec4 Exp_Angle_0_0;              // exponent, 0, 0, 0
    };

    std::vector<lig_buf> Lights {}; // lights vector
    prim
      OpaqueLightScreenPrim, TransLightScreenPrim;
    buffer
      LightsBuf {};                 // lights buffer
    BOOL
      IsLightsUpdated = FALSE,      // update lights flag
      IsSunEnabled = TRUE;          // Is sun enable flag
    FLT EnvTime = 12.0;             // environment time
    /* Pipeline constructor.
     * ARGUMENTS:
     *   - pointer to render:
     *       render *Rnd;
     */
    render_pipeline( render* Rnd );

    /* Create render pipeline function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (BOOL) TRUE if render pipeline was initialized successfully, FALSE otherwise.
     */
    BOOL Create( VOID );

    /* Destroy render pipeline function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Close( VOID );

    /* Fill global matrixes buffer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID FillGlobalMatrices( VOID );

    /* Clear draw queue function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID ClearDrawQueue( VOID );

    /* Fill lights buffer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID FillLights( VOID );

    /* Instantly draw primitive to screen function.
     * ARGUMENTS:
     *   - primitive pointer:
     *       prim Pr;
     * RETURNS: None.
     */
    VOID DrawInstant( const prim Pr );

    /* Primitive add to draw queue function.
     * ARGUMENTS:
     *   - primitive pointer:
     *       prim Pr;
     *   - transformation matrix:
     *       const matr &Transform;
     *   - flag if primitive is static:
     *       BOOL IsStaticDraw;
     * RETURNS: None.
     */
    VOID Draw( prim Pr, const matr &Transform = matr::Identity(), BOOL IsStaticDraw = FALSE );

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
    INT Draw( model Mdl, const matr &World, BOOL IsStaticDraw = FALSE );

    /* Draw current frame function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID FrameDraw( VOID );

    /* Draw every primitive into target function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID DrawQueuedPrims( vlk::blend_mode Mode );

    /* Init render pipeline function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    BOOL InitPipeline( VOID );

    /* Close render pipeline function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID ClosePipeline( VOID );

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
    VOID OpaquePass( VkCommandBuffer Cmd, VkFramebuffer Fb, VkExtent2D Extent );

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
    VOID TransparentPass( VkCommandBuffer Cmd, VkFramebuffer Fb, VkExtent2D Extent );

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
    VOID ShadowPass( VkCommandBuffer Cmd, VkFramebuffer Fb, VkExtent2D Extent );

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
    VOID MergePass( VkCommandBuffer Cmd, VkFramebuffer Fb, VkExtent2D Extent );

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
    VOID PostProcessingPass( VkCommandBuffer Cmd, VkFramebuffer Fb, VkExtent2D Extent );

    /* Resize render pass frame buffers function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Resize( VOID );

    /* Illuminate scene function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID OpaqueLightPassDraw( VOID );          

    /* Illuminate scene function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID TransLightPassDraw( VOID );
    
  }; /* End of 'render_pipeline' class */
} /* end of 'tam' namespace */

#endif /* __tam_render_pipeline_h_ */

/* END OF 'tam_render_pipeline.h' FILE */
