/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_instance.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Vulkan instance declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 29.01.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_vlk_instance_h_
#define __tam_vlk_instance_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* 'vlk' namespace */
  namespace vlk
  {
    /* 'instance' handle class */
    class instance : virtual public vlk_ref
    {
    public:
      VkInstance Instance {}; // Instance object 

      /* Create instance function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) TRUE if instance was successfully created, FALSE otherwise.
       */
      BOOL Create( VOID );

      /* Destroy instance function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Destroy( VOID );
    }; /* End of 'instance' class */
  } /* end of 'vlk' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_vlk_instance_h_ */

/* END OF 'tam_vlk_instance.h' FILE */
