/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_cg_physics.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math support module.
 *               Computation geometry: simple rigid-body broad/narrow phase stubs.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 10.04.2026
 * NOTE        : Namespace 'mth::cg'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_cg_physics_h_
#define __mth_cg_physics_h_

#include <vector>

#include "mth_cg_aabb.h"
#include "mth_cg_bvh.h"
#include "mth_cg_mesh.h"

/* 'mth' namespace */
namespace mth
{
  /* Computation geometry: collision tests and acceleration structures */
  namespace cg
  {
    /* Dynamic or static body: mesh in model space plus world pose */
    template<typename type>
      class physics_object
      {
      public:
        /* Local geometry copy used for BVH and narrow-phase tests */
        mesh<type> Geom;
        /* Acceleration structure over Geom (model space) */
        bvh<type> Tree;
        /* World-space axis-aligned bounds after translation */
        aabb<type> Bounds;
        /* World-space origin of the mesh */
        vec3<type> Position;
        /* Linear velocity placeholder for future integration */
        vec3<type> Velocity;
        /* TRUE disables MoveTo / Translate */
        BOOL IsStatic;

        /* Build mesh, BVH, and initial world bounds class constructor.
         * ARGUMENTS:
         *   - triangle mesh data:
         *       const mesh<type> &M;
         *   - static flag:
         *       BOOL StaticObj;
         * RETURNS: None.
         */
        physics_object( const mesh<type> &M, BOOL StaticObj = FALSE ) :
          Geom(M), Tree(Geom),
          Position(vec3<type>::Zero()), Velocity(vec3<type>::Zero()),
          IsStatic(StaticObj)
        {
          UpdateBounds();
        } /* End of 'physics_object' function */

        /* Refresh Bounds from Tree.RootBounds() plus Position function.
         * ARGUMENTS: None.
         * RETURNS: None.
         */
        VOID UpdateBounds( VOID )
        {
          aabb<type> Local = Tree.RootBounds();

          Bounds = Local;
          if (Bounds.IsValid())
          {
            Bounds.Min += Position;
            Bounds.Max += Position;
          }
        } /* End of 'UpdateBounds' function */

        /* Set world position for a dynamic body function.
         * ARGUMENTS:
         *   - new translation:
         *       const vec3<type> &NewPos;
         * RETURNS: None.
         */
        VOID MoveTo( const vec3<type> &NewPos )
        {
          if (!IsStatic)
          {
            Position = NewPos;
            UpdateBounds();
          }
        } /* End of 'MoveTo' function */

        /* Add delta to world position when not static function.
         * ARGUMENTS:
         *   - translation delta:
         *       const vec3<type> &Delta;
         * RETURNS: None.
         */
        VOID Translate( const vec3<type> &Delta )
        {
          if (!IsStatic)
          {
            Position += Delta;
            UpdateBounds();
          }
        } /* End of 'Translate' function */
      }; /* End of 'physics_object' class */

    /* Naive O(n^2) pairwise collision pass over registered bodies */
    template<typename type>
      class physics_world
      {
      private:
        /* Non-owning pointers to bodies under test */
        std::vector<physics_object<type> *> Objects;

        /* Apply MTV to resolve static/dynamic or split between two dynamic bodies function.
         * ARGUMENTS:
         *   - first body:
         *       physics_object<type> *A;
         *   - second body:
         *       physics_object<type> *B;
         *   - minimum translation from AABB separation:
         *       const vec3<type> &Mtv;
         * RETURNS:
         *   (BOOL) FALSE if both static; TRUE after a resolution step.
         */
        BOOL ResolveCollision( physics_object<type> *A, physics_object<type> *B,
                               const vec3<type> &Mtv )
        {
          if (A->IsStatic && B->IsStatic)
            return FALSE;
          if (A->IsStatic)
          {
            B->Translate(Mtv);
            return TRUE;
          }
          if (B->IsStatic)
          {
            A->Translate(-Mtv);
            return TRUE;
          }
          type Half = static_cast<type>(0.5);

          A->Translate(-Mtv * Half);
          B->Translate(Mtv * Half);
          return TRUE;
        } /* End of 'ResolveCollision' function */

