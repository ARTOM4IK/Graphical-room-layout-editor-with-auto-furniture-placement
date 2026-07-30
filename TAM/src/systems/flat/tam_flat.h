/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_flat.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Flat main structures declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kupchenkov Ivan (IK4),
 *               Novozhilova Maryana (MN7),
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 30.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_flat_h_
#define __tam_flat_h_

#include "tam_def.h"
#include "tam_furniture.h"

/* Project namespace */
namespace tam
{
  class flat;
  /* Point for wall editor class */
  class segment_point
  {
  public:
    vec2 Pos = vec2(INFINITY);             // Wall point position
    std::vector<INT> SegmentIndices {};    // Indices of segments with this point
    std::vector<std::string> RoomNames {}; // Rooms with this point

    /* Class default constructor */
    segment_point( VOID )
    {
    } /* End of 'segment_point' function */

    /* Class constructor.
     * ARGUMENTS:
     *   - point position:
     *       FLT X, Y;
     */
    segment_point( FLT X, FLT Y )
    {
      Pos = vec2(X, Y);
    } /* End of 'segment_point' function */
  }; /* End of 'segment_point' class */

  /* Wall data representation structure */
  struct segment_wall
  {
  public:
    std::vector<vec2> BasePoints {}; // Base wall points
    material WallMaterial {};        // Wall material
    prim WallPrim {};                // Wall primitive
    FLT TexCoord = 0.0;              // Texture coordinate
    mesh WallMesh {};                // Wall triangle mesh
  }; /* End of 'segment_wall' structure */

  /* Segment type enum */
  enum segment_type
  {
    eWALL, // Default plain wall
    eDOOR, // Wall with door in it
    eWIN   // Wall with window in it
  }; /* End of 'segment_type' enum */

  /* Wall segment representation structure */
  struct segment
  {
    INT Point1 = -1, Point2 = -1;                    // Indices of segment points
    INT Wall1 = -1, Wall2 = -1;                      // Walls' indices from both sides of segment
    segment_type Type = segment_type::eWALL;         // Segment type
    std::vector<vertex_std> AdditionalVertices = {}; // Additional vertices (window, door or none, depending on type)

    FLT
      WindowLeftDist = 0.3,   // Window's left side distance from left side
      WindowRightDist = 0.3,  // Window's right side distance from left side
      WindowBottomDist = 0.5, // Window distance from bottom side
      WindowTopDist = 0.3,    // Window distance from top side
      DoorWidth = 1.0,        // Door width
      DoorHeight = 2.3;       // Door height

    std::vector<std::pair<INT, FLT>> angles1 {}, angles2 {}; // Debug vectors of indices of connected segment with the angle

    /* Class default constructor */
    segment( VOID )
    {
    } /* End of 'segment' function */

    /* Class constructor.
     * ARGUMENTS:
     *   - segment points indeces:
     *       INT P1, P2;
     */
    segment( INT P1, INT P2 )
    {
      Point1 = P1;
      Point2 = P2;
    } /* End of 'segment' function */
  }; /* End of 'segment' structure */

  /* Floor representation type */
  class floor
  {
  private:
    flat *Flat {};                    // Pointer to flat
    // std::vector<INT> FloorPoints {};  // Floor points indices in map of segment points of flat layout
    std::string RoomName {};          // Name of room with this floor

  public:
    prim FloorPrim {};       // Floor primitive
    material FloorMtl {};    // Floor material
    std::string FloorTex {}; // Floor texture name
    FLT FloorScale = 1.0;    // Floor texture scale

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
    BOOL Create( flat *BaseFlat, const std::string &RoomName, FLT YCoord = 0.0 );

    /* Delete floor function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Delete( VOID );

    /* Update floor primitive function.
     * ARGUMENTS:
     *   - Y coordinate of the primitive (floor - 0):
     *       FLT YCoord;
     * RETURNS:
     *   (BOOL) TRUE if floor was successfully triangulated, FALSE otherwise.
     */
    BOOL UpdateFloorPrimitive( FLT YCoord = 0.0 );
  }; /* End of 'floor' class */

  /* Room data representation type */
  struct room
  {
    /* Furniture placing info data type */
    struct furniture_entry
    {
      std::string Name; // Furniture name in global flag model list
      vec3 Pos;         // Place position
      FLT RotationY;    // Place rotation around Y axis in degrees
      INT Id;           // Object pickup Id

