/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_descriptors.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Descriptor sets and layouts implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 22.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Create descriptor set layout function.
 * ARGUMENTS:
 *   - descriptor set layout bindings:
 *       std::vector<VkDescriptorSetLayoutBinding> DescriptorSetLayoutBindings;
 * RETURNS:
 *   (BOOL) TRUE if descriptor set layout was successfully created, FALSE otherwise.
 */
BOOL tam::vlk::descriptor_set_layout::Create( std::vector<VkDescriptorSetLayoutBinding> DSLayoutBindings )
{
  VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutCreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    .flags = 0,
    .bindingCount = (UINT32)std::size(DSLayoutBindings),
    .pBindings = DSLayoutBindings.data(),
  };
  DescriptorSetLayoutBindings = DSLayoutBindings;

  if (vkCreateDescriptorSetLayout(VlkPtr->Device, &DescriptorSetLayoutCreateInfo, nullptr, &DescriptorSetLayout) != VK_SUCCESS)
  {
    logger::error << "Unable to create descriptor set layout.";
    return FALSE;
  }
  // logger::info << "Descriptor set layout created successfully.";
  return TRUE;
} /* End of 'tam::vlk::descriptor_set_layout::Create' function */

/* Destroy descriptor set layout function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::descriptor_set_layout::Destroy( VOID )
{
  if (DescriptorSetLayout != VK_NULL_HANDLE && VlkPtr->Device != VK_NULL_HANDLE)
  {
    vkDestroyDescriptorSetLayout(VlkPtr->Device, DescriptorSetLayout, nullptr);
    DescriptorSetLayout = VK_NULL_HANDLE;
  }
} /* End of 'tam::vlk::descriptor_set_layout::Destroy' function */

/* Create descriptor set (attachments) function.
  * ARGUMENTS:
  *   - descriptor set layout:
  *       const descriptor_set_layout &DescriptorSetLayout;
  *   - image views of images to be attached:
  *       std::vector<VkImageView> ImageViews;
  * RETURNS:
  *   (BOOL) depending on result.
  */
BOOL tam::vlk::descriptor_set::Create( const descriptor_set_layout &DescriptorSetLayout, std::vector<VkImageView> ImageViews )
{
  vkDeviceWaitIdle(VlkPtr->Device);
  VkDescriptorSetAllocateInfo AllocInfo
  {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    .descriptorPool = VlkPtr->DescriptorPool,
    .descriptorSetCount = 1,
    .pSetLayouts = &DescriptorSetLayout.DescriptorSetLayout,
  };
  VkResult Res {};
  if ((Res = vkAllocateDescriptorSets(VlkPtr->Device, &AllocInfo, &DescriptorSet)) != VK_SUCCESS)
  {
    tam::logger::error << "Error creating descriptor set: " + std::to_string((UINT)Res);
    return FALSE;
  }

  DescriptorWrites = {};
  std::vector<VkDescriptorBufferInfo> BufferInfos {};
  UINT AttachmentIndex = 0, i = 0;
  std::vector<VkDescriptorImageInfo> ImageInfos {};
  if (ImageViews.size() != 0)
  {
    VkDescriptorType ImageDescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    for (INT j = 0; j < ImageViews.size(); j++)
    {
      VkDescriptorImageInfo ImageInfo {};
      if (ImageDescriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)
        ImageInfo.sampler = VlkPtr->DefSampler.Sampler;
      else
        ImageInfo.sampler = VK_NULL_HANDLE;
      ImageInfo.imageView = ImageViews[j];
      ImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      ImageInfos.push_back(ImageInfo);
    }
    VkWriteDescriptorSet DescriptorWrite
    {
      .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .pNext = nullptr,
      .dstSet = DescriptorSet,
      .dstBinding = i,
      .dstArrayElement = 0,
      .descriptorCount = (UINT)ImageInfos.size(),
      .descriptorType = ImageDescriptorType,
      .pImageInfo = ImageInfos.data(),
    };
    DescriptorWrites.push_back(DescriptorWrite);
  }
  vkUpdateDescriptorSets(VlkPtr->Device, (UINT)DescriptorWrites.size(), DescriptorWrites.data(), 0, nullptr);
  // tam::logger::info << "Attachment descriptor set created successfully.";
  return TRUE;
} /* End of 'tam::vlk::descriptor_set::Create' function */

/* Create descriptor set function.
 * ARGUMENTS:
 *   - descriptor set layout:
 *       const descriptor_set_layout &DescriptorSetLayout;
 *   - buffers to attach:
 *       std::vector<buffer *> Buffers;
 *   - image views of images to be attached:
 *       std::vector<VkImageView> ImageViews;
 *   - vector of image samplers:
 *       std::vector<sampler> Samplers;
 * RETURNS:
 *   (BOOL) depending on result.
 */
