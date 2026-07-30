/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_surface.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Vulkan surface implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 13.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Create surface function.
 * ARGUMENTS:
 *   - window handle reference:
 *      HWND &hWnd;
 * RETURNS:
 *   (BOOL) TRUE if surface was created, FALSE otherwise.
 */
BOOL tam::vlk::surface::Create( HWND &hWnd )
{
  VkWin32SurfaceCreateInfoKHR SurfaceCreateInfo 
  {
    .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
    .flags = 0,
    .hinstance = GetModuleHandle(nullptr),
    .hwnd = hWnd,
  };

  if (vkCreateWin32SurfaceKHR(VlkRef->Instance, &SurfaceCreateInfo, nullptr, &Surface) != VK_SUCCESS)
  {
    logger::error << "Surface wasn't created!";
    return FALSE;
  }
  // Get avaiable present modess
  UINT PresentModesCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(VlkRef->PhysDevice, VlkRef->Surface, &PresentModesCount, nullptr);
  std::vector<VkPresentModeKHR> PresentModes;
  PresentModes.resize(PresentModesCount);
  vkGetPhysicalDeviceSurfacePresentModesKHR(VlkRef->PhysDevice, VlkRef->Surface, &PresentModesCount, PresentModes.data());
  for (auto &PresMode : PresentModes)
  {
    if (PresMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
    {
      PresentMode = PresMode;
      break;
    }
    else if (PresMode == VK_PRESENT_MODE_MAILBOX_KHR ||
             PresMode == VK_PRESENT_MODE_FIFO_KHR ||
             PresMode == VK_PRESENT_MODE_FIFO_RELAXED_KHR)
      PresentMode = PresMode;
  }
  if (PresentMode == VK_PRESENT_MODE_MAX_ENUM_KHR)
  {
    logger::error << "No acceptible present mode.";
    return FALSE;
  }
  switch (PresentMode)
  {
  case VK_PRESENT_MODE_IMMEDIATE_KHR:
    logger::info << "Present mode: " >> 0x88AAFF << "VK_PRESENT_MODE_IMMEDIATE_KHR";
    break;
  case VK_PRESENT_MODE_MAILBOX_KHR:
    logger::info << "Present mode: " >> 0x88AAFF << "VK_PRESENT_MODE_MAILBOX_KHR";
    break;
  case VK_PRESENT_MODE_FIFO_KHR:
    logger::info << "Present mode: " >> 0x88AAFF << "VK_PRESENT_MODE_FIFO_KHR";
    break;
  case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
    logger::info << "Present mode: " >> 0x88AAFF << "VK_PRESENT_MODE_FIFO_RELAXED_KHR ";
    break;
  }

  /* Get surface formats for swapchain */
  vkGetPhysicalDeviceSurfaceFormatsKHR(VlkRef->PhysDevice, Surface, &SurfaceFormatsCount, nullptr);
  SurfaceFormats.resize(SurfaceFormatsCount);
  vkGetPhysicalDeviceSurfaceFormatsKHR(VlkRef->PhysDevice, Surface, &SurfaceFormatsCount, SurfaceFormats.data());
  for (auto &SurfaceFormat : SurfaceFormats)
  {
    if (SurfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
        SurfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
    {
      SwapchainColorSpace = SurfaceFormat.colorSpace;
      SwapchainImageFormat = SurfaceFormat.format;
      break;
    }
    else if (SwapchainColorSpace == VK_COLOR_SPACE_MAX_ENUM_KHR &&
             SurfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
             SurfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
    {
      SwapchainColorSpace = SurfaceFormat.colorSpace;
      SwapchainImageFormat = SurfaceFormat.format;
    }
  }
  /* Handle case if there is no apropriate color space or image format */
  if (SwapchainColorSpace == VK_COLOR_SPACE_MAX_ENUM_KHR)
  {
    logger::error << "Surface wasn't created: no apropriate surface color space or image format found.";
    return FALSE;
  }
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VlkRef->PhysDevice, Surface, &SurfaceCapabilities);
  logger::info << "Surface successfully created.";
  return TRUE;
} /* End of 'tam::vlk::surface::Create' function */

/* Resize surface function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::surface::Resize( VOID )
{
  if (Surface == VK_NULL_HANDLE)
    return;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VlkRef->PhysDevice, Surface, &SurfaceCapabilities);
} /* End of 'tam::vlk::surface::Resize' function */

/* Destroy surface function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::surface::Destroy( VOID )
{
  if (Surface != VK_NULL_HANDLE)
    vkDestroySurfaceKHR(VlkRef->Instance, Surface, nullptr);
  Surface = VK_NULL_HANDLE;
} /* End of 'tam::vlk::surface::Destroy' function */

/* END OF 'tam_vlk_surface.cpp' FILE */
