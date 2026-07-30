/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_sample_unit_AT7.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               AT7 test unit implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 14.03.2026
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
    /* AT7 unit sample representation class */
    class unit_sample : public unit
    {
    private:
      static const INT TestNo = 100;         // Number of test instances
      texture TestTex;                       // Test texture
      texture TestTextures[TestNo];          // Test texture
      material_pattern TestMtlPat;           // Test material pattern
      material TestMtl;                      // Test material
      prim TestPrim;                         // Test primitive
      material_pattern TestPatterns[TestNo]; // Test material patterns array
      material TestMaterials[TestNo];        // Test materials array
      prim TestPrims[TestNo];                // Test primitives array
      model TestModel;                       // Test model
      model Test1;                           // Test model 1
      model Test2;                           // Test model 2
    public:

      /* Class constructor.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       */
      unit_sample( anim *Ani )
      {
        logger::info << "AT7 unit sample initialization.";

        TestTex = Ani->TextureCreate("bin/textures/giraffe.jpg");
        TestMtlPat = Ani->MaterialPatternCreate(Ani->ShaderCreate("samples/test_AT7"), "Default test material pattern", 1, 0, {}, {});
        TestMtl = Ani->MaterialCreate("Test material", TestMtlPat, {TestTex});

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

        for (INT i = 0; i < TestNo; i++)
        {
          TestPatterns[i] = Ani->MaterialPatternCreate(Ani->ShaderCreate("samples/test_AT7"), std::format("Test material pattern # {}", i), 1, 0, {}, {});
          TestMaterials[i] = Ani->MaterialCreate(std::format("Test material # {}", i), TestPatterns[i], {TestTex});
          TestPrims[i] = Ani->PrimitiveCreate(TestMaterials[i], std::span(Vertices.data(), Vertices.size()), {});
        }
        TestModel = Ani->ModelCreate("bin/models/GLTF/spider/scene.gltf", matr::Scale(vec3(10)));
        Test1 = Ani->ModelCreate("bin/models/GLTF/spider/scene.gltf", matr::Scale(vec3(1)));
        Test2 = Ani->ModelCreate("bin/models/GLTF/spider/scene.gltf", matr::Scale(vec3(0.1)));
      } /* End of 'unit_sample' function */

      /* Get response from unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
        /*for (auto &Tex: TestTextures)
          Tex = Ani->TextureCreate("bin/textures/water1.png");
        for (auto &Tex : TestTextures)
          Tex = nullptr;*/
      } /* End of 'Response' function */

      /* Render unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Render( anim *Ani ) override
      {
        srand(30);
        for (INT i = 0; i < 1000; i++)
          Ani->Draw(TestPrim, matr::Translate(vec3::Rnd1() * 10));
        for (INT i = 0; i < TestNo; i++)
          Ani->Draw(TestPrims[i], matr::Translate(vec3(0, 20, 0) + vec3::Rnd1() * 10));
        TestModel->DoAnim("Walk", Ani->Time);
        Ani->Draw(TestModel, matr::Identity());
        Ani->Draw(Test1, matr::Translate(vec3(30, 10, 10)));
        Ani->Draw(Test2, matr::Translate(vec3(-30, 10, -10)));
      } /* End of 'Render' function */

      /* Test unit destructor */
      ~unit_sample( VOID )
      {
        logger::info << "AT7 unit deinitialization.";
      } /* End of type destructor */
    };
    static tam::anim::unit_register<unit_sample> _("AT7 test unit");
  } /* end of anonymous namespace */
} /* end of 'tam' namespace */

/* END OF 'tam_sample_unit_AT7.cpp' FILE */
