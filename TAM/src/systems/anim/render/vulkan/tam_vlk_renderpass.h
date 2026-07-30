/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

 /* FILE NAME   : tam_vlk_renderpass.h
  * PURPOSE     : Tough Arrangement Manager project.
  *               Vulkan system module.
  *               Renderpass structure declaration file.
  * PROGRAMMER  : CGSG'Sr'2025.
  *               Tretyakov Andrey (AT7).
  * LAST UPDATE : 21.03.2026
  * NOTE        : Namespace 'tam'.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __tam_vlk_renderpass_h_
#define __tam_vlk_renderpass_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* 'vlk' namespace */
  namespace vlk
  {
    /* 'renderpass' handle class */
    class renderpass
    {
    private:
      core *VlkRef;
    public:
      VkRenderPass RenderPass{};                                     // Vulkan renderpass instance
      std::vector<VkAttachmentDescription> AttachmentDescriptions{}; // vector that contains information about attachment's structures
      std::vector<VkAttachmentReference> AttachmentReferences{};     // vector of descriptions of attachment's layouts during the render
      std::vector<VkSubpassDescription> SubpassDescriptions{};       // vector of subpass descriptions
      std::vector<VkSubpassDependency> SubpassDependencies{};        // vector of subpasses dependencies (order, masks etc.)

      /* Default class constructor */
      renderpass( VOID )
      {
      } /* End of 'renderpass' function */

      /* Class constructor for render pipeline only */
      renderpass( core *Ptr );

      /* Create renderpass function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) TRUE if renderpass was created, FALSE otherwise.
       */
      BOOL Create( VOID );

      /* Create shadow renderpass with one subpass function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) TRUE if renderpass was created, FALSE otherwise.
       */
      BOOL CreateShadow( VOID );

      /* Create deferred renderpass with two subpasses function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) TRUE if renderpass was created, FALSE otherwise.
       */
      BOOL CreateDeferred( VOID );

      /* Create merge renderpass with one subpass function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) TRUE if renderpass was created, FALSE otherwise.
       */
      BOOL CreateMerge( VOID );

      /* Create post processing renderpass with one subpass function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) TRUE if renderpass was created, FALSE otherwise.
       */
      BOOL CreatePostProc( VOID );

      /* Destroy renderpass function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Destroy( VOID );
    }; /* End of 'renderpass' class */
  } /* end of 'vlk' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_vlk_renderpass_h_ */

/* END OF 'tam_vlk_renderpass.h' FILE */
