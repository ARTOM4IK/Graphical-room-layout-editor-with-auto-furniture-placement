/***************************************************************
 * Copyright (C) 2024-2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_font.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Font class declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 24.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_res_font_h_
#define __tam_res_font_h_

#include "../primitives/tam_res_primitives.h"
#include "../material_patterns/tam_res_material_pattern.h"
#include "../materials/tam_res_material.h"

// #include "anim/render/render_resources/tam_rnd_resources.h"
// #include "anim/render/core/core_resources/buffer/tam_vlk_buffer.h"
// #include "anim/render/render_resources/material/tam_res_material.h"

/* Project namespace */
namespace tam
{
  // TAM_FORWARD(material, std::string)
  // TAM_FORWARD(prim, INT)

  class font_res;

  // Font interface type
  using font = std::shared_ptr<font_res>;


  /* Font resource representation type */
  class font_res : public resource
  {
    friend class font_manager;
    friend class resource_manager<font_res, std::string>;

  private:

    std::array<prim, 256> Glyphs {};        // Font characters primitives
    std::array<prim, 256> GlyphsScreen {};  // Font characters screen precision primitives

  public:

    SDWORD LineH {}, BaseH {};        // Font line height and base line height in pixels
    std::array<FLT, 256> AdvanceX {}; // Every letter shift right value (0 if no letter present)

    // Font reference name
    std::string Name;

    /* Class move constructor.
     * ARGUMENTS:
     *   - rvalue reference:
     *       font_res &&Old;
     */
    font_res( font_res &&Old );

    /* Class destructor */
    ~font_res( VOID );

  private:
    // Core primitive resource

    /* Class constructor.
     * ARGUMENTS:
     *   - pointer to render:
     *       render *Rnd;
     */
    font_res( render *Rnd );

    /* Create font function.
     * ARGUMENTS:
     *   - font file name:
     *       const std::string &FontFileName;
     *   - font letters draw color:
     *       const vec4 &TextColor;
     * RETURNS:
     *   (font_res &) self reference to font resource.
     */
    font_res & Create( const std::string &FontFileName, const vec4 &TextColor );

    /* Free primitive function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );

  public:

    /* Draw string in screen space function.
     * ARGUMENTS:
     *   - string to draw:
     *       const std::string &Text;
     *   - draw position:
     *       const vec3 &Pos;
     *   - font size (0 - for original size, otherwise - size in pixels):
     *       FLT Size;
     * RETURNS: None.
     */
    VOID Draw( const std::string &Text, const vec3 &Pos, FLT Size = 0 );

    /* Draw string in 3D space function.
     * ARGUMENTS:
     *   - string to draw:
     *       const std::string &Text;
     *   - draw position:
     *       const vec3 &Pos;
     *   - font size:
     *       FLT Size;
     * RETURNS: None.
     */
    VOID Draw3D( const std::string &Text, const vec3 &Pos, FLT Size = 0 );

    /* Draw string in billboard mode function.
     * ARGUMENTS:
     *   - string to draw:
     *       const std::string &Text;
     *   - draw position:
     *       const vec3 &Pos;
     *   - font size in space units:
     *       FLT Size;
     * RETURNS: None.
     */
    VOID DrawBB( const std::string &Text, const vec3 &Pos, FLT Size = 0 );

    /* Draw static string in screen space function.
     * ARGUMENTS:
     *   - string to draw:
     *       const std::string &Text;
     *   - draw position:
     *       const vec3 &Pos;
     *   - font size (0 - for original size, otherwise - size in pixels):
     *       FLT Size;
     * RETURNS: None.
     */
    VOID DrawStatic( const std::string &Text, const vec3 &Pos, FLT Size = 0 );

    /* Draw static string in 3D space function.
     * ARGUMENTS:
     *   - string to draw:
     *       const std::string &Text;
     *   - draw position:
     *       const vec3 &Pos;
     *   - font size:
     *       FLT Size;
     * RETURNS: None.
     */
    VOID DrawStatic3D( const std::string &Text, const vec3 &Pos, FLT Size = 0 );

    /* Draw static string in billboard mode function.
     * ARGUMENTS:
     *   - string to draw:
     *       const std::string &Text;
     *   - draw position:
     *       const vec3 &Pos;
     *   - font size in space units:
     *       FLT Size;
     * RETURNS: None.
     */
    VOID DrawStaticBB( const std::string &Text, const vec3 &Pos, FLT Size = 0 );
  }; /* End of 'font' class */

  /* Define strong and weak user types */
  // TAM_FORWARD(font, std::string)

  /* Fonts manager representation type */
  class font_manager : public resource_manager<font_res, std::string>
  {
    friend class font_res;
  protected:
    // First initialization flag
    BOOL IsFirstCreation = TRUE;

    // Font material patterns
    material_pattern MtlPatScreen {}, MtlPatDirect {};

    /* Free all fonts function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID FreeFonts( VOID );

  public:
    /* Class constructor.
     * ARGUMENTS:
     *   - render reference:
     *       render &RndPtr;
     */
    font_manager( render &RndRef );

    /* Class destructor */
    ~font_manager( VOID )
    {
      logger::info << "Fonts deinitialization";
    } /* End of '~font_manager' function */

    /* Create font function.
     * ARGUMENTS:
     *   - font file name:
     *       const std::string &FontFileName;
     *   - font letters draw color:
     *       const vec4 &TextColor;
     * RETURNS:
     *   (font) self reference.
     */
    font FontCreate( const std::string &FontFileName, const vec4 &TextColor = vec4(1, 1, 1, 1) );
  }; /* End of 'font_manager' class */
} /* end of 'tam' namespace */

#endif /* __tam_res_font_h_ */

/* END OF 'tam_res_font.h' FILE */

