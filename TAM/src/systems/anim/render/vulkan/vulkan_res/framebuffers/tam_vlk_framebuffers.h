/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

 /* FILE NAME   : tam_vlk_framebuffers.h
  * PURPOSE     : Tough Arrangement Manager project.
  *               Frame buffer declaration file.
  * PROGRAMMER  : CGSG'Sr'2025.
  *               Arsentev Artemy (AA4).
  * LAST UPDATE : 10.03.2026
  * NOTE        : Namespace 'tam'.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __tam_vlk_framebuffers_h_
#define __tam_vlk_framebuffers_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* 'vlk' namespace */
  namespace vlk
  {
    /* 'framebuffer' handle class */
    class framebuffer
    {
    private:
      core* VlkPtr; // Vulkan global handler pointer

    public:
      VkFramebuffer FrameBuffer;

      /* Class default constructor */
      framebuffer( core* Ptr );

      /* Vulkan frame buffer creation function.
       * ARGUMENTS:
       *   - Render pass:
       *       VkRenderPass &RenderPass;
       *   - Frame buffer size:
       *       INT &W, INT &H;
       *   - Image views of Attachments:
       *       std::vector<VkImageView> &Attachments;
       * RETURNS:
       *   (BOOL) success creation result.
       */
      BOOL Create( VkRenderPass RenderPass, INT W, INT H, std::vector<VkImageView> &Attachments );

      /* Vulkan destroy frame buffer function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Destroy( VOID );
    }; /* End of 'framebuffer' class */
  } /* end of 'vlk' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_vlk_framebuffers_h_ */

/* END OF 'tam_vlk_framebuffers.h' FILE */
