/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_geom2.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math support module.
 *               2D geometry implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 02.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_geom2_h_
#define __mth_geom2_h_

#include "mth_def.h"
#include "mth_vec2.h"

/* 'mth' namespace */
namespace mth
{
  /* 'obb2' handle class */
  template<typename type>
    class obb2
    {
    public:
      vec2<type> Center {};   // center
      vec2<type> AxisX;       // X axis
      vec2<type> AxisY;       // Y axis
      vec2<type> HalfSize {}; // half size

      /* type constructor.
       * ARGUMENTS:
       *   - center vector:
       *       vec2<type> NewCenter;
       *   - axis X and axis Y vectors:
       *       vec2<type> NewAxisX, vec2<type> NewAxisY;
       *   - half size:
       *       vec2<type> NewHalfSize;
       * RETURNS: None.
       */
      obb2( vec2<type> NewCenter = vec2<type>::Zero(), vec2<type> NewAxisX = vec2<type>(1, 0),
        vec2<type> NewAxisY = vec2<type>(0, 1), vec2<type> NewHalfSize = vec2<type>::Zero() ) :
        Center(NewCenter), AxisX(NewAxisX), AxisY(NewAxisY), HalfSize(NewHalfSize)
      {
      } /* End of 'obb2' function */

      /* type constructor.
       * ARGUMENTS:
       *   - center vector:
       *       vec2<type> NewCenter;
       *   - full size:
       *       vec2<type> Size;
       *   - angle in degrees:
       *       type Angle;
       * RETURNS: None.
       */
      obb2( vec2<type> NewCenter, vec2<type> Size, type Angle ) :
        Center(NewCenter), HalfSize(Size / (type)2.0)
      {
        type a = (type)D2R(Angle);
        vec2<type> NewAxisX = vec2<type>(cos(a), sin(a));
        vec2<type> NewAxisY = NewAxisX.LeftNormal();

        AxisX = NewAxisX.Normalizing();
        AxisY = NewAxisY.Normalizing();
      } /* End of 'obb2' function */

      /* Get OBB corner points function.
       * ARGUMENTS:
       *   - resulting corners:
       *       vec2<type> Out[4];
       * RETURNS: None.
       */
      VOID GetCorners( vec2<type> Out[4] ) const
      {
        vec2<type> X = AxisX * HalfSize.X;
        vec2<type> Y = AxisY * HalfSize.Y;

        Out[0] = Center - X - Y;
        Out[1] = Center + X - Y;
        Out[2] = Center + X + Y;
        Out[3] = Center - X + Y;
      } /* End of 'GetCorners' function */

      /* Project OBB on axis function.
       * ARGUMENTS:
       *   - OBB:
       *       const obb2 &Box;
       *   - axis:
       *       const vec2<type> &Axis;
       * RETURNS:
       *   (vec2<type>) projection interval.
       */
      vec2<type> ProjectOBB( const obb2 &Box, const vec2<type> &Axis ) const
      {
        vec2<type> corners[4];
        type MinP, MaxP;

        Box.GetCorners(corners);
        MinP = MaxP = corners[0] & Axis;
        for (INT i = 1; i < 4; i++)
        {
          type p = corners[i] & Axis;
          MinP = Min(MinP, p);
          MaxP = Max(MaxP, p);
        }
        return vec2<type>(MinP, MaxP);
      } /* End of 'ProjectOBB' function */

      /* Check if OBB intersects OBB function.
       * ARGUMENTS:
       *   - OBBs:
       *       const obb2 &A, const obb2 &B;
       * RETURNS:
       *   (BOOL) intersection flag.
       */
      BOOL IsOBBIntersectOBB( const obb2 &A, const obb2 &B ) const
      {
        vec2<type> axes[4] = {A.AxisX, A.AxisY, B.AxisX, B.AxisY};

        for (auto &axis : axes)
        {
          auto [amin, amax] = ProjectOBB(A, axis);
          auto [bmin, bmax] = ProjectOBB(B, axis);

          if (amax < bmin - Threshold || bmax < amin - Threshold)
            return FALSE;
        }
        return TRUE;
      } /* End of 'IsOBBIntersectOBB' function */

