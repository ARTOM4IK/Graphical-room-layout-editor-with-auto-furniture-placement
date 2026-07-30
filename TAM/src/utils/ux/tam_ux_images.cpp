/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_images.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX image cache implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 12.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux_images.h"

/* Acquire image by file path (increment ref count) function.
 * ARGUMENTS:
 *   - image file path:
 *       const std::string &Path;
 * RETURNS:
 *   (VOID *) texture descriptor (ImTextureID).
 */
VOID * tam::ux::image_cache::Acquire( const std::string &Path )
{
  auto it = Cache.find(Path);
  if (it != Cache.end())
  {
    it->second.RefCount++;
    return it->second.TexID;
  }

  if (!LoadFn)
    return nullptr;

  auto [tex_id, resource] = LoadFn(Path);
  if (tex_id == nullptr)
    return nullptr;

  entry e;
  e.TexID = tex_id;
  e.Resource = std::move(resource);
  e.RefCount = 1;
  Cache[Path] = std::move(e);
  return tex_id;
} /* End of 'tam::ux::image_cache::Acquire' function */

/* Acquire icon by name from icon base directory function.
 * ARGUMENTS:
 *   - icon file name (e.g. "settings.ico"):
 *       const std::string &Name;
 * RETURNS:
 *   (VOID *) texture descriptor (ImTextureID).
 */
VOID * tam::ux::image_cache::AcquireIcon( const std::string &Name )
{
  return Acquire(IconBasePath + Name);
} /* End of 'tam::ux::image_cache::AcquireIcon' function */

/* Release image by file path (decrement ref count) function.
 * ARGUMENTS:
 *   - image file path:
 *       const std::string &Path;
 * RETURNS: None.
 */
VOID tam::ux::image_cache::Release( const std::string &Path )
{
  auto it = Cache.find(Path);
  if (it == Cache.end())
    return;

  it->second.RefCount--;
  if (it->second.RefCount <= 0)
  {
    if (UnloadFn && it->second.TexID != nullptr)
      UnloadFn(it->second.TexID);
    Cache.erase(it);
  }
} /* End of 'tam::ux::image_cache::Release' function */

/* Release icon by name function.
 * ARGUMENTS:
 *   - icon file name:
 *       const std::string &Name;
 * RETURNS: None.
 */
VOID tam::ux::image_cache::ReleaseIcon( const std::string &Name )
{
  Release(IconBasePath + Name);
} /* End of 'tam::ux::image_cache::ReleaseIcon' function */

/* Get image descriptor without changing ref count function.
 * ARGUMENTS:
 *   - image file path:
 *       const std::string &Path;
 * RETURNS:
 *   (VOID *) texture descriptor or nullptr.
 */
VOID * tam::ux::image_cache::Get( const std::string &Path ) const
{
  auto it = Cache.find(Path);
  if (it != Cache.end())
    return it->second.TexID;
  return nullptr;
} /* End of 'tam::ux::image_cache::Get' function */

/* Get icon descriptor without changing ref count function.
 * ARGUMENTS:
 *   - icon file name:
 *       const std::string &Name;
 * RETURNS:
 *   (VOID *) texture descriptor or nullptr.
 */
VOID * tam::ux::image_cache::GetIcon( const std::string &Name ) const
{
  return Get(IconBasePath + Name);
} /* End of 'tam::ux::image_cache::GetIcon' function */

/* Remove all entries with zero ref count function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::image_cache::Cleanup( VOID )
{
  for (auto it = Cache.begin(); it != Cache.end(); )
  {
    if (it->second.RefCount <= 0)
    {
      if (UnloadFn && it->second.TexID != nullptr)
        UnloadFn(it->second.TexID);
      it = Cache.erase(it);
    }
    else
      ++it;
  }
} /* End of 'tam::ux::image_cache::Cleanup' function */

/* Unload and remove all cached entries function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::image_cache::Clear( VOID )
{
  for (auto &[path, e] : Cache)
    if (UnloadFn && e.TexID != nullptr)
      UnloadFn(e.TexID);
  Cache.clear();
} /* End of 'tam::ux::image_cache::Clear' function */

/* END OF 'tam_ux_images.cpp' FILE */
