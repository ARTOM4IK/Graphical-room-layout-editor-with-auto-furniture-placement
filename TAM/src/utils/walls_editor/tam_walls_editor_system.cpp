/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : walls_editor_system.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Walls editor implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kupchenkov Ivan (IK4).
 * LAST UPDATE : 12.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_walls_editor_system.h"

/* Initialize walls editor function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::wall_editor::Init( VOID )
{
  shader Shd = Ani->ShaderCreate("default");
  WallMtlPat = Ani->MaterialPatternCreate(Shd, "Walls pattern", 1, 0, {}, {});
  WallMtl = Ani->MaterialCreate("Walls material", WallMtlPat);
} /* End of 'tam::wall_editor::Init' function */

/* Deinitialize walls editor function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::wall_editor::Close( VOID )
{
  Ani->MaterialPatternDelete(WallMtlPat);
  Ani->MaterialDelete(WallMtl);
  Walls.clear();
} /* End of 'tam::wall_editor::Close' function */

/* Add wall point function.
 * ARGUMENTS:
 *   - point coords:
 *       FLT X, Y;
 * RETURNS: None.
 */
VOID tam::wall_editor::AddPoint( FLT X, FLT Y )
{
  if (Walls.size() == 0)
  {
    Walls.resize(1);
    wall_point P1(X, Y);
    Walls[0].P1 = P1;
    Walls[0].P2 = P1;
    vec3 P = vec3(P1.Pos.X, WallBaseY, P1.Pos.Y);
    std::vector<vertex_std> Vertices 
    {
      vertex_std
      {
        P + vec3(WallDepth, 0, WallDepth),
        vec2(0, 0),
        vec3(0, 0, -1),
        vec4(0.89, 0.8, 0.74, 1)
      },
      vertex_std
      {
        P + vec3(WallDepth, 0, -WallDepth),
        vec2(0, 1),
        vec3(0, 0, -1),
        vec4(0.89, 0.8, 0.74, 1)
      },
      vertex_std
      {
        P + vec3(-WallDepth, 0, -WallDepth),
        vec2(1, 1),
        vec3(0, 0, -1),
        vec4(0.89, 0.8, 0.74, 1)
      },
      vertex_std
      {
        P + vec3(-WallDepth, 0, WallDepth),
        vec2(1, 0),
        vec3(0, 0, -1),
        vec4(0.89, 0.8, 0.74, 1)
      },
      vertex_std
      {
        P + vec3(WallDepth, WallHeight, WallDepth),
        vec2(0, 0),
        vec3(0, 0, -1),
        vec4(0.89, 0.8, 0.74, 1)
      },
      vertex_std
      {
        P + vec3(WallDepth, WallHeight, -WallDepth),
        vec2(0, 1),
        vec3(0, 0, -1),
        vec4(0.89, 0.8, 0.74, 1)
      },
      vertex_std
      {
        P + vec3(-WallDepth, WallHeight, -WallDepth),
        vec2(1, 1),
        vec3(0, 0, -1),
        vec4(0.89, 0.8, 0.74, 1)
      },
      vertex_std
      {
        P + vec3(-WallDepth, WallHeight, WallDepth),
        vec2(1, 0),
        vec3(0, 0, -1),
        vec4(0.89, 0.8, 0.74, 1)
      }
    };

    std::vector<INT> Ind = 
    {
      0, 1, 2, 0, 2, 3,
      
      0, 1, 5, 0, 5, 4,
      1, 2, 5, 2, 5, 6,
      2, 3, 7, 2, 6, 7,
      3, 0, 7, 7, 0, 4,
      
      4, 5, 6, 4, 7, 6
    };

    Walls[0].WallPrim = Ani->PrimitiveCreate(WallMtl, std::span(Vertices.data(), Vertices.size()), std::span(Ind.data(), Ind.size()) );
  }
  else if ((Walls.size() == 1 && Walls[0].P1 == Walls[0].P2) || (IsNewStart && Walls[Walls.size() - 1].P1 == Walls[Walls.size() - 1].P2))
  {
    INT LastNum = (INT)Walls.size() - 1;
    Ani->PrimitiveDelete(Walls[LastNum].WallPrim);

    Walls[LastNum].P2 = wall_point(X, Y);
    vec3 Pp1 = vec3(Walls[LastNum].P1.Pos.X, WallBaseY, Walls[LastNum].P1.Pos.Y);
    vec3 Pp2 = vec3(Walls[LastNum].P2.Pos.X, WallBaseY, Walls[LastNum].P2.Pos.Y);
    vec3 Pvec = Pp2 - Pp1;
    vec3 PP = (Pvec % vec3(0, 1, 0)).Normalize() * WallDepth;

    wall_point P1;
    P1.Pos = Walls[LastNum].P1.Pos;
    P1.PrimPoint1 = (Pp1 + PP) - (Pvec.Normalize() * WallDepth);
    P1.PrimPoint2 = (Pp1 - PP) - (Pvec.Normalize() * WallDepth);

    wall_point P2;
    P2.Pos = Walls[LastNum].P2.Pos;
    P2.PrimPoint1 = (Pp2 + PP) + (Pvec.Normalize() * WallDepth);
    P2.PrimPoint2 = (Pp2 - PP) + (Pvec.Normalize() * WallDepth);

    Walls.resize(LastNum);
    AddWall(P1, P2);
  }
  else
  {
    INT LastNum = (INT)Walls.size() - 1;
    wall_point P1 = Walls[LastNum].P1;
    wall_point P2 = Walls[LastNum].P2;
    wall_point P3 = wall_point(X, Y);
    vec2 DPos12 = (P2.Pos - P1.Pos).Normalize();
    vec2 DPos23 = (P3.Pos - P2.Pos).Normalize();
    vec3 P2PP1 = vec3(P2.PrimPoint1.X - P2.Pos.X, P2.PrimPoint1.Y, P2.PrimPoint1.Z - P2.Pos.Y);
    vec3 P2PP2 = vec3(P2.PrimPoint2.X - P2.Pos.X, P2.PrimPoint2.Y, P2.PrimPoint2.Z - P2.Pos.Y);
    if (((DPos12.X/DPos23.X) == (DPos12.Y/DPos23.Y)) || (abs((DPos12.X/DPos23.X) - (DPos12.Y/DPos23.Y)) < 0.08))
    {
      P3.PrimPoint1 = vec3(P3.Pos.X + P2PP1.X, P2PP1.Y, P3.Pos.Y + P2PP1.Z);
      P3.PrimPoint2 = vec3(P3.Pos.X + P2PP2.X, P2PP2.Y, P3.Pos.Y + P2PP2.Z);
      P2.PrimPoint1 = P2.PrimPoint1 - (vec3(DPos23.X, 0, DPos23.Y).Normalize() * WallDepth);
      P2.PrimPoint2 = P2.PrimPoint2 - (vec3(DPos23.X, 0, DPos23.Y).Normalize() * WallDepth);
      Walls.resize(LastNum);
      AddWall(P1, P2);
      AddWall(P2, P3);
    }
    else
    {
      vec3 Pp2 = vec3(P2.Pos.X, WallBaseY, P2.Pos.Y);
      vec3 Pp3 = vec3(P3.Pos.X, WallBaseY, P3.Pos.Y);
      vec3 Pvec = Pp3 - Pp2;
      vec3 PP = (Pvec % vec3(0, 1, 0)).Normalize() * WallDepth;
  
      P3.PrimPoint1 = (Pp3 + PP) + (Pvec.Normalize() * WallDepth);
      P3.PrimPoint2 = (Pp3 - PP) + (Pvec.Normalize() * WallDepth);

      vec2 Pb12 = P2.Pos - P1.Pos;
      vec2 Pb32 = P2.Pos - P3.Pos;
      
      vec2 P1P2Line1 = vec2(P1.PrimPoint1.X + Pb12.X * 100, P1.PrimPoint1.Z + Pb12.Y * 100);
      vec2 P3P2Line1 = vec2(P3.PrimPoint1.X + Pb32.X * 100, P3.PrimPoint1.Z + Pb32.Y * 100);
      vec2 Point1 = vec2::LinesIntersection(vec2(P1.PrimPoint1.X, P1.PrimPoint1.Z), P1P2Line1, vec2(P3.PrimPoint1.X, P3.PrimPoint1.Z), P3P2Line1);
      
      vec2 P1P2Line2 = vec2(P1.PrimPoint2.X + Pb12.X * 100, P1.PrimPoint2.Z + Pb12.Y * 100);
      vec2 P3P2Line2 = vec2(P3.PrimPoint2.X + Pb32.X * 100, P3.PrimPoint2.Z + Pb32.Y * 100);
      vec2 Point2 = vec2::LinesIntersection(vec2(P1.PrimPoint2.X, P1.PrimPoint2.Z), P1P2Line2, vec2(P3.PrimPoint2.X, P3.PrimPoint2.Z), P3P2Line2);
      
      P2.PrimPoint1 = vec3(Point1.X, WallBaseY, Point1.Y);
      P2.PrimPoint2 = vec3(Point2.X, WallBaseY, Point2.Y);

      Walls.resize(LastNum);
      AddWall(P1, P2);
      AddWall(P2, P3);
    }
  }

} /* End of 'tam::wall_editor::AddPoint' function */

