/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_swapchain.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Swapchain functions implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 14.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Create swapchain function.
 * ARGUMENTS:
 *   - old swapchain (optional):
 *       VkSwapchainKHR OldSwapchain;
 * RETURNS:
 *   (BOOL) TRUE if swapchain was successfully created, FALSE otherwise.
 */
BOOL tam::vlk::swapchain::Create( VkSwapchainKHR OldSwapchain )
{
  /* Get window actual sizes */
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VlkRef->PhysDevice, VlkRef->Surface, &(VlkRef->SurfaceCapabilities));
  VkSwapchainCreateInfoKHR SwapchainCreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .flags = 0,
    .surface = VlkRef->Surface,
    .minImageCount = VlkRef->UsedFramesOnTheFly,
    .imageFormat = VlkRef->SwapchainImageFormat,
    .imageColorSpace = VlkRef->SwapchainColorSpace,
    .imageExtent
    {
      .width = (UINT)VlkRef->SurfaceCapabilities.currentExtent.width,
      .height = (UINT)VlkRef->SurfaceCapabilities.currentExtent.height,
    },
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
    .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = nullptr,
    .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    .presentMode = VlkRef->PresentMode, // VK_PRESENT_MODE_MAILBOX_KHR
    .clipped = VK_TRUE, // VK_FALSE
    .oldSwapchain = OldSwapchain,
  };

  if (vkCreateSwapchainKHR(VlkRef->Device, &SwapchainCreateInfo, nullptr, &Swapchain) != VK_SUCCESS)
  {
    logger::error << "Unable to create swapchain.";
    return FALSE;
  }

  if (vkGetSwapchainImagesKHR(VlkRef->Device, Swapchain, &SwapchainImagesCount, nullptr) != VK_SUCCESS)
  {
    logger::error << "Unable to count swapchain images.";
    return FALSE;
  }

  Images.resize(SwapchainImagesCount);
  if (vkGetSwapchainImagesKHR(VlkRef->Device, Swapchain, &SwapchainImagesCount, Images.data()) != VK_SUCCESS)
  {
    logger::error << "Unable to get swapchain images.";
    return FALSE;
  }

  ImageViews.resize(SwapchainImagesCount);
  for (INT i = 0; i < Images.size(); i++)
  {
    VkImageViewCreateInfo ImageViewCreateInfo
    {
      .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .image = Images[i],
      .viewType = VK_IMAGE_VIEW_TYPE_2D,
      .format = VlkRef->SwapchainImageFormat,
      .components =
      {
        .r = VK_COMPONENT_SWIZZLE_IDENTITY,
        .g = VK_COMPONENT_SWIZZLE_IDENTITY,
        .b = VK_COMPONENT_SWIZZLE_IDENTITY,
        .a = VK_COMPONENT_SWIZZLE_IDENTITY,
      },

      // How to use image and what part of it will be used.
      .subresourceRange =
      {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1,
      },
    };

    if (vkCreateImageView(VlkRef->Device, &ImageViewCreateInfo, nullptr, &ImageViews[i]) != VK_SUCCESS)
    {
      logger::error << std::format("Unable to create a swapchain image ({}).", i);
      return FALSE;
    }
  }
  SwapchainInfo = SwapchainCreateInfo;
  ///* Create depth image for swapchain */
  //if (!VlkRef->DepthImage.Create(SwapchainInfo.imageExtent.width, SwapchainInfo.imageExtent.height,
  //                               FALSE, FALSE, VK_FORMAT_D32_SFLOAT,
  //                               VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
  //                               VK_IMAGE_LAYOUT_UNDEFINED, nullptr, 0, 1))
  //{
  //  logger::error << "Error creating depth image!";
  //  return FALSE;
  //}

  /* Resize image vectors for deferred passes*/
  Opaq.resize(8);
  Trans.resize(8);
  Merge.resize(5);

  /* Color attachments */
  for (INT i = 0; i < 7; i++)
  {
    /* Destroy attachments */
    VlkRef->OpaqDefferedImg[i].Destroy();
    VlkRef->TransDefferedImg[i].Destroy();

    /* Create attachments */
    VlkRef->OpaqDefferedImg[i].Create(SwapchainInfo.imageExtent.width, SwapchainInfo.imageExtent.height, TRUE, FALSE, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_IMAGE_LAYOUT_UNDEFINED, nullptr, SwapchainInfo.imageExtent.width * SwapchainInfo.imageExtent.height * 4, 1);
    VlkRef->TransDefferedImg[i].Create(SwapchainInfo.imageExtent.width, SwapchainInfo.imageExtent.height, TRUE, FALSE, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_IMAGE_LAYOUT_UNDEFINED, nullptr, SwapchainInfo.imageExtent.width * SwapchainInfo.imageExtent.height * 4, 1);

    /* Cache image views for image creating */
    Opaq[i] = VlkRef->OpaqDefferedImg[i].View;
    Trans[i] = VlkRef->TransDefferedImg[i].View;
  }

  /* Depth attachments:
   * Destroy attachments */
  VlkRef->OpaqDefferedImg[7].Destroy();
  VlkRef->TransDefferedImg[7].Destroy();

  /* Create depth attachments */
  VlkRef->OpaqDefferedImg[7].Create(SwapchainInfo.imageExtent.width, SwapchainInfo.imageExtent.height, FALSE, FALSE, VK_FORMAT_D32_SFLOAT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, VK_IMAGE_LAYOUT_UNDEFINED, nullptr, 0, 1);
  VlkRef->TransDefferedImg[7].Create(SwapchainInfo.imageExtent.width, SwapchainInfo.imageExtent.height, FALSE, FALSE, VK_FORMAT_D32_SFLOAT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, VK_IMAGE_LAYOUT_UNDEFINED, nullptr, 0, 1);

  /* Cache image views for image creating */
  Opaq[7] = VlkRef->OpaqDefferedImg[7].View;
  Trans[7] = VlkRef->TransDefferedImg[7].View;

  ///* In opaque attachment */
  //VlkRef->MergeImg[0].Destroy();
  //VlkRef->MergeImg[0].Create(SwapchainInfo.imageExtent.width, SwapchainInfo.imageExtent.height, TRUE, FALSE, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_IMAGE_LAYOUT_UNDEFINED, nullptr, SwapchainInfo.imageExtent.width * SwapchainInfo.imageExtent.height * 4, 1);
  //Merge[0] = VlkRef->MergeImg[0].View;

  ///* In transparent attachment */
  //VlkRef->MergeImg[0].Destroy();
  //VlkRef->MergeImg[1].Create(SwapchainInfo.imageExtent.width, SwapchainInfo.imageExtent.height, TRUE, FALSE, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_IMAGE_LAYOUT_UNDEFINED, nullptr, SwapchainInfo.imageExtent.width * SwapchainInfo.imageExtent.height * 4, 1);
  //Merge[1] = VlkRef->MergeImg[1].View;

  ///* In opaque depth attachment */
  //VlkRef->MergeImg[0].Destroy();
  //VlkRef->MergeImg[2].Create(SwapchainInfo.imageExtent.width, SwapchainInfo.imageExtent.height, FALSE, FALSE, VK_FORMAT_D32_SFLOAT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, VK_IMAGE_LAYOUT_UNDEFINED, nullptr, 0, 1);
  //Merge[2] = VlkRef->MergeImg[2].View;

  ///* In transparent depth attachment */
  //VlkRef->MergeImg[0].Destroy();
  //VlkRef->MergeImg[3].Create(SwapchainInfo.imageExtent.width, SwapchainInfo.imageExtent.height, FALSE, FALSE, VK_FORMAT_D32_SFLOAT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, VK_IMAGE_LAYOUT_UNDEFINED, nullptr, 0, 1);
  //Merge[3] = VlkRef->MergeImg[3].View;

  Merge[0] = VlkRef->OpaqDefferedImg[6].View;
  Merge[1] = VlkRef->TransDefferedImg[6].View;
  Merge[2] = VlkRef->OpaqDefferedImg[7].View;
  Merge[3] = VlkRef->TransDefferedImg[7].View;

  /* Out merge color attachment */
  VlkRef->MergeImg[4].Destroy();
  VlkRef->MergeImg[4].Create(SwapchainInfo.imageExtent.width, SwapchainInfo.imageExtent.height, TRUE, FALSE, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_IMAGE_LAYOUT_UNDEFINED, nullptr, SwapchainInfo.imageExtent.width * SwapchainInfo.imageExtent.height * 4, 1);
  Merge[4] = VlkRef->MergeImg[4].View;

  /* Recreate deffered frame buffers */
  VlkRef->OpaqueFrameBuf.Destroy();
  VlkRef->OpaqueFrameBuf.Create(VlkRef->OpaqueRenderPass.RenderPass, VlkRef->SurfaceCapabilities.currentExtent.width, VlkRef->SurfaceCapabilities.currentExtent.height, Opaq);
  VlkRef->TransFrameBuf.Destroy();
  VlkRef->TransFrameBuf.Create(VlkRef->TransRenderPass.RenderPass, VlkRef->SurfaceCapabilities.currentExtent.width, VlkRef->SurfaceCapabilities.currentExtent.height, Trans);
  VlkRef->MergeFrameBuf.Destroy();
  VlkRef->MergeFrameBuf.Create(VlkRef->MergeRenderPass.RenderPass, VlkRef->SurfaceCapabilities.currentExtent.width, VlkRef->SurfaceCapabilities.currentExtent.height, Merge);

  /* Recreate merge frame buffers */
  CreateFrameBuffers();
  if (OldSwapchain == VK_NULL_HANDLE)
    logger::info << "Swapchain successfully created.";
  return TRUE;
} /* End of 'tam::vlk::swapchain::Create' function */

