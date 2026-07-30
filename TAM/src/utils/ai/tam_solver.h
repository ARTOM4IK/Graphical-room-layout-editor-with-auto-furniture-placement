/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_solver.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Layout solver declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 21.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_solver_h_
#define __tam_solver_h_

#include "utils/ai/tam_layout.h"

/* Project namespace */
namespace tam
{
  /* Layout solver candidate representation class */
  class layout_candidate
  {
  public:
    vec2 Translation {};      // Translation in model pivot space
    vec2 Center {};           // OBB center in room
    FLT Rotation = 0;         // Y rotation (degrees)
    obb2 Box {};              // Bound box in room
    FLT WallDistance = 0;     // Dist candidate center to room perimeter
    FLT CenterDistance = 0;   // Dist candidate center to room center
    FLT DoorDistance = 0;     // Dist candidate center to nearest door keepout
    FLT WindowDistance = 0;   // Dist candidate center to nearest window keepout
    FLT CornerDistance = 0;   // Dist candidate center to nearest room corner
    BOOL IsWallAligned = FALSE; // Cached flag for wall-friendly cardinal orientation
    DBL RankScore = 0;        // Learn-to-rank score
    DBL SeedScore = 0;        // Static score before pairwise optimization
  }; /* End of 'layout_candidate' class */

  /* Opening keepout representation class */
  class opening_keepout
  {
  public:
    std::string OpeningId {}; // Source opening identifier
    BOOL IsDoor = FALSE;      // Door flag
    BOOL IsWindow = FALSE;    // Window flag
    obb2 Box {};              // Final OBB of opening keepout zone
  }; /* End of 'opening_keepout' class */

  /* Prepared item representation class */
  class prepared_item
  {
  public:
    furniture_asset Asset {};                    // Asset semantic data
    furniture_request Item {};                   // Requested room instance
    std::vector<layout_candidate> Candidates {}; // Candidate placements for this item
    std::string TypeNameLower {};                // Lower-case type name cache (hot token checks)
    std::string AssetIdLower {};                 // Lower-case asset id cache (hot token checks)
    BOOL IsBathroomFixture = FALSE;              // Cached bathroom fixture semantic flag
    BOOL IsLaundryAppliance = FALSE;             // Cached washing-machine semantic flag
    BOOL IsDiningTableLike = FALSE;              // Cached dining table semantic flag
    BOOL IsToiletLike = FALSE;                   // Cached toilet semantic flag
    BOOL IsSinkLike = FALSE;                     // Cached sink semantic flag
    BOOL IsBathLike = FALSE;                     // Cached bath or shower semantic flag
    BOOL IsSofaLike = FALSE;                     // Cached sofa semantic flag
    BOOL IsWardrobeLike = FALSE;                 // Cached wardrobe/storage semantic flag
    BOOL IsDresserLike = FALSE;                  // Cached dresser/commode semantic flag
  }; /* End of 'prepared_item' class */

  /* State evaluation representation class */
  class state_evaluation
  {
  public:
    DBL HardPenalty = 0;                  // Accumulated hard-constraint penalty
    DBL SoftScore = 0;                    // Accumulated soft-layout score
    std::vector<violation> Violations {}; // Human-readable violations
  }; /* End of 'state_evaluation' class */

  /* Internal solver runtime representation class */
  class solver_runtime
  {
  private:
    problem Problem {};                       // Full copied problem
    std::vector<prepared_item> Items {};      // Prepared and sorted items
    std::vector<opening_keepout> Openings {}; // Keepouts for doors and windows
    vec2 RoomCenter {};                       // Room polygon center
    BOOL RoomIsBathroom = FALSE;              // Cached bathroom flag
    BOOL RoomIsLaundry = FALSE;               // Cached laundry flag
    BOOL RoomIsKitchen = FALSE;               // Cached kitchen flag
    BOOL RoomIsDining = FALSE;                // Cached dining flag

    /* Check if item type contains token function.
     * ARGUMENTS:
     *   - prepared item:
     *       const prepared_item &Item;
     *   - token:
     *       const std::string &Token;
     * RETURNS:
     *   (BOOL) contain flag.
     */
    BOOL ItemContains( const prepared_item &Item, const std::string &Token ) const;

    /* Check if current room type contains token function.
     * ARGUMENTS:
     *   - token:
     *       const std::string &Token;
     * RETURNS:
     *   (BOOL) contain flag.
     */
    BOOL IsRoomType( const std::string &Token ) const;

