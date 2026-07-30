/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_buffers.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               buffer resource declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 17.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_res_buffers_h_
#define __tam_res_buffers_h_

#include "..\tam_res.h"

/* 'tam' namespace */
namespace tam
{
  /* 'buffer_res' handle class */
  class buffer_res : public resource
  {
  public:
    UINT Size = 0;          // Size of buffer
    UINT Id {};             // Buffer ID
    vlk::buffer VlkBuffer;  // Vulkan buffer

    /* Buffer class constructor.
     * ARGUMENTS:
     *   - render pointer:
     *       render *Rnd;
     */
    buffer_res( render *rnd );

    /* Buffer move constructor.
     * ARGUMENTS:
     *   - old buffer rvalue:
     *       buffer_res &&Old;
     */
    buffer_res( buffer_res &&Old );

    /* Class destructor */
    ~buffer_res( VOID )
    {
      Free();
    } /* End of '~buffer_res' function */

    /* Buffer create function.
     * ARGUMENTS:
     *   - buffer data:
     *       const data_type *Data;
     *   - buffer usage flags:
     *       VkBufferUsageFlags BufferType;
     *   - buffer memory property flags:
     *       VkMemoryPropertyFlags Flags;
     *   - buffer sharing mode:
     *       VkSharingMode SharingMode;
     * RETURNS:
     *   (buffer &) self reference.
     */
    template<typename data_type>
      buffer_res & Create( data_type *Data = {}, VkBufferUsageFlags BufferType = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VkMemoryPropertyFlags Flags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VkSharingMode SharingMode = {} )
      {
        Free();
        Size = sizeof(data_type);
        if (Size <= 0)
          return *this;
        if (!VlkBuffer.Create(Size, BufferType, Flags, SharingMode))
          return *this;
        if (Data != nullptr)
          VlkBuffer.Update(Data, Size, 0);
        return *this;
      } /* End of 'Create' function */

    /* Buffer create function.
     * ARGUMENTS:
     *   - number of data elements:
     *       INT NumOfElements;
     *   - buffer data:
     *       const data_type *Data;
     *   - buffer usage flags:
     *       VkBufferUsageFlags BufferType;
     *   - buffer memory property flags:
     *       VkMemoryPropertyFlags Flags;
     *   - buffer sharing mode:
     *       VkSharingMode SharingMode;
     * RETURNS:
     *   (buffer &) self reference.
     */
    template<typename data_type>
      buffer_res & Create( INT NumOfElements, data_type *Data  = {}, VkBufferUsageFlags BufferType = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VkMemoryPropertyFlags Flags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VkSharingMode SharingMode = {} ) 
      {
        Free();
        Size = sizeof(data_type) * NumOfElements;
        if (Size == 0)
          return *this;
        if (!VlkBuffer.Create(Size, BufferType, Flags, SharingMode))
          return *this;
        if (Data != nullptr)
          VlkBuffer.Update(Data, Size, 0);
        return *this;
      } /* End of 'Create' function */

    /* Buffer create function.
     * ARGUMENTS:
     *   - buffer data:
     *       VOID *Data;
     *   - Size buffer data:
     *       INT SizeofData;
     *   - buffer usage flags:
     *       VkBufferUsageFlags BufferType;
     *   - buffer memory property flags:
     *       VkMemoryPropertyFlags Flags;
     *   - buffer sharing mode:
     *       VkSharingMode SharingMode;
     * RETURNS:
     *   (buffer &) self reference.
     */
    buffer_res & Create( VOID *Data = {}, INT SizeofData = 0, VkBufferUsageFlags BufferType = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,  VkMemoryPropertyFlags Flags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VkSharingMode SharingMode = {} ) 
    {
      Free();
      Size = SizeofData;
      if (Size == 0)
        return *this;
      if (!VlkBuffer.Create(Size, BufferType, Flags, SharingMode))
        return *this;
      if (Data != nullptr)
        VlkBuffer.Update(Data, Size, 0);
      return *this;
    } /* End of 'Create' function */

