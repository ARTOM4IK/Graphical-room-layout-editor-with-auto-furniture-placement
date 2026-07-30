/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_layout.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Layout solver common helpers and flat implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 21.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Project namespace */
namespace tam
{
  /* Build synthetic solver instance id from room furniture key and index function.
   * ARGUMENTS:
   *   - furniture name:
   *       const std::string &FurnitureName;
   *   - furniture index inside room list:
   *       INT FurnitureNo;
   * RETURNS:
   *   (std::string) synthetic instance id.
   */
  std::string MakeSyntheticInstanceId( const std::string &FurnitureName, INT FurnitureNo )
  {
    return std::format("{}::{}", FurnitureName, FurnitureNo);
  } /* End of 'MakeSyntheticInstanceId' function */

  /* Build real opening edge inside wall segment function.
   * ARGUMENTS:
   *   - flat and segment:
   *       const flat &Flat; const segment &Segment;
   *   - resulting opening edge points:
   *       vec2 &OutP1, vec2 &OutP2;
   * RETURNS:
   *   (BOOL) build result.
   */
  BOOL BuildOpeningEdgeFromSegment( const flat &Flat, const segment &Segment, vec2 &OutP1, vec2 &OutP2 )
  {
    vec2 WallP1 = Flat.SegmentPoints.at(Segment.Point1).Pos;
    vec2 WallP2 = Flat.SegmentPoints.at(Segment.Point2).Pos;
    vec2 WallDir = WallP2 - WallP1;
    FLT WallLength = !WallDir;

    if (WallLength <= Threshold)
      return FALSE;

    WallDir /= WallLength;

    if (Segment.Type == segment_type::eDOOR)
    {
      FLT DoorWidth = mth::Min(Segment.DoorWidth, WallLength);
      FLT Offset = (WallLength - DoorWidth) * 0.5;

      // Door opening is centered inside the wall segment
      OutP1 = WallP1 + WallDir * Offset;
      OutP2 = WallP1 + WallDir * (Offset + DoorWidth);
      return DoorWidth > Threshold;
    }

    if (Segment.Type == segment_type::eWIN)
    {
      FLT LeftOffset = mth::Max((FLT)0, Segment.WindowLeftDist);
      FLT RightOffset = mth::Max((FLT)0, Segment.WindowRightDist);
      FLT Start = LeftOffset;
      FLT End = WallLength - RightOffset;

      // Window offsets in flat are stored in the segment local direction
      // So we use Segment.Point1 -> Segment.Point2
      if (Start >= End - Threshold)
        return FALSE;
      OutP1 = WallP1 + WallDir * Start;
      OutP2 = WallP1 + WallDir * End;
      return TRUE;
    }

    return FALSE;
  } /* End of 'BuildOpeningEdgeFromSegment' function */

  /* Parse synthetic solver instance id function.
   * ARGUMENTS:
   *   - synthetic instance id:
   *       const std::string &InstanceId;
   *   - resulting furniture name:
   *       std::string &FurnitureName;
   *   - resulting furniture index:
   *       INT &FurnitureNo;
   * RETURNS:
   *   (BOOL) parse result.
   */
  BOOL ParseSyntheticInstanceId( const std::string &InstanceId, std::string &FurnitureName, INT &FurnitureNo )
  {
    size_t Pos = InstanceId.rfind("::");

    if (Pos == std::string::npos)
      return FALSE;
    FurnitureName = InstanceId.substr(0, Pos);
    FurnitureNo = atoi(InstanceId.substr(Pos + 2).c_str());
    return FurnitureName != "";
  } /* End of 'ParseSyntheticInstanceId' function */

  /* Find asset in problem catalog function.
   * ARGUMENTS:
   *   - layout problem:
   *       const problem &Problem;
   *   - asset id:
   *       const std::string &AssetId;
   * RETURNS:
   *   (const furniture_asset *) found asset or nullptr.
   */
  const furniture_asset * FindProblemAsset( const problem &Problem, const std::string &AssetId )
  {
    auto It = std::find_if(Problem.Assets.begin(), Problem.Assets.end(),
      [&AssetId]( const furniture_asset &Asset )
      {
        return Asset.AssetId == AssetId;
      });

    return It == Problem.Assets.end() ? nullptr : &*It;
  } /* End of 'FindProblemAsset' function */

