/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_surface.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Surface structure definition file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 13.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_vlk_surface_h_
#define __tam_vlk_surface_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* 'vlk' namespace */
  namespace vlk
  {
    /* 'surface' handle class */
    class surface : virtual public vlk_ref
    {
    public:
      VkSurfaceKHR Surface {};                           // Surface object
      UINT32 SurfaceFormatsCount = 0;                    // Number of surface formats
      std::vector<VkSurfaceFormatKHR> SurfaceFormats {}; // Surface formats array
      VkSurfaceCapabilitiesKHR SurfaceCapabilities {};   // Surface capabilities

      VkFormat SwapchainImageFormat = VK_FORMAT_MAX_ENUM;                // format of swapchain's images
      VkColorSpaceKHR SwapchainColorSpace = VK_COLOR_SPACE_MAX_ENUM_KHR; // color space of swapchain
      VkPresentModeKHR PresentMode = VK_PRESENT_MODE_MAX_ENUM_KHR;       // chosen present mode

      /* Create surface function.
       * ARGUMENTS:
       *   - window handle reference:
       *      HWND &hWnd;
       * RETURNS:
       *   (BOOL) TRUE if surface was created, FALSE otherwise.
       */
      BOOL Create( HWND &hWnd );

      /* Resize surface function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Resize( VOID );

      /* Destroy surface function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Destroy( VOID );
    }; /* End of 'surface' class */
  } /* end of 'vlk' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_vlk_surface_h_ */

/* END OF 'tam_vlk_surface.h' FILE */
