/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_vec3.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math support module.
 *               3D vector module implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 26.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_vec3_h_
#define __mth_vec3_h_

#include "mth_def.h"

/* 'mth' namespace */
namespace mth
{
  /* 'vec3' handle class */
  template<typename type>
    class vec3
    {
    public:
      type X, Y, Z; // coordinates of vector

      /* Class default constructor */
      vec3( VOID )
      {
      } /* End of 'vec3' function */

      /* Type constructor.
       * ARGUMENTS:
       *   - coordinates:
       *       type &x, &y, &z;
       * RETURNS: None.
       */
      vec3( type x, type y, type z ) : X(x), Y(y), Z(z)
      {
      } /* End of 'vec3' function */

      /* Type constructor.
       * ARGUMENTS:
       *   - coordinate:
       *       type N;
       * RETURNS: None.
       */
      explicit vec3( type N ) : X(N), Y(N), Z(N)
      {
      } /* End of 'vec3' function */

      /* Add two vectors function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec3 &V2;
       * RETURNS:
       *   (vec3) result vector.
       */
      vec3 operator+( const vec3 &V2 ) const
      {
        return vec3(X + V2.X, Y + V2.Y, Z + V2.Z);
      } /* End of 'operator+' function */

      /* Add vector to first vector function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec3 &V2;
       * RETURNS:
       *   (&vec3) result vector.
       */
      vec3 & operator+=( const vec3 &V2 )
      {
        X += V2.X;
        Y += V2.Y;
        Z += V2.Z;
        return *this;
      } /* End of 'operator+=' function */

      /* Subtract one vector from another function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec3 &V2;
       * RETURNS:
       *   (vec3) result vector.
       */
      vec3 operator-( const vec3 &V2 ) const
      {
        return vec3(X - V2.X, Y - V2.Y, Z - V2.Z);
      } /* End of 'operator-' function */

      /* Obtain negated vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) result vector.
       */
      vec3<type> operator-( VOID ) const
      {
        return vec3(-X, -Y, -Z);
      } /* End of 'operator-' function */

      /* Subtract vector from first vector function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec3 &V2;
       * RETURNS:
       *   (&vec3) result vector.
       */
      vec3<type> & operator-=( const vec3 &V2 )
      {
        X -= V2.X;
        Y -= V2.Y;
        Z -= V2.Z;
        return *this;
      } /* End of 'operator-=' function */

      /* Multiply vector on a number function.
       * ARGUMENTS:
       *   - multiplier:
       *       type N;
       * RETURNS:
       *   (vec3) result vector.
       */
      vec3<type> operator*( type N ) const
      {
        return vec3(X * N, Y * N, Z * N);
      } /* End of 'operator*' function */

      /* Subtract vector from first vector function.
       * ARGUMENTS:
       *   - multiplier:
       *       type N;
       * RETURNS:
       *   (&vec3) result vector.
       */
      vec3<type> & operator*=( type N )
      {
        X *= N;
        Y *= N;
        Z *= N;
        return *this;
      } /* End of 'operator*=' function */

      /* Multiply vectors function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec &V2;
       * RETURNS:
       *   (vec3) result vector.
       */
      vec3<type> operator*( const vec3 &V2 ) const
      {
        return vec3<type>(X * V2.X, Y * V2.Y, Z * V2.Z);
      } /* End of 'operator*' function */

      /* Multiply vectors function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec &V2;
       * RETURNS:
       *   (&vec3) result vector.
       */
      vec3<type> & operator*=( const vec3 &V2 )
      {
        X *= V2.X;
        Y *= V2.Y;
        Z *= V2.Z;
        return *this;
      } /* End of 'operator*=' function */

      /* Divide vector by a number function.
       * ARGUMENTS:
       *   - divisor:
       *       type N;
       * RETURNS:
       *   (vec3) result vector.
       */
      vec3<type> operator/( type N ) const
      {
        if (N == 0)
          return vec3(0);
        return vec3<type>(X / N, Y / N, Z / N);
      } /* End of 'operator*' function */

      /* Divide vector by a number function.
       * ARGUMENTS:
       *   - divisor:
       *       type N;
       * RETURNS:
       *   (&vec3) result vector.
       */
      vec3<type> & operator/=( type N )
      {
        if (N == 0)
          X = 0, Y = 0, Z = 0;
        else
          X /= N, Y /= N, Z /= N;
        return *this;
      } /* End of 'operator/=' function */

      /* Divide vector by a vector function.
       * ARGUMENTS:
       *   - divisor vector:
       *       const vec &V2;
       * RETURNS:
       *   (vec3) result vector.
       */
      vec3<type> operator/( const vec3 &V2 ) const
      {
        if (V2.X == 0 || V2.Y == 0 || V2.Z == 0)
          return vec3(0);
        return vec3<type>(X / V2.X, Y / V2.Y, Z / V2.Z);
      } /* End of 'operator/' function */

