/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_sample_unit_VG4.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Sample unit: OBJ meshes, draw, mth::cg collision + mouse drag on plane.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 09.04.2026
 * NOTE        : Namespace 'tam'. Unit name: "VG4Collision". Mesh type: tam::mesh (tam_def.h).
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

#include <cmath>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

/* Project namespace */
namespace tam
{
  /* Anonymous namespace */
  namespace
  {
    /* Maximum OBJ slots (edit kModelPaths below). */
    static constexpr size_t kMaxObjModels = 8;

    /* OBJ paths: replace with your files (e.g. bin/models/cdtest/cow.obj). */
    static const char *const kModelPaths[kMaxObjModels] =
    {
      "bin/models/cdtest/cow.obj",
      "bin/models/cdtest/elephant.obj",
      "bin/models/cdtest/xtrail.obj",
      "bin/models/cdtest/shark.obj",
      "bin/models/cdtest/mammut.obj",
      "bin/models/cdtest/desk.obj",
      "bin/models/cdtest/kall.obj",
      "bin/models/cdtest/sofa.obj",
    };

    /* Parse first integer from OBJ face corner token (before '/'). */
    INT ParseFaceVertexIndex( const std::string &Tok )
    {
      size_t Slash = Tok.find('/');

      if (Slash == std::string::npos)
        return std::stoi(Tok);
      return std::stoi(Tok.substr(0, Slash));
    } /* End of 'ParseFaceVertexIndex' function */

    /* Fill mesh with axis-aligned cube [-S,S]^3 (fallback geometry). */
    VOID MakeUnitCubeMesh( mesh &Out, FLT S )
    {
      Out.Vertices =
      {
        vec3(-S, -S, -S), vec3(S, -S, -S), vec3(S, S, -S), vec3(-S, S, -S),
        vec3(-S, -S, S), vec3(S, -S, S), vec3(S, S, S), vec3(-S, S, S),
      };
      Out.Triangles =
      {
        {0, 1, 2}, {0, 2, 3},
        {4, 6, 5}, {4, 7, 6},
        {0, 4, 5}, {0, 5, 1},
        {2, 6, 7}, {2, 7, 3},
        {0, 3, 7}, {0, 7, 4},
        {1, 5, 6}, {1, 6, 2},
      };
    } /* End of 'MakeUnitCubeMesh' function */

    /* Shift every vertex by Delta (placement for static bodies). */
    VOID TranslateMeshVertices( mesh &M, const vec3 &Delta )
    {
      for (vec3 &P : M.Vertices)
        P += Delta;
    } /* End of 'TranslateMeshVertices' function */

    /* Scale mesh to about TargetExtent and center at origin. */
    VOID NormalizeMeshToUnitCube( mesh &M, FLT TargetExtent )
    {
      if (M.Vertices.empty())
        return;
      vec3 Mn = M.Vertices[0], Mx = M.Vertices[0];

      for (const vec3 &P : M.Vertices)
      {
        Mn = Mn.Min(P);
        Mx = Mx.Max(P);
      }
      vec3 Center = (Mn + Mx) * 0.5f;
      FLT Ext = COM_MAX(Mx.X - Mn.X, COM_MAX(Mx.Y - Mn.Y, Mx.Z - Mn.Z));
      FLT Scl = Ext > 1e-6f ? TargetExtent / Ext : 1.f;

      for (vec3 &P : M.Vertices)
        P = (P - Center) * Scl;
    } /* End of 'NormalizeMeshToUnitCube' function */

    /* Accumulate face normals per vertex (area-weighted), then normalize. */
    VOID ComputeSmoothNormals( const mesh &M, std::vector<vec3> &OutN )
    {
      INT Nv = (INT)M.Vertices.size();
      INT ti;

      OutN.assign((size_t)Nv, vec3(0, 0, 0));
      for (ti = 0; ti < M.TriangleCount(); ti++)
      {
        const mth::cg::triangle &T = M.Triangles[ti];
        vec3
          P0 = M.Vertices[T.V0],
          P1 = M.Vertices[T.V1],
          P2 = M.Vertices[T.V2];
        vec3 E1 = P1 - P0;
        vec3 E2 = P2 - P0;
        vec3 Fn = (E1 % E2).Normalizing();

        OutN[T.V0] += Fn;
        OutN[T.V1] += Fn;
        OutN[T.V2] += Fn;
      }
      for (vec3 &N : OutN)
      {
        if (!!N)
          N.Normalize();
        else
          N = vec3(0, 1, 0);
      }
    } /* End of 'ComputeSmoothNormals' function */

