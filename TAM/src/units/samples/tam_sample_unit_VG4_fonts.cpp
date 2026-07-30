/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_sample_unit_VG4.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Font output test unit implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 21.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Project namespace */
namespace tam
{
  /* Units namespace */
  namespace units
  {
    /* Drawing test unit class */
    class font_sample : public unit
    {
    private:
      font Fnt, Fnt1, Fnt2; // Work fonts

    public:

      /* Class constructor.
       * ARGUMENTS:
       *   - pointer to anim:
       *       anim *Ani;
       */
      font_sample( anim *Ani )
      {
        Fnt = Ani->FontCreate("book", vec4(1, 1, 0, 0.80));
        Fnt1 = Ani->FontCreate("consolas", vec4(1, 1, 1, 1));
        Fnt2 = Ani->FontCreate("consolas", vec4(0, 1, 1, 0.8));
      } /* End of 'font_sample' function */

      /* Default class destructor */
      ~font_sample( VOID ) override
      {
      } /* End of '~font_sample' function */

      /* Render unit function.
       * ARGUMENTS:
       *   - pointer to anim:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Render( anim *Ani ) override
      {
        auto str =
          "0123456789012345678\n"
          "abc\tdef\trr\n"
          "q\trrree\td\n"
          "a\tb\tc\tee\td\n"
          "a\rb\rc\r\n"
          R"ops(Lorem ipsum dolor sit amet,
consectetur adipiscing elit. Aliquam lectus risus,
finibus ornare vestibulum et, feugiat quis dui. Vivamus
sit amet dolor et magna facilisis rhoncus. Curabitur
maximus est sed porta scelerisque. Sed suscipit, arcu
volutpat feugiat posuere, eros nisi tristique nibh,
mollis vehicula lectus tortor eu purus. Donec ut
tortor blandit, sagittis diam eget, suscipit eros.
Quisque at magna neque. Nulla faucibus mi at nunc
mattis placerat. Pellentesque quis augue quis elit
tristique auctor. Integer varius est orci, vel egestas
felis dictum nec. Phasellus porta ex sit amet turpis
finibus, sed vestibulum nisl efficitur. Praesent ultrices
diam enim. In ut tellus sed sem placerat sollicitudin.
Donec quis mollis dolor. Etiam viverra, arcu cursus
porttitor porttitor, diam nunc auctor nisl, quis
placerat magna erat et odio. Phasellus feugiat
turpis quis mollis lacinia. Sed ac orci et nisi
venenatis pharetra ac non arcu.)ops";
        Fnt->Draw3D(str, vec3(-8, 0, 0), 0.8);
        Fnt1->Draw(str + std::format("\nTAM System\nCGSG Seniors 2025 Project\nFPS:\t{:.5f}\nTime:\t{:.2f}", Ani->FPS, Ani->GlobalTime), vec3(300, 0, 0), Fnt1->LineH / 2);
        Fnt->Draw(str + std::format("\nTAM System\nCGSG Seniors 2025 Project\nFPS:\t{:.5f}\nTime:\t{:.2f}", Ani->FPS, Ani->GlobalTime), vec3(0, 0, 0), Fnt->LineH / 2);
      
        vec3 p = matr::RotateY(Ani->Time * 30).TransformPoint(vec3(8, 0, 0));
        //Ani->DrawSphere(p, vec3(0.1), vec4(0, 1, 1, 1));
        Fnt2->DrawBB(std::format("TAM System\nCGSG Seniors 2025 Project\nFPS: {:.5f}", Ani->FPS), p + vec3(0, 0, 0), 0.8);
      } /* End of 'Render' function */

      /* Response unit function.
       * ARGUMENTS:
       *   - pointer to anim:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
        /// shader Shd = Ani->ShaderCreate("default");
        /// material_pattern Pat = Ani->MaterialPatternCreate(Shd, "Test pattern", 0, 0, {}, {},
        /// {
        ///   vlk::vertex_attribute::eVEC3,
        ///   vlk::vertex_attribute::eVEC2,
        ///   vlk::vertex_attribute::eVEC3,
        ///   vlk::vertex_attribute::eVEC4,
        /// }, vlk::topology::TRIMESH, FALSE);
        /// material Mtl = Ani->MaterialCreate("Test material", Pat, {}, {});
        /// std::vector<vertex_std> vs =
        /// {
        ///   {vec3(0), vec2(0), vec3(0), vec4(1)},
        ///   {vec3(1, 0, 0), vec2(0), vec3(0), vec4(1)},
        ///   {vec3(0, 1, 0), vec2(0), vec3(0), vec4(1)},
        /// };
        /// prim Pr = Ani->PrimitiveCreate(Mtl,
        ///   std::span<vertex_std>(vs), {});
      } /* End of 'Response' function */
    }; /* End of 'font_sample' class */
  } /* end of 'units' namespace */
  static anim::unit_register<units::font_sample> _("FontTest");
} /* end of 'tam' namespace */

/* END OF 'tam_sample_unit_VG4.cpp' FILE */