      /* Obtain furniture place transform matrix function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (matr) transformation matrix.
       */
      matr GetTransform( VOID ) const
      {
        return
          matr::RotateY(RotationY) *
          matr::Translate(Pos);
      } /* End of 'GetTransform' function */
    }; /* End of 'furniture_entry' structure */
    std::string RoomName {};                               // Name of room
    std::map<std::string, std::vector<furniture_entry>> Furniture {}; // Room's furniture map : key - furniture id,
                                                           //                        value: xyz - translation vector, w - rotate angle.
    floor RoomFloor {};                                    // Room floor
    floor RoomCeiling {};                                  // Room ceiling
    std::vector<INT> RoomPoints {};                        // Room points' indices
    std::vector<std::pair<INT, INT>> RoomSegmentWalls {};  // Room walls' segment indices and wall indices
    FLT Area = 0;                                          // Room's area
    vec2 CenterPoint {};                                   // Center of room space (might be outside of room)
    vec3 WallsColor {};                                    // Walls color (stored in material buffer)
    FLT WallpaperScale = 1.0;                              // Wallpapers (texture) scale
    vec2 WallpaperInitial  = vec2(1.0);                    // Wallpapers initial width and height
    vec2 WallpaperTransformed = vec2(1.0);                 // Wallpaper transformed width and height
    material WallsMaterial {};                             // Room's walls' material
    std::vector<std::string> TextureNames {};              // Vector of texture names
    /// std::vector<INT> RoomWallsIndices {};                         // Room walls' indices
  }; /* End of 'room' structure */

  /* Flat representation type */
  class flat
  {
    friend class floor;
  private:
    anim *Ani {};                                  // Pointer to animation
    INT SegmentPointsCounter = 0;                  // First free index of segment points
    INT SegmentsCounter = 0;                       // First free index of segments
    INT SegmentWallsCounter = 0;                   // First free index of walls

    material_pattern DefaultFlatMtlPat {};         // Default material pattern for flat walls
    material DefaultFlatMtl {};                    // Default material for flat walls

    /// Temporary information for non-room space...

    /* Update segment data function.
     * ARGUMENTS:
     *   - index of segment to update data of:
     *       INT SegmentInd;
     * RETURNS: None.
     */
    VOID UpdateSegmentLayout( INT SegmentInd );

    /* Update (or create) segment walls function.
     * ARGUMENTS:
     *   - indices of segments to be updated:
     *       INT SegmentInd1, SegmentInd2;
     *   - is inner angle needed or outer:
     *       BOOL IsInner;
     * RETURNS: None.
     */
    VOID UpdateSegmentWalls( INT SegmentInd1, INT SegmentInd2, BOOL IsInner = TRUE );

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
    VOID CreateWallPrimBy4Points( INT SegmentInd, INT WallNo, std::vector<vec2> vertices,
                                  FLT TexCoordOffset = 0.0, vec2 TexScale = vec2(1.0) );

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
    VOID UpdateWallPrimWindow( INT SegmentInd, INT WallNo, FLT TexCoordOffset = 0.0, vec2 TexScale = vec2(1.0) );

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
    VOID UpdateWallPrimDoor( INT SegmentInd, INT WallNo, FLT TexCoordOffset = 0.0, vec2 TexScale = vec2(1.0) );

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
    VOID CreateWallPrimByUpdatedBasePoints( INT SegmentInd, INT WallNo, vec2 vertex, INT PointInd, FLT TexCoordOffset = 0.0 );

  public:
    std::map<std::string, room> Rooms {};          // Rooms of flat array
    BOOL IsRoomsUpdate {};                         // Flag if room list was edited
    std::map<INT, segment_point> SegmentPoints {}; // Map of segment points of flat layout
    std::map<INT, segment> Segments {};            // Map of segments of flat layout
    std::map<INT, segment_wall> SegmentWalls {};   // Map of walls of flat layout
    FLT
      WallsWidth = 0.1,                            // Walls width
      WallsHeight = 3.0;                           // Walls height
    std::map<std::string, std::pair<furniture, model>> FlatFurnitureCollection {}; // Map of loaded from collection furniture models
    BOOL
      DrawFurniture = TRUE,  // Flag if needed to draw furniture in rooms
      DrawWalls = TRUE,      // Flag if needed to draw walls
      DrawFloor = TRUE,      // Flag if needed to draw floor
      DrawCeiling = FALSE,   // Flag if needed to draw ceiling
      DrawingScheme = FALSE; // Flag if needed to draw scheme

    std::map<std::string, texture> Wallpapers {};     // Loaded wallpapers textures

