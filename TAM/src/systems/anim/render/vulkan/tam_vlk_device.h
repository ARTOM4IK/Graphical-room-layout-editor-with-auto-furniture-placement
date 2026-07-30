/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_device.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Logical device structure declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 29.01.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_vlk_device_h_
#define __tam_vlk_device_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* 'vlk' namespace */
  namespace vlk
  {
    /* 'device' handle class */
    class device : virtual public vlk_ref
    {
    public:
      VkDevice Device  = VK_NULL_HANDLE; // GPU device object

      /* Create logical device function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) TRUE if logical device was successfully created, FALSE otherwise.
       */
      BOOL Create( VOID );

      /* Destroy logical device function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Destroy( VOID );
    }; /* End of 'device' class */
  } /* end of 'vlk' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_vlk_device_h_ */

/* END OF 'tam_vlk_device.h' FILE */
