/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_matr.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math support module.
 *               Matrix evaluation implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 05.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_matr_h_
#define __mth_matr_h_

#include "mth_def.h"

/* 'mth' namespace */
namespace mth
{
  /* Matrix bare data type */
  template<class type>
    struct matr_data
    {
    public:
      /* Transformation matrix */
      type M[4][4];

      /* Class constructor */
      matr_data( VOID )
      {
      } /* End of 'matr_data' function */

      /* Class constructor.
       * ARGUMENTS:
       *   - source matrix:
       *       const type Mtr[4][4];
       */
      matr_data( const type Mtr[4][4] )
      {
        CopyMemory(M, Mtr, sizeof(type) * 16);
      } /* End of 'matr_data' function */
    }; /* End of 'matr_data' structure */

  /* Space transformation matrix handle class */
  template<class type>
    class matr : public matr_data<type>
    {
    private: 
      /* Inverse matrix and it presence flag */
      mutable BOOL IsInverseObtain;
      mutable type InverseM[4][4];
 
    public:
      /* Class default constructor */
      matr( VOID ) : IsInverseObtain(FALSE)
      {
      } /* End of 'matr' function */

      /* Matrix constructor.
       * ARGUMENTS:
       *   - matrix 4x4 values:
       *       type
       *         A00, A01, A02, A03,
       *         A10, A11, A12, A13,
       *         A20, A21, A22, A23,
       *         A30, A31, A32, A33;
       */
      matr( type A00, type A01, type A02, type A03,
            type A10, type A11, type A12, type A13,
            type A20, type A21, type A22, type A23,
            type A30, type A31, type A32, type A33 ) : IsInverseObtain(FALSE)
      {
        type (*M)[4] = matr_data<type>::M;

        M[0][0] = A00, M[0][1] = A01, M[0][2] = A02, M[0][3] = A03;
        M[1][0] = A10, M[1][1] = A11, M[1][2] = A12, M[1][3] = A13;
        M[2][0] = A20, M[2][1] = A21, M[2][2] = A22, M[2][3] = A23;
        M[3][0] = A30, M[3][1] = A31, M[3][2] = A32, M[3][3] = A33;
      } /* End of 'matr' function */

      /* Class constructor.
       * ARGUMENTS:
       *   - matrix elements:
       *       type R[4][4];
       */
      matr( type R[4][4] ) : IsInverseObtain(FALSE)
      {
        CopyMemory(matr_data<type>::M, R, sizeof(matr_data<type>::M));
      } /* End of 'matr' function */

      /* Obtain matrix first element pointer function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (type *) pointer to A00 element.
       */
      operator type *( VOID )
      {
        return matr_data<type>::M[0];
      } /* End of 'operator type *' function */

      /* Obtain matrix first element pointer function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (const type *) pointer to A00 element.
       */
      operator const type *( VOID ) const
      {
        return matr_data<type>::M[0];
      } /* End of 'operator const type *' function */

