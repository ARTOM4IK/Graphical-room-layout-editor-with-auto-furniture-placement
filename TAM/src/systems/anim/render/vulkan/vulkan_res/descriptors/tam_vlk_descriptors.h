/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_descriptors.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Descriptor sets and layouts definition file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 19.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_vlk_descriptors_h_
#define __tam_vlk_descriptors_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* 'vlk' namespace */
  namespace vlk
  {
    /* Vulkan core class forward declaration */
    class core;

    /* Vulkan buffer class forward declaration */
    class buffer;

    /* 'descriptor_set_layout' handle class */
    class descriptor_set_layout
    {
    public:
      // Vulkan class pointer.
      core *VlkPtr;
      // Descriptor set layout bindings.
      std::vector<VkDescriptorSetLayoutBinding> DescriptorSetLayoutBindings {};
      // Descriptor set layout.
      VkDescriptorSetLayout DescriptorSetLayout {};

      /* Descriptor set layout handle type constructor.
       * ARGUMENTS:
       *    - global Vulkan handle pointer:
       *        core *Vlk;
       */
      descriptor_set_layout( core *Vlk )
      {
        VlkPtr = Vlk;
      } /* End of 'descriptor_set_layout' function */

      /* Class destructor */
      ~descriptor_set_layout( VOID )
      {
        // AT7 violation: Destroy();
      } /* End of '~descriptor_set_layout' function */

      /* Create descriptor set layout function.
       * ARGUMENTS:
       *   - descriptor set layout bindings:
       *       std::vector<VkDescriptorSetLayoutBinding> DescriptorSetLayoutBindings;
       * RETURNS:
       *   (BOOL) TRUE if descriptor set layout was successfully created, FALSE otherwise.
       */
      BOOL Create( std::vector<VkDescriptorSetLayoutBinding> DSLayoutBindings );

      /* Destroy descriptor set layout function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Destroy( VOID );
    }; /* End of 'descriptor_set_layout' class */

    /* 'descriptor_set' handle class */
    class descriptor_set
    {
      core *VlkPtr; // Vulkan class pointer
    public:
      VkDescriptorSet DescriptorSet {};                      // Descriptor set.
      std::vector<VkWriteDescriptorSet> DescriptorWrites {}; // Descriptor writes vector

      /* Descriptor set handle type constructor.
       * ARGUMENTS:
       *    - global Vulkan handle pointer:
       *        core *Vlk;
       */
      descriptor_set( core *Vlk )
      {
        VlkPtr = Vlk;
      } /* End of 'descriptor_set' function */

      /* Class destructor */
      ~descriptor_set( VOID )
      {
        // Destroy();
      } /* End of '~descriptor_set' function */

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
      BOOL Create( const descriptor_set_layout &DescriptorSetLayout,
        const std::vector<buffer*>& Buffers, const std::vector<VkImageView>& ImageViews, const std::vector<sampler>& Samplers, const std::vector<std::pair<VkImageView, VkImageLayout>> &AttachmentsIsColor = {}, const std::vector<sampler> &AttSamplers = {} );

      /* Create descriptor set (attachments) function.
       * ARGUMENTS:
       *   - descriptor set layout:
       *       const descriptor_set_layout &DescriptorSetLayout;
       *   - image views of images to be attached:
       *       std::vector<VkImageView> ImageViews;
       * RETURNS:
       *   (BOOL) depending on result.
       */
      BOOL Create( const descriptor_set_layout &DescriptorSetLayout, std::vector<VkImageView> ImageViews );

      /* Update descriptor set (attachments) function.
        * ARGUMENTS:
        *   - images to be attached:
        *       std::vector<image> Images;
        *   - buffers to be attached:
        *       std::vector<buffer *> &Buffers;
        * RETURNS: None.
        */
      VOID Update( const std::vector<std::pair<VkImageView, VkImageLayout>> &ImagesIsColor, const std::vector<buffer *> &Buffers = {} );

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
      VOID Update( std::vector<buffer *> Buffers, std::vector<VkImageView> ImageViews, std::vector<sampler> Samplers );

      /* Destroy descriptor set function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Destroy( VOID );
    }; /* End of 'descriptor_set' class */
  } /* end of 'vlk' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_vlk_descriptors_h_ */

/* END OF 'tam_vlk_descriptors.h' FILE */
