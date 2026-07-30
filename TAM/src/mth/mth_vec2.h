/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_vec2.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math support module.
 *               2D vector module implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 02.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_vec2_h_
#define __mth_vec2_h_

#include "mth_def.h"

/* 'mth' namespace */
namespace mth
{
  /* 'vec2' handle class */
  template<typename type>
    class vec2
    {
    public:
      type X, Y; // coordinates of vector

      /* Class default constructor */
      vec2( VOID )
      {
      } /* End of 'vec2' function */

      /* type constructor.
       * ARGUMENTS:
       *   - coordinates:
       *       type x, y;
       * RETURNS: None.
       */
      vec2( type x, type y ) : X(x), Y(y)
      {
      } /* End of 'vec2' function */

      /* type constructor.
       * ARGUMENTS:
       *   - coordinate:
       *       type N;
       * RETURNS: None.
       */
      explicit vec2( type N ) : X(N), Y(N)
      {
      } /* End of 'vec2' function */

      /* Add two vectors function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec2 &V2;
       * RETURNS:
       *   (vec2) result vector.
       */
      vec2<type> operator+( const vec2<type> &V2 ) const
      {
        return vec2(X + V2.X, Y + V2.Y);
      } /* End of 'operator+' function */

      /* Add vector to first vector function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec2 &V2;
       * RETURNS:
       *   (&vec2) result vector.
       */
      vec2<type> & operator+=( const vec2<type> &V2 )
      {
        X += V2.X;
        Y += V2.Y;
        return *this;
      } /* End of 'operator+=' function */

      /* Subtract one vector from another function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec2 &V2;
       * RETURNS:
       *   (vec2) result vector.
       */
      vec2<type> operator-( const vec2<type> &V2 ) const
      {
        return vec2(X - V2.X, Y - V2.Y);
      } /* End of 'operator-' function */

      /* Obtain negated vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2) result vector.
       */
      vec2<type> operator-( VOID ) const
      {
        return vec2(-X, -Y);
      } /* End of 'operator-' function */

      /* Subtract vector from first vector function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec2 &V2;
       * RETURNS:
       *   (vec2 &) result vector.
       */
      vec2<type> & operator-=( const vec2 &V2 )
      {
        X -= V2.X;
        Y -= V2.Y;
        return *this;
      } /* End of 'operator-=' function */

      /* Multiply vector on a number function.
       * ARGUMENTS:
       *   - multiplier:
       *       type N;
       * RETURNS:
       *   (vec2) result vector.
       */
      vec2<type> operator*( type N ) const
      {
        return vec2(X * N, Y * N);
      } /* End of 'operator*' function */

      /* Subtract vector from first vector function.
       * ARGUMENTS:
       *   - multiplier:
       *       type N;
       * RETURNS:
       *   (vec2 &) result vector.
       */
      vec2<type> & operator*=( type N )
      {
        X *= N;
        Y *= N;
        return *this;
      } /* End of 'operator*=' function */

      /* Multiply vectors function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec3 &V2;
       * RETURNS:
       *   (vec2) result vector.
       */
      vec2<type> operator*( const vec2 &V2 ) const
      {
        return vec2(X * V2.X, Y * V2.Y);
      } /* End of 'operator*' function */

      /* Multiply vectors function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec2 &V2;
       * RETURNS:
       *   (vec2 &) result vector.
       */
      vec2 & operator*=( const vec2 &V2 )
      {
        X *= V2.X;
        Y *= V2.Y;
        return *this;
      } /* End of 'operator*=' function */

      /* Divide vector by a number function.
       * ARGUMENTS:
       *   - divisor:
       *       type N;
       * RETURNS:
       *   (vec2) result vector.
       */
      vec2<type> operator/( type N ) const
      {
        if (N == 0)
          return vec2(0);
        return vec2(X / N, Y / N);
      } /* End of 'operator*' function */

