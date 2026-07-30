/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_flat.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Flat system functions implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kupchenkov Ivan (IK4),
 *               Novozhilova Maryana (MN7),
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 01.05.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* 'tam' namespace */
namespace tam
{
  /* Remove duplicate room polygon points function.
   * ARGUMENTS:
   *   - source polygon:
   *       std::vector<vec2> &Polygon;
   * RETURNS: None.
   */
  VOID NormalizeRoomPolygonPoints( std::vector<vec2> &Polygon )
  {
    std::vector<vec2> Result {};

    for (auto &P : Polygon)
      if (Result.empty() || Result.back().Distance(P) > 1e-4)
        Result.push_back(P);
    if (Result.size() > 1 && Result.front().Distance(Result.back()) < 1e-4)
      Result.pop_back();
    Polygon = Result;
  } /* End of 'NormalizeRoomPolygonPoints' function */

  /* Check if room polygon is simple function.
   * ARGUMENTS:
   *   - source polygon:
   *       const std::vector<vec2> &Polygon;
   * RETURNS:
   *   (BOOL) simplicity flag.
   */
  BOOL IsSimpleRoomPolygon( const std::vector<vec2> &Polygon )
  {
    if (Polygon.size() < 3)
      return FALSE;
    if (fabs(vec2::Zero().SignedPolygonArea(Polygon)) < 1e-4)
      return FALSE;

    for (INT i = 0; i < (INT)Polygon.size(); i++)
    {
      INT ni = (i + 1) % (INT)Polygon.size();

      for (INT j = i + 1; j < (INT)Polygon.size(); j++)
      {
        INT nj = (j + 1) % (INT)Polygon.size();

        if (i == j || i == nj || ni == j || ni == nj)
          continue;
        if (vec2::Zero().IsSegmentIntersectSegment(Polygon[i], Polygon[ni], Polygon[j], Polygon[nj]))
          return FALSE;
      }
    }
    return TRUE;
  } /* End of 'IsSimpleRoomPolygon' function */

  /* Build fallback ordered room polygon function.
   * ARGUMENTS:
   *   - source flat:
   *       const flat &Flat;
   *   - source room:
   *       const room &Room;
   * RETURNS:
   *   (std::vector<vec2>) resulting polygon.
   */
  std::vector<vec2> BuildFallbackRoomPolygon( const flat &Flat, const room &Room )
  {
    std::vector<vec2> Polygon {};
    vec2 Center = vec2::Zero();

    for (auto &PointId : Room.RoomPoints)
      if (Flat.SegmentPoints.contains(PointId))
      {
        vec2 P = Flat.SegmentPoints.at(PointId).Pos;

        if (std::find_if(Polygon.begin(), Polygon.end(),
              [&P]( const vec2 &Q )
              {
                return Q.Distance(P) < 1e-4;
              }) == Polygon.end())
          Polygon.push_back(P);
      }
    if (Polygon.size() < 3)
      return {};

    Center = vec2::Zero().PolygonCenter(Polygon);
    std::stable_sort(Polygon.begin(), Polygon.end(),
      [&Center]( const vec2 &A, const vec2 &B )
      {
        return atan2(A.Y - Center.Y, A.X - Center.X) < atan2(B.Y - Center.Y, B.X - Center.X);
      });
    vec2::Zero().EnsureCCW(Polygon);
    return Polygon;
  } /* End of 'BuildFallbackRoomPolygon' function */
} /* end of 'tam' namespace */

/* Get ordered room polygon function.
 * ARGUMENTS:
 *   - room name:
 *       const std::string &RoomName;
 *   - resulting polygon:
 *       std::vector<vec2> &Polygon;
 * RETURNS:
 *   (BOOL) build result.
 */
BOOL tam::flat::GetRoomPolygon( const std::string &RoomName, std::vector<vec2> &Polygon ) const
{
  Polygon.clear();
  if (!Rooms.contains(RoomName))
    return FALSE;

  for (auto &PointId : Rooms.at(RoomName).RoomPoints)
    if (SegmentPoints.contains(PointId))
      Polygon.push_back(SegmentPoints.at(PointId).Pos);
  NormalizeRoomPolygonPoints(Polygon);
  if (!IsSimpleRoomPolygon(Polygon))
  {
    Polygon = BuildFallbackRoomPolygon(*this, Rooms.at(RoomName));
    NormalizeRoomPolygonPoints(Polygon);
  }
  if (!IsSimpleRoomPolygon(Polygon))
    return FALSE;
  vec2::Zero().EnsureCCW(Polygon);
  return TRUE;
} /* End of 'tam::flat::GetRoomPolygon' function */

/* Flat destructor. */
tam::flat::~flat( VOID )
{
  if (Ani == nullptr)
    return;

  // Clear runtime-loaded flat content first.
  Clear(FALSE);

  // Release default resources created in flat::Create().
  texture DefaultWallpaper {};
  if (DefaultFlatMtl != nullptr && !DefaultFlatMtl->Textures.empty())
    DefaultWallpaper = DefaultFlatMtl->Textures[0];

  if (SchemeMaterial != nullptr)
    Ani->MaterialDelete(SchemeMaterial);
  if (DefaultFlatMtl != nullptr)
    Ani->MaterialDelete(DefaultFlatMtl);
  if (DefaultFlatMtlPat != nullptr)
    Ani->MaterialPatternDelete(DefaultFlatMtlPat);
  if (DefaultWallpaper != nullptr)
    Ani->TextureDelete(DefaultWallpaper);

  SchemeMaterial = nullptr;
  DefaultFlatMtl = nullptr;
  DefaultFlatMtlPat = nullptr;
  Ani = nullptr;
} /* End of 'tam::flat::~flat' function */

/* Create flat function.
 * ARGUMENTS:
 *   - animation context pointer:
 *       anim *Ani;
 * RETURNS: None.
 */
VOID tam::flat::Create( anim *Ani )
{
  this->Ani = Ani;
  DefaultFlatMtlPat = Ani->MaterialPatternCreate(Ani->ShaderCreate("defpbr"), "Flat default material pattern", 8, 1, {}, {});

  BUF_MTL_PBR BufMtlPBR
  {
    .BaseColor = vec4(0.6, 0.6, 0.55, 1),
    .MetallicRoughnessScale = vec3(0.18, 0.8, 1.0),
    .TextureFlags = {0, 0, 0, 0, 0, 0, 0, 0}
  };
  mth::vec4<BYTE> Data = mth::vec4<BYTE>(255);
  texture Tex = Ani->TextureCreate("Default wallpaper", 1, 1, 4, Data, TRUE, TRUE, FALSE);
  Wallpapers.insert({Tex->Name, Tex});
  DefaultFlatMtl = Ani->MaterialCreate("Flat default material",
                                       DefaultFlatMtlPat, {Tex},
                                       {Ani->BufferCreate<BUF_MTL_PBR>(&BufMtlPBR, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)});
  BufMtlPBR.BaseColor = vec4(1);
  BufMtlPBR.TextureFlags[0] = 1;
  SchemeMaterial = Ani->MaterialCreate("Flat scheme material", DefaultFlatMtlPat, {}, {Ani->BufferCreate<BUF_MTL_PBR>(&BufMtlPBR, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)});
} /* End of 'tam::flat::Create' function */

/* Update flat material function.
 * ARGUMENTS:
 *   - new walls color:
 *       vec3 NewColor;
 *   - textures to be applied:
 *       std::vector<texture> Texs;
 * RETURNS: None.
 */
VOID tam::flat::UpdateFlatMaterial( vec3 NewColor, std::vector<texture> Texs )
{
  BUF_MTL_PBR BufMtlPBR
  {
    .BaseColor = vec4(NewColor, 1),
    .MetallicRoughnessScale = vec3(0.18, 0.8, 1.0),
  };
  INT i;
  for (i = 0; i < Texs.size(); i++)
  {
    if (Texs[i] != nullptr)
      BufMtlPBR.TextureFlags[i] = 1;
    else
      break;
  }
  for (; i < 8; i++)
    BufMtlPBR.TextureFlags[i] = 0;
  DefaultFlatMtl->Update(Texs, {Ani->BufferCreate<BUF_MTL_PBR>(&BufMtlPBR, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)});
} /* End of 'tam::flat::UpdateFlatMaterial' function */

/* Clear flat informaton function.
 * ARGUMENTS:
 *   - flag for creating texture after clearing:
 *       BOOL IsToCreate;
 * RETURNS: None.
 */
VOID tam::flat::Clear( BOOL IsToCreate )
{
  // Break render pipeline draw links (material / primitive lists) before
  // destroying GPU resources so shared_ptr refcounts can reach zero.
  if (Ani != nullptr)
    Ani->ClearDrawQueue();

  // Delete scheme primitive if present.
  if (SchemePrim != nullptr)
    Ani->PrimitiveDelete(SchemePrim);

  // Delete wall primitives.
  for (auto &[WallId, Wall] : SegmentWalls)
  {
    if (Wall.WallPrim != nullptr)
      Ani->PrimitiveDelete(Wall.WallPrim);
    // Detach room materials from wall entries so MaterialDelete can
    // drop corresponding resources from material manager stock.
    Wall.WallMaterial = DefaultFlatMtl;
  }

  // Explicitly release room-owned render resources.
  for (auto &[RoomName, Room] : Rooms)
  {
    if (Room.RoomFloor.FloorPrim != nullptr)
      Ani->PrimitiveDelete(Room.RoomFloor.FloorPrim);
    if (Room.RoomCeiling.FloorPrim != nullptr)
      Ani->PrimitiveDelete(Room.RoomCeiling.FloorPrim);
    if (Room.RoomFloor.FloorMtl != nullptr)
      Ani->MaterialDelete(Room.RoomFloor.FloorMtl);
    if (Room.RoomCeiling.FloorMtl != nullptr)
      Ani->MaterialDelete(Room.RoomCeiling.FloorMtl);
    if (Room.WallsMaterial != nullptr)
      Ani->MaterialDelete(Room.WallsMaterial);
  }

  // Delete loaded furniture models.
  for (auto &[FurnitureName, FurnitureData] : FlatFurnitureCollection)
    if (FurnitureData.second != nullptr)
      Ani->ModelDelete(FurnitureData.second);

  // Delete loaded wallpapers for this flat.
  // Keep default wallpaper alive because it is referenced by DefaultFlatMtl.
  for (auto &[TexName, Tex] : Wallpapers)
    if (TexName != "Default wallpaper" && Tex != nullptr)
      Ani->TextureDelete(Tex);

  /* Clear all maps and restore default data */
  SchemePrim = nullptr;
  SchemeScale = 0.0;
  SchemeTranslate = vec3(0.0);
  SchemeTex = "";
  SchemeRotate = 0;
  Rooms.clear();
  SegmentPointsCounter = 0;
  SegmentPoints.clear();
  SegmentsCounter = 0;
  Segments.clear();
  SegmentWallsCounter = 0;
  SegmentWalls.clear();
  FlatFurnitureCollection.clear();
  Wallpapers.clear();
  WallsWidth = 0.1;
  WallsHeight = 3.0;

  if (IsToCreate)
  {
    texture Tex = Ani->TextureCreate("Default wallpaper", 1, 1, 4, mth::vec4<BYTE>(255), TRUE, TRUE, FALSE);
    Wallpapers.insert({Tex->Name, Tex});
  }
} /* End of 'tam::flat::Clear' function */

/* Update segment data function.
 * ARGUMENTS:
 *   - index of segment to updata data of:
 *       INT SegmentInd;
 * RETURNS: None.
 */
