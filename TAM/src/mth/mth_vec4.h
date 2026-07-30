/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_vec4.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math support module.
 *               4D vector module implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 05.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_vec4_h_
#define __mth_vec4_h_

#include "mth_def.h"

/* 'mth' namespace */
namespace mth
{
  /* 'vec4' handle class */
  template<typename type>
    class vec4
    {
    public:
      type X, Y, Z, W; // vec4 coordinates

      /* Class default constructor */
      vec4( VOID ) // : X(0), Y(0), Z(0), W(0)
      {
      } /* End of 'vec4' function */

      /* Type constructor.
       * ARGUMENTS:
       *   - coordinates:
       *       type x, y, z, w;
       * RETURNS: None.
       */
      vec4( type x, type y, type z, type w ) : X(x), Y(y), Z(z), W(w)
      {
      } /* End of 'vec4' function */

      /* Type constructor.
       * ARGUMENTS:
       *   - 3-D vector:
       *       vec3 V;
       *   - W-coordinate:
       *       type A;
       */
      vec4( vec3<type> V, type A = 0 ) : X(V.X), Y(V.Y), Z(V.Z), W(A)
      {
      } /* End of 'vec4' function */

      /* Type constructor.
       * ARGUMENTS:
       *   - coordinate:
       *       type N;
       * RETURNS: None.
       */
      explicit vec4( type N ) : X(N), Y(N), Z(N), W(N)
      {
      } /* End of 'vec4' function */

      /* Add two vectors function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec4 &V2;
       * RETURNS:
       *   (vec4) result vector.
       */
      vec4<type> operator+( const vec4 &V2 ) const
      {
        return vec4(X + V2.X, Y + V2.Y, Z + V2.Z, W + V2.W);
      } /* End of 'operator+' function */

      /* Add vector to first vector function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec4 &V2;
       * RETURNS:
       *   (&vec4) result vector.
       */
      vec4<type> & operator+=( const vec4 &V2 )
      {
        X += V2.X;
        Y += V2.Y;
        Z += V2.Z;
        W += V2.W;
        return *this;
      } /* End of 'operator+=' function */

      /* Subtract one vector from another function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec4 &V2;
       * RETURNS:
       *   (vec4) result vector.
       */
      vec4<type> operator-( const vec4 &V2 ) const
      {
        return vec4(X - V2.X, Y - V2.Y, Z - V2.Z, W - V2.W);
      } /* End of 'operator-' function */

      /* Obtain negated vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec4) result vector.
       */
      vec4<type> operator-( VOID ) const
      {
        return vec4(-X, -Y, -Z, -W);
      } /* End of 'operator-' function */

      /* Subtract vector from first vector function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec4 &V2;
       * RETURNS:
       *   (&vec4) result vector.
       */
      vec4<type> & operator-=( const vec4 &V2 )
      {
        X -= V2.X;
        Y -= V2.Y;
        Z -= V2.Z;
        W -= V2.W;
        return *this;
      } /* End of 'operator-=' function */

      /* Multiply vector on a number function.
       * ARGUMENTS:
       *   - multiplier:
       *       const type &N;
       * RETURNS:
       *   (vec4) result vector.
       */
      vec4<type> operator*( const type &N ) const
      {
        return vec4(X * N, Y * N, Z * N, W * N);
      } /* End of 'operator*' function */

      /* Subtract vector from first vector function.
       * ARGUMENTS:
       *   - multiplier:
       *       const type &N;
       * RETURNS:
       *   (&vec4<type>) result vector.
       */
      vec4<type> & operator*=( const type &N )
      {
        X *= N;
        Y *= N;
        Z *= N;
        W *= N;
        return *this;
      } /* End of 'operator*=' function */

      /* Multiply vectors function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec &V2;
       * RETURNS:
       *   (vec4<type>) result vector.
       */
      vec4<type> operator*( const vec4 &V2 ) const
      {
        return vec4<type>(X * V2.X, Y * V2.Y, Z * V2.Z, W * V2.W);
      } /* End of 'operator*' function */

      /* Multiply vectors function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec &V2;
       * RETURNS:
       *   (&vec4<type>) result vector.
       */
      vec4<type> & operator*=( const vec4 &V2 )
      {
        X *= V2.X;
        Y *= V2.Y;
        Z *= V2.Z;
        W *= V2.W;
        return *this;
      } /* End of 'operator*=' function */

      /* Divide vector by a number function.
       * ARGUMENTS:
       *   - divisor:
       *       const type &N;
       * RETURNS:
       *   (vec4<type>) result vector.
       */
      vec4<type> operator/( const type &N ) const
      {
        if (N == 0)
          return vec4(0);
        return vec4<type>(X / N, Y / N, Z / N, W / N);
      } /* End of 'operator*' function */

      /* Divide vector by a number function.
       * ARGUMENTS:
       *   - divisor:
       *       const type &N;
       * RETURNS:
       *   (&vec4<type>) result vector.
       */
      vec4<type> & operator/=( const type &N )
      {
        if (N == 0)
          X = 0, Y = 0, Z = 0, W = 0;
        else
          X /= N, Y /= N, Z /= N, W /= N;
        return *this;
      } /* End of 'operator/=' function */

      /* Divide vector by a vector function.
       * ARGUMENTS:
       *   - divisor vector:
       *       const vec &V2;
       * RETURNS:
       *   (vec4<type>) result vector.
       */
      vec4<type> operator/( const vec4 &V2 ) const
      {
        if (V2.X == 0 || V2.Y == 0 || V2.Z == 0 || V2.W == 0)
          return vec4(0);
        return vec4<type>(X / V2.X, Y / V2.Y, Z / V2.Z, W / V2.W);
      } /* End of 'operator/' function */

