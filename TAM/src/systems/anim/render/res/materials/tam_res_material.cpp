/***************************************************************
 * Copyright (C) 20260
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_material.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Material functions implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3),
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 30.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_res_material.h"

/* Class constructor.
 * ARGUMENTS:
 *   - pointer to the render context:
 *       render *RndPtr;
 */
tam::material_res::material_res( render *RndPtr ) : resource(RndPtr), MtlDescriptorSet(RndPtr)
{
} /* End of 'material_res' function */

/* Class move constructor.
 * ARGUMENTS:
 *   - rvalue reference to material_res:
 *       material_res &&Old;
 */
tam::material_res::material_res( material_res &&Old ) : 
  resource(Old.Rnd),
  Name(Old.Name), MtlPat(Old.MtlPat), Textures(Old.Textures),
  Buffers(Old.Buffers), MtlDescriptorSet(Old.MtlDescriptorSet)
{
  Rnd = Old.Rnd;
  Old.Name = "";
  Old.MtlPat = nullptr;
  Old.StoreFrameNo = 0;
  Old.Textures.clear();
  Old.Buffers.clear();
  Old.MtlDescriptorSet = VK_NULL_HANDLE;
  Old.NextDraw = nullptr;
  Old.FirstDraw = nullptr;
} /* End of 'material_res' function */

/* Material creation function.
 * ARGUMENTS:
 *   - material name:
 *       const std::string &NewName;
 *   - material pattern:
 *       material_pattern NewMtlPat;
 *   - textures vector:
 *       std::vector<texture> Texs;
 *   - buffers vector:
 *       std::vector<buffer> Bufs;
 * RETURNS:
 *   (material_res &) self reference.
 */
tam::material_res & tam::material_res::Create( const std::string &NewName, material_pattern &NewMtlPat,
  const std::vector<texture> &Texs, const std::vector<buffer> &Bufs, const std::vector<texture> &Attachments )
{
  if (NewMtlPat == nullptr)
  {
    logger::error << "Unable to create material without material pattern.";
    return *this;
  }
  Free();
  MtlPat = NewMtlPat;
  Name = NewName;

  if (MtlPat->MtlNumOfBuffers != 0 || MtlPat->MtlNumOfTextures != 0 || MtlPat->MtlNumOfAttachments != 0)
  {
    std::vector<VkImageView> ImageViews {};
    std::vector<vlk::sampler> Samplers {};
    std::vector<std::pair<VkImageView, VkImageLayout>> AttachmentViews {};
    INT i = 0;
    Textures.resize(MtlPat->MtlNumOfTextures);
    for (INT n = COM_MIN(MtlPat->MtlNumOfTextures, (INT)Texs.size()); i < n; i++)
    {
      Textures[i] = Texs[i];
      ImageViews.push_back(Texs[i]->Img.View);
    }
    for (; i < MtlPat->MtlNumOfTextures; i++)
      Textures[i++] = nullptr;

    std::vector<vlk::buffer *> VlkBuffers {};
    i = 0;
    Buffers.resize(MtlPat->MtlNumOfBuffers);
    for (INT n = COM_MIN(MtlPat->MtlNumOfBuffers, (INT)Bufs.size()); i < n; i++)
    {
      Buffers[i] = Bufs[i];
      VlkBuffers.push_back(&Bufs[i]->VlkBuffer);
    }
    for (; i < MtlPat->MtlNumOfBuffers; i++)
      Buffers[i++] = nullptr;
    for (auto &att : Attachments)
      AttachmentViews.push_back({att->Img.View, att->Img.BindLayout});
    MtlDescriptorSet.Create(MtlPat->DescriptorSetLayouts[(UINT)MtlPat->DescriptorSetLayouts.size() - 1],
      VlkBuffers, ImageViews, {}, AttachmentViews);
  }
  return *this;
} /* End of 'tam::material_res::Create' function */

/* Material destroy function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::material_res::Free( VOID )
{
  Name = "";
  MtlPat = nullptr;
  StoreFrameNo = 0;
  MtlDescriptorSet.Destroy();
  Textures.clear();
  Buffers.clear();
  NextDraw = nullptr;
  FirstDraw = nullptr;
} /* End of 'tam::material_res::Free' function */

/* Apply material function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::material_res::Apply( VOID )
{
  if (MtlDescriptorSet.DescriptorSet != VK_NULL_HANDLE)
    vkCmdBindDescriptorSets(Rnd->CommandBuffers[Rnd->CurrentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS,
      MtlPat->PatternPipeline.Layout,  MtlPat->MaterialDescriptorNo, 1, &MtlDescriptorSet.DescriptorSet, 0, nullptr);
} /* End of 'tam::material_res::Apply' function */

/* Update material buffers and textures function.
 * ARGUMENTS:
 *   - textures vector:
 *        const std::vector<texture> &Texs;
 *   - buffers vector:
 *        const std::vector<buffer> &Bufs;
 *   - attachments vector:
 *        std::vector<texture> &Attachments;
 * RETURNS: None.
 */
VOID tam::material_res::Update( const std::vector<texture> &Texs, const std::vector<buffer> &Bufs, const std::vector<texture> &Attachments)
{
  if (MtlPat->MtlNumOfBuffers != 0 || MtlPat->MtlNumOfTextures != 0 || MtlPat->MtlNumOfAttachments != 0)
  {
    std::vector<VkImageView> ImageViews {};
    std::vector<vlk::sampler> Samplers {};
    std::vector<std::pair<VkImageView, VkImageLayout>> AttachmentViews {};
    INT i = 0;
    Textures.resize(MtlPat->MtlNumOfTextures);
    for (INT n = COM_MIN(MtlPat->MtlNumOfTextures, (INT)Texs.size()); i < n; i++)
    {
      if (Texs[i] == nullptr)
        break;
      Textures[i] = Texs[i];
      ImageViews.push_back(Texs[i]->Img.View);
    }
    for (; i < MtlPat->MtlNumOfTextures; i++)
      Textures[i++] = nullptr;

    std::vector<vlk::buffer *> VlkBuffers {};
    i = 0;
    Buffers.resize(MtlPat->MtlNumOfBuffers);
    for (INT n = COM_MIN(MtlPat->MtlNumOfBuffers, (INT)Bufs.size()); i < n; i++)
    {
      if (Bufs[i] == nullptr)
        break;
      Buffers[i] = Bufs[i];
      VlkBuffers.push_back(&Bufs[i]->VlkBuffer);
    }
    for (; i < MtlPat->MtlNumOfBuffers; i++)
      Buffers[i++] = nullptr;
    for (auto &att : Attachments)
      AttachmentViews.push_back({att->Img.View, att->Img.BindLayout});
    MtlDescriptorSet.Create(MtlPat->DescriptorSetLayouts[(UINT)MtlPat->DescriptorSetLayouts.size() - 1],
      VlkBuffers, ImageViews, {}, AttachmentViews);
  }
} /* End of 'tam::material_res::Update' function */

/* END OF 'tam_res_material.cpp' FILE */
