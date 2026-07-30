/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : walls_editor_system.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Walls editor declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kupchenkov Ivan (IK4).
 * LAST UPDATE : 12.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __walls_editor_system_h_
#define __walls_editor_system_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* Render class */
  class anim;

  /* Wall class */
  class wall;

  /* Point for wall editor class */
  class wall_point
  {
  public:
    vec2 Pos;                     // Wall point position
    vec3 PrimPoint1, PrimPoint2;  // Points for primitive

    /* Class default constructor */
    wall_point( VOID )
    {
    } /* End of 'wall_point' function */

    /* Class constructor.
     * ARGUMENTS:
     *   - point position:
     *       FLT X, Y;
     */
    wall_point( FLT X, FLT Y )
    {
      Pos = vec2(X, Y);
    } /* End of 'wall_point' function */

    BOOL operator==( wall_point P )
    {
      return (Pos.X == P.Pos.X && Pos.Y == P.Pos.Y);
    }

  }; /* End of 'wall_point' class */

  /* Wall class */
  class wall
  {
  public:
    wall_point P1, P2;  // Wall points
    prim WallPrim;      // Wall primitive

    /* Class default constructor */
    wall( VOID )
    {
    } /* End of 'wall' function */

    /* Class default destructor */
    ~wall( VOID )
    {
    } /* End of '~wall' function */

  }; /* End of 'wall' class */

  /* Walls editor manager class */
  class wall_editor
  {
  public:
    inline static FLT WallDepth = 0.8, WallHeight = 8.0, WallBaseY = -0.18;  // Walls params
    std::vector<wall> Walls;                                                 // All walls
    material_pattern WallMtlPat;                                             // Walls material pattern
    material WallMtl;                                                        // Walls material
    BOOL IsNewStart = TRUE;                                                  // Flag is the point is new start for walls
    anim *Ani;                                                          // Ref to render

    /* Class constructor.
     * ARGUMENTS:
     *   - ref to render:
     *       render &Rnd;
     */
    wall_editor( anim *AniP ) : Ani(AniP)
    {}

    /* Initialize walls editor function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Init( VOID );

    /* Deinitialize walls editor function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Close( VOID );

    /* Add wall point function.
     * ARGUMENTS:
     *   - point coords:
     *       FLT X, Y;
     * RETURNS: None.
     */
    VOID AddPoint( FLT X, FLT Y );

    /* Add wall new start point function.
     * ARGUMENTS:
     *   - point coords:
     *       FLT X, Y;
     * RETURNS: None.
     */
    VOID AddNewPoint( FLT X, FLT Y );

    /* Add wall function.
     * ARGUMENTS:
     *   - wall points:
     *       wall_point P1, P2;
     * RETURNS: None.
     */
    VOID AddWall( wall_point P1, wall_point P2 );

    /* Update walls height and plane Y coord function.
     * ARGUMENTS:
     *   - new height:
     *       FLT Height;
     *   - new base Y coord (-1 if no changing):
     *       FLT BaseY;
     * RETURNS: None.
     */
    VOID UpdateHeight( FLT Height, FLT BaseY = -1 );

  }; /* End of 'wall_editor' class */

} /* end of 'tam' namespace */

#endif // __walls_editor_system_h_

/* END OF 'walls_editor_system.h' FILE */
