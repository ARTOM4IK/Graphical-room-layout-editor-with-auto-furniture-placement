/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_physdevice.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Physical device functions implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 07.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Choose physical device function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) TRUE if physical device was successfully found, FALSE otherwise.
 */
BOOL tam::vlk::physdevice::Create( VOID )
{
  // Get number of physical devices.
  std::vector<VkPhysicalDevice> PhysDevices;
  UINT32 PhysicalDeviceCount = 0;
  if (vkEnumeratePhysicalDevices(VlkRef->Instance, &PhysicalDeviceCount, nullptr) != VK_SUCCESS)
  {
    logger::error << "Unable to count physical devices.";
    return FALSE;
  }

  // Obtaining physical devices.
  PhysDevices.resize(PhysicalDeviceCount);
  if (vkEnumeratePhysicalDevices(VlkRef->Instance, &PhysicalDeviceCount, PhysDevices.data()) != VK_SUCCESS)
  {
    logger::error << "Unable to obtain physical devices.";
    return FALSE;
  }

  // Choosing physical device based on the requirements.
  PhysDevice = VK_NULL_HANDLE;
  UINT64 CurVideoMemory = 0;
  std::vector<VkQueueFamilyProperties> QueueFamilies;

  UINT ExtensionCount = 0;
  std::vector<VkExtensionProperties> AvailableExtensions;
  for (VkPhysicalDevice &Device : PhysDevices)
  {
    VkPhysicalDeviceProperties CurDeviceProperties;
    vkGetPhysicalDeviceProperties(Device, &CurDeviceProperties);

    VkPhysicalDeviceFeatures CurDeviceFeatures;
    vkGetPhysicalDeviceFeatures(Device, &CurDeviceFeatures);

    vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, nullptr);

    QueueFamilies.resize(QueueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, QueueFamilies.data());

    vkEnumerateDeviceExtensionProperties(Device, nullptr, &ExtensionCount, nullptr);

    AvailableExtensions.resize(ExtensionCount);

    vkEnumerateDeviceExtensionProperties(Device, nullptr, &ExtensionCount, AvailableExtensions.data());

    if (CurDeviceProperties.limits.maxMemoryAllocationCount >= CurVideoMemory)
    {
      PhysDevice = Device;
      CurVideoMemory = CurDeviceProperties.limits.maxMemoryAllocationCount;
      VideoMemory = CurVideoMemory;
      PhysicalDeviceProperties = CurDeviceProperties;
      PhysicalDeviceFeatures = CurDeviceFeatures;
      IsSwapchain = FALSE;
      for (auto Ext: AvailableExtensions)
        if(strcmp(Ext.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
          IsSwapchain = TRUE;
    }
  }
  if (PhysDevice == VK_NULL_HANDLE)
  {
    logger::error << "Unable to obtain physical device.";
    return FALSE;
  }
  VkPhysicalDeviceIDProperties PhysDeviceIDProps
  {
    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES
  };
  VkPhysicalDeviceProperties2 PhysDeviceProps
  {
    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2
  };
  PhysDeviceProps.pNext = &PhysDeviceIDProps;
  vkGetPhysicalDeviceProperties2(PhysDevice, &PhysDeviceProps);
  memcpy(SystemProfiler.DeviceLUID, PhysDeviceIDProps.deviceLUID, sizeof(SystemProfiler.DeviceLUID));
  strncpy(SystemProfiler.DeviceName, PhysicalDeviceProperties.deviceName, sizeof(SystemProfiler.DeviceName) - 1);

  logger::info << std::format("GPU name: {}", PhysicalDeviceProperties.deviceName);
  logger::info << std::format("Device ID: {}", PhysicalDeviceProperties.deviceID);
  logger::info << std::format("Driver version: {:x}", PhysicalDeviceProperties.driverVersion);
  logger::info << std::format("Total memory: {:02} MB", static_cast<FLT>(CurVideoMemory) / 1024 / 1024);
  if (IsSwapchain)
    logger::info << "Swapchain is supported.";
  else
    logger::error << "Swapchain is not supported.";

  logger::info << "Physical device successfully chosen.";

#ifndef NDEBUG
  logger::line << logger::SetColorStr(0x00FFFF) << "\nPhysical device info:\n" << logger::SetDefStr();
  logger::line << std::format("Physical device name: {}\n", PhysicalDeviceProperties.deviceName);
  logger::line << std::format("Physical device type: {}\n",
    PhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_OTHER ? "Other" :
    PhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? "Integrated GPU" :
    PhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? "Discrete GPU" :
    PhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU ? "Virtual GPU" :
    PhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU ? "CPU" : " Unknown");
  logger::line << std::format("Buffer granularity: {}\n", PhysicalDeviceProperties.limits.bufferImageGranularity);
  logger::line << std::format("Max color attachments: {}\n", static_cast<UINT>(PhysicalDeviceProperties.limits.maxColorAttachments));
  logger::line << std::format("Point Size Granularity: {}\n", PhysicalDeviceProperties.limits.pointSizeGranularity);
  logger::line << std::format("Line Width Granularity: {}\n", PhysicalDeviceProperties.limits.lineWidthGranularity);
  logger::line << std::format("Optimal Buffer Copy Offset Alignment: {}\n", PhysicalDeviceProperties.limits.optimalBufferCopyOffsetAlignment);
  logger::line << std::format("Optimal Buffer Copy Row Pitch Alignment: {}\n", PhysicalDeviceProperties.limits.optimalBufferCopyRowPitchAlignment);
  logger::line << std::format("Non Coherent Atom Size: {}\n", PhysicalDeviceProperties.limits.nonCoherentAtomSize);
  logger::line << std::format("Max memory allocations: {}\n", PhysicalDeviceProperties.limits.maxMemoryAllocationCount);
  logger::line << std::format("Max Push Constants Size: {}\n", PhysicalDeviceProperties.limits.maxPushConstantsSize);
  logger::line << std::format("Max Sampler Allocation Count: {}\n", PhysicalDeviceProperties.limits.maxSamplerAllocationCount);
  logger::line << std::format("Max Geometry Input Components: {}\n", PhysicalDeviceProperties.limits.maxGeometryInputComponents);
  logger::line << std::format("Max Geometry Output Components: {}\n", PhysicalDeviceProperties.limits.maxGeometryOutputComponents);
  logger::line << std::format("Max Geometry Output Vertices: {}\n", PhysicalDeviceProperties.limits.maxGeometryOutputVertices);
  logger::line << std::format("Max Fragment Output Attachments: {}\n", PhysicalDeviceProperties.limits.maxFragmentOutputAttachments);
  logger::line << std::format("Max Color Attachments: {}\n", PhysicalDeviceProperties.limits.maxColorAttachments);

  logger::info << logger::SetColorStr(0x00FFFF) << std::format("\nPhysical Device Features:\n") << logger::SetDefStr();
  logger::line << std::format("geometryShader = {:2}\n", PhysicalDeviceFeatures.geometryShader);
  logger::line << std::format("tessellationShader = {:2}\n", PhysicalDeviceFeatures.tessellationShader );
  logger::line << std::format("multiDrawIndirect = {:2}\n", PhysicalDeviceFeatures.multiDrawIndirect );
  logger::line << std::format("wideLines = {:2}\n", PhysicalDeviceFeatures.wideLines );
  logger::line << std::format("largePoints = {:2}\n", PhysicalDeviceFeatures.largePoints );
  logger::line << std::format("multiViewport = {:2}\n", PhysicalDeviceFeatures.multiViewport );
  logger::line << std::format("occlusionQueryPrecise = {:2}\n", PhysicalDeviceFeatures.occlusionQueryPrecise );
  logger::line << std::format("pipelineStatisticsQuery = {:2}\n", PhysicalDeviceFeatures.pipelineStatisticsQuery );
  logger::line << std::format("shaderFloat64 = {:2}\n", PhysicalDeviceFeatures.shaderFloat64 );
  logger::line << std::format("shaderInt64 = {:2}\n", PhysicalDeviceFeatures.shaderInt64 );
  logger::line << std::format("shaderInt16 = {:2}\n", PhysicalDeviceFeatures.shaderInt16 );

  logger::line << logger::SetColorStr(0x00FFFF) << std::format("\nMemory heaps size properties:\n") << logger::SetDefStr();
#endif /* NDEBUG */
  VkPhysicalDeviceMemoryProperties PhysDeviceMemoryProperties;
  vkGetPhysicalDeviceMemoryProperties(PhysDevice, &PhysDeviceMemoryProperties);
  for (INT m = 0; m < PhysDeviceMemoryProperties.memoryHeapCount; m++)
  {
    const CHAR *heapinfo = PhysDeviceMemoryProperties.memoryHeaps[m].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT ? " device local " :
                           PhysDeviceMemoryProperties.memoryHeaps[m].flags & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT ? " multi instance " : " host ";
#ifndef NDEBUG
    logger::line << std::format("{}, {} MiB\n", heapinfo, PhysDeviceMemoryProperties.memoryHeaps[m].size / 1024 / 1024);
#endif /* NDEBUG */
    if (PhysDeviceMemoryProperties.memoryHeaps[m].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT &&
        SystemProfiler.TotalGPUMemory < PhysDeviceMemoryProperties.memoryHeaps[m].size)
      SystemProfiler.TotalGPUMemory = PhysDeviceMemoryProperties.memoryHeaps[m].size;
  }
#ifndef NDEBUG
  logger::line << logger::SetColorStr(0x00FFFF) << std::format("\nMemory heaps types properties:\n") << logger::SetDefStr();
  for (INT i = 0; i < PhysDeviceMemoryProperties.memoryTypeCount; i++ )
  {
    VkMemoryType vmt = PhysDeviceMemoryProperties.memoryTypes[i];
    VkMemoryPropertyFlags vmpf = vmt.propertyFlags;
    logger::line << std::format("{} :: Memory (heap index: {})", i, vmt.heapIndex);
    if ((vmpf & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0)
      logger::line << std::format(" Device_Local");
    if ((vmpf & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0)
      logger::line << std::format(" Host_Visible");
    if ((vmpf & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != 0)
      logger::line << std::format(" Host_Coherent");
    if ((vmpf & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) != 0)
      logger::line << std::format(" Host_Cached");
    if ((vmpf & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) != 0)
      logger::line << std::format(" Lazily_Allocated");
    if ((vmpf & VK_MEMORY_PROPERTY_PROTECTED_BIT) != 0)
      logger::line << std::format(" Protected");
    if ((vmpf & VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD) != 0)
      logger::line << std::format(" Device_Coherent_AMD");
    if ((vmpf & VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD) != 0)
      logger::line << std::format(" Device_Uncached_AND");
    if ((vmpf & VK_MEMORY_PROPERTY_RDMA_CAPABLE_BIT_NV) != 0)
      logger::line << std::format(" RDMA_Capable_nVidia");
    logger::line << std::format("\n");
  }
  logger::line << logger::SetColorStr(0x00FFFF) << std::format("\nMemory heaps info properties:\n") << logger::SetDefStr();
  for (INT i = 0; i < PhysDeviceMemoryProperties.memoryHeapCount; i++ )
  {
    VkMemoryHeap vmt = PhysDeviceMemoryProperties.memoryHeaps[i];

    VkMemoryHeapFlags vmpf = vmt.flags;
    logger::line << std::format("{} :: Heap (size: {} byte[s] / {} KiB / {} MiB / {} GiB)", i, vmt.size, vmt.size / 1024, vmt.size / (1024 * 1024), vmt.size / (1024 * 1024 * 1024));
    if ((vmpf & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) != 0)
      logger::line << std::format(" DeviceLocal");
    if ((vmpf & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT) != 0)
      logger::line << std::format(" Multi-instance");
    if ((vmpf & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT_KHR) != 0)
      logger::line << std::format(" Multi-instance(KHR)");
    logger::line << std::format("\n");
  }

  // Get device queue families
  UINT QueueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(PhysDevice, &QueueFamilyCount, nullptr);
  QueueFamilies.resize(QueueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(PhysDevice, &QueueFamilyCount, QueueFamilies.data());
  logger::line << logger::SetColorStr(0x00FFFF) << std::format("\nQueues info properties:\n") << logger::SetDefStr();
  for (INT i = 0; i < QueueFamilyCount; i++ )
  {
    std::string s = std::format("\t{:2} : Queue Family Count = {:2} -> ", std::to_string(i), QueueFamilies[i].queueCount);
    if ((QueueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0 )
      s += " Graphics";
    if ((QueueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0 )
      s += " Compute";
    if ((QueueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0 )
      s += " Transfer";
    s += "\n";
    logger::line << std::format("{}", s);
  }
#endif /* NDEBUG */
  return TRUE;
} /* End of 'tam::vlk::physdevice::Create' function */

/* END OF 'tam_vlk_physdevice.cpp' FILE */
