/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_sample_unit_MN7.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Unit sample implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 05.03.2026
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
    /* MN7 unit sample representation class */
    class unit_sample : public unit
    {
    private:
      shader TestShd {}; // Test shader
      material_pattern TestMtlPat {}; // Test material pattern
      material TestMaterial {}; // Test material
      prim TestPrim {}; // Test primitive
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
        /*std::vector<vertex> V
        {
          {
            {0, 0, 0.5}
          },
          {
            {0, 1, 0.5}
          },
          {
            {1, 1, 0.5}
          },
        };*/
        //TestPrim = Ani->primitive_manager::PrimitiveCreate(TestMaterial, V.data(), {vertex_type::eVEC3}, (UINT)V.size(), {});
        //TestMdl = Ani->ModelCreate("bin/models/G3DM/cow.g3dm");
        //TestMdl = Ani->ModelCreate("bin/models/G3DM/helic.g3dm");
        //TestMdl = Ani->ModelCreate("bin/models/GLTF/astro1.gltf");
        //TestMdl = Ani->ModelCreate("bin/models/GLTF/Strawberry.gltf");
        TestMdl = Ani->ModelCreate("bin/models/GLTF/spider/scene.gltf");
        //TestMdl = Ani->ModelCreate("bin/models/GLTF/astro1.gltf");

        vec2 p1 = vec2(0, 0), p2 = vec2(2, 0), q1 = vec2(2, 1), q2 = vec2(0, -1);
        vec2 p = vec2::SegmentsIntersection(p1, p2, q1, q2);
        FLT a = vec2::SegmentDistance(vec2(2, 0), vec2(0, 0), vec2(2, 2));
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
        //Ani->Draw(TestPrim, matr::Identity());
        TestMdl->DoAnim("Walk", Ani->Time);
        Ani->Draw(TestMdl, matr::Identity());//*matr::RotateY(180) **/matr::Scale(vec3(0.1)));// matr::Translate(vec3(15, 0, -10)));
      } /* End of 'Render' function */

      /* Test unit destructor */
      ~unit_sample( VOID )
      {
        //TestPrim->Free();
        TestMdl->Free();
        TestMaterial->Free();
        TestMtlPat->Free();
        TestShd->Free();
      } /* End of type destructor */
    };
    static tam::anim::unit_register<unit_sample> _("MN7 test unit");
  } /* end of anonymous namespace */
} /* end of 'tam' namespace */

/* END OF 'tam_sample_unit_MN7.cpp' FILE */
