/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_images.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Image & sampler declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kupchenkov Ivan (IK4).
 * LAST UPDATE : 07.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_vlk_images_h_
#define __tam_vlk_images_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* 'vlk' namespace */
  namespace vlk
  {
    /* 'sampler' handle class */
    class sampler
    {
    private:
      core *VlkPtr;                        // Vulkan global handler pointer
    public:
      VkSampler Sampler = VK_NULL_HANDLE;  // Sampler

      /* Class default constructor.
       * ARGUMENTS:
       *   - pointer to vulkan handler:
       *       core *Ptr;
       */
      sampler( core *Ptr ) : VlkPtr(Ptr)
      {
      } /* End of 'sampler' function */

      /* Class destructor */
      ~sampler( VOID )
      {
        // To delete sampler you must write Sampler.Destroy() (because if sampler, which = DefSampler, destroyed all textures samplers will be invalid)
        // Destroy();
      } /* End of '~sampler' function */

      /* Create sampler function.
       * ARGUMENTS:
       *   - filters type:
       *       VkFilter FilterMag, FilterMin;
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
      BOOL Create( VkFilter FilterMag = VK_FILTER_LINEAR, VkFilter FilterMin = VK_FILTER_LINEAR, VkSamplerMipmapMode MipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR, VkSamplerAddressMode AddresModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT, VkSamplerAddressMode AddresModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT, VkSamplerAddressMode AddresModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT, BOOL AnisotropyEnable = FALSE, FLT MaxAnisotropy = 0.0, FLT MinLod = 1.0, FLT MaxLod = VK_LOD_CLAMP_NONE );

      /* Destroy sampler function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Destroy( VOID );

    }; /* End of 'sampler' class */

    /* 'image' handle class */
    class image
    {
    private:
      core *VlkPtr;                                      // Vulkan global handler pointer
      VmaAllocation Allocation {};                       // Vma Allocation

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
      VOID SetLayout( VkImageAspectFlags AspectMask, VkImageLayout NewImageLayout, VkPipelineStageFlags SrcStageMask, VkPipelineStageFlags DstStageMask );

    public:
      VkImage Image = VK_NULL_HANDLE;                    // Vulkan image
      VkImageView View = VK_NULL_HANDLE;                 // Vulkan image view
      VkImageLayout 
        Layout = VK_IMAGE_LAYOUT_UNDEFINED,              // Vulkan image layout (VK_IMAGE_LAYOUT_***)
        BindLayout;                                       // Vulkan image layout to bind only
      ///sampler Sampler;                                // Sampler for image
      VkFormat Format = VK_FORMAT_UNDEFINED;             // Vulkan image format (VK_FORMAT_***)
      VkImageUsageFlags UsageFlags;                      // Vulkan image usage flags (VK_IMAGE_USAGE_***)
      INT W = 0, H = 0;                                  // Size of image
      UINT MipLevels = 0;                                // Mip levels count
      BOOL IsCubeImg = FALSE;                            // Is cube or not
      BOOL IsColorImg = FALSE;                           // Is color

      /* Class default constructor.
       * ARGUMENTS:
       *   - pointer to vulkan handler:
       *       core *Ptr;
       */
      image( core *Ptr ) : VlkPtr(Ptr)///, Sampler(Ref)
      {
      } /* End of 'image' function */

      /* Class destructor */
      ~image( VOID )
      {
        Destroy();
      } /* End of '~image' function */

      /* Vulkan image creation function.
       * ARGUMENTS:
       *   - image size:
       *       INT ImgW, ImgH;
       *   - image color flag (otherwise Image is depth buffer):
       *       BOOL IsColor,
       *   - cube map flag (create 6 layers and 'PixelData' contain 6 joined same size images):
       *       BOOL IsCube,
       *   - vulkan image Format (see VK_FORMAT_***):
       *       VkFormat ImgFormat;
       *   - vulkan image usage flags bitwise combination (VK_IMAGE_USAGE_***):
       *       VkImageUsageFlags ImgUsageFlags;
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
      BOOL Create( INT ImgW, INT ImgH, BOOL IsColor, BOOL IsCube, VkFormat ImgFormat, VkImageUsageFlags ImgUsageFlags, VkImageLayout ImgLayout, const VOID *PixelsData, UINT PixelsDataSize, UINT MipCount = 1 );

      /* Vulkan image free function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Destroy( VOID );

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
      VOID SetLayout( VkImageLayout NewImageLayout, VkImageSubresourceRange SubresourceRange, VkPipelineStageFlags SrcStageMask, VkPipelineStageFlags DstStageMask );

      /* Generate mipmaps for vulkan image function.
       * ARGUMENTS:
       *   - image size:
       *       INT ImgW, ImgH;
       *   - number of mipmaps to be create:
       *       UINT Levels;
       *   - vulkan Image Format (see VK_FORMAT_***):
       *       VkFormat ImgFormat;
       *   - cube map flag (create 6 layers and 'PixelData' contain 6 joined same size images):
       *       BOOL IsCube;
       * RETURNS: None.
       */
      VOID GenerateMipmaps( INT ImgW, INT ImgH, UINT Levels, VkFormat ImgFormat, BOOL IsCube );

      /* Update pixels data for vulkan image function.
       * ARGUMENTS:
       *   - image pixel data:
       *       const VOID *PixelsData;
       *   - image pixel data size (in bytes):
       *       UINT PixelsDataSize;
       * RETURNS: None.
       */
      VOID UpdateData( const VOID *PixelsData, UINT PixelsDataSize );

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
      BOOL Resize( INT ImgW, INT ImgH, const VOID *PixelsData, UINT PixelsDataSize );

      /* Get pixels data from vulkan image function.
       * ARGUMENTS:
       *   - image pixel data:
       *       const VOID *PixelsData;
       *   - image pixel data size (in bytes):
       *       UINT PixelsDataSize;
       * RETURNS: None.
       */
      VOID GetData( VOID *PixelsData, UINT PixelsDataSize );

    }; /* End of 'image' class */

  } /* end of 'vlk' namespace */

} /* end of 'tam' namespace */

#endif /* __tam_vlk_images_h_ */

/* END OF 'tam_vlk_images.h' FILE */