    /* Build a seed that changes from solve to solve function.
     * ARGUMENTS:
     *   - room name:
     *       const std::string &RoomName;
     *   - item count:
     *       INT ItemCount;
     * RETURNS:
     *   (UINT) pseudo-random but reproducible seed for one build call.
     */
    UINT BuildProblemRandomSeed( const std::string &RoomName, INT ItemCount )
    {
      static std::atomic_uint Counter = 1;
      UINT TimeSeed = (UINT)std::chrono::high_resolution_clock::now().time_since_epoch().count();
      UINT NameSeed = (UINT)std::hash<std::string>()(RoomName);
      UINT CountSeed = (UINT)(ItemCount * 2654435761u);

      // Counter guarantees that two solves in the same clock tick still differ
      return TimeSeed ^ NameSeed ^ CountSeed ^ Counter.fetch_add(1);
    } /* End of 'BuildProblemRandomSeed' function */

    /* Build render-space OBB for placement function.
     * ARGUMENTS:
     *   - source flat:
     *       const flat &Flat;
     *   - placement:
     *       const placement &Placement;
     *   - output box:
     *       obb2 &OutBox;
     * RETURNS:
     *   (BOOL) build result.
     */
    BOOL TryBuildRenderPlacementBox( const flat &Flat, const placement &Placement, obb2 &OutBox )
    {
      if (!Flat.FlatFurnitureCollection.contains(Placement.AssetId))
        return FALSE;

      auto &Furniture = Flat.FlatFurnitureCollection.at(Placement.AssetId).first;
      vec3 Center = (Furniture.MinBB + Furniture.MaxBB) / 2.0;
      vec3 Size = Furniture.MaxBB - Furniture.MinBB;
      matr World = matr::RotateY(Placement.RotationY) * matr::Translate(Placement.Position);
      vec3 WorldCenter = World.TransformPoint(Center);
      vec3 WorldAxisX = World.TransformVector(vec3(1, 0, 0)).Normalizing();
      vec3 WorldAxisY = World.TransformVector(vec3(0, 0, 1)).Normalizing();

      // Render transform must generate the same box that solver thinks it generated
      OutBox = obb2(vec2(WorldCenter.X, WorldCenter.Z),
        vec2(WorldAxisX.X, WorldAxisX.Z).Normalizing(),
        vec2(WorldAxisY.X, WorldAxisY.Z).Normalizing(),
        vec2(fabs(Size.X) / 2.0, fabs(Size.Z) / 2.0));
      return TRUE;
    } /* End of 'TryBuildRenderPlacementBox' function */

    /* Check solver and renderer placement parity function.
     * ARGUMENTS:
     *   - solver box:
     *       const obb2 &SolverBox;
     *   - render box:
     *       const obb2 &RenderBox;
     * RETURNS:
     *   (BOOL) parity flag.
     */
    BOOL IsRenderPlacementConsistent( const obb2 &SolverBox, const obb2 &RenderBox )
    {
      return SolverBox.Center.Distance(RenderBox.Center) <= 1e-3 &&
        fabs(SolverBox.HalfSize.X - RenderBox.HalfSize.X) <= 1e-3 &&
        fabs(SolverBox.HalfSize.Y - RenderBox.HalfSize.Y) <= 1e-3 &&
        fabs(fabs(SolverBox.AxisX & RenderBox.AxisX) - 1.0) <= 1e-3 &&
        fabs(fabs(SolverBox.AxisY & RenderBox.AxisY) - 1.0) <= 1e-3;
    } /* End of 'IsRenderPlacementConsistent' function */