    /* Check if item is bathroom fixture function.
     * ARGUMENTS:
     *   - prepared item:
     *       const prepared_item &Item;
     * RETURNS:
     *   (BOOL) fixture flag.
     */
    BOOL IsBathroomFixture( const prepared_item &Item ) const;

    /* Check if item is laundry appliance function.
     * ARGUMENTS:
     *   - prepared item:
     *       const prepared_item &Item;
     * RETURNS:
     *   (BOOL) appliance flag.
     */
    BOOL IsLaundryAppliance( const prepared_item &Item ) const;

    /* Check if item behaves like dining table function.
     * ARGUMENTS:
     *   - prepared item:
     *       const prepared_item &Item;
     * RETURNS:
     *   (BOOL) dining-table flag.
     */
    BOOL IsDiningTableLike( const prepared_item &Item ) const;

    /* Check if item behaves like a table function.
     * ARGUMENTS:
     *   - prepared item:
     *       const prepared_item &Item;
     * RETURNS:
     *   (BOOL) table-like flag.
     */
    BOOL IsTableLike( const prepared_item &Item ) const;

    /* Obtain target seat-to-table distance function.
     * ARGUMENTS:
     *   - seat-like item:
     *       const prepared_item &Seat;
     *   - table-like item:
     *       const prepared_item &Table;
     * RETURNS:
     *   (FLT) preferred distance between centers.
     */
    FLT GetTargetDistance( const prepared_item &Seat, const prepared_item &Table ) const;

    /* Obtain semantic front direction function.
     * ARGUMENTS:
     *   - prepared item:
     *       const prepared_item &Item;
     *   - candidate box:
     *       const obb2 &Box;
     * RETURNS:
     *   (vec2) normalized front direction.
     */
    vec2 GetFrontDirection( const prepared_item &Item, const obb2 &Box ) const;

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
    DBL GetFacingScore( const prepared_item &Item, const obb2 &Box, const vec2 &TargetDir ) const;

    /* Obtain opening proximity preference score function.
     * ARGUMENTS:
     *   - prepared item:
     *       const prepared_item &Item;
     *   - candidate:
     *       const layout_candidate &Cand;
     * RETURNS:
     *   (DBL) score contribution.
     */
    DBL GetOpeningPrefScore( const prepared_item &Item, const layout_candidate &Cand ) const;

    /* Obtain room-specific semantic score function.
     * ARGUMENTS:
     *   - prepared item:
     *       const prepared_item &Item;
     *   - candidate:
     *       const layout_candidate &Cand;
     * RETURNS:
     *   (DBL) score contribution.
     */
    DBL GetRoomSemanticScore( const prepared_item &Item, const layout_candidate &Cand ) const;

    /* Obtain pair-specific semantic score function.
     * ARGUMENTS:
     *   - first item and candidate:
     *       const prepared_item &A; const layout_candidate &CandA;
     *   - second item and candidate:
     *       const prepared_item &B; const layout_candidate &CandB;
     * RETURNS:
     *   (DBL) score contribution.
     */
    DBL GetPairSemanticScore( const prepared_item &A, const layout_candidate &CandA, const prepared_item &B, const layout_candidate &CandB ) const;

    /* Build semantic front clearance box function.
     * ARGUMENTS:
     *   - prepared item:
     *       const prepared_item &Item;
     *   - candidate box:
     *       const obb2 &Box;
     * RETURNS:
     *   (obb2) front clearance box.
     */
    obb2 MakeFrontClearanceBox( const prepared_item &Item, const obb2 &Box ) const;

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
    FLT ChooseWallRotation( const furniture_asset &Asset, const vec2 &Dir, const vec2 &Inward ) const;

    /* Find first opening that intersects a box function.
     * ARGUMENTS:
     *   - box:
     *       const obb2 &Box;
     * RETURNS:
     *   (const opening_keepout *) opening pointer or nullptr.
     */
    const opening_keepout * FindIntersectedOpening( const obb2 &Box ) const;

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
    VOID AddOpeningViolation( state_evaluation &Eval, const prepared_item &Item, const opening_keepout &Opening, DBL Penalty, BOOL NeedViolations ) const;

