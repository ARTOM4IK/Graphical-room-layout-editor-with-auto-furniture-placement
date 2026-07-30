/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_samplers.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Sampler implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kupchenkov Ivan (IK4).
 * LAST UPDATE : 12.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Create sampler function.
 * ARGUMENTS:
 *   - filters type:
 *       VkFilter FilterMin, FilterMax;
 *   - mipmap mode:
 *       VkSamplerMipmapMode MipMapMode;
 *   - addres mode:
 *       VkSamplerAddressMode AddresModeU, AddresModeV, AddresModeW;
 *   - is anisotropy enable:
 *       BOOL AnisotropyEnable;
 *   - max anisotropy:
 *       FLT MaxAnisotropy;
 *   - min and max lod:
 *       FLT MinLod, MaxLod;
 * RETURNS:
 *   (BOOL) TRUE if sampler was successfully created, FALSE if not.
 */
BOOL tam::vlk::sampler::Create( VkFilter FilterMag, VkFilter FilterMin, VkSamplerMipmapMode MipMapMode, VkSamplerAddressMode AddresModeU, VkSamplerAddressMode AddresModeV, VkSamplerAddressMode AddresModeW, BOOL AnisotropyEnable, FLT MaxAnisotropy, FLT MinLod, FLT MaxLod )
{
  VkSamplerCreateInfo Info
  {
    .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
    .magFilter = FilterMag,
    .minFilter = FilterMin,
    .mipmapMode = MipMapMode,
    .addressModeU = AddresModeU,
    .addressModeV = AddresModeV,
    .addressModeW = AddresModeW,
    .mipLodBias = 0.0f,
    .anisotropyEnable = (UINT)AnisotropyEnable,
    .maxAnisotropy = MaxAnisotropy,
    .minLod = MinLod,
    .maxLod = MaxLod,
    .borderColor = {},
    .unnormalizedCoordinates = FALSE
  };

  if (vkCreateSampler(VlkPtr->Device, &Info, nullptr, &Sampler) != VK_SUCCESS)
  {
    tam::logger::error << "vkCreateSampler not sucess!";
    return FALSE;
  }
  return TRUE;
} /* End of 'tam::vlk::image::Create' function */

/* Destroy sampler function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::sampler::Destroy( VOID )
{
  if (Sampler != VK_NULL_HANDLE)
  {
    vkDestroySampler(VlkPtr->Device, Sampler, nullptr);
    Sampler = VK_NULL_HANDLE;
  }
} /* End of 'tam::vlk::image::Destroy' function */

/* END OF 'tam_vlk_samplers.cpp' FILE */