      /* Multiply two matrices function.
       * ARGUMENTS:
       *   - second matrix:
       *       const matr<type> &M2;
       * RETURNS:
       *   (matr<type>) result matrix.
       */
      matr<type> operator*( const matr<type> &M ) const
      {
        const type (*M1)[4] = matr_data<type>::M;
        return matr<type>(M1[0][0] * M.M[0][0] + M1[0][1] * M.M[1][0] + M1[0][2] * M.M[2][0] + M1[0][3] * M.M[3][0],
          M1[0][0] * M.M[0][1] + M1[0][1] * M.M[1][1] + M1[0][2] * M.M[2][1] + M1[0][3] * M.M[3][1],
          M1[0][0] * M.M[0][2] + M1[0][1] * M.M[1][2] + M1[0][2] * M.M[2][2] + M1[0][3] * M.M[3][2],
          M1[0][0] * M.M[0][3] + M1[0][1] * M.M[1][3] + M1[0][2] * M.M[2][3] + M1[0][3] * M.M[3][3],
          M1[1][0] * M.M[0][0] + M1[1][1] * M.M[1][0] + M1[1][2] * M.M[2][0] + M1[1][3] * M.M[3][0],
          M1[1][0] * M.M[0][1] + M1[1][1] * M.M[1][1] + M1[1][2] * M.M[2][1] + M1[1][3] * M.M[3][1],
          M1[1][0] * M.M[0][2] + M1[1][1] * M.M[1][2] + M1[1][2] * M.M[2][2] + M1[1][3] * M.M[3][2],
          M1[1][0] * M.M[0][3] + M1[1][1] * M.M[1][3] + M1[1][2] * M.M[2][3] + M1[1][3] * M.M[3][3],
          M1[2][0] * M.M[0][0] + M1[2][1] * M.M[1][0] + M1[2][2] * M.M[2][0] + M1[2][3] * M.M[3][0],
          M1[2][0] * M.M[0][1] + M1[2][1] * M.M[1][1] + M1[2][2] * M.M[2][1] + M1[2][3] * M.M[3][1],
          M1[2][0] * M.M[0][2] + M1[2][1] * M.M[1][2] + M1[2][2] * M.M[2][2] + M1[2][3] * M.M[3][2],
          M1[2][0] * M.M[0][3] + M1[2][1] * M.M[1][3] + M1[2][2] * M.M[2][3] + M1[2][3] * M.M[3][3],
          M1[3][0] * M.M[0][0] + M1[3][1] * M.M[1][0] + M1[3][2] * M.M[2][0] + M1[3][3] * M.M[3][0],
          M1[3][0] * M.M[0][1] + M1[3][1] * M.M[1][1] + M1[3][2] * M.M[2][1] + M1[3][3] * M.M[3][1],
          M1[3][0] * M.M[0][2] + M1[3][1] * M.M[1][2] + M1[3][2] * M.M[2][2] + M1[3][3] * M.M[3][2],
          M1[3][0] * M.M[0][3] + M1[3][1] * M.M[1][3] + M1[3][2] * M.M[2][3] + M1[3][3] * M.M[3][3]);
      } /* End of 'operator*' function */

      /* Plus two matrices function.
       * ARGUMENTS:
       *   - second matrix:
       *       const matr<type> &M2;
       * RETURNS:
       *   (matr<type>) result matrix.
       */
      matr<type> operator+( const matr<type> &M2 )
      {
        const type (*M)[4] = matr_data<type>::M;
        return matr<type>(M[0][0] + M2.M[0][0], M[0][1] + M2.M[0][1], M[0][2] + M2.M[0][2], M[0][3] + M2.M[0][3],
                          M[1][0] + M2.M[1][0], M[1][1] + M2.M[1][1], M[1][2] + M2.M[1][2], M[1][3] + M2.M[1][3],
                          M[2][0] + M2.M[2][0], M[2][1] + M2.M[2][1], M[2][2] + M2.M[2][2], M[2][3] + M2.M[2][3],
                          M[3][0] + M2.M[3][0], M[3][1] + M2.M[3][1], M[3][2] + M2.M[3][2], M[3][3] + M2.M[3][3]);
      } /* End of 'operator+' function */

      /* Minus two matrices function.
       * ARGUMENTS:
       *   - second matrix:
       *       const matr<type> &M2;
       * RETURNS:
       *   (matr<type>) result matrix.
       */
      matr<type> operator-( const matr<type> &M2 )
      {
        const type (*M)[4] = matr_data<type>::M;
        return matr<type>(M[0][0] - M2.M[0][0], M[0][1] - M2.M[0][1], M[0][2] - M2.M[0][2], M[0][3] - M2.M[0][3],
                          M[1][0] - M2.M[1][0], M[1][1] - M2.M[1][1], M[1][2] - M2.M[1][2], M[1][3] - M2.M[1][3],
                          M[2][0] - M2.M[2][0], M[2][1] - M2.M[2][1], M[2][2] - M2.M[2][2], M[2][3] - M2.M[2][3],
                          M[3][0] - M2.M[3][0], M[3][1] - M2.M[3][1], M[3][2] - M2.M[3][2], M[3][3] - M2.M[3][3]);
      } /* End of 'operator-' function */

