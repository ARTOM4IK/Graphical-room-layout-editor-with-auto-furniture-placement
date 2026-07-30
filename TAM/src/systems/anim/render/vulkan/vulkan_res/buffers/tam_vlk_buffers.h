/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_buffers.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Buffers declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 03.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_vlk_buffers_h_
#define __tam_vlk_buffers_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* 'vlk' namespace */
  namespace vlk
  {
    /* 'buffer' handle class */
    class buffer
    {
    private:
      core &VlkRef;                                             // Vulkan global handler reference
      VmaAllocation Allocation;                                 // Buffer VMA allocation
      VkBufferUsageFlags UsageFlags {};                         // Usage flags (e.g. VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
      VkSharingMode SharingMode = VK_SHARING_MODE_EXCLUSIVE;    // Sharing mode between queues of buffer
      VkMemoryPropertyFlags MemoryFlags {};                     // Memory requirements for buffer

    public:
      VkBuffer Buffer {};                                       // Vulkan buffer instance
      UINT Size {};                                             // Buffer size in bytes

      /* Vulkan buffer handle type constructor.
       * ARGUMENTS:
       *    - global Vulkan handle reference:
       *        core &Vlk;
       */
      buffer( core &Vlk ) : VlkRef(Vlk)
      {
      } /* End of 'buffer' function */

      /* Class destructor */
      ~buffer( VOID )
      {
      } /* End of '~buffer' function */

      /* Vulkan buffer creation function.
       * ARGUMENTS:
       *   - buffer size in bytes:
       *       VkDeviceSize BufSize;
       *   - vulkan buffer usage flags bitwise combination (VK_BUFFER_USAGE_***):
       *       VkBufferUsageFlags BufUsageFlags;
       *   - vulkan buffer property (see VK_MEMORY_PROPERTY_***):
       *       VkMemoryPropertyFlags BufMemoryProperties;
       *   - sharing mode:
       *       VkSharingMode BufSharingMode;
       * RETURNS:
       *   (BOOL) TRUE if buffer was created successfully, FLASE otherwise.
       */
      BOOL Create( VkDeviceSize BufSize, VkBufferUsageFlags BufUsageFlags, VkMemoryPropertyFlags BufMemoryProperties, VkSharingMode BufSharingMode );

      /* Destroy Vulkan buffer function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Destroy( VOID );

      /* Read memory from GPU to CPU function.
       * ARGUMENTS:
       *   - pointer to where copy data from buffer:
       *       VOID *Dst;
       *   - size of information to read:
       *       VkDeviceSize ReadSize;
       *   - offset from start of buffer:
       *       VkDeviceSize ReadOffset;
       * RETURNS: None.
       */
      VOID Read( VOID *Dst, VkDeviceSize ReadSize, VkDeviceSize ReadOffset );

      /* Write memory from GPU to CPU function.
       * ARGUMENTS:
       *   - pointer to source data:
       *       VOID *Data;
       *   - size of information to write:
       *       VkDeviceSize WriteSize;
       *   - offset from start of buffer:
       *       VkDeviceSize WriteOffset;
       * RETURNS: None.
       */
      VOID Write( VOID *Data, VkDeviceSize WriteSize, VkDeviceSize WriteOffset );

      /* Resize buffer with saving data function.
       * ARGUMENTS:
       *   - new size of buffer:
       *       UINT NewSize;
       * RETURNS: None.
       */
      VOID Resize( UINT NewSize );

      /* Buffer update function.
       * ARGUMENTS:
       *   - buffer data:
       *       VOID *Data;
       *   - length of elements, which will be updated:
       *       INT DataSize;
       *   - start of elements update:
       *       INT Offset;
       * RETURNS: None.
       */
      VOID Update( VOID *Data, INT DataSize, INT Offset );

      /* Get data from buffer function.
       * ARGUMENTS:
       *    - destination pointer:
       *        VOID *Dst;
       *   - size of data:
       *       UINT Size;
       *   - offset from buffer start:
       *       UINT Offset;
       */
      VOID GetData( VOID *Dst, UINT Size, UINT Offset );

      /* Move to buffer by using staging buffer function.
       * ARGUMENTS:
       *   - source buffer reference:
       *       const buffer &Src;
       *   - size of data to move:
       *       UINT Size;
       *   - offset from start of the buffer:
       *       UINT Offset;
       * RETURNS: None.
       */
      VOID WriteByStage( const buffer &Src, UINT Size, UINT Offset = 0 );

      /* Move data to buffer by using staging buffer function.
       * ARGUMENTS:
       *   - source buffer reference:
       *       VOID *Src;
       *   - size of data to move:
       *       UINT Size;
       *   - offset from start of the buffer:
       *       UINT Offset;
       * RETURNS: None.
       */
      VOID WriteByStage( VOID *Src, UINT Size, UINT Offset = 0 );

      /* Get data from GPU buffer by using staging buffer function.
       * ARGUMENTS:
       *   - destination buffer reference:
       *       VOID *Dst;
       *   - size of data to move:
       *       UINT Size;
       *   - offset from start of the buffer:
       *       UINT Offset;
       * RETURNS: None.
       */
      VOID ReadByStage( VOID *Dst, UINT Size, UINT Offset = 0 );
    }; /* End of 'buffer' class */
  } /* end of 'vlk' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_vlk_buffers_h_ */

/* END OF 'tam_vlk_buffers.h' FILE */
