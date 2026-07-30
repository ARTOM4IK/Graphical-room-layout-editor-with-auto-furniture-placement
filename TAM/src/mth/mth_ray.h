/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_ray.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math support module.
 *               Ray module implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 24.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_ray_h_
#define __mth_ray_h_

#include "mth_def.h"

/* 'mth' namespace */
namespace mth
{
  /* Ray representation type */
  template<typename type>
    class ray
    {
    public:
      vec3<type> Org, Dir; // Origin and direction vectors

      /* Type constructor.
       * ARGUMENTS: None.
       */
      ray( VOID )
      {
      } /* End of 'ray' function */

      /* Type constructor.
       * ARGUMENTS:
       *   - reference to the origin vector:
       *       const vec3<type> &O;
       *   - reference to the direction vector:
       *       const vec3<type> &D;
       */
      ray( const vec3<type> &O, const vec3<type> &D ) : Org(O), Dir(D.Normalizing())
      {
      } /* End of 'ray' function */

      /* Get point on the ray location function.
       * ARGUMENTS:
       *   - distance between origin and point:
       *       type T;
       * RETURNS:
       *   (vec3<type>) point on the ray.
       */
      vec3<type> operator()( type T ) const
      {
        return Org + Dir * T;
      } /* End of 'operator()' function */

      /* Get ray intersection with plane function.
       * ARGUMENTS:
       *   - plane normal:
       *       vec3 T;
       *   - distance between basis origin and plane:
       * RETURNS:
       *   (DBL) ray parameter.
       */
      DBL IntersectPlane( vec3<type> N = vec3<type>(0.0, 1.0, 0.0), DBL D = 0 ) const
      {
        DBL dot_dir, dot_org, t;

        dot_dir = Dir & N;

        if (abs(dot_dir) < Threshold)
          return 0;
        dot_org = Org & N;
        t = -dot_org / dot_dir;
        if (t > Threshold)
          return t;
        return 0;
      } /* End of 'IntersectPlane' function */

      /* Out put 3D vector function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID OutPut( VOID )
      {
        std::cout << "Origin vector: <" << Org.X << ", " << Org.Y << ", " << Org.Z << ">";
        std::cout << "Direction vector: <" << Dir.X << ", " << Dir.Y << ", " << Dir.Z << ">";
      } /* End of 'OutPut' function */

      /* Check ray AABB (axial aligned bound box) intersection function.
       * ARGUMENTS:
       *   - minimal and maximal AABB box vectors:
       *       const vec3<type> &MinBB, &MaxBB;
       * RETURNS:
       *   (BOOL) TRUE if ray intersects box, FALSE otherwise.
       */
      BOOL IsBoxIntersect( const vec3<type> &MinBB, const vec3<type> &MaxBB ) const
      {
        DBL t, tnear = 0, tfar = 0;
        INT tnear_no = -1, tfar_no = -1;

        for (INT i = 0; i < 3; i++)
        {
          // intersection by sides along 'i' axis
          if (Dir[i] != 0)
          {
            if (Dir[i] > 0)
            {
              // ray starts outside and go away
              if (Org[i] > MaxBB[i])
                return FALSE;
              // check if ray starts inside
              if (Org[i] < MinBB[i])
              {
                // 'tnear' is present
                t = (MinBB[i] - Org[i]) / Dir[i];
                if (tnear_no == -1 || tnear < t)
                  tnear = t, tnear_no = i * 2 + 0;
              }
              t = (MaxBB[i] - Org[i]) / Dir[i];
              if (tfar_no == -1 || tfar > t)
                tfar = t, tfar_no = i * 2 + 1;
            }
            else
            {
              // ray starts outside and go away
              if (Org[i] < MinBB[i])
                return FALSE;
              // check if ray starts inside
              if (Org[i] > MaxBB[i])
              {
                // 'tnear' is present
                t = (MaxBB[i] - Org[i]) / Dir[i];
                if (tnear_no == -1 || tnear < t)
                  tnear = t, tnear_no = i * 2 + 1;
              }
              t = (MinBB[i] - Org[i]) / Dir[i];
              if (tfar_no == -1 || tfar > t)
                tfar = t, tfar_no = i * 2 + 0;
            }
          }
        }
        // ray goes outside box
        if (tnear_no != -1 && tnear > tfar)
          return FALSE;
        return TRUE;
      } /* End of 'IsBoxIntersect' function */

      /* Check ray sphere intersection function.
       * ARGUMENTS:
       *   - sphere center position:
       *       const vec3<type> &C;
       *   - sphere radius:
       *       type R;
       * RETURNS:
       *   (BOOL) TRUE if ray intersects sphere, FALSE otherwise.
       */
      BOOL IsSphereIntersect( const vec3<type> &C, type R ) const
      {
        // Check for intersection
        vec3 OC = C - Org;
        DBL
          R2 = R * R,
          oc2 = OC & OC,
          ok = OC & Dir,
          ok2 = ok * ok,
          h2 = R2 - (oc2 - ok2);

        // Ray leaves sphere behind
        if (ok < 0)
        {
          // Check for ray starts outside sphere
          if (h2 < ok2)
            return FALSE;
          // Ray starts inside sphere
          return TRUE;
        }

        // Ray starts before sphere
        if (h2 < 0.00000001)
          return FALSE;

        // Ray intersects sphere
        return TRUE;
      } /* End of 'IsSphereIntersect' function */
    }; /* End of 'ray' class */
} /* end of 'mth' namespace */

#endif /* __mth_ray_h_ */

/* END OF 'mth_ray.h' FILE */
