/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_cg_triangles.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math support module.
 *               Computation geometry: triangle-triangle intersection tests.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 09.04.2026
 * NOTE        : Namespace 'mth::cg'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_cg_triangles_h_
#define __mth_cg_triangles_h_

#include <cmath>
#include <type_traits>

#include <commondf.h>

#include "mth_cg_mesh.h"
#include "../mth_def.h"

/* Math types and operations: vectors, matrices, rays, etc. */
namespace mth
{
  /* Computation geometry: collision tests and acceleration structures */
  namespace cg
  {
    /* Internal helpers not part of the public collision API */
    namespace detail
    {
      /* Get segment-triangle test epsilon for the given scalar type.
       * ARGUMENTS: None.
       * RETURNS:
       *   (type) small positive comparison threshold.
       */
      template<typename type>
        inline type TriangleTestEpsilon( VOID )
        {
          if constexpr (std::is_same_v<type, FLT>)
            return static_cast<type>(1e-6f);
          return static_cast<type>(Threshold);
        } /* End of 'TriangleTestEpsilon' function */
    } /* end of 'detail' namespace */

    /* Barycentric test: point in triangle plane (coplanar assumed).
     * ARGUMENTS:
     *   - point:
     *       const vec3<type> &P;
     *   - triangle corners:
     *       const vec3<type> &T0, &T1, &T2;
     * RETURNS:
     *   (BOOL) TRUE if barycentric coordinates are inside the triangle.
     */
    template<typename type>
      BOOL PointInTriangle( const vec3<type> &P,
                            const vec3<type> &T0, const vec3<type> &T1, const vec3<type> &T2 )
      {
        vec3<type>
          V0 = T1 - T0,
          V1 = T2 - T0,
          V2 = P - T0;
        type
          Dot00 = V0 & V0,
          Dot01 = V0 & V1,
          Dot02 = V0 & V2,
          Dot11 = V1 & V1,
          Dot12 = V1 & V2,
          Denom = Dot00 * Dot11 - Dot01 * Dot01;

        if (std::abs(static_cast<DBL>(Denom)) < Threshold)
          return FALSE;
        type InvDenom = static_cast<type>(1) / Denom;
        type U = (Dot11 * Dot02 - Dot01 * Dot12) * InvDenom;
        type V = (Dot00 * Dot12 - Dot01 * Dot02) * InvDenom;

        return U >= static_cast<type>(0) && V >= static_cast<type>(0) && U + V <= static_cast<type>(1);
      } /* End of 'PointInTriangle' function */

    /* Open segment versus triangle: endpoints excluded as hits.
     * ARGUMENTS:
     *   - segment endpoints:
     *       const vec3<type> &P0, &P1;
     *   - triangle corners:
     *       const vec3<type> &T0, &T1, &T2;
     * RETURNS:
     *   (BOOL) TRUE if the segment cuts the triangle interior.
     */
    template<typename type>
      BOOL SegmentIntersectsTriangle( const vec3<type> &P0, const vec3<type> &P1,
                                      const vec3<type> &T0, const vec3<type> &T1,
                                      const vec3<type> &T2 )
      {
        vec3<type>
          RayDir = P1 - P0,
          Edge1 = T1 - T0,
          Edge2 = T2 - T0,
          H = RayDir % Edge2;
        type A = Edge1 & H;
        type Eps = detail::TriangleTestEpsilon<type>();

        if (std::abs(static_cast<DBL>(A)) < static_cast<DBL>(Eps))
          return FALSE;

        type F = static_cast<type>(1) / A;
        vec3<type> S = P0 - T0;
        type U = F * (S & H);

        if (U < static_cast<type>(0) || U > static_cast<type>(1))
          return FALSE;

        vec3<type> Q = S % Edge1;
        type V = F * (RayDir & Q);

        if (V < static_cast<type>(0) || U + V > static_cast<type>(1))
          return FALSE;

        type Tpar = F * (Edge2 & Q);

        return Tpar > Eps && Tpar < static_cast<type>(1) - Eps;
      } /* End of 'SegmentIntersectsTriangle' function */