VOID tam::flat::UpdateSegmentLayout( INT SegmentInd )
{
  segment Segment = Segments[SegmentInd];
  vec3
    P0 = vec3(SegmentPoints[Segment.Point1].Pos.X, 0, SegmentPoints[Segment.Point1].Pos.Y),
    P1 = vec3(SegmentPoints[Segment.Point2].Pos.X, 0, SegmentPoints[Segment.Point2].Pos.Y);

  vec2 P0P1 = SegmentPoints[Segment.Point2].Pos - SegmentPoints[Segment.Point1].Pos;
  vec3 P0B00 = (vec3(0, 1, 0) % vec3(P0P1.X, 0, P0P1.Y).Normalizing()) * WallsWidth / 2.0;
  vec2
    B00 = SegmentPoints[Segment.Point1].Pos + vec2(P0B00.X, P0B00.Z),
    B01 = SegmentPoints[Segment.Point1].Pos - vec2(P0B00.X, P0B00.Z),
    B10 = SegmentPoints[Segment.Point2].Pos + vec2(P0B00.X, P0B00.Z),
    B11 = SegmentPoints[Segment.Point2].Pos - vec2(P0B00.X, P0B00.Z);
  std::vector<vec2>
    v1 = {SegmentPoints[Segment.Point1].Pos, B00, B10, SegmentPoints[Segment.Point2].Pos},
    v2 = {SegmentPoints[Segment.Point1].Pos, B01, B11, SegmentPoints[Segment.Point2].Pos};

  // Create wall 1
  if (!SegmentWalls.contains(Segments[SegmentInd].Wall1))
  {
    Segments[SegmentInd].Wall1 = SegmentWallsCounter;
    SegmentWalls[Segments[SegmentInd].Wall1].BasePoints = v1;
    SegmentWalls[Segments[SegmentInd].Wall1].WallMaterial = DefaultFlatMtl;
    CreateWallPrimBy4Points(SegmentInd, 1, v1);
    SegmentWallsCounter++;
  }

  // Create wall 2
  if (!SegmentWalls.contains(Segments[SegmentInd].Wall2))
  {
    Segments[SegmentInd].Wall2 = SegmentWallsCounter;
    SegmentWalls[Segments[SegmentInd].Wall2].BasePoints = v2;
    SegmentWalls[Segments[SegmentInd].Wall2].WallMaterial = DefaultFlatMtl;
    CreateWallPrimBy4Points(SegmentInd, 2, v2);
    SegmentWallsCounter++;
  }

  if (SegmentPoints[Segments[SegmentInd].Point1].SegmentIndices.size() == 1 &&
      SegmentPoints[Segments[SegmentInd].Point2].SegmentIndices.size() == 1)
  {
    std::vector<vec2>
      w1 = {SegmentPoints[Segments[SegmentInd].Point1].Pos, B00,
        B10, SegmentPoints[Segments[SegmentInd].Point2].Pos},
      w2 = {SegmentPoints[Segments[SegmentInd].Point1].Pos, B01,
        B11, SegmentPoints[Segments[SegmentInd].Point2].Pos};
    CreateWallPrimBy4Points(SegmentInd, 1, w1);
    CreateWallPrimBy4Points(SegmentInd, 2, w2);
  }
  else if (SegmentPoints[Segments[SegmentInd].Point1].SegmentIndices.size() == 1)
  {
    std::vector<vec2>
      w1 = {SegmentPoints[Segments[SegmentInd].Point1].Pos, B00,
        SegmentWalls[Segments[SegmentInd].Wall1].BasePoints[2], SegmentPoints[Segments[SegmentInd].Point2].Pos},
      w2 = {SegmentPoints[Segments[SegmentInd].Point1].Pos, B01,
        SegmentWalls[Segments[SegmentInd].Wall2].BasePoints[2], SegmentPoints[Segments[SegmentInd].Point2].Pos};
    CreateWallPrimBy4Points(SegmentInd, 1, w1);
    CreateWallPrimBy4Points(SegmentInd, 2, w2);
  }
  else if (SegmentPoints[Segments[SegmentInd].Point2].SegmentIndices.size() == 1)
  {
    std::vector<vec2>
      w1 = {SegmentPoints[Segments[SegmentInd].Point1].Pos, SegmentWalls[Segments[SegmentInd].Wall1].BasePoints[1],
        B10, SegmentPoints[Segments[SegmentInd].Point2].Pos},
      w2 = {SegmentPoints[Segments[SegmentInd].Point1].Pos, SegmentWalls[Segments[SegmentInd].Wall2].BasePoints[1],
        B11, SegmentPoints[Segments[SegmentInd].Point2].Pos};
    CreateWallPrimBy4Points(SegmentInd, 1, w1);
    CreateWallPrimBy4Points(SegmentInd, 2, w2);
  }
  else
  {
    std::vector<vec2>
      w1 = {SegmentPoints[Segments[SegmentInd].Point1].Pos, SegmentWalls[Segments[SegmentInd].Wall1].BasePoints[1],
        SegmentWalls[Segments[SegmentInd].Wall1].BasePoints[2], SegmentPoints[Segments[SegmentInd].Point2].Pos},
      w2 = {SegmentPoints[Segments[SegmentInd].Point1].Pos, SegmentWalls[Segments[SegmentInd].Wall2].BasePoints[1],
        SegmentWalls[Segments[SegmentInd].Wall2].BasePoints[2], SegmentPoints[Segments[SegmentInd].Point2].Pos};
    CreateWallPrimBy4Points(SegmentInd, 1, w1);
    CreateWallPrimBy4Points(SegmentInd, 2, w2);
  }

  Segments[SegmentInd].angles1.clear();
  Segments[SegmentInd].angles2.clear();

  if ((UINT)SegmentPoints[Segment.Point1].SegmentIndices.size() > 1)
  {
    for (auto &ind : SegmentPoints[Segment.Point1].SegmentIndices)
    {
      if (ind != SegmentInd && Segments.contains(ind) && Segments[ind].Point1 != -1 && Segments[ind].Point2 != -1)
      {
        segment NewSegment = Segments[ind];
        vec3 P0P1 = P1 - P0, P0P2 = vec3(0);
        if (NewSegment.Point1 == Segment.Point1)
          P0P2 = vec3(SegmentPoints[NewSegment.Point2].Pos.X, 0, SegmentPoints[NewSegment.Point2].Pos.Y) - P0;
        else
          P0P2 = vec3(SegmentPoints[NewSegment.Point1].Pos.X, 0, SegmentPoints[NewSegment.Point1].Pos.Y) - P0;
        vec3 vp = P0P2 % P0P1;
        FLT sc = P0P1 & P0P2, len = !P0P1 * !P0P2, angle = 0;
        if (vp.Y > 0)
          angle = R2D(acos(sc / len));
        else
          angle = 360 - R2D(acos(sc / len));

        Segments[SegmentInd].angles1.push_back(std::pair(ind, angle));
        if (NewSegment.Point1 == Segment.Point1)
        {
          Segments[ind].angles1.push_back(std::pair(SegmentInd, 360 - angle));
          std::sort(Segments[ind].angles1.begin(), Segments[ind].angles1.end(),
            []( const std::pair<INT, FLT> &A,
                const std::pair<INT, FLT> &B )
            {
              return A.second < B.second;
            });
        }
        else
        {
          Segments[ind].angles2.push_back(std::pair(SegmentInd, 360 - angle));
          std::sort(Segments[ind].angles2.begin(), Segments[ind].angles2.end(),
            []( const std::pair<INT, FLT> &A,
                const std::pair<INT, FLT> &B )
            {
              return A.second < B.second;
            });
        }
      }
    }
    std::sort(Segments[SegmentInd].angles1.begin(), Segments[SegmentInd].angles1.end(),
      []( const std::pair<INT, FLT> &A,
          const std::pair<INT, FLT> &B )
      {
        return A.second < B.second;
      });

    if ((UINT)Segments[SegmentInd].angles1.size() > 0)
    {
      UpdateSegmentWalls(Segments[SegmentInd].angles1[0].first, SegmentInd,
        Segments[SegmentInd].angles1[0].second <= 180 ? 1 : 0);
      if ((UINT)Segments[SegmentInd].angles1.size() > 1)
        UpdateSegmentWalls(Segments[SegmentInd].angles1[(UINT)Segments[SegmentInd].angles1.size() - 1].first, SegmentInd,
          Segments[SegmentInd].angles1[(UINT)Segments[SegmentInd].angles1.size() - 1].second > 180 ? 1 : 0);
      else
        UpdateSegmentWalls(Segments[SegmentInd].angles1[0].first, SegmentInd,
          Segments[SegmentInd].angles1[0].second >= 180 ? 1 : 0);
    }
  }

  if ((UINT)SegmentPoints[Segment.Point2].SegmentIndices.size() > 1)
  {
    for (auto &ind : SegmentPoints[Segment.Point2].SegmentIndices)
    {
      if (ind != SegmentInd && Segments.contains(ind) && Segments[ind].Point1 != -1 && Segments[ind].Point2 != -1)
      {
        segment NewSegment = Segments[ind];
        vec3 P1P0 = P0 - P1, P1P2 = vec3(0);
        if (NewSegment.Point1 == Segment.Point2)
          P1P2 = vec3(SegmentPoints[NewSegment.Point2].Pos.X, 0, SegmentPoints[NewSegment.Point2].Pos.Y) - P1;
        else
          P1P2 = vec3(SegmentPoints[NewSegment.Point1].Pos.X, 0, SegmentPoints[NewSegment.Point1].Pos.Y) - P1;
        vec3 vp = P1P2 % P1P0;
        FLT sc = P1P0 & P1P2, len = !P1P0 * !P1P2, angle = 0;
        if (vp.Y > 0)
          angle = R2D(acos(sc / len));
        else
          angle = 360 - R2D(acos(sc / len));
        Segments[SegmentInd].angles2.push_back(std::pair(ind, angle));
        if (NewSegment.Point1 == Segment.Point2)
        {
          Segments[ind].angles1.push_back(std::pair(SegmentInd, 360 - angle));
          std::sort(Segments[ind].angles1.begin(), Segments[ind].angles1.end(),
            []( const std::pair<INT, FLT> &A,
                const std::pair<INT, FLT> &B )
            {
              return A.second < B.second;
            });
        }
        else
        {
          Segments[ind].angles2.push_back(std::pair(SegmentInd, 360 - angle));
          std::sort(Segments[ind].angles2.begin(), Segments[ind].angles2.end(),
            []( const std::pair<INT, FLT> &A,
                const std::pair<INT, FLT> &B )
            {
              return A.second < B.second;
            });
        }
      }
    }
    std::sort(Segments[SegmentInd].angles2.begin(), Segments[SegmentInd].angles2.end(),
      []( const std::pair<INT, FLT> &A,
          const std::pair<INT, FLT> &B )
      {
        return A.second < B.second;
      });

    if ((UINT)Segments[SegmentInd].angles2.size() > 0)
    {
      UpdateSegmentWalls(Segments[SegmentInd].angles2[0].first, SegmentInd,
        Segments[SegmentInd].angles2[0].second <= 180 ? 1 : 0);
      if((UINT)Segments[SegmentInd].angles2.size() > 1)
        UpdateSegmentWalls(Segments[SegmentInd].angles2[(UINT)Segments[SegmentInd].angles2.size() - 1].first, SegmentInd,
          Segments[SegmentInd].angles2[(UINT)Segments[SegmentInd].angles2.size() - 1].second > 180 ? 1 : 0);
      else
        UpdateSegmentWalls(Segments[SegmentInd].angles2[0].first, SegmentInd,
          Segments[SegmentInd].angles2[0].second >= 180 ? 1 : 0);
    }
  }
} /* End of 'tam::flat::UpdateSegmentLayout' function */

/* Update (or create) segment walls function.
 * ARGUMENTS:
 *   - indices of segments to be updated:
 *       INT SegmentInd1, SegmentInd2;
 *   - is inner angle needed or outer:
 *       BOOL IsInner;
 * RETURNS: None.
 */
VOID tam::flat::UpdateSegmentWalls( INT SegmentInd1, INT SegmentInd2, BOOL IsInner )
{
  segment Segment1 = Segments[SegmentInd1], Segment2 = Segments[SegmentInd2];
  segment_point
    p0 = SegmentPoints[Segment1.Point1], p1 = SegmentPoints[Segment1.Point2],
    p2 = SegmentPoints[Segment2.Point1], p3 = SegmentPoints[Segment2.Point2], P0, P1, P2;
  // 1 - as supposed to be, -1 - vice versa
  INT IsOtherwise1 = 1, IsOtherwise2 = 1;
  vec2 B00, B01, B10, B11, B20, B21;
  if (Segment2.Point1 == Segment1.Point2)
    P0 = p0, P1 = p1, P2 = p3;
  else if (Segment2.Point2 == Segment1.Point2)
    P0 = p0, P1 = p1, P2 = p2, IsOtherwise2 = -1;
  else if (Segment2.Point1 == Segment1.Point1)
    P0 = p1, P1 = p0, P2 = p3, IsOtherwise1 = -1;
  else if (Segment2.Point2 == Segment1.Point1)
    P0 = p1, P1 = p0, P2 = p2, IsOtherwise1 = -1, IsOtherwise2 = -1;

  vec2 P1P0 = P0.Pos - P1.Pos;
  vec2 P1P2 = P2.Pos - P1.Pos;

  vec3 P0B00 = -(vec3(0, 1, 0) % vec3(P1P0.X, 0, P1P0.Y)).Normalizing() * WallsWidth / 2.0;
  vec3 P2B20 = (vec3(0, 1, 0) % vec3(P1P2.X, 0, P1P2.Y)).Normalizing() * WallsWidth / 2.0;

  B10 = P1.Pos + vec2(P0B00.X, P0B00.Z), B11 = P1.Pos - vec2(P0B00.X, P0B00.Z);

  vec3 vp = vec3(P1P2.X, 0, P1P2.Y) % vec3(P1P0.X, 0, P1P0.Y);
  FLT sc = P1P0 & P1P2, len = !P1P0 * !P1P2, angle = 0;
  if (vp.Y > 0)
    angle = R2D(acos(sc / len));
  else
    angle = 360 - R2D(acos(sc / len));

  FLT cosa1 = cos(D2R(angle));

  BOOL IsB11In = TRUE;
  FLT tg = 0, d = 0;
  if (abs(cosa1) <= 0.99)
    tg = 1 / tan(acos(cosa1) / 2.0);
  d = -WallsWidth / 2.0 * tg;
  if (B10.Distance(P0.Pos) + B10.Distance(P2.Pos) < P1.Pos.Distance(P0.Pos) + P1.Pos.Distance(P2.Pos))
    d = -d, IsB11In = FALSE;
  B10 += P1P0.Normalizing() * d, B11 -= P1P0.Normalizing() * d;

  // 1
  if (IsOtherwise1 == 1 && IsOtherwise2 == 1)
  {
    if ((IsInner && IsB11In) || (!IsInner && !IsB11In))
    {
      CreateWallPrimByUpdatedBasePoints(SegmentInd1, 2, B11, 2);
      CreateWallPrimByUpdatedBasePoints(SegmentInd2, 2, B11, 1);
    }
    else if ((IsInner && !IsB11In) || (!IsInner && IsB11In))
    {
      CreateWallPrimByUpdatedBasePoints(SegmentInd1, 1, B10, 2);
      CreateWallPrimByUpdatedBasePoints(SegmentInd2, 1, B10, 1);
    }
  }
  // 2
  else if (IsOtherwise1 != 1 && IsOtherwise2 == 1)
  {
    if (IsInner && IsB11In || (!IsInner && !IsB11In))
    {
      CreateWallPrimByUpdatedBasePoints(SegmentInd1, 1, B11, 1);
      CreateWallPrimByUpdatedBasePoints(SegmentInd2, 2, B11, 1);
    }
    else if ((IsInner && !IsB11In) || (!IsInner && IsB11In))
    {
      CreateWallPrimByUpdatedBasePoints(SegmentInd1, 2, B10, 1);
      CreateWallPrimByUpdatedBasePoints(SegmentInd2, 1, B10, 1);
    }
  }
  // 3
  else if (IsOtherwise1 == 1 && IsOtherwise2 != 1)
  {
    if (IsInner && IsB11In || (!IsInner && !IsB11In))
    {
      CreateWallPrimByUpdatedBasePoints(SegmentInd1, 2, B11, 2);
      CreateWallPrimByUpdatedBasePoints(SegmentInd2, 1, B11, 2);
    }
    else if ((IsInner && !IsB11In) || (!IsInner && IsB11In))
    {
      CreateWallPrimByUpdatedBasePoints(SegmentInd1, 1, B10, 2);
      CreateWallPrimByUpdatedBasePoints(SegmentInd2, 2, B10, 2);
    }
  }
  else
  {
    if (IsInner && IsB11In || (!IsInner && !IsB11In))
    {
      CreateWallPrimByUpdatedBasePoints(SegmentInd1, 1, B11, 1);
      CreateWallPrimByUpdatedBasePoints(SegmentInd2, 1, B11, 2);
    }
    else if ((IsInner && !IsB11In) || (!IsInner && IsB11In))
    {
      CreateWallPrimByUpdatedBasePoints(SegmentInd1, 2, B10, 1);
      CreateWallPrimByUpdatedBasePoints(SegmentInd2, 2, B10, 2);
    }
  }
} /* End of 'tam::flat::UpdateSegmentWalls' function */

