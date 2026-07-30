/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_def.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math support module.
 *               Common math definitions file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 12.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_def_h_
#define __mth_def_h_

#include <cmath>

/* Define useful types for representation double and float information */
typedef double DBL;
typedef float FLT;

/* Pi math constant */
#define PI 3.14159265358979323846

/* Set threshold for calculations */
const DBL Threshold = 1e-8;

/* Degrees to radians conversion */
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)

/* Radians to degrees conversion */
#define R2D(A) (((A) * 180.0) / PI)
#define Radian2Degree(a) R2D(a)

/* Microdefinitions of useful random numbers generators */
#define Rand0() (DBL(rand()) / RAND_MAX)
#define Rand1() (1 - DBL(rand()) / RAND_MAX * 2)

/* 'mth' namespace */
namespace mth
{
  /* Clamp value function.
   * ARGUMENTS:
   *   - value:
   *       type val;
   *   - minimum border:
   *       type min;
   *   - maximum border:
   *       type max;
   * RETURNS:
   *   (type) clamped value.
   */
  template<typename type>
    type Clamp( type val, type min, type max )
    {
      return val < min ? min : val > max ? max : val;
    } /* End of 'Clamp' function */

  /* Get max value from two values function.
   * ARGUMENTS:
   *   - values:
   *       type A, type B;
   * RETURNS:
   *   (type) max value.
   */
  template<typename type>
    type Max( type A, type B )
    {
      return A > B ? A : B;
    } /* End of 'Max' function */

  /* Get min value from two values function.
   * ARGUMENTS:
   *   - values:
   *       type A, type B;
   * RETURNS:
   *   (type) min value.
   */
  template<typename type>
    type Min( type A, type B )
    {
      return A < B ? A : B;
    } /* End of 'Min' function */

  /* Normalize angle function.
   * ARGUMENTS:
   *   - angle:
   *       type Angle;
   * RETURNS:
   *   (type) normalized angle in [0; 360).
   */
  template<typename type>
    type NormalizeAngle( type Angle )
    {
      while (Angle < 0)
        Angle += 360;
      while (Angle >= 360)
        Angle -= 360;
      return Angle;
    } /* End of 'NormalizeAngle' function */
} /* end of 'mth' namespace */

#endif /* __mth_def_h_ */

/* END OF 'mth_def.h' FILE */
