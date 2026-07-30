/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_physdevice.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Physical device declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 29.01.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_vlk_physdevice_h_
#define __tam_vlk_physdevice_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* 'vlk' namespace */
  namespace vlk
  {
    /* 'physdevice' handle class */
    class physdevice : virtual public vlk_ref
    {
    public:
      VkPhysicalDevice PhysDevice {};                         // Physical GPU device object
      UINT32 QueueFamilyCount {};                             // Amount of queue families
      UINT64 VideoMemory {};                                  // Video memory
      VkPhysicalDeviceFeatures PhysicalDeviceFeatures {};     // Structure with enable device features
      VkPhysicalDeviceProperties PhysicalDeviceProperties {}; // Structure with available device properties
      std::vector<VkQueueFamilyProperties> QueueFamilies {};  // Queue families properties
      BOOL IsSwapchain = FALSE;                               // Flag if swapchain is valid or not

      /* Choose physical device function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) TRUE if physical device was successfully found, FALSE otherwise.
       */
      BOOL Create( VOID );
    }; /* End of 'physdevice' class */
  } /* end of 'vlk' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_vlk_physdevice_h_ */

/* END OF 'tam_vlk_physdevice.h' FILE */