    /* Validate final solution against problem and renderer function.
     * ARGUMENTS:
     *   - source flat:
     *       const flat &Flat;
     *   - layout problem:
     *       const problem &Problem;
     *   - solver result:
     *       const solution &Solution;
     * RETURNS:
     *   (BOOL) validation result.
     */
    BOOL ValidateSolutionAgainstProblem( const flat &Flat, const problem &Problem, const solution &Solution, std::string *OutMessage = nullptr )
    {
      std::vector<obb2> Boxes {};
      std::vector<obb2> OpeningBoxes {};

      // Opening keepouts are rebuilt here
      // Final guard before touching scene data
      for (auto &Opening : Problem.Room.Openings)
        OpeningBoxes.push_back(MakeOpeningKeepout(Opening, Problem.Room.Polygon));

      for (auto &Placement : Solution.Placements)
      {
        const furniture_asset *Asset = FindProblemAsset(Problem, Placement.AssetId);
        obb2 SolverBox {}, RenderBox {};

        if (Asset == nullptr)
        {
          if (OutMessage != nullptr)
            *OutMessage = std::format("Missing asset '{}' for placement '{}'", Placement.AssetId, Placement.InstanceId);
          return FALSE;
        }
        if (!std::isfinite(Placement.Position.X) || !std::isfinite(Placement.Position.Y) ||
          !std::isfinite(Placement.Position.Z) || !std::isfinite(Placement.RotationY))
        {
          if (OutMessage != nullptr)
            *OutMessage = std::format("Placement '{}' contains non-finite transform values", Placement.InstanceId);
          return FALSE;
        }

        // First we rebuild the solver-space box from the exported placement
        SolverBox = MakeAssetOBB(*Asset, vec2(Placement.Position.X, Placement.Position.Z), Placement.RotationY);
        if (!TryBuildRenderPlacementBox(Flat, Placement, RenderBox))
        {
          if (OutMessage != nullptr)
            *OutMessage = std::format("Render box rebuild failed for placement '{}'", Placement.InstanceId);
          return FALSE;
        }
        if (!IsRenderPlacementConsistent(SolverBox, RenderBox))
        {
          if (OutMessage != nullptr)
            *OutMessage = std::format("Render parity mismatch for placement '{}'", Placement.InstanceId);
          return FALSE;
        }
        if (!obb2().IsOBBInsidePolygon(SolverBox, Problem.Room.Polygon))
        {
          if (OutMessage != nullptr)
            *OutMessage = std::format("Placement '{}' is outside room polygon", Placement.InstanceId);
          return FALSE;
        }

        // Neighbor rooms remain forbidden even if room polygons are tightly connected
        for (auto &ForbiddenPolygon : Problem.ForbiddenPolygons)
          if (obb2().IsOBBIntersectPolygon(SolverBox, ForbiddenPolygon))
          {
            if (OutMessage != nullptr)
              *OutMessage = std::format("Placement '{}' intersects forbidden polygon", Placement.InstanceId);
            return FALSE;
          }

        // Openings are also a hard stop for the final placement
        for (auto &OpeningBox : OpeningBoxes)
          if (obb2().IsOBBIntersectOBB(SolverBox, OpeningBox))
          {
            if (OutMessage != nullptr)
              *OutMessage = std::format("Placement '{}' intersects opening keepout", Placement.InstanceId);
            return FALSE;
          }

        // Furniture items still may not overlap one another
        for (auto &OtherBox : Boxes)
          if (obb2().IsOBBIntersectOBB(SolverBox, OtherBox))
          {
            if (OutMessage != nullptr)
              *OutMessage = std::format("Placement '{}' overlaps another furniture item", Placement.InstanceId);
            return FALSE;
          }
        Boxes.push_back(SolverBox);
      }
      return TRUE;
    } /* End of 'ValidateSolutionAgainstProblem' function */

    /* Dump compact solver debug information function.
     * ARGUMENTS:
     *   - room name:
     *       const std::string &RoomName;
     *   - layout problem:
     *       const problem &Problem;
     *   - solver result:
     *       const solution &Solution;
     * RETURNS: None.
     */
    VOID LogSolverDebug( const std::string &RoomName, const problem &Problem, const solution &Solution )
    {
      logger::out << std::format(
        "Layout solver debug for room '{}': status='{}', items={}, openings={}, hard_penalty={}, soft_score={}.",
        RoomName, Solution.StatusMessage, Problem.Items.size(), Problem.Room.Openings.size(),
        Solution.HardPenalty, Solution.SoftScore);
    } /* End of 'LogSolverDebug' function */

    /* Check if source text contains token function.
     * ARGUMENTS:
     *   - source text and token:
     *       const std::string &Text, const std::string &Token;
     * RETURNS:
     *   (BOOL) contain flag.
     */
    BOOL TextContains( const std::string &Text, const std::string &Token )
    {
      return ToLower(Text).find(ToLower(Token)) != std::string::npos;
    } /* End of 'TextContains' function */

    /* Detect room semantic type by room name function.
     * ARGUMENTS:
     *   - room name:
     *       const std::string &RoomName;
     * RETURNS:
     *   (std::string) detected room type.
     */
    std::string DetectRoomType( const std::string &RoomName )
    {
      if (TextContains(RoomName, "bed") || TextContains(RoomName, "sleep"))
        return "bedroom";
      if (TextContains(RoomName, "living") || TextContains(RoomName, "hall"))
        return "living_room";
      if (TextContains(RoomName, "office") || TextContains(RoomName, "desk"))
        return "office";
      if (TextContains(RoomName, "bath") || TextContains(RoomName, "toilet") ||
        TextContains(RoomName, "wc"))
        return "bathroom";
      if (TextContains(RoomName, "laundry") || TextContains(RoomName, "washing"))
        return "laundry_room";
      if (TextContains(RoomName, "dining"))
        return "dining_room";
      if (TextContains(RoomName, "kitchen"))
        return "kitchen";
      return "generic";
    } /* End of 'DetectRoomType' function */

