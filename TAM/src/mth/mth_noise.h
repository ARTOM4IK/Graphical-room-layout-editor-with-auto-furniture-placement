/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_noise.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math support module.
 *               Perlin noise module declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 03.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_noise_h_
#define __mth_noise_h_

#include "mth_def.h"
#include "mth_vec2.h"

#define TAB_BITS 8
#define TAB_SIZE (1 << TAB_BITS)
#define TAB_MASK (TAB_SIZE - 1)

#define INDEX1(X) (Perm[(X) & TAB_MASK])
#define INDEX2(X, Y) (Perm[(X + INDEX1(Y)) & TAB_MASK])
#define INDEX3(X, Y, Z) (Perm[(X + INDEX2(Y, Z) & TAB_MASK)])

/* 'mth' namespace */
namespace mth
{
  /* 'noise' handle class */
  class noise
  {
    inline static DBL TabNoise[TAB_SIZE] {}; // noise array
    inline static INT Perm[TAB_SIZE] {};     // permutations array
    inline static INT NoiseOctaves = 1;      // count of octaves
  public:

    /* Class default constructor */
    noise( VOID );

    /* Obtain noise from 1 coordinate function.
     * ARGUMENTS:
     *   - coordinate:
     *       DBL X;
     * RETURNS:
     *   (DBL) result noise.
     */
    static DBL Noise1D( DBL X );

    /* 1D turb noise function.
     * ARGUMENTS:
     *   - Interpolation coefficient:
     *       DBL X;
     *   - Octaves:
     *       INT Octaves;
     * RETURNS:
     *   (DBL) value at this point.
     */
    static DBL NoiseTurb1D( DBL X, INT Octaves );

    /* 2D 1D turb noise function.
     * ARGUMENTS:
     *   - coordinate:
     *       DBL X;
     *   - Octaves:
     *       INT Octaves;
     * RETURNS:
     *   (vec2<DBL>) value at this point.
     */
    static mth::vec2<DBL> Noise2Turb1D( DBL X, INT Octaves );

    /* 2D noise function.
     * ARGUMENTS:
     *   - coordinates:
     *       DBL X, Y;
     * RETURNS:
     *   (DBL) value at this point.
     */
    static DBL Noise2D( DBL X, DBL Y );

    /* 2D turb noise function.
     * ARGUMENTS:
     *   - coordinates:
     *       DBL X, Y;
     *   - Octaves:
     *       INT Octaves;
     * RETURNS:
     *   (DBL) value at this point.
     */
    static DBL NoiseTurb2D( DBL X, DBL Y, INT Octaves = 1 );
  }; /* End of 'noise' class */
} /* end of 'mth' namespace */

#endif /* __mth_noise_h_ */

/* END OF 'mth_noise.h' FILE */