      /* Divide vector by a vector function.
       * ARGUMENTS:
       *   - divisor vector:
       *       const vec4 &V2;
       * RETURNS:
       *   (&vec4<type>) result vector.
       */
      vec4<type> & operator/=( const vec4 &V2 )
      {
        if (V2.X == 0 || V2.Y == 0 || V2.Z == 0 || V2.W == 0)
          X = 0, Y = 0, Z = 0, W = 0;
        else
          X /= V2.X, Y /= V2.Y, Z /= V2.Z, W /= V2.W;
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
       *       const vec4 &V2;
       * RETURNS:
       *   (type) result of dot production.
       */
      type operator&( const vec4 &V2 ) const
      {
        return X * V2.X + Y * V2.Y + Z * V2.Z + W * V2.W;
      } /* End of 'operator&' function */

      /* Normalize vector function.
       * ARGUMENTS: none.
       * RETURNS:
       *   (&vec4<type>) normalized vector.
       */
      vec4<type> & Normalize( VOID )
      {
        *this /= !*this;
        return *this;
      } /* End of 'Normalize' function */

      /* Get normalized vector function.
       * ARGUMENTS: none.
       * RETURNS:
       *   (vec4<type>) normalized vector.
       */
      vec4<type> Normalizing( VOID ) const
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
        return X * X + Y * Y + Z * Z + W * W;
      } /* End of 'Length2' function */

      /* Get distance between two vectors function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec4 &V2;
       * RETURNS:
       *   (type) result distance.
       */
      type Distance( const vec4 &V2 ) const
      {
        return !(*this - V2);
      } /* End of 'Distance' function */

      /* Get zero vector function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      static vec4 Zero( VOID )
      {
        return vec4(0);
      } /* End of 'Zero' function */

      /* Get random vector between 0 and 1 function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec4) result random vec4.
       */
      static vec4 Rnd0( VOID )
      {
        return vec4(DBL(rand() / RAND_MAX),
                    DBL(rand() / RAND_MAX),
                    DBL(rand() / RAND_MAX),
                    DBL(rand() / RAND_MAX));
      } /* End of 'Rnd0' function */

      /* Get random vector between -1 and 1 function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec4) result random vec4.
       */
      static vec4 Rnd1( VOID )
      {
        return vec4(DBL(rand() / RAND_MAX) * 2 - 1,
                    DBL(rand() / RAND_MAX) * 2 - 1,
                    DBL(rand() / RAND_MAX) * 2 - 1,
                    DBL(rand() / RAND_MAX) * 2 - 1);
      } /* End of 'Rnd1' function */

      /* Get vector coordinate reference by index function.
       * ARGUMENTS:
       *   - index of coordinate:
       *       INT N;
       * RETURNS:
       *   (&type) result reference to coordinate.
       */
      type & operator[]( INT N )
      {
        if (N <= 0)
          return X;
        if (N == 1)
          return Y;
        if (N == 2)
          return Z;
        return W;
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

      /* Interpolate 4D vector function.
       * ARGUMENTS:
       *   - vectors to interpolate between:
       *       const vec4<type> &V1, &V2;
       *   - interpolation coefficient:
       *       DBL T;
       * RETURNS:
       *   (matr) result matrix.
       */
      static vec4<type> Lerp( const vec4<type> &V1, const vec4<type> &V2, DBL T )
      {
        return vec4<type>(V1.X * (1 - T) + V2.X * T,
                          V1.Y * (1 - T) + V2.Y * T,
                          V1.Z * (1 - T) + V2.Z * T,
                          V1.W * (1 - T) + V2.W * T);
      } /* End of 'Lerp' function */

      /* Obtain rotation specified by quaternion matrix function.
       * ARGUMENTS:
       *   - quaternions to interpolate between:
       *       const vec4<type2> &Q1, &Q2;
       *   - interpolation coefficient:
       *       DBL T;
       * RETURNS:
       *   (matr) result matrix.
       */
      static vec4 SlerpQuat( const vec4 &Q1, const vec4 &Q2, DBL T )
      {
        auto
          len1 = Q1.X * Q1.X + Q1.Y * Q1.Y + Q1.Z * Q1.Z + Q1.W * Q1.W,
          len2 = Q2.X * Q2.X + Q2.Y * Q2.Y + Q2.Z * Q2.Z + Q2.W * Q2.W;
        vec4 q1, q2;
        if (abs(len1) >= 0.0001 && abs(len1 - 1) >= 0.0001)
          len1 = std::sqrt(len1), q1 = Q1 / len1;
        else
          q1 = Q1;
        if (abs(len2) >= 0.0001 && abs(len2 - 1) >= 0.0001)
          len2 = std::sqrt(len2), q2 = Q2 / len2;
        else
          q2 = Q2;
        DBL dot = q1 & q2;
        if (dot < 0)
          q1 *= -1;
        DBL
          theta = acos(dot),
          sin_theta = sqrt(1 - dot * dot);
        if (abs(sin_theta) < 0.00001 || dot > 1)
          return Lerp(q1, q2, T);
        return q1 * (sin(theta * (1 - T)) / sin_theta) +
               q2 * (sin(theta * T) / sin_theta);
      } /* End of 'SlerpQuat' function */
    }; /* End of 'vec4' class */
} /* end of 'mth' namespace */

#endif /* __mth_vec4_h_ */

/* END OF 'mth_vec4.h' FILE */