/* Add wall new start point function.
 * ARGUMENTS:
 *   - point coords:
 *       FLT X, Y;
 * RETURNS: None.
 */
VOID tam::wall_editor::AddNewPoint( FLT X, FLT Y )
{
  wall Wall;
  wall_point P1(X, Y);
  Wall.P1 = P1;
  Wall.P2 = P1;
  vec3 P = vec3(P1.Pos.X, WallBaseY, P1.Pos.Y);
  std::vector<vertex_std> Vertices 
  {
    vertex_std
    {
      P + vec3(WallDepth, 0, WallDepth),
      vec2(0, 0),
      vec3(0, 0, -1),
      vec4(0.89, 0.8, 0.74, 1)
    },
    vertex_std
    {
      P + vec3(WallDepth, 0, -WallDepth),
      vec2(0, 1),
      vec3(0, 0, -1),
      vec4(0.89, 0.8, 0.74, 1)
    },
    vertex_std
    {
      P + vec3(-WallDepth, 0, -WallDepth),
      vec2(1, 1),
      vec3(0, 0, -1),
      vec4(0.89, 0.8, 0.74, 1)
    },
    vertex_std
    {
      P + vec3(-WallDepth, 0, WallDepth),
      vec2(1, 0),
      vec3(0, 0, -1),
      vec4(0.89, 0.8, 0.74, 1)
    },
    vertex_std
    {
      P + vec3(WallDepth, WallHeight, WallDepth),
      vec2(0, 0),
      vec3(0, 0, -1),
      vec4(0.89, 0.8, 0.74, 1)
    },
    vertex_std
    {
      P + vec3(WallDepth, WallHeight, -WallDepth),
      vec2(0, 1),
      vec3(0, 0, -1),
      vec4(0.89, 0.8, 0.74, 1)
    },
    vertex_std
    {
      P + vec3(-WallDepth, WallHeight, -WallDepth),
      vec2(1, 1),
      vec3(0, 0, -1),
      vec4(0.89, 0.8, 0.74, 1)
    },
    vertex_std
    {
      P + vec3(-WallDepth, WallHeight, WallDepth),
      vec2(1, 0),
      vec3(0, 0, -1),
      vec4(0.89, 0.8, 0.74, 1)
    }
  };

  std::vector<INT> Ind = 
  {
    0, 1, 2, 0, 2, 3,

    0, 1, 5, 0, 5, 4,
    1, 2, 5, 2, 5, 6,
    2, 3, 7, 2, 6, 7,
    3, 0, 7, 7, 0, 4,

    4, 5, 6, 4, 7, 6
  };

  Wall.WallPrim = Ani->PrimitiveCreate(WallMtl, std::span(Vertices.data(), Vertices.size()), std::span(Ind.data(), Ind.size()) );
  Walls.push_back(Wall);
} /* End of 'tam::wall_editor::AddNewPoint' function */

