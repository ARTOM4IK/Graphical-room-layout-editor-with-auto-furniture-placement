/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_material_pattern.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Material pattern functions implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 05.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Material pattern constructor.
 * ARGUMENTS:
 *   - pointer to render:
 *       render *RndPtr;
 */
tam::material_pattern_res::material_pattern_res( render *RndPtr ) : resource(RndPtr), PatternPipeline(RndPtr), PatternDS(RndPtr)
{
} /* End of 'tam::material_pattern_res::material_pattern_res' function */

/* Material pattern move constructor.
 * ARGUMENTS:
 *   - rvalue reference to material_pattern_res:
 *       material_pattern_res &&Old;
 */
tam::material_pattern_res::material_pattern_res( material_pattern_res &&Old ) :
  resource(Old.Rnd), Name(Old.Name), DescriptorSetLayouts(Old.DescriptorSetLayouts), PatternDS(Old.PatternDS), PatternPipeline(Old.PatternPipeline), 
  Shader(Old.Shader), PatternBuffers(Old.PatternBuffers), PatternTextures(Old.PatternTextures),
  MtlNumOfTextures(Old.MtlNumOfTextures), MtlNumOfBuffers(Old.MtlNumOfBuffers)
{
  Old.Name = "";
  Old.DescriptorSetLayouts.clear();
  Old.PatternDS = VK_NULL_HANDLE;
  Old.PatternPipeline = VK_NULL_HANDLE;
  Old.PatternBuffers.clear();
  Old.PatternTextures.clear();
  Old.MtlNumOfTextures = 0;
  Old.MtlNumOfBuffers = 0;
  Old.ApplyDescriptors.clear();
  Old.MaterialDescriptorNo = 0;
  Old.Shader = nullptr;
} /* End of 'tam::material_pattern_res::material_pattern_res' function */

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
 * RETURNS:
 *   (material_pattern_res &) reference to new material pattern.
 */
tam::material_pattern_res & tam::material_pattern_res::Create( shader NewShd, const std::string &MtlPatName, INT TexturesCount, INT BuffersCount,
                                                               const std::vector<buffer> &NewPatternBuffers, const std::vector<texture> &NewPatternTextures,
                                                               const std::vector<vlk::vertex_attribute> &AttribDescrs, vlk::topology Topology, vlk::blend_mode BlendMode, UINT PatchControlPoints, INT AttachmentsCount, const std::vector<texture> &Attachments )
{
  Free();

  Name = MtlPatName;
  if (NewShd == nullptr)
  {
    NewShd = Rnd->ShaderCreate("default");
    if (NewShd == nullptr)
    {
      logger::warning << std::format("Error creating {}: no suitable shader was found!", MtlPatName);
      return *this;
    }
  }
  DescriptorSetLayouts.push_back(Rnd->FrameSyncDescriptorSetLayout);
  ApplyDescriptors.push_back(Rnd->FrameSyncDescriptorSet.DescriptorSet);
  MaterialDescriptorNo++;
  PatternBuffers = NewPatternBuffers;
  PatternTextures = NewPatternTextures;
  PatternAttachments = Attachments;
  /* Try to create descriptor set layout for pattern's descriptor set */
  if (NewPatternBuffers.size() != 0 || NewPatternTextures.size() != 0 || Attachments.size() != 0)
  {
    vlk::descriptor_set_layout PatternDSLayout(Rnd);
    std::vector<VkDescriptorSetLayoutBinding> DSLayoutBindings {};

    /* Get list of descriptors of buffers and textures for material pattern */
    INT Binding = 0;
    for (auto &Buf : PatternBuffers)
      DSLayoutBindings.push_back(VkDescriptorSetLayoutBinding
        {
          .binding = static_cast<UINT>(Binding++),
          .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
          .descriptorCount = 1,
          .stageFlags = VK_SHADER_STAGE_ALL,
        });
    for (auto &Tex : PatternTextures)
      DSLayoutBindings.push_back(VkDescriptorSetLayoutBinding
        {
          .binding = static_cast<UINT>(Binding++),
          .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
          .descriptorCount = 1,
          .stageFlags = VK_SHADER_STAGE_ALL,
        });
    for (auto &Att : PatternAttachments)
      DSLayoutBindings.push_back(VkDescriptorSetLayoutBinding
      {
        .binding = static_cast<UINT>(Binding++),
        .descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
      });

    if (!PatternDSLayout.Create(DSLayoutBindings))
      logger::warning << "Wanted to create descriptor set layout for material pattern, but couldn't.";
    else
    {
      DescriptorSetLayouts.emplace_back(PatternDSLayout);
      logger::info << "Created descriptor set layout for material pattern.";
      std::vector<vlk::buffer *> DSBuffers {};
      DSBuffers.reserve(NewPatternBuffers.size());
      for (auto &Buf : NewPatternBuffers)
        DSBuffers.push_back(&Buf->VlkBuffer);
      std::vector<VkImageView> DSImageViews {};
      std::vector<std::pair<VkImageView, VkImageLayout>> DSAttachmentViews {};
      std::vector<vlk::sampler> DSAttachmentSamplers {};
      std::vector<vlk::sampler> DSSamplers {};
      for (auto &Tex : NewPatternTextures)
      {
        DSImageViews.push_back(Tex->Img.View);
        DSSamplers.push_back(Tex->Sampler);

      }
      for (auto &Att : Attachments)
      {
        DSAttachmentViews.push_back({Att->Img.View, Att->Img.BindLayout});
        DSAttachmentSamplers.push_back(Att->Sampler);
      }
      if (!PatternDS.Create(DescriptorSetLayouts.back(), DSBuffers, DSImageViews, DSSamplers, DSAttachmentViews, DSAttachmentSamplers))
      {
        logger::error << "Error creating " >> 0x88AAFF << MtlPatName >> 0x33FA88 << ": unable to create descriptor set for pattern.";
        return *this;
      }
      MaterialDescriptorNo++;
      ApplyDescriptors.push_back(PatternDS.DescriptorSet);
    }
  }

  MtlNumOfTextures = TexturesCount;
  MtlNumOfBuffers = BuffersCount;
  MtlNumOfAttachments = AttachmentsCount;
  /* Create descriptor set for materials if it's necessary */
  if (MtlNumOfBuffers != 0 || MtlNumOfTextures != 0 || MtlNumOfAttachments != 0)
  {
    vlk::descriptor_set_layout MaterialDSLayout(Rnd);
    std::vector<VkDescriptorSetLayoutBinding> DSLayoutBindings {};
    INT Binding = 0;
    for (INT i = 0; i < MtlNumOfBuffers; i++)
      DSLayoutBindings.push_back(VkDescriptorSetLayoutBinding
        {
          .binding = static_cast<UINT>(Binding++),
          .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
          .descriptorCount = 1,
          .stageFlags = VK_SHADER_STAGE_ALL,
        });
    if (MtlNumOfTextures != 0)
      DSLayoutBindings.push_back(VkDescriptorSetLayoutBinding
        {
          .binding = static_cast<UINT>(Binding++),
          .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
          .descriptorCount = static_cast<UINT>(MtlNumOfTextures),
          .stageFlags = VK_SHADER_STAGE_ALL,
        });
    for (INT i = 0; i < MtlNumOfAttachments; i++)
      DSLayoutBindings.push_back(VkDescriptorSetLayoutBinding
        {
          .binding = static_cast<UINT>(Binding++),
          .descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
          .descriptorCount = 1,
          .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        });
    if (!MaterialDSLayout.Create(DSLayoutBindings))
      logger::warning << "Wanted to create descriptor set layout for material pattern, but couldn't.";
    else
    {
      DescriptorSetLayouts.emplace_back(MaterialDSLayout);
      logger::info << "Created descriptor set layout for material pattern.";
    }
  }
  /* Create pipeline for material pattern */
  if (!PatternPipeline.Create(NewShd->VlkShd, DescriptorSetLayouts, AttribDescrs, Topology, BlendMode, PatchControlPoints))
  {
    logger::error << std::format("Error creating {}: pipeline creation error.", MtlPatName);
    Free();
  }
  else
    Shader = NewShd;
  return *this;
} /* End of 'tam::material_pattern_res::Create' function */