/* Create wall primitive by 4 (floor) points function.
 * ARGUMENTS:
 *   - index of segment to be updated:
 *       INT SegmentInd;
 *   - index of wall in this segment to be updated:
 *       INT WallNo;
 *   - vertices to be used:
 *       std::vector<vec2> vertices;
 *   - texture coordinates offset (depending on the previously created walls):
 *       FLT TexCoordOffset;
 *   - texture scaling coefficient:
 *       vec2 TexScale;
 * RETURNS: None.
 */
VOID tam::flat::CreateWallPrimBy4Points( INT SegmentInd, INT WallNo, std::vector<vec2> vertices,
                                         FLT TexCoordOffset, vec2 TexScale )
{
  INT WallInd = WallNo == 1 ? Segments[SegmentInd].Wall1 : Segments[SegmentInd].Wall2;
  if (Segments[SegmentInd].Type == segment_type::eWIN)
  {
    SegmentWalls[WallInd].BasePoints = vertices;
    UpdateWallPrimWindow(SegmentInd, WallNo, TexCoordOffset);
  }
  else if (Segments[SegmentInd].Type == segment_type::eDOOR)
  {
    SegmentWalls[WallInd].BasePoints = vertices;
    UpdateWallPrimDoor(SegmentInd, WallNo, TexCoordOffset);
  }
  else
  {
    vec3 back_normal {}, left_normal {}, right_normal {};
    // -(5 - 6) % (5 - 1)
    back_normal = ((vec3(vertices[1].X, WallsHeight, vertices[1].Y) - vec3(vertices[2].X, WallsHeight, vertices[2].Y)) %
      (vec3(vertices[1].X, WallsHeight, vertices[1].Y) - vec3(vertices[1].X, 0, vertices[1].Y))).Normalize();
    // -(4 - 5) % (4 - 0)
    left_normal = ((vec3(vertices[0].X, WallsHeight, vertices[0].Y) - vec3(vertices[1].X, WallsHeight, vertices[1].Y)) %
      (vec3(vertices[0].X, WallsHeight, vertices[0].Y) - vec3(vertices[0].X, 0, vertices[0].Y))).Normalize();
    left_normal *= (left_normal & (vec3(vertices[3].X - vertices[0].X, 0, vertices[3].Y - vertices[0].Y))) > 0 ? -1 : 1;
    // (6 - 7) % (6 - 2)
    right_normal = ((vec3(vertices[2].X, WallsHeight, vertices[2].Y) - vec3(vertices[3].X, WallsHeight, vertices[3].Y)) %
      (vec3(vertices[2].X, WallsHeight, vertices[2].Y) - vec3(vertices[2].X, 0, vertices[2].Y))).Normalize();
    right_normal *= (right_normal & (vec3(vertices[3].X - vertices[0].X, 0, vertices[3].Y - vertices[0].Y))) > 0 ? -1 : 1;
    FLT WallLength = !(vertices[2] - vertices[1]);
    FLT
      LeftTexCoord = TexCoordOffset,
      RightTexCoord = (WallLength) / TexScale.X + TexCoordOffset,
      TopTexCoord = WallsHeight / TexScale.Y;

    std::vector<vertex_std> PrimVertices
    {
      // Bottom side
      // 0
      {
        {vertices[0].X, 0, vertices[0].Y},
        {0, 0},
        {0, -1, 0},
        {1, 0, 0, 1},
      },
      // 1
      {
        {vertices[1].X, 0, vertices[1].Y},
        {0, 0},
        {0, -1, 0},
        {1, 0, 0, 1},
      },
      // 2
      {
        {vertices[2].X, 0, vertices[2].Y},
        {0, 0},
        {0, -1, 0},
        {1, 0, 0, 1},
      },
      // 3
      {
        {vertices[3].X, 0, vertices[3].Y},
        {0, 0},
        {0, -1, 0},
        {1, 0, 0, 1},
      },

      // Left side
      // 0
      {
        {vertices[0].X, 0, vertices[0].Y},
        {0, 0},
        left_normal,
        {1, 0, 0, 1},
      },
      // 4
      {
        {vertices[0].X, WallsHeight, vertices[0].Y},
        {0, 0},
        left_normal,
        {1, 0, 0, 1},
      },
      // 5
      {
        {vertices[1].X, WallsHeight, vertices[1].Y},
        {0, 0},
        left_normal,
        {1, 0, 0, 1},
      },
      // 1
      {
        {vertices[1].X, 0, vertices[1].Y},
        {0, 0},
        left_normal,
        {1, 0, 0, 1},
      },

      // Back side
      // 1
      {
        {vertices[1].X, 0, vertices[1].Y},
        {LeftTexCoord, 0},
        back_normal,
        {1, 0, 0, 1},
      },
      // 5
      {
        {vertices[1].X, WallsHeight, vertices[1].Y},
        {LeftTexCoord, TopTexCoord},
        back_normal,
        {1, 0, 0, 1},
      },
      // 6
      {
        {vertices[2].X, WallsHeight, vertices[2].Y},
        {RightTexCoord, TopTexCoord},
        back_normal,
        {1, 0, 0, 1},
      },
      // 2
      {
        {vertices[2].X, 0, vertices[2].Y},
        {RightTexCoord, 0},
        back_normal,
        {1, 0, 0, 1},
      },

      // Right side
      // 3
      {
        {vertices[3].X, 0, vertices[3].Y},
        {0, 0},
        right_normal,
        {1, 0, 0, 1},
      },
      // 7
      {
        {vertices[3].X, WallsHeight, vertices[3].Y},
        {0, 0},
        right_normal,
        {1, 0, 0, 1},
      },
      // 6
      {
        {vertices[2].X, WallsHeight, vertices[2].Y},
        {0, 0},
        right_normal,
        {1, 0, 0, 1},
      },
      // 2
      {
        {vertices[2].X, 0, vertices[2].Y},
        {0, 0},
        right_normal,
        {1, 0, 0, 1},
      },

      // Front side
      // 0
      {
        {vertices[0].X, 0, vertices[0].Y},
        {0, 0},
        -back_normal,
        {1, 0, 0, 1},
      },
      // 4
      {
        {vertices[0].X, WallsHeight, vertices[0].Y},
        {0, 0},
        -back_normal,
        {1, 0, 0, 1},
      },
      // 7
      {
        {vertices[3].X, WallsHeight, vertices[3].Y},
        {0, 0},
        -back_normal,
        {1, 0, 0, 1},
      },
      // 3
      {
        {vertices[3].X, 0, vertices[3].Y},
        {0, 0},
        -back_normal,
        {1, 0, 0, 1},
      },

      // Top side
      // 4
      {
        {vertices[0].X, WallsHeight, vertices[0].Y},
        {0, 0},
        {0, 1, 0},
        {1, 0, 0, 1},
      },
      // 5
      {
        {vertices[1].X, WallsHeight, vertices[1].Y},
        {0, 0},
        {0, 1, 0},
        {1, 0, 0, 1},
      },
      // 6
      {
        {vertices[2].X, WallsHeight, vertices[2].Y},
        {0, 0},
        {0, 1, 0},
        {1, 0, 0, 1},
      },
      // 7
      {
        {vertices[3].X, WallsHeight, vertices[3].Y},
        {0, 0},
        {0, 1, 0},
        {1, 0, 0, 1},
      },
    };

    std::vector<INT> indices
    {
      // Bottom side
      0, 1, 2, 0, 2, 3,
      // Left side
      4, 6, 5, 4, 7, 6,
      // Back side
      8, 9, 10, 8, 10, 11,
      // Right side
      12, 13, 14, 12, 14, 15,
      // Top side
      20, 22, 21, 20, 23, 22,
    };

    std::vector<vec3> final_vertices {};
    for (INT ind = 0; ind < (UINT)indices.size(); ind++)
    {
      if (ind % 3 == 0)
        SegmentWalls[WallInd].WallMesh.Triangles.push_back(mth::cg::triangle(indices[ind], indices[ind + 1], indices[ind + 2]));
    }
    for (auto &v : PrimVertices)
      final_vertices.push_back(v.P);
    SegmentWalls[WallInd].WallMesh.Vertices = final_vertices;

    SegmentWalls[WallInd].WallPrim = Ani->PrimitiveCreate<vertex_std>(SegmentWalls[WallInd].WallMaterial,
      std::span(PrimVertices), std::span(indices));
    SegmentWalls[WallInd].BasePoints = vertices;
    SegmentWalls[WallInd].TexCoord = RightTexCoord;
  }
} /* End of 'tam::flat::CreateWallPrimBy4Points' function */

/* Create wall primitive by 4 (floor) points function.
 * ARGUMENTS:
 *   - index of segment to be updated:
 *       INT SegmentInd;
 *   - index of wall in this segment to be updated:
 *       INT WallNo;
 *   - updated vertex to be used:
 *       vec2 vertex;
 *   - is point the first or the second one:
 *       INT PointInd;
 *   - texture coordinates offset (depending on the previously created walls):
 *       FLT TexCoordOffset;
 * RETURNS: None.
 */
VOID tam::flat::CreateWallPrimByUpdatedBasePoints( INT SegmentInd, INT WallNo, vec2 vertex, INT PointInd, FLT TexCoordOffset )
{
  INT WallInd = WallNo == 1 ? Segments[SegmentInd].Wall1 : Segments[SegmentInd].Wall2;
  if (WallInd >= SegmentWallsCounter)
    return;
  SegmentWalls[WallInd].BasePoints[PointInd] = vertex;
  CreateWallPrimBy4Points(SegmentInd, WallNo, SegmentWalls[WallInd].BasePoints, TexCoordOffset);
} /* End of 'tam::flat::CreateWallPrimByUpdatedBasePoints' function */

/* Create wall primitive creating window function.
 * ARGUMENTS:
 *   - index of segment to be updated:
 *       INT SegmentInd;
 *   - index of wall to be updated (1 or 2):
 *       INT WallNo;
 *   - texture coordinates offset (depending on the previously created walls):
 *       FLT TexCoordOffset;
 *   - texture scaling coefficient:
 *       vec2 TexScale;
 * RETURNS: None.
 */