/* Resize swapchain function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) TRUE if swapchain was successfully resized, FALSE otherwise.
 */
BOOL tam::vlk::swapchain::Resize( VOID )
{
  if (VlkRef->SurfaceCapabilities.maxImageExtent.width == 0 ||
      VlkRef->SurfaceCapabilities.maxImageExtent.height == 0)
  {
    logger::warning << "Try to resize swapchain into zero sizes.";
    return FALSE;
  }
  vkDeviceWaitIdle(VlkRef->Device);
  VkSwapchainKHR OldSC = Swapchain;
  for (INT i = 0; i < ImageViews.size(); i++)
    vkDestroyImageView(VlkRef->Device, ImageViews[i], nullptr), ImageViews[i] = VK_NULL_HANDLE;
  //VlkRef->DepthImage.Destroy();
  Create(OldSC);
  vkDestroySwapchainKHR(VlkRef->Device, OldSC, nullptr);
  return TRUE;
} /* End of 'tam::vlk::swapchain::Resize' function */

/* Create frame buffers for swapchain function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) TRUE if frame buffers were successfully created, FALSE otherwise.
 */
BOOL tam::vlk::swapchain::CreateFrameBuffers( VOID )
{
  for (auto &fb : FrameBuffers)
    if (fb != VK_NULL_HANDLE)
    {
      vkDestroyFramebuffer(VlkRef->Device, fb, nullptr);
      fb = VK_NULL_HANDLE;
    }
  FrameBuffers.resize(ImageViews.size());
  for (INT i = 0; i < ImageViews.size(); i++)
  {
    std::array<VkImageView, 2> Attachments =
    {
      VlkRef->MergeImg[4].View,
      //VlkRef->OpaqDefferedImg[6].View,
      //VlkRef->TransDefferedImg[6].View,
      //VlkRef->OpaqDefferedImg[7].View,
      //VlkRef->TransDefferedImg[7].View,
      ///* Merge output: swapchain image */
      ImageViews[i],
    };

    VkFramebufferCreateInfo FrameBufferCreateInfo
    {
      .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
      .renderPass = VlkRef->PostProcRenderPass.RenderPass,
      .attachmentCount = static_cast<UINT>(Attachments.size()),
      .pAttachments = Attachments.data(),
      .width = SwapchainInfo.imageExtent.width,
      .height = SwapchainInfo.imageExtent.height,
      .layers = 1,
    };

    if (vkCreateFramebuffer(VlkRef->Device, &FrameBufferCreateInfo, nullptr, &FrameBuffers[i]) != VK_SUCCESS)
    {
      logger::error << "Unable to create frame buffer.";
      return FALSE;
    }
  }
  return TRUE;
} /* End of 'tam::vlk::swapchain::CreateFrameBuffers' function */

