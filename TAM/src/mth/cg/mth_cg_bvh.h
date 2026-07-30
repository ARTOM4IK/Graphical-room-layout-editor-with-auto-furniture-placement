/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_cg_bvh.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math support module.
 *               Computation geometry: bounding volume hierarchy for triangles.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 10.04.2026
 * NOTE        : Namespace 'mth::cg'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_cg_bvh_h_
#define __mth_cg_bvh_h_

#include <limits>
#include <memory>
#include <vector>

#include "mth_cg_aabb.h"
#include "mth_cg_mesh.h"
#include "mth_cg_triangles.h"

/* 'mth' namespace */
namespace mth
{
  /* Computation geometry: collision tests and acceleration structures */
  namespace cg
  {
    /* One node: either a leaf with triangles or an internal split */
    template<typename type>
      struct bvh_node
      {
        /* Bounds of all triangles in this subtree (mesh space) */
        aabb<type> Bounds;
        /* Triangle indices stored only at leaves */
        std::vector<INT> TriangleIndices;
        /* Left/right child subtrees (empty at leaves) */
        std::unique_ptr<bvh_node> Left, Right;
        /* TRUE if TriangleIndices is authoritative; FALSE if children are used */
        BOOL IsLeaf;

        /* Class default constructor */
        bvh_node( VOID ) : IsLeaf(TRUE)
        {
        } /* End of 'bvh_node' function */
      }; /* End of 'bvh_node' struct */

    /* Hierarchy over a mesh; does not copy vertex data (pointer to source mesh) */
    template<typename type>
      class bvh
      {
      private:
        /* Non-owning pointer to geometry used for bounds and leaf tests */
        const mesh<type> *MeshPtr;
        /* Root of the binary tree (nullptr if mesh had no triangles) */
        std::unique_ptr<bvh_node<type>> Root;

        /* Tight AABB around all triangles listed in Indices function.
         * ARGUMENTS:
         *   - triangle indices into *MeshPtr:
         *       const std::vector<INT> &Indices;
         * RETURNS:
         *   (aabb<type>) union of triangle bounds (possibly invalid if empty).
         */
        aabb<type> ComputeBounds( const std::vector<INT> &Indices ) const
        {
          aabb<type> B;
          type
            Inf = std::numeric_limits<type>::max(),
            NInf = std::numeric_limits<type>::lowest();

          B.Min = vec3<type>(Inf);
          B.Max = vec3<type>(NInf);

          for (INT Idx : Indices)
          {
            if (Idx < 0 || Idx >= MeshPtr->TriangleCount())
              continue;
            const triangle &Tri = MeshPtr->Triangles[Idx];

            if (Tri.V0 >= 0 && Tri.V0 < MeshPtr->VertexCount())
              B.Expand(MeshPtr->Vertices[Tri.V0]);
            if (Tri.V1 >= 0 && Tri.V1 < MeshPtr->VertexCount())
              B.Expand(MeshPtr->Vertices[Tri.V1]);
            if (Tri.V2 >= 0 && Tri.V2 < MeshPtr->VertexCount())
              B.Expand(MeshPtr->Vertices[Tri.V2]);
          }
          return B;
        } /* End of 'ComputeBounds' function */

        /* Recursively partition triangle indices until leaves hold at most eight function.
         * ARGUMENTS:
         *   - working index set (modified only by callees via value splits):
         *       std::vector<INT> &Indices;
         * RETURNS:
         *   (std::unique_ptr<bvh_node<type>>) subtree root.
         */
        std::unique_ptr<bvh_node<type>> BuildRecursive( std::vector<INT> &Indices )
        {
          std::unique_ptr<bvh_node<type>> Node = std::make_unique<bvh_node<type>>();

          Node->TriangleIndices = Indices;
          Node->Bounds = ComputeBounds(Indices);
          INT Count = (INT)Indices.size();

          if (Count > 8)
          {
            INT Mid = Count / 2;

            Node->IsLeaf = FALSE;
            std::vector<INT> L(Indices.begin(), Indices.begin() + Mid);
            std::vector<INT> R(Indices.begin() + Mid, Indices.end());

            Node->TriangleIndices.clear();
            Node->Left = BuildRecursive(L);
            Node->Right = BuildRecursive(R);
          }
          return Node;
        } /* End of 'BuildRecursive' function */

