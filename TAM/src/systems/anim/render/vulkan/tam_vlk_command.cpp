/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_command.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Command buffer functions implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 14.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Create command pool function.
 * ARGUMENTS:
 *   - command buffer byte:
 *       VkCommandPoolCreateFlagBits CommandPoolFlag;
 * RETURNS:
 *   (BOOL) depending on result.
 */
BOOL tam::vlk::core::CreateCommandPool( VkCommandPoolCreateFlags CommandPoolFlag )
{
  VkCommandPoolCreateInfo GraphicsCommandPoolCreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .flags = CommandPoolFlag,
    .queueFamilyIndex = GraphicsQueueFamilyIndex,
  };
  if (vkCreateCommandPool(Device, &GraphicsCommandPoolCreateInfo, nullptr, &GraphicsCommandPool) != VK_SUCCESS)
  {
    logger::error << "graphics command pool wasn't created!";
    return FALSE;
  }

  VkCommandPoolCreateInfo TransferCommandPoolCreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .flags = CommandPoolFlag,
    .queueFamilyIndex = TransferQueueFamilyIndex,
  };
  if (vkCreateCommandPool(Device, &TransferCommandPoolCreateInfo, nullptr, &TransferCommandPool) != VK_SUCCESS)
  {
    logger::error << "Transfer command pool wasn't created!";
    return FALSE;
  }

  logger::info << "Command pools successfully created.";
  return TRUE;
} /* End of 'tam::vlk::core::CreateCommandPool' function */

/* Delete command pool function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::core::DestroyCommandPool( VOID )
{
  if (GraphicsCommandPool != VK_NULL_HANDLE)
  {
    vkDestroyCommandPool(Device, GraphicsCommandPool, nullptr);
    GraphicsCommandPool = VK_NULL_HANDLE;
  }
  if (TransferCommandPool != VK_NULL_HANDLE)
  {
    vkDestroyCommandPool(Device, TransferCommandPool, nullptr);
    TransferCommandPool = VK_NULL_HANDLE;
  }
} /* End of 'tam::vlk::core::DestroyCommandPool' function */

/* Create command buffer function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) depending on result.
 */
BOOL tam::vlk::core::CreateCommandBuffer( VOID )
{
  CommandBuffers.resize(UsedFramesOnTheFly);
  VkCommandBufferAllocateInfo BufferAllocateInfo
  {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .commandPool = GraphicsCommandPool,
    .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    .commandBufferCount = UsedFramesOnTheFly,
  };
  VkCommandBufferAllocateInfo TransferAllocateInfo
  {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .commandPool = TransferCommandPool,
    .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    .commandBufferCount = 1,
  };
  if (vkAllocateCommandBuffers(Device, &BufferAllocateInfo, CommandBuffers.data()) != VK_SUCCESS)
  {
    logger::error << "Command bufffers wasn't created!";
    return FALSE;
  }
  logger::info << "Command buffers successfully created.";
  if (vkAllocateCommandBuffers(Device, &TransferAllocateInfo, &TransferCommandBuffer) != VK_SUCCESS)
  {
    logger::error << "Transfer command buffer wasn't created!";
    return FALSE;
  }
  logger::info << "Transfer command buffer successfully created.";
  return TRUE;
} /* End of 'tam::vlk::core::CreateCommandBuffer' function */

/* END OF 'tam_vlk_command.cpp' FILE */