/* Destroy swapchain function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::swapchain::Destroy( VOID )
{
  /* Destroy deffered attachments */
  for (INT i = 0; i < 8; i++)
  {
    VlkRef->OpaqDefferedImg[i].Destroy();
    VlkRef->TransDefferedImg[i].Destroy();
  }
  //VlkRef->DepthImage.Destroy();
  for (INT i = 0; i < SwapchainImagesCount; i++)
    if (ImageViews[i] != VK_NULL_HANDLE)
    {
      vkDestroyImageView(VlkRef->Device, ImageViews[i], nullptr);
      ImageViews[i] = VK_NULL_HANDLE;
    }

  /* Destroy merge frame buffers */
  for (VkFramebuffer &FrameBuffer : FrameBuffers)
    if (FrameBuffer != VK_NULL_HANDLE)
    {
      vkDestroyFramebuffer(VlkRef->Device, FrameBuffer, nullptr);
      FrameBuffer = VK_NULL_HANDLE;
    }

  VlkRef->MergeImg[4].Destroy();

  /* Destroy deffered frame buffers */
  VlkRef->OpaqueFrameBuf.Destroy();
  VlkRef->TransFrameBuf.Destroy();
  VlkRef->MergeFrameBuf.Destroy();

  if (Swapchain != VK_NULL_HANDLE)
  {
    vkDestroySwapchainKHR(VlkRef->Device, Swapchain, nullptr);
    Swapchain = VK_NULL_HANDLE;
  }
} /* End of 'tam::vlk::swapchain::Destroy' function */

/* END OF 'tam_vlk_swapchain.cpp' FILE */