      /* Divide vector by a number function.
       * ARGUMENTS:
       *   - divisor:
       *       type N;
       * RETURNS:
       *   (vec2 &) result vector.
       */
      vec2 & operator/=( type N )
      {
        if (N == 0)
          X = 0, Y = 0;
        else
          X /= N, Y /= N;
        return *this;
      } /* End of 'operator/=' function */

      /* Divide vector by a vector function.
       * ARGUMENTS:
       *   - divisor vector:
       *       const vec &V2;
       * RETURNS:
       *   (vec2) result vector.
       */
      vec2<type> operator/( const vec2 &V2 ) const
      {
        if (V2.X == 0 || V2.Y == 0)
          return vec2(0);
        return vec2(X / V2.X, Y / V2.Y);
      } /* End of 'operator/' function */

      /* Divide vector by a vector function.
       * ARGUMENTS:
       *   - divisor vector:
       *       const vec2 &V2;
       * RETURNS:
       *   (vec2 &) result vector.
       */
      vec2<type> & operator/=( const vec2 &V2 )
      {
        if (V2.X == 0 || V2.Y == 0)
          X = 0, Y = 0;
        else
          X /= V2.X, Y /= V2.Y;
        return *this;
      } /* End of 'operator/=' function */

      /* Get length of vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (type) result length.
       */
      type operator!( VOID ) const
      {
        type len = *this & *this;
        if (len != 0 && len != 1)
          if constexpr (std::is_same_v<type, FLT>)
            return sqrtf(len);
          else
            return sqrt(len);
        return len;
      } /* End of 'operator!' function */

      /* Dot product two vectors function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec2 &V2;
       * RETURNS:
       *   (type) result of dot production.
       */
      type operator&( const vec2 &V2 ) const
      {
        return X * V2.X + Y * V2.Y;
      } /* End of 'operator&' function */

      /* Cross product two vectors function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec2 &V2;
       * RETURNS:
       *   (type) result of cross production.
       */
      type operator%( const vec2 &V2 ) const
      {
        return X * V2.Y - Y * V2.X;
      } /* End of 'operator%' function */

      /* Get left normal function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2<type>) left normal of this vector.
       */
      vec2<type> LeftNormal( VOID ) const
      {
        return vec2(-Y, X);
      } /* End of 'LeftNormal' function */

      /* Rotate vector function.
       * ARGUMENTS:
       *   - angle in degrees:
       *       const type &Angle;
       * RETURNS:
       *   (vec2<type>) rotated vector.
       */
      vec2<type> Rotate( const type &Angle ) const
      {
        type a = (type)D2R(Angle);
        type si = sin(a), co = cos(a);

        return vec2(X * co - Y * si, X * si + Y * co);
      } /* End of 'Rotate' function */

      /* Normalize vector function.
       * ARGUMENTS: none.
       * RETURNS:
       *   (vec2 &) normalized vector.
       */
      vec2<type> & Normalize( VOID )
      {
        *this /= !*this;
        return *this;
      } /* End of 'Normalize' function */

      /* Get normalized vector function.
       * ARGUMENTS: none.
       * RETURNS:
       *   (vec2) normalized vector.
       */
      vec2<type> Normalizing( VOID ) const
      {
        return *this / !*this;
      } /* End of 'Normalizing' function */

      /* Get square of length of vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (type) result length.
       */
      type Length2( VOID ) const
      {
        return X * X + Y * Y;
      } /* End of 'Length2' function */

      /* Get distance between two vectors function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec2 &V2;
       * RETURNS:
       *   (type) result distance.
       */
      type Distance( const vec2 &V2 ) const
      {
        return !(*this - V2);
      } /* End of 'Distance' function */

      /* Get zero vector function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      static vec2<type> Zero( VOID )
      {
        return vec2(0);
      } /* End of 'Zero' function */

      /* Get random vector between 0 and 1 function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2) result random vec.
       */
      static vec2 Rnd0( VOID )
      {
        return vec2(DBL(rand() / RAND_MAX),
                    DBL(rand() / RAND_MAX));
      } /* End of 'Rnd0' function */