      /* Check if point is inside OBB function.
       * ARGUMENTS:
       *   - OBB:
       *       const obb2 &Box;
       *   - point:
       *       const vec2<type> &Point;
       * RETURNS:
       *   (BOOL) inside flag.
       */
      BOOL IsPointInOBB( const obb2 &Box, const vec2<type> &Point ) const
      {
        vec2<type> Local = Point - Box.Center;
        type X = fabs(Local & Box.AxisX);
        type Y = fabs(Local & Box.AxisY);

        return X <= Box.HalfSize.X + Threshold && Y <= Box.HalfSize.Y + Threshold;
      } /* End of 'IsPointInOBB' function */

      /* Check if OBB is fully inside polygon function.
       * ARGUMENTS:
       *   - OBB:
       *       const obb2 &Box;
       *   - polygon:
       *       const std::vector<vec2<type>> &Polygon;
       * RETURNS:
       *   (BOOL) inside flag.
       */
      BOOL IsOBBInsidePolygon( const obb2 &Box, const std::vector<vec2<type>> &Polygon ) const
      {
        vec2<type> corners[4];

        if (Polygon.size() < 3)
          return FALSE;

        Box.GetCorners(corners);

        for (INT i = 0; i < 4; i++)
          if (!vec2<type>::Zero().IsPointInPolygon(Polygon, corners[i]))
            return FALSE;

        for (INT i = 0; i < 4; i++)
        {
          INT ni = (i + 1) % 4;

          for (INT j = 0; j < (INT)Polygon.size(); j++)
          {
            INT nj = (j + 1) % (INT)Polygon.size();

            if (vec2<type>::Zero().IsSegmentIntersectSegment(corners[i], corners[ni], Polygon[j], Polygon[nj]))
              if (!vec2<type>::Zero().IsPointOnSegment(corners[i], Polygon[j], Polygon[nj]) &&
                  !vec2<type>::Zero().IsPointOnSegment(corners[ni], Polygon[j], Polygon[nj]))
                return FALSE;
          }
        }
        return TRUE;
      } /* End of 'IsOBBInsidePolygon' function */

      /* Check if OBB intersects polygon function.
       * ARGUMENTS:
       *   - OBB:
       *       const obb2 &Box;
       *   - polygon:
       *       const std::vector<vec2<type>> &Polygon;
       * RETURNS:
       *   (BOOL) intersection flag.
       */
      BOOL IsOBBIntersectPolygon( const obb2 &Box, const std::vector<vec2<type>> &Polygon ) const
      {
        vec2<type> corners[4];

        if (Polygon.size() < 3)
          return FALSE;

        Box.GetCorners(corners);

        for (INT i = 0; i < 4; i++)
          if (vec2<type>::Zero().IsPointInPolygon(Polygon, corners[i]))
            return TRUE;

        for (auto &P : Polygon)
          if (this->IsPointInOBB(Box, P))
            return TRUE;

        for (INT i = 0; i < 4; i++)
        {
          INT ni = (i + 1) % 4;

          for (INT j = 0; j < (INT)Polygon.size(); j++)
          {
            INT nj = (j + 1) % (INT)Polygon.size();

            if (vec2<type>::Zero().IsSegmentIntersectSegment(corners[i], corners[ni], Polygon[j], Polygon[nj]))
              return TRUE;
          }
        }
        return FALSE;
      } /* End of 'IsOBBIntersectPolygon' function */
    }; /* End of 'obb2' class */
} /* end of 'mth' namespace */

#endif /* __mth_geom2_h_ */

/* END OF 'mth_geom2.h' FILE */
