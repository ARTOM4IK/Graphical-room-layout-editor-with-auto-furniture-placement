/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_instance.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Vulkan instance functions implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 27.01.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

// Helpful attribute settings macros and functions
static auto attr =
  []( UINT Attr )
  {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attr); 
  };

// Attribute color values setup functions
static const auto AttrTxt = [=](){attr(0x07);};
static const auto AttrInfo = [=](){attr(0x1E);};
static const auto AttrLayer = [=](){attr(0x0E);};
static const auto AttrMsg = [=](){attr(0x0F);};
static const auto AttrVlk = [=](){attr(0x0B);};
static const auto AttrStr = [=](){attr(0x06);};
static const auto AttrUrl = [=](){attr(0x02);};
static const auto AttrHex = [=](){attr(0x03);};
static const auto AttrFile = [=](){attr(0x02);};
static const auto AttrField = [=](){attr(0x0A);};

/* Helpful message parsing and aout put utility function.
 * ARGUMENTS:
 *   const CHAR *Msg;
 * RETURNS: None.
 */
VOID DebugOutputParsed( const CHAR *Msg )
{
  AttrInfo();
  /// std::cout << "AqIm: " << ActiveSwapchainImage << " CuFr: " << CurrentFrame << std::endl;
  auto is_prefix =
    []( const CHAR *Msg, const CHAR *Prefix ) -> BOOL
    {
      while (*Msg != 0 && *Prefix != 0 && *Msg == *Prefix)
        Msg++, Prefix++;
      return *Prefix == 0;
    };
  AttrTxt();
  const CHAR *ptr = Msg, *p1;

  //"Validation Warning:"
  //"Validation Information:"
  do
  {
    // Camel style detect
    BOOL is_camel = FALSE;
    p1 = ptr;
    if (ptr == Msg ||
        !std::islower((UCHAR)ptr[-1]) &&
        !std::isupper((UCHAR)ptr[-1]) &&
        !std::isdigit((UCHAR)ptr[-1]) &&
        ptr[-1] != '\\' && ptr[-1] != '/')
    {
      while (std::islower((UCHAR)*p1))
        p1++;
      while (p1 != ptr && std::isupper((UCHAR)*p1))
      {
        p1++;
        while (std::islower((UCHAR)*p1) || std::isdigit((UCHAR)*p1))
          p1++;
        is_camel = TRUE;
      }
    }
    // Common parsing
    if (is_prefix(ptr, "VUID-") || is_prefix(ptr, "UNASSIGNED-"))
    {
      AttrLayer();
      while (*ptr != 0 && (std::isalpha((UCHAR)*ptr) || std::isdigit((UCHAR)*ptr) || *ptr == '_' || *ptr == '-'))
        std::cout << *ptr++;
      AttrTxt();
    }
    else if (is_prefix(ptr, "(https://"))
    {
      AttrUrl();
      while (*ptr != 0 && *ptr != ')')
        std::cout << *ptr++;
      AttrTxt();
    }
    else if (is_prefix(ptr, "VK_") || is_prefix(ptr, "vk") || is_prefix(ptr, "Vk"))
    {
      p1 = ptr;
      AttrVlk();
      while (*ptr != 0 && (std::isalpha((UCHAR)*ptr) || std::isdigit((UCHAR)*ptr) || *ptr == '_'))
        std::cout << *ptr++;
      if (is_prefix(p1, "vk") && is_prefix(ptr, "()"))
        std::cout << *ptr++, std::cout << *ptr++;
      AttrTxt();
    }
    else if (is_camel)
    {
      AttrField();
      while (*ptr != 0 && ptr != p1)
        std::cout << *ptr++;
      AttrTxt();
    }
    else if ((is_prefix(ptr, "C:\\") || is_prefix(ptr, "D:\\") || is_prefix(ptr, "X:\\")) &&
             ((p1 = std::strstr(ptr, ".json")) != nullptr ||
              (p1 = std::strstr(ptr, ".dll")) != nullptr ||
              (p1 = std::strstr(ptr, ".txt")) != nullptr))
    {
      AttrFile();
      while (*ptr != 0 && ptr != p1)
        std::cout << *ptr++;
      std::cout << *ptr++;
      while (std::isalpha((UCHAR)*ptr))
        std::cout << *ptr++;
      AttrTxt();
    }
    else if (is_prefix(ptr, "0x"))
    {
      AttrHex();
      std::cout << *ptr++;
      std::cout << *ptr++;
      while (*ptr != 0 && std::isxdigit((UCHAR)*ptr))
        std::cout << *ptr++;
      AttrTxt();
    }
    else if (*ptr == '\"')
    {
      // string
      AttrStr();
      std::cout << *ptr++;
      while (*ptr != 0 && *ptr != '\"')
        std::cout << *ptr++;
      if (*ptr != 0)
        std::cout << *ptr++;
      AttrMsg();
      p1 = ptr;
    }
    else
      std::cout << *ptr++;
  } while (*ptr != 0);
} /* End of 'vivk::vlk::DebugOutputParsed' function */


