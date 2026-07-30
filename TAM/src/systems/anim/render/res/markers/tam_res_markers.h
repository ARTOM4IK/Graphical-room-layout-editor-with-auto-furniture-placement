/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_markers.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Marker system declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 23.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_res_markers_h_
#define __tam_res_markers_h_

#include "../primitives/tam_res_primitives.h"
#include "../material_patterns/tam_res_material_pattern.h"
#include "../materials/tam_res_material.h"

/* 'tam' namespace */
namespace tam
{
  // Start marker buffer size    
  constexpr INT INITIAL_MARKER_CAPACITY = 1024;

  /* 'marker_manager' handle class */
  class marker_manager
  {
  public:
    // Number of different supported markers
    static const int NumOfMarkerTypes = 5;

  private:
    /* Sphere marker buffer entry structure */
    struct BUF_MARKER_SPHERE
    {
      vec4 CR, Color; // Sphere center and radius, color
    }; /* End of 'BUF_MARKER_SPHERE' structure */

    /* Cylinder marker buffer entry structure */
    struct BUF_MARKER_CYLINDER
    {
      vec4 PR0, PR1, Color; // Cylinder enpoints and radiuses, color
    }; /* End of 'BUF_MARKER_CYLINDER' structure */

    /* AABB marker buffer entry structure */
    struct BUF_MARKER_AABB
    {
      vec4 P0, P1, Color; // AABB lower an upper corners, color
    }; /* End of 'BUF_MARKER_AABB' structure */

    /* AABB marker with transform buffer entry structure */
    struct BUF_MARKER_AABB_MATR
    {
      vec4 P0, P1, Color;        // AABB lower an upper corners, color
      matr::matr_data Transform; // AABB transformation matrix
    }; /* End of 'BUF_MARKER_AABB_MATR' structure */

    /* Line marker buffer entry structure */
    struct BUF_MARKER_LINE
    {
      vec4 P0, P1, Color; // Line points, color
    }; /* End of 'BUF_MARKER_LINE' structure */


    // Reference to render
    render &RndRef;

    // Spheres store stock (Center/Radius,Color)
    std::vector<std::tuple<vec4, vec4>> Spheres;

    // Cylinder store stock (P0/Radius,P1/Radius,Color)
    std::vector<std::tuple<vec4, vec4, vec4>> Cylinders;

    // AABB store stock (P0,P1,Color)
    std::vector<std::tuple<vec4, vec4, vec4>> AABBs;

    // AABB with matrix store stock (P0,P1,Color,Transform)
    std::vector<std::tuple<matr::matr_data, vec4, vec4, vec4>> AABBMatrs;

    // Lines store stock (P0/0,P1/0,Color)
    std::vector<std::tuple<vec4, vec4, vec4>> Lines;

    // Markers shaders
    shader ShaderMarkers[NumOfMarkerTypes];

    // Markers material patterns
    material_pattern MtlPatMarkers[vlk::core::UsedFramesOnTheFly][NumOfMarkerTypes] {};
    // Markers materials
    material MtlMarkers[vlk::core::UsedFramesOnTheFly][NumOfMarkerTypes] {};

    // Merkers buffers and sizes
    buffer BufferMarkers[vlk::core::UsedFramesOnTheFly][NumOfMarkerTypes] {};
    INT BufferMarkerSize[vlk::core::UsedFramesOnTheFly][NumOfMarkerTypes] {};

  public:

    /* Class default constructor.
     * ARGUMENTS:
     *   - render reference:
     *       render &Rnd;
     */
    marker_manager( render &Rnd ) : RndRef(Rnd)
    {
    } /* End of 'marker_manager' function */

    /* Class destructor */
    ~marker_manager( VOID )
    {
    } /* End of '~marker_manager' function */

    /* Marker system initialization function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Init( VOID );

    /* Marker system initialization function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Close( VOID )
    {
    } /* End of 'Close' function */

    /* Get markers count function.
     * ARGUMENTS:
     *   - markers stock no (-1 - all, 0 - spheres, 1 - cylinders):
     *       INT MrkStockNo;
     * RETURNS:
     *   (INT) markers count.
     */
    INT GetMarkersCount( INT MrkStockNo )
    {
      if (MrkStockNo == 0)
        return (INT)Spheres.size();
      if (MrkStockNo == 1)
        return (INT)Cylinders.size();
      if (MrkStockNo == 2)
        return (INT)AABBs.size();
      if (MrkStockNo == 3)
        return (INT)AABBMatrs.size();
      if (MrkStockNo == 4)
        return (INT)Lines.size();
      return (INT)Spheres.size() + (INT)Cylinders.size() + (INT)AABBs.size() + (INT)AABBMatrs.size() + (INT)Lines.size();
    } /* End of 'GetMarkersCount' function */

    /* Draw all markers function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID DrawAllMarkers( VOID );

    /* Draw sphere marker function.
     * ARGUMENTS:
     *   - sphere center point:
     *       const vec3 &C;
     *   - sphere radius:
     *       FLT R;
     *   - sphere color:
     *       const vec4 &Color;
     * RETURNS: None.
     */
    VOID DrawSphere( const vec3 &C, FLT R, const vec4 &Color )
    {
      Spheres.push_back({Color, {C, R}});
    } /* End of 'DrawSphere' function */

    /* Draw cylinder marker function.
     * ARGUMENTS:
     *   - cylinder first base center point:
     *       const vec3 &P0;
     *   - cylinder first base radius:
     *       FLT R0;
     *   - cylinder second base center point:
     *       const vec3 &P1;
     *   - cylinder second base radius:
     *       FLT R1;
     *   - cylinder color:
     *       const vec4 &Color;
     * RETURNS: None.
     */
    VOID DrawCylinder( const vec3 &P0, FLT R0, const vec3 &P1, FLT R1, const vec4 &Color )
    {
      Cylinders.push_back({Color, {P1, R1}, {P0, R0}});
    } /* End of 'DrawCylinder' function */

    /* Draw AABB marker function.
     * ARGUMENTS:
     *   - AABB corners points:
     *       const vec3 &P0, &P1;
     *   - AABB color:
     *       const vec4 &Color;
     * RETURNS: None.
     */
    VOID DrawAABB( const vec3 &P0, const vec3 &P1, const vec4 &Color )
    {
      AABBs.push_back({Color, {P1, 0}, {P0, 0}});
    } /* End of 'DrawAABB' function */

    /* Draw AABB with matrix marker function.
     * ARGUMENTS:
     *   - AABB corners points:
     *       const vec3 &P0, &P1;
     *   - AABB color:
     *       const vec4 &Color;
     *   - AABB transformation matrix:
     *       const matr &Transform;
     * RETURNS: None.
     */
    VOID DrawAABBMatr( const vec3 &P0, const vec3 &P1, const vec4 &Color, const matr &Transform )
    {
      AABBMatrs.push_back({Transform, Color, {P1, 0}, {P0, 0}});
    } /* End of 'DrawAABBMatr' function */

    /* Draw cylinder marker function.
     * ARGUMENTS:
     *   - line endpoints:
     *       const vec3 &P0, &P1;
     *   - line color:
     *       const vec4 &Color;
     * RETURNS: None.
     */
    VOID DrawLine( const vec3 &P0, const vec3 &P1, const vec4 &Color )
    {
      Lines.push_back({Color, {P1, 0}, {P0, 0}});
    } /* End of 'DrawLine' function */
  }; /* End of 'marker_manager' class */
} /* end of 'tam' namespace */

#endif /* __tam_res_markers_h_ */

/* END OF 'tam_res_markers.h' FILE */
