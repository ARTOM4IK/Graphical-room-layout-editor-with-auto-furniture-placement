/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_textures.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Textures structure declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 22.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_res_textures_h_
#define __tam_res_textures_h_

#include "..\tam_res.h"

/* 'tam' namespace */
namespace tam
{
  /* 'texture' handle class */
  class texture_res : public resource
  {
  public:
    std::string Name;     // Texture name
    INT W = 0, H = 0;     // Texture size
    vlk::image Img;       // Vulkan image
    vlk::sampler Sampler; // Vulkan image samler
    std::vector<UINT8> Data; // B8G8R8A8

    /* Class default constructor */
    // texture_res( VOID ) : Img(nullptr), Sampler(nullptr)
    // {
    // } /* End of 'texture' function */

    /* Class constructor */
    texture_res( render *RndPtr );

    /* Class move constructor */
    texture_res( texture_res &&Old );

    /* Class destructor */
    ~texture_res( VOID )
    {
      Free();
    } /* End of '~texture' function */

    /* Create texture function.
     * ARGUMENTS:
     *   - texture name:
     *      const std::string &NewName;
     *   - texture size:
     *      INT NewW, INT NewH;
     *   - pixels:
     *      INT BytesPerPixel, BYTE *Pixels;
     *   - color/depth flag:
     *      BOOL IsColor;
     *   - is mips flag:
     *      BOOL IsMips;
     *   - use srgb filter flag:
     *      BOOL Is Filtered;
     *   - is attachment flag:
     *      BOOL IsAttachment;
     * RETURNS:
     *   (texture_res &) texture.
     */
    texture_res & Create( const std::string &NewName, INT NewW, INT NewH, INT BytesPerPixel, BYTE *Pixels, BOOL IsColor = TRUE, BOOL IsMips = TRUE, BOOL IsFiltered = TRUE, BOOL IsAttachment = FALSE );

    /* Create texture function.
     * ARGUMENTS:
     *   - texture name:
     *      const std::string &FileName;
     * RETURNS:
     *   (texture &) texture.
     */
    texture_res & Create( const std::string &FileName );

    /* Create 1x1 texture function.
     * ARGUMENTS:
     *   - color vector:
     *      vec4 Color;
     * RETURNS:
     *   (texture &) texture.
     */
    texture_res & Create( vec4 Color );

    /* Create cubemap texture function.
     * ARGUMENTS:
     *   - texture name:
     *      const std::string &TextureName;
     * RETURNS:
     *   (texture &) cubemap texture.
     */
    texture_res & CreateCubeMap( const std::string &TextureName );

    /* Create texture function.
     * ARGUMENTS:
     *   - texture name:
     *      const std::string &NewName;
     *   - vulkan image reference:
     *      image Image;
     * RETURNS:
     *   (texture &) texture.
     */
    texture_res & Create( const std::string &NewName, vlk::image &Image );

    /* Free texture function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );

    /* Rewrite image in texture without deleting function.
     *   - New vulkan image:
     *      const vlk::image &NewImage;
     * RETURNS: None.
     */
    VOID RewriteImage( const vlk::image &NewImage );

    /* Get vec4 color from texture function.
     * ARGUMENTS:
     *   - Point position:
     *      const vec2 &Pos;
     * RETURNS:
     *   (vec4) color.
     */
    vec4 GetColor( const vec2 &Position );
  }; /* End of 'texture' class */

  /* texture resource user type definition */
  using texture = std::shared_ptr<texture_res>;

  /* 'texture_manager' handle class */
  class texture_manager : public resource_manager<texture_res, std::string>
  {
  public:
    /* Class default constructor */
    texture_manager( render &RndRef ) : resource_manager(RndRef)
    {
    } /* End of 'texture_manager' function */

    /* Class destructor */
    ~texture_manager( VOID )
    {
      logger::info << "Textures deinitialization";
    } /* End of '~texture_manager' function */

    /* Create texture function.
     * ARGUMENTS:
     *   - texture name:
     *      const std::string &NewName;
     *   - flag to check if the texture should be filtered:
     *      BOOL IsFiltered;
     * RETURNS:
     *   (texture *) pointer to texture.
     */
    texture TextureCreate( const std::string &FileName, BOOL IsFiltered = TRUE );

    /* Create texture cubemap function.
     * ARGUMENTS:
     *   - texture name:
     *      const std::string &NewName;
     * RETURNS:
     *   (texture *) pointer to texture.
     */
    texture TextureCreateCubeMap( const std::string &FileName );

    /* Create texture function.
     * ARGUMENTS:
     *   - texture name:
     *      const std::string &NewName;
     *   - texture size:
     *      INT NewW, INT NewH;
     *   - pixels:
     *      INT BytesPerPixel, BYTE *Pixels;
     *   - color/depth flag:
     *      BOOL IsColor;
     *   - is mips flag:
     *      BOOL IsMips;
     *   - use srgb filter flag:
     *      BOOL Is Filtered;
     *   - is attachment flag:
     *      BOOL IsAttachment;
     * RETURNS:
     *   (texture *) pointer to texture.
     */
    texture TextureCreate( const std::string &NewName, INT NewW, INT NewH, INT BytesPerPixel, BYTE *Pixels, BOOL IsColor = TRUE, BOOL IsMips = TRUE, BOOL IsFiltered = TRUE, BOOL IsAttachment = FALSE );

    /* Create 1x1 texture function.
     * ARGUMENTS:
     *   - color vector:
     *      vec4 &Color;
     * RETURNS:
     *   (texture *) pointer to texture.
     */
    texture TextureCreate( vec4 Color );

    /* Create texture function.
     * ARGUMENTS:
     *   - texture name:
     *      const std::string &NewName;
     *   - vulkan image reference:
     *      image Image;
     * RETURNS:
     *   (texture *) pointer to texture.
     */
    texture TextureCreate( const std::string &NewName, vlk::image &Image );

    /* Delete user texture resource function.
     * ARGUMENTS:
     *   - name of the texture:
     *       const std::string &Name;
     * RETURNS:
     *   (texture) user resource type.
     */
    VOID TextureDelete( texture &Entry )
    {
      Delete(Entry);
    } /* End of 'TexDelete' function */
  }; /* End of 'texture_manager' class */
} /* end of 'tam' namespace */

#endif /* __tam_res_textures_h_ */

/* END OF 'tam_res_textures.h' FILE */