/* Add wall function.
 * ARGUMENTS:
 *   - wall points:
 *       wall_point P1, P2;
 * RETURNS: None.
 */
VOID tam::wall_editor::AddWall( wall_point P1, wall_point P2 )
{
  wall Wall;
  vec3 P10 = P1.PrimPoint1;
  vec3 P11 = P1.PrimPoint2;
  Wall.P1.PrimPoint1 = P10;
  Wall.P1.PrimPoint2 = P11;
  Wall.P1.Pos = P1.Pos;
  vec3 P20 = P2.PrimPoint1;
  vec3 P21 = P2.PrimPoint2;
  Wall.P2.PrimPoint1 = P20;
  Wall.P2.PrimPoint2 = P21;
  Wall.P2.Pos = P2.Pos;

  vec3 P10Y = vec3(P10.X, WallBaseY + WallHeight, P10.Z);
  vec3 P11Y = vec3(P11.X, WallBaseY + WallHeight, P11.Z);
  vec3 P20Y = vec3(P20.X, WallBaseY + WallHeight, P20.Z);
  vec3 P21Y = vec3(P21.X, WallBaseY + WallHeight, P21.Z);

  std::vector<vertex_std> Vertices 
  {
    vertex_std
    {
      P10,
      vec2(0, 0),
      vec3(0, 0, -1),
      vec4(0.89 * 0.47, 0.8 * 0.47, 0.74 * 0.47, 1)
    },
    vertex_std
    {
      P11,
      vec2(0, 1),
      vec3(0, 0, -1),
      vec4(0.89, 0.8, 0.74, 1)
    },
    vertex_std
    {
      P21,
      vec2(1, 1),
      vec3(0, 0, -1),
      vec4(0.89, 0.8, 0.74, 1)
    },
    vertex_std
    {
      P20,
      vec2(1, 0),
      vec3(0, 0, -1),
      vec4(0.89 * 0.47, 0.8 * 0.47, 0.74 * 0.47, 1)
    },
    vertex_std
    {
      P10Y,
      vec2(0, 0),
      vec3(0, 0, -1),
      vec4(0.89 * 0.47, 0.8 * 0.47, 0.74 * 0.47, 1)
    },
    vertex_std
    {
      P11Y,
      vec2(0, 1),
      vec3(0, 0, -1),
      vec4(0.89, 0.8, 0.74, 1)
    },
    vertex_std
    {
      P21Y,
      vec2(1, 1),
      vec3(0, 0, -1),
      vec4(0.89, 0.8, 0.74, 1)
    },
    vertex_std
    {
      P20Y,
      vec2(1, 0),
      vec3(0, 0, -1),
      vec4(0.89 * 0.47, 0.8 * 0.47, 0.74 * 0.47, 1)
    }
  };

  std::vector<INT> Ind = 
  {
    0, 1, 2, 0, 2, 3,
    
    0, 1, 5, 0, 5, 4,
    1, 2, 5, 2, 5, 6,
    2, 3, 7, 2, 6, 7,
    3, 0, 7, 7, 0, 4,
    
    4, 5, 6, 4, 7, 6
  };

  Wall.WallPrim = Ani->PrimitiveCreate(WallMtl, std::span(Vertices.data(), Vertices.size()), std::span(Ind.data(), Ind.size()));
  Walls.push_back(Wall);
} /* End of 'tam::wall_editor::AddWall' function */