      /* Get random vector between -1 and 1 function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2) result random vec3.
       */
      static vec2 Rnd1( VOID )
      {
        return vec2(DBL(rand() / RAND_MAX) * 2 - 1,
                    DBL(rand() / RAND_MAX) * 2 - 1);
      } /* End of 'Rnd1' function */

      /* Get vector coordinate by index function.
       * ARGUMENTS:
       *   - index of coordinate:
       *       INT N;
       * RETURNS:
       *   (&type) result link to coordinate.
       */
      type & operator[]( INT N )
      {
        if (N <= 0)
          return X;
        if (N >= 1)
          return Y;
      } /* End of 'operator[]' function */

      /* Get pointer to coordinates function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (type *) pointer to first coordinate.
       */
      operator type *( VOID )
      {
        return &X;
      } /* End of 'operator type *' function */

      /* Find intersection of two segments function.
       * ARGUMENTS:
       *   - two lines edge coordinates:
       *       vec2<type> P1, P2, Q1, Q2;
       * RETURNS:
       *   (vec2<type>) intersection of two lines.
       */
      static vec2<type> LinesIntersection( vec2<type> P1, vec2<type> P2, vec2<type> Q1, vec2<type> Q2 )
      {
        type xp1 = P1.X, yp1 = P1.Y, xp2 = P2.X, yp2 = P2.Y, xq1 = Q1.X, yq1 = Q1.Y, xq2 = Q2.X, yq2 = Q2.Y, t1, t2;
        if (P1 == P2 || Q1 == Q2)
          return vec2<type>(0);
        if (xq1 == xq2 && xp1 == xp2)
          return vec2<type>(0);
        if (yq1 == yq2 && yp1 == yp2)
          return vec2<type>(0);
        // if (xq1 == xq2)
        // {
        //   t1 = (xq1 - xp1) / (xp2 - xp1);
        //   t2 = (yp1 - yq1 + (yp2 - yp1) * t1) / (yq2 - yq1);
        // }
        // else if (xp1 == xp2)
        // {
        //   t2 = (xp1 - xq1) / (xq2 - xq1);
        //   t1 = (yq1 - yp1 + (yq2 - yq1) * t2) / (yp2 - yp1);
        // }
        // else if (yq1 == yq2)
        // {
        //   t1 = (yq1 - yp1) / (yp2 - yp1);
        //   t2 = (xp1 - xq1 + (xp2 - xp1) * t1) / (xq2 - xq1);
        // }
        // else if (yp1 == yp2)
        // {
        //   t2 = (yp1 - yq1) / (yq2 - yq1);
        //   t1 = (xq1 - xp1 + (xq2 - xq1) * t2) / (xp2 - xp1);
        // }
        else
        {
          //t2 = ((xp2 - xp1) * (xq1 - xp1) - (yp2 - yp1) * (yq1 - yp1)) / ((yp2 - yp1) * (yq2 - yq1) - (xp2 - xp1) * (xq2 - xq1));
          t2 = ( (xp2 - xp1) * (yq1 - yp1) - (yp2 - yp1) * (xq1 - xp1) ) / ( (xq2 - xq1) * (yp2 - yp1) - (yq2 - yq1) * (xp2 - xp1) );
          t1 = (xq1 - xp1 + (xq2 - xq1) * t2) / (xp2 - xp1);
        }
        return vec2<type>(xq1 + (xq2 - xq1) * t2, yq1 + (yq2 - yq1) * t2);
      } /* End of 'LinesIntersection' function */