/* Vulkan debug callbacks creation funtion.
 * ARGUMENTS: None.
 * RETURNS:
 *   (VkDebugReportCallbackCreateInfoEXT) filled data.
 */
VkDebugReportCallbackCreateInfoEXT CreateDebugCallbacks( VOID )
{
  VkDebugReportCallbackCreateInfoEXT DebugReportCallbackInfo
  {
    .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
    .flags =
      VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
      VK_DEBUG_REPORT_WARNING_BIT_EXT |
      VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
      VK_DEBUG_REPORT_ERROR_BIT_EXT |
      VK_DEBUG_REPORT_DEBUG_BIT_EXT,
    .pfnCallback =
      /* Debug handle callback function.
       * ARGUMENTS:
       *   - callback flags (see VK_DEBUG_REPORT_***):
       *       VkDebugReportFlagsEXT Flags;
       *   - type of object (VK_DEBUG_REPORT_OBJECT_TYPE_***):
       *       VkDebugReportObjectTypeEXT ObjectType;
       *   - object (for VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT is undefined):
       *       UINT64 Object;
       *   - component (layer, driver, loader) defined value specifying the location:
       *       UINT_PTR Location;
       *   - layer-defined value:
       *       INT32 MessageCode;
       *   - null-terminated string with abbreviation of the name of the component:
       *       const CHAR *LayerPrefix;
       *   - null-terminated message string:
       *       const CHAR *Message;
       *   - user data (from initial call):
       *       VOID *UserData
       * RETURNS:
       *   (VkBool32) always return VK_FALSE.
       */
      []( VkDebugReportFlagsEXT Flags, VkDebugReportObjectTypeEXT ObjectType, UINT64 Object,
          UINT_PTR Location, INT32 MessageCode, const CHAR *LayerPrefix, const CHAR *Message, VOID *UserData ) -> VkBool32
      {
        //vivk::vlk *Vlk = reinterpret_cast<vivk::vlk *>(UserData);

        if ((Flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) != 0)
          return VK_FALSE;
        /// std::string Res = "DEBUG REPORT: (";
        /// 
        /// if ((Flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) != 0)
        ///   Res += "INFO";
        /// if ((Flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) != 0)
        ///   Res += "WARNING";
        /// if ((Flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) != 0)
        ///   Res += "PERFORMANCE";
        /// if ((Flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) != 0)
        ///   Res += "ERROR";
        /// Res += ")";
        /// Res += std::string("{") + LayerPrefix + "} " + Message + "\n";
        /// OutputDebugString(Res.c_str());

        AttrTxt();
        //std::cout << "DEBUG REPORT: ";
        AttrInfo();
        if ((Flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) != 0)
          std::cout << "INFO";
        if ((Flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) != 0)
          std::cout << "WARNING";
        if ((Flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) != 0)
          std::cout << "PERFORMANCE";
        if ((Flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) != 0)
          std::cout << "ERROR";
        AttrLayer();
        static std::map<std::string, UINT> Attrs
        {
          {"Loader Message", 0x0E},
          {"Validation", 0x0C},
        };
        if (Attrs.find(LayerPrefix) != Attrs.end())
          attr(Attrs[LayerPrefix]);
        std::cout << " {" << LayerPrefix << "}\n";
        AttrMsg();

        /// const CHAR *ptr = Message, *p1;
        /// do
        /// {
        ///   if ((p1 = std::strstr(ptr, "\"")) == nullptr)
        ///   {
        ///     if ((p1 = std::strstr(ptr, "VK_")) == nullptr)
        ///       std::cout << ptr;
        ///     else
        ///     {
        ///       // plain text
        ///       while (*ptr != 0 && ptr != p1)
        ///         std::cout << *ptr++;
        ///       // vulkan constant
        ///       AttrVlk();
        ///       while (*ptr != 0 && (std::isalpha((UCHAR)*ptr) || std::isdigit((UCHAR)*ptr) || *ptr == '_'))
        ///         std::cout << *ptr++;
        ///       AttrMsg();
        ///     }
        ///   }
        ///   else
        ///   {
        ///     // plain text
        ///     while (*ptr != 0 && ptr != p1)
        ///       std::cout << *ptr++;
        ///     // string
        ///     AttrStr();
        ///     if (*ptr != 0)
        ///       std::cout << *ptr++;
        ///     while (*ptr != 0 && *ptr != '\"')
        ///       std::cout << *ptr++;
        ///     if (*ptr != 0)
        ///       std::cout << *ptr++;
        ///     AttrMsg();
        ///   }
        /// } while (p1 != nullptr);
        DebugOutputParsed(Message);
        // std::cout << Message << "\n";
        std::cout << std::endl;
        return VK_FALSE;
      },
    .pUserData = nullptr, /// reinterpret_cast<VOID *>(this),
  };
  return DebugReportCallbackInfo;
} /* End of 'vivk::vlk::CreateDebugCallbacks' function */