    /* Detect furniture semantic role by object type name function.
     * ARGUMENTS:
     *   - type name:
     *       const std::string &TypeName;
     * RETURNS:
     *   (furniture_role) detected role.
     */
    furniture_role DetectFurnitureRole( const std::string &TypeName )
    {
      // The slowest place!!!
      if (TextContains(TypeName, "bed") || TextContains(TypeName, "sofa") ||
        TextContains(TypeName, "wardrobe") || TextContains(TypeName, "desk") ||
        TextContains(TypeName, "toilet") || TextContains(TypeName, "wc") ||
        TextContains(TypeName, "bath") || TextContains(TypeName, "bathtub") ||
        TextContains(TypeName, "shower") || TextContains(TypeName, "sink") ||
        TextContains(TypeName, "washbasin") || TextContains(TypeName, "lavatory"))
        return eROLE_ANCHOR;
      if (TextContains(TypeName, "storage") || TextContains(TypeName, "cabinet") ||
        TextContains(TypeName, "dresser") || TextContains(TypeName, "commode") ||
        TextContains(TypeName, "chest") || TextContains(TypeName, "washing_machine") ||
        TextContains(TypeName, "washer") || TextContains(TypeName, "fridge") ||
        TextContains(TypeName, "refrigerator"))
        return eROLE_STORAGE;
      if (TextContains(TypeName, "table"))
        return eROLE_TABLE;
      if (TextContains(TypeName, "chair") || TextContains(TypeName, "seat"))
        return eROLE_SEATING;
      if (TextContains(TypeName, "island") || TextContains(TypeName, "coffee"))
        return eROLE_CENTER;
      return eROLE_UNKNOWN;
    } /* End of 'DetectFurnitureRole' function */

    /* Apply default semantic heuristics to furniture asset function.
     * ARGUMENTS:
     *   - furniture asset:
     *       furniture_asset &Asset;
     * RETURNS: None.
     */
    VOID ApplyDefaultHeuristics( furniture_asset &Asset )
    {
      vec3 Size = Asset.Size;
      FLT PlanArea = Size.X * Size.Z;
      std::string SemanticName = Asset.TypeName == "" ? Asset.AssetId : Asset.TypeName;

      if (Asset.Role == eROLE_UNKNOWN)
        Asset.Role = DetectFurnitureRole(SemanticName);

      if (Asset.AllowedRotations.empty())
        Asset.AllowedRotations = {0, 90, 180, 270};

      // Default clearances depend on object role when asset data did not specify them
      if (Asset.FrontClearance <= 0)
      {
        if (Asset.Role == eROLE_SEATING)
          Asset.FrontClearance = 0.35;
        else if (Asset.Role == eROLE_TABLE)
          Asset.FrontClearance = 0.55;
        else
          Asset.FrontClearance = 0.6;
      }

      if (PlanArea >= 1.2 || Asset.Role == eROLE_ANCHOR || Asset.Role == eROLE_STORAGE)
        Asset.IsLarge = TRUE;

      if (Asset.Role == eROLE_ANCHOR || Asset.Role == eROLE_STORAGE)
        Asset.PreferWall = TRUE;
      if (Asset.PreferWall || Asset.Role == eROLE_ANCHOR || Asset.Role == eROLE_STORAGE)
        Asset.BackClearance = mth::Max(Asset.BackClearance, (FLT)0.08);

      // Large bedroom anchors should immediately move away from the center
      if (TextContains(SemanticName, "bed"))
      {
        Asset.PreferWall = TRUE;
        Asset.KeepCenterFree = TRUE;
        Asset.FrontClearance = mth::Max(Asset.FrontClearance, (FLT)0.75);
        Asset.BackClearance = mth::Max(Asset.BackClearance, (FLT)0.12);

        // Imported bed assets in this project usually keep their semantic front
        // from the headboard side, while solver expects the open usable side
        // Flipping by 180 degrees makes the headboard lean to the wall
        Asset.FrontDirectionAngle = mth::NormalizeAngle(Asset.FrontDirectionAngle /*+ (FLT)180.0*/);
      }

      // Sofas often anchor the seating group and should face the room interior
      if (TextContains(SemanticName, "sofa"))
      {
        Asset.PreferWall = TRUE;
        Asset.KeepCenterFree = TRUE;
        Asset.FrontClearance = mth::Max(Asset.FrontClearance, (FLT)0.75);
        Asset.BackClearance = mth::Max(Asset.BackClearance, (FLT)0.1);
      }

      // Bathroom fixtures mostly live on walls and must keep the center accessible
      if (TextContains(SemanticName, "toilet") || TextContains(SemanticName, "wc"))
      {
        Asset.Role = eROLE_ANCHOR;
        Asset.PreferWall = TRUE;
        Asset.KeepCenterFree = TRUE;
        Asset.FrontClearance = mth::Max(Asset.FrontClearance, (FLT)0.7);
      }
      if (TextContains(SemanticName, "bath") || TextContains(SemanticName, "bathtub") ||
        TextContains(SemanticName, "shower"))
      {
        Asset.Role = eROLE_ANCHOR;
        Asset.IsLarge = TRUE;
        Asset.PreferWall = TRUE;
        Asset.KeepCenterFree = TRUE;
        Asset.FrontClearance = mth::Max(Asset.FrontClearance, (FLT)0.8);
      }
      if (TextContains(SemanticName, "sink") || TextContains(SemanticName, "washbasin") ||
        TextContains(SemanticName, "lavatory"))
      {
        Asset.Role = eROLE_ANCHOR;
        Asset.PreferWall = TRUE;
        Asset.KeepCenterFree = TRUE;
        Asset.FrontClearance = mth::Max(Asset.FrontClearance, (FLT)0.65);
      }

      // Laundry and storage objects are safer and more believable on the room perimeter
      if (TextContains(SemanticName, "washing_machine") || TextContains(SemanticName, "washer"))
      {
        Asset.Role = eROLE_STORAGE;
        Asset.PreferWall = TRUE;
        Asset.KeepCenterFree = TRUE;
        Asset.FrontClearance = mth::Max(Asset.FrontClearance, (FLT)0.65);
      }
      if (TextContains(SemanticName, "dresser") || TextContains(SemanticName, "commode") ||
        TextContains(SemanticName, "chest"))
      {
        Asset.Role = eROLE_STORAGE;
        Asset.PreferWall = TRUE;
        Asset.KeepCenterFree = TRUE;
      }
      if (TextContains(SemanticName, "fridge") || TextContains(SemanticName, "refrigerator"))
      {
        Asset.Role = eROLE_STORAGE;
        Asset.PreferWall = TRUE;
        Asset.KeepCenterFree = TRUE;
        Asset.FrontClearance = mth::Max(Asset.FrontClearance, (FLT)0.8);
      }

      // Dining tables usually own the center of kitchen or dining zones
      if (TextContains(SemanticName, "dining_table") || TextContains(SemanticName, "dining table"))
      {
        Asset.Role = eROLE_TABLE;
        Asset.PreferWall = FALSE;
        Asset.KeepCenterFree = FALSE;
        Asset.FrontClearance = 0;
      }

      if (TextContains(SemanticName, "table") || TextContains(SemanticName, "coffee"))
        Asset.Role = TextContains(SemanticName, "coffee") ? eROLE_CENTER : eROLE_TABLE;
      if (TextContains(SemanticName, "nightstand"))
        Asset.PreferCorner = TRUE;
    } /* End of 'ApplyDefaultHeuristics' function */