      /* Find intersection of two segments function.
       * ARGUMENTS:
       *   - two segments edge coordinates:
       *       vec2<type> P1, P2, Q1, Q2;
       * RETURNS:
       *   (vec2<type>) intersection of two segments.
       */
      static vec2<type> SegmentsIntersection( vec2<type> P1, vec2<type> P2, vec2<type> Q1, vec2<type> Q2 )
      {
        type xp1 = P1.X, yp1 = P1.Y, xp2 = P2.X, yp2 = P2.Y, xq1 = Q1.X, yq1 = Q1.Y, xq2 = Q2.X, yq2 = Q2.Y, t1, t2;
        if (P1 == P2 || Q1 == Q2)
          return vec2<type>(0);
        if (xq1 == xq2 && xp1 == xp2)
          return vec2<type>(0);
        if (yq1 == yq2 && yp1 == yp2)
          return vec2<type>(0);
        if (xq1 == xq2)
        {
          t1 = (xq1 - xp1) / (xp2 - xp1);
          t2 = (yp1 - yq1 + (yp2 - yp1) * t1) / (yq2 - yq1);
        }
        else if (xp1 == xp2)
        {
          t2 = (xp1 - xq1) / (xq2 - xq1);
          t1 = (yq1 - yp1 + (yq2 - yq1) * t2) / (yp2 - yp1);
        }
        else if (yq1 == yq2)
        {
          t1 = (yq1 - yp1) / (yp2 - yp1);
          t2 = (xp1 - xq1 + (xp2 - xp1) * t1) / (xq2 - xq1);
        }
        else if (yp1 == yp2)
        {
          t2 = (yp1 - yq1) / (yq2 - yq1);
          t1 = (xq1 - xp1 + (xq2 - xq1) * t2) / (xp2 - xp1);
        }
        else
        {
          t2 = ((xp2 - xp1) * (xq1 - xp1) - (yp2 - yp1) * (yq1 - yp1)) / ((yp2 - yp1) * (yq2 - yq1) - (xp2 - xp1) * (xq2 - xq1));
          t1 = (xq1 - xp1 + (xq2 - xq1) * t2) / (xp2 - xp1);
        }
        if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1)
          return vec2<type>(xp1 + (xp2 - xp1) * t1, yp1 + (yp2 - yp1) * t1);
        return vec2<type>(0);
      } /* End of 'SegmentsIntersection' function */

      /* Check intersection between ray aand segment function.
       * ARGUMENTS:
       *   - origin and point on ray:
       *       const vec2<type> &P1, &P2;
       *   - edge points of segment:
       *       const vec2<type> &Q1, &Q2;
       * RETURNS:
       *   (BOOL) TRUE if ray intersects segment, FALSE otherwise.
       */
      static BOOL CheckRaySegmentIntersection( const vec2<type> &P1, const vec2<type> &P2,
                                               const vec2<type> &Q1, const vec2<type> &Q2 )
      {
        const type Eps = 1e-6;
        if (vec2<type>::Compare(P1, P2, Eps) || vec2<type>::Compare(Q1, Q2, Eps))
          return FALSE;

        vec2<type> D = P2 - P1;
        vec2<type> E = Q2 - Q1;

        type Cross = D.X * E.Y - D.Y * E.X;

        if (fabs(Cross) < Eps)
        {
          vec2<type> Q1P1 = Q1 - P1;
          type Cross2 = D.X * Q1P1.Y - D.Y * Q1P1.X;
          if (fabs(Cross2) >= Eps)
            return FALSE;

          type Dot = D & D;
          if (fabs(Dot) < Eps)
            return FALSE;

          type T1 = (Q1 - P1) & D, T2 = (Q2 - P1) & D;

          type TMin = COM_MIN(T1, T2), TMax = COM_MAX(T1, T2);

          return TMax >= -Eps;
        }
        vec2<type> Q1P1 = Q1 - P1;
        type T = (Q1P1.X * E.Y - Q1P1.Y * E.X) / Cross;
        type S = (Q1P1.X * D.Y - Q1P1.Y * D.X) / Cross;
        return (T >= -Eps && S >= -Eps && S <= 1.0 + Eps);
      } /* End of 'CheckRaySegmentIntersection' function */

      /* Compare two vectors function.
       * ARGUMENTS:
       *   - vecctors to compare:
       *       const vec2 &V1, &V2;
       *   - square threshold of camparing:
       *       type Threshold2;
       * RETURNS:
       *   (BOOL) TRUE if vectors are identical, FALSE otherwise.
       */
      static BOOL Compare( const vec2<type> &V1, const vec2<type> &V2, type Threshold2 )
      {
        if ((V1 - V2).Length2() < Threshold2)
          return TRUE;
        return FALSE;
      } /*  End of 'Compare' function */