        /* Pick shallowest AABB penetration axis and signed separation vector function.
         * ARGUMENTS:
         *   - first world AABB:
         *       const aabb<type> &A;
         *   - second world AABB:
         *       const aabb<type> &B;
         * RETURNS:
         *   (vec3<type>) translation along one world axis.
         */
        static vec3<type> ComputeOverlapMtv( const aabb<type> &A, const aabb<type> &B )
        {
          type
            Ox = COM_MIN(A.Max.X - B.Min.X, B.Max.X - A.Min.X),
            Oy = COM_MIN(A.Max.Y - B.Min.Y, B.Max.Y - A.Min.Y),
            Oz = COM_MIN(A.Max.Z - B.Min.Z, B.Max.Z - A.Min.Z);

          if (Ox < Oy && Ox < Oz)
          {
            type Sign = A.Max.X - B.Min.X < B.Max.X - A.Min.X ? static_cast<type>(1) : static_cast<type>(-1);

            return vec3<type>(Ox * Sign, static_cast<type>(0), static_cast<type>(0));
          }
          if (Oy < Oz)
          {
            type Sign = A.Max.Y - B.Min.Y < B.Max.Y - A.Min.Y ? static_cast<type>(1) : static_cast<type>(-1);

            return vec3<type>(static_cast<type>(0), Oy * Sign, static_cast<type>(0));
          }
          type Sign = A.Max.Z - B.Min.Z < B.Max.Z - A.Min.Z ? static_cast<type>(1) : static_cast<type>(-1);

          return vec3<type>(static_cast<type>(0), static_cast<type>(0), Oz * Sign);
        } /* End of 'ComputeOverlapMtv' function */

      public:
        /* Register a body pointer (not owned) function.
         * ARGUMENTS:
         *   - body to track:
         *       physics_object<type> *Obj;
         * RETURNS: None.
         */
        VOID AddObject( physics_object<type> *Obj )
        {
          Objects.push_back(Obj);
        } /* End of 'AddObject' function */

        /* Drop all registered pointers function.
         * ARGUMENTS: None.
         * RETURNS: None.
         */
        VOID Clear( VOID )
        {
          Objects.clear();
        } /* End of 'Clear' function */

        /* Broad phase AABB, narrow phase BVH + translated triangles, MTV from AABB overlap function.
         * ARGUMENTS: None.
         * RETURNS: None.
         */
        VOID Update( VOID )
        {
          size_t i, j;

          for (i = 0; i < Objects.size(); i++)
            for (j = i + 1; j < Objects.size(); j++)
            {
              physics_object<type> *A = Objects[i];
              physics_object<type> *B = Objects[j];

              if (A == nullptr || B == nullptr)
                continue;
              if (!A->Bounds.Intersects(B->Bounds))
                continue;
              if (!A->Tree.IntersectsTranslated(B->Tree, A->Position, B->Position))
                continue;
              vec3<type> Mtv = ComputeOverlapMtv(A->Bounds, B->Bounds);

              ResolveCollision(A, B, Mtv);
            }
        } /* End of 'Update' function */

        /* Test one body against all others using AABB then BVH translated intersection function.
         * ARGUMENTS:
         *   - candidate body:
         *       physics_object<type> *Obj;
         * RETURNS:
         *   (BOOL) TRUE if any overlapping mesh intersection exists.
         */
        BOOL HasCollision( physics_object<type> *Obj ) const
        {
          size_t k;

          if (Obj == nullptr)
            return FALSE;
          for (k = 0; k < Objects.size(); k++)
          {
            physics_object<type> *O = Objects[k];

            if (O == nullptr || O == Obj)
              continue;
            if (!Obj->Bounds.Intersects(O->Bounds))
              continue;
            if (Obj->Tree.IntersectsTranslated(O->Tree, Obj->Position, O->Position))
              return TRUE;
          }
          return FALSE;
        } /* End of 'HasCollision' function */
      }; /* End of 'physics_world' class */
  } /* end of 'cg' namespace */
} /* end of 'mth' namespace */

#endif /* __mth_cg_physics_h_ */

/* END OF 'mth_cg_physics.h' FILE */
