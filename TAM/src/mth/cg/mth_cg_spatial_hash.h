/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_cg_spatial_hash.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math support module.
 *               Computation geometry: uniform grid broad-phase hash.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 09.04.2026
 * NOTE        : Namespace 'mth::cg'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_cg_spatial_hash_h_
#define __mth_cg_spatial_hash_h_

#include <algorithm>
#include <cstddef>
#include <functional>
#include <unordered_map>
#include <vector>

#include "mth_cg_aabb.h"

/* 'mth' namespace */
namespace mth
{
  /* Computation geometry: collision tests and acceleration structures */
  namespace cg
  {
    /* Uniform grid hashing: cell keys and index conversion */
    namespace detail
    {
      /* Triple of grid indices identifying one hash bucket */
      struct cell_key
      {
        INT X, Y, Z; /* Cell index by axes */

        /* Compare two cell coordinates.
         * ARGUMENTS:
         *   - other cell:
         *       const cell_key &O;
         * RETURNS:
         *   (BOOL) TRUE if all three indices match.
         */
        BOOL operator==( const cell_key &O ) const
        {
          return X == O.X && Y == O.Y && Z == O.Z;
        } /* End of 'operator==' function */
      }; /* End of 'cell_key' struct */

      /* Hash functor for unordered_map keyed by cell_key */
      struct cell_key_hash
      {
        /* Hash a cell into size_t for map buckets.
         * ARGUMENTS:
         *   - cell:
         *       const cell_key &K;
         * RETURNS:
         *   (std::size_t) combined hash value.
         */
        std::size_t operator()( const cell_key &K ) const noexcept
        {
          std::size_t H = std::hash<INT>{}(K.X);

          H ^= std::hash<INT>{}(K.Y) + 0x9e3779b9 + (H << 6) + (H >> 2);
          H ^= std::hash<INT>{}(K.Z) + 0x9e3779b9 + (H << 6) + (H >> 2);
          return H;
        } /* End of 'operator()' function */
      }; /* End of 'cell_key_hash' struct */

      /* Map scalar coordinate to integer cell index using floor (negative-safe).
       * ARGUMENTS:
       *   - world coordinate on one axis:
       *       type Value;
       *   - cell edge length:
       *       type CellSize;
       * RETURNS:
       *   (INT) cell index along that axis.
       */
      template<typename type>
        inline INT CellIndex( type Value, type CellSize )
        {
          DBL D = static_cast<DBL>(Value) / static_cast<DBL>(CellSize);

          return static_cast<INT>(std::floor(D));
        } /* End of 'CellIndex' function */
    } /* end of 'detail' namespace */

    /* Uniform grid: each object id is stored in every overlapped cell */
    template<typename type>
      class spatial_hash
      {
      private:
        /* Edge length of one cubic cell */
        type CellSize;
        /* Map from cell to object ids touching that cell */
        std::unordered_map<detail::cell_key, std::vector<INT>, detail::cell_key_hash> Grid;

      public:
        /* Create hash with given cell edge length.
         * ARGUMENTS:
         *   - world-space cell size (must be positive in use):
         *       type CellSize;
         * RETURNS: None.
         */
        explicit spatial_hash( type CellSize = static_cast<type>(2) ) :
          CellSize(CellSize)
        {
        } /* End of 'spatial_hash' function */

        /* Remove all cell buckets.
         * ARGUMENTS: None.
         * RETURNS: None.
         */
        VOID Clear( VOID )
        {
          Grid.clear();
        } /* End of 'Clear' function */

        /* Register an object in every cell touched by Bounds.
         * ARGUMENTS:
         *   - arbitrary object id:
         *       INT ObjectId;
         *   - axis-aligned query volume in world space:
         *       const aabb<type> &Bounds;
         * RETURNS: None.
         */
        VOID Insert( INT ObjectId, const aabb<type> &Bounds )
        {
          if (!Bounds.IsValid())
            return;
          INT
            MinX = detail::CellIndex(Bounds.Min.X, CellSize),
            MinY = detail::CellIndex(Bounds.Min.Y, CellSize),
            MinZ = detail::CellIndex(Bounds.Min.Z, CellSize),
            MaxX = detail::CellIndex(Bounds.Max.X, CellSize),
            MaxY = detail::CellIndex(Bounds.Max.Y, CellSize),
            MaxZ = detail::CellIndex(Bounds.Max.Z, CellSize);
          INT cx, cy, cz;

          for (cx = MinX; cx <= MaxX; cx++)
            for (cy = MinY; cy <= MaxY; cy++)
              for (cz = MinZ; cz <= MaxZ; cz++)
                Grid[detail::cell_key{cx, cy, cz}].push_back(ObjectId);
        } /* End of 'Insert' function */

        /* Collect distinct ids from cells overlapping Bounds.
         * ARGUMENTS:
         *   - query volume in world space:
         *       const aabb<type> &Bounds;
         * RETURNS:
         *   (std::vector<INT>) sorted unique candidate ids.
         */
        std::vector<INT> Query( const aabb<type> &Bounds ) const
        {
          std::vector<INT> Result;

          if (!Bounds.IsValid())
            return Result;
          INT
            MinX = detail::CellIndex(Bounds.Min.X, CellSize),
            MinY = detail::CellIndex(Bounds.Min.Y, CellSize),
            MinZ = detail::CellIndex(Bounds.Min.Z, CellSize),
            MaxX = detail::CellIndex(Bounds.Max.X, CellSize),
            MaxY = detail::CellIndex(Bounds.Max.Y, CellSize),
            MaxZ = detail::CellIndex(Bounds.Max.Z, CellSize);
          INT cx, cy, cz;

          for (cx = MinX; cx <= MaxX; cx++)
            for (cy = MinY; cy <= MaxY; cy++)
              for (cz = MinZ; cz <= MaxZ; cz++)
              {
                auto It = Grid.find(detail::cell_key{cx, cy, cz});

                if (It != Grid.end())
                  Result.insert(Result.end(), It->second.begin(), It->second.end());
              }

          std::sort(Result.begin(), Result.end());
          Result.erase(std::unique(Result.begin(), Result.end()), Result.end());
          return Result;
        } /* End of 'Query' function */
      }; /* End of 'spatial_hash' class */
  } /* end of 'cg' namespace */
} /* end of 'mth' namespace */

#endif /* __mth_cg_spatial_hash_h_ */

/* END OF 'mth_cg_spatial_hash.h' FILE */