        /* Depth-first overlap: descend while AABBs overlap, test leaves pairwise function.
         * ARGUMENTS:
         *   - first subtree node:
         *       const bvh_node<type> *NodeA;
         *   - second subtree node:
         *       const bvh_node<type> *NodeB;
         *   - owner of NodeB (for mesh pointer):
         *       const bvh &OtherBVH;
         * RETURNS:
         *   (BOOL) TRUE if any triangle pair intersects.
         */
        BOOL IntersectRecursive( const bvh_node<type> *NodeA, const bvh_node<type> *NodeB,
                                 const bvh &OtherBVH ) const
        {
          if (NodeA == nullptr || NodeB == nullptr)
            return FALSE;
          if (!NodeA->Bounds.Intersects(NodeB->Bounds))
            return FALSE;

          if (NodeA->IsLeaf && NodeB->IsLeaf)
          {
            /* Exhaustive triangle pairs between two leaves */
            for (INT Ia : NodeA->TriangleIndices)
            {
              if (Ia < 0 || Ia >= MeshPtr->TriangleCount())
                continue;
              const triangle &TriA = MeshPtr->Triangles[Ia];
              const vec3<type> &A0 = MeshPtr->Vertices[TriA.V0];
              const vec3<type> &A1 = MeshPtr->Vertices[TriA.V1];
              const vec3<type> &A2 = MeshPtr->Vertices[TriA.V2];

              for (INT Ib : NodeB->TriangleIndices)
              {
                if (Ib < 0 || Ib >= OtherBVH.MeshPtr->TriangleCount())
                  continue;
                const triangle &TriB = OtherBVH.MeshPtr->Triangles[Ib];
                const vec3<type> &B0 = OtherBVH.MeshPtr->Vertices[TriB.V0];
                const vec3<type> &B1 = OtherBVH.MeshPtr->Vertices[TriB.V1];
                const vec3<type> &B2 = OtherBVH.MeshPtr->Vertices[TriB.V2];

                if (TrianglesIntersect(A0, A1, A2, B0, B1, B2))
                  return TRUE;
              }
            }
            return FALSE;
          }

          if (NodeA->IsLeaf)
            return IntersectRecursive(NodeA, NodeB->Left.get(), OtherBVH) ||
                   IntersectRecursive(NodeA, NodeB->Right.get(), OtherBVH);

          if (NodeB->IsLeaf)
            return IntersectRecursive(NodeA->Left.get(), NodeB, OtherBVH) ||
                   IntersectRecursive(NodeA->Right.get(), NodeB, OtherBVH);

          return IntersectRecursive(NodeA->Left.get(), NodeB->Left.get(), OtherBVH) ||
                 IntersectRecursive(NodeA->Left.get(), NodeB->Right.get(), OtherBVH) ||
                 IntersectRecursive(NodeA->Right.get(), NodeB->Left.get(), OtherBVH) ||
                 IntersectRecursive(NodeA->Right.get(), NodeB->Right.get(), OtherBVH);
        } /* End of 'IntersectRecursive' function */

        /* World-space AABB overlap for model-space boxes plus instance translations function.
         * ARGUMENTS:
         *   - first box and translate:
         *       const aabb<type> &BoxA, const vec3<type> &TransA;
         *   - second box and translate:
         *       const aabb<type> &BoxB, const vec3<type> &TransB;
         * RETURNS:
         *   (BOOL) TRUE if intersects.
         */
        static BOOL BoundsOverlapShifted( const aabb<type> &BoxA, const vec3<type> &TransA,
                                          const aabb<type> &BoxB, const vec3<type> &TransB )
        {
          aabb<type> Wa, Wb;

          Wa.Min = BoxA.Min + TransA;
          Wa.Max = BoxA.Max + TransA;
          Wb.Min = BoxB.Min + TransB;
          Wb.Max = BoxB.Max + TransB;
          return Wa.Intersects(Wb);
        } /* End of 'BoundsOverlapShifted' function */

        /* Same as IntersectRecursive but with per-mesh world translations on bounds and triangles function.
         * ARGUMENTS:
         *   - first box and translate:
         *       const aabb<type> &BoxA, const vec3<type> &TransA;
         *   - second box and translate:
         *       const aabb<type> &BoxB, const vec3<type> &TransB;
         *   - other BVH:
         *       const bvh &OtherBVH;
         * RETURNS:
         *   (BOOL) TRUE if intersects.
         */
        BOOL IntersectRecursiveTranslated( const bvh_node<type> *NodeA, const vec3<type> &TransA,
                                           const bvh_node<type> *NodeB, const vec3<type> &TransB,
                                           const bvh &OtherBVH ) const
        {
          if (NodeA == nullptr || NodeB == nullptr)
            return FALSE;
          if (!BoundsOverlapShifted(NodeA->Bounds, TransA, NodeB->Bounds, TransB))
            return FALSE;

          if (NodeA->IsLeaf && NodeB->IsLeaf)
          {
            for (INT Ia : NodeA->TriangleIndices)
            {
              if (Ia < 0 || Ia >= MeshPtr->TriangleCount())
                continue;
              const triangle &TriA = MeshPtr->Triangles[Ia];
              vec3<type>
                A0 = MeshPtr->Vertices[TriA.V0] + TransA,
                A1 = MeshPtr->Vertices[TriA.V1] + TransA,
                A2 = MeshPtr->Vertices[TriA.V2] + TransA;

              for (INT Ib : NodeB->TriangleIndices)
              {
                if (Ib < 0 || Ib >= OtherBVH.MeshPtr->TriangleCount())
                  continue;
                const triangle &TriB = OtherBVH.MeshPtr->Triangles[Ib];
                vec3<type>
                  B0 = OtherBVH.MeshPtr->Vertices[TriB.V0] + TransB,
                  B1 = OtherBVH.MeshPtr->Vertices[TriB.V1] + TransB,
                  B2 = OtherBVH.MeshPtr->Vertices[TriB.V2] + TransB;

                if (TrianglesIntersect(A0, A1, A2, B0, B1, B2))
                  return TRUE;
              }
            }
            return FALSE;
          }

          if (NodeA->IsLeaf)
            return IntersectRecursiveTranslated(NodeA, TransA, NodeB->Left.get(), TransB, OtherBVH) ||
                   IntersectRecursiveTranslated(NodeA, TransA, NodeB->Right.get(), TransB, OtherBVH);

          if (NodeB->IsLeaf)
            return IntersectRecursiveTranslated(NodeA->Left.get(), TransA, NodeB, TransB, OtherBVH) ||
                   IntersectRecursiveTranslated(NodeA->Right.get(), TransA, NodeB, TransB, OtherBVH);

          return IntersectRecursiveTranslated(NodeA->Left.get(), TransA, NodeB->Left.get(), TransB, OtherBVH) ||
                 IntersectRecursiveTranslated(NodeA->Left.get(), TransA, NodeB->Right.get(), TransB, OtherBVH) ||
                 IntersectRecursiveTranslated(NodeA->Right.get(), TransA, NodeB->Left.get(), TransB, OtherBVH) ||
                 IntersectRecursiveTranslated(NodeA->Right.get(), TransA, NodeB->Right.get(), TransB, OtherBVH);
        } /* End of 'IntersectRecursiveTranslated' function */

