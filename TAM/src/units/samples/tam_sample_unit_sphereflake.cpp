/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_sample_unit_sphereflake.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Sphereflake (depth 4), tessellated spheres, chrome, light (1,1,1).
 * PROGRAMMER  : CGSG'Sr'2025.
 * LAST UPDATE : 26.02.2026
 * NOTE        : Namespace 'tam'. Unit name: "Sphereflake". Box 1x1, cell (0,0,0).
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
    /* Light soure material pass data */
    struct BUF_DIRLIGHT_MTL
    {
      vec4 LightDir;    // Normalized light source direction
      vec4 LightColor;  // Light source color
      vec4 CamPos;      // Camera position
      vec4 Ka, Kd, Ks, Ph; // Phong illumination coefficients
    }; /* End of 'BUF_DIRLIGHT_MTL' structure */

    /* Icosahedron: 12 vertices, 20 triangular patches (3 control points each) */
    static const FLT IcosaX = 0.525731112119133606f;
    static const FLT IcosaZ = 0.850650808352039932f;
    static const vec3 IcoV[12] =
    {
      vec3(-IcosaX, 0, IcosaZ), vec3(IcosaX, 0, IcosaZ), vec3(-IcosaX, 0, -IcosaZ), vec3(IcosaX, 0, -IcosaZ),
      vec3(0, IcosaZ, IcosaX), vec3(0, IcosaZ, -IcosaX), vec3(0, -IcosaZ, IcosaX), vec3(0, -IcosaZ, -IcosaX),
      vec3(IcosaZ, IcosaX, 0), vec3(-IcosaZ, IcosaX, 0), vec3(IcosaZ, -IcosaX, 0), vec3(-IcosaZ, -IcosaX, 0)
    };
    static const INT IcoInd[60] =
    {
      0,1,4, 0,4,9, 0,9,11, 0,11,6, 0,6,1,
      1,6,10, 1,10,8, 1,8,4, 4,8,2, 4,2,5,
      4,5,9, 9,5,3, 9,3,11, 11,3,7, 11,7,6,
      6,7,10, 10,7,2, 10,2,8, 2,7,3, 2,3,5
    };

    /* Sphereflake: 9 child directions on parent surface — 6 in horizontal belt slightly below center, 3 above.
     * Unit vectors: belt lat ≈ -30°, top lat ≈ +60°. */
    static const vec3 FlakeDirs[9] =
    {
      /* 6 in horizontal ring, slightly below center (y < 0) */
      vec3(0.866025f, -0.5f, 0.0f),
      vec3(0.433013f, -0.5f, 0.75f),
      vec3(-0.433013f, -0.5f, 0.75f),
      vec3(-0.866025f, -0.5f, 0.0f),
      vec3(-0.433013f, -0.5f, -0.75f),
      vec3(0.433013f, -0.5f, -0.75f),
      /* 3 on top (y > 0) */
      vec3(0.5f, 0.866025f, 0.0f),
      vec3(-0.25f, 0.866025f, 0.433013f),
      vec3(-0.25f, 0.866025f, -0.433013f)
    };

    /* Single sphere instance (position and scale) */
    struct sphere_inst
    {
      vec3 Pos;
      FLT Scale;
    }; /* End of 'sphere_inst' structure */

    /* Sphereflake recursion: 1 sphere + 9 children on its surface (6 in belt slightly below center, 3 on top).
     * On each child the same pattern repeats. Children placed outward, tangent to parent.
     * ARGUMENTS:
     *   - output vector of sphere instances:
     *       std::vector<sphere_inst> &Out;
     *   - current center position:
     *       const vec3 &Pos;
     *   - current sphere scale:
     *       FLT Scale;
     *   - current recursion depth:
     *       INT Depth;
     *   - maximum recursion depth:
     *       INT MaxDepth;
     * RETURNS: None.
     */
    static VOID sphereflake_rec( std::vector<sphere_inst> &Out, const vec3 &Pos, FLT Scale, INT Depth, INT MaxDepth )
    {
      if (Depth >= MaxDepth)
        return;
      Out.push_back({Pos, Scale});
      FLT R = Scale;
      FLT NextScale = Scale * 0.45f;
      FLT DistOutward = R + NextScale;
      for (INT i = 0; i < 9; i++)
        sphereflake_rec(Out, Pos + FlakeDirs[i] * DistOutward, NextScale, Depth + 1, MaxDepth);
    } /* End of 'sphereflake_rec' function */

    /* Sphereflake unit representation class */
    class unit_sphereflake : public unit
    {
      prim IcoPrim;
      material Mtl;
      std::vector<sphere_inst> Spheres;
      buffer LightMtlBuf;
    public:
      /* Class constructor.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       */
      unit_sphereflake( anim *Ani )
      {
        logger::info << "Sphereflake unit init.";
        BUF_DIRLIGHT_MTL LightMtl = {};
        vec3 LightDir(1, 1, 1);
        LightMtl.LightDir = vec4(LightDir.Normalizing(), 0);
        LightMtl.LightColor = vec4(1, 1, 1, 1);
        LightMtl.CamPos = vec4(0, 0, 0, 1);
        /* Chrome: visible silver base + strong specular so not black, clear highlight */
        LightMtl.Ka = vec4(0.2f, 0.2f, 0.25f, 1);
        LightMtl.Kd = vec4(0.35f, 0.35f, 0.4f, 1);
        LightMtl.Ks = vec4(1.0f, 1.0f, 1.0f, 1);
        LightMtl.Ph = vec4(320.0f, 0, 0, 0);
        LightMtlBuf = Ani->buffer_manager::BufferCreate<BUF_DIRLIGHT_MTL>(&LightMtl,
          VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        shader Shd = Ani->ShaderCreate("samples/sphereflake");
        material_pattern Pat = Ani->MaterialPatternCreate(Shd, "Sphereflake pat", 0, 1, {}, {},
          {vlk::vertex_attribute::eVEC3, vlk::vertex_attribute::eVEC2, vlk::vertex_attribute::eVEC3, vlk::vertex_attribute::eVEC4},
          vlk::topology::PATCH, vlk::blend_mode::eOPAQUE, 3);
        Mtl = Ani->MaterialCreate("Sphereflake chrome", Pat, {}, {LightMtlBuf});
        std::vector<vertex_std> V(12);
        for (INT i = 0; i < 12; i++)
          V[i] = vertex_std(IcoV[i], vec2(0, 0), IcoV[i].Normalizing(), vec4(1, 1, 1, 1));
        std::vector<INT> Ind(IcoInd, IcoInd + 60);
        IcoPrim = Ani->PrimitiveCreate(Mtl, std::span(V.data(), V.size()), std::span(Ind.data(), Ind.size()));
        sphereflake_rec(Spheres, vec3(0, 0, 0), 0.4f, 0, 4);
        logger::info << "Sphereflake: " << (INT)Spheres.size() << " spheres.";
      } /* End of 'unit_sphereflake' function */

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
        if (IcoPrim == nullptr) return;
        BUF_DIRLIGHT_MTL LightMtl = {};
        vec3 LightDir = vec3(1, 1, 1).Normalizing();
        LightMtl.LightDir = vec4(LightDir.X, LightDir.Y, LightDir.Z, 0);
        LightMtl.LightColor = vec4(1, 1, 1, 1);
        LightMtl.CamPos = vec4(Ani->Cam.Loc.X, Ani->Cam.Loc.Y, Ani->Cam.Loc.Z, 1);
        LightMtl.Ka = vec4(0.2f, 0.2f, 0.25f, 1);
        LightMtl.Kd = vec4(0.35f, 0.35f, 0.4f, 1);
        LightMtl.Ks = vec4(1.0f, 1.0f, 1.0f, 1);
        LightMtl.Ph = vec4(320.0f, 0, 0, 0);
        LightMtlBuf->Update(&LightMtl, 0, (INT)sizeof(LightMtl));
        /* Fit in 1x1 cell at (0,0,0) */
        matr Base = matr::Translate(vec3(0.5f, 0.5f, 0.5f)) * matr::Scale(vec3(0.5f, 0.5f, 0.5f));
        /* Draw far-to-near so child spheres (in front) are not occluded by the central sphere */
        vec3 CamLoc = Ani->Cam.Loc;
        std::vector<std::pair<FLT, SIZE_T>> Order(Spheres.size());
        for (SIZE_T i = 0; i < Spheres.size(); i++)
        {
          vec3 D = Spheres[i].Pos - CamLoc;
          Order[i] = {!D, i};
        }
        std::sort(Order.begin(), Order.end(),
          []( const std::pair<FLT, SIZE_T> &A,
              const std::pair<FLT, SIZE_T> &B )
          {
            return A.first > B.first;
          });
        for (const auto &P : Order)
        {
          const sphere_inst &S = Spheres[P.second];
          matr T = Base * matr::Translate(S.Pos) * matr::Scale(vec3(S.Scale));
          Ani->Draw(IcoPrim, T);
        }
      } /* End of 'Render' function */

      /* Unit destructor */
      ~unit_sphereflake( VOID )
      {
        logger::info << "Sphereflake unit deinit.";
      } /* End of destructor */
    };
    static anim::unit_register<unit_sphereflake> _("Sphereflake");
  } /* end of anonymous namespace */
} /* end of 'tam' namespace */

/* END OF 'tam_sample_unit_sphereflake.cpp' FILE */
