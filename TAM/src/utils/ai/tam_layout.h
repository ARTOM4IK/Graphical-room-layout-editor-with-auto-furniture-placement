/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_layout.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Layout solver common declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 21.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_layout_h_
#define __tam_layout_h_

#include "tam_def.h"

/* Project namespace */
namespace tam
{
  class flat; /* Forward declaration for flat bridge helpers. */

  using obb2 = mth::obb2<FLT>; /* 2D oriented bounding box alias. */

  /* Furniture semantic role enumeration type */
  enum furniture_role
  {
    eROLE_UNKNOWN, /* Generic object without special semantic rules */
    eROLE_ANCHOR,  /* Large object (should be closer to the wall) */
    eROLE_STORAGE, /* Furniture, where people store something */
    eROLE_TABLE,   /* Any table */
    eROLE_SEATING, /* Any seating */
    eROLE_CENTER,  /* Is often in central room area */
    eROLE_PASSAGE, /* Is sensitive to circulation zones */
  }; /* End of 'furniture_role' enum */

  /* Room opening representation class */
  class opening
  {
  public:
    std::string Id {};        // Unique opening identifier
    BOOL IsDoor = FALSE;      // Door flag to prevent blocking it
    BOOL IsWindow = FALSE;    // Window flag to prevent blocking it
    vec2 Point1 {};           // First opening edge point on floor plane
    vec2 Point2 {};           // Second opening edge point on floor plane
    FLT Width = 0;            // Opening width
    FLT Height = 0;           // Opening height
    FLT KeepoutDepth = 0.9;   // How deep the keepout zone goes into room
  }; /* End of 'opening' class */

  /* Room geometry representation class */
  class room_geometry
  {
  public:
    std::string RoomId {};            // Human-readable room identifier
    std::string RoomType {};          // like bedroom, bathroom, office, etc.
    std::vector<vec2> Polygon {};     // Ordered room polygon on XZ plane
    std::vector<opening> Openings {}; // Doors and windows of the room
    FLT WallHeight = 3.0;             // Room wall height
  }; /* End of 'room_geometry' class */

  /* Furniture asset representation class */
  class furniture_asset
  {
  public:
    std::string AssetId {};                // Asset identifier used by solver
    std::string TypeName {};               // Furniture type
    std::string ModelFileName {};          // Model file name
    vec3 Size {};                          // Approximated model size
    vec3 MinBB {};                         // Min bound point in local space
    vec3 MaxBB {};                         // Max bound point in local space
    std::vector<FLT> AllowedRotations {};  // Allowed OY rotations in degrees
    FLT FrontDirectionAngle = 0.0;         // Front direction angle
    FLT FrontClearance = 0.6;              // Required free space in front
    FLT SideClearance = 0.05;              // Required free space on sides
    FLT BackClearance = 0.05;              // Required free space behind
    BOOL PreferWall = FALSE;               // Object prefers wall placement
    BOOL PreferCorner = FALSE;             // Object prefers corner placement
    BOOL KeepCenterFree = FALSE;           // Object should not occupy room center
    BOOL IsLarge = FALSE;                  // Large object flag
    furniture_role Role = eROLE_UNKNOWN;   // Role of the object
  }; /* End of 'furniture_asset' class */

  /* Furniture request representation class */
  class furniture_request
  {
  public:
    std::string InstanceId {}; // Unique room instance identifier
    std::string AssetId {};    // Asset reference
    INT Priority = 0;          // Placement priority
    BOOL Required = TRUE;      // If FALSE item may be skipped
  }; /* End of 'furniture_request' class */

  /* Solver options representation class */
  class solver_options
  {
  public:
    UINT RandomSeed = 1;            // Deterministic random seed
    INT MaxCandidatesPerItem = 96;  // Maximum stored candidates per item
    INT MultiStartCount = 4;        // Number of constructive restarts
    INT AnnealingIterations = 2500; // Simulated annealing iteration count
    FLT GridStep = 0.5;             // Step of interior candidate grid
    BOOL AllowRepair = TRUE;        // Allow temporary removal (repair stage)
    BOOL UseOpeningPref = TRUE;     // Use soft opening-distance preference
    BOOL UseWallPref = TRUE;        // Use wall and center placement preferences
    BOOL UseRoomSem = TRUE;         // Use room-name semantic rules
    BOOL UsePairSem = TRUE;         // Use pairwise furniture semantic rules
    BOOL UseCandRank = TRUE;        // Use seed and learned candidate ranking
    FLT OpeningWeight = 1.0;        // Opening preference multiplier
  }; /* End of 'solver_options' class */

  /* Full layout problem representation class */
  class problem
  {
  public:
    room_geometry Room {};                               // Target room geometry
    std::vector<std::vector<vec2>> ForbiddenPolygons {}; // Neighbor rooms and blocked zones
    std::vector<furniture_asset> Assets {};              // Furniture asset catalog
    std::vector<furniture_request> Items {};             // Furniture instances to place
    solver_options Options {};                           // Solver tuning parameters
  }; /* End of 'problem' class */

