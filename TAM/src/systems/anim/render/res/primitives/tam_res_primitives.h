/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_primitives.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Resources module.
 *               Primitives declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 07.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_res_primitives_h_
#define __tam_res_primitives_h_

/* Project namespace */
namespace tam
{
  /* Vertex type enum class */
  enum class vertex_type
  {
    eINT,
    eFLOAT,
    eVEC2,
    eVEC3,
    eVEC4,
    eMATR,
  }; /* End of 'vertex_type' class */

  /* Vertex class */
  class vertex_std
  {
  public:
    vec3 P; // Vertex position
    vec2 T; // Texture coordinates
    vec3 N; // Normal
    vec4 C; // Color

    /* Class constructor.
      * ARGUMENTS: None.
      */
    vertex_std( VOID ) // : P(vec3(1)), T(vec2(1)), N(vec3(0, 1, 0)), C(vec4(1))
    {
    } /* End of 'vertex_std' function */

    /* Class constructor.
      * ARGUMENTS:
      *   - vertex position:
      *       const vec3 &NewP;
      *   - vertex texture coordinates:
      *       const vec2 &NewT;
      *   - vertex normal:
      *       const vec3 &NewN;
      *   - vertex color:
      *       const vec4 &NewC;
      */
    vertex_std( const vec3 &NewP, const vec2 &NewT, const vec3 &NewN, const vec4 &NewC ) : P(NewP), T(NewT), N(NewN), C(NewC)
    {
    } /* End of 'vertex_std' function */
  }; /* End of 'vertex_std' class */

  /// /* Primitive data buffer structure */
  /// struct BUF_PRIM
  /// {
  ///   matr::matr_data MatrWVP;  // World * View * Projection matrix
  ///   matr::matr_data MatrW;    // World matrix
  ///   matr::matr_data MatrWInv; // World inverse matrix
  /// }; /* End of 'BUF_PRIM' structure */

  /* 'prim_res' handle class */
  class prim_res : public resource
  {
  private:
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
    VOID CreateData( const VOID *DataV, INT VertexSize, UINT NumOfV, const VOID *DataInd, UINT NumOfInd );
  public:
    buffer
      VertexBuf {}, // Vertex buffer
      IndexBuf {};  // Index buffer
    INT NumOfElements {}; // Number of elements
    matr Transform {}; // Primitive transformation matrix

    INT Id; // Primitive index
    material Mtl {}; // Material pointer
    vec3
      MinBB {0}, // Minimal primitive position
      MaxBB {0}; // Maximal primitive position

    INT PrimitiveObjectId {};                       // Primitive Id (in common case duplicate from root model)

    INT
      NumOfInstances {},                            // Number of instances of primitive
      PreviousFrameNumOfInstances {};               // Number of instances on previous frame
    std::vector<matr::matr_data> WorldMatrixes {};  // Buffer that contains matrices of every instance of this primitive
    INT WorldMatrixesSize {};                       // Current matrix array size
    INT FirstMatrixIndex {};                        // Index of first world matrix of this primitive in global matrix array
    UINT64 StoreFrameNo {};                         // Primitive store frame number
    prim NextDraw;                                  // Pointer to next-to-draw primitive

    /* Primitive data buffer structure */
    struct BUF_PRIM
    {
      matr::matr_data MatrWVP;       // World * View * Projection matrix
      matr::matr_data MatrW;         // World matrix
      matr::matr_data MatrWInvTrans; // World inversed and transposed matrix
      vec4 IsTangentIsWireframe;     // Tangent, wireframe flags
    }; /* End of 'BUF_PRIM' structure */

    /* Class constructor.
     * ARGUMENTS:
     *   - pointer to the render context:
     *       render *RndPtr;
     */
    prim_res( render *RndPtr ) : resource(RndPtr)
    {
    } /* End of 'prim_res' function */

    /* Class move constructor.
     * ARGUMENTS:
     *   - rvalue reference to prim_res:
     *       prim_res &&Old;
     */
    prim_res( prim_res &&Old );

