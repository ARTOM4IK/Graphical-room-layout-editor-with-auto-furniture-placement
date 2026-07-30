/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_sample_unit_AA4.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Editor point unit implementation module.
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
    public:
      mth::ray<FLT> R; // Frame ray
      prim Plane;      // Plane primitive
      INT PlaneSize = 100;
      shader TestShd {}; // Test shader
      material_pattern TestMtlPat {}; // Test material pattern
      material TestMaterial {}; // Test material
      std::vector<vec3> MarkersPositions {};

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
        std::vector<vertex_std> vertices
        {
          {
            vec3(0, 0, 0),
            vec2(0, 0),
            vec3(0, 1, 0),
            vec4(0.3, 0, 0, 1)
          },
          {
            vec3(100, 0, 0),
            vec2(1, 0),
            vec3(0, 1, 0),
            vec4(0.3, 0, 0, 1)
          },
          {
            vec3(100, 0, 100),
            vec2(1, 1),
            vec3(0, 1, 0),
            vec4(0.3, 0, 0, 1)
          },
          {
            vec3(0, 0, 100),
            vec2(0, 1),
            vec3(0, 1, 0),
            vec4(0.3, 0, 0, 1)
          },
        };
        std::vector<INT> indices = {0, 1, 2, 0, 2, 3};
        Plane = Ani->PrimitiveCreate(TestMaterial, std::span(vertices), std::span(indices));
      } /* End of 'unit_sample' function */

      /* Get response from unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
        if (Ani->Keys['R'] && Ani->Keys[VK_LBUTTON])
        {
          R = Ani->Cam.FrameRay(Ani->Mx, Ani->My);
          DBL t = R.IntersectPlane();
          MarkersPositions.push_back(R(t));
        }
      } /* End of 'Response' function */

      /* Render unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Render( anim *Ani ) override
      {
        Ani->Draw(Plane, matr::Identity());
        for (auto &pos : MarkersPositions)
          Ani->DrawSphere(pos, 0.3, vec4(0.3, 0.4, 1, 1));
      } /* End of 'Render' function */

      /* Test unit destructor */
      ~unit_sample( VOID )
      {
      } /* End of type destructor */
    };
    static tam::anim::unit_register<unit_sample> _("Editor point unit");
  } /* end of anonymous namespace */
} /* end of 'tam' namespace */

/* END OF 'tam_editor_unit_point.cpp' FILE */