  /* Layout placement representation class */
  class placement
  {
  public:
    std::string InstanceId {}; // Instance identifier from room
    std::string AssetId {};    // Asset identifier
    vec3 Position {};          // World space translation
    FLT RotationY = 0;         // World space Y rotation (degrees)
  }; /* End of 'placement' class */

  /* Layout violation representation class */
  class violation
  {
  public:
    std::string Code {};      // Machine-readable violation
    std::string Message {};   // Human-readable violation
    std::string InstanceA {}; // First related instance id
    std::string InstanceB {}; // Second related instance id
    DBL Penalty = 0;          // Hard score penalty
  }; /* End of 'violation' class */

  /* Final layout solution representation class */
  class solution
  {
  public:
    BOOL Success = FALSE;                 // Fully valid layout found
    BOOL Infeasible = FALSE;              // Problem cannot be solved
    DBL HardPenalty = 0;                  // Sum of hard-constraint penalties
    DBL SoftScore = 0;                    // Sum of soft-layout bonuses
    INT CandidateCount = 0;               // Number of generated candidates used
    std::string StatusMessage {};         // Text result
    std::vector<placement> Placements {}; // Final placements
    std::vector<violation> Violations {}; // List of detected violations
  }; /* End of 'solution' class */

  /* Check if source text contains token function.
   * ARGUMENTS:
   *   - source text and token:
   *       const std::string &Text, const std::string &Token;
   * RETURNS:
   *   (BOOL) contain flag.
   */
  BOOL TextContains( const std::string &Text, const std::string &Token );

  /* Detect room semantic type by room name function.
   * ARGUMENTS:
   *   - room name:
   *       const std::string &RoomName;
   * RETURNS:
   *   (std::string) detected room type.
   */
  std::string DetectRoomType( const std::string &RoomName );

  /* Detect furniture semantic role by object type name function.
   * ARGUMENTS:
   *   - type name:
   *       const std::string &TypeName;
   * RETURNS:
   *   (furniture_role) detected role.
   */
  furniture_role DetectFurnitureRole( const std::string &TypeName );

  /* Apply default semantic heuristics to furniture asset function.
   * ARGUMENTS:
   *   - furniture asset:
   *       furniture_asset &Asset;
   * RETURNS: None.
   */
  VOID ApplyDefaultHeuristics( furniture_asset &Asset );

  /* Get semantic placement priority for asset function.
   * ARGUMENTS:
   *   - furniture asset:
   *       const furniture_asset &Asset;
   * RETURNS:
   *   (INT) priority value.
   */
  INT GetAssetPriority( const furniture_asset &Asset );

  /* Get 2D XZ footprint size of asset function.
   * ARGUMENTS:
   *   - furniture asset:
   *       const furniture_asset &Asset;
   * RETURNS:
   *   (vec2) XZ footprint size.
   */
  vec2 GetAssetFootprint( const furniture_asset &Asset );

  /* Get local center offset of asset bound box function.
   * ARGUMENTS:
   *   - furniture asset:
   *       const furniture_asset &Asset;
   * RETURNS:
   *   (vec2) local XZ center offset.
   */
  vec2 GetAssetCenterOffset( const furniture_asset &Asset );

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
  vec2 GetTransFromCenter( const furniture_asset &Asset, const vec2 &Center, FLT Angle );

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
  obb2 MakeAssetOBB( const furniture_asset &Asset, const vec2 &Translation, FLT Angle );

  /* Get semantic front direction of asset function.
   * ARGUMENTS:
   *   - furniture asset:
   *       const furniture_asset &Asset;
   *   - asset box:
   *       const obb2 &Box;
   * RETURNS:
   *   (vec2) normalized front direction.
   */
  vec2 GetForwardDirection( const furniture_asset &Asset, const obb2 &Box );

  /* Build opening keepout OBB function.
   * ARGUMENTS:
   *   - source opening:
   *       const opening &Opening;
   *   - room polygon:
   *       const std::vector<vec2> &RoomPolygon;
   * RETURNS:
   *   (obb2) keepout box built inside room polygon.
   */
  obb2 MakeOpeningKeepout( const opening &Opening, const std::vector<vec2> &RoomPolygon );

  /* Save solver debug log to text file function.
   * ARGUMENTS:
   *   - file name:
   *       const std::string &FileName;
   *   - source problem:
   *       const problem &Problem;
   *   - solver solution:
   *       const solution &Solution;
   * RETURNS:
   *   (BOOL) save result.
   */
  BOOL SaveSolverLog( const std::string &FileName, const problem &Problem, const solution &Solution );

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
  BOOL BuildProblem( const flat &Flat, const std::string &RoomName, problem &Problem, const std::string &RoomType = "" );

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
  BOOL ApplySolution( flat &Flat, const std::string &RoomName, const solution &Solution );

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
  BOOL SolveFlatRoom( flat &Flat, const std::string &RoomName, const std::string &RoomType = "", const std::string &OutputFileName = "" );
} /* end of 'tam' namespace */

#endif /* __tam_layout_h_ */

/* END OF 'tam_layout.h' FILE */
