/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_solver.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Layout solver implementation file.
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
  /* Get learn-to-rank feature name by identifier function.
    * ARGUMENTS:
    *   - feature id:
    *       INT FeatureNo;
    * RETURNS:
    *   (std::string) JSON feature key.
    */
  std::string GetRankFeatureName( INT FeatureNo )
  {
    static const std::string FeatureNames[(INT)eRANK_COUNT]
    {
      "prefer_wall",
      "prefer_corner",
      "keep_center_free",
      "is_large",
      "role_anchor",
      "role_storage",
      "role_table",
      "role_seating",
      "room_bathroom",
      "room_dining_or_kitchen",
      "wall_closeness",
      "center_closeness",
      "door_clearance",
      "window_clearance",
      "corner_closeness",
      "wall_aligned",
      "faces_room_center",
      "has_table_context",
      "table_distance_fit",
      "table_facing_alignment"
    };

    if (FeatureNo < 0 || FeatureNo >= (INT)eRANK_COUNT)
      return "";
    return FeatureNames[FeatureNo];
  } /* End of 'GetRankFeatureName' function */

  /* Build default-sized learn-to-rank feature vector function.
    * ARGUMENTS: None.
    * RETURNS:
    *   (std::vector<DBL>) zero-filled feature vector.
    */
  std::vector<DBL> MakeEmptyRankFeatureVector( VOID )
  {
    return std::vector<DBL>((size_t)eRANK_COUNT, 0.0);
  } /* End of 'MakeEmptyRankFeatureVector' function */

  /* Find learn-to-rank dataset file name function.
    * ARGUMENTS: None.
    * RETURNS:
    *   (std::string) resolved dataset file name or empty string.
    */
  std::string FindLayoutRankDatabaseFileName( VOID )
  {
    std::vector<std::string> Candidates
    {
      "bin/layout_ai/layout_rank_dataset.json",
      "../bin/layout_ai/layout_rank_dataset.json",
      "../../bin/layout_ai/layout_rank_dataset.json",
      "../../../bin/layout_ai/layout_rank_dataset.json",
      "../../../../bin/layout_ai/layout_rank_dataset.json"
    };

    for (auto &Candidate : Candidates)
      if (std::filesystem::exists(Candidate))
        return Candidate;
    return "";
  } /* End of 'FindLayoutRankDatabaseFileName' function */

  /* Find trained learn-to-rank model file name function.
   * ARGUMENTS: None.
   * RETURNS:
   *   (std::string) resolved model file name or empty string.
   */
  std::string FindLayoutRankModelFileName( VOID )
  {
    std::vector<std::string> Candidates
    {
      "bin/layout_ai/layout_rank_model.json",
      "../bin/layout_ai/layout_rank_model.json",
      "../../bin/layout_ai/layout_rank_model.json",
      "../../../bin/layout_ai/layout_rank_model.json",
      "../../../../bin/layout_ai/layout_rank_model.json"
    };

    for (auto &Candidate : Candidates)
      if (std::filesystem::exists(Candidate))
        return Candidate;
    return "";
  } /* End of 'FindLayoutRankModelFileName' function */

  /* Load one double array from JSON array function.
    * ARGUMENTS:
    *   - JSON array:
    *       const json &ArrayJson;
    * RETURNS:
    *   (std::vector<DBL>) parsed array.
    */
  std::vector<DBL> LoadDoubleArray( const json &ArrayJson )
  {
    std::vector<DBL> Result {};

    for (auto &Value : ArrayJson.GetArray())
      Result.push_back(Value.GetDouble());
    return Result;
  } /* End of 'LoadDoubleArray' function */

  /* Get metric value from model JSON function.
   * ARGUMENTS:
   *   - root JSON:
   *       const json &Root;
   *   - section name:
   *       const std::string &SectionName;
   *   - metric name:
   *       const std::string &MetricName;
   * RETURNS:
   *   (DBL) metric value or 0.
   */
  DBL GetMetricValue( const json &Root, const std::string &SectionName, const std::string &MetricName )
  {
    if (!Root.GetJsObject().contains(SectionName))
      return 0;
    return Root[SectionName][MetricName].GetDouble();
  } /* End of 'GetMetricValue' function */

  /* Evaluate feature vector with trained linear model function.
    * ARGUMENTS:
    *   - source feature vector:
    *       const std::vector<DBL> &Features;
    * RETURNS:
    *   (DBL) linear model score.
    */
  DBL linear_rank_model::Evaluate( const std::vector<DBL> &Features ) const
  {
    DBL Score = Bias;
    INT Count = (INT)mth::Min(Weights.size(), Features.size());

    if (!IsReady)
      return 0;
    for (INT FeatureNo = 0; FeatureNo < Count; FeatureNo++)
    {
      DBL Value = Features[(size_t)FeatureNo];
      DBL Mean = FeatureNo < (INT)FeatureMean.size() ? FeatureMean[(size_t)FeatureNo] : 0.0;
      DBL Scale = FeatureNo < (INT)FeatureScale.size() ? FeatureScale[(size_t)FeatureNo] : 1.0;

      if (fabs(Scale) <= Threshold)
        Scale = 1.0;
      Value = (Value - Mean) / Scale;
      Score += Weights[(size_t)FeatureNo] * Value;
    }

    if (Score >= 0)
    {
      DBL ExpValue = exp(-Score);

      return 1.0 / (1.0 + ExpValue);
    }

    {
      DBL ExpValue = exp(Score);

      return ExpValue / (1.0 + ExpValue);
    }
  } /* End of 'linear_rank_model::Evaluate' function */

  /* Load trained learn-to-rank model from JSON files function.
    * ARGUMENTS:
    *   - database cache:
    *       layout_rank_database &Database;
    * RETURNS:
    *   (BOOL) load result.
    */
  BOOL LoadLayoutRankDatabase( layout_rank_database &Database )
  {
    json_loader Loader {};
    json Root {}, DatasetRoot {};

    Database = {};
    Database.DatasetFileName = FindLayoutRankDatabaseFileName();
    Database.ModelFileName = FindLayoutRankModelFileName();
    if (Database.ModelFileName == "")
      return FALSE;

    if (Database.DatasetFileName != "")
    {
      DatasetRoot = Loader.Load(Database.DatasetFileName);
      Database.SampleCount = (INT)DatasetRoot["samples"].GetArray().size();
    }

    Root = Loader.Load(Database.ModelFileName);
    Database.Model.Weights = LoadDoubleArray(Root["weights"]);
    Database.Model.FeatureMean = LoadDoubleArray(Root["feature_mean"]);
    Database.Model.FeatureScale = LoadDoubleArray(Root["feature_scale"]);
    Database.Model.Bias = Root["bias"].GetDouble();
    Database.FeatureCount = Root["feature_count"].GetInt((INT)Database.Model.Weights.size());
    Database.TrainLoss = GetMetricValue(Root, "train_metrics", "loss");
    Database.ValidationLoss = GetMetricValue(Root, "validation_metrics", "loss");
    Database.TrainAccuracy = GetMetricValue(Root, "train_metrics", "accuracy");
    Database.ValidationAccuracy = GetMetricValue(Root, "validation_metrics", "accuracy");
    Database.Model.IsReady =
      Database.Model.Weights.size() == (size_t)eRANK_COUNT &&
      Database.Model.FeatureMean.size() == (size_t)eRANK_COUNT &&
      Database.Model.FeatureScale.size() == (size_t)eRANK_COUNT;
    Database.IsLoaded = Database.Model.IsReady;
    return Database.IsLoaded;
  } /* End of 'LoadLayoutRankDatabase' function */

  /* Obtain global cached learn-to-rank database function.
    * ARGUMENTS: None.
    * RETURNS:
    *   (const layout_rank_database &) initialized ranking database.
    */
  const layout_rank_database & GetLayoutRankDatabase( VOID )
  {
    static layout_rank_database Database {};
    static std::once_flag LoadOnce {};

    std::call_once(LoadOnce,
      []()
      {
        if (!LoadLayoutRankDatabase(Database))
          logger::warning << "Layout AI model file was not found or could not be loaded. Solver will continue with heuristics only.";
        else
          logger::info << std::format(
            "Layout AI model loaded: samples={}, features={}, train_acc={:.3f}, val_acc={:.3f}.",
            Database.SampleCount, Database.FeatureCount,
            Database.TrainAccuracy, Database.ValidationAccuracy);
      });
    return Database;
  } /* End of 'GetLayoutRankDatabase' function */

  /* Check if item type contains token function.
    * ARGUMENTS:
    *   - prepared item:
    *       const prepared_item &Item;
    *   - token:
    *       const std::string &Token;
    * RETURNS:
    *   (BOOL) contain flag.
    */
  BOOL solver_runtime::ItemContains( const prepared_item &Item, const std::string &Token ) const
  {
    return Item.TypeNameLower.find(Token) != std::string::npos ||
      Item.AssetIdLower.find(Token) != std::string::npos;
  } /* End of 'solver_runtime::ItemContains' function */

  /* Check if current room type contains token function.
    * ARGUMENTS:
    *   - token:
    *       const std::string &Token;
    * RETURNS:
    *   (BOOL) contain flag.
    */
  BOOL solver_runtime::IsRoomType( const std::string &Token ) const
  {
    if (Token == "bath" || Token == "toilet" || Token == "wc")
      return RoomIsBathroom;
    if (Token == "laundry")
      return RoomIsLaundry;
    if (Token == "kitchen")
      return RoomIsKitchen;
    if (Token == "dining")
      return RoomIsDining;
    return FALSE;
  } /* End of 'solver_runtime::IsRoomType' function */

  /* Check if item is bathroom fixture function.
    * ARGUMENTS:
    *   - prepared item:
    *       const prepared_item &Item;
    * RETURNS:
    *   (BOOL) fixture flag.
    */
  BOOL solver_runtime::IsBathroomFixture( const prepared_item &Item ) const
  {
    return Item.IsBathroomFixture;
  } /* End of 'solver_runtime::IsBathroomFixture' function */

  /* Check if item is laundry appliance function.
    * ARGUMENTS:
    *   - prepared item:
    *       const prepared_item &Item;
    * RETURNS:
    *   (BOOL) appliance flag.
    */
  BOOL solver_runtime::IsLaundryAppliance( const prepared_item &Item ) const
  {
    return Item.IsLaundryAppliance;
  } /* End of 'solver_runtime::IsLaundryAppliance' function */

  /* Check if item behaves like dining table function.
    * ARGUMENTS:
    *   - prepared item:
    *       const prepared_item &Item;
    * RETURNS:
    *   (BOOL) dining-table flag.
    */
  BOOL solver_runtime::IsDiningTableLike( const prepared_item &Item ) const
  {
    return Item.IsDiningTableLike;
  } /* End of 'solver_runtime::IsDiningTableLike' function */

  /* Check if item behaves like a table function.
    * ARGUMENTS:
    *   - prepared item:
    *       const prepared_item &Item;
    * RETURNS:
    *   (BOOL) table-like flag.
    */
  BOOL solver_runtime::IsTableLike( const prepared_item &Item ) const
  {
    return Item.Asset.Role == eROLE_TABLE || Item.IsDiningTableLike;
  } /* End of 'solver_runtime::IsTableLike' function */

  /* Obtain target seat-to-table distance function.
    * ARGUMENTS:
    *   - seat-like item:
    *       const prepared_item &Seat;
    *   - table-like item:
    *       const prepared_item &Table;
    * RETURNS:
    *   (FLT) preferred distance between centers.
    */
  FLT solver_runtime::GetTargetDistance( const prepared_item &Seat, const prepared_item &Table ) const
  {
    vec2 SeatFootprint = GetAssetFootprint(Seat.Asset);
    vec2 TableFootprint = GetAssetFootprint(Table.Asset);
    FLT SeatDepth = mth::Max(SeatFootprint.X, SeatFootprint.Y) * 0.45;
    FLT TableDepth = mth::Min(TableFootprint.X, TableFootprint.Y) * 0.5;
    FLT Gap = IsDiningTableLike(Table) ? (FLT)0.2 : (FLT)0.14;
    FLT Target = SeatDepth + TableDepth + Gap;

    // We clamp the result so oversized or tiny models still produce stable behavior
    return mth::Min((FLT)1.3, mth::Max((FLT)0.7, Target));
  } /* End of 'solver_runtime::GetTargetDistance' function */

  /* Obtain semantic front direction function.
    * ARGUMENTS:
    *   - prepared item:
    *       const prepared_item &Item;
    *   - candidate box:
    *       const obb2 &Box;
    * RETURNS:
    *   (vec2) normalized front direction.
    */
  vec2 solver_runtime::GetFrontDirection( const prepared_item &Item, const obb2 &Box ) const
  {
    return GetForwardDirection(Item.Asset, Box);
  } /* End of 'solver_runtime::GetFrontDirection' function */

  /* Obtain facing alignment score function.
    * ARGUMENTS:
    *   - prepared item:
    *       const prepared_item &Item;
    *   - candidate box:
    *       const obb2 &Box;
    *   - target direction:
    *       const vec2 &TargetDir;
    * RETURNS:
    *   (DBL) alignment score.
    */
  DBL solver_runtime::GetFacingScore( const prepared_item &Item, const obb2 &Box, const vec2 &TargetDir ) const
  {
    if (TargetDir.Length2() <= Threshold)
      return 0;
    return mth::Max(0.0, (DBL)(GetFrontDirection(Item, Box) & TargetDir.Normalizing()));
  } /* End of 'solver_runtime::GetFacingScore' function */

  /* Obtain opening proximity preference score function.
    * ARGUMENTS:
    *   - prepared item:
    *       const prepared_item &Item;
    *   - candidate:
    *       const layout_candidate &Cand;
    * RETURNS:
    *   (DBL) score contribution.
    */
  DBL solver_runtime::GetOpeningPrefScore( const prepared_item &Item, const layout_candidate &Cand ) const
  {
    DBL Score = 0;

    if (!Problem.Options.UseOpeningPref)
      return 0;

    for (auto &Opening : Openings)
    {
      FLT Dist = Cand.Center.Distance(Opening.Box.Center);

      // Every object should stay a bit away from openings even if it does not overlap them
      if (Opening.IsDoor)
        Score -= mth::Max(0.0, 1.6 - (DBL)Dist * 1.2);
      if (Opening.IsWindow)
        Score -= mth::Max(0.0, 1.1 - (DBL)Dist);

      // Large or tall objects should stay visually farther from doors
      if (Opening.IsDoor && (Item.Asset.IsLarge || Item.Asset.Role == eROLE_STORAGE || IsBathroomFixture(Item)))
        Score -= mth::Max(0.0, 3.2 - (DBL)Dist * 2.0);

      // Storage and bathroom fixtures near windows usually look implausible
      if (Opening.IsWindow && (Item.Asset.Role == eROLE_STORAGE || IsBathroomFixture(Item) ||
        IsLaundryAppliance(Item) || Item.IsWardrobeLike || Item.IsDresserLike))
        Score -= mth::Max(0.0, 2.4 - (DBL)Dist * 1.7);

      // Seats and center objects directly near a window often look like they clip the opening zone
      if (Opening.IsWindow && (Item.Asset.Role == eROLE_SEATING || Item.Asset.Role == eROLE_CENTER))
        Score -= mth::Max(0.0, 1.8 - (DBL)Dist * 1.35);
    }
    return Score * Problem.Options.OpeningWeight;
  } /* End of 'solver_runtime::GetOpeningPrefScore' function */

  /* Obtain room-specific semantic score function.
    * ARGUMENTS:
    *   - prepared item:
    *       const prepared_item &Item;
    *   - candidate:
    *       const layout_candidate &Cand;
    * RETURNS:
    *   (DBL) score contribution.
    */
  DBL solver_runtime::GetRoomSemanticScore( const prepared_item &Item, const layout_candidate &Cand ) const
  {
    DBL Score = GetOpeningPrefScore(Item, Cand);
    FLT WallDist = vec2::Zero().DistanceToPolygonEdges(Problem.Room.Polygon, Cand.Center);
    FLT CenterDist = Cand.Center.Distance(RoomCenter);

    if (!Problem.Options.UseRoomSem)
      return Score;

    // Bathroom fixtures should hug the perimeter and keep the center free
    if (Problem.Options.UseWallPref && RoomIsBathroom && IsBathroomFixture(Item))
    {
      Score += 3.0 / (1.0 + WallDist * 4.0);
      Score += CenterDist * 0.75;
    }

    // Laundry appliances prefer room perimeter because front access matters
    if (Problem.Options.UseWallPref &&
        (RoomIsLaundry || RoomIsBathroom || RoomIsKitchen) && IsLaundryAppliance(Item))
    {
      Score += 2.5 / (1.0 + WallDist * 4.0);
      Score += CenterDist * 0.35;
    }

    // Dining tables often organize the center of kitchen / dining areas
    if ((RoomIsDining || RoomIsKitchen) && IsDiningTableLike(Item))
      Score += 6.0 / (1.0 + CenterDist * 1.5);

    // Storage objects should stay out of central circulation even in generic rooms
    if (Problem.Options.UseWallPref && Item.Asset.Role == eROLE_STORAGE)
      Score += CenterDist * 0.25;

    return Score;
  } /* End of 'solver_runtime::GetRoomSemanticScore' function */

  /* Obtain pair-specific semantic score function.
    * ARGUMENTS:
    *   - first item and candidate:
    *       const prepared_item &A; const layout_candidate &CandA;
    *   - second item and candidate:
    *       const prepared_item &B; const layout_candidate &CandB;
    * RETURNS:
    *   (DBL) score contribution.
    */
  DBL solver_runtime::GetPairSemanticScore( const prepared_item &A, const layout_candidate &CandA, const prepared_item &B, const layout_candidate &CandB ) const
  {
    DBL Score = 0;
    FLT Dist = CandA.Center.Distance(CandB.Center);
    BOOL ABathroom = IsBathroomFixture(A);
    BOOL BBathroom = IsBathroomFixture(B);
    BOOL AToilet = A.IsToiletLike;
    BOOL BToilet = B.IsToiletLike;
    BOOL ASink = A.IsSinkLike;
    BOOL BSink = B.IsSinkLike;
    BOOL ABath = A.IsBathLike;
    BOOL BBath = B.IsBathLike;
    BOOL AWasher = IsLaundryAppliance(A);
    BOOL BWasher = IsLaundryAppliance(B);
    FLT TargetDist = 0;

    // Object pairs add semantic structure on top of pure collision-free placement.
    if (A.Asset.Role == eROLE_TABLE && B.IsSofaLike)
      Score += mth::Max(0.0, 8.0 - (DBL)fabs(Dist - 0.9) * 6.0);
    if (B.Asset.Role == eROLE_TABLE && A.IsSofaLike)
      Score += mth::Max(0.0, 8.0 - (DBL)fabs(Dist - 0.9) * 6.0);
    if (A.Asset.Role == eROLE_SEATING && B.Asset.Role == eROLE_TABLE)
    {
      TargetDist = GetTargetDistance(A, B);
      Score += mth::Max(0.0, 7.5 - (DBL)fabs(Dist - TargetDist) * 7.0);
      Score += 9.0 * GetFacingScore(A, CandA.Box, CandB.Center - CandA.Center);

      // If a chair drifts too far from the table, the whole seating group stops looking usable
      if (Dist > TargetDist + 0.45)
        Score -= (DBL)(Dist - (TargetDist + 0.45)) * 10.0;
    }
    if (B.Asset.Role == eROLE_SEATING && A.Asset.Role == eROLE_TABLE)
    {
      TargetDist = GetTargetDistance(B, A);
      Score += mth::Max(0.0, 7.5 - (DBL)fabs(Dist - TargetDist) * 7.0);
      Score += 9.0 * GetFacingScore(B, CandB.Box, CandA.Center - CandB.Center);

      // The same rule is mirrored for the opposite pair order.
      if (Dist > TargetDist + 0.45)
        Score -= (DBL)(Dist - (TargetDist + 0.45)) * 10.0;
    }
    if (A.Asset.Role == eROLE_ANCHOR && B.Asset.Role == eROLE_CENTER)
      Score += 2.5 * GetFacingScore(A, CandA.Box, CandB.Center - CandA.Center);
    if (B.Asset.Role == eROLE_ANCHOR && A.Asset.Role == eROLE_CENTER)
      Score += 2.5 * GetFacingScore(B, CandB.Box, CandA.Center - CandB.Center);

    // Bathroom fixtures usually form a compact perimeter group
    if (RoomIsBathroom && ABathroom && BBathroom)
    {
      if ((AToilet && BSink) || (BToilet && ASink))
        Score += mth::Max(0.0, 4.5 - (DBL)fabs(Dist - 0.95) * 3.0);
      if ((ABath && BSink) || (BBath && ASink))
        Score += mth::Max(0.0, 2.8 - (DBL)fabs(Dist - 1.2) * 2.0);
      if ((AWasher && BSink) || (BWasher && ASink))
        Score += mth::Max(0.0, 3.4 - (DBL)fabs(Dist - 0.95) * 2.6);
    }

    // Dining tables want chairs around them stronger than a generic table does
    if ((RoomIsDining || RoomIsKitchen) &&
      A.Asset.Role == eROLE_SEATING && IsDiningTableLike(B))
    {
      TargetDist = GetTargetDistance(A, B);
      Score += mth::Max(0.0, 8.5 - (DBL)fabs(Dist - TargetDist) * 7.0);
      Score += 10.5 * GetFacingScore(A, CandA.Box, CandB.Center - CandA.Center);
      if (Dist > TargetDist + 0.4)
        Score -= (DBL)(Dist - (TargetDist + 0.4)) * 12.0;
    }
    if ((RoomIsDining || RoomIsKitchen) &&
      B.Asset.Role == eROLE_SEATING && IsDiningTableLike(A))
    {
      TargetDist = GetTargetDistance(B, A);
      Score += mth::Max(0.0, 8.5 - (DBL)fabs(Dist - TargetDist) * 7.0);
      Score += 10.5 * GetFacingScore(B, CandB.Box, CandA.Center - CandB.Center);
      if (Dist > TargetDist + 0.4)
        Score -= (DBL)(Dist - (TargetDist + 0.4)) * 12.0;
    }
    return Score;
  } /* End of 'solver_runtime::GetPairSemanticScore' function */

  /* Build semantic front clearance box function.
    * ARGUMENTS:
    *   - prepared item:
    *       const prepared_item &Item;
    *   - candidate box:
    *       const obb2 &Box;
    * RETURNS:
    *   (obb2) front clearance box.
    */
  obb2 solver_runtime::MakeFrontClearanceBox( const prepared_item &Item, const obb2 &Box ) const
  {
    vec2 FrontDir = GetFrontDirection(Item, Box);
    vec2 SideDir = FrontDir.LeftNormal().Normalizing();
    vec2 SideProjection = obb2().ProjectOBB(Box, SideDir);
    vec2 FrontProjection = obb2().ProjectOBB(Box, FrontDir);
    FLT MinSide = SideProjection.X, MaxSide = SideProjection.Y;
    FLT MinFront = FrontProjection.X, MaxFront = FrontProjection.Y;
    FLT SideHalf = (MaxSide - MinSide) * 0.5 * 0.85;
    FLT FrontHalf = (MaxFront - MinFront) * 0.5;

    // Front clearance is another box attached to the semantic "front" side of the object
    return obb2(Box.Center + FrontDir * (FrontHalf + Item.Asset.FrontClearance / 2.0), SideDir, FrontDir,
      vec2(SideHalf, Item.Asset.FrontClearance / 2.0));
  } /* End of 'solver_runtime::MakeFrontClearanceBox' function */

  /* Choose best wall-aligned rotation function.
    * ARGUMENTS:
    *   - furniture asset:
    *       const furniture_asset &Asset;
    *   - wall direction:
    *       const vec2 &Dir;
    *   - inward normal:
    *       const vec2 &Inward;
    * RETURNS:
    *   (FLT) best rotation angle.
    */
  FLT solver_runtime::ChooseWallRotation( const furniture_asset &Asset, const vec2 &Dir, const vec2 &Inward ) const
  {
    FLT BestAngle = 0;
    FLT BestScore = -FLT_MAX;
    std::vector<FLT> Rotations = Asset.AllowedRotations;

    if (Rotations.empty())
      Rotations = {0, 90, 180, 270};
    for (auto Angle : Rotations)
    {
      obb2 Box(vec2::Zero(), GetAssetFootprint(Asset), Angle);
      FLT Score = fabs(Box.AxisX & Dir) +
        2.0 * mth::Max((FLT)0,
        GetForwardDirection(Asset, Box) & Inward);

      if (Score > BestScore)
      {
        BestScore = Score;
        BestAngle = mth::NormalizeAngle(Angle);
      }
    }
    return BestAngle;
  } /* End of 'solver_runtime::ChooseWallRotation' function */

  /* Find first opening that intersects a box function.
    * ARGUMENTS:
    *   - box:
    *       const obb2 &Box;
    * RETURNS:
    *   (const opening_keepout *) opening pointer or nullptr.
    */
  const opening_keepout * solver_runtime::FindIntersectedOpening( const obb2 &Box ) const
  {
    for (auto &Opening : Openings)
      if (obb2().IsOBBIntersectOBB(Box, Opening.Box))
        return &Opening;
    return nullptr;
  } /* End of 'solver_runtime::FindIntersectedOpening' function */

  /* Add opening violation function.
    * ARGUMENTS:
    *   - evaluation:
    *       state_evaluation &Eval;
    *   - prepared item:
    *       const prepared_item &Item;
    *   - opening keepout:
    *       const opening_keepout &Opening;
    *   - penalty:
    *       DBL Penalty;
    *   - need human-readable violations:
    *       BOOL NeedViolations;
    * RETURNS: None.
    */
  VOID solver_runtime::AddOpeningViolation( state_evaluation &Eval, const prepared_item &Item,
    const opening_keepout &Opening, DBL Penalty, BOOL NeedViolations ) const
  {
    violation Violation {};

    Eval.HardPenalty += Penalty;
    if (!NeedViolations)
      return;

    // Doors and windows are reported separately so debug output is more understandable
    Violation.Code = Opening.IsDoor ? "door_blocked" : "window_blocked";
    Violation.Message = Opening.IsDoor ?
      "Furniture item blocks a door keepout zone" :
      "Furniture item blocks a window keepout zone";
    Violation.InstanceA = Item.Item.InstanceId;
    Violation.InstanceB = Opening.OpeningId;
    Violation.Penalty = Penalty;
    Eval.Violations.push_back(Violation);
  } /* End of 'solver_runtime::AddOpeningViolation' function */

  /* Obtain minimal clearance from candidate box to room walls function.
    * ARGUMENTS:
    *   - candidate box:
    *       const obb2 &Box;
    * RETURNS:
    *   (FLT) minimal corner-to-wall distance.
    */
  FLT solver_runtime::GetWallClearance( const obb2 &Box ) const
  {
    vec2 Corners[4] {};
    FLT Dist = FLT_MAX;

    Box.GetCorners(Corners);

    // Corner distance is enough here because candidates are already known to be inside the polygon
    for (INT CornerNo = 0; CornerNo < 4; CornerNo++)
      for (INT EdgeNo = 0; EdgeNo < (INT)Problem.Room.Polygon.size(); EdgeNo++)
      {
        INT NextEdgeNo = (EdgeNo + 1) % (INT)Problem.Room.Polygon.size();

        Dist = mth::Min(Dist, vec2::SegmentDistance(Corners[CornerNo],
          Problem.Room.Polygon[EdgeNo], Problem.Room.Polygon[NextEdgeNo]));
      }
    return Dist;
  } /* End of 'solver_runtime::GetWallClearance' function */

  /* Obtain distance from candidate center to nearest selected opening type function.
    * ARGUMENTS:
    *   - candidate:
    *       const layout_candidate &Cand;
    *   - count doors:
    *       BOOL CountDoors;
    *   - count windows:
    *       BOOL CountWindows;
    * RETURNS:
    *   (FLT) nearest distance or large sentinel value.
    */
  FLT solver_runtime::GetOpeningDistance( const layout_candidate &Cand, BOOL CountDoors, BOOL CountWindows ) const
  {
    FLT Dist = FLT_MAX;

    for (auto &Opening : Openings)
      if ((CountDoors && Opening.IsDoor) || (CountWindows && Opening.IsWindow))
        Dist = mth::Min(Dist, Cand.Center.Distance(Opening.Box.Center));
    return Dist;
  } /* End of 'solver_runtime::GetOpeningDistance' function */

  /* Obtain distance from candidate center to nearest room corner function.
    * ARGUMENTS:
    *   - candidate:
    *       const layout_candidate &Cand;
    * RETURNS:
    *   (FLT) nearest corner distance.
    */
  FLT solver_runtime::GetCornerDistance( const layout_candidate &Cand ) const
  {
    FLT Dist = FLT_MAX;

    for (auto &Point : Problem.Room.Polygon)
      Dist = mth::Min(Dist, Cand.Center.Distance(Point));
    return Dist;
  } /* End of 'solver_runtime::GetCornerDistance' function */

  /* Check if rotation is cardinal / wall-friendly function.
    * ARGUMENTS:
    *   - rotation angle:
    *       FLT Rotation;
    * RETURNS:
    *   (BOOL) alignment flag.
    */
  BOOL solver_runtime::IsAlignedRotation( FLT Rotation ) const
  {
    FLT Rem = fmod(mth::NormalizeAngle(Rotation), 90.0);
    FLT Diff = mth::Min(Rem, (FLT)90.0 - Rem);

    return Diff <= (FLT)6.0;
  } /* End of 'solver_runtime::IsAlignedRotation' function */

  /* Obtain learn-to-rank linear score for candidate function.
    * ARGUMENTS:
    *   - prepared item:
    *       const prepared_item &Item;
    *   - candidate:
    *       const layout_candidate &Cand;
    *   - optional nearest table item:
    *       const prepared_item *TableItem;
    *   - optional nearest table candidate:
    *       const layout_candidate *TableCand;
    * RETURNS:
    *   (DBL) ranking score.
    */
  DBL solver_runtime::GetRankScore( const prepared_item &Item, const layout_candidate &Cand,
    const prepared_item *TableItem, const layout_candidate *TableCand ) const
  {
    const layout_rank_database &Database = GetLayoutRankDatabase();
    std::vector<DBL> Features = MakeEmptyRankFeatureVector();
    DBL FacingRoomCenter = 0;

    if (!Problem.Options.UseCandRank || !Database.Model.IsReady)
      return 0;

    if (Cand.CenterDistance > Threshold)
      FacingRoomCenter = GetFacingScore(Item, Cand.Box, RoomCenter - Cand.Center);

    // These features describe stable geometric facts about one candidate
    Features[(SIZE_T)eRANK_PREFER_WALL] = Item.Asset.PreferWall ? 1.0 : 0.0;
    Features[(SIZE_T)eRANK_PREFER_CORNER] = Item.Asset.PreferCorner ? 1.0 : 0.0;
    Features[(SIZE_T)eRANK_KEEP_CENTER_FREE] = Item.Asset.KeepCenterFree ? 1.0 : 0.0;
    Features[(SIZE_T)eRANK_IS_LARGE] = Item.Asset.IsLarge ? 1.0 : 0.0;
    Features[(SIZE_T)eRANK_ROLE_ANCHOR] = Item.Asset.Role == eROLE_ANCHOR ? 1.0 : 0.0;
    Features[(SIZE_T)eRANK_ROLE_STORAGE] = Item.Asset.Role == eROLE_STORAGE ? 1.0 : 0.0;
    Features[(SIZE_T)eRANK_ROLE_TABLE] = Item.Asset.Role == eROLE_TABLE ? 1.0 : 0.0;
    Features[(SIZE_T)eRANK_ROLE_SEATING] = Item.Asset.Role == eROLE_SEATING ? 1.0 : 0.0;
    Features[(SIZE_T)eRANK_ROOM_BATHROOM] = RoomIsBathroom ? 1.0 : 0.0;
    Features[(SIZE_T)eRANK_ROOM_DINING] = (RoomIsDining || RoomIsKitchen) ? 1.0 : 0.0;
    Features[(SIZE_T)eRANK_WALL_CLOSENESS] = 1.0 / (1.0 + Cand.WallDistance * 4.0);
    Features[(SIZE_T)eRANK_CENTER_CLOSENESS] = 1.0 / (1.0 + Cand.CenterDistance * 2.0);
    Features[(SIZE_T)eRANK_DOOR_CLEARANCE] = mth::Min(1.0, (DBL)Cand.DoorDistance / 3.0);
    Features[(SIZE_T)eRANK_WINDOW_CLEARANCE] = mth::Min(1.0, (DBL)Cand.WindowDistance / 3.0);
    Features[(SIZE_T)eRANK_CORNER_CLOSENESS] = 1.0 / (1.0 + Cand.CornerDistance * 2.0);
    Features[(SIZE_T)eRANK_WALL_ALIGNED] = Cand.IsWallAligned ? 1.0 : 0.0;
    Features[(SIZE_T)eRANK_FACES_ROOM_CENTER] = FacingRoomCenter;

    // Context features are only filled when we know which table a chair belongs to
    if (TableItem != nullptr && TableCand != nullptr)
    {
      FLT TargetDist = GetTargetDistance(Item, *TableItem);
      FLT Dist = Cand.Center.Distance(TableCand->Center);

      Features[(SIZE_T)eRANK_HAS_TABLE_CONTEXT] = 1.0;
      Features[(SIZE_T)eRANK_TABLE_DISTANCE_FIT] = mth::Max(0.0, 1.0 - fabs((DBL)(Dist - TargetDist)) * 1.6);
      Features[(SIZE_T)eRANK_TABLE_FACING_ALIGNMENT] =
        GetFacingScore(Item, Cand.Box, TableCand->Center - Cand.Center);
    }

    // The trained model predicts probability that one candidate looks human-like.
    // Solver uses a centered score so weak candidates can also receive a negative contribution.
    return (Database.Model.Evaluate(Features) - 0.5) * 2.0;
  } /* End of 'solver_runtime::GetRankScore' function */

  /* Add candidate if it is valid function.
    * ARGUMENTS:
    *   - prepared item:
    *       prepared_item &Item;
    *   - candidate center:
    *       vec2 Center;
    *   - candidate rotation:
    *       FLT Rotation;
    * RETURNS: None.
    */
  VOID solver_runtime::AddCandidate( prepared_item &Item, vec2 Center, FLT Rotation )
  {
    layout_candidate Cand {};
    FLT MinWallGap = 0;

    // Candidate is stored in pivot space because the renderer also places furniture by pivot
    Cand.Rotation = mth::NormalizeAngle(Rotation);
    Cand.Translation = GetTransFromCenter(Item.Asset, Center, Cand.Rotation);
    Cand.Box = MakeAssetOBB(Item.Asset, Cand.Translation, Cand.Rotation);
    Cand.Center = Cand.Box.Center;

    // Early rejection keeps the candidate pool compact and makes the annealer faster later
    if (!obb2().IsOBBInsidePolygon(Cand.Box, Problem.Room.Polygon))
      return;
    MinWallGap = Problem.Options.UseWallPref &&
      (Item.Asset.PreferWall || Item.Asset.Role == eROLE_ANCHOR || Item.Asset.Role == eROLE_STORAGE) ?
      mth::Max((FLT)0.04, Item.Asset.BackClearance * (FLT)0.55) :
      (FLT)0.025;
    if (GetWallClearance(Cand.Box) + Threshold < MinWallGap)
      return;
    if (FindIntersectedOpening(Cand.Box) != nullptr)
      return;
    for (auto &ForbiddenPolygon : Problem.ForbiddenPolygons)
      if (obb2().IsOBBIntersectPolygon(Cand.Box, ForbiddenPolygon))
        return;
    for (auto &Existing : Item.Candidates)
      if ((Existing.Center - Cand.Center).Length2() < 1e-4 &&
          fabs(Existing.Rotation - Cand.Rotation) < 1e-4)
        return;

    // We cache geometric metrics once here because the same candidate will be evaluated many times later
    Cand.WallDistance = vec2::Zero().DistanceToPolygonEdges(Problem.Room.Polygon, Cand.Center);
    Cand.CenterDistance = Cand.Center.Distance(RoomCenter);
    Cand.DoorDistance = GetOpeningDistance(Cand, TRUE, FALSE);
    Cand.WindowDistance = GetOpeningDistance(Cand, FALSE, TRUE);
    Cand.CornerDistance = GetCornerDistance(Cand);
    Cand.IsWallAligned = IsAlignedRotation(Cand.Rotation);

    // Seed score is only a rough prior that says which candidates look promising before pair reasoning
    Cand.SeedScore = 0;
    if (Problem.Options.UseCandRank)
    {
      if (Problem.Options.UseWallPref && Item.Asset.PreferWall)
        Cand.SeedScore += 8.0 / (1.0 + Cand.WallDistance * 4.0);
      if (Problem.Options.UseWallPref && Item.Asset.PreferCorner)
        Cand.SeedScore += 3.0 / (1.0 + Cand.CornerDistance);
      if (Problem.Options.UseWallPref && Item.Asset.KeepCenterFree)
        Cand.SeedScore += Cand.CenterDistance;
      if (Problem.Options.UseWallPref &&
          (Item.Asset.PreferWall || Item.Asset.Role == eROLE_ANCHOR ||
           Item.Asset.Role == eROLE_STORAGE || Item.Asset.Role == eROLE_SEATING))
        Cand.SeedScore += 2.0 * GetFacingScore(Item, Cand.Box, RoomCenter - Cand.Center);

      // Name-based room rules already help to rank candidates before pairwise reasoning starts
      Cand.SeedScore += GetRoomSemanticScore(Item, Cand);
      Cand.RankScore = GetRankScore(Item, Cand);
      Cand.SeedScore += Cand.RankScore * 5.0;
    }

    Item.Candidates.push_back(Cand);
  } /* End of 'solver_runtime::AddCandidate' function */

  /* Build candidate list for item function.
    * ARGUMENTS:
    *   - prepared item:
    *       prepared_item &Item;
    * RETURNS: None.
    */
  VOID solver_runtime::BuildCandidates( prepared_item &Item )
  {
    vec2 Footprint = GetAssetFootprint(Item.Asset);
    FLT Step = mth::Max((FLT)0.45, Problem.Options.GridStep);
    FLT MinX = FLT_MAX, MaxX = -FLT_MAX, MinY = FLT_MAX, MaxY = -FLT_MAX;

    // First pass prefers wall-aligned placements for anchor furniture
    if (Problem.Options.UseWallPref)
      for (INT i = 0; i < (INT)Problem.Room.Polygon.size(); i++)
    {
      INT ni = (i + 1) % (INT)Problem.Room.Polygon.size();
      vec2 P1 = Problem.Room.Polygon[i], P2 = Problem.Room.Polygon[ni];
      vec2 Edge = P2 - P1;
      FLT Len = !Edge;
      vec2 Dir, Inward;
      FLT Rot;
      obb2 RotatedBox {};
      auto AlongRange = std::pair<FLT, FLT> {};
      auto InwardRange = std::pair<FLT, FLT> {};
      FLT HalfAlong, HalfInward, Start, Finish;
      INT Count;

      if (Len <= Threshold)
        continue;

      Dir = Edge / Len;
      Inward = Dir.LeftNormal().Normalizing();
      Rot = ChooseWallRotation(Item.Asset, Dir, Inward);
      RotatedBox = obb2(vec2::Zero(), Footprint, Rot);
      {
        vec2 Projection = obb2().ProjectOBB(RotatedBox, Dir);

        AlongRange = std::pair<FLT, FLT>(Projection.X, Projection.Y);
      }
      {
        vec2 Projection = obb2().ProjectOBB(RotatedBox, Inward);

        InwardRange = std::pair<FLT, FLT>(Projection.X, Projection.Y);
      }
      HalfAlong = (AlongRange.second - AlongRange.first) / 2.0;
      HalfInward = (InwardRange.second - InwardRange.first) / 2.0;
      Start = HalfAlong + Item.Asset.SideClearance;
      Finish = Len - HalfAlong - Item.Asset.SideClearance;

      // Very short walls are skipped because the object cannot be safely centered on them
      if (Finish <= Start)
        continue;

      Count = mth::Max(1, mth::Min(6,
        1 + (INT)((Finish - Start) / mth::Max(Step, Footprint.X * (FLT)0.75))));

      for (INT s = 0; s < Count; s++)
      {
        FLT t = Count == 1 ? (Start + Finish) / 2.0 :
          Start + (Finish - Start) * s / (Count - 1);
        FLT WallGap = Item.Asset.BackClearance + (FLT)0.05;
        vec2 CandidateCenter = P1 + Dir * t +
                                Inward * (HalfInward + WallGap);

        AddCandidate(Item, CandidateCenter, Rot);
      }
    }

    // Corner placements are useful for nightstands and compact storage objects
    if (Problem.Options.UseWallPref)
      for (INT i = 0; i < (INT)Problem.Room.Polygon.size(); i++)
    {
      INT pi = (i - 1 + (INT)Problem.Room.Polygon.size()) % (INT)Problem.Room.Polygon.size();
      INT ni = (i + 1) % (INT)Problem.Room.Polygon.size();
      vec2 PrevDir = (Problem.Room.Polygon[i] - Problem.Room.Polygon[pi]).Normalizing();
      vec2 NextDir = (Problem.Room.Polygon[ni] - Problem.Room.Polygon[i]).Normalizing();
      vec2 Inward = (PrevDir.LeftNormal() + NextDir.LeftNormal()).Normalizing();
      vec2 Longest = Problem.Room.Polygon[i].Distance(Problem.Room.Polygon[pi]) >
        Problem.Room.Polygon[ni].Distance(Problem.Room.Polygon[i]) ? PrevDir : NextDir;
      vec2 CandidateCenter = Problem.Room.Polygon[i] +
        Inward * ((!Footprint) / 2.0 + Item.Asset.BackClearance + (FLT)0.08);
      FLT Rot = ChooseWallRotation(Item.Asset, Longest, Inward);

      AddCandidate(Item, CandidateCenter, Rot);
    }

    // Interior grid gives the optimizer non-wall alternatives when wall placements are impossible
    for (auto &P : Problem.Room.Polygon)
    {
      MinX = mth::Min(MinX, P.X);
      MaxX = mth::Max(MaxX, P.X);
      MinY = mth::Min(MinY, P.Y);
      MaxY = mth::Max(MaxY, P.Y);
    }
    for (FLT X = MinX + Footprint.X / 2.0; X <= MaxX - Footprint.X / 2.0; X += Step)
      for (FLT Y = MinY + Footprint.Y / 2.0; Y <= MaxY - Footprint.Y / 2.0; Y += Step)
        if (vec2::Zero().IsPointInPolygon(Problem.Room.Polygon, vec2(X, Y)))
          for (auto Rot : Item.Asset.AllowedRotations)
            AddCandidate(Item, vec2(X, Y), Rot);

    if (Problem.Options.UseCandRank)
      std::stable_sort(Item.Candidates.begin(), Item.Candidates.end(),
        []( const layout_candidate &A, const layout_candidate &B )
        {
          return A.SeedScore > B.SeedScore;
        });
    if ((INT)Item.Candidates.size() > Problem.Options.MaxCandidatesPerItem)
      Item.Candidates.resize(Problem.Options.MaxCandidatesPerItem);
  } /* End of 'solver_runtime::BuildCandidates' function */

  /* Evaluate state function.
    * ARGUMENTS:
    *   - candidate indices by item:
    *       const std::vector<INT> &State;
    *   - count of active items:
    *       INT ActiveCount;
    *   - ignore missing flags:
    *       const std::vector<BOOL> &IgnoreMissing;
    *   - is violations list needed:
    *       BOOL NeedViolations;
    * RETURNS:
    *   (state_evaluation) evaluation result.
    */
  state_evaluation solver_runtime::EvaluateState( const std::vector<INT> &State, INT ActiveCount,
    const std::vector<BOOL> &IgnoreMissing, BOOL NeedViolations ) const
  {
    state_evaluation Eval {};
    std::vector<FLT> NearestTableDist((size_t)ActiveCount, FLT_MAX);
    std::vector<FLT> NearestTableTarget((size_t)ActiveCount, 0);
    std::vector<BOOL> HasNearestTable((size_t)ActiveCount, FALSE);
    std::vector<INT> NearestTableNo((size_t)ActiveCount, -1);

    for (INT i = 0; i < ActiveCount; i++)
    {
      const prepared_item &Item = Items[i];

      if (State[i] < 0 || State[i] >= (INT)Item.Candidates.size())
      {
        if (Item.Item.Required && !IgnoreMissing[i])
        {
          violation Violation {};

          Eval.HardPenalty += 5000;
          if (NeedViolations)
          {
            Violation.Code = "required_item_missing";
            Violation.Message = "Required furniture item is not placed";
            Violation.InstanceA = Item.Item.InstanceId;
            Violation.Penalty = 5000;
            Eval.Violations.push_back(Violation);
          }
        }
        continue;
      }

      const layout_candidate &Cand = Item.Candidates[State[i]];
      FLT WallDist = Cand.WallDistance;
      FLT CenterDist = Cand.CenterDistance;
      vec2 FrontDir = GetFrontDirection(Item, Cand.Box);
      const opening_keepout *BlockedOpening = nullptr;

      if (!obb2().IsOBBInsidePolygon(Cand.Box, Problem.Room.Polygon))
      {
        violation Violation {};

        Eval.HardPenalty += 2000;
        if (NeedViolations)
        {
          Violation.Code = "outside_room";
          Violation.Message = "Furniture item is placed outside room polygon";
          Violation.InstanceA = Item.Item.InstanceId;
          Violation.Penalty = 2000;
          Eval.Violations.push_back(Violation);
        }
      }

      BlockedOpening = FindIntersectedOpening(Cand.Box);
      if (BlockedOpening != nullptr)
        AddOpeningViolation(Eval, Item, *BlockedOpening,
          BlockedOpening->IsDoor ? 2000 : 1500, NeedViolations);

      for (auto &ForbiddenPolygon : Problem.ForbiddenPolygons)
        if (obb2().IsOBBIntersectPolygon(Cand.Box, ForbiddenPolygon))
        {
          violation Violation {};

          Eval.HardPenalty += 2200;
          if (NeedViolations)
          {
            Violation.Code = "other_room_overlap";
            Violation.Message = "Furniture item intersects a forbidden neighboring room polygon";
            Violation.InstanceA = Item.Item.InstanceId;
            Violation.Penalty = 2200;
            Eval.Violations.push_back(Violation);
          }
        }

      if (Problem.Options.UseWallPref)
      {
        if (Item.Asset.PreferWall)
          Eval.SoftScore += 12.0 / (1.0 + WallDist * 4.0);
        else
          Eval.SoftScore += mth::Min(3.0, (DBL)WallDist);
        if (Item.Asset.KeepCenterFree)
          Eval.SoftScore += CenterDist;
        else if (Item.Asset.Role == eROLE_CENTER)
          Eval.SoftScore += 8.0 / (1.0 + CenterDist);
        if ((Item.Asset.Role == eROLE_ANCHOR || Item.Asset.Role == eROLE_STORAGE) &&
            CenterDist > Threshold)
          Eval.SoftScore += 4.0 * mth::Max(0.0,
            (DBL)(FrontDir & (RoomCenter - Cand.Center).Normalizing()));
        if (Item.Asset.Role == eROLE_SEATING && CenterDist > Threshold)
          Eval.SoftScore += 2.0 * mth::Max(0.0,
            (DBL)(FrontDir & (RoomCenter - Cand.Center).Normalizing()));
      }
      if (Problem.Options.UseCandRank)
        Eval.SoftScore += Cand.RankScore * 3.0;

      // Room- and object-name-specific rules live in a separate helper
      Eval.SoftScore += GetRoomSemanticScore(Item, Cand);

      // Cardinal rotations usually look cleaner in an editor and fit walls better
      if (Problem.Options.UseWallPref)
      {
        FLT Rem = fmod(mth::NormalizeAngle(Cand.Rotation), 90.0);
        FLT Diff = mth::Min(Rem, (FLT)90.0 - Rem);
        Eval.SoftScore += mth::Max(0.0, 4.0 - (DBL)Diff * 0.1);
      }

      if (Item.Asset.FrontClearance > 0)
      {
        obb2 FrontBox = MakeFrontClearanceBox(Item, Cand.Box);
        const opening_keepout *FrontOpening = nullptr;

        // Front clearance keeps objects usable after they are placed
        if (!obb2().IsOBBInsidePolygon(FrontBox, Problem.Room.Polygon))
          Eval.HardPenalty += 350;
        for (auto &ForbiddenPolygon : Problem.ForbiddenPolygons)
          if (obb2().IsOBBIntersectPolygon(FrontBox, ForbiddenPolygon))
            Eval.HardPenalty += 300;

        FrontOpening = FindIntersectedOpening(FrontBox);
        // We keep usable space in front of furniture away from both door and window zones
        if (FrontOpening != nullptr)
          Eval.HardPenalty += FrontOpening->IsDoor ? 350 : 180;
      }
    }

    for (INT i = 0; i < ActiveCount; i++)
      if (State[i] >= 0 && State[i] < (INT)Items[i].Candidates.size())
        for (INT j = i + 1; j < ActiveCount; j++)
          if (State[j] >= 0 && State[j] < (INT)Items[j].Candidates.size())
          {
            const prepared_item &A = Items[i];
            const prepared_item &B = Items[j];
            const layout_candidate &CandA = A.Candidates[State[i]];
            const layout_candidate &CandB = B.Candidates[State[j]];
            FLT Dist = CandA.Center.Distance(CandB.Center);

            if (obb2().IsOBBIntersectOBB(CandA.Box, CandB.Box))
            {
              violation Violation {};

              Eval.HardPenalty += 2500;
              if (NeedViolations)
              {
                Violation.Code = "overlap";
                Violation.Message = "Furniture items intersect each other";
                Violation.InstanceA = A.Item.InstanceId;
                Violation.InstanceB = B.Item.InstanceId;
                Violation.Penalty = 2500;
                Eval.Violations.push_back(Violation);
              }
            }

            if (Problem.Options.UsePairSem)
            {
              // Pair rules refine a collision-free layout into a more believable arrangement
              Eval.SoftScore += GetPairSemanticScore(A, CandA, B, CandB);
            }

            // We remember the closest relevant table for every seating object
            // This lets us later penalize lonely chairs that ended up too far away
            if (Problem.Options.UsePairSem && A.Asset.Role == eROLE_SEATING && IsTableLike(B))
            {
              FLT TargetDist = GetTargetDistance(A, B);

              if (!HasNearestTable[i] || Dist < NearestTableDist[i])
              {
                HasNearestTable[i] = TRUE;
                NearestTableDist[i] = Dist;
                NearestTableTarget[i] = TargetDist;
                NearestTableNo[i] = j;
              }
            }
            if (Problem.Options.UsePairSem && B.Asset.Role == eROLE_SEATING && IsTableLike(A))
            {
              FLT TargetDist = GetTargetDistance(B, A);

              if (!HasNearestTable[j] || Dist < NearestTableDist[j])
              {
                HasNearestTable[j] = TRUE;
                NearestTableDist[j] = Dist;
                NearestTableTarget[j] = TargetDist;
                NearestTableNo[j] = i;
              }
            }

            if (A.Asset.FrontClearance > 0)
            {
              obb2 FrontBox = MakeFrontClearanceBox(A, CandA.Box);

              if (obb2().IsOBBIntersectOBB(FrontBox, CandB.Box))
              {
                violation Violation {};

                Eval.HardPenalty += 400;
                if (NeedViolations)
                {
                  Violation.Code = "front_clearance_blocked";
                  Violation.Message = "Furniture front clearance is blocked by another object";
                  Violation.InstanceA = A.Item.InstanceId;
                  Violation.InstanceB = B.Item.InstanceId;
                  Violation.Penalty = 400;
                  Eval.Violations.push_back(Violation);
                }
              }
            }
          }

    if (Problem.Options.UsePairSem)
      for (INT i = 0; i < ActiveCount; i++)
        if (State[i] >= 0 && State[i] < (INT)Items[i].Candidates.size() &&
            Items[i].Asset.Role == eROLE_SEATING && HasNearestTable[i])
      {
        FLT ExtraDist = NearestTableDist[i] - (NearestTableTarget[i] + (FLT)0.35);
        const layout_candidate &SeatCand = Items[i].Candidates[State[i]];
        const layout_candidate &TableCand = Items[NearestTableNo[i]].Candidates[State[NearestTableNo[i]]];

        // This is the main anti-drift rule for chairs
        // A seating object may vary a little, but it should still belong to a table group
        if (ExtraDist > 0)
          Eval.SoftScore -= (DBL)ExtraDist * 14.0;
        if (Problem.Options.UseCandRank)
          Eval.SoftScore += GetRankScore(Items[i], SeatCand,
            &Items[NearestTableNo[i]], &TableCand) * 7.0;
      }
    return Eval;
  } /* End of 'solver_runtime::EvaluateState' function */

  /* Obtain state energy function.
    * ARGUMENTS:
    *   - state:
    *       const std::vector<INT> &State;
    * RETURNS:
    *   (DBL) result energy.
    */
  DBL solver_runtime::GetEnergy( const std::vector<INT> &State ) const
  {
    std::vector<BOOL> Ignore(Items.size(), FALSE);
    state_evaluation Eval = EvaluateState(State, (INT)Items.size(), Ignore, FALSE);

    return Eval.HardPenalty * 100000.0 - Eval.SoftScore;
  } /* End of 'solver_runtime::GetEnergy' function */

  /* Obtain candidate iteration order function.
    * ARGUMENTS:
    *   - item number:
    *       INT ItemNo;
    *   - random generator:
    *       std::mt19937 &Gen;
    * RETURNS:
    *   (std::vector<INT>) candidate order.
    */
  std::vector<INT> solver_runtime::BuildCandidateOrder( INT ItemNo, std::mt19937 &Gen ) const
  {
    std::vector<INT> Order {};

    Order.resize(Items[ItemNo].Candidates.size());
    for (INT i = 0; i < (INT)Order.size(); i++)
      Order[i] = i;

    // The generator is shared by reference so every random step advances one common sequence
    // This keeps one solve reproducible while still making different solves diverge by seed
    if (Order.size() > 1)
      std::shuffle(Order.begin(), Order.end(), Gen);

    // We bucket close scores together, so the previous shuffle survives inside near-equal groups
    if (Problem.Options.UseCandRank)
      std::stable_sort(Order.begin(), Order.end(),
        [this, ItemNo]( INT A, INT B )
        {
          DBL ScoreA = std::floor(Items[ItemNo].Candidates[A].SeedScore * 4.0) / 4.0;
          DBL ScoreB = std::floor(Items[ItemNo].Candidates[B].SeedScore * 4.0) / 4.0;

          return ScoreA > ScoreB;
        });
    return Order;
  } /* End of 'solver_runtime::BuildCandidateOrder' function */

  /* Pick one candidate from best candidate bucket function.
    * ARGUMENTS:
    *   - sorted candidate list:
    *       const std::vector<std::pair<INT, DBL>> &Candidates;
    *   - random generator:
    *       std::mt19937 &Gen;
    * RETURNS:
    *   (INT) selected candidate id.
    */
  INT solver_runtime::PickFromBucket( const std::vector<std::pair<INT, DBL>> &Candidates, std::mt19937 &Gen ) const
  {
    INT Limit = 0;
    std::vector<DBL> Weights {};
    DBL BestScore = -DBL_MAX;

    if (Candidates.empty())
      return -1;
    if (Candidates.size() == 1)
      return Candidates[0].first;

    BestScore = Candidates[0].second;
    while (Limit < (INT)Candidates.size() && Limit < 4 &&
            BestScore - Candidates[Limit].second <= 1.25)
      Limit++;
    if (Limit <= 1)
      return Candidates[0].first;

    // We prefer the best candidate, but still allow nearby alternatives to survive
    for (INT i = 0; i < Limit; i++)
      Weights.push_back((DBL)(Limit - i) * (DBL)(Limit - i));

    std::discrete_distribution<INT> Pick(Weights.begin(), Weights.end());
    return Candidates[Pick(Gen)].first;
  } /* End of 'solver_runtime::PickFromBucket' function */

  /* Construct initial state function.
    * ARGUMENTS:
    *   - start seed:
    *       UINT StartSeed;
    * RETURNS:
    *   (std::vector<INT>) constructed state.
    */
  std::vector<INT> solver_runtime::ConstructState( UINT StartSeed ) const
  {
    std::vector<INT> State(Items.size(), -1);
    std::vector<BOOL> IgnoreMissing(Items.size(), FALSE);
    std::vector<INT> Deferred {};
    std::mt19937 Gen(StartSeed);

    for (INT ItemNo = 0; ItemNo < (INT)Items.size(); ItemNo++)
    {
      const std::vector<INT> Order = BuildCandidateOrder(ItemNo, Gen);
      std::vector<std::pair<INT, DBL>> ValidCandidates {};

      for (auto CandNo : Order)
      {
        std::vector<INT> Temp = State;
        state_evaluation Eval {};

        Temp[ItemNo] = CandNo;
        Eval = EvaluateState(Temp, ItemNo + 1, IgnoreMissing, FALSE);
        if (Eval.HardPenalty == 0)
          ValidCandidates.push_back({CandNo, Eval.SoftScore});
      }

      std::stable_sort(ValidCandidates.begin(), ValidCandidates.end(),
        []( const std::pair<INT, DBL> &A, const std::pair<INT, DBL> &B )
        {
          return A.second > B.second;
        });
      State[ItemNo] = PickFromBucket(ValidCandidates, Gen);
      if (State[ItemNo] != -1)
        continue;

      if (!Problem.Options.AllowRepair)
        continue;

      // Repair temporarily removes lower-priority objects so anchor objects can stabilize first
      for (auto CandNo : Order)
      {
        std::vector<INT> Temp = State;

        Temp[ItemNo] = CandNo;
        for (INT OldNo = 0; OldNo < ItemNo; OldNo++)
          if (State[OldNo] != -1 && Items[OldNo].Item.Priority < Items[ItemNo].Item.Priority)
          {
            std::vector<BOOL> TempIgnore = IgnoreMissing;
            state_evaluation Eval {};

            Temp[OldNo] = -1;
            TempIgnore[OldNo] = TRUE;
            Eval = EvaluateState(Temp, ItemNo + 1, TempIgnore, FALSE);
            if (Eval.HardPenalty == 0)
            {
              State = Temp;
              IgnoreMissing = TempIgnore;
              if (std::find(Deferred.begin(), Deferred.end(), OldNo) == Deferred.end())
                Deferred.push_back(OldNo);
              break;
            }
            Temp[OldNo] = State[OldNo];
          }
        if (State[ItemNo] != -1)
          break;
      }
    }

    // Reinsert lower-priority items after anchor furniture is fixed
    for (auto ItemNo : Deferred)
    {
      std::vector<std::pair<INT, DBL>> ValidCandidates {};
      std::vector<INT> Order = BuildCandidateOrder(ItemNo, Gen);

      IgnoreMissing[ItemNo] = FALSE;
      for (auto CandNo : Order)
      {
        std::vector<INT> Temp = State;
        state_evaluation Eval {};

        Temp[ItemNo] = CandNo;
        Eval = EvaluateState(Temp, (INT)Items.size(), IgnoreMissing, FALSE);
        if (Eval.HardPenalty == 0)
          ValidCandidates.push_back({CandNo, Eval.SoftScore});
      }

      std::stable_sort(ValidCandidates.begin(), ValidCandidates.end(),
        []( const std::pair<INT, DBL> &A, const std::pair<INT, DBL> &B )
        {
          return A.second > B.second;
        });
      State[ItemNo] = PickFromBucket(ValidCandidates, Gen);
    }
    return State;
  } /* End of 'solver_runtime::ConstructState' function */

  /* Improve state by simulated annealing function.
    * ARGUMENTS:
    *   - source state:
    *       const std::vector<INT> &InitialState;
    *   - start seed:
    *       UINT StartSeed;
    * RETURNS:
    *   (std::vector<INT>) improved state.
    */
  std::vector<INT> solver_runtime::ImproveState( const std::vector<INT> &InitialState, UINT StartSeed ) const
  {
    std::vector<INT> State = InitialState, Best = InitialState;
    std::mt19937 Gen(StartSeed + 9137);
    std::uniform_real_distribution<DBL> Prob(0.0, 1.0);
    DBL Energy = GetEnergy(State), BestEnergy = Energy;

    if (Items.empty())
      return State;

    for (INT Iter = 0; Iter < Problem.Options.AnnealingIterations; Iter++)
    {
      DBL Temp = 2.5 * (1.0 -
        (DBL)Iter / mth::Max(1, Problem.Options.AnnealingIterations)) + 0.05;
      INT ItemNo = (INT)(Gen() % Items.size());
      std::vector<INT> Next = State;
      INT CandCount = (INT)Items[ItemNo].Candidates.size();
      INT CandNo = CandCount == 0 ? -1 :
        (INT)(Gen() % (CandCount + (Items[ItemNo].Item.Required ? 0 : 1))) -
        (Items[ItemNo].Item.Required ? 0 : 1);
      DBL NewEnergy;

      Next[ItemNo] = CandNo;
      NewEnergy = GetEnergy(Next);
      if (NewEnergy < Energy || Prob(Gen) < exp((Energy - NewEnergy) / Temp))
      {
        State = Next;
        Energy = NewEnergy;
        if (Energy < BestEnergy)
        {
          Best = State;
          BestEnergy = Energy;
        }
      }
    }
    return Best;
  } /* End of 'solver_runtime::ImproveState' function */

  /* Refine local orientations and near-table placement for already valid state function.
    * ARGUMENTS:
    *   - source state:
    *       const std::vector<INT> &InitialState;
    * RETURNS:
    *   (std::vector<INT>) polished state.
    */
  std::vector<INT> solver_runtime::PolishState( const std::vector<INT> &InitialState ) const
  {
    std::vector<INT> State = InitialState;
    std::vector<BOOL> Ignore(Items.size(), FALSE);

    if (!Problem.Options.UsePairSem)
      return State;

    // This post-pass keeps the good global arrangement but lets seating locally rotate or shift
    // to a better candidate relative to the nearest table
    for (INT ItemNo = 0; ItemNo < (INT)Items.size(); ItemNo++)
      if (Items[ItemNo].Asset.Role == eROLE_SEATING &&
          State[ItemNo] >= 0 && State[ItemNo] < (INT)Items[ItemNo].Candidates.size())
      {
        INT NearestTableNo = -1;
        FLT NearestTableDist = FLT_MAX;
        INT BestCandNo = State[ItemNo];
        DBL BestScore = -DBL_MAX;

        for (INT OtherNo = 0; OtherNo < (INT)Items.size(); OtherNo++)
          if (OtherNo != ItemNo &&
              State[OtherNo] >= 0 && State[OtherNo] < (INT)Items[OtherNo].Candidates.size() &&
              IsTableLike(Items[OtherNo]))
          {
            FLT Dist = Items[ItemNo].Candidates[State[ItemNo]].Center.Distance(
              Items[OtherNo].Candidates[State[OtherNo]].Center);

            if (Dist < NearestTableDist)
            {
              NearestTableDist = Dist;
              NearestTableNo = OtherNo;
            }
          }

        if (NearestTableNo == -1)
          continue;

        for (INT CandNo = 0; CandNo < (INT)Items[ItemNo].Candidates.size(); CandNo++)
        {
          std::vector<INT> Temp = State;
          state_evaluation Eval {};
          const layout_candidate &Cand = Items[ItemNo].Candidates[CandNo];
          const layout_candidate &TableCand = Items[NearestTableNo].Candidates[State[NearestTableNo]];
          FLT TargetDist = GetTargetDistance(Items[ItemNo], Items[NearestTableNo]);
          FLT Dist = Cand.Center.Distance(TableCand.Center);
          DBL FacingScore = GetFacingScore(Items[ItemNo], Cand.Box, TableCand.Center - Cand.Center);
          DBL RankScore = GetRankScore(Items[ItemNo], Cand, &Items[NearestTableNo], &TableCand);
          DBL Score = 0;

          Temp[ItemNo] = CandNo;
          Eval = EvaluateState(Temp, (INT)Items.size(), Ignore, FALSE);
          if (Eval.HardPenalty > 0.001)
            continue;

          // We mostly preserve global quality, but strongly prefer candidates that actually face the table
          Score = Eval.SoftScore + FacingScore * 12.0 + RankScore * 9.0 -
            fabs((DBL)(Dist - TargetDist)) * 5.0;
          if (Score > BestScore)
          {
            BestScore = Score;
            BestCandNo = CandNo;
          }
        }
        State[ItemNo] = BestCandNo;
      }
    return State;
  } /* End of 'solver_runtime::PolishState' function */

  /* Class constructor.
    * ARGUMENTS:
    *   - input problem:
    *       const problem &NewProblem;
    */
  solver_runtime::solver_runtime( const problem &NewProblem ) : Problem(NewProblem)
  {
    if (Problem.Room.RoomType == "")
      Problem.Room.RoomType = DetectRoomType(Problem.Room.RoomId);

    std::string RoomTypeLower = ToLower(Problem.Room.RoomType);
    std::string RoomIdLower = ToLower(Problem.Room.RoomId);

    vec2::Zero().EnsureCCW(Problem.Room.Polygon);
    RoomCenter = vec2::Zero().PolygonCenter(Problem.Room.Polygon);
    RoomIsBathroom = RoomTypeLower.find("bath") != std::string::npos ||
      RoomTypeLower.find("toilet") != std::string::npos ||
      RoomTypeLower.find("wc") != std::string::npos ||
      RoomIdLower.find("bath") != std::string::npos ||
      RoomIdLower.find("toilet") != std::string::npos ||
      RoomIdLower.find("wc") != std::string::npos;
    RoomIsLaundry = RoomTypeLower.find("laundry") != std::string::npos ||
      RoomIdLower.find("laundry") != std::string::npos;
    RoomIsKitchen = RoomTypeLower.find("kitchen") != std::string::npos ||
      RoomIdLower.find("kitchen") != std::string::npos;
    RoomIsDining = RoomTypeLower.find("dining") != std::string::npos ||
      RoomIdLower.find("dining") != std::string::npos;

    for (auto &Opening : Problem.Room.Openings)
    {
      opening_keepout Keepout {};

      Keepout.OpeningId = Opening.Id;
      Keepout.IsDoor = Opening.IsDoor;
      Keepout.IsWindow = Opening.IsWindow;
      Keepout.Box = MakeOpeningKeepout(Opening, Problem.Room.Polygon);
      Openings.push_back(Keepout);
    }

    // Check the speed!
    for (auto &Item : Problem.Items)
    {
      prepared_item Prepared {};
      auto Asset = std::find_if(Problem.Assets.begin(), Problem.Assets.end(),
        [&Item]( const furniture_asset &FurnitureAsset )
        {
          return FurnitureAsset.AssetId == Item.AssetId;
        });

      if (Asset == Problem.Assets.end())
        continue;
      Prepared.Asset = *Asset;
      ApplyDefaultHeuristics(Prepared.Asset);
      Prepared.Item = Item;
      Prepared.TypeNameLower = ToLower(Prepared.Asset.TypeName);
      Prepared.AssetIdLower = ToLower(Prepared.Asset.AssetId);
      Prepared.IsToiletLike = Prepared.TypeNameLower.find("toilet") != std::string::npos ||
        Prepared.TypeNameLower.find("wc") != std::string::npos ||
        Prepared.AssetIdLower.find("toilet") != std::string::npos ||
        Prepared.AssetIdLower.find("wc") != std::string::npos;
      Prepared.IsSinkLike = Prepared.TypeNameLower.find("sink") != std::string::npos ||
        Prepared.TypeNameLower.find("washbasin") != std::string::npos ||
        Prepared.TypeNameLower.find("lavatory") != std::string::npos ||
        Prepared.AssetIdLower.find("sink") != std::string::npos ||
        Prepared.AssetIdLower.find("washbasin") != std::string::npos ||
        Prepared.AssetIdLower.find("lavatory") != std::string::npos;
      Prepared.IsBathLike = Prepared.TypeNameLower.find("bath") != std::string::npos ||
        Prepared.TypeNameLower.find("bathtub") != std::string::npos ||
        Prepared.TypeNameLower.find("shower") != std::string::npos ||
        Prepared.AssetIdLower.find("bath") != std::string::npos ||
        Prepared.AssetIdLower.find("bathtub") != std::string::npos ||
        Prepared.AssetIdLower.find("shower") != std::string::npos;
      Prepared.IsBathroomFixture = Prepared.IsToiletLike || Prepared.IsSinkLike || Prepared.IsBathLike;
      Prepared.IsLaundryAppliance = Prepared.TypeNameLower.find("washing_machine") != std::string::npos ||
        Prepared.TypeNameLower.find("washer") != std::string::npos ||
        Prepared.AssetIdLower.find("washing_machine") != std::string::npos ||
        Prepared.AssetIdLower.find("washer") != std::string::npos;
      Prepared.IsSofaLike = Prepared.TypeNameLower.find("sofa") != std::string::npos ||
        Prepared.AssetIdLower.find("sofa") != std::string::npos;
      Prepared.IsWardrobeLike = Prepared.TypeNameLower.find("wardrobe") != std::string::npos ||
        Prepared.TypeNameLower.find("cabinet") != std::string::npos ||
        Prepared.AssetIdLower.find("wardrobe") != std::string::npos ||
        Prepared.AssetIdLower.find("cabinet") != std::string::npos;
      Prepared.IsDresserLike = Prepared.TypeNameLower.find("dresser") != std::string::npos ||
        Prepared.TypeNameLower.find("commode") != std::string::npos ||
        Prepared.TypeNameLower.find("chest") != std::string::npos ||
        Prepared.AssetIdLower.find("dresser") != std::string::npos ||
        Prepared.AssetIdLower.find("commode") != std::string::npos ||
        Prepared.AssetIdLower.find("chest") != std::string::npos;
      Prepared.IsDiningTableLike = Prepared.Asset.Role == eROLE_TABLE &&
        Prepared.TypeNameLower.find("coffee") == std::string::npos &&
        Prepared.TypeNameLower.find("nightstand") == std::string::npos &&
        Prepared.AssetIdLower.find("coffee") == std::string::npos &&
        Prepared.AssetIdLower.find("nightstand") == std::string::npos;
      if (Prepared.Item.Priority == 0)
        Prepared.Item.Priority = GetAssetPriority(Prepared.Asset);
      BuildCandidates(Prepared);
      Items.push_back(Prepared);
    }

    std::stable_sort(Items.begin(), Items.end(),
      []( const prepared_item &A, const prepared_item &B )
      {
        if (A.Item.Priority != B.Item.Priority)
          return A.Item.Priority > B.Item.Priority;
        return (A.Asset.Size.X * A.Asset.Size.Z) > (B.Asset.Size.X * B.Asset.Size.Z);
      });
  } /* End of 'solver_runtime::solver_runtime' function */

  /* Run solver function.
    * ARGUMENTS: None.
    * RETURNS:
    *   (solution) result solution.
    */
  solution solver_runtime::Run( VOID ) const
  {
    solution Result {};
    std::vector<INT> BestState {};
    DBL BestEnergy = DBL_MAX;

    for (auto &Item : Items)
      Result.CandidateCount += (INT)Item.Candidates.size();

    if (Items.empty() || Problem.Room.Polygon.size() < 3)
    {
      Result.Infeasible = TRUE;
      Result.StatusMessage = "Layout problem is empty or room polygon is invalid";
      return Result;
    }

    for (INT StartNo = 0; StartNo < mth::Max(1, Problem.Options.MultiStartCount); StartNo++)
    {
      std::vector<INT> State = ConstructState(Problem.Options.RandomSeed + StartNo * 9973);
      std::vector<INT> Improved = ImproveState(State, Problem.Options.RandomSeed + StartNo * 1237);
      std::vector<INT> Polished = PolishState(Improved);
      DBL Energy = GetEnergy(Polished);

      if (Energy < BestEnergy)
      {
        BestState = Polished;
        BestEnergy = Energy;
      }
    }

    if (BestState.empty())
    {
      Result.Infeasible = TRUE;
      Result.StatusMessage = "No candidate states were produced";
      return Result;
    }

    {
      std::vector<BOOL> Ignore(Items.size(), FALSE);
      state_evaluation Eval = EvaluateState(BestState, (INT)Items.size(), Ignore, TRUE);

      Result.HardPenalty = Eval.HardPenalty;
      Result.SoftScore = Eval.SoftScore;
      Result.Violations = Eval.Violations;
      Result.Success = Eval.HardPenalty <= 0.001;
      Result.Infeasible = !Result.Success;
      Result.StatusMessage = Result.Success ? "Layout solved successfully" :
        "No fully valid layout was found for the current room";
      if (Result.Success)
        for (INT i = 0; i < (INT)Items.size(); i++)
          if (BestState[i] >= 0 && BestState[i] < (INT)Items[i].Candidates.size())
          {
            const layout_candidate &Cand = Items[i].Candidates[BestState[i]];
            placement Placement {};

            Placement.InstanceId = Items[i].Item.InstanceId;
            Placement.AssetId = Items[i].Item.AssetId;
            Placement.Position = vec3(Cand.Translation.X, 0, Cand.Translation.Y);
            Placement.RotationY = Cand.Rotation;
            Result.Placements.push_back(Placement);
          }
    }
    return Result;
  } /* End of 'solver_runtime::Run' function */

  /* Solve layout problem function.
    * ARGUMENTS:
    *   - input problem:
    *       const problem &Problem;
    * RETURNS:
    *   (solution) result layout solution.
    */
  solution solver::Solve( const problem &Problem ) const
  {
    solver_runtime Runtime(Problem);
    return Runtime.Run();
  } /* End of 'solver::Solve' function */
} /* end of 'tam' namespace */

/* END OF 'tam_solver.cpp' FILE */
