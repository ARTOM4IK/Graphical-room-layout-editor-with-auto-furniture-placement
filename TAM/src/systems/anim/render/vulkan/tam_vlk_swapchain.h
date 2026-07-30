/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_swapchain.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Swapchain declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 14.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_vlk_swapchain_h_
#define __tam_vlk_swapchain_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* 'vlk' namespace */
  namespace vlk
  {
    /* 'swapchain' handle class */
    class swapchain : virtual public vlk_ref
    {
    public:
      VkSwapchainKHR Swapchain {};                // Swapchain object
      UINT32 SwapchainImagesCount {};             // Number of swapchain images
      std::vector<VkImage> Images {};             // Swapchain images
      std::vector<VkImageView> ImageViews {};     // Swapchain image views
      VkSwapchainCreateInfoKHR SwapchainInfo {};  // Swapchain info
      std::vector<VkFramebuffer> FrameBuffers {}; // Merge frame buffers vector
      std::vector<VkImageView> Opaq, Trans, Merge;       // Image view vectors for vulkan image creating

      /* Create swapchain function.
       * ARGUMENTS:
       *   - old swapchain (optional):
       *       VkSwapchainKHR OldSwapchain;
       * RETURNS:
       *   (BOOL) TRUE if swapchain was successfully created, FALSE otherwise.
       */
      BOOL Create( VkSwapchainKHR OldSwapchain = VK_NULL_HANDLE );

      /* Resize swapchain function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) TRUE if swapchain was successfully resized, FALSE otherwise.
       */
      BOOL Resize( VOID );

      /* Create frame buffers for swapchain function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) TRUE if frame buffers were successfully created, FALSE otherwise.
       */
      BOOL CreateFrameBuffers( VOID );

      /* Destroy swapchain function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Destroy( VOID );
    }; /* End of 'swapchain' class */
  } /* end of 'vlk' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_vlk_swapchain_h_ */

/* END OF 'tam_vlk_swapchain.h' FILE */
