/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_images.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX image cache declaration file.
 *               Ref-counted image/icon storage with automatic
 *               load and unload via host-provided callbacks.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 12.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ux_images_h_
#define __tam_ux_images_h_

#include "tam_def.h"

#include <any>

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* Image/icon cache class with reference counting.
     * Loads images through host-provided callback, keeps
     * them alive while at least one user holds a reference.
     * Icons are loaded from IconBasePath + Name.
     */
    class image_cache
    {
    private:
      /* Single cached image entry type */
      struct entry
      {
        VOID *TexID {};         // Opaque texture descriptor (ImTextureID)
        std::any Resource {};   // Opaque resource handle keeping texture alive
        INT RefCount {};        // Active reference count
      }; /* End of 'entry' structure */

      std::map<std::string, entry> Cache {};  // Path -> cached entry

    public:
      std::string IconBasePath = "bin/ux/icon/"; // Default icon directory

      /* Host-provided image load callback.
       * Takes file path, returns {texture descriptor, resource handle}.
       */
      std::function<std::pair<VOID *, std::any>( const std::string & )> LoadFn {};

      /* Host-provided image unload callback.
       * Takes texture descriptor, releases GPU resources.
       */
      std::function<VOID(VOID *)> UnloadFn {};

      /* Acquire image by file path (increment ref count) function.
       * ARGUMENTS:
       *   - image file path:
       *       const std::string &Path;
       * RETURNS:
       *   (VOID *) texture descriptor (ImTextureID).
       */
      VOID * Acquire( const std::string &Path );

      /* Acquire icon by name from icon base directory function.
       * ARGUMENTS:
       *   - icon file name (e.g. "settings.ico"):
       *       const std::string &Name;
       * RETURNS:
       *   (VOID *) texture descriptor (ImTextureID).
       */
      VOID * AcquireIcon( const std::string &Name );

      /* Release image by file path (decrement ref count) function.
       * ARGUMENTS:
       *   - image file path:
       *       const std::string &Path;
       * RETURNS: None.
       */
      VOID Release( const std::string &Path );

      /* Release icon by name function.
       * ARGUMENTS:
       *   - icon file name:
       *       const std::string &Name;
       * RETURNS: None.
       */
      VOID ReleaseIcon( const std::string &Name );

      /* Get image descriptor without changing ref count function.
       * ARGUMENTS:
       *   - image file path:
       *       const std::string &Path;
       * RETURNS:
       *   (VOID *) texture descriptor or nullptr.
       */
      VOID * Get( const std::string &Path ) const;

      /* Get icon descriptor without changing ref count function.
       * ARGUMENTS:
       *   - icon file name:
       *       const std::string &Name;
       * RETURNS:
       *   (VOID *) texture descriptor or nullptr.
       */
      VOID * GetIcon( const std::string &Name ) const;

      /* Remove all entries with zero ref count function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Cleanup( VOID );

      /* Unload and remove all cached entries function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Clear( VOID );
    }; /* End of 'image_cache' class */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_ux_images_h_ */

/* END OF 'tam_ux_images.h' FILE */
