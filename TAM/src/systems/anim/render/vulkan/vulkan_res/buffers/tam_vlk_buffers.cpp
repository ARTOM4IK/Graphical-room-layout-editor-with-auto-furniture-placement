/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_buffers.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Buffers implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 11.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

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
 *   (BOOL) TRUE if buffer was created successfully, FALSE otherwise.
 */
BOOL tam::vlk::buffer::Create( VkDeviceSize BufSize, VkBufferUsageFlags BufUsageFlags, VkMemoryPropertyFlags BufMemoryProperties, VkSharingMode BufSharingMode )
{
  VkBufferCreateInfo BufferCreateInfo
   {
     .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
     .size = BufSize,
     .usage = BufUsageFlags,
     .sharingMode = BufSharingMode,
   };
  MemoryFlags = BufMemoryProperties;
  SharingMode = BufSharingMode;
  UsageFlags = BufUsageFlags;
  Size = BufSize;

  VmaAllocationCreateInfo AllocationCreateInfo {};
  AllocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
  AllocationCreateInfo.requiredFlags = BufMemoryProperties;
  AllocationCreateInfo.flags =
    VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT |
    VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
  AllocationCreateInfo.priority = 1.0f;
  if (vmaCreateBuffer(VlkRef.Allocator, &BufferCreateInfo, &AllocationCreateInfo, &Buffer, &Allocation, nullptr) != VK_SUCCESS)
  {
    logger::error << "Unable to create Vulkan buffer.";
    return FALSE;
  }
  return TRUE;
} /* End of 'tam::vlk::buffer::Create' function */

