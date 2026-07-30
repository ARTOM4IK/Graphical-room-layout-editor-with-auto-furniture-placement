/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_cg_triangulation.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math support module.
 *               Computation geometry tasks.
 *               Polygon triangulation function.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 12.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_cg_triangulation_h_
#define __mth_cg_triangulation_h_

#include "mth_vec2.h"

/* 'mth' namespace */
namespace mth
{
  /* Check if point in triangle function.
   * ARGUMENTS:
   *   - triangle coordinates:
   *       const vec2<type> &P0, &P1, &P2;
   *   - point to check:
   *       const vec2<type> &Pnt;
   * RETURNS:
   *   (BOOL) TRUE if inside, FALSE otherwise.
   */
  template<typename type>
    BOOL IsPointInsideTriangle( const vec2<type> &P0, const vec2<type> &P1, const vec2<type> &P2,
                   const vec2<type> &Pnt )
    {
      DBL denom = (P1.Y - P2.Y) * (P0.X - P2.X) + (P2.X - P1.X) * (P0.Y - P2.Y);
      DBL u = ((P1.Y - P2.Y) * (Pnt.X - P2.X) + (P2.X - P1.X) * (Pnt.Y - P2.Y)) / denom;
      DBL v = ((P2.Y - P0.Y) * (Pnt.X - P2.X) + (P0.X - P2.X) * (Pnt.Y - P2.Y)) / denom;

      if (u >= 0 && u <= 1 && v >= 0 && v <= 1 && u + v <= 1)
        return TRUE;
      return FALSE;
    } /* End of 'IsInside' function */

  /* Check three points for counter-clock-wise order function.
   * ARGUMENTS:
   *   - three points to check:
   *       const vec2<type> &P0, &P1, &P2;
   * RETURNS:
   *   (BOOL) TRUE if order is counter-clock-wise, FALSE is clock-wise.
   */
  template<typename type>
    BOOL IsCCW( const vec2<type> &P0, const vec2<type> &P1, const vec2<type> &P2 )
    {
      if ((P2.Y - P0.Y) * (P2.X + P0.X) -
          (P1.Y - P0.Y) * (P1.X + P0.X) -
          (P2.Y - P1.Y) * (P2.X + P1.X) > 0)
        return TRUE;
      return FALSE;
    } /* End of 'IsCCW' function */

  /* Check point stock for counter-clock-wise order function .
   * ARGUMENTS:
   *   - point stock to check:
   *       std::vector<vec2<type>> &Pnts;
   * RETURNS:
   *   (BOOL) TRUE if order is counter-clock-wise, FALSE is clock-wise.
   */
  template<typename type>
    static BOOL IsCCW( const std::vector<vec2<type>> &Pnts )
    {
      DBL sum = 0;

      for (INT i = 0; i < Pnts.size(); i++)
      {
        INT n2 = (i + 1) % Pnts.size();

        sum += (Pnts[i].Y - Pnts[n2].Y) * (Pnts[i].X + Pnts[n2].X);
      }
      if (sum > 0)
        return TRUE;
      return FALSE;
    } /* End of 'IsCCW' function */

  /* Triangulate array of points function .
   * ARGUMENTS:
   *   - point stock to triangulate:
   *       std::vector<vec2<type>> &Pnts;
   * RETURNS:
   *   (std::vector<INT>) result array of triplet indexes (size 0 if error).
   */
  template<typename type>
    static std::vector<INT> Triangulate( const std::vector<vec2<type>> &Pnts )
    {
      std::vector<INT> Ind;
      std::vector<INT> pi;
      INT sub = 0;

      if (Pnts.size() < 3)
        return {};
      if ((Pnts[0] - Pnts[Pnts.size() - 1]).Length2() < Threshold)
        sub = 1;

      // Copy start point indexes
      if (IsCCW(Pnts))
        for (INT i = 0; i < Pnts.size() - sub; i++)
          pi.push_back(i);
      else
        for (INT i = (INT)Pnts.size() - 1 - sub; i >= 0; i--)
          pi.push_back(i);

      // While all points are not handled
      while (pi.size() > 2)
      {
        std::vector<INT> pnew;
        INT i;

        for (i = 0; i < pi.size(); i++)
        {
          INT
            n1 = (i + 1) % pi.size(),
            n2 = (i + 2) % pi.size();

          // Select current tree neighbour points (must be CCW triangle)
          if (IsCCW(Pnts[pi[i]], Pnts[pi[n1]], Pnts[pi[n2]]))
          {
            // auto &[P0, P1, P2] = {Pnts[pi[i]], Pnts[pi[n1]], Pnts[pi[n2]]};
            INT j;

            // Check all other points to not lie in selected triangle
            for (j = 0; j < pi.size(); j++)
              if (j != i && j != n1 && j != n2 &&
                  IsPointInsideTriangle(Pnts[pi[i]], Pnts[pi[n1]], Pnts[pi[n2]], Pnts[pi[j]]))
                break;
            if (j == pi.size())
            {
              // Add a triangle
              //INT a[3] = {pi[i], pi[n1], pi[n2]};

              Ind.push_back(pi[i]);
              Ind.push_back(pi[n1]);
              Ind.push_back(pi[n2]);
              for (INT k = 0; k < pi.size(); k++)
                if (k != n1)
                  pnew.push_back(pi[k]);
              break;
            }
          }
        }
        if (i == pi.size())
        {
          i = 0;
          return {};
        }
        pi = pnew;
      }
      return Ind;
    } /* End of 'Triangulate' function */
} /* end of 'mth' namespace */

#endif /* __mth_cg_triangulation_h_ */

/* END OF 'mth_cg_triangulation.h' FILE */