    /* Obtain minimal clearance from candidate box to room walls function.
     * ARGUMENTS:
     *   - candidate box:
     *       const obb2 &Box;
     * RETURNS:
     *   (FLT) minimal corner-to-wall distance.
     */
    FLT GetWallClearance( const obb2 &Box ) const;

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
    FLT GetOpeningDistance( const layout_candidate &Cand, BOOL CountDoors, BOOL CountWindows ) const;

    /* Obtain distance from candidate center to nearest room corner function.
     * ARGUMENTS:
     *   - candidate:
     *       const layout_candidate &Cand;
     * RETURNS:
     *   (FLT) nearest corner distance.
     */
    FLT GetCornerDistance( const layout_candidate &Cand ) const;

    /* Check if rotation is cardinal / wall-friendly function.
     * ARGUMENTS:
     *   - rotation angle:
     *       FLT Rotation;
     * RETURNS:
     *   (BOOL) alignment flag.
     */
    BOOL IsAlignedRotation( FLT Rotation ) const;

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
    DBL GetRankScore( const prepared_item &Item, const layout_candidate &Cand,
      const prepared_item *TableItem = nullptr,
      const layout_candidate *TableCand = nullptr ) const;

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
    VOID AddCandidate( prepared_item &Item, vec2 Center, FLT Rotation );

    /* Build candidate list for item function.
     * ARGUMENTS:
     *   - prepared item:
     *       prepared_item &Item;
     * RETURNS: None.
     */
    VOID BuildCandidates( prepared_item &Item );

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
    state_evaluation EvaluateState( const std::vector<INT> &State, INT ActiveCount,
      const std::vector<BOOL> &IgnoreMissing, BOOL NeedViolations = FALSE ) const;

    /* Obtain state energy function.
     * ARGUMENTS:
     *   - state:
     *       const std::vector<INT> &State;
     * RETURNS:
     *   (DBL) result energy.
     */
    DBL GetEnergy( const std::vector<INT> &State ) const;

    /* Obtain candidate iteration order function.
     * ARGUMENTS:
     *   - item number:
     *       INT ItemNo;
     *   - random generator:
     *       std::mt19937 &Gen;
     * RETURNS:
     *   (std::vector<INT>) candidate order.
     */
    /* Pick one candidate from best candidate bucket function.
     * ARGUMENTS:
     *   - sorted candidate list:
     *       const std::vector<std::pair<INT, DBL>> &Candidates;
     *   - random generator:
     *       std::mt19937 &Gen;
     * RETURNS:
     *   (INT) selected candidate id.
     */
    std::vector<INT> BuildCandidateOrder( INT ItemNo, std::mt19937 &Gen ) const;

    /* Pick one candidate from best candidate bucket function.
     * ARGUMENTS:
     *   - sorted candidate list:
     *       const std::vector<std::pair<INT, DBL>> &Candidates;
     *   - random generator:
     *       std::mt19937 &Gen;
     * RETURNS:
     *   (INT) selected candidate id.
     */
    INT PickFromBucket( const std::vector<std::pair<INT, DBL>> &Candidates, std::mt19937 &Gen ) const;

    /* Construct initial state function.
     * ARGUMENTS:
     *   - start seed:
     *       UINT StartSeed;
     * RETURNS:
     *   (std::vector<INT>) constructed state.
     */
    std::vector<INT> ConstructState( UINT StartSeed ) const;

    /* Improve state by simulated annealing function.
     * ARGUMENTS:
     *   - source state:
     *       const std::vector<INT> &InitialState;
     *   - start seed:
     *       UINT StartSeed;
     * RETURNS:
     *   (std::vector<INT>) improved state.
     */
    std::vector<INT> ImproveState( const std::vector<INT> &InitialState, UINT StartSeed ) const;

    /* Refine local orientations and near-table placement for already valid state function.
     * ARGUMENTS:
     *   - source state:
     *       const std::vector<INT> &InitialState;
     * RETURNS:
     *   (std::vector<INT>) polished state.
     */
    std::vector<INT> PolishState( const std::vector<INT> &InitialState ) const;

  public:
    /* Class constructor.
     * ARGUMENTS:
     *   - input problem:
     *       const problem &NewProblem;
     */
    solver_runtime( const problem &NewProblem );

    /* Run solver function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (solution) result solution.
     */
    solution Run( VOID ) const;
  }; /* End of 'solver_runtime' class */