/* Create instance function.
 * ARGUMENTS: None.
 * RETURNS: 
 *   (BOOL) TRUE if instance was successfully created, FALSE otherwise.
 */
BOOL tam::vlk::instance::Create( VOID )
{
  // Informational application's structure
  VkApplicationInfo ApplicationInfo
  {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pApplicationName = "TAM Vulkan Animation Project",
    .applicationVersion = 1,
    .apiVersion = VK_MAKE_VERSION(1, 4, 0),   // Target version of VulkanAPI.
  };

  // Informational structure for instance's creation.
  VkInstanceCreateInfo InstanceCreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pApplicationInfo = &ApplicationInfo,
  };

  UINT InstanceExtensionsCount = 0;
  std::vector<VkExtensionProperties> ExtensionProperties;
  // Query the instance extensions.
  vkEnumerateInstanceExtensionProperties(nullptr, &InstanceExtensionsCount, nullptr);
  // If there are any extensions, query their properties.
  if (InstanceExtensionsCount != 0)
  {
    ExtensionProperties.resize(InstanceExtensionsCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &InstanceExtensionsCount, ExtensionProperties.data());
  }

  // Set instance's extensions.
  BOOL HasValidationFeatures = FALSE;
  std::vector<const CHAR *> InstanceExtensionNames {};
  for (auto &ext : ExtensionProperties)
  {
#ifndef NDEBUG
    if (strcmp(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, ext.extensionName) == 0)
      InstanceExtensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif // NDEBUG
    if (strcmp(VK_KHR_SURFACE_EXTENSION_NAME, ext.extensionName) == 0)
      InstanceExtensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    else if (strcmp(VK_KHR_WIN32_SURFACE_EXTENSION_NAME, ext.extensionName) == 0)
      InstanceExtensionNames.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#ifndef NDEBUG
    else if (strcmp(VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME, ext.extensionName) == 0)
    {
      HasValidationFeatures = TRUE;
      InstanceExtensionNames.push_back(VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME);
    }
#endif // NDEBUG
  }

  InstanceCreateInfo.enabledExtensionCount = (UINT32)InstanceExtensionNames.size();
  InstanceCreateInfo.ppEnabledExtensionNames = InstanceExtensionNames.data();

  UINT LayerPropertiesCount = 0;
  std::vector<VkLayerProperties> LayerProperties;
  vkEnumerateInstanceLayerProperties(&LayerPropertiesCount, nullptr);
  if (LayerPropertiesCount != 0)
  {
    LayerProperties.resize(LayerPropertiesCount);
    vkEnumerateInstanceLayerProperties(&LayerPropertiesCount, LayerProperties.data());
  }

  // Set instance's layers.
  std::vector<const CHAR *> EnabledLayersNames {};
  for (auto &layer : LayerProperties)
    if (strcmp("VK_LAYER_KHRONOS_validation", layer.layerName) == 0)
    {
#ifndef NDEBUG
      EnabledLayersNames.push_back("VK_LAYER_KHRONOS_validation");
#endif // NDEBUG
      break;
    }

  InstanceCreateInfo.enabledLayerCount = (UINT32)EnabledLayersNames.size();
  InstanceCreateInfo.ppEnabledLayerNames = EnabledLayersNames.data();

#ifndef NDEBUG
  // Set validation possibility.
  VkValidationFeaturesEXT ValidationFeatures
  {
    .sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT,
  };
  std::vector<VkValidationFeatureEnableEXT> ValidationFeaturesEnable
  {
    VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT,    // Used for shaders' validation
    VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT   // Provides additional warnings' output
  };
  ValidationFeatures.enabledValidationFeatureCount = (UINT32)ValidationFeaturesEnable.size();
  ValidationFeatures.pEnabledValidationFeatures = ValidationFeaturesEnable.data();

  if (HasValidationFeatures)
    InstanceCreateInfo.pNext = &ValidationFeatures;

  VkDebugReportCallbackCreateInfoEXT DebugReportCallbackInfo = CreateDebugCallbacks();
  ValidationFeatures.pNext = &DebugReportCallbackInfo;
#endif // NDEBUG
  if (vkCreateInstance(&InstanceCreateInfo, nullptr, &Instance) != VK_SUCCESS)
  {
    logger::error << "Unable to create vulkan instance.";
    return FALSE;
  }
  logger::info << "Vulkan instance successfully created.";
  return TRUE;
} /* End of 'tam::vlk::instance::Create' function */

/* Destroy instance function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::instance::Destroy( VOID )
{
  if (Instance != VK_NULL_HANDLE)
    vkDestroyInstance(Instance, nullptr);
  Instance = VK_NULL_HANDLE;
} /* End of 'tam::vlk::instance::Destroy' function */

/* END OF 'tam_vlk_instance.cpp' FILE */