    /* Class destructor */
    ~prim_res( VOID )
    {
      Free();
    } /* End of '~prim_res' function */

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
    prim_res & Create( material NewMtl,
                       const VOID *DataV, INT VertexSize, INT NumOfV,
                       const std::span<INT> &Ind );

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
    prim_res & Create( material NewMtl,
                       const VOID *DataV, std::vector<vertex_type> VertexAttributes, INT NumOfV,
                       const std::span<INT> &Ind );

    /* Primitive creation function.
     * ARGUMENTS:
     *   - primitive's material:
     *       material NewMtl;
     *   - vertex array:
     *       const std::span<vertex> &V;
     *   - index array:
     *       const std::span<INT> &Ind;
     * RETURNS:
     *   (prim_res &) self reference.
     */
     template <class vertex_type>
       prim_res & Create( material NewMtl,
                          const std::span<vertex_type> &V,
                          const std::span<INT> &Ind )
       {
         Free();
         if (NewMtl == nullptr)
           return *this;
         Mtl = NewMtl;
         Transform = matr::Identity();
         NumOfElements = 0;

         if (V.size() != 0)
         {
           // Collect min-max info
           if constexpr (requires{vertex_type::P;})
           {
             MinBB = MaxBB = V[0].P;
             for (auto vrt : V)
               MinBB = vrt.P.Min(MinBB), MaxBB = vrt.P.Max(MaxBB);
           }
         }

         CreateData(V.data(), sizeof(vertex_type), (UINT)V.size(), Ind.data(), (UINT)Ind.size());
         return *this;
       } /* End of 'Create' function */

    /* Free primitive function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );
  }; /* End of 'prim_res' class */

  /* Primitive interface type */
  using prim = std::shared_ptr<prim_res>;

  /* 'primitive_manager' handle class */
  class primitive_manager : public resource_manager<prim_res>
  {
  public:
    /* Class constructor.
     * ARGUMENTS:
     *   - render instance reference:
     *       render &NewRnd;
     * RETURNS: None.
     */
    primitive_manager( render &NewRnd ) : resource_manager(NewRnd)
    {
    } /* End of 'primitive_manager' function */

    /* Class destructor */
    ~primitive_manager( VOID )
    {
      logger::info << "Primitives deinitialization";
    } /* End of '~primitive_manager' function */

    /* Primitive creation function.
     * ARGUMENTS:
     *   - primitive's material:
     *       material NewMtl;
     *   - primitive type:
     *       prim_type NewType;
     *   - vertex array:
     *       const std::span<vertex_type> &V;
     *   - index array:
     *       const std::span<INT> &Ind;
     * RETURNS:
     *   (prim) created primitive interface.
     */
     template <class vertex_type>
       prim PrimitiveCreate( material NewMtl,
                             const std::span<vertex_type> &V,
                             const std::span<INT> &Ind )
       {
         prim Prim = Allocate();

         Prim->Create<vertex_type>(NewMtl, V, Ind);
         return Prim;
       } /* End of 'PrimitiveCreate' function */

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
     *   (prim) created primitive interface.
     */
    prim PrimitiveCreate( material NewMtl,
                          const VOID *DataV, INT VertexSize, INT NumOfV,
                          const std::span<INT> &Ind )
    {
      prim Prim = Allocate();

      Prim->Create(NewMtl, DataV, VertexSize, NumOfV, Ind);
      return Prim;
    } /* End of 'PrimitiveCreate' function */

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
     *   (prim) created primitive interface.
     */
    prim PrimitiveCreate( material NewMtl,
                          const VOID *DataV, std::vector<vertex_type> VertexAttributes, INT NumOfV,
                          const std::span<INT> &Ind )
    {
      prim Prim = Allocate();

      Prim->Create(NewMtl, DataV, VertexAttributes, NumOfV, Ind);
      return Prim;
    } /* End of 'PrimitiveCreate' function */

    /* Delete primitive function.
     * ARGUMENTS:
     *   - primitive to free:
     *       prim &Prim;
     * RETURNS: None.
     */
    VOID PrimitiveDelete( prim &Prim )
    {
      Delete(Prim);
    } /* End of 'PrimitiveDelete' function */
  }; /* End of 'primitive_manager' class */
} /* end of 'tam' namespace */

#endif /* __tam_res_primitives_h_ */

/* END OF 'tam_res_primitives.h' FILE */