    /* Same shallow-axis MTV as mth::cg::physics_world (push secondary objects). */
    vec3 AabbOverlapMtv( const mth::cg::aabb<FLT> &A, const mth::cg::aabb<FLT> &B )
    {
      FLT
        Ox = COM_MIN(A.Max.X - B.Min.X, B.Max.X - A.Min.X),
        Oy = COM_MIN(A.Max.Y - B.Min.Y, B.Max.Y - A.Min.Y),
        Oz = COM_MIN(A.Max.Z - B.Min.Z, B.Max.Z - A.Min.Z);

      if (Ox < Oy && Ox < Oz)
      {
        FLT Sign = A.Max.X - B.Min.X < B.Max.X - A.Min.X ? 1.f : -1.f;

        return vec3(Ox * Sign, 0, 0);
      }
      if (Oy < Oz)
      {
        FLT Sign = A.Max.Y - B.Min.Y < B.Max.Y - A.Min.Y ? 1.f : -1.f;

        return vec3(0, Oy * Sign, 0);
      }
      FLT Sign = A.Max.Z - B.Min.Z < B.Max.Z - A.Min.Z ? 1.f : -1.f;

      return vec3(0, 0, Oz * Sign);
    } /* End of 'AabbOverlapMtv' function */

    /* Load Wavefront .OBJ into mesh (positions + triangulated faces).
     * ARGUMENTS:
     *   - path:
     *       const std::string &Path;
     *   - vertex scale:
     *       FLT VertScale;
     *   - output:
     *       mesh &OutMesh;
     * RETURNS:
     *   (BOOL) TRUE if at least one triangle exists.
     */
    BOOL LoadWavefrontObj( const std::string &Path, FLT VertScale, mesh &OutMesh )
    {
      try
      {
        std::ifstream F(Path);

        if (!F)
          return FALSE;

        std::vector<vec3> Pos;
        std::string Line;

        OutMesh.Vertices.clear();
        OutMesh.Triangles.clear();

        while (std::getline(F, Line))
        {
          if (Line.empty() || Line[0] == '#')
            continue;
          std::istringstream Iss(Line);
          std::string Tag;

          Iss >> Tag;
          if (Tag == "v")
          {
            FLT x, y, z;

            Iss >> x >> y >> z;
            Pos.push_back(vec3(x, y, z) * VertScale);
          }
          else if (Tag == "f")
          {
            std::vector<INT> Idx;
            std::string Tok;

            while (Iss >> Tok)
            {
              INT Vi = ParseFaceVertexIndex(Tok);

              if (Vi < 0)
                Vi = (INT)Pos.size() + Vi + 1;
              else
                Vi--;
              if (Vi >= 0 && Vi < (INT)Pos.size())
                Idx.push_back(Vi);
            }
            if (Idx.size() < 3)
              continue;
            INT a0 = Idx[0], k;

            for (k = 1; k + 1 < (INT)Idx.size(); k++)
              OutMesh.Triangles.push_back(mth::cg::triangle(a0, Idx[k], Idx[k + 1]));
          }
        }

        OutMesh.Vertices = std::move(Pos);
        return !OutMesh.Triangles.empty();
      }
      catch ( ... )
      {
        OutMesh.Vertices.clear();
        OutMesh.Triangles.clear();
        return FALSE;
      }
    } /* End of 'LoadWavefrontObj' function */

    /* Build drawable primitive: normals for default.vert Phong lighting, vertex color white. */
    prim PrimitiveFromMesh( anim *Ani, material Mtl, const mesh &Mesh, const std::vector<vec3> &Norms )
    {
      std::vector<vertex_std> Verts;
      std::vector<INT> Ind;
      INT i;

      Verts.reserve(Mesh.Vertices.size());
      for (i = 0; i < (INT)Mesh.Vertices.size(); i++)
      {
        vec3 N = i < (INT)Norms.size() ? Norms[i] : vec3(0, 1, 0);

        Verts.emplace_back(Mesh.Vertices[i], vec2(0, 0), N, vec4(1, 1, 1, 1));
      }
      Ind.reserve((size_t)Mesh.TriangleCount() * 3);
      for (i = 0; i < Mesh.TriangleCount(); i++)
      {
        const mth::cg::triangle &T = Mesh.Triangles[i];

        Ind.push_back(T.V0);
        Ind.push_back(T.V1);
        Ind.push_back(T.V2);
      }
      return Ani->PrimitiveCreate(Mtl, std::span(Verts.data(), Verts.size()),
        std::span(Ind.data(), Ind.size()));
    } /* End of 'PrimitiveFromMesh' function */

    /* Distinct diffuse tints for Phong material buffer (default.frag uses KdTrans). */
    vec4 ModelDiffuseColor( size_t Slot )
    {
      static const vec4 Palette[8] =
      {
        vec4(0.90f, 0.42f, 0.28f, 1),
        vec4(0.32f, 0.78f, 0.38f, 1),
        vec4(0.38f, 0.48f, 0.95f, 1),
        vec4(0.92f, 0.86f, 0.32f, 1),
        vec4(0.78f, 0.38f, 0.82f, 1),
        vec4(0.38f, 0.82f, 0.88f, 1),
        vec4(0.88f, 0.52f, 0.38f, 1),
        vec4(0.55f, 0.55f, 0.62f, 1),
      };

      return Palette[Slot % 8];
    } /* End of 'ModelDiffuseColor' function */

