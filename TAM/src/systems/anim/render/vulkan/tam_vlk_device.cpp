/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

 /* FILE NAME   : tam_vlk_device.cpp
  * PURPOSE     : Tough Arrangement Manager project.
  *               Vulkan system module.
  *               Logical device functions implementation file.
  * PROGRAMMER  : CGSG'Sr'2025.
  *               Novozhilova Maryana (MN7).
  * LAST UPDATE : 29.01.2026
  * NOTE        : Namespace 'tam'.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include "tam.h"

  /* Create logical device function.
   * ARGUMENTS: None.
   * RETURNS:
   *   (BOOL) TRUE if logical device was successfully found, FALSE otherwise.
   */
BOOL tam::vlk::device::Create(VOID)
{
    UINT32 QueueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(VlkRef->PhysDevice, &QueueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> QueueFamilyProperties{ QueueFamilyCount };
    vkGetPhysicalDeviceQueueFamilyProperties(VlkRef->PhysDevice, &QueueFamilyCount, QueueFamilyProperties.data());

    /* Queue family indices structure */
    struct queue_family_indices
    {
        std::optional<UINT32> GraphicsFamily; // Rendering queue family index
        std::optional<UINT32> TransferFamily; // Transfer queue family index
        std::optional<UINT32> PresentFamily;  // Drawing queue family index
        std::optional<UINT32> ComputeFamily;  // Compute queue family index
    }; /* End of 'queue_family_indices' structure */

    queue_family_indices Indices;

    //Find family, with graphics, present, transfer and compute support
    for (INT i = 0; i < QueueFamilyCount; i++)
        if (!Indices.GraphicsFamily && (QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0 && (QueueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0
            && (QueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0)
        {
            VkBool32 supportsPresent = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(VlkRef->PhysDevice, i, VlkRef->Surface, &supportsPresent);

            if (supportsPresent)
            {
                Indices.GraphicsFamily = i, Indices.PresentFamily = i, Indices.TransferFamily = i, Indices.ComputeFamily = i;
                break;
            }
        }

    VlkRef->GraphicsQueueFamilyIndex = Indices.GraphicsFamily.value(),
        VlkRef->PresentQueueFamilyIndex = Indices.PresentFamily.value();
    VlkRef->TransferQueueFamilyIndex = Indices.TransferFamily.value();

    std::vector<FLT> QueuePriorities(QueueFamilyCount);
    QueuePriorities[0] = 1.0;
    QueuePriorities[1] = 0.5;
    QueuePriorities[2] = 0.2;

    VkDeviceQueueCreateInfo QueueInfo[]
    {
      {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueFamilyIndex = Indices.GraphicsFamily.value(),
        .queueCount = QueueFamilyProperties[Indices.GraphicsFamily.value()].queueCount < 3 ? 1U : 3U,
        .pQueuePriorities = &QueuePriorities[0]
      },
    };

    // Set logical device's extensions.
    std::vector<const CHAR*> ExtensionNames
    {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
      VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME,
      VK_EXT_GRAPHICS_PIPELINE_LIBRARY_EXTENSION_NAME
    };

    VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures PhysicalDeviceSeparateDepthStencilLayoutsFeatures
    {
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SEPARATE_DEPTH_STENCIL_LAYOUTS_FEATURES,
      .separateDepthStencilLayouts = TRUE,
    };

    VkPhysicalDeviceHostQueryResetFeatures PhysicalDeviceHostQueryResetFeatures
    {
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES,
      .pNext = &PhysicalDeviceSeparateDepthStencilLayoutsFeatures,
      .hostQueryReset = VK_TRUE,
    };

    VkPhysicalDevice8BitStorageFeatures PhysicalDevice8BitStorageFeatures
    {
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES,
      .pNext = &PhysicalDeviceHostQueryResetFeatures,
      .uniformAndStorageBuffer8BitAccess = TRUE,
    };

    VkPhysicalDeviceBufferDeviceAddressFeatures PhysicalDeviceBufferDeviceAddressFeatures
    {
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES,
      .pNext = &PhysicalDevice8BitStorageFeatures,
      .bufferDeviceAddress = TRUE,
    };

    VkPhysicalDeviceTimelineSemaphoreFeatures PhysicalDeviceTimelineSemaphoreFeatures
    {
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES,
      .pNext = &PhysicalDeviceBufferDeviceAddressFeatures,
      .timelineSemaphore = TRUE,
    };

    VkPhysicalDeviceVulkanMemoryModelFeatures PhysicalDeviceVulkanMemoryModelFeatures
    {
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_MEMORY_MODEL_FEATURES,
      .pNext = &PhysicalDeviceTimelineSemaphoreFeatures,
      .vulkanMemoryModel = TRUE,
      .vulkanMemoryModelDeviceScope = TRUE,
    };

    VkPhysicalDeviceExtendedDynamicState3FeaturesEXT PhysicalDeviceExtendedDynamicState3Features
    {
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_3_FEATURES_EXT,
      .pNext = &PhysicalDeviceVulkanMemoryModelFeatures,
      .extendedDynamicState3PolygonMode = VK_TRUE,
    };
    VkDeviceCreateInfo DeviceCreateInfo
    {
      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .pNext = &PhysicalDeviceExtendedDynamicState3Features,
      .queueCreateInfoCount = 1,
      .pQueueCreateInfos = QueueInfo,
      .enabledLayerCount = 0,
      .enabledExtensionCount = static_cast<UINT>(ExtensionNames.size()),
      .ppEnabledExtensionNames = ExtensionNames.data(),
      .pEnabledFeatures = &VlkRef->PhysicalDeviceFeatures
    };

    vkCreateDevice(VlkRef->PhysDevice, &DeviceCreateInfo, nullptr, &Device);
    if (Device == VK_NULL_HANDLE)
    {
        logger::error << "Unable to create logical device.";
        return FALSE;
    }

    vkGetDeviceQueue(VlkRef->Device, VlkRef->GraphicsQueueFamilyIndex, 0, &VlkRef->GraphicsQueue);
    vkGetDeviceQueue(VlkRef->Device, VlkRef->PresentQueueFamilyIndex, 0, &VlkRef->PresentQueue);

    if (QueueInfo->queueCount == 1)
    {
        //use only one avaliable queue
        vkGetDeviceQueue(VlkRef->Device, VlkRef->TransferQueueFamilyIndex, 0, &VlkRef->TransferQueue);
        vkGetDeviceQueue(VlkRef->Device, VlkRef->ComputeQueueFamilyIndex, 0, &VlkRef->ComputeQueue);
    }
    else
    {
        //use 3 parallel avaliable queues
        vkGetDeviceQueue(VlkRef->Device, VlkRef->TransferQueueFamilyIndex, 1, &VlkRef->TransferQueue);
        vkGetDeviceQueue(VlkRef->Device, VlkRef->ComputeQueueFamilyIndex, 2, &VlkRef->ComputeQueue);
    }

    // Create query pool
    if (VlkRef->PhysicalDeviceFeatures.pipelineStatisticsQuery)
    {
        VkQueryPoolCreateInfo QueryPoolCreateInfo
        {
          .sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO,
          .queryType = VK_QUERY_TYPE_PIPELINE_STATISTICS,
          .queryCount = VlkRef->UsedFramesOnTheFly,
          .pipelineStatistics =
            VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT |
            VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT |
            VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT |
            VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT |
            VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT |
            VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT |
            VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT,
        };
        vkCreateQueryPool(VlkRef->Device, &QueryPoolCreateInfo, nullptr, &VlkRef->StatisticsQueryPool);
    }
    VkQueryPoolCreateInfo QueryPoolCreateInfo
    {
      .sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO,
      .queryType = VK_QUERY_TYPE_TIMESTAMP,
      .queryCount = VlkRef->UsedFramesOnTheFly * 2,  // 2 marks per frame
    };
    vkCreateQueryPool(VlkRef->Device, &QueryPoolCreateInfo, nullptr, &VlkRef->TimestampQueryPool);
    VlkRef->TimestampPeriod = VlkRef->PhysicalDeviceProperties.limits.timestampPeriod;

    logger::info << "Logical device successfully created.";
    return TRUE;
} /* End of 'tam::vlk::device::Create' function */

/* Destroy logical device function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::device::Destroy(VOID)
{
    if (Device != VK_NULL_HANDLE)
    {
        if (VlkRef->StatisticsQueryPool != VK_NULL_HANDLE)
            vkDestroyQueryPool(Device, VlkRef->StatisticsQueryPool, nullptr);
        if (VlkRef->TimestampQueryPool != VK_NULL_HANDLE)
            vkDestroyQueryPool(Device, VlkRef->TimestampQueryPool, nullptr);
        vkDestroyDevice(Device, nullptr);
    }
    VlkRef->StatisticsQueryPool = VK_NULL_HANDLE;
    VlkRef->TimestampQueryPool = VK_NULL_HANDLE;
    Device = VK_NULL_HANDLE;
} /* End of 'tam::vlk::device::Destroy' function */

/* END OF 'tam_vlk_device.cpp' FILE */