  /* Public layout solver class */
  class solver
  {
  public:
    /* Solve layout problem function.
     * ARGUMENTS:
     *   - input problem:
     *       const problem &Problem;
     * RETURNS:
     *   (solution) resulting layout solution.
     */
    solution Solve( const problem &Problem ) const;
  }; /* End of 'solver' class */
  /* Learn-to-rank feature identifier enumeration type */
  enum rank_feature_id
  {
    eRANK_PREFER_WALL,            // Asset prefers perimeter placement
    eRANK_PREFER_CORNER,          // Asset likes corners
    eRANK_KEEP_CENTER_FREE,       // Asset should stay away from room center
    eRANK_IS_LARGE,               // Asset occupies large floor area
    eRANK_ROLE_ANCHOR,            // Anchor role flag
    eRANK_ROLE_STORAGE,           // Storage role flag
    eRANK_ROLE_TABLE,             // Table role flag
    eRANK_ROLE_SEATING,           // Seating role flag
    eRANK_ROOM_BATHROOM,          // Bathroom-like room flag
    eRANK_ROOM_DINING,            // Dining or kitchen-like room flag
    eRANK_WALL_CLOSENESS,         // Candidate closeness to room perimeter
    eRANK_CENTER_CLOSENESS,       // Candidate closeness to room center
    eRANK_DOOR_CLEARANCE,         // Normalized distance to nearest door
    eRANK_WINDOW_CLEARANCE,       // Normalized distance to nearest window
    eRANK_CORNER_CLOSENESS,       // Candidate closeness to nearest room corner
    eRANK_WALL_ALIGNED,           // Candidate uses wall-friendly cardinal rotation
    eRANK_FACES_ROOM_CENTER,      // Candidate front looks into room interior
    eRANK_HAS_TABLE_CONTEXT,      // Context flag for seat-table ranking
    eRANK_TABLE_DISTANCE_FIT,     // Candidate distance to nearest table is plausible
    eRANK_TABLE_FACING_ALIGNMENT, // Candidate looks at the nearest table
    eRANK_COUNT                   // Total number of ranking features
  }; /* End of 'rank_feature_id' enum */

  /* One learn-to-rank database sample representation class */
  class rank_dataset_sample
  {
  public:
    std::string GroupId {};         // Preference group identifier
    std::string RoomType {};        // Human-readable room type
    std::string AssetName {};       // Human-readable asset name
    std::vector<DBL> Features {};   // Learn-to-rank feature vector
    DBL Label = 0;                  // Relative human quality score inside one preference group
    std::string Comment {};         // Comment why this sample is good or bad
  }; /* End of 'rank_dataset_sample' class */

  /* Trained linear rank model representation class */
  class linear_rank_model
  {
  public:
    std::vector<DBL> Weights {};      // Learned linear weights
    std::vector<DBL> FeatureMean {};  // Per-feature training mean for normalization
    std::vector<DBL> FeatureScale {}; // Per-feature scale for normalization
    DBL Bias = 0;                     // Global linear bias term
    BOOL IsReady = FALSE;             // If model was loaded from trained file flag

    /* Evaluate feature vector with trained linear model function.
      * ARGUMENTS:
      *   - source feature vector:
      *       const std::vector<DBL> &Features;
      * RETURNS:
      *   (DBL) predicted candidate quality in [0; 1].
      */
    DBL Evaluate( const std::vector<DBL> &Features ) const;
  }; /* End of 'linear_rank_model' class */

  /* Learn-to-rank database cache representation class */
  class layout_rank_database
  {
  public:
    std::string DatasetFileName {}; // Resolved source dataset file name
    std::string ModelFileName {};   // Resolved trained model file name
    INT SampleCount = 0;            // Metadata: dataset sample count used for training
    INT FeatureCount = 0;           // Metadata: number of features in model
    DBL TrainLoss = 0;              // Metadata: training binary cross-entropy
    DBL ValidationLoss = 0;         // Metadata: validation binary cross-entropy
    DBL TrainAccuracy = 0;          // Metadata: training accuracy
    DBL ValidationAccuracy = 0;     // Metadata: validation accuracy
    linear_rank_model Model {};     // Trained linear quality model
    BOOL IsLoaded = FALSE;          // If metadata was initialized flag
  }; /* End of 'layout_rank_database' class */
} /* end of 'tam' namespace */

#endif /* __tam_solver_h_ */

/* END OF 'tam_solver.h' FILE */
