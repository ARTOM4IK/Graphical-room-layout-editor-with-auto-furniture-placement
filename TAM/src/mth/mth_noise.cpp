/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_noise.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math support module.
 *               Perlin noise functions implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 03.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "mth_noise.h"

/* Class default constructor */
mth::noise::noise( VOID )
{
  /* Fill TabNoise array with some random numbers */
  for (INT i = 0; i < TAB_SIZE; i++)
  {
    TabNoise[i] = (DBL)rand() / RAND_MAX ;
    Perm[i]= i;
  }
  for (INT i = 0; i < TAB_SIZE * 8; i++)
  {
    INT
      a = rand() & TAB_MASK,
      b = rand() & TAB_MASK,
      tmp = Perm[a];

    Perm[a]= Perm[b];
    Perm[b] = tmp;
  }
} /* End of 'noise' function */

/* Obtain noise from 1 coordinate function.
  * ARGUMENTS:
  *   - coordinate:
  *       DBL X;
  * RETURNS:
  *   (DBL) result noise.
  */
DBL mth::noise::Noise1D( DBL X )
{
  INT ix = floor(X), ix1;
  DBL fx;

  fx = X - ix;
  fx = (3 - 2 * fx)* fx * fx;
  ix &= TAB_MASK;
  ix1 = (ix + 1) & TAB_MASK;
  return TabNoise[ix] * (1 - fx) + TabNoise[ix1] * fx;
} /* End of 'Noise1D' function */

/* 1D turb noise function.
 * ARGUMENTS:
 *   - coordinate:
 *       DBL X;
 *   - octaves:
 *       INT Octaves;
 * RETURNS:
 *   (DBL) value at this point.
 */
DBL mth::noise::NoiseTurb1D( DBL X, INT Octaves )
{
  INT i, frac = 1;
  DBL val = 0;

  for (i = 0; i < Octaves; i++)
  {
    val += Noise1D(X) / frac;
    X = (X + 18.47) * 2;
    frac *= 2;
  }
  return val * (1 << (Octaves - 1)) / ((1 << Octaves) - 1);
} /* End of 'mth::noise::NoiseTurb1D' function */

/* 2D 1D turb noise function.
  * ARGUMENTS:
  *   - coordinate:
  *       DBL X;
  *   - Octaves:
  *       INT Octaves;
  * RETURNS:
  *   (vec2<DBL>) value at this point.
  */
mth::vec2<DBL> mth::noise::Noise2Turb1D( DBL X, INT Octaves )
{
  return vec2<DBL>(NoiseTurb1D(X, Octaves), NoiseTurb1D(X * 1.8 + 30.47, Octaves));
} /* End of 'Noise2Turb1D' function */

/* 2D noise function.
 * ARGUMENTS:
 *   - coordinates:
 *       DBL X, Y;
 * RETURNS:
 *   (DBL) value at this point.
 */
DBL mth::noise::Noise2D( DBL X, DBL Y )
{
  INT ix = floor(X), ix1, iy = floor(Y), iy1;
  DBL fx, fy;

  fx = X - ix;
  fx = (3 - 2 * fx) * fx * fx;
  ix &= TAB_MASK;
  ix1 = (ix + 1) & TAB_MASK;
  fy = Y - iy;
  fy = (3 - 2 * fy) * fy * fy;
  iy &= TAB_MASK;
  iy1 = (iy + 1) & TAB_MASK;
  return
    TabNoise[INDEX2(iy, ix)] * (1 - fx) * (1 - fy) +
    TabNoise[INDEX2(iy, ix1)] * fx * (1 - fy) + 
    TabNoise[INDEX2(iy1, ix)] * (1 - fx) * fy +
    TabNoise[INDEX2(iy1, ix1)] * fx * fy;
} /* End of 'mth::noise::Noise2D' function */

/* 2D turb noise function.
 * ARGUMENTS:
 *   - coordinates:
 *       DBL X, Y;
 *   - Octaves:
 *       INT Octaves;
 * RETURNS:
 *   (DBL) value at this point.
 */
DBL mth::noise::NoiseTurb2D( DBL X, DBL Y, INT Octaves )
{
  INT i, frac = 1;
  DBL val = 0;

  for (i = 0; i < Octaves; i++)
  {
    val += Noise2D(X, Y) / frac;
    X = (X + 18.47) * 2;
    Y = (Y + 30.102) * 2;
    frac *= 2;
  }
  return val * (1 << (Octaves - 1)) / ((1 << Octaves) - 1);
} /* End of 'mth::noise::NoiseTurb2D' function */

/* END OF 'mth_noise.cpp' FILE */