    /* Get semantic placement priority for asset function.
     * ARGUMENTS:
     *   - furniture asset:
     *       const furniture_asset &Asset;
     * RETURNS:
     *   (INT) priority value.
     */
    INT GetAssetPriority( const furniture_asset &Asset )
    {
      if (Asset.Role == eROLE_ANCHOR)
        return 100;
      if (Asset.Role == eROLE_STORAGE)
        return 90;
      if (Asset.Role == eROLE_TABLE)
        return 70;
      if (Asset.Role == eROLE_SEATING)
        return 40;
      if (Asset.Role == eROLE_CENTER)
        return 30;
      return Asset.IsLarge ? 80 : 50;
    } /* End of 'GetAssetPriority' function */

    /* Get 2D XZ footprint size of asset function.
     * ARGUMENTS:
     *   - furniture asset:
     *       const furniture_asset &Asset;
     * RETURNS:
     *   (vec2) XZ footprint size.
     */
    vec2 GetAssetFootprint( const furniture_asset &Asset )
    {
      vec3 Size = Asset.MaxBB - Asset.MinBB;

      if (fabs(Size.X) <= Threshold || fabs(Size.Z) <= Threshold)
        Size = Asset.Size;
      if (fabs(Size.X) <= Threshold)
        Size.X = 0.6;
      if (fabs(Size.Z) <= Threshold)
        Size.Z = 0.6;

      return vec2(fabs(Size.X), fabs(Size.Z));
    } /* End of 'GetAssetFootprint' function */

    /* Get local center offset of asset bound box function.
     * ARGUMENTS:
     *   - furniture asset:
     *       const furniture_asset &Asset;
     * RETURNS:
     *   (vec2) local XZ center offset.
     */
    vec2 GetAssetCenterOffset( const furniture_asset &Asset )
    {
      vec3 Size = Asset.MaxBB - Asset.MinBB;

      if (fabs(Size.X) <= Threshold && fabs(Size.Z) <= Threshold)
        return vec2::Zero();
      return vec2((Asset.MinBB.X + Asset.MaxBB.X) * 0.5, (Asset.MinBB.Z + Asset.MaxBB.Z) * 0.5);
    } /* End of 'GetAssetCenterOffset' function */

