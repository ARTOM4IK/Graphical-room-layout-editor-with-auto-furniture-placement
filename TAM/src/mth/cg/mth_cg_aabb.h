/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_cg_aabb.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math support module.
 *               Computation geometry: axis-aligned bounding box.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 09.04.2026
 * NOTE        : Namespace 'mth::cg'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_cg_aabb_h_
#define __mth_cg_aabb_h_

#include "../mth_vec3.h"

/* 'mth' namespace */
namespace mth
{
  /* Computation geometry: collision tests and acceleration structures */
  namespace cg
  {
    /* Axis-aligned box: minimal and maximal corners per axis */
    template<typename type>
      struct aabb
      {
        vec3<type> Min, Max; /* Corners with smallest/largest points */

        /* Class default constructor */
        aabb( VOID )
        {
        } /* End of 'aabb' function */

        /* Class constructor from minimal and maximal corners.
         * ARGUMENTS:
         *   - minimal/maximal corners:
         *       const vec3<type> &A, &B;
         * RETURNS: None.
         */
        aabb( const vec3<type> &A, const vec3<type> &B ) : Min(A), Max(B)
        {
        } /* End of 'aabb' function */

        /* Check that the box is non-degenerate on every axis function.
         * ARGUMENTS: None.
         * RETURNS:
         *   (BOOL) TRUE if Min <= Max on X, Y and Z.
         */
        BOOL IsValid( VOID ) const
        {
          return Min.X <= Max.X && Min.Y <= Max.Y && Min.Z <= Max.Z;
        } /* End of 'IsValid' function */

        /* Test overlap with another axis-aligned box function.
         * ARGUMENTS:
         *   - other box:
         *       const aabb &Other;
         * RETURNS:
         *   (BOOL) TRUE if volumes overlap.
         */
        BOOL Intersects( const aabb &Other ) const
        {
          if (!IsValid() || !Other.IsValid())
            return FALSE;
          return Min.X <= Other.Max.X && Max.X >= Other.Min.X &&
                 Min.Y <= Other.Max.Y && Max.Y >= Other.Min.Y &&
                 Min.Z <= Other.Max.Z && Max.Z >= Other.Min.Z;
        } /* End of 'Intersects' function */

        /* Grow the box to contain a point; invalid box becomes a point function.
         * ARGUMENTS:
         *   - point to include:
         *       const vec3<type> &P;
         * RETURNS: None.
         */
        VOID Expand( const vec3<type> &P )
        {
          if (!IsValid())
          {
            Min = Max = P;
            return;
          }
          Min.X = COM_MIN(Min.X, P.X);
          Min.Y = COM_MIN(Min.Y, P.Y);
          Min.Z = COM_MIN(Min.Z, P.Z);
          Max.X = COM_MAX(Max.X, P.X);
          Max.Y = COM_MAX(Max.Y, P.Y);
          Max.Z = COM_MAX(Max.Z, P.Z);
        } /* End of 'Expand' function */

        /* Replace this box with the smallest AABB covering both operands function.
         * ARGUMENTS:
         *   - other box:
         *       const aabb &Other;
         * RETURNS: None.
         */
        VOID UnionWith( const aabb &Other )
        {
          if (!Other.IsValid())
            return;
          if (!IsValid())
            *this = Other;
          else
          {
            Min.X = COM_MIN(Min.X, Other.Min.X);
            Min.Y = COM_MIN(Min.Y, Other.Min.Y);
            Min.Z = COM_MIN(Min.Z, Other.Min.Z);
            Max.X = COM_MAX(Max.X, Other.Max.X);
            Max.Y = COM_MAX(Max.Y, Other.Max.Y);
            Max.Z = COM_MAX(Max.Z, Other.Max.Z);
          }
        } /* End of 'UnionWith' function */

        /* Move the box by a translation vector function.
         * ARGUMENTS:
         *   - offset:
         *       const vec3<type> &D;
         * RETURNS: None.
         */
        VOID Translate( const vec3<type> &D )
        {
          Min += D;
          Max += D;
        } /* End of 'Translate' function */
      }; /* End of 'aabb' struct */
  } /* end of 'cg' namespace */
} /* end of 'mth' namespace */

#endif /* __mth_cg_aabb_h_ */

/* END OF 'mth_cg_aabb.h' FILE */
