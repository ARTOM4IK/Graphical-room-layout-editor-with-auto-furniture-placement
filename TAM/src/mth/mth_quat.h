/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_quat.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math support module.
 *               Quaternions module implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 26.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_quat_h_
#define __mth_quat_h_

#include "tam_def.h"

/* 'mth' namespace */
namespace mth
{
  /* 'quat' handle class */
  template<typename type>
    class quat
    {
    public:
      type X, Y, Z, W;

      /* Class default constructor */
      quat( VOID )
      {
      } /* End of 'quat' function */

      /* Component-based constructor function.
       * ARGUMENTS:
       *   - X, Y, Z, W components:
       *       type NewX, NewY, NewZ, NewW;
       * RETURNS: None.
       */
      quat( type NewX, type NewY, type NewZ, type NewW ) : X(NewX), Y(NewY), Z(NewZ), W(NewW)
      {
      } /* End of 'quat' function */

      /* Obtain quaternion length function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (type) quaternion length.
       */
      type operator!( VOID ) const
      {
        type len = X * X + Y * Y + Z * Z + W * W;
        if (len == 1 || len == 0)
          return len;
        return std::sqrt(len);
      } /* End of 'operator!' function */

      /* Dot product of 2 quaternions function.
       * ARGUMENTS:
       *   - quaternion:
       *       const quat &Q;
       * RETURNS:
       *   (type) dot product.
       */
      type operator&( const quat &Q ) const
      {
        return X * Q.X + Y * Q.Y + Z * Q.Z + W * Q.W;
      } /* End of 'operator&' function */

      /* Add two quaternions function.
       * ARGUMENTS:
       *   - quaternion:
       *       const quat &Q;
       * RETURNS:
       *   (quat) result quaternion.
       */
      quat operator+( const quat &Q ) const
      {
        return quat(X + Q.X, Y + Q.Y, Z + Q.Z, W + Q.W);
      } /* End of 'operator+' function */

      /* Subtract two quaternions function.
       * ARGUMENTS:
       *   - quaternion:
       *       const quat &Q;
       * RETURNS:
       *   (quat) result quaternion.
       */
      quat operator-( const quat &Q ) const
      {
        return quat(X - Q.X, Y - Q.Y, Z - Q.Z, W - Q.W);
      } /* End of 'operator-' function */

      /* Negate quaternion function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (quat) negated quaternion.
       */
      quat operator-( VOID ) const
      {
        return quat(-X, -Y, -Z, -W);
      } /* End of 'operator-' function */

      /* Multiply quaternion by scalar function.
       * ARGUMENTS:
       *   - scalar value:
       *       const type &S;
       * RETURNS:
       *   (quat) result quaternion.
       */
      quat operator*( const type &S ) const
      {
        return quat(X * S, Y * S, Z * S, W * S);
      } /* End of 'operator*' function */

      /* Divide quaternion by scalar function.
       * ARGUMENTS:
       *   - scalar value:
       *       const type &S;
       * RETURNS:
       *   (quat) result quaternion.
       */
      quat operator/( const type &S ) const
      {
        return quat(X / S, Y / S, Z / S, W / S);
      } /* End of 'operator/' function */

      /* Add quaternion to this quaternion function.
       * ARGUMENTS:
       *   - quaternion:
       *       const quat &Q;
       * RETURNS:
       *   (quat &) updated quaternion reference.
       */
      quat & operator+=( const quat &Q )
      {
        X += Q.X;
        Y += Q.Y;
        Z += Q.Z;
        W += Q.W;
        return *this;
      } /* End of 'operator+=' function */

      /* Subtract quaternion from this quaternion function.
       * ARGUMENTS:
       *   - quaternion:
       *       const quat &Q;
       * RETURNS:
       *   (quat &) updated quaternion reference.
       */
      quat & operator-=( const quat &Q )
      {
        X -= Q.X;
        Y -= Q.Y;
        Z -= Q.Z;
        W -= Q.W;
        return *this;
      } /* End of 'operator-=' function */

      /* Multiply this quaternion by scalar function.
       * ARGUMENTS:
       *   - scalar value:
       *       const type &S;
       * RETURNS:
       *   (quat &) updated quaternion reference.
       */
      quat & operator*=( const type &S )
      {
        X *= S;
        Y *= S;
        Z *= S;
        W *= S;
        return *this;
      } /* End of 'operator*=' function */

      /* Divide this quaternion by scalar function.
       * ARGUMENTS:
       *   - scalar value:
       *       const type &S;
       * RETURNS:
       *   (quat &) updated quaternion reference.
       */
      quat & operator/=( const type &S )
      {
        X /= S;
        Y /= S;
        Z /= S;
        W /= S;
        return *this;
      } /* End of 'operator/=' function */