VOID tam::flat::UpdateWallPrimWindow( INT SegmentInd, INT WallNo, FLT TexCoordOffset, vec2 TexScale )
{
  std::vector<vec2> base_vertices {};
  if (WallNo != 1 && WallNo != 2)
    return;
  INT Wall;
  if (WallNo == 1)
    Wall = Segments[SegmentInd].Wall1;
  else
    Wall = Segments[SegmentInd].Wall2;

  base_vertices = SegmentWalls[Wall].BasePoints;
  vec2 P0P3 = base_vertices[3] - base_vertices[0];
  FLT
    sc1 = (base_vertices[1] - base_vertices[0]) & (P0P3.Normalizing()),
    sc2 = (base_vertices[2] - base_vertices[3]) & (P0P3.Normalizing());
  FLT ActiveZoneWidth = !P0P3;
  if (sc1 > 0)
    ActiveZoneWidth -= sc1;
  if (sc2 < 0)
    ActiveZoneWidth += sc2;
  if (ActiveZoneWidth < 0)
    return;

  FLT
    WallLength = !P0P3,
    WindowWidth = WallLength - Segments[SegmentInd].WindowRightDist - Segments[SegmentInd].WindowLeftDist,
    WindowHeight = WallsHeight - Segments[SegmentInd].WindowBottomDist - Segments[SegmentInd].WindowTopDist;

  if (Segments[SegmentInd].WindowRightDist < 0)
    Segments[SegmentInd].WindowRightDist = 0;
  if (Segments[SegmentInd].WindowLeftDist < 0)
    Segments[SegmentInd].WindowLeftDist = 0;
  if (Segments[SegmentInd].WindowBottomDist < 0)
    Segments[SegmentInd].WindowBottomDist = 0;
  if (Segments[SegmentInd].WindowTopDist < 0)
    Segments[SegmentInd].WindowTopDist = 0;

  if (Segments[SegmentInd].WindowRightDist > WallLength)
    Segments[SegmentInd].WindowRightDist = WallLength;
  if (Segments[SegmentInd].WindowLeftDist > WallLength - Segments[SegmentInd].WindowRightDist)
    Segments[SegmentInd].WindowLeftDist = WallLength - Segments[SegmentInd].WindowRightDist;
  if (Segments[SegmentInd].WindowBottomDist > WallsHeight)
    Segments[SegmentInd].WindowBottomDist = WallsHeight;
  if (Segments[SegmentInd].WindowTopDist > WallsHeight)
    Segments[SegmentInd].WindowTopDist = WallsHeight;

  WindowWidth = WindowWidth > ActiveZoneWidth ? ActiveZoneWidth : WindowWidth;
  vec3
    V00 = vec3(base_vertices[0].X, 0, base_vertices[0].Y),
    V10 = vec3(base_vertices[1].X, 0, base_vertices[1].Y),
    V20 = vec3(base_vertices[2].X, 0, base_vertices[2].Y),
    V30 = vec3(base_vertices[3].X, 0, base_vertices[3].Y),
    V01 = vec3(base_vertices[0].X, WallsHeight, base_vertices[0].Y),
    V11 = vec3(base_vertices[1].X, WallsHeight, base_vertices[1].Y),
    V21 = vec3(base_vertices[2].X, WallsHeight, base_vertices[2].Y),
    V31 = vec3(base_vertices[3].X, WallsHeight, base_vertices[3].Y);
  vec3 Dir = (V30 - V00).Normalizing(), H = vec3(0, 1, 0);
  vec3
    p10 = V00 + H * (Segments[SegmentInd].WindowBottomDist) + Dir * (Segments[SegmentInd].WindowLeftDist),
    p20 = V00 + H * (WallsHeight - Segments[SegmentInd].WindowTopDist) + Dir * (Segments[SegmentInd].WindowLeftDist),
    p30 = V00 + H * (WallsHeight - Segments[SegmentInd].WindowTopDist) + Dir * (WallLength - Segments[SegmentInd].WindowRightDist),
    p40 = V00 + H * (Segments[SegmentInd].WindowBottomDist) + Dir * (WallLength - Segments[SegmentInd].WindowRightDist);
  vec3 normal = (Dir % H).Normalizing();
  normal *= ((Dir % normal) & (Dir % (V10 - V00))) > 0 ? 1 : -1;
  vec3
    p11 = p10 + normal * WallsWidth / 2.0,
    p21 = p20 + normal * WallsWidth / 2.0,
    p31 = p30 + normal * WallsWidth / 2.0,
    p41 = p40 + normal * WallsWidth / 2.0;
  vec3 back_normal {}, outer_left_normal {}, outer_right_normal {}, inner_left_normal {};
  back_normal = ((V11 - V10) % (V20 - V10)).Normalizing();
  outer_left_normal = ((V01 - V00) % (V10 - V00)).Normalizing();
  outer_right_normal = ((V20 - V30) % (V31 - V30)).Normalizing();
  inner_left_normal = ((p11 - p10) % (p20 - p10)).Normalizing();
  FLT
    LeftWinTexCoord = (Segments[SegmentInd].WindowLeftDist) / TexScale.X + TexCoordOffset,
    RightWinTexCoord = (WallLength - Segments[SegmentInd].WindowRightDist) / TexScale.X + TexCoordOffset,
    BottomWinTexCoord = Segments[SegmentInd].WindowBottomDist / TexScale.Y,
    TopWinTexCoord = (WallsHeight - Segments[SegmentInd].WindowTopDist) / TexScale.Y;
  FLT
    LeftTexCoord = TexCoordOffset,
    RightTexCoord = (WallLength) / TexScale.X + TexCoordOffset,
    TopTexCoord = WallsHeight / TexScale.Y;

  std::vector<vertex_std> vertices =
  {
    /* Front side */
    // Outer points 1-8
    {V00, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {V01, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {V31, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {V30, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},

    // Inner points
    {p10, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {p20, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {p30, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {p40, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},

    /* Outer left side 9-12 */
    {V00, {0, 0}, outer_left_normal, {1, 0, 0, 1}},
    {V01, {0, 0}, outer_left_normal, {1, 0, 0, 1}},
    {V11, {0, 0}, outer_left_normal, {1, 0, 0, 1}},
    {V10, {0, 0}, outer_left_normal, {1, 0, 0, 1}},


    /* Back side 13-20 */
    // Outer points
    {V10, {LeftTexCoord, 0}, back_normal, {1, 0, 0, 1}},
    {V11, {LeftTexCoord, TopTexCoord}, back_normal, {1, 0, 0, 1}},
    {V21, {RightTexCoord, TopTexCoord}, back_normal, {1, 0, 0, 1}},
    {V20, {RightTexCoord, 0}, back_normal, {1, 0, 0, 1}},

    // Inner points
    {p11, {LeftWinTexCoord, BottomWinTexCoord}, back_normal, {1, 0, 0, 1}},
    {p21, {LeftWinTexCoord, TopWinTexCoord}, back_normal, {1, 0, 0, 1}},
    {p31, {RightWinTexCoord, TopWinTexCoord}, back_normal, {1, 0, 0, 1}},
    {p41, {RightWinTexCoord, BottomWinTexCoord}, back_normal, {1, 0, 0, 1}},


    /* Outer right side 21-24 */
    {V30, {0, 0}, outer_right_normal, {1, 0, 0, 1}},
    {V31, {0, 0}, outer_right_normal, {1, 0, 0, 1}},
    {V21, {0, 0}, outer_right_normal, {1, 0, 0, 1}},
    {V20, {0, 0}, outer_right_normal, {1, 0, 0, 1}},


    /* Outer top side 25-28 */
    {V01, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {V11, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {V21, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {V31, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},


    /* Outer bottom side 29-32 */
    {V00, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},
    {V10, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},
    {V20, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},
    {V30, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},


    /* Inner left side 33-36 */
    {p10, {0, 0}, inner_left_normal, {1, 0, 0, 1}},
    {p20, {0, 0}, inner_left_normal, {1, 0, 0, 1}},
    {p21, {0, 0}, inner_left_normal, {1, 0, 0, 1}},
    {p11, {0, 0}, inner_left_normal, {1, 0, 0, 1}},


    /* Inner top side 37-40 */
    {p20, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},
    {p21, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},
    {p31, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},
    {p30, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},


    /* Inner right side 41-44 */
    {p40, {0, 0}, -inner_left_normal, {1, 0, 0, 1}},
    {p30, {0, 0}, -inner_left_normal, {1, 0, 0, 1}},
    {p31, {0, 0}, -inner_left_normal, {1, 0, 0, 1}},
    {p41, {0, 0}, -inner_left_normal, {1, 0, 0, 1}},


    /* Inner bottom side 45-48 */
    {p10, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {p11, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {p41, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {p40, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
  };

  std::vector<INT> indices1 {}, indices2 {}, indices3 {}, indices4 {},
    add1t {}, add1b {}, add2t {}, add2b {}, add3l {}, add3r {}, add4l {}, add4r {};

  // Left prism
  indices1 =
  {
    /* Left side */
    8, 9, 10, 8, 11, 10,

    /* Right side */
    32, 33, 34, 32, 34, 35,

    /* Back side */
    12, 13, 17, 12, 17, 16,
  };
  add1t = {9, 10, 34, 9, 34, 33};
  add1b = {8, 11, 35, 8, 32, 35};

  // Right prism
  indices2 =
  {
    /* Right side */
    20, 21, 22, 20, 22, 23,

    /* Left side */
    40, 41, 42, 40, 42, 43,

    /* Back side */
    15, 19, 18, 15, 18, 14,
  };
  add2t = {21, 41, 42, 21, 42, 22};
  add2b = {20, 40, 43, 20, 43, 23};

  // Top prism
  indices3 =
  {
    /* Top side */
    24, 25, 26, 24, 26, 27,

    /* Bottom side */
    36, 37, 38, 36, 38, 39,

    /* Back side */
    13, 17, 18, 13, 18, 14,
  };
  add3l = {24, 25, 36, 25, 36, 37};
  add3r = {27, 39, 38, 27, 38, 26};

  // Bottom prism
  indices4 =
  {
    /* Top side */
    44, 45, 46, 44, 46, 47,

    /* Bottom side */
    28, 29, 30, 28, 30, 31,

    /* Back side */
    12, 16, 19, 12, 19, 15,
  };
  add4l = {28, 29, 44, 29, 44, 45};
  add4r = {31, 47, 46, 31, 46, 30};

  std::vector<INT> indices = {};
  if (Segments[SegmentInd].WindowLeftDist != 0)
    indices.insert(indices.begin(), indices1.begin(), indices1.end());
  else
  {
    indices.insert(indices.begin(), add3l.begin(), add3l.end());
    indices.insert(indices.end(), add4l.begin(), add4l.end());
  }
  if (Segments[SegmentInd].WindowRightDist != 0)
    indices.insert(indices.end(), indices2.begin(), indices2.end());
  else
  {
    indices.insert(indices.end(), add3r.begin(), add3r.end());
    indices.insert(indices.end(), add4r.begin(), add4r.end());
  }
  if (Segments[SegmentInd].WindowTopDist != 0)
    indices.insert(indices.end(), indices3.begin(), indices3.end());
  else
  {
    indices.insert(indices.end(), add1t.begin(), add1t.end());
    indices.insert(indices.end(), add2t.begin(), add2t.end());
  }
  if (Segments[SegmentInd].WindowBottomDist != 0)
    indices.insert(indices.end(), indices4.begin(), indices4.end());
  else
  {
    indices.insert(indices.end(), add1b.begin(), add1b.end());
    indices.insert(indices.end(), add2b.begin(), add2b.end());
  }

  std::vector<vec3> final_vertices {};
  for (INT ind = 0; ind < (UINT)indices.size(); ind++)
  {
    if (ind % 3 == 0)
      SegmentWalls[Wall].WallMesh.Triangles.push_back(mth::cg::triangle(indices[ind], indices[ind + 1], indices[ind + 2]));
  }
  for (auto &v : vertices)
    final_vertices.push_back(v.P);
  SegmentWalls[Wall].WallMesh.Vertices = final_vertices;

  SegmentWalls[Wall].WallPrim = Ani->PrimitiveCreate<vertex_std>(SegmentWalls[Wall].WallMaterial,
    std::span(vertices), std::span(indices));
  SegmentWalls[Wall].TexCoord = RightTexCoord;
  Segments[SegmentInd].AdditionalVertices =
    {
      {p10, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
      {p20, {0, 1}, {0, 1, 0}, {1, 0, 0, 1}},
      {p30, {1, 1}, {0, 1, 0}, {1, 0, 0, 1}},
      {p40, {1, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    };
} /* End of 'tam::flat::UpdateWallPrimWindow' function */

/* Create wall primitive creating door function.
 * ARGUMENTS:
 *   - index of segment to be updated:
 *       INT SegmentInd;
 *   - index of wall to be updated (1 or 2):
 *       INT WallNo;
 *   - texture coordinates offset (depending on the previously created walls):
 *       FLT TexCoordOffset;
 *   - texture scaling coefficient:
 *       vec2 TexScale;
 * RETURNS: None.
 */
VOID tam::flat::UpdateWallPrimDoor( INT SegmentInd, INT WallNo, FLT TexCoordOffset, vec2 TexScale )
{
  std::vector<vec2> base_vertices {};
  if (WallNo != 1 && WallNo != 2)
    return;
  INT Wall;
  if (WallNo == 1)
    Wall = Segments[SegmentInd].Wall1;
  else
    Wall = Segments[SegmentInd].Wall2;

  base_vertices = SegmentWalls[Wall].BasePoints;
  vec2 P0P3 = base_vertices[2] - base_vertices[1];
  FLT
    sc1 = (base_vertices[1] - base_vertices[0]) & (P0P3.Normalizing()),
    sc2 = (base_vertices[2] - base_vertices[3]) & (P0P3.Normalizing());
  FLT ActiveZoneWidth = !P0P3;
  if (sc1 > 0)
    ActiveZoneWidth -= sc1;
  if (sc2 < 0)
    ActiveZoneWidth += sc2;
  if (ActiveZoneWidth < 0)
    return;

  Segments[SegmentInd].DoorHeight = Segments[SegmentInd].DoorHeight > WallsHeight * 0.9 ? WallsHeight * 0.9 : Segments[SegmentInd].DoorHeight;
  Segments[SegmentInd].DoorWidth = Segments[SegmentInd].DoorWidth > ActiveZoneWidth ? ActiveZoneWidth : Segments[SegmentInd].DoorWidth;
  FLT DoorWidth = Segments[SegmentInd].DoorWidth, DoorHeight = Segments[SegmentInd].DoorHeight;
  vec3
    V00 = vec3(base_vertices[0].X, 0, base_vertices[0].Y),
    V10 = vec3(base_vertices[1].X, 0, base_vertices[1].Y),
    V20 = vec3(base_vertices[2].X, 0, base_vertices[2].Y),
    V30 = vec3(base_vertices[3].X, 0, base_vertices[3].Y),
    V01 = vec3(base_vertices[0].X, WallsHeight, base_vertices[0].Y),
    V11 = vec3(base_vertices[1].X, WallsHeight, base_vertices[1].Y),
    V21 = vec3(base_vertices[2].X, WallsHeight, base_vertices[2].Y),
    V31 = vec3(base_vertices[3].X, WallsHeight, base_vertices[3].Y);
  vec3 Dir = (V30 - V00).Normalizing(), H = vec3(0, 1, 0);
  FLT WallLength = !(base_vertices[3] - base_vertices[0]);
  vec3
    p10 = V00 + Dir * (WallLength - DoorWidth) / 2.0,
    p20 = V00 + H * DoorHeight + Dir * (WallLength - DoorWidth) / 2.0,
    p30 = V00 + H * DoorHeight + Dir * (WallLength + DoorWidth) / 2.0,
    p40 = V00 + Dir * (WallLength + DoorWidth) / 2.0;
  vec3 normal = (Dir % H).Normalizing();
  normal *= ((Dir % normal) & (Dir % (V10 - V00))) > 0 ? 1 : -1;
  vec3
    p11 = p10 + normal * WallsWidth / 2.0,
    p21 = p20 + normal * WallsWidth / 2.0,
    p31 = p30 + normal * WallsWidth / 2.0,
    p41 = p40 + normal * WallsWidth / 2.0;
  vec3 back_normal {}, outer_left_normal {}, outer_right_normal {}, inner_left_normal {};
  back_normal = ((V11 - V10) % (V20 - V10)).Normalizing();
  outer_left_normal = ((V01 - V00) % (V10 - V00)).Normalizing();
  outer_right_normal = ((V20 - V30) % (V31 - V30)).Normalizing();
  inner_left_normal = ((p11 - p10) % (p20 - p10)).Normalizing();
  FLT
    LeftDoorTexCoord = ((WallLength - DoorWidth) / 2.0) / TexScale.X + TexCoordOffset,
    RightDoorTexCoord = ((WallLength + DoorWidth) / 2.0) / TexScale.X + TexCoordOffset,
    TopDoorTexCoord = DoorHeight / TexScale.Y;
  FLT
    LeftTexCoord = TexCoordOffset,
    RightTexCoord = (WallLength) / TexScale.X + TexCoordOffset,
    TopTexCoord = WallsHeight / TexScale.Y;

  std::vector<vertex_std> vertices
  {
    /* Front side */
    // Outer points
    {V00, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {V01, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {V31, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {V30, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},

    // Inner points
    {p10, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {p20, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {p30, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {p40, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},


    /* Outer left side */
    {V00, {0, 0}, outer_left_normal, {1, 0, 0, 1}},
    {V01, {0, 0}, outer_left_normal, {1, 0, 0, 1}},
    {V11, {0, 0}, outer_left_normal, {1, 0, 0, 1}},
    {V10, {0, 0}, outer_left_normal, {1, 0, 0, 1}},


    /* Back side */
    // Outer points
    {V10, {LeftTexCoord, 0}, back_normal, {1, 0, 0, 1}},
    {V11, {LeftTexCoord, TopTexCoord}, back_normal, {1, 0, 0, 1}},
    {V21, {RightTexCoord, TopTexCoord}, back_normal, {1, 0, 0, 1}},
    {V20, {RightTexCoord, 0}, back_normal, {1, 0, 0, 1}},

    // Inner points
    {p11, {LeftDoorTexCoord, 0}, back_normal, {1, 0, 0, 1}},
    {p21, {LeftDoorTexCoord, TopDoorTexCoord}, back_normal, {1, 0, 0, 1}},
    {p31, {RightDoorTexCoord, TopDoorTexCoord}, back_normal, {1, 0, 0, 1}},
    {p41, {RightDoorTexCoord, 0}, back_normal, {1, 0, 0, 1}},


    /* Outer right side */
    {V30, {0, 0}, outer_right_normal, {1, 0, 0, 1}},
    {V31, {0, 0}, outer_right_normal, {1, 0, 0, 1}},
    {V21, {0, 0}, outer_right_normal, {1, 0, 0, 1}},
    {V20, {0, 0}, outer_right_normal, {1, 0, 0, 1}},


    /* Outer top side */
    {V01, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {V11, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {V21, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {V31, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},


    /* Outer bottom side 28-31 -> 28-35 */
    {V00, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},
    {V10, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},
    {p11, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},
    {p10, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},

    {p40, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},
    {p41, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},
    {V20, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},
    {V30, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},


    /* Inner left side */
    {p10, {0, 0}, inner_left_normal, {1, 0, 0, 1}},
    {p20, {0, 0}, inner_left_normal, {1, 0, 0, 1}},
    {p21, {0, 0}, inner_left_normal, {1, 0, 0, 1}},
    {p11, {0, 0}, inner_left_normal, {1, 0, 0, 1}},


    /* Inner top side */
    {p20, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},
    {p21, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},
    {p31, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},
    {p30, {0, 0}, {0, -1, 0}, {1, 0, 0, 1}},


    /* Inner right side */
    {p40, {0, 0}, -inner_left_normal, {1, 0, 0, 1}},
    {p30, {0, 0}, -inner_left_normal, {1, 0, 0, 1}},
    {p31, {0, 0}, -inner_left_normal, {1, 0, 0, 1}},
    {p41, {0, 0}, -inner_left_normal, {1, 0, 0, 1}},


    /* Inner bottom side */
    {p10, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {p11, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {p41, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {p40, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
  };

  std::vector<INT> indices
  {
    /* Outer left side */
    8, 9, 10, 8, 11, 10,

    /* Back side (front + 12) */
    // Tr 1
    12, 16, 13,
    // Tr 2
    16, 13, 17,
    // Tr 3
    17, 13, 14,
    // Tr 4
    17, 14, 18,
    // Tr 5
    15, 18, 14,
    // Tr 6
    15, 19, 18,
    // Tr 7
    12, 19, 15,
    // Tr 8
    12, 16, 19,

    /* Outer right side */
    20, 21, 22, 20, 22, 23,

    /* Outer top side */
    24, 25, 26, 24, 26, 27,

    /* Outer bottom side */
    28, 29, 30, 28, 30, 31,
    32, 33, 34, 32, 34, 35,

    /* Inner left side */
    36, 37, 38, 36, 38, 39,

    /* Inner top side */
    40, 41, 42, 40, 42, 43,

    /* Inner right side */
    44, 45, 46, 44, 46, 47,
  };

  std::vector<vec3> final_vertices {};
  for (INT ind = 0; ind < (UINT)indices.size(); ind++)
  {
    if (ind % 3 == 0)
      SegmentWalls[Wall].WallMesh.Triangles.push_back(mth::cg::triangle(indices[ind], indices[ind + 1], indices[ind + 2]));
  }
  for (auto &v : vertices)
    final_vertices.push_back(v.P);
  SegmentWalls[Wall].WallMesh.Vertices = final_vertices;

  SegmentWalls[Wall].WallPrim = Ani->PrimitiveCreate<vertex_std>(SegmentWalls[Wall].WallMaterial,
    std::span(vertices), std::span(indices));
  SegmentWalls[Wall].TexCoord = RightTexCoord;
  Segments[SegmentInd].AdditionalVertices =
    {
      {p10, {0, 0}, {0, 1, 0}, {1, 0, 0, 1}},
      {p20, {0, 1}, {0, 1, 0}, {1, 0, 0, 1}},
      {p30, {1, 1}, {0, 1, 0}, {1, 0, 0, 1}},
      {p40, {1, 0}, {0, 1, 0}, {1, 0, 0, 1}}
    };
} /* End of 'tam::flat::UpdateWallPrimDoor' function */

/* Change segment type by recreating wall primitives function.
 * ARGUMENTS:
 *   - index of segment to be updated:
 *       INT SegmentInd;
 *   - new segment type:
 *       segment_type NewType;
 * RETURNS: None.
 */
VOID tam::flat::ChangeSegmentType( INT SegmentInd, segment_type NewType )
{
  Segments[SegmentInd].Type = NewType;
  if (NewType == segment_type::eWIN)
  {
    UpdateWallPrimWindow(SegmentInd, 1);
    UpdateWallPrimWindow(SegmentInd, 2);
  }
  else if (NewType == segment_type::eDOOR)
  {
    UpdateWallPrimDoor(SegmentInd, 1);
    UpdateWallPrimDoor(SegmentInd, 2);
  }
  else
  {
    CreateWallPrimBy4Points(SegmentInd, 1, SegmentWalls[Segments[SegmentInd].Wall1].BasePoints);
    CreateWallPrimBy4Points(SegmentInd, 2, SegmentWalls[Segments[SegmentInd].Wall2].BasePoints);
  }
} /* End of 'tam::flat::ChangeSegmentType' function */

/* Add segment point function.
 * ARGUMENTS:
 *   - point coords:
 *       FLT X, Y;
 * RETURNS:
 *   (INT) point index in map of segment points of flat layout.
 */
INT tam::flat::AddPoint( FLT X, FLT Y )
{
  INT p = FindNearestPoint(X, Y, Threshold);
  if (p != -1)
    return p;
  SegmentPoints[SegmentPointsCounter] = segment_point(X, Y);
  return SegmentPointsCounter++;
} /* End of 'tam::flat::AddPoint' function */

/* Update selected segment points function.
 * ARGUMENTS:
 *   - points indeces in map of segment points of flat layout:
 *       std::vector<INT> PointInds;
 *   - new coords for point:
 *       FLT NewX, NewY;
 *   - flag for clamping to integer coordinates:
 *       BOOL IsClamping;
 * RETURNS: None.
 */
VOID tam::flat::UpdateSelectedPoints( std::vector<INT> PointInds, FLT NewX, FLT NewY, BOOL IsClamping )
{
  if (vec2(NewX, NewY).Length2() < Threshold)
    return;
  std::vector<INT> UpdatedSegments;
  for (auto &Ind : PointInds)
  {
    if (!SegmentPoints.contains(Ind))
      continue;
    SegmentPoints[Ind].Pos += vec2(NewX, NewY);
    if (IsClamping)
      SegmentPoints[Ind].Pos = vec2(std::roundf(SegmentPoints[Ind].Pos.X), std::roundf(SegmentPoints[Ind].Pos.Y));
  }
  for (auto &Ind : PointInds)
  {
    
    for (auto &Segment : SegmentPoints[Ind].SegmentIndices)
    {
      INT Point;
      BOOL IsPointContains = FALSE;
      if (Segments[Segment].Point1 == Ind)
        Point = Segments[Segment].Point2;
      else
        Point = Segments[Segment].Point1;
      auto PointIt = std::find(PointInds.begin(), PointInds.end(), Point);
      if (PointIt != PointInds.end())
        IsPointContains = TRUE;
      if (std::find(UpdatedSegments.begin(), UpdatedSegments.end(), Segment) == UpdatedSegments.end())
      {
        UpdateSegmentLayout(Segment);
        UpdatedSegments.push_back(Segment);
      }
      if (!IsPointContains)
      {
        for (auto &Connected : SegmentPoints[Point].SegmentIndices)
          if (Connected != Segment && std::find(UpdatedSegments.begin(), UpdatedSegments.end(), Connected) == UpdatedSegments.end())
          {
            UpdateSegmentLayout(Connected);
            UpdatedSegments.push_back(Connected);
          }
      }
    }
    for (auto &RoomName : SegmentPoints[Ind].RoomNames)
      UpdateRoom(RoomName); // Rooms[].RoomFloor.UpdateFloorPrimitive();
  }
} /* End of 'tam::flat::UpdateSelectedPoints' function */

/* Delete segment point function.
 * ARGUMENTS:
 *   - point index in map of segment points of flat layout:
 *       INT PointInd;
 * RETURNS: None.
 */
VOID tam::flat::DeletePoint( INT PointInd )
{
  if (!SegmentPoints.contains(PointInd))
    return;

  // Delete point from every room
  for (auto &RoomName : SegmentPoints[PointInd].RoomNames)
  {
    if (auto Point = std::find(Rooms[RoomName].RoomPoints.begin(),
                               Rooms[RoomName].RoomPoints.end(),
                               PointInd); Point != Rooms[RoomName].RoomPoints.end())
    {
      Rooms[RoomName].RoomPoints.erase(Point);
      UpdateRoom(RoomName);
    }
  }
  std::vector<INT> SegmentInds = SegmentPoints[PointInd].SegmentIndices;
  SegmentPoints.erase(PointInd);
  if (SegmentPoints.size() == 0)
    SegmentPointsCounter = 0;
  for (auto &SegmentNum : SegmentInds)
  {
    if (Segments[SegmentNum].Point1 == PointInd)
      Segments[SegmentNum].Point1 = -1;
    if (Segments[SegmentNum].Point2 == PointInd)
      Segments[SegmentNum].Point2 = -1;
  }
  for (auto &SegmentNum : SegmentInds)
    DeleteSegment(SegmentNum);
} /* End of 'tam::flat::DeletePoint' function */

/* Find nearest segment point function.
 * ARGUMENTS:
 *   - coords to find near:
 *       FLT X, Y;
 *   - threshold for finding:
 *       FLT FindThreshold;
 * RETURNS:
 *   (INT) point index in map of segment points of flat layout, -1 if no finded point.
 */
INT tam::flat::FindNearestPoint( FLT X, FLT Y, FLT FindThreshold )
{
  FLT Threshold2 = FindThreshold * FindThreshold;
  FLT NearestDist = Threshold2 + 0.1;
  INT Nearest = -1;

  for (auto &Point : SegmentPoints)
  {
    FLT CurrentDist = (Point.second.Pos - vec2(X, Y)).Length2();
    
    if (CurrentDist <= Threshold2 && CurrentDist < NearestDist)
    {
      NearestDist = CurrentDist;
      Nearest = Point.first;
    }
  }
  return Nearest;
} /* End of 'tam::flat::FindNearestPoint' function */

/* Correct segment function.
 * ARGUMENTS:
 *   - segment to correct:
 *       INT SegmentInd;
 * RETURNS: None.
 */
VOID tam::flat::CorrectSegment( INT SegmentInd )
{
  if (!Segments.contains(SegmentInd))
    return;
  std::vector<INT> NewSegments;
  INT Point1 = Segments[SegmentInd].Point1;
  INT Point2 = Segments[SegmentInd].Point2;
  INT SaveP1 = Point1;
  INT SaveP2 = Point2;
  BOOL IsCorrected = FALSE;
  for (auto &Num : SegmentPoints[Point1].SegmentIndices)
  {
    if (Num == SegmentInd || !Segments.contains(Num))
      continue;
    INT Segment = Num;
    vec2 P1 = SegmentPoints[Point1].Pos;
    vec2 P2 = SegmentPoints[Point2].Pos;
    INT Point3;
    vec2 P3;
    if (Segments[Segment].Point1 == Point1)
      Point3 = Segments[Segment].Point2;
    else
      Point3 = Segments[Segment].Point1;
    if (Point2 == Point3)
      continue;
    P3 = SegmentPoints[Point3].Pos;
    vec2 DP12 = P2 - P1;
    vec2 DP13 = P3 - P1;
    if ((DP12.X / DP13.X) > 0 && ((DP12.Y == 0 && DP13.Y == 0) || (DP12.Y != 0 && DP13.Y != 0 && abs((DP12.X / DP13.X) - (DP12.Y / DP13.Y)) < 0.01)) || 
        (DP12.Y / DP13.Y) > 0 && ((DP12.X == 0 && DP13.X == 0) || (DP12.X != 0 && DP13.X != 0 && abs((DP12.X / DP13.X) - (DP12.Y / DP13.Y)) < 0.01)))
    {
      if (DP12.Length2() > DP13.Length2())
      {
        DeleteSegment(SegmentInd, FALSE);
        NewSegments.push_back(AddSegment(Point3, Point2, FALSE));
        IsCorrected = TRUE;
        break;
      }
      else
      {
        DeleteSegment(SegmentInd, FALSE);
        DeleteSegment(Segment, FALSE);
        NewSegments.push_back(AddSegment(Point1, Point2, FALSE));
        NewSegments.push_back(AddSegment(Point2, Point3, FALSE));
        IsCorrected = TRUE;
        break;
      }
    }
  }

  if (!IsCorrected)
  {
    Point1 = Segments[SegmentInd].Point2;
    Point2 = Segments[SegmentInd].Point1;
    for (auto &Num : SegmentPoints[Point1].SegmentIndices)
    {
      if (Num == SegmentInd || !Segments.contains(Num))
        continue;
      INT Segment = Num;
      vec2 P1 = SegmentPoints[Point1].Pos;
      vec2 P2 = SegmentPoints[Point2].Pos;
      INT Point3;
      vec2 P3;
      if (Segments[Segment].Point1 == Point1)
        Point3 = Segments[Segment].Point2;
      else
        Point3 = Segments[Segment].Point1;
      if (Point2 == Point3)
        continue;
      P3 = SegmentPoints[Point3].Pos;
      vec2 DP12 = P2 - P1;
      vec2 DP13 = P3 - P1;
      if ((DP12.X / DP13.X) > 0 && ((DP12.Y == 0 && DP13.Y == 0) || (DP12.Y != 0 && DP13.Y != 0 && abs((DP12.X / DP13.X) - (DP12.Y / DP13.Y)) < 0.01)) || 
          (DP12.Y / DP13.Y) > 0 && ((DP12.X == 0 && DP13.X == 0) || (DP12.X != 0 && DP13.X != 0 && abs((DP12.X / DP13.X) - (DP12.Y / DP13.Y)) < 0.01)))
      {
        if (DP12.Length2() > DP13.Length2())
        {
          DeleteSegment(SegmentInd, FALSE);
          NewSegments.push_back(AddSegment(Point2, Point3, FALSE));
          IsCorrected = TRUE;
          break;
        }
        else
        {
          DeleteSegment(Segment, FALSE);
          NewSegments.push_back(AddSegment(Point2, Point1, FALSE));
          NewSegments.push_back(AddSegment(Point3, Point2, FALSE));
          IsCorrected = TRUE;
          break;
        }
      }
    }
  }
} /* End of 'tam::flat::CorrectSegment' function */

/* Add segment function.
 * ARGUMENTS:
 *   - segment points:
 *       INT PointInd1, PointInd2;
 * RETURNS:
 *   (INT) segment index in map of segments of flat layout.
 */
INT tam::flat::AddSegment( INT PointInd1, INT PointInd2, BOOL NeedUpdate )
{
  // check for adding segment that already was created
  for (auto &Segment : SegmentPoints[PointInd1].SegmentIndices)
    if ((Segments[Segment].Point1 == PointInd1 && Segments[Segment].Point2 == PointInd2) ||
        (Segments[Segment].Point1 == PointInd2 && Segments[Segment].Point2 == PointInd1))
      return Segment;

  if (PointInd1 == PointInd2)
    return -1;

  if (NeedUpdate)
  {
    INT MinToP1 = -1;
    INT MinToP2 = -1;
    INT PointI1 = PointInd1;
    INT PointI2 = PointInd2;
    vec2 DP12 = SegmentPoints[PointI2].Pos - SegmentPoints[PointI1].Pos;
    INT MinDist = DP12.Length2();
    
    for (auto &[PointInd, Point] : SegmentPoints)
    {
      if (PointInd == PointI1 || PointInd == PointI2)
        continue;
      BOOL IsOk = TRUE;
      for (auto &Segm : SegmentPoints[PointInd].SegmentIndices)
        if (Segments[Segm].Point1 == PointI1 || Segments[Segm].Point1 == PointI2 || Segments[Segm].Point2 == PointI1 || Segments[Segm].Point2 == PointI2)
        {
          IsOk = FALSE;
          break;
        }
      if (!IsOk)
        continue;
      vec2 DP13 = Point.Pos - SegmentPoints[PointI1].Pos;
      if ((DP12.X / DP13.X) > 0 && ((DP12.Y == 0 && DP13.Y == 0) || (DP12.Y != 0 && DP13.Y != 0 && abs((DP12.X / DP13.X) - (DP12.Y / DP13.Y)) < 0.01)) || 
          (DP12.Y / DP13.Y) > 0 && ((DP12.X == 0 && DP13.X == 0) || (DP12.X != 0 && DP13.X != 0 && abs((DP12.X / DP13.X) - (DP12.Y / DP13.Y)) < 0.01)))
      {
        if (DP13.Length2() < MinDist)
          MinDist = DP13.Length2(), MinToP1 = PointInd;
      }
    }

    if (MinToP1 >= 0)
      AddSegment(PointI1, MinToP1);

    MinDist = DP12.Length2();
    for (auto &[PointInd, Point] : SegmentPoints)
    {
      if (PointInd == PointI1 || PointInd == PointI2)
        continue;
      BOOL IsOk = TRUE;
      for (auto &Segm : SegmentPoints[PointInd].SegmentIndices)
        if (Segments[Segm].Point1 == PointI1 || Segments[Segm].Point1 == PointI2 || Segments[Segm].Point2 == PointI1 || Segments[Segm].Point2 == PointI2)
        {
          IsOk = FALSE;
          break;
        }
      if (!IsOk)
        continue;
      vec2 DP23 = SegmentPoints[PointI2].Pos - Point.Pos;
      if ((DP12.X / DP23.X) > 0 && ((DP12.Y == 0 && DP23.Y == 0) || (DP12.Y != 0 && DP23.Y != 0 && abs((DP12.X / DP23.X) - (DP12.Y / DP23.Y)) < 0.01)) || 
          (DP12.Y / DP23.Y) > 0 && ((DP12.X == 0 && DP23.X == 0) || (DP12.X != 0 && DP23.X != 0 && abs((DP12.X / DP23.X) - (DP12.Y / DP23.Y)) < 0.01)))
      {
        if (DP23.Length2() < MinDist)
          MinDist = DP23.Length2(), MinToP2 = PointInd;
      }
    }
    if (MinToP2 >= 0)
      AddSegment(MinToP2, PointI2);

    if (MinToP1 >= 0 && MinToP2 >= 0 && MinToP1 != MinToP2)
    {
      INT Last = AddSegment(MinToP1, MinToP2);
      return Last;
    }
  }

  INT SaveNumber = -1;
  for (auto &S : Segments)
    if (SaveNumber < S.first)
      SaveNumber = S.first;
  SaveNumber++;

  Segments[SaveNumber] = segment(PointInd1, PointInd2);
  SegmentPoints[PointInd1].SegmentIndices.push_back(SaveNumber);
  SegmentPoints[PointInd2].SegmentIndices.push_back(SaveNumber);

  UpdateSegmentLayout(SaveNumber);
  CorrectSegment(SaveNumber);

  for (auto &S : Segments)
    if (SaveNumber < S.first)
      SaveNumber = S.first;
  SaveNumber++;
  SegmentsCounter = SaveNumber;
  return SaveNumber - 1;
} /* End of 'tam::flat::AddSegment' function */

/* Update selected segments function.
 * ARGUMENTS:
 *   - points indeces in map of segment of flat layout:
 *       std::vector<INT> SegmentInds;
 *   - new coords for point:
 *       FLT NewX, NewY;
 *   - flag for clamping to integer coordinates:
 *       BOOL IsClamping;
 * RETURNS: None.
 */
VOID tam::flat::UpdateSelectedSegments( std::vector<INT> SegmentInds, FLT NewX, FLT NewY, BOOL IsClamping )
{
  if (!vec2(NewX, NewY) <= Threshold)
    return;
  // Just update segments
  if (SegmentInds.size() == 0 || (NewX == INFINITY && NewY == INFINITY))
  {
    for (auto &Segment : Segments)
      UpdateSegmentLayout(Segment.first);
  }
  // Update segments positions
  std::vector<INT> Points;
  for (auto &Segment : SegmentInds)
  {
    if (!Segments.contains(Segment))
      continue;
    if (std::find(Points.begin(), Points.end(), Segments[Segment].Point1) == Points.end())
      Points.push_back(Segments[Segment].Point1);
    if (std::find(Points.begin(), Points.end(), Segments[Segment].Point2) == Points.end())
      Points.push_back(Segments[Segment].Point2);
  }
  UpdateSelectedPoints(Points, NewX, NewY, IsClamping);
} /* End of 'tam::flat::UpdateSelectedSegments' function */

/* Delete segment function.
 * ARGUMENTS:
 *   - segment index in map of segments of flat layout:
 *       INT SegmentInd;
 * RETURNS: None.
 */
VOID tam::flat::DeleteSegment( INT SegmentInd, BOOL NeedUpdate )
{
  if (!Segments.contains(SegmentInd))
    return;
  INT P1 = Segments[SegmentInd].Point1;
  INT P2 = Segments[SegmentInd].Point2;
  INT W1 = Segments[SegmentInd].Wall1;
  INT W2 = Segments[SegmentInd].Wall2;

  // Delete segment information from it's points
  for (auto &P : {P1, P2})
  {
    if (!SegmentPoints.contains(P))
      continue;
    auto SegmentIt = std::find(SegmentPoints[P].SegmentIndices.begin(), SegmentPoints[P].SegmentIndices.end(), SegmentInd);
    if (SegmentIt != SegmentPoints[P].SegmentIndices.end())
      SegmentPoints[P].SegmentIndices.erase(SegmentIt);
  }

  Segments.erase(SegmentInd);
  if (Segments.size() == 0)
    SegmentsCounter = 0;
  if (SegmentInd == SegmentsCounter - 1)
    SegmentsCounter--;
  if (NeedUpdate)
  {
    if (P1 != -1 && SegmentPoints.contains(P1))
      for (auto &SegmentNum : SegmentPoints[P1].SegmentIndices)
        if (SegmentNum != SegmentInd && Segments.contains(SegmentNum))
          UpdateSegmentLayout(SegmentNum);
    if (P2 != -1 && SegmentPoints.contains(P2))
      for (auto &SegmentNum : SegmentPoints[P2].SegmentIndices)
        if (SegmentNum != SegmentInd && Segments.contains(SegmentNum))
          UpdateSegmentLayout(SegmentNum);
  }
  Ani->PrimitiveDelete(SegmentWalls[W1].WallPrim);
  Ani->PrimitiveDelete(SegmentWalls[W2].WallPrim);
  SegmentWalls.erase(W1);
  SegmentWalls.erase(W2);
} /* End of 'tam::flat::DeleteSegment' function */

/* Find nearest segment function.
 * ARGUMENTS:
 *   - coords to find near:
 *       FLT X, Y;
 *   - threshold for finding:
 *       FLT FindThreshold;
 * RETURNS:
 *   (INT) segment index in map of segments of flat layout, -1 if no finded point.
 */
INT tam::flat::FindNearestSegment( FLT X, FLT Y, FLT FindThreshold )
{
  FLT NearestDist = FindThreshold + 0.1;
  INT Nearest = -1;

  for (auto &Segment : Segments)
  {
    FLT CurrentDist = vec2::SegmentDistance(vec2(X, Y), SegmentPoints[Segment.second.Point1].Pos, SegmentPoints[Segment.second.Point2].Pos);

    if (CurrentDist <= FindThreshold && CurrentDist < NearestDist)
    {
      NearestDist = CurrentDist;
      Nearest = Segment.first;
    }
  }
  return Nearest;
} /* End of 'tam::flat::FindNearestSegment' function */

/*** Floor handle module ***/

/* Create floor function.
 * ARGUMENTS:
 *   - flat pointer:
 *       flat *BaseFlat;
 *   - room name:
 *       const std::string &RoomName;
 *   - Y coordinate (floor - 0):
 *       FLT YCoord;
 * RETURNS:
 *   (BOOL) TRUE if floor was successfully created, FALSE otherwise.
 */
BOOL tam::floor::Create( flat *BaseFlat, const std::string &RoomName, FLT YCoord )
{
  Flat = BaseFlat;
  this->RoomName = RoomName;

  vec3 Color = YCoord > 0 ? vec3(1) : vec3::Rnd0();
  // Create floor material
  BUF_MTL_PBR BufMtlPBR
  {
    .BaseColor = vec4(Color, 1),
    .MetallicRoughnessScale = vec3(0.18, 0.8, FloorScale),
    //.TextureFlags = {0, 0, 0, 0, 0, 0, 0, 0}
  };
  FloorMtl = Flat->Ani->MaterialCreate(Flat->Rooms[RoomName].RoomName + "_floor material_" + std::to_string(YCoord),
                                      Flat->DefaultFlatMtlPat, {Flat->Wallpapers["Default wallpaper"]},
                                      {Flat->Ani->BufferCreate<BUF_MTL_PBR>(&BufMtlPBR, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)});

  // Create floor primitive
  return UpdateFloorPrimitive(YCoord);
} /* End of 'tam::floor::Create' function */

/* Delete floor function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::floor::Delete( VOID )
{
  Flat = nullptr;
  RoomName.clear();
  FloorMtl = nullptr;
  FloorPrim = nullptr;
} /* End of 'tam::floor::Delete' function */

/* Update floor primitive function.
 * ARGUMENTS:
 *   - Y coordinate of the primitive (floor - 0):
 *       FLT YCoord;
 * RETURNS:
 *   (BOOL) TRUE if floor was successfully triangulated, FALSE otherwise.
 */
BOOL tam::floor::UpdateFloorPrimitive( FLT YCoord )
{
  std::vector<vertex_std> FlV {};
  std::vector<vec2> pnts {};

  for (auto &P : Flat->Rooms[RoomName].RoomPoints)
  {
    if (!Flat->SegmentPoints.contains(P))
      continue;
    vertex_std V;
    V.C = vec4(0.6, 0.30, 0.18, 1);
    V.N = vec3(0, 1, 0);
    V.P = vec3(Flat->SegmentPoints[P].Pos.X, YCoord, Flat->SegmentPoints[P].Pos.Y);
    V.T = vec2(V.P.X, V.P.Z);
    FlV.push_back(V);
    pnts.push_back({Flat->SegmentPoints[P].Pos.X, Flat->SegmentPoints[P].Pos.Y});
  }
  auto Ind = mth::Triangulate(pnts);
  if (Ind.size() < Flat->Rooms[RoomName].RoomPoints.size() || Ind.size() == 0)
  {
    FloorPrim = nullptr;
    return FALSE;
  }
  INT BiggestTriangleInd = 0;
  FLT BiggestSquareArea = 0, TotalSquare = 0;
  // Get index of the biggest square area of floor triangles
  for (INT i = 0; i < Ind.size(); i += 3)
  {
    FLT 
      A = !(pnts[Ind[i]] - pnts[Ind[i + 1]]),
      B = !(pnts[Ind[i]] - pnts[Ind[i + 2]]),
      C = !(pnts[Ind[i + 1]] - pnts[Ind[i + 2]]);
    FLT P = (A + B + C) / 2;
    FLT Square = P * (P - A) * (P - B) * (P - C);
    TotalSquare += sqrt(Square);
    if (Square > BiggestSquareArea)
      BiggestSquareArea = Square, BiggestTriangleInd = i;
  }
  Flat->Rooms[RoomName].CenterPoint = (pnts[Ind[BiggestTriangleInd]] + pnts[Ind[BiggestTriangleInd + 1]] + pnts[Ind[BiggestTriangleInd + 2]]) / 3;
  Flat->Rooms[RoomName].Area = TotalSquare;
  FloorPrim = Flat->Ani->PrimitiveCreate<vertex_std>(FloorMtl, std::span(FlV), std::span(Ind));
  return TRUE;
} /* End of 'tam::floor::UpdateFloorPrimitive' function */

/*** Rooms handle module ***/

/* Add room by control points function.
 * ARGUMENTS:
 *   - name of room:
 *       const std::string &Name;
 *   - array of room's border points:
 *       const std::vector<INT> &RoomPoints;
 * RETURNS:
 *   (BOOL) TRUE if room was successfully added, FALSE otherwise.
 */
BOOL tam::flat::AddRoom( const std::string &Name, const std::vector<INT> &RoomPoints )
{
  if (RoomPoints.size() < 3 || Name == "")
    return FALSE;
  Rooms[Name].RoomPoints = RoomPoints;
  Rooms[Name].RoomName = Name;
  BUF_MTL_PBR BufMtlPBR
  {
    .BaseColor = vec4(0.6, 0.6, 0.55, 1),
    .MetallicRoughnessScale = vec3(0.18, 0.8, 1.0),
    .TextureFlags = {0, 0, 0, 0, 0, 0, 0, 0}
  };
  Rooms[Name].WallsMaterial = Ani->MaterialCreate("Flat material::" + Name,
                              DefaultFlatMtlPat, {Wallpapers["Default wallpaper"]},
                              {Ani->BufferCreate<BUF_MTL_PBR>(&BufMtlPBR, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)});
  Rooms[Name].WallsColor = vec3(0.6, 0.6, 0.55);
  Rooms[Name].TextureNames = {"Default wallpaper"};
  std::vector<vec2> Points {};
  for (auto &PointInd : RoomPoints)
    Points.push_back(SegmentPoints[PointInd].Pos);
  if (IsCCW(Points))
    for (INT i = 0; i < (UINT)Rooms[Name].RoomPoints.size() / 2; i++)
      std::swap(Rooms[Name].RoomPoints[i], Rooms[Name].RoomPoints[(UINT)Points.size() - 1 - i]);

  if (!Rooms[Name].RoomFloor.Create(this, Name))
  {
    if (Rooms[Name].WallsMaterial != nullptr)
      Ani->MaterialDelete(Rooms[Name].WallsMaterial);
    Rooms.erase(Name);
  }
  else
  {
    for (auto &Point : RoomPoints)
      SegmentPoints[Point].RoomNames.push_back(Name);
    Rooms[Name].RoomCeiling.Create(this, Name, WallsHeight);
    FindWallsIndices(Name);
    //UpdateRoomWalls(Name, Rooms[Name].WallpaperTransformed);
    FLT PrevTexCoord = 0.0;
    vec2 WH = Rooms[Name].WallpaperTransformed;
    INT i = 0;
    for (auto &[SegmentInd, WallNo]: Rooms[Name].RoomSegmentWalls)
    {
      INT WallInd = WallNo == 1 ? Segments[SegmentInd].Wall1 : Segments[SegmentInd].Wall2;
      CreateWallPrimBy4Points(SegmentInd, WallNo, SegmentWalls[WallInd].BasePoints, PrevTexCoord, WH);
      PrevTexCoord = SegmentWalls[WallInd].TexCoord / WH.X;
    }
  }
  IsRoomsUpdate = TRUE;
  return TRUE;
} /* End of 'tam::flat::AddRoom' function */

/* Update room function.
 * ARGUMENTS:
 *   - name of room:
 *       const std::string &Name;
 * RETURNS:
 *   (BOOL) TRUE if room was updated successfully, FALSE otherwise.
 */
BOOL tam::flat::UpdateRoom( const std::string &Name )
{
  if (!Rooms.contains(Name))
    return FALSE;
  if (!Rooms[Name].RoomFloor.UpdateFloorPrimitive())
  {
    DeleteRoom(Name);
    return FALSE;
  }
  Rooms[Name].RoomCeiling.UpdateFloorPrimitive();
  return TRUE;
} /* End of 'tam::flat::UpdateRoom' function */

/* Update room walls (because of texture coordinates) function.
 * ARGUMENTS:
 *   - name of room:
 *       const std::string &RoomName;
 *   - texture coordinates scaling coefficients:
 *       vec2 NewRoomTexCoordTransformed;
 * RETURNS: None.
 */
VOID tam::flat::UpdateRoomWalls( const std::string &RoomName, vec2 NewRoomTexCoordTransformed )
{
  FLT PrevTexCoord = 0.0;
  vec2 WH = Rooms[RoomName].WallpaperTransformed;
  INT i = 0;
  for (auto &[SegmentInd, WallNo]: Rooms[RoomName].RoomSegmentWalls)
  {
    INT WallInd = WallNo == 1 ? Segments[SegmentInd].Wall1 : Segments[SegmentInd].Wall2;
    CreateWallPrimBy4Points(SegmentInd, WallNo, SegmentWalls[WallInd].BasePoints, PrevTexCoord, WH);
    PrevTexCoord = SegmentWalls[WallInd].TexCoord / WH.X;
  }
} /* End of 'tam::flat::UpdateRoomWalls' function */

/* Update room material buffer function.
 * ARGUMENTS:
 *   - name of room:
 *       const std::string &RoomName;
 *   - new walls color:
 *       vec3 NewColor;
 *   - wallpaper scale coefficient:
 *       FLT Scale;
 * RETURNS:
 *   (BOOL) TRUE if room was updated successfully, FALSE otherwise.
 */
BOOL tam::flat::UpdateRoomMtlBuf( const std::string &RoomName, vec3 NewColor, FLT Scale )
{
  if (!Rooms.contains(RoomName))
    return FALSE;
  Rooms[RoomName].WallsColor = NewColor;
  Rooms[RoomName].WallpaperScale = Scale;
  BUF_MTL_PBR BufMtlPBR
  {
    .BaseColor = vec4(NewColor, 1),
    .MetallicRoughnessScale = vec3(0.18, 0.8, Scale),
  };
  INT i;
  std::vector<texture> RoomTexs {};
  for (i = 0; i < Rooms[RoomName].TextureNames.size(); i++)
  {
    if (Rooms[RoomName].TextureNames[i] != "" && Rooms[RoomName].TextureNames[i] != "Default wallpaper")
    {
      RoomTexs.push_back(Wallpapers[Rooms[RoomName].TextureNames[i]]);
      BufMtlPBR.TextureFlags[i] = 1;
    }
    else
      break;
  }
  for (; i < 8; i++)
    BufMtlPBR.TextureFlags[i] = 0;
  Rooms[RoomName].WallsMaterial->Buffers[0]->Update(&BufMtlPBR);
  for (auto &[Segment, WallNo] : Rooms[RoomName].RoomSegmentWalls)
  {
    INT Wall = WallNo == 1 ? Segments[Segment].Wall1 : Segments[Segment].Wall2;
    SegmentWalls[Wall].WallMaterial = Rooms[RoomName].WallsMaterial;
    SegmentWalls[Wall].WallPrim->Mtl = SegmentWalls[Wall].WallMaterial;
  }
  return TRUE;
} /* End of 'tam::flat::UpdateRoomMtl' function */

/* Update room material buffer function.
 * ARGUMENTS:
 *   - name of room:
 *       const std::string &RoomName;
 *   - walls textures:
 *       const std::vector<std::string> &TexNames;
 * RETURNS:
 *   (BOOL) TRUE if room was updated successfully, FALSE otherwise.
 */
BOOL tam::flat::UpdateRoomTexs( const std::string &RoomName, const std::vector<std::string> &TexNames )
{
  if (!Rooms.contains(RoomName))
    return FALSE;
  FindWallsIndices(RoomName);
  UINT PrevSize = (UINT)Rooms[RoomName].TextureNames.size();
  Rooms[RoomName].TextureNames = TexNames;
  if (/* PrevSize == 0 && */TexNames.size() == 0)
  {
    Rooms[RoomName].TextureNames = {"Default wallpaper"};
    UpdateRoomMtlBuf(RoomName, Rooms[RoomName].WallsColor, Rooms[RoomName].WallpaperScale);
    return TRUE;
  }
  if ((UINT)TexNames.size() > PrevSize)
    UpdateRoomMtlBuf(RoomName, Rooms[RoomName].WallsColor, Rooms[RoomName].WallpaperScale);
  if ((UINT)TexNames.size() >= PrevSize)
    Rooms[RoomName].TextureNames = TexNames;

  std::vector<texture> RoomTexs {};
  for (auto &TexName : TexNames)
    if (TexName != "")
      RoomTexs.push_back(Wallpapers[TexName]);

  Rooms[RoomName].WallsMaterial->Update(RoomTexs, Rooms[RoomName].WallsMaterial->Buffers);

  for (auto &[Segment, WallNo] : Rooms[RoomName].RoomSegmentWalls)
  {
    INT Wall = WallNo == 1 ? Segments[Segment].Wall1 : Segments[Segment].Wall2;
    SegmentWalls[Wall].WallMaterial = Rooms[RoomName].WallsMaterial;
    SegmentWalls[Wall].WallPrim->Mtl = SegmentWalls[Wall].WallMaterial;
  }
  return TRUE;
} /* End of 'tam::flat::UpdateRoomTexs' function */

/* Find room walls indices function.
 * ARGUMENTS:
 *   - name of room:
 *       const std::string &Name;
 * RETURNS: None.
 */
VOID tam::flat::FindWallsIndices( const std::string &Name )
{
  std::vector<std::pair<INT, BOOL>> RoomSegments = {};
  for (INT i = 0; i < (UINT)Rooms[Name].RoomPoints.size(); i++)
  {
    INT P1 = Rooms[Name].RoomPoints[i], P2;
    if (i == (UINT)Rooms[Name].RoomPoints.size() - 1)
      P2 = Rooms[Name].RoomPoints[0];
    else
      P2 = Rooms[Name].RoomPoints[i + 1];
    std::vector<INT> list1 = SegmentPoints[P1].SegmentIndices, list2 = SegmentPoints[P2].SegmentIndices;
    for (auto &segment1 : list1)
      for (auto &segment2 : list2)
      {
        if (segment1 == segment2)
          RoomSegments.push_back(std::pair(segment1, P1 == Segments[segment1].Point1 ? 0 : 1));
      }
  }
  Rooms[Name].RoomSegmentWalls = {};
  for (auto &[SegmentInd, IsReversed] : RoomSegments)
  {
    if (IsReversed)
    {
      SegmentWalls[Segments[SegmentInd].Wall1].WallMaterial= Rooms[Name].WallsMaterial;
      Rooms[Name].RoomSegmentWalls.push_back(std::pair(SegmentInd, 1));
    }
    else
    {
      SegmentWalls[Segments[SegmentInd].Wall2].WallMaterial = Rooms[Name].WallsMaterial;
      Rooms[Name].RoomSegmentWalls.push_back(std::pair(SegmentInd, 2));
    }
  }
} /* End of 'tam::flat::FindWallsIndices' function */

/* Delete room function.
 * ARGUMENTS:
 *   - name of room:
 *       const std::string &Name;
 * RETURNS: None.
 */
VOID tam::flat::DeleteRoom( const std::string &Name )
{
  if (Rooms.find(Name) != Rooms.end())
  {
    // Walls of a room are shared with segment primitives, so detach room
    // material from wall primitives before deleting room material.
    for (auto &[Segment, WallNo] : Rooms[Name].RoomSegmentWalls)
      if (Segments.contains(Segment))
      {
        INT Wall = WallNo == 1 ? Segments[Segment].Wall1 : Segments[Segment].Wall2;
        if (SegmentWalls.contains(Wall))
        {
          SegmentWalls[Wall].WallMaterial = DefaultFlatMtl;
          if (SegmentWalls[Wall].WallPrim != nullptr)
            SegmentWalls[Wall].WallPrim->Mtl = DefaultFlatMtl;
        }
      }

    if (Rooms[Name].RoomFloor.FloorPrim != nullptr)
      Ani->PrimitiveDelete(Rooms[Name].RoomFloor.FloorPrim);
    if (Rooms[Name].RoomCeiling.FloorPrim != nullptr)
      Ani->PrimitiveDelete(Rooms[Name].RoomCeiling.FloorPrim);
    if (Rooms[Name].RoomFloor.FloorMtl != nullptr)
      Ani->MaterialDelete(Rooms[Name].RoomFloor.FloorMtl);
    if (Rooms[Name].RoomCeiling.FloorMtl != nullptr)
      Ani->MaterialDelete(Rooms[Name].RoomCeiling.FloorMtl);
    if (Rooms[Name].WallsMaterial != nullptr)
      Ani->MaterialDelete(Rooms[Name].WallsMaterial);

    Rooms[Name].RoomFloor.Delete();
    Rooms[Name].RoomCeiling.Delete();
    Rooms[Name].TextureNames.clear();
    for (auto &Point : Rooms[Name].RoomPoints)
      if (SegmentPoints.contains(Point))
      {
        if (auto Room = std::find(SegmentPoints[Point].RoomNames.begin(),
                      SegmentPoints[Point].RoomNames.end(),
                      Name); Room != SegmentPoints[Point].RoomNames.end())
          SegmentPoints[Point].RoomNames.erase(Room);
      }
    Rooms.erase(Name);
    IsRoomsUpdate = TRUE;
  }
} /* End of 'tam::flat::DeleteRoom' function */

/* Get room by point on plane function.
 * ARGUMENTS:
 *   - point position on plane:
 *       const vec2 &SearchPoint;
 * RETURNS:
 *   (std::string) if room was found - it's name, "" otherwise.
 */
std::string tam::flat::GetRoomByPoint( const vec2 &SearchPoint )
{
  for (auto &[Name, Room] : Rooms)
  {
    std::vector<vec2> Polygon {};

    if (GetRoomPolygon(Name, Polygon) && vec2::Zero().IsPointInPolygon(Polygon, SearchPoint))
      return Name;
  }
  return "";
} /* End of 'tam::flat::GetRoomByPoint' function */

/* Add furniture to room by id function.
 * ARGUMENTS:
 *   - room name:
 *       const std::string &RoomName;
 *   - furniture reference:
 *       const furniture &Furniture;
 *   - furniture translation vector:
 *       vec3 Translation;
 *   - furniture OY rotation:
 *       FLT Rotation;
 */
VOID tam::flat::AddFurnitureToRoom( const std::string &RoomName, const furniture &Furniture,
                                    vec3 Translation, FLT Rotation )
{
  if (Rooms.contains(RoomName))
  {
    // Find existing furniture instance in collection
    if (FlatFurnitureCollection.contains(Furniture.Name))
      Rooms[RoomName].Furniture[Furniture.Name].push_back({Furniture.Name, Translation, Rotation, 0});
    // Load model to collection
    else
    {
      FlatFurnitureCollection[Furniture.Name] = {Furniture, Ani->ModelCreate(Furniture.FileName, Furniture.GetTransform())};
      Rooms[RoomName].Furniture[Furniture.Name].push_back({Furniture.Name, Translation, Rotation, 0});
    }
  }
} /* End of 'tam::flat::AddFurnitureToRoom' function */

/* Delete furniture from room function.
 * ARGUMENTS:
 *   - room name:
 *       const std::string &RoomName;
 *   - furniture id:
 *       const std::string &FurnitureId;
 *   - index of furniture instance to delete:
 *       INT FurnitureNo;
 * RETURNS: None.
 */
VOID tam::flat::DeleteFurnitureFromRoom( const std::string &RoomName, const std::string &FurnitureId, INT FurnitureNo )
{
  if (Rooms.contains(RoomName) && Rooms[RoomName].Furniture.contains(FurnitureId))
    if (FurnitureNo >= 0 && FurnitureNo < Rooms[RoomName].Furniture[FurnitureId].size())
    {
      Rooms[RoomName].Furniture[FurnitureId].erase(Rooms[RoomName].Furniture[FurnitureId].begin() + FurnitureNo);
      // Delete empty furniture list
      if (Rooms[RoomName].Furniture[FurnitureId].size() == 0)
        Rooms[RoomName].Furniture.erase(FurnitureId);
    }
} /* End of 'tam::flat::DeleteFurnitureFromRoomToRoom' function */

/* Update furniture in room by name function.
 * ARGUMENTS:
 *   - room name:
 *       const std::string &RoomName;
 *   - furniture name:
 *       const std::string &FurnitureName;
 *   - furniture number in list of same furniture instances:
 *       INT FurnitureNo;
 *   - new furniture translation vector:
 *       vec3 NewTranslation;
 *   - enw furniture OY rotation:
 *       FLT NewRotation;
 */
VOID tam::flat::UpdateRoomFurniture( const std::string &RoomName, const std::string &FurnitureName,
                                     INT FurnitureNo, vec3 NewTranslation, FLT NewRotation )
{
  if (Rooms.contains(RoomName) && Rooms[RoomName].Furniture.contains(FurnitureName))
    if (FurnitureNo >= 0 && FurnitureNo < Rooms[RoomName].Furniture[FurnitureName].size())
    {
      Rooms[RoomName].Furniture[FurnitureName][FurnitureNo].Name = FurnitureName;
      Rooms[RoomName].Furniture[FurnitureName][FurnitureNo].Pos = NewTranslation;
      Rooms[RoomName].Furniture[FurnitureName][FurnitureNo].RotationY = NewRotation;
    }
} /* End of 'tam::flat::UpdateRoomFurniture' function */

/* Auto-arrange room furniture function.
 * ARGUMENTS:
 *   - room name:
 *       const std::string &RoomName;
 *   - optional room type override:
 *       const std::string &RoomType;
 *   - optional output debug log file:
 *       const std::string &OutputFileName;
 * RETURNS:
 *   (BOOL) arrangement result.
 */
BOOL tam::flat::AutoArrangeRoom( const std::string &RoomName, const std::string &RoomType, const std::string &OutputFileName )
{
  return SolveFlatRoom(*this, RoomName, RoomType, OutputFileName);
} /* End of 'tam::flat::AutoArrangeRoom' function */

/* Draw room filling (furniture, floor) function.
 * ARGUMENTS:
 *   - room name:
 *       const std::string &RoomName;
 * RETURNS: None.
 */
VOID tam::flat::DrawRoomFilling( const std::string &RoomName )
{
  if (Rooms.contains(RoomName))
  {
    if (DrawFloor)
      Ani->Draw(Rooms[RoomName].RoomFloor.FloorPrim);
    if (DrawCeiling)
      Ani->Draw(Rooms[RoomName].RoomCeiling.FloorPrim);
    if (DrawFurniture)
      for (auto &[Key, Fur] : Rooms[RoomName].Furniture)
      {
        auto Mdl = FlatFurnitureCollection[Key].second;
        for (auto &Entry : Fur)
        {
          matr ResMatr = matr::RotateY(Entry.RotationY) * matr::Translate(Entry.Pos);
          Entry.Id = Ani->Draw(Mdl, ResMatr);
          // Debug draw mesh part
          /// for (INT i = 0; i < Mdl->Geometry.Triangles.size(); i++)
          /// {
          ///   vec3 
          ///     P1 = ResMatr.TransformPoint(Mdl->Geometry.Vertices[Mdl->Geometry.Triangles[i].V0]),
          ///     P2 = ResMatr.TransformPoint(Mdl->Geometry.Vertices[Mdl->Geometry.Triangles[i].V1]),
          ///     P3 = ResMatr.TransformPoint(Mdl->Geometry.Vertices[Mdl->Geometry.Triangles[i].V2]);
          /// 
          ///   Ani->DrawLine(P1, P2, vec4(1));
          ///   Ani->DrawLine(P1, P3, vec4(1));
          ///   Ani->DrawLine(P3, P2, vec4(1));
          /// }
        }
      }
  }
} /* End of 'tam::flat::DrawRoomFilling' function */

/* Draw flat function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::flat::DrawFlat( VOID )
{
  if (DrawWalls)
    for (auto &[Key, Wall] : SegmentWalls)
      Ani->Draw(Wall.WallPrim);
  for (auto &[Key, Room] : Rooms)
    DrawRoomFilling(Key);
  if (DrawingScheme)
    Ani->Draw(SchemePrim, matr::RotateY(SchemeRotate) * matr::Scale(vec3(SchemeScale)) * matr::Translate(SchemeTranslate));
} /* End of 'tam::flat::DrawFlat' function */

/* Draw flat using markers for debug function.
 * ARGUMENTS:
 *   - transform matrix:
 *       const matr &Transform;
 * RETURNS: None.
 */
VOID tam::flat::DrawFlatMarkers( const matr &Transform )
{
  for (auto &[ind, segment] : Segments)
  {
    segment_wall
      Wall1 = SegmentWalls[segment.Wall1],
      Wall2 = SegmentWalls[segment.Wall2];
    Ani->DrawCylinder(vec3(Wall1.BasePoints[0].X, 0, Wall1.BasePoints[0].Y), 0.1,
      vec3(Wall1.BasePoints[1].X, 0, Wall1.BasePoints[1].Y), 0.1, vec4(0, 1, 0, 1));
    Ani->DrawCylinder(vec3(Wall1.BasePoints[1].X, 0, Wall1.BasePoints[1].Y), 0.1,
      vec3(Wall1.BasePoints[2].X, 0, Wall1.BasePoints[2].Y), 0.1, vec4(0, 1, 0, 1));
    Ani->DrawCylinder(vec3(Wall1.BasePoints[3].X, 0, Wall1.BasePoints[3].Y), 0.1,
      vec3(Wall1.BasePoints[2].X, 0, Wall1.BasePoints[2].Y), 0.1, vec4(0, 1, 0, 1));

    Ani->DrawCylinder(vec3(Wall1.BasePoints[3].X, 0, Wall1.BasePoints[3].Y), 0.1,
      vec3(Wall1.BasePoints[0].X, 0, Wall1.BasePoints[0].Y), 0.1, vec4(0, 1, 1, 1));

    Ani->DrawCylinder(vec3(Wall2.BasePoints[0].X, 0, Wall2.BasePoints[0].Y), 0.1,
      vec3(Wall2.BasePoints[1].X, 0, Wall2.BasePoints[1].Y), 0.1, vec4(1, 0, 0, 1));
    Ani->DrawCylinder(vec3(Wall2.BasePoints[1].X, 0, Wall2.BasePoints[1].Y), 0.1,
      vec3(Wall2.BasePoints[2].X, 0, Wall2.BasePoints[2].Y), 0.1, vec4(1, 0, 0, 1));
    Ani->DrawCylinder(vec3(Wall2.BasePoints[3].X, 0, Wall2.BasePoints[3].Y), 0.1,
      vec3(Wall2.BasePoints[2].X, 0, Wall2.BasePoints[2].Y), 0.1, vec4(1, 0, 0, 1));
  }
} /* End of 'tam::flat::DrawFlatMarkers' function */

/* Color all connected segments depending on the angle function.
 * ARGUMENTS:
 *   - index of the segment to apply function to:
 *       INT SegmentInd;
 * RETURNS: None.
 */
VOID tam::flat::ColorSegmentAngles( INT SegmentInd )
{
  if (!Segments.contains(SegmentInd))
    return;
  INT cnt1 = 0, cnt2 = 0, maxcnt1 = (UINT)Segments[SegmentInd].angles1.size(), maxcnt2 = (UINT)Segments[SegmentInd].angles2.size();
  for (auto &[Ind, Angle] : Segments[SegmentInd].angles1)
  {
    if (Segments.contains(Ind))
    {
      Ani->DrawCylinder(vec3(SegmentPoints[Segments[Ind].Point1].Pos.X, 0, SegmentPoints[Segments[Ind].Point1].Pos.Y), 0.3,
        vec3(SegmentPoints[Segments[Ind].Point2].Pos.X, 0, SegmentPoints[Segments[Ind].Point2].Pos.Y), 0.3,
        vec4((DBL)cnt1 / maxcnt1, (DBL)cnt1 / maxcnt1, (DBL)cnt1 / maxcnt1, 1));
      cnt1++;
    }
  }
  for (auto &[Ind, Angle] : Segments[SegmentInd].angles2)
  {
    if (Segments.contains(Ind))
    {
      Ani->DrawCylinder(vec3(SegmentPoints[Segments[Ind].Point1].Pos.X, 0, SegmentPoints[Segments[Ind].Point1].Pos.Y), 0.3,
        vec3(SegmentPoints[Segments[Ind].Point2].Pos.X, 0, SegmentPoints[Segments[Ind].Point2].Pos.Y), 0.3,
        vec4((DBL)cnt2 / maxcnt2, (DBL)cnt2 / maxcnt2, (DBL)cnt2 / maxcnt2, 1));
      cnt2++;
    }
  }
} /* End of 'tam::flat::ColorSegmentAngles' function */

/* END OF 'tam_flat.cpp' FILE */