      public:
        /* Build a binary BVH over all triangles of M class constructor.
         * ARGUMENTS:
         *   - source mesh (lifetime must exceed this object):
         *       const mesh<type> &M;
         * RETURNS: None.
         */
        explicit bvh( const mesh<type> &M ) : MeshPtr(&M)
        {
          INT N = M.TriangleCount();

          if (N <= 0)
          {
            Root = nullptr;
            return;
          }
          std::vector<INT> All(N);

          for (INT i = 0; i < N; i++)
            All[i] = i;
          Root = BuildRecursive(All);
        } /* End of 'bvh' function */

        /* Copying constructor would duplicate unique tree ownership; disabled.
         * ARGUMENTS:
         *   - source (unused):
         *       const bvh &;
         * RETURNS: None (deleted).
         */
        bvh( const bvh & ) = delete;

        /* Copy assignment function disabled.
         * ARGUMENTS:
         *   - source (unused):
         *       const bvh &;
         * RETURNS: None (deleted).
         */
        bvh & operator=( const bvh & ) = delete;

        /* Move constructor leaves source empty and transfers the tree.
         * ARGUMENTS:
         *   - donor tree:
         *       bvh &&;
         * RETURNS: None.
         */
        bvh( bvh && ) noexcept = default;

        /* Move assignment transfers tree ownership from donor function.
         * ARGUMENTS:
         *   - donor tree:
         *       bvh &&;
         * RETURNS:
         *   (bvh &) reference to this tree.
         */
        bvh & operator=( bvh && ) noexcept = default;

        /* Bounding box of the root in mesh space function.
         * ARGUMENTS: None.
         * RETURNS:
         *   (aabb<type>) invalid box if the tree is empty.
         */
        aabb<type> RootBounds( VOID ) const
        {
          if (Root != nullptr)
            return Root->Bounds;
          aabb<type> B;

          B.Min = vec3<type>(static_cast<type>(1));
          B.Max = vec3<type>(static_cast<type>(0));
          return B;
        } /* End of 'RootBounds' function */

        /* Whether any triangle pair between two hierarchies intersects (mesh space) function.
         * ARGUMENTS:
         *   - other tree built over its own mesh:
         *       const bvh &Other;
         * RETURNS:
         *   (BOOL) TRUE if a triangle intersection exists.
         */
        BOOL Intersects( const bvh &Other ) const
        {
          if (Root == nullptr || Other.Root == nullptr)
            return FALSE;
          return IntersectRecursive(Root.get(), Other.Root.get(), Other);
        } /* End of 'Intersects' function */

        /* Triangle intersection with world translations (BVH narrow phase, not O(n*m) mesh brute force).
         * ARGUMENTS:
         *   - other hierarchy:
         *       const bvh &Other;
         *   - this mesh origin in world space:
         *       const vec3<type> &TransSelf;
         *   - other mesh origin in world space:
         *       const vec3<type> &TransOther;
         * RETURNS:
         *   (BOOL) TRUE if any translated triangle pair intersects.
         */
        BOOL IntersectsTranslated( const bvh &Other, const vec3<type> &TransSelf,
                                 const vec3<type> &TransOther ) const
        {
          if (Root == nullptr || Other.Root == nullptr)
            return FALSE;
          return IntersectRecursiveTranslated(Root.get(), TransSelf, Other.Root.get(), TransOther, Other);
        } /* End of 'IntersectsTranslated' function */
      }; /* End of 'bvh' class */
  } /* end of 'cg' namespace */
} /* end of 'mth' namespace */

#endif /* __mth_cg_bvh_h_ */

/* END OF 'mth_cg_bvh.h' FILE */
