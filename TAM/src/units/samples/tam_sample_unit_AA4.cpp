/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_sample_unit_AA4.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               AA4 test unit implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
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
    /* AA4 unit sample representation class */
    class unit_sample : public unit
    {
    private:
      shader TestShd {}; // Test shader
      material_pattern TestMtlPat {}; // Test material pattern
      material TestMaterial {}; // Test material
      prim TestPrim[1000] {}; // Test primitive
      model TestMdl {}; // Test model
    public:

      /* Class constructor.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       */
      unit_sample( anim *Ani )
      {
        TestShd = Ani->shader_manager::ShaderCreate("default");
        TestMtlPat = Ani->material_pattern_manager::MaterialPatternCreate(TestShd, "Test material pattern", 0, 0, {}, {},
          {vlk::vertex_attribute::eVEC3, vlk::vertex_attribute::eVEC2, vlk::vertex_attribute::eVEC3,
          vlk::vertex_attribute::eVEC4}, vlk::topology::TRIMESH);
        TestMaterial = Ani->material_manager::MaterialCreate("Test material", TestMtlPat);
        TestMdl = Ani->ModelCreate("bin/models/GLTF/spider/scene.gltf");
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
        for (INT i = 0; i < 1000; i++)
          TestPrim[i] = Ani->PrimitiveCreate(TestMaterial, std::span(Vertices.data(), Vertices.size()), {});
      } /* End of 'unit_sample' function */

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
        for (INT i = 0; i < 1000; i++)
        {
          Ani->Draw(TestPrim[i], matr::Identity());//*matr::RotateY(180) **/matr::Scale(vec3(0.1)));// matr::Translate(vec3(15, 0, -10)));
        }
        SystemProfiler.Collect("Begin CPU Reaclc Matrixes");

        for (INT i = 0; i < 1000; i++)
        {
          for (auto &mat : TestPrim[i]->WorldMatrixes)
          {
            matr MatInv = (matr(mat.M)).Inverse();
            matr WVP = matr(mat.M) * Ani->Cam.VP;
          }
          
        }
        SystemProfiler.Collect("End CPU Reaclc Matrixes");
      } /* End of 'Render' function */

      /* Test unit destructor */
      ~unit_sample( VOID )
      {
        TestMaterial->Free();
        TestMtlPat->Free();
        TestShd->Free();
      } /* End of type destructor */
    };
    static tam::anim::unit_register<unit_sample> _("TestMatr");
  } /* end of anonymous namespace */
} /* end of 'tam' namespace */

/* END OF 'tam_sample_unit_AA4.cpp' FILE */