      /* Multiply matrix and a scalar function.
       * ARGUMENTS:
       *   - multiplier:
       *       const type mul;
       * RETURNS:
       *   (matr<type>) result matrix.
       */
      matr<type> operator*( const type Mul )
      {
        const type (*M)[4] = matr_data<type>::M;
        return matr<type>(M[0][0] * Mul, M[0][1] * Mul, M[0][2] * Mul, M[0][3] * Mul,
                          M[1][0] * Mul, M[1][1] * Mul, M[1][2] * Mul, M[1][3] * Mul,
                          M[2][0] * Mul, M[2][1] * Mul, M[2][2] * Mul, M[2][3] * Mul,
                          M[3][0] * Mul, M[3][1] * Mul, M[3][2] * Mul, M[3][3] * Mul);
      } /* End of 'operator*' function */

      /* Get transposed 4x4 matrix.
       * ARGUMENTS: None.
       * RETURNS:
       *   (matr<type>) result transposed matrix.
       */
      matr<type> Transpose( VOID )
      {
        const type (*M)[4] = matr_data<type>::M;
        return matr(M[0][0], M[1][0], M[2][0], M[3][0],
                    M[0][1], M[1][1], M[2][1], M[3][1],
                    M[0][2], M[1][2], M[2][2], M[3][2],
                    M[0][3], M[1][3], M[2][3], M[3][3]);
      } /* End of 'Transpose' function */

      /* Get transposed 4x4 matrix.
       * ARGUMENTS: None.
       * RETURNS:
       *   (matr<type>) result transposed matrix.
       */
      static matr<type> Transpose( type *M )
      {
        return matr(M[0][0], M[1][0], M[2][0], M[3][0],
                    M[0][1], M[1][1], M[2][1], M[3][1],
                    M[0][2], M[1][2], M[2][2], M[3][2],
                    M[0][3], M[1][3], M[2][3], M[3][3]);
      } /* End of 'Transpose' function */

      /* Get determinant of 2x2 matrix function.
       * ARGUMENTS:
       *   - matrix elements:
       *       type A11, A12, A21, A22;
       * RETURNS:
       *   (type) determinant.
       */
      static type Determ2x2( const type &A11, const type &A12,
                             const type &A21, const type &A22 )
      {
        return A11 * A22 - A12 * A21;
      } /* End of 'Determ2x2' function */

      /* Get determinant of 3x3 matrix function.
       * ARGUMENTS:
       *   - matrix elements:
       *       const type &A11, &A12, &A13, &A21, &A22, &A23, &A31, &A32, &A33;
       * RETURNS:
       *   (type) determinant.
       */
      static type Determ3x3( const type &A11, const type &A12, const type &A13,
                             const type &A21, const type &A22, const type &A23,
                             const type &A31, const type &A32, const type &A33 )
      {
        return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 - A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
      } /* End of 'Determ3x3' function */

      /* Get determinant of 4x4 matrix function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (type) determinant.
       */
      type Determ( VOID ) const
      {
        const type (*M)[4] = matr_data<type>::M;
        return
          M[0][0] * Determ3x3(M[1][1], M[1][2], M[1][3],
                              M[2][1], M[2][2], M[2][3],
                              M[3][1], M[3][2], M[3][3]) +

          -M[0][1] * Determ3x3(M[1][0], M[1][2], M[1][3],
                               M[2][0], M[2][2], M[2][3],
                               M[3][0], M[3][2], M[3][3]) +

          M[0][2] * Determ3x3(M[1][0], M[1][1], M[1][3],
                              M[2][0], M[2][1], M[2][3],
                              M[3][0], M[3][1], M[3][3]) +

          -M[0][3] * Determ3x3(M[1][0], M[1][1], M[1][2],
                               M[2][0], M[2][1], M[2][2],
                               M[3][0], M[3][1], M[3][2]);
      } /* End of 'Determ' function */