    /* Build asset translation from desired box center function.
     * ARGUMENTS:
     *   - furniture asset:
     *       const furniture_asset &Asset;
     *   - desired OBB center:
     *       const vec2 &Center;
     *   - rotation angle:
     *       FLT Angle;
     * RETURNS:
     *   (vec2) translation for asset pivot.
     */
    vec2 GetTransFromCenter( const furniture_asset &Asset, const vec2 &Center, FLT Angle )
    {
      // Mirror the sign here so solver-space placement matches the real render transform
      return Center - GetAssetCenterOffset(Asset).Rotate(-Angle);
    } /* End of 'GetTransFromCenter' function */

    /* Build OBB for furniture asset function.
     * ARGUMENTS:
     *   - furniture asset:
     *       const furniture_asset &Asset;
     *   - world translation:
     *       const vec2 &Translation;
     *   - rotation angle:
     *       FLT Angle;
     * RETURNS:
     *   (obb2) resulting OBB.
     */
    obb2 MakeAssetOBB( const furniture_asset &Asset, const vec2 &Translation, FLT Angle )
    {
      // The local BB center must be rotated with the same convention that renderer uses
      vec2 Center = Translation + GetAssetCenterOffset(Asset).Rotate(-Angle);
      return obb2(Center, GetAssetFootprint(Asset), Angle);
    } /* End of 'MakeAssetOBB' function */

    /* Get semantic front direction of asset function.
     * ARGUMENTS:
     *   - furniture asset:
     *       const furniture_asset &Asset;
     *   - asset box:
     *       const obb2 &Box;
     * RETURNS:
     *   (vec2) normalized front direction.
     */
    vec2 GetForwardDirection( const furniture_asset &Asset, const obb2 &Box )
    {
      FLT A = (FLT)D2R(Asset.FrontDirectionAngle);
      FLT BoxAngle = (FLT)R2D(atan2(Box.AxisX.Y, Box.AxisX.X));
      vec2 RenderAxisX = vec2(cos(D2R(BoxAngle)), -sin(D2R(BoxAngle)));
      vec2 RenderAxisZ = vec2(sin(D2R(BoxAngle)), cos(D2R(BoxAngle)));
      vec2 Dir = RenderAxisZ * cos(A) + RenderAxisX * sin(A);

      // Solver facing logic must use the same Y-rotation convention as the real renderer
      // Should fix the problem that an object may look visually turned away while the solver thinks it faces the target
      return Dir.Normalizing();
    } /* End of 'GetForwardDirection' function */

    /* Build opening keepout OBB function.
     * ARGUMENTS:
     *   - source opening:
     *       const opening &Opening;
     *   - room polygon:
     *       const std::vector<vec2> &RoomPolygon;
     * RETURNS:
     *   (obb2) keepout box built inside room polygon.
     */
    obb2 MakeOpeningKeepout( const opening &Opening, const std::vector<vec2> &RoomPolygon )
    {
      vec2 Edge = Opening.Point2 - Opening.Point1;
      FLT Length = !Edge;
      vec2 Dir = Length <= Threshold ? vec2(1, 0) : Edge / Length;
      vec2 Left = Dir.LeftNormal().Normalizing();
      vec2 Right = -Left;
      vec2 MidPoint = (Opening.Point1 + Opening.Point2) / 2.0;
      vec2 RoomMid = vec2::Zero().PolygonCenter(RoomPolygon);
      vec2 Inward = Left;
      FLT TestOffset = 0.12;
      FLT Depth = Opening.KeepoutDepth;
      FLT HalfWidth = Length / 2.0;

      // Door keepout is slightly deeper because a person must pass through it
      if (Opening.IsDoor)
      {
        Depth = mth::Max(Depth, (FLT)1.35);
        HalfWidth += mth::Max((FLT)0.28, Length * (FLT)0.08);
      }
      // Window keepout is shallower, but still push tall storage away from it
      else if (Opening.IsWindow)
      {
        Depth = mth::Max(Depth, (FLT)0.9);
        HalfWidth += mth::Max((FLT)0.22, Length * (FLT)0.06);
      }

      // We choose the normal that really points inside the room polygon
      if (RoomPolygon.size() >= 3)
      {
        BOOL IsLeftInside = vec2::Zero().IsPointInPolygon(RoomPolygon, MidPoint + Left * TestOffset);
        BOOL IsRightInside = vec2::Zero().IsPointInPolygon(RoomPolygon, MidPoint + Right * TestOffset);

        if (IsLeftInside && !IsRightInside)
          Inward = Left;
        else if (!IsLeftInside && IsRightInside)
          Inward = Right;
        else if (((RoomMid - MidPoint) & Right) > ((RoomMid - MidPoint) & Left))
          Inward = Right;
      }

      // The keepout is aligned with the opening edge and pushed into the room interior
      return obb2(MidPoint + Inward * (Depth / 2.0), Dir, Inward, vec2(HalfWidth, Depth / 2.0));
    } /* End of 'MakeOpeningKeepout' function */

