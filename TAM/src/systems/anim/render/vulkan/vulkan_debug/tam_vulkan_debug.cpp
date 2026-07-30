/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vulkan_debug.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Vulkan debug functions implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 05.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Parse object type function.
 * ARGUMENTS:
 *   - Object type:
 *      const VkObjectType Type;
 * RETURNS:
 *   (std::string) object type as a string.
 */
std::string ObjToString( const VkObjectType Type )
{
  switch(Type)
  {
  case(VK_OBJECT_TYPE_UNKNOWN):
    return "UNKNOWN";
  case(VK_OBJECT_TYPE_INSTANCE):
    return "INSTANCE";
  case(VK_OBJECT_TYPE_PHYSICAL_DEVICE):
    return "PHYSICAL_DEVICE";
  case(VK_OBJECT_TYPE_DEVICE):
    return "DEVICE";
  case(VK_OBJECT_TYPE_QUEUE):
    return "QUEUE";
  case(VK_OBJECT_TYPE_SEMAPHORE):
    return "SEMAPHORE";
  case(VK_OBJECT_TYPE_COMMAND_BUFFER):
    return "COMMAND_BUFFER";
  case(VK_OBJECT_TYPE_FENCE):
    return "FENCE";
  case(VK_OBJECT_TYPE_DEVICE_MEMORY):
    return "DEVICE_MEMORY";
  case(VK_OBJECT_TYPE_BUFFER):
    return "BUFFER";
  case(VK_OBJECT_TYPE_IMAGE):
    return "IMAGE";
  case(VK_OBJECT_TYPE_EVENT):
    return "EVENT";
  case(VK_OBJECT_TYPE_QUERY_POOL):
    return "QUERY_POOL";
  case(VK_OBJECT_TYPE_BUFFER_VIEW):
    return "BUFFER_VIEW";
  case(VK_OBJECT_TYPE_IMAGE_VIEW):
    return "IMAGE_VIEW";
  case(VK_OBJECT_TYPE_SHADER_MODULE):
    return "SHADER_MODULE";
  case(VK_OBJECT_TYPE_PIPELINE_CACHE):
    return "PIPELINE_CACHE";
  case(VK_OBJECT_TYPE_PIPELINE_LAYOUT):
    return "PIPELINE_LAYOUT";
  case(VK_OBJECT_TYPE_RENDER_PASS):
    return "RENDER_PASS";
  case(VK_OBJECT_TYPE_PIPELINE):
    return "PIPELINE";
  case(VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT):
    return "DESCRIPTOR_SET_LAYOUT";
  case(VK_OBJECT_TYPE_SAMPLER):
    return "SAMPLER";
  case(VK_OBJECT_TYPE_DESCRIPTOR_POOL):
    return "DESCRIPTOR_POOL";
  case(VK_OBJECT_TYPE_DESCRIPTOR_SET):
    return "DESCRIPTOR_SET";
  case(VK_OBJECT_TYPE_FRAMEBUFFER):
    return "FRAMEBUFFER";
  case(VK_OBJECT_TYPE_COMMAND_POOL):
    return "COMMAND_POOL";
  case(VK_OBJECT_TYPE_PRIVATE_DATA_SLOT):
    return "PRIVATE_DATA_SLOT";
  case(VK_OBJECT_TYPE_SURFACE_KHR):
    return "SURFACE_KHR";
  case(VK_OBJECT_TYPE_SWAPCHAIN_KHR):
    return "SWAPCHAIN_KHR";
  case(VK_OBJECT_TYPE_DISPLAY_KHR):
    return "DISPLAY_KHR";
  case(VK_OBJECT_TYPE_DISPLAY_MODE_KHR):
    return "DISPLAY_MODE_KHR";
  case(VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT):
    return "DEBUG_REPORT_CALLBACK_EXT";
  }
  return "";
} /* End of 'ObjToString' function */

/* Helpful message parsing and output utility function.
 * ARGUMENTS:
 *   - message to output:
 *       const CHAR *Msg;
 * RETURNS: None.
 */
VOID tam::vlk::debug::DebugOutputParsed( const CHAR *Msg )
{
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
} /* End of 'tam::vlk::debug::DebugOutputParsed' function */


/* Create Vulkan debug function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) TRUE if Vulkan debug was successfully created, FALSE otherwise.
 */
