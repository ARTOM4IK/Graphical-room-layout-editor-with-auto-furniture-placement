/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_material_pattern.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Material pattern structure declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 17.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_res_material_pattern_h_
#define __tam_res_material_pattern_h_

#include "tam_def.h"
#include ".\systems\anim\render\res\tam_res.h"

/* 'tam' namespace */
namespace tam
{
  /* Forward declaraiton of 'material_pattern_res' class for 'material_pattern' type usage */
  class material_pattern_res;
  /* Forward declaraiton of 'material_res' class for 'material' type usage */
  class material_res;
  /* Define material pattern interface type */
  using material_pattern = std::shared_ptr<material_pattern_res>;
  /* Define material interface type */
  using material = std::shared_ptr<material_res>;

  /* 'material_pattern' handle class */
  class material_pattern_res : public resource
  {
  public:
    std::string Name {}; // Material pattern name

    std::vector<vlk::descriptor_set_layout> DescriptorSetLayouts {}; // Layouts for global, pattern's (optionally), material's descriptor set layouts
    vlk::descriptor_set PatternDS;                                   // Optional descriptor set for material pattern
    std::vector<VkDescriptorSet> ApplyDescriptors {};                // All descriptor sets that will be applied with material pattern
    vlk::pipeline PatternPipeline;                                   // Material pattern pipeline
    UINT MaterialDescriptorNo = 0;                                   // Material descriptor set number

    shader Shader;                        // Shader that this material pattern will use
    std::vector<buffer> PatternBuffers;   // Buffers attached to material pattern
    std::vector<texture> PatternTextures; // Textures attached to material pattern
    std::vector<texture> PatternAttachments;
    INT MtlNumOfTextures = 0;             // Max number of textures in material
    INT MtlNumOfBuffers = 0;              // Max number of buffers in material
    INT MtlNumOfAttachments = 0;              // Max number of attachments in material

    UINT64 StoreFrameNo {};       // Number of frame when material was drawn
    material_pattern NextDraw {}; // Pointer to next-to-draw material pattern
    material FirstDraw {};        // Pointer to first-to-draw material

    /* Material pattern constructor.
     * ARGUMENTS:
     *   - pointer to render:
     *       render *RndPtr;
     */
    material_pattern_res( render *RndPtr );

    /* Material pattern move constructor.
     * ARGUMENTS:
     *   - rvalue reference to material_pattern_res:
     *       material_pattern_res &&Old;
     */
    material_pattern_res( material_pattern_res &&Old );

    /* Class destructor */
    ~material_pattern_res( VOID )
    {
      Free();
    } /* End of '~material_pattern' function */

    /* Create new material pattern function.
     * ARGUMENTS:
     *   - shader interface:
     *       shader NewShd;
     *   - new material pattern name:
     *       const std::string &MtlPatName;
     *   - number of textures for material:
     *       INT TexturesCount;
     *   - number of buffers for material:
     *       INT BuffersCount;
     *   - vector of pattern's buffers:
     *       const std::vector<buffer> &NewPatternBuffers;
     *   - vector of pattern's textures:
     *       const std::vector<texture> &NewPatternTextures;
     *   - vertex attributes info:
     *       const std::vector<vlk::vertex_attribute> &AttribDescrs;
     *   - primitive topology type:
     *       vlk::topology Topology;
     *   - blend mode:
     *       vlk::blend_mode BlendMode;
     *   - patch control points (for tessellation; 3 or 16):
     *       UINT PatchControlPoints;
     * RETURNS:
     *   (material_pattern_res &) reference to new material pattern.
     */
    material_pattern_res & Create( shader NewShd, const std::string &MtlPatName, INT TexturesCount, INT BuffersCount,
                                   const std::vector<buffer> &NewPatternBuffers, const std::vector<texture> &NewPatternTextures,
      const std::vector<vlk::vertex_attribute> &AttribDescrs, vlk::topology Topology, vlk::blend_mode BlendMode, UINT PatchControlPoints = 0, INT AttachmentsCount = 0, const std::vector<texture> &Attachments = {} );

    /* Material pattern destroy function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );

    /* Apply material pattern function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Apply( VOID );

    /* Update material pattern function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Update( VOID );

  }; /* End of 'material_pattern_res' class */

  /* 'material_pattern_manager' handle class */
  class material_pattern_manager : public resource_manager<material_pattern_res, std::string>
  {
  public:
    /* Material pattern manager class constructor.
     * ARGUMENTS:
     *   - reference to render:
     *       render &Rnd;
     */
    material_pattern_manager( render &Rnd ) : resource_manager(Rnd)
    {
    } /* End of 'material_pattern_manager' function */

    /* Class destructor */
    ~material_pattern_manager( VOID )
    {
      logger::info << "Material patterns deinitialization";
    } /* End of '~material_pattern_manager' function */

    /* Create new material pattern function.
     * ARGUMENTS:
     *   - shader interface:
     *       shader NewShd;
     *   - new material pattern name:
     *       const std::string &MtlPatName;
     *   - number of textures for material:
     *       INT TexturesCount;
     *   - number of buffers for material:
     *       INT BuffersCount;
     *   - vector of pattern's buffers:
     *       const std::vector<buffer> &NewPatternBuffers;
     *   - vector of pattern's textures:
     *       const std::vector<texture> &NewPatternTextures;
     *   - vertex attributes info:
     *       const std::vector<vlk::vertex_attribute> &AttribDescrs;
     *   - primitive topology type:
     *       vlk::topology Topology;
     *   - blend mode flag:
     *       vlk::blend_mode BlendMode;
     *   - patch control points (for tessellation):
     *       UINT PatchControlPoints;
     * RETURNS:
     *   (material_pattern) shared pointer to new material pattern.
     */
    material_pattern MaterialPatternCreate( shader NewShd, const std::string &MtlPatName, INT TexturesCount, INT BuffersCount,
                                            const std::vector<buffer> &NewPatternBuffers, const std::vector<texture> &NewPatternTextures,
                                            const std::vector<vlk::vertex_attribute> &AttribDescrs =
                                            {vlk::vertex_attribute::eVEC3, vlk::vertex_attribute::eVEC2, vlk::vertex_attribute::eVEC3, vlk::vertex_attribute::eVEC4,},
      vlk::topology Topology = vlk::topology::TRIMESH, vlk::blend_mode BlendMode = vlk::blend_mode::eOPAQUE, UINT PatchControlPoints = 0, INT AttachmentsCount = 0, const std::vector<texture> &Attachments = {} )
    {
      material_pattern MtlPat = Allocate(MtlPatName);
      if (MtlPat->Name == "")
        MtlPat->Create(NewShd, MtlPatName, TexturesCount, BuffersCount, NewPatternBuffers, NewPatternTextures, AttribDescrs, Topology, BlendMode, PatchControlPoints, AttachmentsCount, Attachments);
      return MtlPat;
    } /* End of 'MaterialPatternCreate' function */

    /* Free material pattern function.
     * ARGUMENTS:
     *   - material pattern to free:
     *       material_pattern &MtlPat;
     * RETURNS: None.
     */
    VOID MaterialPatternDelete( material_pattern &MtlPat )
    {
      resource_manager::Delete(MtlPat);
    } /* End of 'MaterialPatternDelete' function */

    /* Update material patterns function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID MaterialPatternsUpdate( VOID );
  }; /* End of 'material_pattern_manager' class */
} /* end of 'tam' namespace */

#endif /* __tam_res_material_pattern_h_ */

/* END OF 'tam_res_material_pattern.h' FILE */