      /* Find distance between point and segment function.
       * ARGUMENTS:
       *   - point:
       *       vec2<type> P;
       *   - segment edge coordinates:
       *       vec2<type> P1, P2;
       * RETURNS:
       *   (vec2<type>) distance between point and a segment.
       */
      static type SegmentDistance( vec2<type> P, vec2<type> P1, vec2<type> P2 )
      {
        type x1 = P1.X, y1 = P1.Y, x2 = P2.X, y2 = P2.Y, xp = P.X, yp = P.Y, t;
        if (x1 == x2 && y1 == y2)
          return !(P1 - P);
        t = ((x2 - x1) * (xp - x1) + (y2 - y1) * (yp - y1)) / ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        if (t >= 0 && t <= 1)
        {
          type
            xq = x1 + (x2 - x1) * t,
            yq = y1 + (y2 - y1) * t;
          return !vec2(xq - xp, yq - yp);
        }
        return COM_MIN(!vec2(x1 - xp, y1 - yp), !vec2(x2 - xp, y2 - yp));
      } /* End of 'SegmentDistance' function */

      /* Get nearest point to needed on segment function.
       * ARGUMENTS:
       *   - point to search closest point to:
       *       const vec2 &SearchPoint;
       *   - first segment point:
       *       const vec2 &P1;
       *   - second segment point:
       *       const vec2 &P2;
       * RETURNS:
       *   (vec2<type>) found point.
       */
      static vec2<type> GetNearestPointOnSegment( const vec2<type> &SearchPoint, const vec2<type> &P1, const vec2<type> &P2 )
      {
        vec2<type> P1P2 = P2 - P1;
        type t = (SearchPoint - P1) & P1P2 / P1P2.Length2();
        if (t <= 0)
          return P1;
        else if (t >= 1)
          return P2;
        else
          return P1 + P1P2 * t;
      } /* End of 'GetNearestPointOnSegment' function */

      /* Get signed polygon area function.
       * ARGUMENTS:
       *   - polygon:
       *       const std::vector<vec2<type>> &Polygon;
       * RETURNS:
       *   (type) signed polygon area.
       */
      type SignedPolygonArea( const std::vector<vec2<type>> &Polygon )
      {
        type Area = 0;

        if (Polygon.size() < 3)
          return 0;
        for (INT i = 0; i < (INT)Polygon.size(); i++)
        {
          INT ni = (i + 1) % (INT)Polygon.size();
          Area += Polygon[i] % Polygon[ni];
        }
        return Area * 0.5;
      } /* End of 'SignedPolygonArea' function */

      /* Ensure polygon has counter-clockwise order function.
       * ARGUMENTS:
       *   - polygon:
       *       std::vector<vec2<type>> &Polygon;
       * RETURNS: None.
       */
      VOID EnsureCCW( std::vector<vec2<type>> &Polygon )
      {
        if (SignedPolygonArea(Polygon) < 0)
          std::reverse(Polygon.begin(), Polygon.end());
      } /* End of 'EnsureCCW' function */

      /* Get polygon center function.
       * ARGUMENTS:
       *   - polygon:
       *       const std::vector<vec2<type>> &Polygon;
       * RETURNS:
       *   (vec2<type>) polygon center.
       */
      vec2<type> PolygonCenter( const std::vector<vec2<type>> &Polygon )
      {
        vec2<type> C = Zero();

        if (Polygon.empty())
          return C;
        for (auto &P : Polygon)
          C += P;
        return C / (type)Polygon.size();
      } /* End of 'PolygonCenter' function */