      /* Obtain matrix identity function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (matr) matrix identity.
       */
      static const matr Identity( VOID )
      {
        return matr(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
      } /* End of 'Identity' function */

      /* Evaluate inverse matrix function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID EvaluateInverseMatrix( VOID ) const
      {
        if (IsInverseObtain)
          return;

        IsInverseObtain = TRUE;
        const type (*M)[4] = matr_data<type>::M;
        type det = Determ();

        InverseM[0][0] =
          Determ3x3(M[1][1], M[1][2], M[1][3],
                    M[2][1], M[2][2], M[2][3],
                    M[3][1], M[3][2], M[3][3]) / det;
        InverseM[1][0] =
         -Determ3x3(M[1][0], M[1][2], M[1][3],
                    M[2][0], M[2][2], M[2][3],
                    M[3][0], M[3][2], M[3][3]) / det;
        InverseM[2][0] =
          Determ3x3(M[1][0], M[1][1], M[1][3],
                    M[2][0], M[2][1], M[2][3],
                    M[3][0], M[3][1], M[3][3]) / det;
        InverseM[3][0] =
         -Determ3x3(M[1][0], M[1][1], M[1][2],
                    M[2][0], M[2][1], M[2][2],
                    M[3][0], M[3][1], M[3][2]) / det;
        InverseM[0][1] =
         -Determ3x3(M[0][1], M[0][2], M[0][3],
                    M[2][1], M[2][2], M[2][3],
                    M[3][1], M[3][2], M[3][3]) / det;
        InverseM[1][1] =
          Determ3x3(M[0][0], M[0][2], M[0][3],
                    M[2][0], M[2][2], M[2][3],
                    M[3][0], M[3][2], M[3][3]) / det;
        InverseM[2][1] =
         -Determ3x3(M[0][0], M[0][1], M[0][3],
                    M[2][0], M[2][1], M[2][3],
                    M[3][0], M[3][1], M[3][3]) / det;
        InverseM[3][1] =
          Determ3x3(M[0][0], M[0][1], M[0][2],
                    M[2][0], M[2][1], M[2][2],
                    M[3][0], M[3][1], M[3][2]) / det;
        InverseM[0][2] =
          Determ3x3(M[0][1], M[0][2], M[0][3],
                    M[1][1], M[1][2], M[1][3],
                    M[3][1], M[3][2], M[3][3]) / det;
        InverseM[1][2] =
         -Determ3x3(M[0][0], M[0][2], M[0][3],
                    M[1][0], M[1][2], M[1][3],
                    M[3][0], M[3][2], M[3][3]) / det;
        InverseM[2][2] =
          Determ3x3(M[0][0], M[0][1], M[0][3],
                    M[1][0], M[1][1], M[1][3],
                    M[3][0], M[3][1], M[3][3]) / det;
        InverseM[3][2] =
         -Determ3x3(M[0][0], M[0][1], M[0][2],
                    M[1][0], M[1][1], M[1][2],
                    M[3][0], M[3][1], M[3][2]) / det;
        InverseM[0][3] =
         -Determ3x3(M[0][1], M[0][2], M[0][3],
                    M[1][1], M[1][2], M[1][3],
                    M[2][1], M[2][2], M[2][3]) / det;
        InverseM[1][3] =
          Determ3x3(M[0][0], M[0][2], M[0][3],
                    M[1][0], M[1][2], M[1][3],
                    M[2][0], M[2][2], M[2][3]) / det;
        InverseM[2][3] =
         -Determ3x3(M[0][0], M[0][1], M[0][3],
                    M[1][0], M[1][1], M[1][3],
                    M[2][0], M[2][1], M[2][3]) / det;
        InverseM[3][3] =
          Determ3x3(M[0][0], M[0][1], M[0][2],
                    M[1][0], M[1][1], M[1][2],
                    M[2][0], M[2][1], M[2][2]) / det;
      } /* End of 'EvaluateInverseMatrix' function */

      /* Obtain inverse matrix function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (matr) result matrix.
       */
      matr<type> Inverse( VOID ) const
      {
        EvaluateInverseMatrix();
        return matr<type>(InverseM);
      } /* End of 'Inverse' function */

      /* Obtain translation matrix function.
       * ARGUMENTS:
       *   - coordinates delta values:
       *       const type &Dx, &Dy, &Dz;
       * RETURNS:
       *   (matr) translate transformatrion matrix.
       */
      static matr Translate( const type &Dx, const type &Dy, const type &Dz )
      {
        return matr(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    Dx, Dy, Dz, 1);
      } /* End of 'Translate' function */

      /* Obtain translation matrix function.
       * ARGUMENTS:
       *   - vector delta value:
       *       const vec3<type> &D;
       * RETURNS:
       *   (matr) translate transformatrion matrix.
       */
      static matr Translate( const vec3<type> &D )
      {
        return matr(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    D.X, D.Y, D.Z, 1);
      } /* End of 'Translate' function */

      /* Obtain scale matrix function.
       * ARGUMENTS:
       *   - coordinates scale values:
       *       const type &Dx, &Dy, &Dz;
       * RETURNS:
       *   (matr) scale transformatrion matrix.
       */
      static matr Scale( const type &Sx, const type &Sy, const type &Sz )
      {
        return matr(Sx, 0, 0, 0,
                    0, Sy, 0, 0,
                    0, 0, Sz, 0,
                    0, 0, 0, 1);
      } /* End of 'Scale' function */

      /* Obtain scale matrix function.
       * ARGUMENTS:
       *   - vector scale value:
       *       const vec3<type> &D;
       * RETURNS:
       *   (matr) scale transformatrion matrix.
       */
      static matr Scale( const vec3<type> &D )
      {
        return matr(D.X, 0, 0, 0,
                    0, D.Y, 0, 0,
                    0, 0, D.Z, 0,
                    0, 0, 0, 1);
      } /* End of 'Scale' function */

      /* Obtain rotate x-axis matrix function.
       * ARGUMENTS:
       *   - angle in degrees:
       *       const type &AngleInDegree;
       * RETURNS:
       *   (matr) rotate transformatrion matrix.
       */
      static matr RotateX( const type &AngleInDegree )
      {
        type a = D2R(AngleInDegree), si = sin(a), co = cos(a);

        return matr(1, 0, 0, 0,
                    0, co, si, 0,
                    0, -si, co, 0,
                    0, 0, 0, 1);
      } /* End of 'RotateX' function */

      /* Obtain rotate y-axis matrix function.
       * ARGUMENTS:
       *   - angle in degrees:
       *       const type &AngleInDegree;
       * RETURNS:
       *   (matr) rotate transformatrion matrix.
       */
      static matr RotateY( const type &AngleInDegree )
      {
        type a = D2R(AngleInDegree), si = sin(a), co = cos(a);

        return matr(co, 0, -si,  0,
                    0, 1, 0, 0,
                    si, 0, co, 0,
                    0, 0, 0, 1);
      } /* End of 'RotateY' function */

      /* Obtain rotate z-axis matrix function.
       * ARGUMENTS:
       *   - angle in degrees:
       *       const type &AngleInDegree;
       * RETURNS:
       *   (matr) rotate transformatrion matrix.
       */
      static matr RotateZ( const type &AngleInDegree )
      {
        type a = D2R(AngleInDegree), si = sin(a), co = cos(a);

        return matr(co, si, 0, 0,
                    -si, co, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
      } /* End of 'RotateZ' function */

      /* Obtain rotate any axis matrix function.
       * ARGUMENTS:
       *   - angle in degrees:
       *       const type &AngleInDegree;
       *   - axis vector:
       *       const vec3<vtype> &V;
       * RETURNS:
       *   (matr) rotate transformatrion matrix.
       */
      static matr Rotate( const type &AngleInDegree, const vec3<type> &V )
      {
        type a = D2R(AngleInDegree), si = sin(a), co = cos(a);
        vec3<type> R = V.Normalizing();

        return matr(co + R.X * R.X * (1 - co),
                    R.X * R.Y *(1 - co) + R.Z * si,
                    R.X * R.Z * (1 - co) - R.Y * si,
                    0,
                    R.Y * R.X * (1 - co) - R.Z * si,
                    co + R.Y * R.Y * (1 - co),
                    R.Y * R.Z * (1 - co) + R.X * si,
                    0,
                    R.Z * R.X * (1 - co) + R.Y * si,
                    R.Z * R.Y * (1 - co) - R.X * si,
                    co + R.Z * R.Z * (1 - co),
                    0,
                    0, 0, 0, 1);
      } /* End of 'Rotate' function */

      /* Matrix look-at viewer setup function.
       * ARGUMENTS:
       *   - viewer position, look-at point, approximate up direction:
       *       const vec3<type> &Loc, &At, &Up1;
       * RETURNS:
       *   (matr) result matrix.
       */
      static matr View( const vec3<type> &Loc, const vec3<type> &At, const vec3<type> &Up1 )
      {
        vec3<type>
          Dir = (At - Loc).Normalizing(),
          Right = (Dir % Up1).Normalizing(),
          Up = (Right % Dir).Normalizing();

        return matr<type>(Right.X, Up.X, -Dir.X, 0,
                          Right.Y, Up.Y, -Dir.Y, 0,
                          Right.Z, Up.Z, -Dir.Z, 0,
                          -(Loc & Right), -(Loc & Up), Loc & Dir, 1);
      } /* End of 'View' function */

      /* Obtain view frustum matrix function.
       * ARGUMENTS:
       *   - distances to frustum planes:
       *       const type &l, &r, &b, &t, &n, &f;
       * RETURNS:
       *   (MATR) result frustum matrix.
       */
      static matr Frustum( const type &L, const type &R, const type &B, const type &T, const type &N, const type &F )
      {
        return matr(2 * N / (R - L), 0, 0, 0,
                    0, 2 * N / (T - B), 0, 0,
                    (R + L) / (R - L), (T + B) / (T - B), -(F + N) / (F - N), -1,
                    0, 0, -(2 * N * F / (F - N)), 0);
      } /* End of 'Frustum' function */

      /* Obtain view ortho matrix function.
       * ARGUMENTS:
       *   - distances to ortho planes:
       *       const type &l, &r, &b, &t, &n, &f;
       * RETURNS:
       *   (matr) result ortho matrix.
       */
      static matr Ortho( const type &L, const type &R, const type &B, const type &T, const type &N, const type &F )
      {
        return matr(2 / (R - L), 0, 0, 0,
                    0, 2 / (T - B), 0, 0,
                    0, 0, -2 / (F - N), 0,
                    -(R + L) / (R - L), -(T + B) / (T - B), - (F + N) / (F - N), 1);
      } /* End of 'Ortho' function */

      /* Transformation point by matrix function.
       * ARGUMENTS:
       *   - vector to be tranform:
       *       const vec3<type> &V;
       * RETURNS:
       *   (vec3) transformed vector.
       */
      template<typename vtype>
        vec3<vtype> TransformPoint( const vec3<vtype> &V ) const
        {
          const type (*M)[4] = matr_data<type>::M;
          vtype w = V.X * M[0][3] + V.Y * M[1][3] + V.Z * M[2][3] + M[3][3];

          return vec3<vtype>((V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0] + M[3][0]) / w,
                             (V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1] + M[3][1]) / w,
                             (V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2] + M[3][2]) / w);
        } /* End of 'TransformPoint' function */

