/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_images.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Image implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kupchenkov Ivan (IK4).
 * LAST UPDATE : 14.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Vulkan image creation function.
 * ARGUMENTS:
 *   - image size:
 *       INT W, H;
 *   - image color flag (otherwise Image is depth buffer):
 *       BOOL IsColor,
 *   - cube map flag (create 6 layers and 'PixelData' contain 6 joined same size images):
 *       BOOL IsCube,
 *   - vulkan image Format (see VK_FORMAT_***):
 *       VkFormat Format;
 *   - vulkan image usage flags bitwise combination (VK_IMAGE_USAGE_***):
 *       VkImageUsageFlags UsageFlags;
 *   - vulkan image layout (see VK_IMAGE_LAYOUT_***):
 *       VkImageLayout Layout;
 *   - image pixel data:
 *       const VOID *PixelsData;
 *   - image pixel data size (in bytes):
 *       UINT PixelsDataSize;
 *   - number of mipmaps to be create:
 *       UINT MipCount;
 * RETURNS:
 *   (BOOL) success creation result.
 */
BOOL tam::vlk::image::Create( INT ImgW, INT ImgH, BOOL IsColor, BOOL IsCube, VkFormat ImgFormat, VkImageUsageFlags ImgUsageFlags, VkImageLayout ImgLayout, const VOID* PixelsData, UINT PixelsDataSize, UINT MipCount )
{
  if (MipCount < 1)
    MipCount = 1;
  W = COM_MAX(ImgW, 1);
  H = COM_MAX(ImgH, 1);
  Format = ImgFormat;
  UsageFlags = ImgUsageFlags;
  Layout = ImgLayout;
  MipLevels = MipCount;
  IsCubeImg = IsCube;
  IsColorImg = IsColor;

  // Create vulkan image
  VkImageCreateInfo ImageCreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
    .flags = IsCube ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : VkImageCreateFlags(0),
    .imageType = VK_IMAGE_TYPE_2D,
    .format = Format,
    .extent {.width = (UINT)W, .height = (UINT)H, .depth = 1 },
    .mipLevels = MipCount,
    .arrayLayers = IsCube ? 6U : 1U,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .tiling = VK_IMAGE_TILING_OPTIMAL,
    .usage = UsageFlags,
    .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .initialLayout = Layout,
  };
  VmaAllocationCreateInfo AllocationCreateInfo {};
  AllocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
  AllocationCreateInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
  AllocationCreateInfo.flags = 0;
  AllocationCreateInfo.priority = 1.0f;
  if (vmaCreateImage(VlkPtr->Allocator, &ImageCreateInfo, &AllocationCreateInfo, &Image, &Allocation, nullptr) != VK_SUCCESS )
  {
    tam::logger::error << "Error in vmaCreateImage";
    return FALSE;
  }

  // Create image view
  VkImageViewCreateInfo ImageViewCreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .pNext = nullptr,
    .image = Image,
    .viewType = IsCube ? VK_IMAGE_VIEW_TYPE_CUBE : VK_IMAGE_VIEW_TYPE_2D,
    .format = Format,
    .components = {.r = VK_COMPONENT_SWIZZLE_IDENTITY, .g = VK_COMPONENT_SWIZZLE_IDENTITY, .b = VK_COMPONENT_SWIZZLE_IDENTITY, .a = VK_COMPONENT_SWIZZLE_IDENTITY},
    .subresourceRange
    {
      .aspectMask = VkImageAspectFlags(IsColor ? VK_IMAGE_ASPECT_COLOR_BIT : VK_IMAGE_ASPECT_DEPTH_BIT),
      .baseMipLevel = 0,
      .levelCount = MipCount,
      .baseArrayLayer = 0,
      .layerCount = IsCube ? 6U : 1U,
    }
  };
  vkCreateImageView(VlkPtr->Device, &ImageViewCreateInfo, nullptr, &View);

  // Write pixels data to image
  if (PixelsData != nullptr)
  {
    vkResetCommandBuffer(VlkPtr->TransferCommandBuffer, 0);

    // Writing pixels data to staging buffer
    VlkPtr->StagingBuffer.Update((VOID *)PixelsData, PixelsDataSize, 0);

    VkCommandBufferBeginInfo CommandBufferBeginInfo
    {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };
    vkBeginCommandBuffer(VlkPtr->TransferCommandBuffer, &CommandBufferBeginInfo);

    // First image memory barrier
    VkImageMemoryBarrier ImageMemoryBarrier1
    {
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      .srcAccessMask = 0,
      .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
      .oldLayout = ImgLayout,
      .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .image = Image,
      .subresourceRange
      {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = MipCount,
        .baseArrayLayer = 0,
        .layerCount = IsCube ? 6U : 1U,
      },
    };
    vkCmdPipelineBarrier(VlkPtr->TransferCommandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier1);

    VkBufferImageCopy Region
    {
      .bufferOffset = 0,
      .bufferRowLength = (UINT)W,
      .bufferImageHeight = (UINT)H,
      .imageSubresource
      {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .mipLevel = 0,
        .baseArrayLayer = 0,
        .layerCount = IsCube ? 6U : 1U,
      },
      .imageOffset {.x = 0, .y = 0, .z = 0},
      .imageExtent {.width = (UINT)W, .height = (UINT)H, .depth = 1},
    };

    // Copy pixels data from staging buffer to image
    vkCmdCopyBufferToImage(VlkPtr->TransferCommandBuffer, VlkPtr->StagingBuffer.Buffer, Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &Region);

    
    if (MipCount == 1)
    {
      // Second image memory barrier
      VkImageMemoryBarrier ImageMemoryBarrier2
      {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
        .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .newLayout = IsColor ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = Image,
        .subresourceRange
        {
          .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
          .baseMipLevel = 0,
          .levelCount = MipCount,
          .baseArrayLayer = 0,
          .layerCount = IsCube ? 6U : 1U,
        },
      };
      vkCmdPipelineBarrier(VlkPtr->TransferCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier2);
    }
    else
    {
      // Generating mipmaps for image
      GenerateMipmaps(W, H, MipCount, Format, IsCube);
    }

    vkEndCommandBuffer(VlkPtr->TransferCommandBuffer);

    VkPipelineStageFlags PipelineStageFlags[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    VkSubmitInfo SubmitInfo
    {
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .waitSemaphoreCount = 0,
      .pWaitDstStageMask = PipelineStageFlags,
      .commandBufferCount = 1,
      .pCommandBuffers = &VlkPtr->TransferCommandBuffer,
    };

    vkResetFences(VlkPtr->Device, 1, &VlkPtr->TransferFence);
    vkQueueSubmit(VlkPtr->TransferQueue, 1, &SubmitInfo, VlkPtr->TransferFence);
    vkWaitForFences(VlkPtr->Device, 1, &VlkPtr->TransferFence, VK_TRUE, UINT64_MAX);
    vkResetFences(VlkPtr->Device, 1, &VlkPtr->TransferFence);
    Layout = IsColor ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
  }
  BindLayout = IsColor ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
  return TRUE;
} /* End of 'tam::vlk::image::Create' function */