      /* Identity quaternion function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (quat) identity quaternion.
       */
      static quat Identity( VOID )
      {
        return quat(0, 0, 0, 1);
      } /* End of 'Identity' function */

      /* Quaternion conjugate function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (quat) conjugate quaternion.
       */
      quat Conjugate( VOID ) const
      {
        return quat(-X, -Y, -Z, W);
      } /* End of 'Conjugate' function */

      /* Quaternion inverse function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (quat) inverse quaternion.
       */
      quat Inverse( VOID ) const
      {
        type norm = X * X + Y * Y + Z * Z + W * W;
        if (norm == 0)
          return Identity();
        return Conjugate() / norm;
      } /* End of 'Inverse' function */

      /* Quaternion multiplication function.
       * ARGUMENTS:
       *   - quaternion:
       *       const quat &Q;
       * RETURNS:
       *   (quat) result quaternion.
       */
      quat operator*( const quat &Q ) const
      {
        return quat(
          W * Q.X + X * Q.W + Y * Q.Z - Z * Q.Y,
          W * Q.Y - X * Q.Z + Y * Q.W + Z * Q.X,
          W * Q.Z + X * Q.Y - Y * Q.X + Z * Q.W,
          W * Q.W - X * Q.X - Y * Q.Y - Z * Q.Z);
      } /* End of 'operator*' function */

      /* Quaternion multiplication with assignment function.
       * ARGUMENTS:
       *   - quaternion:
       *       const quat &Q;
       * RETURNS:
       *   (quat &) result quaternion reference.
       */
      quat & operator*=( const quat &Q )
      {
        *this = *this * Q;
        return *this;
      } /* End of 'operator*=' function */

      /* Quaternion division function.
       * ARGUMENTS:
       *   - quaternion:
       *       const quat &Q;
       * RETURNS:
       *   (quat) result quaternion.
       */
      quat operator/( const quat &Q ) const
      {
        return *this * Q.Inverse();
      } /* End of 'operator/' function */

      /* Quaternion division with assignment function.
       * ARGUMENTS:
       *   - quaternion:
       *       const quat &Q;
       * RETURNS:
       *   (quat &) result quaternion reference.
       */
      quat & operator/=( const quat &Q )
      {
        *this = *this / Q;
        return *this;
      } /* End of 'operator/=' function */

      /* Normalize this quaternion function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (quat &) normalized quaternion reference.
       */
      quat & Normalize( VOID )
      {
        type len = !*this;
        if (len != 0 && len != 1)
        {
          X /= len;
          Y /= len;
          Z /= len;
          W /= len;
        }
        return *this;
      } /* End of 'Normalize' function */

      /* Get normalized quaternion function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (quat) normalized quaternion.
       */
      quat Normalizing( VOID ) const
      {
        type len = !*this;
        if (len == 0 || len == 1)
          return *this;
        return quat(X / len, Y / len, Z / len, W / len);
      } /* End of 'Normalizing' function */

      /* Create rotation quaternion from axis and angle function.
       * ARGUMENTS:
       *   - axis:
       *       const vec3<type> &Axis;
       *   - angle in radians:
       *       type Angle;
       * RETURNS:
       *   (quat) rotation quaternion.
       */
      static quat Rotate( const vec3<type> &Axis, type Angle )
      {
        type HalfAngle = Angle * 0.5;
        type SinHalf = sin(HalfAngle);
    
        return quat(
          Axis.X * SinHalf,
          Axis.Y * SinHalf,
          Axis.Z * SinHalf,
          cos(HalfAngle)).Normalize();
      } /* End of 'Rotate' function */

      /* Obtain quaternion's i component function.
       * ARGUMENTS:
       *   - index:
       *       const INT i;
       * RETURNS:
       *   (type &) component reference.
       */
      type & operator[]( const INT i )
      {
        return i == 0 ? X : i == 1 ? Y : i == 2 ? Z : W;
      } /* End of 'operator[]' function */

      /* Obtain quaternion's i component function (const version).
       * ARGUMENTS:
       *   - index:
       *       const INT i;
       * RETURNS:
       *   (type) component value.
       */
      type operator[]( const INT i ) const
      {
        return i == 0 ? X : i == 1 ? Y : i == 2 ? Z : W;
      } /* End of 'operator[]' function */

      /* Obtain quaternion's first component's pointer function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (type *) quaternion component pointer.
       */
      operator type *( VOID )
      {
        return &X;
      } /* End of 'operator type *' function */
    }; /* End of 'quat' class */
} /* end of 'mth' namespace */

#endif /* __mth_quat_h_ */

/* END OF 'mth_quat.h' FILE */