BOOL tam::vlk::debug::Create( VOID ) 
{
  VkDebugUtilsMessengerCreateInfoEXT DebugUtilsMessengerCreateInfo
  {
     .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
     .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,
     .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
    .pfnUserCallback =
       []( VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity, 
           VkDebugUtilsMessageTypeFlagsEXT MessageTypes, 
           const VkDebugUtilsMessengerCallbackDataEXT *CallBackData,
           VOID *UserData ) -> VkBool32
      {
        DWORD MsgColor = 0xFFFFF;
        // tam::logger::out << std::endl;
        tam::logger::out << tam::logger::SetColorStr(0xEE9900) << "<Vulkan Debug> " << tam::logger::SetDefStr();

        switch (MessageSeverity)
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
          tam::logger::line << /* "Severity: " << */ tam::logger::SetColorStr(0xFFFFFF) << "VERBOSE" << tam::logger::SetDefStr();
          break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
          tam::logger::line << /* "Severity: " << */ tam::logger::SetColorStr(0xFFFF00, 0x0000FF /* 0x0088FF */) << "INFO" << tam::logger::SetDefStr();
          MsgColor = 0xAAAAFF;
          break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
          tam::logger::line << /* "Severity: " << */ tam::logger::SetColorStr(0xFFFFFF, 0xAA8822 /* 0xDDDD00 */) << "WARNING" << tam::logger::SetDefStr();
          MsgColor = 0xFFFFAA;
          break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
          tam::logger::line << /* "Severity: " << */ tam::logger::SetColorStr(0xFFFFFF, 0xFF0000 /* 0xEE0000 */) << "ERROR" << tam::logger::SetDefStr();
          MsgColor = 0xFFAAAA;
          break;
        }

        // tam::logger::line << " Types:";
        tam::logger::line << tam::logger::SetColorStr(0xDD00DD);
        if (MessageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
          tam::logger::line << tam::logger::SetColorStr(0xDD88DD) << " GENERAL";
        if (MessageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
          tam::logger::line << tam::logger::SetColorStr(0xFFAADD) << " VALIDATION";
        if (MessageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
          tam::logger::line << tam::logger::SetColorStr(0xDDFFDD) << " PERFORMANCE";
        
        tam::logger::line << " ";
        tam::logger::line << tam::logger::SetColorStr(0xFFFF00);
        // static std::map<std::string, UINT> Attrs
        // {
        //   {"Loader Message", 0xFFFF00},
        //   {"Validation", 0xFF0000},
        // };
        // if (Attrs.find(LayerPrefix) != Attrs.end())
        //   tam::logger::line << tam::logger::SetColorStr(Attrs[LayerPrefix]);
        // std::cout << " {" << LayerPrefix << "}\n";
        // AttrMsg();
        DebugOutputParsed(CallBackData->pMessage);

        // tam::logger::out << tam::logger::SetDefStr() << "Message ID: " << CallBackData->messageIdNumber;
        // if (CallBackData->pMessageIdName)
        //   tam::logger::line << "(" << tam::logger::SetColorStr(0x00DD00) << CallBackData->pMessageIdName << tam::logger::SetDefStr() << ")";
        //   
        // std::string msg = CallBackData->pMessage;
        // if (SIZE_T pos = msg.find("(https://"); pos != std::string::npos)
        //   msg.erase(pos);
        // tam::logger::out << "Message: " << tam::logger::SetColorStr(MsgColor) << msg << tam::logger::SetDefStr();
        // 
        // if (CallBackData->objectCount > 0)
        // {
        //   tam::logger::out << "Related Objects (" << (INT)CallBackData->objectCount << ")";
        //     
        //   for (UINT i = 0; i < CallBackData->objectCount; i++)
        //   {
        //     const auto& obj = CallBackData->pObjects[i];
        //       
        //     tam::logger::out << "Object [" << (INT)i << "]:";
        //     tam::logger::line << tam::logger::SetColorStr(0x00FFEE) << ObjToString(obj.objectType) << tam::logger::SetDefStr();
        //       
        //     if (obj.pObjectName)
        //       tam::logger::out << "  Name:" << obj.pObjectName;
        //       
        //     if (obj.objectType == VK_OBJECT_TYPE_DEVICE || obj.objectType == VK_OBJECT_TYPE_PHYSICAL_DEVICE)
        //       tam::logger::out << "  Device Object";
        //     else if (obj.objectType == VK_OBJECT_TYPE_QUEUE)
        //       tam::logger::out << "  Queue Object";
        //     else if (obj.objectType == VK_OBJECT_TYPE_COMMAND_BUFFER)
        //       tam::logger::out << "  Command Buffer Object";
        //     else if (obj.objectType == VK_OBJECT_TYPE_BUFFER || obj.objectType == VK_OBJECT_TYPE_IMAGE)
        //       tam::logger::out << "  Resource Object";
        //   }
        // }
        return VK_FALSE;
      },

     .pUserData = nullptr
  };

  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(VlkRef->Instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr)
    func(VlkRef->Instance, &DebugUtilsMessengerCreateInfo, nullptr, &DebugUtilsMessenger);
  else
  {
    tam::logger::error << "Can't create debug handle";
    return FALSE;
  }
  return TRUE;
} /* End of 'tam::vlk::debug::Create' function */

/* Destroy Vulkan debug function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::debug::Destroy( VOID )
{
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(VlkRef->Instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr)
    func(VlkRef->Instance, DebugUtilsMessenger, nullptr);
  else
    tam::logger::error << "Can't destroy debug handle";
} /* End of 'tam::vlk::debug::Destroy' function */

/* END OF 'tam_vulkan_debug.cpp' FILE */