    material SchemeMaterial;                       // Scheme material
    prim SchemePrim;                               // Scheme primitive
    std::string SchemeTex;                        // Scheme texture path

    // Scheme params (translate, rotate, scale)
    vec3 SchemeTranslate = vec3(0, 0, 0);
    INT SchemeRotate = 0;
    FLT SchemeScale = 0.0;


    /* Flat destructor. */
    ~flat( VOID );

    /* Create flat function.
     * ARGUMENTS:
     *   - animation context pointer:
     *       anim *Ani;
     * RETURNS: None.
     */
    VOID Create( anim *Ani );

    /* Clear flat informaton function.
     * ARGUMENTS:
     *   - flag for creating texture after clearing:
     *       BOOL IsToCreate;
     * RETURNS: None.
     */
    VOID Clear( BOOL IsToCreate = TRUE );

    /* Update flat material function.
     * ARGUMENTS:
     *   - new walls color:
     *       vec3 NewColor;
     *   - textures to be applied:
     *       std::vector<texture> Texs;
     * RETURNS: None.
     */
    VOID UpdateFlatMaterial( vec3 NewColor, std::vector<texture> Texs = {} );

    /*** Functions for editing flat information ***/

    /* Add segment point function.
     * ARGUMENTS:
     *   - point coords:
     *       FLT X, Y;
     * RETURNS:
     *   (INT) point index in map of segment points of flat layout.
     */
    INT AddPoint( FLT X, FLT Y );

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
    VOID UpdateSelectedPoints( std::vector<INT> PointInds, FLT NewX, FLT NewY, BOOL IsClamping = FALSE );

    /* Delete segment point function.
     * ARGUMENTS:
     *   - point index in map of segment points of flat layout:
     *       INT PointInd;
     * RETURNS: None.
     */
    VOID DeletePoint( INT PointInd );

    /* Find nearest segment point function.
     * ARGUMENTS:
     *   - coords to find near:
     *       FLT X, Y;
     *   - threshold for finding:
     *       FLT FindThreshold;
     * RETURNS:
     *   (INT) point index in map of segment points of flat layout, -1 if no finded point.
     */
    INT FindNearestPoint( FLT X, FLT Y, FLT FindThreshold = 0.1 );

    /* Correct segment function.
     * ARGUMENTS:
     *   - segment to correct:
     *       INT SegmentInd;
     * RETURNS: None.
     */
    VOID CorrectSegment( INT SegmentInd );

    /* Add segment function.
     * ARGUMENTS:
     *   - segment points:
     *       INT PointInd1, PointInd2;
     * RETURNS:
     *   (INT) segment index in map of segments of flat layout.
     */
    INT AddSegment( INT PointInd1, INT PointInd2, BOOL NeedUpdate = TRUE );

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
    VOID UpdateSelectedSegments( std::vector<INT> SegmentInds, FLT NewX, FLT NewY, BOOL IsClamping = FALSE );

    /* Delete segment function.
     * ARGUMENTS:
     *   - segment index in map of segments of flat layout:
     *       INT SegmentInd;
     * RETURNS: None.
     */
    VOID DeleteSegment( INT SegmentInd, BOOL NeedUpdate = TRUE );

    /* Change segment type by recreating wall primitives function.
     * ARGUMENTS:
     *   - index of segment to be updated:
     *       INT SegmentInd;
     *   - new segment type:
     *       segment_type NewType;
     * RETURNS: None.
     */
    VOID ChangeSegmentType( INT SegmentInd, segment_type NewType );

    /* Find nearest segment function.
     * ARGUMENTS:
     *   - coords to find near:
     *       FLT X, Y;
     *   - threshold for finding:
     *       FLT FindThreshold;
     * RETURNS:
     *   (INT) segment index in map of segments of flat layout, -1 if no finded point.
     */
    INT FindNearestSegment( FLT X, FLT Y, FLT FindThreshold = 0.30 );

    /* Add room by control points function.
     * ARGUMENTS:
     *   - name of room:
     *       const std::string &Name;
     *   - array of room's border points:
     *       const std::vector<INT> &RoomPoints;
     * RETURNS:
     *   (BOOL) TRUE if room was successfully added, FALSE otherwise.
     */
    BOOL AddRoom( const std::string &Name, const std::vector<INT> &RoomPoints );

    /* Delete room function.
     * ARGUMENTS:
     *   - name of room:
     *       const std::string &Name;
     * RETURNS: None.
     */
    VOID DeleteRoom( const std::string &Name );