      /* Check if point is on segment function.
       * ARGUMENTS:
       *   - point:
       *       const vec2<type> &P;
       *   - segment edge points:
       *       const vec2<type> &A, const vec2<type> &B;
       * RETURNS:
       *   (BOOL) point-on-segment flag.
       */
      BOOL IsPointOnSegment( const vec2<type> &P, const vec2<type> &A, const vec2<type> &B )
      {
        if (fabs((B - A) % (P - A)) > 1e-4)
          return FALSE;
        return P.X >= mth::Min(A.X, B.X) - 1e-4 &&
          P.X <= mth::Max(A.X, B.X) + 1e-4 &&
          P.Y >= mth::Min(A.Y, B.Y) - 1e-4 &&
          P.Y <= mth::Max(A.Y, B.Y) + 1e-4;
      } /* End of 'IsPointOnSegment' function */

      /* Check if segment intersects segment function.
       * ARGUMENTS:
       *   - first segment:
       *       const vec2<type> &A1, const vec2<type> &A2;
       *   - second segment:
       *       const vec2<type> &B1, const vec2<type> &B2;
       * RETURNS:
       *   (BOOL) intersection flag.
       */
      BOOL IsSegmentIntersectSegment( const vec2<type> &A1, const vec2<type> &A2,
                                      const vec2<type> &B1, const vec2<type> &B2 )
      {
        type C1 = (A2 - A1) % (B1 - A1);
        type C2 = (A2 - A1) % (B2 - A1);
        type C3 = (B2 - B1) % (A1 - B1);
        type C4 = (B2 - B1) % (A2 - B1);

        if (IsPointOnSegment(A1, B1, B2) || IsPointOnSegment(A2, B1, B2) ||
            IsPointOnSegment(B1, A1, A2) || IsPointOnSegment(B2, A1, A2))
          return TRUE;
        return ((C1 > 0 && C2 < 0) || (C1 < 0 && C2 > 0)) &&
          ((C3 > 0 && C4 < 0) || (C3 < 0 && C4 > 0));
      } /* End of 'IsSegmentIntersectSegment' function */

      /* Check if point is inside polygon function.
       * ARGUMENTS:
       *   - polygon:
       *       const std::vector<vec2<type>> &Polygon;
       *   - point:
       *       const vec2<type> &Point;
       * RETURNS:
       *   (BOOL) inside flag.
       */
      BOOL IsPointInPolygon( const std::vector<vec2<type>> &Polygon, const vec2<type> &Point )
      {
        BOOL Inside = FALSE;

        if (Polygon.size() < 3)
          return FALSE;

        for (INT i = 0, j = (INT)Polygon.size() - 1; i < (INT)Polygon.size(); j = i++)
        {
          const vec2<type> &Pi = Polygon[i];
          const vec2<type> &Pj = Polygon[j];

          if (IsPointOnSegment(Point, Pj, Pi))
            return TRUE;
          if (((Pi.Y > Point.Y) != (Pj.Y > Point.Y)) &&
              (Point.X < (Pj.X - Pi.X) * (Point.Y - Pi.Y) / ((Pj.Y - Pi.Y) + 1e-12) + Pi.X))
            Inside = !Inside;
        }
        return Inside;
      } /* End of 'IsPointInPolygon' function */

      /* Get distance from point to polygon edges function.
       * ARGUMENTS:
       *   - polygon:
       *       const std::vector<vec2<type>> &Polygon;
       *   - point:
       *       const vec2<type> &Point;
       * RETURNS:
       *   (type) minimal edge distance.
       */
      type DistanceToPolygonEdges( const std::vector<vec2<type>> &Polygon, const vec2<type> &Point )
      {
        type Dist = (type)DBL_MAX;

        if (Polygon.size() < 2)
          return Dist;
        for (INT i = 0; i < (INT)Polygon.size(); i++)
        {
          INT ni = (i + 1) % (INT)Polygon.size();
          Dist = mth::Min(Dist, SegmentDistance(Point, Polygon[i], Polygon[ni]));
        }
        return Dist;
      } /* End of 'DistanceToPolygonEdges' function */
    }; /* End of 'vec2' class */
} /* end of 'mth' namespace */

#endif /* __mth_vec2_h_ */

/* END OF 'mth_vec2.h' FILE */
