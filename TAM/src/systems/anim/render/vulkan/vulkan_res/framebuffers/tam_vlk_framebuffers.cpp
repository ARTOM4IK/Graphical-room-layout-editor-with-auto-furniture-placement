/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

 /* FILE NAME   : tam_vlk_framebuffers.cpp
  * PURPOSE     : Tough Arrangement Manager project.
  *               Frame buffer implementation module.
  * PROGRAMMER  : CGSG'Sr'2025.
  *               Arsentev Artemy (AA4).
  * LAST UPDATE : 10.03.2026
  * NOTE        : Namespace 'tam'.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include "tam.h"

/* Class default constructor */
tam::vlk::framebuffer::framebuffer( core* Ptr ): VlkPtr(Ptr)
{
} /* End of 'framebuffer' function */

/* Vulkan frame buffer creation function.
 * ARGUMENTS:
 *   - Render pass:
 *       VkRenderPass &RenderPass;
 *   - Frame buffer size:
 *       INT &W, INT &H;
 *   - Image views of Attachments:
 *       std::vector<VkImageView> &Attachments;
 * RETURNS:
 *   (BOOL) success creation result.
 */
BOOL tam::vlk::framebuffer::Create( VkRenderPass RenderPass, INT W, INT H, std::vector<VkImageView> &Attachments )
{
  VkFramebufferCreateInfo FrameBufferCreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    .renderPass = RenderPass,
    .attachmentCount = static_cast<UINT>(Attachments.size()),
    .pAttachments = Attachments.data(),
    .width = (UINT)W,
    .height = (UINT)H,
    .layers = 1,
  };

  if (vkCreateFramebuffer(VlkPtr->Device, &FrameBufferCreateInfo, nullptr, &FrameBuffer) != VK_SUCCESS)
  {
    logger::error << "Unable to create frame buffer.";
    return FALSE;
  }

  return TRUE;
} /* End of 'tam::vlk::framebuffer::Create' function */

/* Vulkan destroy frame buffer function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::framebuffer::Destroy( VOID )
{
  if (FrameBuffer != VK_NULL_HANDLE)
  {
    vkDestroyFramebuffer(VlkPtr->Device, FrameBuffer, nullptr);
    FrameBuffer = VK_NULL_HANDLE;
  }
} /* End of 'tam::vlk::framebuffer::Destroy' function */

/* END OF 'tam_vlk_framebuffers.cpp' FILE */