    /* Build layout problem from flat room function.
     * ARGUMENTS:
     *   - source flat:
     *       const flat &Flat;
     *   - room name:
     *       const std::string &RoomName;
     *   - result problem:
     *       problem &Problem;
     *   - explicit room type override:
     *       const std::string &RoomType;
     * RETURNS:
     *   (BOOL) build result.
     */
    BOOL BuildProblem( const flat &Flat, const std::string &RoomName, problem &Problem, const std::string &RoomType )
    {
      if (!Flat.Rooms.contains(RoomName))
        return FALSE;

      Problem = {};
      Problem.Room.RoomId = RoomName;
      Problem.Room.RoomType = RoomType == "" ? DetectRoomType(RoomName) : RoomType;
      Problem.Room.WallHeight = Flat.WallsHeight;

      auto &Room = Flat.Rooms.at(RoomName);
      std::vector<vec2> RoomPolygon {};

      if (!Flat.GetRoomPolygon(RoomName, RoomPolygon))
        return FALSE;
      Problem.Room.Polygon = RoomPolygon;
      vec2::Zero().EnsureCCW(Problem.Room.Polygon);

      for (auto &[OtherRoomName, OtherRoom] : Flat.Rooms)
        if (OtherRoomName != RoomName)
        {
          std::vector<vec2> OtherPolygon {};

          if (Flat.GetRoomPolygon(OtherRoomName, OtherPolygon))
          {
            vec2::Zero().EnsureCCW(OtherPolygon);
            Problem.ForbiddenPolygons.push_back(OtherPolygon);
          }
        }

      // Walk along room boundary and convert matching wall segments into solver openings.
      for (INT i = 0; i < (INT)Room.RoomPoints.size(); i++)
      {
        INT ni = (i + 1) % (INT)Room.RoomPoints.size();
        INT P1 = Room.RoomPoints[i], P2 = Room.RoomPoints[ni];

        for (auto &[SegmentId, Segment] : Flat.Segments)
          if ((Segment.Point1 == P1 && Segment.Point2 == P2) ||
              (Segment.Point1 == P2 && Segment.Point2 == P1))
          {
            if (Segment.Type == segment_type::eDOOR || Segment.Type == segment_type::eWIN)
            {
              opening Opening {};
              vec2 OpeningP1 {}, OpeningP2 {};

              if (!BuildOpeningEdgeFromSegment(Flat, Segment, OpeningP1, OpeningP2))
                break;

              Opening.Id = std::format("{}_opening_{}", RoomName, SegmentId);
              Opening.IsDoor = Segment.Type == segment_type::eDOOR;
              Opening.IsWindow = Segment.Type == segment_type::eWIN;
              // The solver must use the real door/window span instead of the whole wall segment
              // But one short opening incorrectly blocks the whole wall for placement
              Opening.Point1 = OpeningP1;
              Opening.Point2 = OpeningP2;
              Opening.Width = Opening.Point1.Distance(Opening.Point2);
              Opening.Height = Segment.Type == segment_type::eDOOR ? Segment.DoorHeight :
                mth::Max((FLT)0.3, Flat.WallsHeight - Segment.WindowBottomDist - Segment.WindowTopDist);
              Opening.KeepoutDepth = Segment.Type == segment_type::eDOOR ? 1.25 : 0.75;
              Problem.Room.Openings.push_back(Opening);
            }
            break;
          }
      }

      std::set<std::string> AddedAssets {};

      for (auto &[FurnitureName, FurnitureEntries] : Room.Furniture)
        if (Flat.FlatFurnitureCollection.contains(FurnitureName))
        {
          auto &AssetPair = Flat.FlatFurnitureCollection.at(FurnitureName);

          if (!AddedAssets.contains(FurnitureName))
          {
            furniture_asset Asset {};

            Asset.AssetId = FurnitureName;
            Asset.TypeName = AssetPair.first.Name;
            Asset.ModelFileName = AssetPair.first.FileName;
            Asset.Size = AssetPair.first.Size;
            Asset.MinBB = AssetPair.first.MinBB;
            Asset.MaxBB = AssetPair.first.MaxBB;
            Asset.FrontDirectionAngle = AssetPair.first.Rotate.Y;
            Asset.AllowedRotations = {0, 90, 180, 270};
            ApplyDefaultHeuristics(Asset);
            Problem.Assets.push_back(Asset);
            AddedAssets.insert(FurnitureName);
          }

          // Each room entry becomes one independent solver item
          for (INT FurnitureNo = 0; FurnitureNo < (INT)FurnitureEntries.size(); FurnitureNo++)
          {
            furniture_request Item {};
            furniture_asset Asset {};

            Asset.AssetId = FurnitureName;
            Asset.TypeName = AssetPair.first.Name;
            Asset.ModelFileName = AssetPair.first.FileName;
            Asset.Size = AssetPair.first.Size;
            Asset.MinBB = AssetPair.first.MinBB;
            Asset.MaxBB = AssetPair.first.MaxBB;
            Asset.FrontDirectionAngle = AssetPair.first.Rotate.Y;
            Asset.AllowedRotations = {0, 90, 180, 270};
            ApplyDefaultHeuristics(Asset);

            Item.InstanceId = MakeSyntheticInstanceId(FurnitureName, FurnitureNo);
            Item.AssetId = Asset.AssetId;
            Item.Priority = GetAssetPriority(Asset);
            Item.Required = TRUE;
            Problem.Items.push_back(Item);
          }
        }

      // Interactive editor solve should stay quick on simple rooms
      // Scale optimization budget by item count
      if ((INT)Problem.Items.size() <= 3)
      {
        Problem.Options.MaxCandidatesPerItem = 72;
        Problem.Options.MultiStartCount = 2;
        Problem.Options.AnnealingIterations = 450;
      }
      else if ((INT)Problem.Items.size() <= 5)
      {
        Problem.Options.MaxCandidatesPerItem = 84;
        Problem.Options.MultiStartCount = 3;
        Problem.Options.AnnealingIterations = 800;
      }
      else
      {
        Problem.Options.MaxCandidatesPerItem = 96;
        Problem.Options.MultiStartCount = 3;
        Problem.Options.AnnealingIterations = 1200;
      }

      // Every build gets a fresh seed to get different result
      Problem.Options.RandomSeed = BuildProblemRandomSeed(RoomName, (INT)Problem.Items.size());
      return !Problem.Items.empty();
    } /* End of 'BuildProblem' function */