/* Vulkan image free function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::image::Destroy( VOID )
{
  if (View != VK_NULL_HANDLE)
    vkDestroyImageView(VlkPtr->Device, View, nullptr);
  if (Image != VK_NULL_HANDLE && Allocation != VK_NULL_HANDLE)
    vmaDestroyImage(VlkPtr->Allocator, Image, Allocation);

  View = VK_NULL_HANDLE;
  Image = VK_NULL_HANDLE;
  Allocation = VK_NULL_HANDLE;
  ///Sampler.Destroy();
} /* End of 'tam::vlk::image::Destroy' function */

/* Vulkan image set layout function.
 * Create an image memory 'ImageMemoryBarrier' for changing the layout of Image and put it into an active command buffer
 * ARGUMENTS:
 *   - new vulkan layout (see VK_IMAGE_LAYOUT_***):
 *       VkImageLayout NewImageLayout;
 *   - vulkan image subrsourcse description:
 *       VkImageSubresourceRange SubresourceRange;
 *   - vulkan pipeline source and destination stage flags (see VK_PIPELINE_STAGE_***):
 *       VkPipelineStageFlags SrcStageMask, DstStageMask;
 * RETURNS: None.
 */
VOID tam::vlk::image::SetLayout( VkImageLayout NewImageLayout, VkImageSubresourceRange SubresourceRange, VkPipelineStageFlags SrcStageMask, VkPipelineStageFlags DstStageMask )
{
  // Image memory barrier
  VkImageMemoryBarrier ImageMemoryBarrier
  {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .oldLayout = Layout,
    .newLayout = NewImageLayout,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .image = Image,
    .subresourceRange = SubresourceRange,
  };
  Layout = NewImageLayout;

  VkPipelineStageFlags PipelineStageFlags = 0;

  if (NewImageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
  {
    ImageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    PipelineStageFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
  }

  else if(NewImageLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
  {
    ImageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    ImageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    PipelineStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  }

  else if(NewImageLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
  {
    ImageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    ImageMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    PipelineStageFlags = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  }
 
  vkCmdPipelineBarrier(VlkPtr->TransferCommandBuffer, SrcStageMask, DstStageMask, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);
} /* End of 'tam::vlk::image::SetLayout' function */

/* Generate mipmaps for vulkan image function.
 * ARGUMENTS:
 *   - image size:
 *       INT W, H;
*   - number of mipmaps to be create:
*       UINT Levels;
 *   - vulkan Image Format (see VK_FORMAT_***):
 *       VkFormat Format;
 *   - cube map flag (create 6 layers and 'PixelData' contain 6 joined same size images):
 *       BOOL IsCube;
 * RETURNS: None.
 */
VOID tam::vlk::image::GenerateMipmaps( INT ImgW, INT ImgH, UINT Levels, VkFormat ImgFormat, BOOL IsCube )
{
  W = COM_MAX(ImgW, 1);
  H = COM_MAX(ImgH, 1);
  Format = ImgFormat;
  MipLevels = Levels;
  IsCubeImg = IsCube;

  if (Levels == 1)
    return;

  VkFormatProperties FormatProperties;
  vkGetPhysicalDeviceFormatProperties(VlkPtr->PhysDevice, Format, &FormatProperties);

  VkCommandBufferBeginInfo CommandBufferBeginInfo
  {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
  };
  //vkBeginCommandBuffer(VlkPtr->TransferCommandBuffer, &CommandBufferBeginInfo);

  // Image memory barriers for mip levels
  VkImageMemoryBarrier ImageMemoryBarrier
  {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .image = Image,
    .subresourceRange
    {
      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      .baseMipLevel = 0,
      .levelCount = 1,
      .baseArrayLayer = 0,
      .layerCount = IsCube ? 6U : 1U,
    },
  };

  ImageMemoryBarrier.subresourceRange.baseMipLevel = 0;
  ImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
  ImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  ImageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
  ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  
  //vkCmdPipelineBarrier(VlkPtr->TransferCommandBuffer,
  //  VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
  //  0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);

  //for (UINT MipLevel = 1; MipLevel < Levels; MipLevel++)
  //{
  //  ImageMemoryBarrier.subresourceRange.baseMipLevel = MipLevel;
  //  ImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  //  ImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  //  ImageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
  //  ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

  //  // Barrier for mip level
  //  vkCmdPipelineBarrier(VlkPtr->TransferCommandBuffer,
  //    VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
  //    0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);
  //}

  INT MipWidth = W, MipHeight = H;
  for (UINT MipLevel = 1; MipLevel < Levels; MipLevel++)
  {
    ImageMemoryBarrier.subresourceRange.baseMipLevel = MipLevel - 1;
    ImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    ImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    ImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

    vkCmdPipelineBarrier(VlkPtr->TransferCommandBuffer,
      VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
      0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);

    // Image blit
    VkImageBlit Blit
    {
      .srcSubresource
      {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .mipLevel = MipLevel - 1,
        .baseArrayLayer = 0,
        .layerCount = IsCube ? 6U : 1U,
      },
      .srcOffsets
      {
        {.x = 0, .y = 0, .z = 0},
        {.x = MipWidth, .y = MipHeight, .z = 1},
      },
      .dstSubresource
      {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .mipLevel = MipLevel,
        .baseArrayLayer = 0,
        .layerCount = IsCube ? 6U : 1U,
      },
      .dstOffsets
      {
        {.x = 0, .y = 0, .z = 0},
        {.x = MipWidth > 1 ? MipWidth / 2 : 1, .y = MipHeight > 1 ? MipHeight / 2 : 1, .z = 1},
      },
    };
    vkCmdBlitImage(VlkPtr->TransferCommandBuffer, Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &Blit, VK_FILTER_LINEAR);

    ImageMemoryBarrier.subresourceRange.baseMipLevel = MipLevel - 1;
    ImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    ImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    ImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    vkCmdPipelineBarrier(VlkPtr->TransferCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);

    if (MipWidth > 1)
      MipWidth /= 2;
    if (MipHeight > 1)
      MipHeight /= 2;
  }

  for (UINT MipLevel = 0; MipLevel < Levels; MipLevel++)
  {
    ImageMemoryBarrier.subresourceRange.baseMipLevel = MipLevel;
    ImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    ImageMemoryBarrier.newLayout = IsColorImg ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    ImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    ImageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    vkCmdPipelineBarrier(VlkPtr->TransferCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier);
  }

  //vkEndCommandBuffer(VlkPtr->TransferCommandBuffer);
  
  VkPipelineStageFlags PipelineStageFlags[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  VkSubmitInfo SubmitInfo
  {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .waitSemaphoreCount = 0,
    .pWaitDstStageMask = PipelineStageFlags,
    .commandBufferCount = 1,
    .pCommandBuffers = &VlkPtr->TransferCommandBuffer,
  };
  //vkWaitForFences(VlkPtr->Device, 1, &VlkPtr->Fences[VlkPtr->CurrentFrame], VK_TRUE, UINT64_MAX);
  //vkResetFences(VlkPtr->Device, 1, &VlkPtr->Fences[VlkPtr->CurrentFrame]);
  //vkQueueSubmit(VlkPtr->TransferQueue, 1, &SubmitInfo, VlkPtr->Fences[VlkPtr->CurrentFrame]);
  vkQueueWaitIdle(VlkPtr->TransferQueue);
  //vkWaitForFences(VlkPtr->Device, 1, &VlkPtr->Fences[VlkPtr->CurrentFrame], VK_TRUE, UINT64_MAX);
} /* End of 'tam::vlk::image::GenerateMipmaps' function */

/* Vulkan image set layout (on first mip level and layer) function.
 * ARGUMENTS:
 *   - new vulkan layout (see VK_IMAGE_LAYOUT_***):
 *       VkImageLayout NewImageLayout;
 *   - vulkan image subrsourcse description:
 *       VkImageSubresourceRange SubresourceRange;
 *   - vulkan pipeline source and destination stage flags (see VK_PIPELINE_STAGE_***):
 *       VkPipelineStageFlags SrcStageMask, DstStageMask;
 * RETURNS: None.
 */
VOID tam::vlk::image::SetLayout( VkImageAspectFlags AspectMask, VkImageLayout NewImageLayout, VkPipelineStageFlags SrcStageMask, VkPipelineStageFlags DstStageMask )
{
  VkImageSubresourceRange SubresourceRange = {};
  SubresourceRange.aspectMask = AspectMask;
  SubresourceRange.baseMipLevel = 0;
  SubresourceRange.levelCount = 1;
  SubresourceRange.layerCount = 1;
  SetLayout(NewImageLayout, SubresourceRange, SrcStageMask, DstStageMask);
} /* End of 'tam::vlk::image::SetLayout' function */

/* Update pixels data for vulkan image function.
 * ARGUMENTS:
 *   - image pixel data:
 *       const VOID *PixelsData;
 *   - image pixel data size (in bytes):
 *       UINT PixelsDataSize;
 * RETURNS: None.
 */
VOID tam::vlk::image::UpdateData( const VOID *PixelsData, UINT PixelsDataSize )
{
  // Updating pixels data
  if (PixelsData != nullptr)
  {
    vkResetCommandBuffer(VlkPtr->TransferCommandBuffer, 0);

    // Writing pixels data to staging buffer
    VlkPtr->StagingBuffer.Update((VOID *)PixelsData, PixelsDataSize, 0);

    VkCommandBufferBeginInfo CommandBufferBeginInfo
    {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };
    vkBeginCommandBuffer(VlkPtr->TransferCommandBuffer, &CommandBufferBeginInfo);

    VkImageMemoryBarrier ImageMemoryBarrier1
    {
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      .srcAccessMask = 0,
      .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
      .oldLayout = Layout,
      .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .image = Image,
      .subresourceRange
      {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = MipLevels,
        .baseArrayLayer = 0,
        .layerCount = IsCubeImg ? 6U : 1U,
      },
    };
    vkCmdPipelineBarrier(VlkPtr->TransferCommandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier1);

    VkBufferImageCopy Region
    {
      .bufferOffset = 0,
      .bufferRowLength = (UINT)W,
      .bufferImageHeight = (UINT)H,
      .imageSubresource
      {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .mipLevel = 0,
        .baseArrayLayer = 0,
        .layerCount = IsCubeImg ? 6U : 1U,
      },
      .imageOffset {.x = 0, .y = 0, .z = 0},
      .imageExtent {.width = (UINT)W, .height = (UINT)H, .depth = 1},
    };

    // Copy pixels data from staging buffer to image
    vkCmdCopyBufferToImage(VlkPtr->TransferCommandBuffer, VlkPtr->StagingBuffer.Buffer, Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &Region);

    if (MipLevels == 1)
    {
      // Second image memory barrier
      VkImageMemoryBarrier ImageMemoryBarrier2
      {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
        .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = Image,
        .subresourceRange
        {
          .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
          .baseMipLevel = 0,
          .levelCount = MipLevels,
          .baseArrayLayer = 0,
          .layerCount = 1,
        },
      };
      vkCmdPipelineBarrier(VlkPtr->TransferCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &ImageMemoryBarrier2);
    }
    else
    {
      // Generating mipmaps for image
      GenerateMipmaps(W, H, MipLevels, Format, IsCubeImg);
    }

    vkEndCommandBuffer(VlkPtr->TransferCommandBuffer);
    VkPipelineStageFlags PipelineStageFlags[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    VkSubmitInfo SubmitInfo
    {
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .waitSemaphoreCount = 0,
      .pWaitDstStageMask = PipelineStageFlags,
      .commandBufferCount = 1,
      .pCommandBuffers = &VlkPtr->TransferCommandBuffer,
    };
    
    vkResetFences(VlkPtr->Device, 1, &VlkPtr->TransferFence);
    vkQueueSubmit(VlkPtr->TransferQueue, 1, &SubmitInfo, VlkPtr->TransferFence);
    vkWaitForFences(VlkPtr->Device, 1, &VlkPtr->TransferFence, VK_TRUE, UINT64_MAX);
  }
} /* End of 'tam::vlk::image::UpdateData' function */

/* Vulkan image creation function.
 * ARGUMENTS:
 *   - image size:
 *       INT ImgW, ImgH;
 *   - image pixel data:
 *       const VOID *PixelsData;
 *   - image pixel data size (in bytes):
 *       UINT PixelsDataSize;
 * RETURNS:
 *   (BOOL) success result.
 */
BOOL tam::vlk::image::Resize( INT ImgW, INT ImgH, const VOID *PixelsData, UINT PixelsDataSize )
{
  Destroy();
  Create(ImgW, ImgH, IsColorImg, IsCubeImg, Format, UsageFlags, Layout, PixelsData, PixelsDataSize, MipLevels);
  return TRUE;
} /* End of 'tam::vlk::image::Resize' function */

/* Get pixels data from vulkan image function.
 * ARGUMENTS:
 *   - image pixel data:
 *       const VOID *PixelsData;
 *   - image pixel data size (in bytes):
 *       UINT PixelsDataSize;
 * RETURNS: None.
 */
VOID tam::vlk::image::GetData( VOID *PixelsData, UINT PixelsDataSize )
{
  VkCommandBufferBeginInfo CommandBufferBeginInfo
  {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
  };
  vkBeginCommandBuffer(VlkPtr->TransferCommandBuffer, &CommandBufferBeginInfo);

  // Before copy
  VkImageMemoryBarrier ToTransferBarrier
  {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
    .dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT,
    .oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    .newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .image = Image,
    .subresourceRange
    {
      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      .levelCount = 1,
      .layerCount = IsCubeImg ? 6U : 1U,
    }
  };

  vkCmdPipelineBarrier(VlkPtr->TransferCommandBuffer, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
    VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &ToTransferBarrier);

  VkBufferImageCopy Region
  {
    .bufferOffset = 0,
    .bufferRowLength = 0,
    .bufferImageHeight = 0,
    .imageSubresource
    {
      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      .mipLevel = 0,
      .baseArrayLayer = 0,
      .layerCount = IsCubeImg ? 6U : 1U,
    },
    .imageOffset {.x = 0, .y = 0, .z = 0},
    .imageExtent {.width = (UINT)W, .height = (UINT)H, .depth = 1},
  };

  // Check required size for srtaging buffer
  UINT requiredSize = (UINT)(W * H * 4 * (IsCubeImg ? 6U : 1U));
  if (VlkPtr->StagingBuffer.Size < requiredSize)
    VlkPtr->StagingBuffer.Resize(requiredSize);

  // Copy image data to staging buffer
  vkCmdCopyImageToBuffer(VlkPtr->TransferCommandBuffer, Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VlkPtr->StagingBuffer.Buffer, 1, &Region);

  // After copy
  VkImageMemoryBarrier ToShaderBarrier
  {
    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT,
    .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
    .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
    .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .image = Image,
    .subresourceRange
    {
    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
    .levelCount = 1,
    .layerCount = IsCubeImg ? 6U : 1U,
    }
  };

  vkCmdPipelineBarrier(VlkPtr->TransferCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
    VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &ToShaderBarrier);

  vkEndCommandBuffer(VlkPtr->TransferCommandBuffer);

  VkSubmitInfo submit
  {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .commandBufferCount = 1,
    .pCommandBuffers = &VlkPtr->TransferCommandBuffer,
  };

  vkQueueSubmit(VlkPtr->TransferQueue, 1, &submit, VK_NULL_HANDLE);
  vkQueueWaitIdle(VlkPtr->TransferQueue);

  // Read pixels data from staging buffer
  VlkPtr->StagingBuffer.Read(PixelsData, PixelsDataSize, 0);
} /* End of 'tam::vlk::image::GetData' function */

/* END OF 'tam_vlk_images.cpp' FILE */