/* Destroy Vulkan buffer function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::buffer::Destroy( VOID )
{
  if (Buffer != VK_NULL_HANDLE)
  {
    vkDeviceWaitIdle(VlkRef.Device);
    vmaDestroyBuffer(VlkRef.Allocator, Buffer, Allocation);
    Buffer = VK_NULL_HANDLE;
    Allocation = VK_NULL_HANDLE;
  }
} /* End of 'tam::vlk::buffer::Destroy' function */

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
VOID tam::vlk::buffer::Read( VOID *Dst, VkDeviceSize ReadSize, VkDeviceSize ReadOffset )
{
  if ((MemoryFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0)
  {
    VOID *MapPointer;
    if (vmaMapMemory(VlkRef.Allocator, Allocation, &MapPointer) != VK_SUCCESS)
    {
      logger::error << "Error reading data from buffer!";
      return;
    }
    memcpy(Dst, (BYTE *)MapPointer + ReadOffset, ReadSize);
    vmaUnmapMemory(VlkRef.Allocator, Allocation);
  }
  else if ((UsageFlags & VK_BUFFER_USAGE_TRANSFER_SRC_BIT) != 0)
    ReadByStage(Dst, ReadSize, ReadOffset);
  else
    logger::error << "Error reading data from buffer to CPU!";
} /* End of 'tam::vlk::buffer::Read' function */

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
VOID tam::vlk::buffer::Write( VOID *Data, VkDeviceSize WriteSize, VkDeviceSize WriteOffset )
{
  if ((MemoryFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0)
  {
    VOID *MapPointer;
    if (vmaMapMemory(VlkRef.Allocator, Allocation, &MapPointer) != VK_SUCCESS)
    {
      logger::error << "Unable to map memory.";
      return;
    }
    memcpy((BYTE *)MapPointer + WriteOffset, Data, WriteSize);
    vmaUnmapMemory(VlkRef.Allocator, Allocation);
    /// vmaFlushAllocation(VlkRef.Allocator, Allocation, 0, WriteSize);
  }
  else
    WriteByStage(Data, WriteSize, WriteOffset);
} /* End of 'tam::vlk::buffer::Write' function */

/* Resize buffer with saving data function.
 * ARGUMENTS:
 *   - new size of buffer:
 *       UINT NewSize;
 * RETURNS: None.
 */
VOID tam::vlk::buffer::Resize( UINT NewSize )
{
  if (NewSize < 1)
    NewSize = 1;
  if (NewSize < Size)
  {
    logger::warning << std::format("Cut buffer's size to lower value!");
    return;
  }
  /// BYTE *OldData =  new BYTE[NewSize];
  /// ZeroMemory(OldData, NewSize);
  /// GetData(OldData, Size, 0);

  Destroy();
  Size = NewSize;
  Create(NewSize, UsageFlags, MemoryFlags, SharingMode);
  /// Write(OldData, NewSize, 0);
  /// delete[] OldData;
} /* End of 'tam::vlk::buffer::Resize' function */

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
VOID tam::vlk::buffer::Update( VOID *Data, INT DataSize, INT Offset )
{
  if (this == nullptr)
    return;
  if (DataSize < 0)
    DataSize = Size;
  if (Offset < 0)
    Offset = 0;
  if (Size < DataSize + Offset)
    Resize(DataSize + Offset);

  if (Data != nullptr)
    if ((MemoryFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0)
      Write(Data, DataSize, Offset);
    else
      WriteByStage(Data, (UINT)DataSize);
} /* End of 'tavk::buffer::Update' finction */

/* Get data from buffer function.
 * ARGUMENTS:
 *    - destination pointer:
 *        VOID *Dst;
 *   - size of data:
 *       UINT Size;
 *   - offset from buffer start:
 *       UINT Offset;
 * RETURNS: None.
 */
VOID tam::vlk::buffer::GetData( VOID *Dst, UINT Size, UINT Offset )
{
  if ((MemoryFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0)
    Read(Dst, Size, Offset);
  else if ((UsageFlags & VK_BUFFER_USAGE_TRANSFER_SRC_BIT) != 0)
    ReadByStage(Dst, Size, Offset);
  else
    logger::error << "Error reading data from buffer to CPU!";
} /* End of 'tam::vlk::buffer::Getdata' function */

/* Move data between buffers by using staging buffer function.
 * ARGUMENTS:
 *   - source buffer reference:
 *       const buffer &Src;
 *   - size of data to move:
 *       UINT Size;
 *   - offset from start of the buffer:
 *       UINT Offset;
 * RETURNS: None.
 */
VOID tam::vlk::buffer::WriteByStage( const buffer &Src, UINT Size, UINT Offset )
{
  BOOL IsTransBufBegin = FALSE;
  if (!VlkRef.IsTransferBufferActive)
  {
    vkResetCommandBuffer(VlkRef.TransferCommandBuffer, 0);
    IsTransBufBegin = TRUE;
    VkCommandBufferBeginInfo BeginInfo
    {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
      .pInheritanceInfo = nullptr,
    };
    if (vkBeginCommandBuffer(VlkRef.TransferCommandBuffer, &BeginInfo) != VK_SUCCESS)
    {
      logger::error << "Error begining staging command buffer work!";
      return;
    }
    VlkRef.IsTransferBufferActive = TRUE;
  }
  if (VlkRef.StagingBuffer.Size < Size)
    VlkRef.StagingBuffer.Resize(Size);
  VkBufferCopy CopyRegion
  {
    .srcOffset = 0,
    .dstOffset = Offset,
    .size = Size,
  };
  vkCmdCopyBuffer(VlkRef.TransferCommandBuffer, Src.Buffer, VlkRef.StagingBuffer.Buffer, 1, &CopyRegion);
  vkCmdCopyBuffer(VlkRef.TransferCommandBuffer, VlkRef.StagingBuffer.Buffer, Buffer, 1, &CopyRegion);

  if (IsTransBufBegin)
  {
    VlkRef.IsTransferBufferActive = FALSE;
    if (vkEndCommandBuffer(VlkRef.TransferCommandBuffer) != VK_SUCCESS)
    {
      logger::error << "Error copying buffer into another!";
      return;
    }
    VkSubmitInfo SubmitInfo
    {
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .commandBufferCount = 1,
      .pCommandBuffers = &VlkRef.TransferCommandBuffer,
    };
    vkResetFences(VlkRef.Device, 1, &VlkRef.TransferFence);
    vkQueueSubmit(VlkRef.TransferQueue, 1, &SubmitInfo, VlkRef.TransferFence);
    vkWaitForFences(VlkRef.Device, 1, &VlkRef.TransferFence, VK_TRUE, UINT64_MAX);
  }
} /* End of 'tam::vlk::buffer::MoveDataByStage' function */

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
VOID tam::vlk::buffer::WriteByStage( VOID *Src, UINT Size, UINT Offset )
{
  BOOL IsTransBufBegin = FALSE;
  if (!VlkRef.IsTransferBufferActive)
  {
    vkResetCommandBuffer(VlkRef.TransferCommandBuffer, 0);
    IsTransBufBegin = TRUE;
    VkCommandBufferBeginInfo BeginInfo
    {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
      .pInheritanceInfo = nullptr,
    };
    if (vkBeginCommandBuffer(VlkRef.TransferCommandBuffer, &BeginInfo) != VK_SUCCESS)
    {
      logger::error << "Error begining staging command buffer work!";
      return;
    }
    VlkRef.IsTransferBufferActive = TRUE;
  }
  if (VlkRef.StagingBuffer.Size < Size)
    VlkRef.StagingBuffer.Resize(Size);
  VkBufferCopy CopyRegion
  {
    .srcOffset = 0,
    .dstOffset = Offset,
    .size = Size,
  };
  VlkRef.StagingBuffer.Update(Src, Size, 0);
  vkCmdCopyBuffer(VlkRef.TransferCommandBuffer, VlkRef.StagingBuffer.Buffer, Buffer, 1, &CopyRegion);

  if (IsTransBufBegin)
  {
    VlkRef.IsTransferBufferActive = FALSE;
    if (vkEndCommandBuffer(VlkRef.TransferCommandBuffer) != VK_SUCCESS)
    {
      logger::error << "Error copying buffer into another!";
      return;
    }
    VkSubmitInfo SubmitInfo
    {
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .commandBufferCount = 1,
      .pCommandBuffers = &VlkRef.TransferCommandBuffer,
    };
    vkResetFences(VlkRef.Device, 1, &VlkRef.TransferFence);
    vkQueueSubmit(VlkRef.TransferQueue, 1, &SubmitInfo, VlkRef.TransferFence);
    vkWaitForFences(VlkRef.Device, 1, &VlkRef.TransferFence, VK_TRUE, UINT64_MAX);
  }
} /* End of 'tam::vlk::buffer::WriteByStage' function */

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
VOID tam::vlk::buffer::ReadByStage( VOID *Dst, UINT Size, UINT Offset )
{
  if (VlkRef.StagingBuffer.Size < Size)
    VlkRef.StagingBuffer.Resize(Size);
  if (Size + Offset > this->Size)
  {
    logger::warning << "Tried to read data outside the buffer (read size > buffer size).";
    return;
  }

  BOOL IsTransBufBegin = FALSE;
  if (!VlkRef.IsTransferBufferActive)
  {
    IsTransBufBegin = TRUE;
    VkCommandBufferBeginInfo BeginInfo
    {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
      .pInheritanceInfo = nullptr,
    };
    if (vkBeginCommandBuffer(VlkRef.TransferCommandBuffer, &BeginInfo) != VK_SUCCESS)
    {
      logger::error << "Error begining staging command buffer work!";
      return;
    }
    VlkRef.IsTransferBufferActive = TRUE;
  }

  VkBufferCopy CopyRegion
  {
    .srcOffset = Offset,
    .dstOffset = 0,
    .size = Size,
  };
  vkCmdCopyBuffer(VlkRef.TransferCommandBuffer, Buffer, VlkRef.StagingBuffer.Buffer, 1, &CopyRegion);

  if (IsTransBufBegin)
  {
    VlkRef.IsTransferBufferActive = FALSE;
    if (vkEndCommandBuffer(VlkRef.TransferCommandBuffer) != VK_SUCCESS)
    {
      logger::error << "Error copying buffer into another!";
      return;
    }
    VkSubmitInfo SubmitInfo
    {
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .commandBufferCount = 1,
      .pCommandBuffers = &VlkRef.TransferCommandBuffer,
    };
    vkResetFences(VlkRef.Device, 1, &VlkRef.TransferFence);
    vkQueueSubmit(VlkRef.TransferQueue, 1, &SubmitInfo, VlkRef.TransferFence);
    vkWaitForFences(VlkRef.Device, 1, &VlkRef.TransferFence, VK_TRUE, UINT64_MAX);
  }
  VlkRef.StagingBuffer.GetData(Dst, Size, 0);
} /* End of 'tam::vlk::buffer::ReadByStage' function */

/* END OF 'tam_vlk_buffers.cpp' FILE */