    /* Apply solution to flat room function.
     * ARGUMENTS:
     *   - target flat:
     *       flat &Flat;
     *   - room name:
     *       const std::string &RoomName;
     *   - layout solution:
     *       const solution &Solution;
     * RETURNS:
     *   (BOOL) apply result.
     */
    BOOL ApplySolution( flat &Flat, const std::string &RoomName, const solution &Solution )
    {
      if (!Flat.Rooms.contains(RoomName) || !Solution.Success)
        return FALSE;

      for (auto &Placement : Solution.Placements)
      {
        std::string FurnitureName {};
        INT FurnitureNo = 0;

        if (!ParseSyntheticInstanceId(Placement.InstanceId, FurnitureName, FurnitureNo))
          return FALSE;
        Flat.UpdateRoomFurniture(RoomName, FurnitureName, FurnitureNo, Placement.Position, Placement.RotationY);
      }
      return TRUE;
    } /* End of 'ApplySolution' function */

    /* Solve flat room layout function.
     * ARGUMENTS:
     *   - target flat:
     *       flat &Flat;
     *   - room name:
     *       const std::string &RoomName;
     *   - explicit room type override:
     *       const std::string &RoomType;
     *   - unused legacy output file name parameter:
     *       const std::string &OutputFileName;
     * RETURNS:
     *   (BOOL) solve result.
     */
    BOOL SolveFlatRoom( flat &Flat, const std::string &RoomName, const std::string &RoomType, const std::string &OutputFileName )
    {
      problem Problem {};
      solver Solver {};
      solution Result {};
      std::string ValidationMessage {};

      (VOID)OutputFileName;

      if (!BuildProblem(Flat, RoomName, Problem, RoomType))
      {
        logger::warning << "Layout solver failed at problem build stage for room '" << RoomName << "'.";
        return FALSE;
      }
      Result = Solver.Solve(Problem);
      LogSolverDebug(RoomName, Problem, Result);
      if (!Result.Success)
      {
        logger::warning << "Layout solver search failed for room '" << RoomName << "': " << Result.StatusMessage << ".";
        return FALSE;
      }
      if (!ValidateSolutionAgainstProblem(Flat, Problem, Result, &ValidationMessage))
      {
        logger::warning << "Layout solver final validation failed for room '" << RoomName << "': " <<
          ValidationMessage << ".";
        return FALSE;
      }
      if (!ApplySolution(Flat, RoomName, Result))
      {
        logger::warning << "Layout solver apply stage failed for room '" << RoomName << "'.";
        return FALSE;
      }
      return TRUE;
    } /* End of 'SolveFlatRoom' function */
} /* end of 'tam' namespace */

/* END OF 'tam_layout.cpp' FILE */
