/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vulkan_debug.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Vulkan debug structure declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 05.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_vulkan_debug_h_
#define __tam_vulkan_debug_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* 'vlk' namespace */
  namespace vlk
  {
    /* 'debug' handle class */
    class debug : virtual public vlk_ref
    {
    private:
      VkDebugUtilsMessengerEXT DebugUtilsMessenger {}; // Debug messenger

      /* Helpful message parsing and output utility function.
       * ARGUMENTS:
       *   - message to output:
       *       const CHAR *Msg;
       * RETURNS: None.
       */
      static VOID DebugOutputParsed( const CHAR *Msg );

    public:
      /* Create Vulkan debug function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) TRUE if Vulkan debug was successfully created, FALSE otherwise.
       */
      BOOL Create( VOID );

      /* Destroy Vulkan debug function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Destroy( VOID );
    }; /* End of 'debug' class */
  } /* end of 'vlk' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_vulkan_debug_h_ */

/* END OF 'tam_vulkan_debug.h' FILE */
