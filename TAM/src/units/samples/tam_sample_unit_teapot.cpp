/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_sample_unit_teapot.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Utah teapot, gold material, tessellation (Bezier), light (1,1,1).
 * PROGRAMMER  : CGSG'Sr'2025.
 * LAST UPDATE : 26.02.2026
 * NOTE        : Namespace 'tam'. Unit name: "Teapot". Box 1x1, cell (1,0,0).
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_teapot_data.h"

/* 'tam' namespace */
namespace tam
{
  /* Anonymous namespace */
  namespace
  {
    /* Light soure material pass data */
    struct BUF_DIRLIGHT_MTL
    {
      vec4 LightDir;    // Normalized light source direction
      vec4 LightColor;  // Light source color
      vec4 CamPos;      // Camera position
      vec4 Ka, Kd, Ks, Ph; // Phong illumination coefficients
    }; /* End of 'BUF_DIRLIGHT_MTL' structure */

    /* Teapot unit representation class */
    class unit_teapot : public unit
    {
      prim TeapotPrim;
      material Mtl;
      buffer LightMtlBuf;
    public:
      /* Class constructor.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       */
      unit_teapot( anim *Ani )
      {
        logger::info << "Teapot unit init.";
        BUF_DIRLIGHT_MTL LightMtl = {};
        vec3 LightDir = vec3(1, 1, 1).Normalizing();
        LightMtl.LightDir = vec4(LightDir.X, LightDir.Y, LightDir.Z, 0);
        LightMtl.LightColor = vec4(1, 1, 1, 1);
        LightMtl.CamPos = vec4(0, 0, 0, 1);
        LightMtl.Ka = vec4(0.24725f, 0.1995f, 0.0745f, 1);
        LightMtl.Kd = vec4(0.75164f, 0.60648f, 0.22648f, 1);
        LightMtl.Ks = vec4(0.628281f, 0.555802f, 0.366065f, 1);
        LightMtl.Ph = vec4(51.2f, 0, 0, 0);
        LightMtlBuf = Ani->buffer_manager::BufferCreate<BUF_DIRLIGHT_MTL>(&LightMtl,
          VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        shader Shd = Ani->ShaderCreate("samples/teapot");
        material_pattern Pat =
          Ani->MaterialPatternCreate(Shd, "Teapot pat", 0, 1, {}, {},
            {
              vlk::vertex_attribute::eVEC3,
              vlk::vertex_attribute::eVEC2,
              vlk::vertex_attribute::eVEC3,
              vlk::vertex_attribute::eVEC4
            }, vlk::topology::PATCH, vlk::blend_mode::eOPAQUE, 16);
        Mtl = Ani->MaterialCreate("Teapot gold", Pat, {}, {LightMtlBuf});
        std::vector<vertex_std> V(teapot_data::NumVertices);
        for (INT i = 0; i < teapot_data::NumVertices; i++)
          V[i] = vertex_std(teapot_data::Vertices[i], vec2(0, 0), vec3(0, 1, 0), vec4(1, 1, 1, 1));
        std::vector<INT> Ind;
        Ind.reserve(teapot_data::NumPatches * teapot_data::IndicesPerPatch);
        for (INT p = 0; p < teapot_data::NumPatches; p++)
          for (INT i = 0; i < 4; i++)
            for (INT j = 0; j < 4; j++)
              Ind.push_back(teapot_data::Indices[p][i][j] - 1);
        TeapotPrim = Ani->PrimitiveCreate(Mtl, std::span(V.data(), V.size()), std::span(Ind.data(), Ind.size()));
        logger::info << "Teapot: 32 patches, 306 ctrl pts.";
      } /* End of 'unit_teapot' function */

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
        if (TeapotPrim == nullptr) return;
        BUF_DIRLIGHT_MTL LightMtl = {};
        vec3 LightDir = vec3(1, 1, 1).Normalizing();
        LightMtl.LightDir = vec4(LightDir.X, LightDir.Y, LightDir.Z, 0);
        LightMtl.LightColor = vec4(1, 1, 1, 1);
        LightMtl.CamPos = vec4(Ani->Cam.Loc.X, Ani->Cam.Loc.Y, Ani->Cam.Loc.Z, 1);
        LightMtl.Ka = vec4(0.24725f, 0.1995f, 0.0745f, 1);
        LightMtl.Kd = vec4(0.75164f, 0.60648f, 0.22648f, 1);
        LightMtl.Ks = vec4(0.628281f, 0.555802f, 0.366065f, 1);
        LightMtl.Ph = vec4(51.2f, 0, 0, 0);
        LightMtlBuf->Update(&LightMtl, 0, (INT)sizeof(LightMtl));
        /* Fit in 1x1 cell at (1,0,0) */
        matr Base = matr::Translate(vec3(1.5f, 0.5f, 0.5f)) * matr::Scale(vec3(0.25f, 0.25f, 0.25f)) * matr::RotateX(-90);
        Base = Base * matr::Translate({0, 3, 0});
        Ani->Draw(TeapotPrim, Base);
      } /* End of 'Render' function */

      /* Unit destructor */
      ~unit_teapot( VOID )
      {
        logger::info << "Teapot unit deinit.";
      } /* End of destructor */
    };
    static anim::unit_register<unit_teapot> _("Teapot");
  } /* end of anonymous namespace */
} /* end of 'tam' namespace */

/* END OF 'tam_sample_unit_teapot.cpp' FILE */