BOOL tam::vlk::descriptor_set::Create( const descriptor_set_layout &DescriptorSetLayout,
  const std::vector<buffer *> &Buffers, const std::vector<VkImageView> &ImageViews, const std::vector<sampler> &Samplers, const std::vector<std::pair<VkImageView, VkImageLayout>> &AttachmentsIsColor, const std::vector<sampler> &AttSamplers )
{
  vkDeviceWaitIdle(VlkPtr->Device);
  VkDescriptorSetAllocateInfo AllocInfo
  {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    .descriptorPool = VlkPtr->DescriptorPool,
    .descriptorSetCount = 1,
    .pSetLayouts = &DescriptorSetLayout.DescriptorSetLayout,
  };
  VkResult Res {};
  if ((Res = vkAllocateDescriptorSets(VlkPtr->Device, &AllocInfo, &DescriptorSet)) != VK_SUCCESS)
  {
    tam::logger::error << "Error creating descriptor set: " + std::to_string((UINT)Res);
    return FALSE;
  }

  DescriptorWrites = {};
  std::vector<VkDescriptorBufferInfo> BufferInfos {};
  BufferInfos.resize(Buffers.size());
  UINT AttachmentIndex = 0, i = 0;
  for (auto Buf : Buffers)
  {
    if (Buf == nullptr)
      continue;
    BufferInfos[i].buffer = Buf->Buffer;
    BufferInfos[i].offset = 0;
    BufferInfos[i].range = Buf->Size;
    VkWriteDescriptorSet DescriptorWrite
    {
      .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .pNext = nullptr,
      .dstSet = DescriptorSet,
      .dstBinding = i,
      .dstArrayElement = 0,
      .descriptorCount = 1,
      .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
      .pBufferInfo = &BufferInfos[i],
    };
    i++;
    DescriptorWrites.push_back(DescriptorWrite);
  };
  std::vector<VkDescriptorImageInfo> ImageInfos {};
  std::vector<VkDescriptorImageInfo> AttachmentInfos {};
  if (ImageViews.size() != 0)
  {
    VkDescriptorType ImageDescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    for (INT j = 0; j < ImageViews.size(); j++)
    {
      VkDescriptorImageInfo ImageInfo {};
      if (ImageDescriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
        ImageInfo.sampler = VlkPtr->DefSampler.Sampler;
      else
        ImageInfo.sampler = VK_NULL_HANDLE;
      ImageInfo.imageView = ImageViews[j];
      ImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      ImageInfos.push_back(ImageInfo);
    }
    VkWriteDescriptorSet DescriptorWrite
    {
      .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .pNext = nullptr,
      .dstSet = DescriptorSet,
      .dstBinding = i,
      .dstArrayElement = 0,
      .descriptorCount = (UINT)ImageInfos.size(),
      .descriptorType = ImageDescriptorType,
      .pImageInfo = ImageInfos.data(),
    };
    DescriptorWrites.push_back(DescriptorWrite);
  }
  if (AttachmentsIsColor.size() != 0)
  {
    VkDescriptorType AttachmentDescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    for (INT j = 0; j < AttachmentsIsColor.size(); j++)
    {
      VkDescriptorImageInfo AttachInfo {};
      if (AttachmentDescriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)
        AttachInfo.sampler = VlkPtr->DefSampler.Sampler;
      else
        AttachInfo.sampler = VK_NULL_HANDLE;
      AttachInfo.imageView = AttachmentsIsColor[j].first;
      AttachInfo.imageLayout = AttachmentsIsColor[j].second;
      AttachmentInfos.push_back(AttachInfo);
    }
    VkWriteDescriptorSet AttachDescriptorWrite
    {
      .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .pNext = nullptr,
      .dstSet = DescriptorSet,
      .dstBinding = i,
      .dstArrayElement = 0,
      .descriptorCount = (UINT)AttachmentInfos.size(),
      .descriptorType = AttachmentDescriptorType,
      .pImageInfo = AttachmentInfos.data(),
    };
    DescriptorWrites.push_back(AttachDescriptorWrite);
  }
  vkUpdateDescriptorSets(VlkPtr->Device, (UINT)DescriptorWrites.size(), DescriptorWrites.data(), 0, nullptr);
  // tam::logger::info << "Descriptor set created successfully.";
  return TRUE;
} /* End of 'tam::vlk::descriptor_set::Create' function */

/* Update descriptor set (attachments) function.
 * ARGUMENTS:
 *   - images to be attached:
 *       std::vector<image> Images;
 *   - buffers to be attached:
 *       std::vector<buffer *> &Buffers;
 * RETURNS: None.
 */
VOID tam::vlk::descriptor_set::Update( const std::vector<std::pair<VkImageView, VkImageLayout>> &ImagesIsColor, const std::vector<buffer *> &Buffers )
{
  vkDeviceWaitIdle(VlkPtr->Device);
  std::vector<VkDescriptorBufferInfo> BufferInfos {};
  BufferInfos.resize(Buffers.size());
  UINT AttachmentIndex = 0, i = 0;
  DescriptorWrites = {};
  for (auto Buf : Buffers)
  {
    if (Buf == nullptr)
      continue;
    BufferInfos[i].buffer = Buf->Buffer;
    BufferInfos[i].offset = 0;
    BufferInfos[i].range = Buf->Size;
    VkWriteDescriptorSet DescriptorWrite
    {
      .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .pNext = nullptr,
      .dstSet = DescriptorSet,
      .dstBinding = i,
      .dstArrayElement = 0,
      .descriptorCount = 1,
      .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
      .pBufferInfo = &BufferInfos[i],
    };
    i++;
    DescriptorWrites.push_back(DescriptorWrite);
  };

  std::vector<VkDescriptorImageInfo> ImageInfos {};
  if (ImagesIsColor.size() != 0)
  {
    VkDescriptorType ImageDescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    for (INT j = 0; j < ImagesIsColor.size(); j++)
    {
      VkDescriptorImageInfo ImageInfo {};
      ImageInfo.imageView = ImagesIsColor[j].first;
      ImageInfo.sampler = VlkPtr->DefSampler.Sampler;
      ImageInfo.imageLayout = ImagesIsColor[j].second;
      ImageInfos.push_back(ImageInfo);
    }
    VkWriteDescriptorSet DescriptorWrite
    {
      .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .pNext = nullptr,
      .dstSet = DescriptorSet,
      .dstBinding = i,
      .dstArrayElement = 0,
      .descriptorCount = (UINT)ImageInfos.size(),
      .descriptorType = ImageDescriptorType,
      .pImageInfo = ImageInfos.data(),
    };
    DescriptorWrites.push_back(DescriptorWrite);
  }
  vkUpdateDescriptorSets(VlkPtr->Device, (UINT)DescriptorWrites.size(), DescriptorWrites.data(), 0, nullptr);
  // tam::logger::info << "Attachment descriptor set updated successfully.";
} /* End of 'tam::vlk::descriptor_set::Update' function */

/* Update descriptor set function.
 * ARGUMENTS:
 *   - buffers to attach:
 *       std::vector<buffer *> Buffers;
 *   - image views of images to be attached:
 *       std::vector<VkImageView> ImageViews;
 *   - vector of image samplers:
 *       std::vector<sampler> Samplers;
 * RETURNS: None.
 */
VOID tam::vlk::descriptor_set::Update( std::vector<buffer *> Buffers,
  std::vector<VkImageView> ImageViews, std::vector<sampler> Samplers )
{
  vkDeviceWaitIdle(VlkPtr->Device);
  std::vector<VkDescriptorBufferInfo> BufferInfos {};
  BufferInfos.resize(Buffers.size());
  UINT AttachmentIndex = 0, i = 0;
  DescriptorWrites = {};
  for (auto Buf : Buffers)
  {
    if (Buf == nullptr)
      continue;
    BufferInfos[i].buffer = Buf->Buffer;
    BufferInfos[i].offset = 0;
    BufferInfos[i].range = Buf->Size;
    VkWriteDescriptorSet DescriptorWrite
    {
      .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .pNext = nullptr,
      .dstSet = DescriptorSet,
      .dstBinding = i,
      .dstArrayElement = 0,
      .descriptorCount = 1,
      .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
      .pBufferInfo = &BufferInfos[i],
    };
    i++;
    DescriptorWrites.push_back(DescriptorWrite);
  };
  std::vector<VkDescriptorImageInfo> ImageInfos {};
  if (ImageViews.size() != 0)
  {
    VkDescriptorType ImageDescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    for (INT j = 0; j < ImageViews.size(); j++)
    {
      VkDescriptorImageInfo ImageInfo {};
      if (ImageDescriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
      {
        if (j < Samplers.size())
          ImageInfo.sampler = Samplers[j].Sampler;
        else
          ImageInfo.sampler = VlkPtr->DefSampler.Sampler;
      }
      else
      {
        ImageInfo.sampler = VK_NULL_HANDLE;
      }
      ImageInfo.imageView = ImageViews[j];
      ImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      ImageInfos.push_back(ImageInfo);
    }
    VkWriteDescriptorSet DescriptorWrite
    {
      .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .pNext = nullptr,
      .dstSet = DescriptorSet,
      .dstBinding = i,
      .dstArrayElement = 0,
      .descriptorCount = (UINT)ImageInfos.size(),
      .descriptorType = ImageDescriptorType,
      .pImageInfo = ImageInfos.data(),
    };
    DescriptorWrites.push_back(DescriptorWrite);
  }
  vkUpdateDescriptorSets(VlkPtr->Device, (UINT)DescriptorWrites.size(), DescriptorWrites.data(), 0, nullptr);
  // tam::logger::info << "Descriptor set updated successfully.";
} /* End of 'tam::vlk::descriptor_set::Update' function */

/* Destroy descriptor set function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::descriptor_set::Destroy( VOID )
{
  if (DescriptorSet != VK_NULL_HANDLE && VlkPtr->Device != VK_NULL_HANDLE)
  {
    vkFreeDescriptorSets(VlkPtr->Device, VlkPtr->DescriptorPool, 1, &DescriptorSet);
    DescriptorSet = VK_NULL_HANDLE;
  }
  DescriptorWrites.clear();
} /* End of 'tam::vlk::descriptor_set::Destroy' function */

/* END OF 'tam_vlk_descriptors.cpp' FILE */