    /* Update room function.
     * ARGUMENTS:
     *   - name of room:
     *       const std::string &Name;
     * RETURNS:
     *   (BOOL) TRUE if room was updated successfully, FALSE otherwise.
     */
    BOOL UpdateRoom( const std::string &Name );

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
    BOOL UpdateRoomMtlBuf( const std::string &RoomName, vec3 NewColor, FLT Scale );

    /* Update room material buffer function.
     * ARGUMENTS:
     *   - name of room:
     *       const std::string &RoomName;
     *   - walls textures:
     *       const std::vector<std::string> &TexNames;
     * RETURNS:
     *   (BOOL) TRUE if room was updated successfully, FALSE otherwise.
     */
    BOOL UpdateRoomTexs( const std::string &RoomName, const std::vector<std::string> &TexNames );

    /* Update room walls (because of texture coordinates) function.
     * ARGUMENTS:
     *   - name of room:
     *       const std::string &RoomName;
     *   - texture coordinates scaling coefficients:
     *       vec2 NewRoomTexCoordTransformed;
     * RETURNS: None.
     */
    VOID UpdateRoomWalls( const std::string &RoomName, vec2 NewRoomTexCoordTransformed );

    /* Find room walls indices function.
     * ARGUMENTS:
     *   - name of room:
     *       const std::string &Name;
     * RETURNS: None.
     */
    VOID FindWallsIndices( const std::string &Name );

    /* Add furniture to room function.
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
    VOID AddFurnitureToRoom( const std::string &RoomName, const furniture &Furniture, vec3 Translation = vec3(0), FLT Rotation = 0 );

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
    VOID DeleteFurnitureFromRoom( const std::string &RoomName, const std::string &FurnitureId, INT FurnitureNo = 0 );

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
    VOID UpdateRoomFurniture( const std::string &RoomName, const std::string &FurnitureName, INT FurnitureNo = 0,
                              vec3 NewTranslation = vec3(0), FLT NewRotation = 0 );

    /* Auto-arrange room furniture function.
     * ARGUMENTS:
     *   - room name:
     *       const std::string &RoomName;
     *   - optional room type override:
     *       const std::string &RoomType;
     *   - optional output debug log file:
     *       const std::string &OutputFileName;
     * RETURNS:
     *   (BOOL) arrange result.
     */
    BOOL AutoArrangeRoom( const std::string &RoomName, const std::string &RoomType = "",
                          const std::string &OutputFileName = "" );

    /* Draw room filling (furniture, floor) function.
     * ARGUMENTS:
     *   - room name:
     *       const std::string &RoomName;
     * RETURNS: None.
     */
    VOID DrawRoomFilling( const std::string &RoomName );

    /* Get room by point on plane function.
     * ARGUMENTS:
     *   - point position on plane:
     *       const vec2 &SearchPoint;
     * RETURNS:
     *   (std::string) if room was found - it's name, "" otherwise.
     */
    std::string GetRoomByPoint( const vec2 &SearchPoint );

    /* Build ordered room polygon function.
     * ARGUMENTS:
     *   - room name:
     *       const std::string &RoomName;
     *   - resulting room polygon:
     *       std::vector<vec2> &Polygon;
     * RETURNS:
     *   (BOOL) build result.
     */
    BOOL GetRoomPolygon( const std::string &RoomName, std::vector<vec2> &Polygon ) const;

    /* Draw flat function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID DrawFlat( VOID );

    /* Draw flat using markers for debug function.
     * ARGUMENTS:
     *   - transform matrix:
     *       const matr &Transform;
     * RETURNS: None.
     */
    VOID DrawFlatMarkers( const matr &Transform );

    /* Color all connected segments depending on the angle function.
     * ARGUMENTS:
     *   - index of the segment to apply function to:
     *       INT SegmentInd;
     * RETURNS: None.
     */
    VOID ColorSegmentAngles( INT SegmentInd );

    /* Save flat information into file function.
     * ARGUMENTS:
     *   - name of file to save into:
     *       std::string FileName;
     * RETURNS:
     *   (BOOL) TRUE if flat was successfully saved, FALSE otherwise.
     */
    BOOL SaveToFile( std::string FileName );

    /* Load flat information from file function.
     * ARGUMENTS:
     *   - name of file to load from:
     *       std::string FileName;
     * RETURNS:
     *   (BOOL) TRUE if flat was successfully loaded, FALSE otherwise.
     */
    BOOL LoadFromFile( std::string FileName );
  }; /* End of 'flat' class */
} /* end of 'tam' namespace */

#endif /* __tam_flat_h_ */

/* END OF 'tam_flat.h' FILE */