    /* GPU primitive + physics body */
    struct obj_model_entry
    {
      prim DrawPrim {};
      std::unique_ptr<mth::cg::physics_object<FLT>> Body {};
    }; /* End of 'obj_model_entry' struct */

    /* After the player pushes one body, relax penetrations among others only (indices >= 1).
     * For each pair J < K with mesh overlap, translate K by the same AABB MTV as player push.
     * Several passes propagate a chain J -> K -> L without moving the player.
     * ARGUMENTS:
     *   - model entries:
     *       std::vector<obj_model_entry> &Models;
     *   - max relaxation sweeps:
     *       INT MaxPasses;
     * RETURNS: None.
     */
    VOID PropagateNonPlayerSeparations( std::vector<obj_model_entry> &Models, INT MaxPasses )
    {
      INT Pass;
      size_t J, K;

      for (Pass = 0; Pass < MaxPasses; Pass++)
      {
        BOOL Any = FALSE;

        for (J = 1; J < Models.size(); J++)
        {
          mth::cg::physics_object<FLT> *A = Models[J].Body.get();

          if (A == nullptr)
            continue;
          for (K = J + 1; K < Models.size(); K++)
          {
            mth::cg::physics_object<FLT> *B = Models[K].Body.get();

            if (B == nullptr)
              continue;
            if (!A->Bounds.Intersects(B->Bounds))
              continue;
            if (!A->Tree.IntersectsTranslated(B->Tree, A->Position, B->Position))
              continue;
            vec3 Mtv = AabbOverlapMtv(A->Bounds, B->Bounds);

            B->Translate(Mtv);
            Any = TRUE;
          }
        }
        if (!Any)
          break;
      }
    } /* End of 'PropagateNonPlayerSeparations' function */

    /* OBJ + mth::cg collision sample */
    class unit_cg_obj_collision : public unit
    {
    private:
      shader Shd {};
      material_pattern MtlPat {};
      std::vector<material> ModelMtls {};
      std::vector<buffer> ModelMtlBufs {};
      std::vector<obj_model_entry> Models {};
      mth::cg::physics_world<FLT> World {};
      FLT DragPlaneY {};

