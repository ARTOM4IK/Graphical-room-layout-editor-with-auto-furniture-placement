/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_sample_unit_IK4.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               !!! implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3),
 *               Arsentev Artemy (AA4),
 *               Nechaev Vladimir (VN4),
 *               Kupchenkov Ivan (IK4),
 *               Novozhilova Maryana (MN7),
 *               Tretyakov Andrey (AT7),
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 20.01.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* 'tam' namespace */
namespace tam
{
  /* Anonymous namespace */
  namespace
  {
    /* IK4 unit sample representation class */
    class unit_IK4 : public unit
    {
    private:
      texture TestTex;             // Test texture
      material_pattern TestMtlPat; // Test material pattern
      material TestMtl;            // Test material
      prim TestPrim;               // Test primitive
      texture TestTex1;             // Test texture
      material TestMtl1;            // Test material
      prim TestPrim1;               // Test primitive
    public:

      /* Class constructor.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       */
      unit_IK4( anim *Ani )
      {
        logger::info << "IK4 unit initialization.";
        TestTex = Ani->TextureCreate("bin/textures/water.png");
        shader Shd_T = Ani->ShaderCreate("samples/test_IK4");
        TestMtlPat = Ani->MaterialPatternCreate(Shd_T, "Default test material pattern", 1, 0, {}, {});
        TestMtl = Ani->MaterialCreate("test material", TestMtlPat, {TestTex});
        TestTex1 = Ani->TextureCreate("bin/textures/ice.png");
        TestMtl1 = Ani->MaterialCreate("material1", TestMtlPat, {TestTex1});

        std::vector<vertex_std> Vertices 
        {
          vertex_std
          {
            vec3(0, 0, 0.5),
            vec2(0, 0),
            vec3(0, 0, -1),
            vec4(1, 0, 0, 1)
          },
          vertex_std
          {
            vec3(0, 1, 0.5),
            vec2(0, 1),
            vec3(0, 0, -1),
            vec4(1, 1, 0, 1)
          },
          vertex_std
          {
            vec3(1, 1, 0.5),
            vec2(1, 1),
            vec3(0, 0, -1),
            vec4(0, 0, 1, 1)
          }
        };
        TestPrim = Ani->PrimitiveCreate(TestMtl, std::span(Vertices.data(), Vertices.size()), {});
        TestPrim1 = Ani->PrimitiveCreate(TestMtl1, std::span(Vertices.data(), Vertices.size()), {});
      } /* End of 'unit_IK4' function */

      /* Get response from unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
      } /* End of 'Response' function */

      /* Render unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Render( anim *Ani ) override
      {
        for (INT i = -18; i < 18; i++)
          for (INT j = -18; j < 18; j++)
            Ani->Draw(TestPrim, matr::Scale(vec3(8)) * matr::Translate(i, i * j, j) * matr::RotateX(Ani->Time * (i - j)) * matr::RotateY(sin(Ani->Time) * (j + i)) );
        for (INT i = -18; i < 18; i++)
          for (INT j = -18; j < 18; j++)
            Ani->Draw(TestPrim1, matr::Scale(vec3(3.0)) * matr::Translate(j * i, i, i * j) * matr::RotateX(Ani->Time * (j - i)) * matr::RotateY(sin(Ani->Time) * (-j - i)) );
      } /* End of 'Render' function */

      /* Test unit destructor */
      ~unit_IK4( VOID )
      {
        logger::info << "IK4 unit deinitialization.";
      } /* End of type destructor */
    };
    static tam::anim::unit_register<unit_IK4> _("IK4 unit");
  } /* end of anonymous namespace */
} /* end of 'tam' namespace */

/* END OF 'tam_sample_unit_IK4.cpp' FILE */