      /* Transformation vector by matrix function.
       * ARGUMENTS:
       *   - vector to be tranform:
       *       const vec3<type> &V;
       * RETURNS:
       *   (vec3) transformed vector.
       */
      template<typename vtype>
        vec3<vtype> TransformVector( const vec3<vtype> &V ) const
        {
          const type (*M)[4] = matr_data<type>::M;
          return vec3<vtype>(V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0],
                             V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1],
                             V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2]);
        } /* End of 'TransformVector' function */

      /* Transformation normal vector by matrix function.
       * ARGUMENTS:
       *   - normal vector to be tranform:
       *       const vec3<type> &N;
       * RETURNS:
       *   (vec3) transformed vector.
       */
      template<typename vtype>
        vec3<vtype> TransformNormal( const vec3<vtype> &N ) const
        {
          EvaluateInverseMatrix();
          return vec3<vtype>(N.X * InverseM[0][0] + N.Y * InverseM[0][1] + N.Z * InverseM[0][2],
                             N.X * InverseM[1][0] + N.Y * InverseM[1][1] + N.Z * InverseM[1][2],
                             N.X * InverseM[2][0] + N.Y * InverseM[2][1] + N.Z * InverseM[2][2]);
        } /* End of 'TransformNormal' function */

      /* Multiply 3-D vector on matrix4x4 function.
       * ARGUMENTS:
       *   - vector to be multiplied:
       *       const vec3<vtype> &V;
       * RETURNS:
       *   (vec3<vtype>) result vector.
       */
      template<typename vtype>
        vec3<vtype> Transform4x4( const vec3<vtype> &V ) const
        {
          const type (*M)[4] = matr_data<type>::M;
 
          return vec3<vtype>((V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0] + M[3][0]),
                             (V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1] + M[3][1]),
                             (V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2] + M[3][2]));
        } /* End of 'Transform4x4' function */

      /* Transformation point by inverse matrix function.
       * ARGUMENTS:
       *   - vector to be tranformed:
       *       const vec3<type> &V;
       * RETURNS:
       *   (vec3) transformed vector.
       */
      template<typename vtype>
        vec3<vtype> InvTransformPoint( const vec3<vtype> &V ) const
        {
          EvaluateInverseMatrix();
          vtype w = V.X * InverseM[0][3] + V.Y * InverseM[1][3] + V.Z * InverseM[2][3] + InverseM[3][3];

          return vec3<vtype>((V.X * InverseM[0][0] + V.Y * InverseM[1][0] + V.Z * InverseM[2][0] + InverseM[3][0]) / w,
                             (V.X * InverseM[0][1] + V.Y * InverseM[1][1] + V.Z * InverseM[2][1] + InverseM[3][1]) / w,
                             (V.X * InverseM[0][2] + V.Y * InverseM[1][2] + V.Z * InverseM[2][2] + InverseM[3][2]) / w);
        } /* End of 'TransformPoint' function */

      /* Transformation vector by inverse matrix function.
       * ARGUMENTS:
       *   - vector to be tranformed:
       *       const vec3<type> &V;
       * RETURNS:
       *   (vec3) transformed vector.
       */
      template<typename vtype>
        vec3<vtype> InvTransformVector( const vec3<vtype> &V ) const
        {
          EvaluateInverseMatrix();
          matr T = matr<type>(InverseM).Transpose();
          return vec3<vtype>(V.X * T.M[0][0] + V.Y * T.M[1][0] + V.Z * T.M[2][0],
                             V.X * T.M[0][1] + V.Y * T.M[1][1] + V.Z * T.M[2][1],
                             V.X * T.M[0][2] + V.Y * T.M[1][2] + V.Z * T.M[2][2]);
        } /* End of 'InvTransformVector' function */

      /* Transformation normal vector by invesematrix function.
       * ARGUMENTS:
       *   - normal vector to be tranform:
       *       const vec3<type> &N;
       * RETURNS:
       *   (vec3) transformed vector.
       */
      template<typename vtype>
        vec3<vtype> InvTransformNormal( const vec3<vtype> &N ) const
        {
          const type (*M)[4] = matr_data<type>::M;
          return vec3<vtype>(N.X * M[0][0] + N.Y * M[0][1] + N.Z * M[0][2],
                             N.X * M[1][0] + N.Y * M[1][1] + N.Z * M[1][2],
                             N.X * M[2][0] + N.Y * M[2][1] + N.Z * M[2][2]);
        } /* End of 'InvTransformNormal' function */

      /* Obtain rotation specified by quaternion matrix function.
       * ARGUMENTS:
       *   - rotation quaternion:
       *       type AngleInDegree;
       *   - vector rotate around:
       *       const vec3<type> &Axis;
       * RETURNS:
       *   (matr) result matrix.
       */
      static matr RotateQuat( const vec4<type> &Q )
      {
        auto [x, y, z, w] = Q;
        auto len = x * x + y * y + z * z + w * w;

        if (len != 0 && std::abs(len - 1) > 0.0001)
          len = std::sqrt(len), x  /= len, y /= len, z /= len, w /= len;
        return matr(1 - 2 * y * y - 2 * z * z, 2 * x * y + 2 * w * z, 2 * x * z - 2 * w * y, 0,
                    2 * x * y - 2 * w * z, 1 - 2 * x * x - 2 * z * z, 2 * y * z + 2 * w * x, 0,
                    2 * x * z + 2 * w * y, 2 * y * z - 2 * w * x, 1 - 2 * x * x - 2 * y * y, 0,
                    0, 0, 0, 1);
      } /* End of 'RotateQuat' function */
    }; /* End of 'matr' class */
} /* end of 'mth' namespace */

#endif /* __mth_matr_h_ */

/* END OF 'mth_matr.h' FILE */