/* Free material pattern function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::material_pattern_res::Free( VOID )
{
  MtlNumOfBuffers = 0;
  PatternBuffers.clear();
  MtlNumOfTextures = 0;
  PatternTextures.clear();
  PatternDS.Destroy();
  ApplyDescriptors.clear();
  for (INT i = 0; i < DescriptorSetLayouts.size(); i++)
    // Skip first global descriptor set layout
    if (i != 0)
      DescriptorSetLayouts[i].Destroy();
  DescriptorSetLayouts.clear();
  PatternPipeline.Destroy();
  MaterialDescriptorNo = 0;
} /* End of 'tam::material_pattern_res::Free' function */

/* Apply material pattern function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::material_pattern_res::Apply( VOID )
{
  if (PatternPipeline.Pipeline != VK_NULL_HANDLE)
  {
    vkCmdBindPipeline(Rnd->CommandBuffers[Rnd->CurrentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, PatternPipeline.Pipeline);
    if (ApplyDescriptors.size() > 0)
      vkCmdBindDescriptorSets(Rnd->CommandBuffers[Rnd->CurrentFrame],
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        PatternPipeline.Layout,
        0,
        static_cast<UINT>(ApplyDescriptors.size()),
        ApplyDescriptors.data(),
        0,
        nullptr
      );
  }
} /* End of 'tam::material_pattern_res::Apply' function */

/* Update material pattern function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::material_pattern_res::Update( VOID )
{
  if (Shader->IsChanged)
    if (!PatternPipeline.Update(Shader->VlkShd))
      logger::error << "Error updating " >> 0x88AAFF << Name >> 0x33FA88 << '.';
} /* End of 'tam::material_pattern_res::Update' function */

/* Update material patterns function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::material_pattern_manager::MaterialPatternsUpdate( VOID )
{
  for (const auto &MtlPat :Stock)
    MtlPat.second.get()->Update();
} /* End of 'tam::material_pattern_manager::MaterialPatternsUpdate' function */

/* END OF 'tam_res_material_pattern.cpp' FILE */
