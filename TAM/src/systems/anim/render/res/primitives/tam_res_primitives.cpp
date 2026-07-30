/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_primitives.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Resources module.
 *               Primitives implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 07.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Class move constructor.
 * ARGUMENTS:
 *   - rvalue reference to prim_res:
 *       prim_res &&Old;
 */
tam::prim_res::prim_res( prim_res &&Old ) : resource(Old.Rnd), NumOfElements(Old.NumOfElements),
  Transform(Old.Transform), Id(Old.Id), MinBB(Old.MinBB), MaxBB(Old.MaxBB),
  VertexBuf(Old.VertexBuf), IndexBuf(Old.IndexBuf), Mtl(Old.Mtl),
  NumOfInstances(Old.NumOfInstances), WorldMatrixes(Old.WorldMatrixes), WorldMatrixesSize(Old.WorldMatrixesSize),
  FirstMatrixIndex(Old.FirstMatrixIndex), StoreFrameNo(Old.StoreFrameNo), NextDraw(Old.NextDraw),
  PreviousFrameNumOfInstances(Old.PreviousFrameNumOfInstances)
{
  Rnd = Old.Rnd;
  Old.Rnd = nullptr;
  Old.VertexBuf = nullptr;
  Old.IndexBuf = nullptr;
  Old.Mtl = nullptr;
  Old.NumOfInstances = 0;
  Old.PreviousFrameNumOfInstances = 0;
  Old.WorldMatrixes = {};
  Old.WorldMatrixesSize = 0;
  Old.FirstMatrixIndex = 0;
  Old.StoreFrameNo =  0;
  Old.NextDraw = nullptr;
} /* End of 'tam::prim_res::prim_res' function */

/* Primitive data creation function.
 * ARGUMENTS:
 *   - vertex data:
 *       const VOID *DataV;
 *   - number of vertices:
 *       INT NumOfV;
 *   - index data:
 *       const VOID *DataInd;
 *   - number of indices:
 *       INT NumOfInd;
 * RETURNS: None.
 */
VOID tam::prim_res::CreateData( const VOID *DataV, INT VertexSize, UINT NumOfV, const VOID *DataInd, UINT NumOfInd )
{
  if (NumOfV <= 0 || DataV == nullptr)
    return;
  NumOfElements = NumOfV;
  VertexBuf = Rnd->BufferCreate(VertexSize * NumOfV, (VOID *)DataV,
  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_SHARING_MODE_EXCLUSIVE);

  if (NumOfInd > 0)
  {
    IndexBuf = Rnd->BufferCreate(sizeof(UINT) * NumOfInd, (VOID *)DataInd,
      VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_SHARING_MODE_EXCLUSIVE);
    NumOfElements = NumOfInd;
  }
} /* End of 'tam::prim_res::CreateData' function */

/* Primitive creation function.
 * ARGUMENTS:
 *   - material to created primitive with:
 *       material NewMtl;
 *   - vertex data:
 *       const VOID *DataV;
 *   - vertex size:
 *       INT VertexSize;
 *   - number of vertices:
 *       INT NumOfV;
 *   - vertex array:
 *       const std::span<vertex_type> &V;
 *   - index array:
 *       const std::span<INT> &Ind;
 * RETURNS:
 *   (prim_res &) self reference.
 */
tam::prim_res & tam::prim_res::Create( material NewMtl, const VOID *DataV, INT VertexSize, INT NumOfV,
                                       const std::span<INT> &Ind )
{
  Free();
  if (NewMtl == nullptr)
  {
    tam::logger::error << "Unable to create primitive without material.";
    return *this;
  }
  Mtl = NewMtl;
  Transform = matr::Identity();
  CreateData(DataV, VertexSize, NumOfV, Ind.data(), (UINT)Ind.size());
  return *this;
} /* End of 'tam::prim_res::Create' function */

/* Primitive creation function.
 * ARGUMENTS:
 *   - material to created primitive with:
 *       material NewMtl;
 *   - vertex data:
 *       const VOID *DataV;
 *   - vector of vertex attributes:
 *       std::vector<vertex_type> VertexAttributes;
 *   - number of vertices:
 *       INT NumOfV;
 *   - vertex array:
 *       const std::span<vertex_type> &V;
 *   - index array:
 *       const std::span<INT> &Ind;
 * RETURNS:
 *   (prim_res &) self reference.
 */
tam::prim_res & tam::prim_res::Create( material NewMtl, const VOID *DataV, std::vector<vertex_type> VertexAttributes,
  INT NumOfV, const std::span<INT> &Ind )
{
  Free();
  if (NewMtl == nullptr)
  {
    tam::logger::error << "Unable to create primitive without material.";
    return *this;
  }
  Mtl = NewMtl;
  Transform = matr::Identity();
  UINT Size = 0;
  for (INT i = 0; i < VertexAttributes.size(); i++)
    Size += VertexAttributes[i] == vertex_type::eVEC2 ? 8 :
      VertexAttributes[i] == vertex_type::eVEC3 ? 12 : 16;
  CreateData(DataV, Size, NumOfV, Ind.data(), (UINT)Ind.size());
  return *this;
} /* End of 'tam::prim_res::Create' function */

/* Free primitive function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::prim_res::Free( VOID )
{
  NumOfElements = 0;
  NumOfInstances = 0;
  WorldMatrixesSize = 0;
  FirstMatrixIndex = 0;
  StoreFrameNo =  0;
  WorldMatrixes.clear();
  WorldMatrixes.shrink_to_fit();
  NextDraw = nullptr;
  Mtl = nullptr;
} /* End of 'tam::prim_res::Free' function */

/* END OF 'tam_res_primitives.cpp' FILE */
