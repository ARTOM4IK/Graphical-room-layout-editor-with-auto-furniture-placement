/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_sample_unit_mip_test.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Visual test unit: display each mip level of a texture.
 * PROGRAMMER  : CGSG'Sr'2025.
 * LAST UPDATE : 12.03.2026
 * NOTE        : Namespace 'tam'. Unit name: "MipTest".
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
    /* Mip-map visual test unit representation class */
    class unit_mip_test : public unit
    {
    private:
      texture TestTex;             // Texture with mip chain
      buffer MipLevelBuf;          // Buffer: current mip level (float)
      material_pattern MtlPat;     // Material pattern (1 tex, 1 buf)
      material Mtl;                // Material
      std::vector<prim> QuadPrims; // Fullscreen quad
      UINT MipLevels = 0;          // Number of mip levels (from texture)
      vec3 LightDir = vec3(1, 1, 1).Normalizing(); // Light direction for light gizmo
    public:

      /* Class constructor.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       */
      unit_mip_test( anim *Ani )
      {
        logger::info << "MipTest unit initialization.";
        TestTex = Ani->TextureCreate("bin/textures/giraffe.jpg");
        if (TestTex == nullptr)
        {
          logger::error << "MipTest: failed to load texture.";
          return;
        }
        MipLevels = TestTex->Img.MipLevels;
        if (MipLevels == 0)
          MipLevels = 1;

        shader Shd = Ani->ShaderCreate("samples/mip_test");
        if (Shd == nullptr)
        {
          logger::error << "MipTest: failed to load shader samples/mip_test.";
          return;
        }
        MtlPat = Ani->MaterialPatternCreate(Shd, "MipTest pattern", 1, 1, {}, {},
            {
              vlk::vertex_attribute::eVEC3,
              vlk::vertex_attribute::eVEC2,
              vlk::vertex_attribute::eVEC3,
              vlk::vertex_attribute::eVEC4
            }, vlk::topology::TRIMESH, vlk::blend_mode::eOPAQUE);
        for (INT i = 0; i < MipLevels; i++)
        {
          FLT level = i;
          MipLevelBuf = Ani->buffer_manager::BufferCreate<FLT>(&level,
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);//VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);//
          if (MipLevelBuf == nullptr)
          {
            logger::error << "MipTest: failed to create mip level buffer.";
            continue;
          }
          Mtl = Ani->MaterialCreate(std::format("MipTest material #{}", i), MtlPat, {TestTex}, {MipLevelBuf});
          if (Mtl == nullptr)
          {
            logger::error << "MipTest: failed to create material.";
            continue;
          }
          std::vector<vertex_std> Vertices
          {
            {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
            {{1, 0, 0}, {1, 0}, {0, 0, 1}, {1, 1, 1, 1}},
            {{1, 1, 0}, {1, 1}, {0, 0, 1}, {1, 1, 1, 1}},
            {{0, 1, 0}, {0, 1}, {0, 0, 1}, {1, 1, 1, 1}}
          };
          std::vector<INT> Indices{0, 1, 2, 0, 2, 3};
          QuadPrims.push_back(Ani->PrimitiveCreate(Mtl, std::span(Vertices.data(), Vertices.size()), std::span(Indices.data(), Indices.size())));
        }
        logger::info << "MipTest unit ready. Mip levels: " << (INT)MipLevels;
      } /* End of 'unit_mip_test' function */

      /* Get response from unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
        // Transform gizmo interaction zone: bottom-right 25% of the viewport
        FLT rgn_x = Ani->Cam.FrameW * 0.75;
        FLT rgn_y = Ani->Cam.FrameH * 0.75;

        if (Ani->Mx > rgn_x && Ani->My > rgn_y)
        {
          if (Ani->Keys[VK_LBUTTON])
          {
            FLT rot_speed = 0.3;
            matr rot = matr::Rotate(-Ani->Mdx * rot_speed, vec3(0, 1, 0)) *
                       matr::Rotate(-Ani->Mdy * rot_speed, Ani->Cam.Right);
            vec3 offset = rot.TransformVector(Ani->Cam.Loc - Ani->Cam.At);
            Ani->Cam.SetLocAtUp(Ani->Cam.At + offset, Ani->Cam.At, vec3(0, 1, 0));
            Ani->IsCamBufUpdate = TRUE;
          }
          if (Ani->Keys[VK_RBUTTON])
          {
            FLT dist = !(Ani->Cam.At - Ani->Cam.Loc);
            FLT pan_speed = dist * 0.002;
            vec3 dv = Ani->Cam.Right * (-Ani->Mdx * pan_speed) +
                      Ani->Cam.Up * (Ani->Mdy * pan_speed);
            Ani->Cam.At = Ani->Cam.At + dv;
            Ani->Cam.Loc = Ani->Cam.Loc + dv;
            Ani->Cam.SetLocAtUp(Ani->Cam.Loc, Ani->Cam.At, vec3(0, 1, 0));
            Ani->IsCamBufUpdate = TRUE;
          }
          if (Ani->Mdz != 0)
          {
            vec3 to_cam = Ani->Cam.Loc - Ani->Cam.At;
            FLT dist = !to_cam;
            dist -= Ani->Mdz * 0.001 * dist;
            if (dist < 0.1)
              dist = 0.1;
            Ani->Cam.SetLocAtUp(Ani->Cam.At + to_cam.Normalizing() * dist, Ani->Cam.At, vec3(0, 1, 0));
            Ani->IsCamBufUpdate = TRUE;
          }
        }
      } /* End of 'Response' function */

      /* Render unit function: draw one quad per mip level in a row.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Render( anim *Ani ) override
      {
        FLT QuadW = 1.0f;
        FLT Gap = 0.05f;
        FLT StartX = -(FLT)(MipLevels - 1) * (QuadW + Gap) * 0.5f;
        for (INT i = 0; i < QuadPrims.size(); i++)
        {
          FLT X = StartX + i * (QuadW + Gap);
          matr T = matr::Translate(vec3(X, 0, 0)) * matr::Scale(vec3(QuadW, QuadW, 1));
          Ani->Draw(QuadPrims[i], T);
        }
        Ani->DrawSphere({0, 0, 2}, 0.30, {1, 1, 0, 1});
        Ani->DrawSphere({0, 1, 0}, 0.30, {1, 0, 0, 1});

        INT s = 5;
        auto cf =
          [=]( INT InColor ) -> FLT
          {
            return (InColor + s) / (2 * s + 1.0);
          };
        for (INT i = -s; i <= s; i++)
          for (INT j = -s; j <= s; j++)
            for (INT k = -s; k <= s; k++)
            {
              Ani->DrawSphere(vec3(k, j, i), 0.18, {cf(k), cf(j), cf(i), 1});
              if (k < s)
                Ani->DrawCylinder(vec3(k, j, i), 0.08, vec3(k + 1, j, i), 0.08, {1 - cf(k), 1 - cf(j), 1 - cf(i), 1});
              if (j < s)
                Ani->DrawCylinder(vec3(k, j, i), 0.08, vec3(k, j + 1, i), 0.08, {1 - cf(k), 1 - cf(j), 1 - cf(i), 1});
              if (i < s)
                Ani->DrawCylinder(vec3(k, j, i), 0.08, vec3(k, j, i + 1), 0.08, {1 - cf(k), 1 - cf(j), 1 - cf(i), 1});
            }

        // Axes view (orientation gizmo)
        FLT
          gizmo_distance = 5.0,
          visible_w = Ani->Cam.Wp * gizmo_distance / Ani->Cam.ProjDist,
          visible_h = Ani->Cam.Hp * gizmo_distance / Ani->Cam.ProjDist,
          min_size = COM_MIN(visible_w, visible_h),
          axis_len = min_size * 0.08,
          len1 = axis_len * 0.47, len2 = axis_len * 0.30,
          margin = axis_len * 0.5,
          min_r = 0.01,
          r1 = COM_MAX(axis_len * 0.08, min_r),
          r2 = COM_MAX(axis_len * 0.30, min_r * 2);

        vec3 p1, p =
          Ani->Cam.Loc + Ani->Cam.Dir * gizmo_distance -
          Ani->Cam.Right * (visible_w * 0.5 - margin - axis_len) -
          Ani->Cam.Up * (visible_h * 0.5 - margin - axis_len);

        Ani->DrawCylinder(p, r1, p1 = p + vec3(len1, 0, 0), r1, vec4(1, 0, 0, 1));
        Ani->DrawCylinder(p1, r2, p1 + vec3(len2, 0, 0), 0.0, vec4(1, 0, 0, 1));
        Ani->DrawCylinder(p, r1, p1 = p + vec3(0, len1, 0), r1, vec4(0, 1, 0, 1));
        Ani->DrawCylinder(p1, r2, p1 + vec3(0, len2, 0), 0.0, vec4(0, 1, 0, 1));
        Ani->DrawCylinder(p, r1, p1 = p + vec3(0, 0, len1), r1, vec4(0, 0, 1, 1));
        Ani->DrawCylinder(p1, r2, p1 + vec3(0, 0, len2), 0.0, vec4(0, 0, 1, 1));

        // Transform gizmo (bottom-right): world axes with handle spheres
        vec3 tp =
          Ani->Cam.Loc + Ani->Cam.Dir * gizmo_distance +
          Ani->Cam.Right * (visible_w * 0.5 - margin - axis_len) -
          Ani->Cam.Up * (visible_h * 0.5 - margin - axis_len);

        Ani->DrawSphere(tp, r2, vec4(0.9, 0.9, 0.9, 1));
        Ani->DrawCylinder(tp, r1, p1 = tp + vec3(len1, 0, 0), r1, vec4(1, 0, 0, 1));
        Ani->DrawCylinder(p1, r2, p1 + vec3(len2, 0, 0), 0.0, vec4(1, 0, 0, 1));
        Ani->DrawSphere(p1 + vec3(len2, 0, 0), r1 * 1.5, vec4(1, 0.3, 0.3, 1));
        Ani->DrawCylinder(tp, r1, p1 = tp + vec3(0, len1, 0), r1, vec4(0, 1, 0, 1));
        Ani->DrawCylinder(p1, r2, p1 + vec3(0, len2, 0), 0.0, vec4(0, 1, 0, 1));
        Ani->DrawSphere(p1 + vec3(0, len2, 0), r1 * 1.5, vec4(0.3, 1, 0.3, 1));
        Ani->DrawCylinder(tp, r1, p1 = tp + vec3(0, 0, len1), r1, vec4(0, 0, 1, 1));
        Ani->DrawCylinder(p1, r2, p1 + vec3(0, 0, len2), 0.0, vec4(0, 0, 1, 1));
        Ani->DrawSphere(p1 + vec3(0, 0, len2), r1 * 1.5, vec4(0.3, 0.3, 1, 1));

        // Light gizmo (bottom-center): sun icon with direction arrow
        vec3 lp =
          Ani->Cam.Loc + Ani->Cam.Dir * gizmo_distance -
          Ani->Cam.Up * (visible_h * 0.5 - margin - axis_len);

        FLT
          sun_r = axis_len * 0.25,
          ray_len = axis_len * 0.35,
          ray_r = COM_MAX(axis_len * 0.04, min_r * 0.5);

        Ani->DrawSphere(lp, sun_r, vec4(1, 0.9, 0.2, 1));
        for (INT i = 0; i < 8; i++)
        {
          FLT a = i * 0.7853981634;
          vec3 d = Ani->Cam.Right * (FLT)cos(a) + Ani->Cam.Up * (FLT)sin(a);
          Ani->DrawCylinder(lp + d * sun_r * 1.4, ray_r,
                            lp + d * (sun_r * 1.4 + ray_len), ray_r,
                            vec4(1, 0.8, 0.1, 1));
        }
        Ani->DrawCylinder(lp, r1 * 0.7,
                          p1 = lp + LightDir * (axis_len * 0.5), r1 * 0.7,
                          vec4(1, 1, 0.5, 1));
        Ani->DrawCylinder(p1, r2 * 0.7,
                          p1 + LightDir * (axis_len * 0.3), 0.0,
                          vec4(1, 1, 0.5, 1));
      } /* End of 'Render' function */

      /* Unit destructor */
      ~unit_mip_test( VOID )
      {
        logger::info << "MipTest unit deinitialization.";
      } /* End of destructor */
    };
    static tam::anim::unit_register<unit_mip_test> _("MipTest");
  } /* end of anonymous namespace */
} /* end of 'tam' namespace */

/* END OF 'tam_sample_unit_mip_test.cpp' FILE */
