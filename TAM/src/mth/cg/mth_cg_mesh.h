/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_cg_mesh.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math support module.
 *               Computation geometry: indexed triangle mesh handles.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 09.04.2026
 * NOTE        : Namespace 'mth::cg'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_cg_mesh_h_
#define __mth_cg_mesh_h_

#include <vector>

#include "../mth_vec3.h"

/* 'mth' namespace */
namespace mth
{
  /* Computation geometry: collision tests and acceleration structures */
  namespace cg
  {
    /* One triangle: three indices into a vertex array */
    struct triangle
    {
      INT V0, V1, V2; /* Triangle vertex indices */

      /* Class default constructor */
      triangle( VOID ) : V0(0), V1(0), V2(0)
      {
      } /* End of 'triangle' function */

      /* Initialize three corner indices constructor.
       * ARGUMENTS:
       *   - vertices indices:
       *       INT I0, I1, I2;
       * RETURNS: None.
       */
      triangle( INT I0, INT I1, INT I2 ) : V0(I0), V1(I1), V2(I2)
      {
      } /* End of 'triangle' function */
    }; /* End of 'triangle' struct */

    /* Indexed surface: vertex buffer plus triangle index list */
    template<typename type>
      struct mesh
      {
        /* Vertex positions in model space */
        std::vector<vec3<type>> Vertices;
        /* Faces as index triplets into Vertices */
        std::vector<triangle> Triangles;

        /* Class default constructor */
        mesh( VOID ) = default;

        /* Get number of vertices function.
         * ARGUMENTS: None.
         * RETURNS:
         *   (INT) vertex count.
         */
        INT VertexCount( VOID ) const
        {
          return (INT)Vertices.size();
        } /* End of 'VertexCount' function */

        /* Get number of triangles function.
         * ARGUMENTS: None.
         * RETURNS:
         *   (INT) triangle count.
         */
        INT TriangleCount( VOID ) const
        {
          return (INT)Triangles.size();
        } /* End of 'TriangleCount' function */
      }; /* End of 'mesh' struct */
  } /* end of 'cg' namespace */
} /* end of 'mth' namespace */

#endif /* __mth_cg_mesh_h_ */

/* END OF 'mth_cg_mesh.h' FILE */