    /* Resource free function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );

    /* Buffer update function.
     * ARGUMENTS:
     *   - buffer data:
     *       const data_type *Data;
     *   - start of elements update:
     *       INT Start;
     *   - length of elements, which will be updated:
     *       INT BufSize;
     * RETURNS: None.
     */
    template<typename data_type>
      VOID Update( data_type *Data, INT Start = 0, INT BufSize = -1 )
      {
        if (BufSize < 0)
          BufSize = Size;
        if (Start < 0)
          Start = 0;
        if (Start + BufSize > Size)
          BufSize = Size - Start;
        if (Data != nullptr)
          VlkBuffer.Update(Data, BufSize, Start);
      } /* End of 'Update' function */

    /* Buffer resize function.
     * ARGUMENTS:
     *   - new size of buffer:
     *       INT BufSize;
     * RETURNS: None.
     */
    VOID Resize( INT BufSize = -1 )
    {
      if (BufSize >= 0)
        Size = BufSize;
      else
        BufSize = Size;
      VlkBuffer.Resize(BufSize);
    } /* End of 'Resize' function */
  }; /* End of 'buffer_res' class */

  /* buffer resource user type definition */
  using buffer = std::shared_ptr<buffer_res>;
} /* end of 'tam' namespace */

/* 'tam' namespace */
namespace tam
{
  /* 'buffer_manager' handle class */
  class buffer_manager : public resource_manager<buffer_res, INT>
  {
  public:

    /* Class default constructor */
    buffer_manager( render &Rnd ) : resource_manager(Rnd)
    {
    } /* End of 'buffer_manager' function */

    /* Buffer create function.
     * ARGUMENTS:
     *   - buffer data:
     *       const data_type *Data;
     *   - buffer usage flags:
     *       VkBufferUsageFlags BufferType;
     *   - buffer memory property flags:
     *       VkMemoryPropertyFlags Flags;
     *   - buffer sharing mode:
     *       VkSharingMode SharingMode;
     * RETURNS:
     *   (buffer) variable.
     */
    template<typename data_type>
      buffer BufferCreate( data_type *Data = {}, VkBufferUsageFlags BufferType = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VkMemoryPropertyFlags Flags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VkSharingMode SharingMode = {} )
      {
        buffer Buf = Allocate();

        Buf->Create(Data, BufferType, Flags, SharingMode);
        return Buf;
      } /* End of 'BufferCreate' function */

    /* Buffer create function.
     * ARGUMENTS:
     *   - number of data elements:
     *       INT NumOfElements;
     *   - buffer data:
     *       const data_type *Data;
     *   - buffer usage flags:
     *       VkBufferUsageFlags BufferType;
     *   - buffer memory property flags:
     *       VkMemoryPropertyFlags Flags;
     *   - buffer sharing mode:
     *       VkSharingMode SharingMode;
     * RETURNS:
     *   (buffer) variable.
     */
    template<typename data_type>
      buffer BufferCreate( INT NumOfElements, data_type *Data = {}, VkBufferUsageFlags BufferType = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VkMemoryPropertyFlags Flags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VkSharingMode SharingMode = {} )
      {
        buffer Buf = Allocate();

        Buf->Create(NumOfElements, Data, BufferType, Flags, SharingMode);
        return Buf;
      } /* End of 'BufferCreate' function */

    /* Buffer create function.
     * ARGUMENTS:
     *   - number of data elements:
     *       INT NumOfElements;
     *   - buffer data:
     *       const data_type *Data;
     *   - Size buffer data:
     *       INT SizeofData;
     *   - buffer usage flags:
     *       VkBufferUsageFlags BufferType;
     *   - buffer memory property flags:
     *       VkMemoryPropertyFlags Flags;
     *   - buffer sharing mode:
     *       VkSharingMode SharingMode;
     * RETURNS:
     *   (buffer) variable.
     */
    buffer BufferCreate( INT SizeofData, VOID *Data = {}, VkBufferUsageFlags BufferType = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VkMemoryPropertyFlags Flags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VkSharingMode SharingMode = {} )
    {
      buffer Buf = Allocate();

      Buf->Create(Data, SizeofData, BufferType, Flags, SharingMode);
      return Buf;
    } /* End of 'BufferCreate' function */
  }; /* End of 'buffer_manager' class */
} /* end of 'tam' namespace */

#endif /* __tam_res_buffers_h_ */

/* END OF 'tam_res_buffers.h' FILE */