/* Update walls height function.
 * ARGUMENTS:
 *   - new Height:
 *       FLT Height;
 *   - new base Y coord (-1 if no changing):
 *       FLT BaseY;
 * RETURNS: None.
 */
VOID tam::wall_editor::UpdateHeight( FLT Height, FLT BaseY )
{
  WallHeight = Height;
  for (auto &Wall : Walls)
  {
    Ani->PrimitiveDelete(Wall.WallPrim);
    wall_point P1 = Wall.P1;
    wall_point P2 = Wall.P2;

    vec3 P10;
    vec3 P11;
    vec3 P20;
    vec3 P21;

    if (BaseY > 0)
    {
      P10 = P1.PrimPoint1 + vec3(0, -WallBaseY + BaseY, 0);
      P11 = P1.PrimPoint2 + vec3(0, -WallBaseY + BaseY, 0);
      P20 = P2.PrimPoint1 + vec3(0, -WallBaseY + BaseY, 0);
      P21 = P2.PrimPoint2 + vec3(0, -WallBaseY + BaseY, 0);
      WallBaseY = BaseY;
    }
    else
    {
      P10 = P1.PrimPoint1;
      P11 = P1.PrimPoint2;
      P20 = P2.PrimPoint1;
      P21 = P2.PrimPoint2;
    }
    vec3 P10Y = P10 + vec3(0, WallHeight, 0);
    vec3 P11Y = P11 + vec3(0, WallHeight, 0);
    vec3 P20Y = P20 + vec3(0, WallHeight, 0);
    vec3 P21Y = P21 + vec3(0, WallHeight, 0);

    std::vector<vertex_std> Vertices 
    {
      vertex_std
      {
        P10,
        vec2(0, 0),
        vec3(0, 0, -1),
        vec4(0.89 * 0.47, 0.8 * 0.47, 0.74 * 0.47, 1)
      },
      vertex_std
      {
        P11,
        vec2(0, 1),
        vec3(0, 0, -1),
        vec4(0.89, 0.8, 0.74, 1)
      },
      vertex_std
      {
        P21,
        vec2(1, 1),
        vec3(0, 0, -1),
        vec4(0.89, 0.8, 0.74, 1)
      },
      vertex_std
      {
        P20,
        vec2(1, 0),
        vec3(0, 0, -1),
        vec4(0.89 * 0.47, 0.8 * 0.47, 0.74 * 0.47, 1)
      },
      vertex_std
      {
        P10Y,
        vec2(0, 0),
        vec3(0, 0, -1),
        vec4(0.89 * 0.47, 0.8 * 0.47, 0.74 * 0.47, 1)
      },
      vertex_std
      {
        P11Y,
        vec2(0, 1),
        vec3(0, 0, -1),
        vec4(0.89, 0.8, 0.74, 1)
      },
      vertex_std
      {
        P21Y,
        vec2(1, 1),
        vec3(0, 0, -1),
        vec4(0.89, 0.8, 0.74, 1)
      },
      vertex_std
      {
        P20Y,
        vec2(1, 0),
        vec3(0, 0, -1),
        vec4(0.89 * 0.47, 0.8 * 0.47, 0.74 * 0.47, 1)
      }
    };

    std::vector<INT> Ind = 
    {
      0, 1, 2, 0, 2, 3,
      
      0, 1, 5, 0, 5, 4,
      1, 2, 5, 2, 5, 6,
      2, 3, 7, 2, 6, 7,
      3, 0, 7, 7, 0, 4,
      
      4, 5, 6, 4, 7, 6
    };

    Wall.WallPrim = Ani->PrimitiveCreate(WallMtl, std::span(Vertices.data(), Vertices.size()), std::span(Ind.data(), Ind.size()));
  }
} /* End of 'tam::wall_editor::UpdateHeight' function */

/* END OF 'walls_editor_system.cpp' FILE */