    /* Two triangles share volume or intersect in a segment or point.
     * ARGUMENTS:
     *   - first triangle vertices:
     *       const vec3<type> &A0, &A1, &A2;
     *   - second triangle vertices:
     *       const vec3<type> &B0, &B1, &B2;
     * RETURNS:
     *   (BOOL) TRUE if they intersect.
     */
    template<typename type>
      BOOL TrianglesIntersect( const vec3<type> &A0, const vec3<type> &A1, const vec3<type> &A2,
                               const vec3<type> &B0, const vec3<type> &B1, const vec3<type> &B2 )
      {
        vec3<type> VertsA[3] = {A0, A1, A2};
        vec3<type> VertsB[3] = {B0, B1, B2};
        INT i;

        /* Any vertex of A inside B */
        for (i = 0; i < 3; i++)
          if (PointInTriangle(VertsA[i], B0, B1, B2))
            return TRUE;
        /* Any vertex of B inside A */
        for (i = 0; i < 3; i++)
          if (PointInTriangle(VertsB[i], A0, A1, A2))
            return TRUE;

        /* Edges of A against triangle B */
        for (i = 0; i < 3; i++)
        {
          vec3<type>
            E0 = VertsA[i],
            E1 = VertsA[(i + 1) % 3];

          if (SegmentIntersectsTriangle(E0, E1, B0, B1, B2))
            return TRUE;
        }
        /* Edges of B against triangle A */
        for (i = 0; i < 3; i++)
        {
          vec3<type>
            E0 = VertsB[i],
            E1 = VertsB[(i + 1) % 3];

          if (SegmentIntersectsTriangle(E0, E1, A0, A1, A2))
            return TRUE;
        }
        return FALSE;
      } /* End of 'TrianglesIntersect' function */

    /* Brute-force O(n*m) test in a common frame (no translation).
     * ARGUMENTS:
     *   - first mesh:
     *       const mesh<type> &MeshA;
     *   - second mesh:
     *       const mesh<type> &MeshB;
     * RETURNS:
     *   (BOOL) TRUE if any triangle pair intersects.
     */
    template<typename type>
      BOOL MeshesIntersect( const mesh<type> &MeshA, const mesh<type> &MeshB )
      {
        return MeshesIntersectWithTranslation(MeshA, vec3<type>::Zero(), MeshB, vec3<type>::Zero());
      } /* End of 'MeshesIntersect' function */

    /* Same as MeshesIntersect but each mesh is shifted by a translation.
     * ARGUMENTS:
     *   - first mesh:
     *       const mesh<type> &MeshA;
     *   - first mesh translation:
     *       const vec3<type> &TransA;
     *   - second mesh:
     *       const mesh<type> &MeshB;
     *   - second mesh translation:
     *       const vec3<type> &TransB;
     * RETURNS:
     *   (BOOL) TRUE if any translated triangle pair intersects.
     */
    template<typename type>
      BOOL MeshesIntersectWithTranslation( const mesh<type> &MeshA, const vec3<type> &TransA,
                                           const mesh<type> &MeshB, const vec3<type> &TransB )
      {
        for (const triangle &TriA : MeshA.Triangles)
        {
          if (TriA.V0 < 0 || TriA.V1 < 0 || TriA.V2 < 0 ||
              TriA.V0 >= MeshA.VertexCount() || TriA.V1 >= MeshA.VertexCount() ||
              TriA.V2 >= MeshA.VertexCount())
            continue;
          vec3<type>
            A0 = MeshA.Vertices[TriA.V0] + TransA,
            A1 = MeshA.Vertices[TriA.V1] + TransA,
            A2 = MeshA.Vertices[TriA.V2] + TransA;

          for (const triangle &TriB : MeshB.Triangles)
          {
            if (TriB.V0 < 0 || TriB.V1 < 0 || TriB.V2 < 0 ||
                TriB.V0 >= MeshB.VertexCount() || TriB.V1 >= MeshB.VertexCount() ||
                TriB.V2 >= MeshB.VertexCount())
              continue;
            vec3<type>
              B0 = MeshB.Vertices[TriB.V0] + TransB,
              B1 = MeshB.Vertices[TriB.V1] + TransB,
              B2 = MeshB.Vertices[TriB.V2] + TransB;

            if (TrianglesIntersect(A0, A1, A2, B0, B1, B2))
              return TRUE;
          }
        }
        return FALSE;
      } /* End of 'MeshesIntersectWithTranslation' function */
  } /* end of 'cg' namespace */
} /* end of 'mth' namespace */

#endif /* __mth_cg_triangles_h_ */

/* END OF 'mth_cg_triangles.h' FILE */