      /* Divide vector by a vector function.
       * ARGUMENTS:
       *   - divisor vector:
       *       const vec3 &V2;
       * RETURNS:
       *   (&vec3) result vector.
       */
      vec3<type> & operator/=( const vec3 &V2 )
      {
        if (V2.X == 0 || V2.Y == 0 || V2.Z == 0)
          X = 0, Y = 0, Z = 0;
        else
          X /= V2.X, Y /= V2.Y, Z /= V2.Z;
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
       *       const vec3 &V2;
       * RETURNS:
       *   (type) result of dot production.
       */
      type operator&( const vec3 &V2 ) const
      {
        return X * V2.X + Y * V2.Y + Z * V2.Z;
      } /* End of 'operator&' function */

      /* Cross product two vectors function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec3 &V2;
       * RETURNS:
       *   (vec3) result of cross production.
       */
      vec3<type> operator%( const vec3 &V2 ) const
      {
        return vec3<type>(-Z * V2.Y + Y * V2.Z,
                          -X * V2.Z + Z * V2.X,
                          -Y * V2.X + X * V2.Y);
      } /* End of 'operator%' function */

      /* Normalize vector function.
       * ARGUMENTS: none.
       * RETURNS:
       *   (&vec3) normalized vector.
       */
      vec3<type> & Normalize( VOID )
      {
        *this /= !*this;
        return *this;
      } /* End of 'Normalize' function */

      /* Get normalized vector function.
       * ARGUMENTS: none.
       * RETURNS:
       *   (vec3) normalized vector.
       */
      vec3<type> Normalizing( VOID ) const
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
        return X * X + Y * Y + Z * Z;
      } /* End of 'Length2' function */

      /* Get distance between two vectors function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec3 &V2;
       * RETURNS:
       *   (type) result distance.
       */
      type Distance( const vec3 &V2 ) const
      {
        return !(*this - V2);
      } /* End of 'Distance' function */

      /* Get zero vector function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      static vec3<type> Zero( VOID )
      {
        return vec3(0);
      } /* End of 'Zero' function */

      /* Get random vector between 0 and 1 function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) result random vec3.
       */
      static vec3 Rnd0( VOID )
      {
        return vec3(1.0 * rand() / RAND_MAX,
                    1.0 * rand() / RAND_MAX,
                    1.0 * rand() / RAND_MAX);
      } /* End of 'Rnd0' function */

      /* Get random vector between -1 and 1 function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) result random vec3.
       */
      static vec3 Rnd1( VOID )
      {
        return vec3(2.0 * rand() / RAND_MAX - 1,
                    2.0 * rand() / RAND_MAX - 1,
                    2.0 * rand() / RAND_MAX - 1);
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
        if (N == 1)
          return Y;
        return Z;
      } /* End of 'operator[]' function */

      /* Get vector constant coordinate by index function.
       * ARGUMENTS:
       *   - index of coordinate:
       *       INT N;
       * RETURNS:
       *   (&type) result link to coordinate.
       */
      const type & operator[]( INT N ) const
      {
        if (N <= 0)
          return X;
        if (N == 1)
          return Y;
        return Z;
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

      /* Get max coordinate function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (type) max coordinate value.
       */
      type MaxC( VOID ) const
      {
        return X >= Y && X >= Z ? X : Y >= X && Y >= Z ? Y : Z;
      } /* End of 'MaxC' function */

      /* Get max from two vector function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec3 &V2;
       * RETURNS:
       *   (vec3) max vector.
       */
      vec3<type> Max( const vec3 &V2 ) const
      {
        return vec3(COM_MAX(X, V2.X), COM_MAX(Y, V2.Y), COM_MAX(Z, V2.Z));
      } /* End of 'Max' function */

      /* Get min from two vector function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec3 &V2;
       * RETURNS:
       *   (vec3) min vector.
       */
      vec3<type> Min( const vec3 &V2 ) const
      {
        return vec3(COM_MIN(X, V2.X), COM_MIN(Y, V2.Y), COM_MIN(Z, V2.Z));
      } /* End of 'Min' function */

      /* Interpolate 3D vector function.
       * ARGUMENTS:
       *   - vectors to interpolate between:
       *       const vec3<type> &V1, &V2;
       *   - interpolation coefficient:
       *       DBL T;
       * RETURNS:
       *   (matr) result matrix.
       */
      static vec3<type> Lerp( const vec3<type> &V1, const vec3<type> &V2, DBL T )
      {
        return vec3<type>(V1.X * (1 - T) + V2.X * T,
                          V1.Y * (1 - T) + V2.Y * T,
                          V1.Z * (1 - T) + V2.Z * T);
      } /* End of 'Lerp' function */
    }; /* End of 'vec3' class */
} /* end of 'mth' namespace */

#endif /* __mth_vec3_h_ */

/* END OF 'mth_vec3.h' FILE */