    public:
      /* Class constructor.
       * ARGUMENTS:
       *   - animation context:
       *       anim *Ani;
       */
      unit_cg_obj_collision( anim *Ani )
      {
        size_t Slot;

        logger::info << "VG4Collision: init OBJ + cg mesh unit.";
        Shd = Ani->ShaderCreate("default");
        MtlPat = Ani->MaterialPatternCreate(Shd, "VG4 cg collision pattern", 0, 0, {}, {},
          {
            vlk::vertex_attribute::eVEC3,
            vlk::vertex_attribute::eVEC2,
            vlk::vertex_attribute::eVEC3,
            vlk::vertex_attribute::eVEC4,
          }, vlk::topology::TRIMESH);
        ModelMtls.reserve(kMaxObjModels);
        ModelMtlBufs.reserve(kMaxObjModels);

        Models.reserve(kMaxObjModels);
        for (Slot = 0; Slot < kMaxObjModels; Slot++)
        {
          const char *PathC = kModelPaths[Slot];
          std::string Path = PathC ? PathC : "";
          mesh Mesh {};
          BOOL Loaded = FALSE;
          BUF_MTL_PHONG MtlData {};
          buffer MtlBuf {};
          material MdlMtl {};
          std::vector<vec3> Norms;

          if (!Path.empty())
            Loaded = LoadWavefrontObj(Path, 1.f, Mesh);
          if (!Loaded)
          {
            logger::warning << std::format("VG4Collision: load failed or empty '{}', using cube.", Path);
            MakeUnitCubeMesh(Mesh, 0.5f);
          }
          else
            NormalizeMeshToUnitCube(Mesh, 1.8f);

          {
            INT Ix = (INT)Slot % 4;
            INT Iz = (INT)Slot / 4;
            vec3 Scatter = vec3((FLT)Ix * 2.2f, 0, (FLT)Iz * 2.2f);

            if (Slot > 0)
              TranslateMeshVertices(Mesh, Scatter);
          }

          ComputeSmoothNormals(Mesh, Norms);

          MtlData.Ka = vec4(0.12f, 0.12f, 0.13f, 1);
          MtlData.KdTrans = ModelDiffuseColor(Slot);
          MtlData.KsPh = vec4(0.35f, 0.35f, 0.35f, 48.f);
          memset(MtlData.TextureFlags, 0, sizeof(MtlData.TextureFlags));
          MtlBuf = Ani->BufferCreate<BUF_MTL_PHONG>(&MtlData,
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
          MdlMtl = Ani->MaterialCreate(std::format("VG4 cg mtl {}", Slot), MtlPat, {}, {MtlBuf});
          ModelMtlBufs.push_back(MtlBuf);
          ModelMtls.push_back(MdlMtl);

          obj_model_entry Ent {};

          Ent.Body = std::make_unique<mth::cg::physics_object<FLT>>(Mesh, FALSE);
          if (Slot == 0)
            Ent.Body->MoveTo(vec3(0, 0.2f, 0));
          Ent.DrawPrim = PrimitiveFromMesh(Ani, ModelMtls.back(), Ent.Body->Geom, Norms);
          Models.push_back(std::move(Ent));
          World.AddObject(Models.back().Body.get());
        }

        DragPlaneY = Models[0].Body->Position.Y;
      } /* End of 'unit_cg_obj_collision' function */

      /* Drag model 0 on plane Y = DragPlaneY; push other bodies by AABB MTV on overlap.
       * ARGUMENTS:
       *   - animation context:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
        if (Models.empty() || !Models[0].Body)
          return;
        if (ImGui::GetCurrentContext() != nullptr)
        {
          ImGuiIO &Io = ImGui::GetIO();

          if (Io.WantCaptureMouse)
            return;
        }

        if (!Ani->MBut[0])
          return;

        mth::cg::physics_object<FLT> *Player = Models[0].Body.get();
        mth::ray<FLT> R = Ani->Cam.FrameRay((FLT)Ani->Mx, (FLT)Ani->My);
        vec3 Org = R.Org;
        vec3 Dir = R.Dir;
        FLT Den = Dir.Y;

        if (std::abs(Den) < 1e-5f)
          return;
        FLT t = (DragPlaneY - Org.Y) / Den;

        if (t <= 0.f)
          return;
        vec3 Hit = Org + Dir * t;
        vec3 NewPos = vec3(Hit.X, Player->Position.Y, Hit.Z);
        vec3 OldPos = Player->Position;
        INT Guard;
        size_t J;

        Player->MoveTo(NewPos);
        Guard = 0;
        while (World.HasCollision(Player) && Guard++ < 12)
        {
          BOOL Pushed = FALSE;

          for (J = 1; J < Models.size(); J++)
          {
            mth::cg::physics_object<FLT> *O = Models[J].Body.get();

            if (O == nullptr)
              continue;
            if (!Player->Bounds.Intersects(O->Bounds))
              continue;
            if (!Player->Tree.IntersectsTranslated(O->Tree, Player->Position, O->Position))
              continue;
            vec3 Mtv = AabbOverlapMtv(Player->Bounds, O->Bounds);

            O->Translate(Mtv);
            PropagateNonPlayerSeparations(Models, 24);
            Pushed = TRUE;
            break;
          }
          if (!Pushed)
            break;
        }
        if (World.HasCollision(Player))
          Player->MoveTo(OldPos);
      } /* End of 'Response' function */

      /* Draw all meshes at physics positions.
       * ARGUMENTS:
       *   - animation context:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Render( anim *Ani ) override
      {
        vec4 AabbColor(1, 0, 0, 1);

        for (obj_model_entry &Ent : Models)
        {
          if (!Ent.DrawPrim || !Ent.Body)
            continue;
          matr W = matr::Translate(Ent.Body->Position);

          Ani->Draw(Ent.DrawPrim, W);
          Ani->DrawAABB(Ent.Body->Bounds.Min, Ent.Body->Bounds.Max, AabbColor);
        }
      } /* End of 'Render' function */

      /* Unit destructor */
      ~unit_cg_obj_collision( VOID ) override
      {
        size_t i;

        for (obj_model_entry &Ent : Models)
        {
          if (Ent.DrawPrim)
            Ent.DrawPrim->Free();
        }
        for (i = 0; i < ModelMtls.size(); i++)
        {
          if (ModelMtls[i])
            ModelMtls[i]->Free();
        }
        for (i = 0; i < ModelMtlBufs.size(); i++)
        {
          if (ModelMtlBufs[i])
            ModelMtlBufs[i]->Free();
        }
        if (MtlPat)
          MtlPat->Free();
        if (Shd)
          Shd->Free();
        logger::info << "VG4Collision: deinit.";
      } /* End of '~unit_cg_obj_collision' function */
    }; /* End of 'unit_cg_obj_collision' class */

    static anim::unit_register<unit_cg_obj_collision> _("VG4Collision");
  } /* end of anonymous namespace */
} /* end of 'tam' namespace */

/* END OF 'tam_sample_unit_VG4.cpp' FILE */
