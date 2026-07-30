/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_models.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Resources module.
 *               Models declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 22.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_res_models_h_
#define __tam_res_models_h_

/* Project namespace */
namespace tam
{
  /* 'model' handle class */
  class model_res : public resource
  {
  public:
    std::string Name {};                                         // Model name
    std::string ModelType {};                                    // Model type : G3DM or GLTF
    matr Transform {};                                           // Model transformation matrix
    gltf GLTFData {};                                            // Data obtained from (.GLTF) model
    vec3
      MinBB {0},                                                 // Min bound box vector
      MaxBB {0};                                                 // Max bound box vector
    mesh Geometry {};                                            // Whole geometry of model
    std::vector<prim> Prims {};                                  // Stock of primitives
    material_pattern MtlPat {}, OpaqueMtlPat {}, TransMtlPat {}; // Model material pattern
    buffer InvJointMatrBuf {}, JointMatrBuf {};                  // Nodes matrices and inverse joint matrices
  private:
    /* Load (.G3DM) model function.
     * ARGUMENTS:
     *   - trimesh filename (.G3DM):
     *       const std::string &FileName;
     *   - matrix to create model with:
     *       const matr &Transform;
     * RETURNS:
     *   (model_res &) reference to loaded model.
     */
    model_res & LoadG3DM( const std::string &FileName, const matr &Transform );

    /* Create (.GLTF) model function.
     * ARGUMENTS:
     *   - filename:
     *       const std::string &FileName;
     * RETURNS: None.
     */
    VOID CreateGLTFModel( const std::string &FileName );

    /* Load (.GLTF) model function.
     * ARGUMENTS:
     *   - filename (.GLTF):
     *       const std::string &FileName;
     *   - matrix to create model with:
     *       const matr &Transform;
     * RETURNS:
     *   (model_res &) reference to loaded model.
     */
    model_res & LoadGLTF( const std::string &FileName, const matr &Transform );

    /* Load (.GLB) model function.
     * ARGUMENTS:
     *   - filename (.GLB):
     *       const std::string &FileName;
     *   - size of gltf part in file:
     *       INT GltfSize;
     *   - binary buffer rvalue:
     *       std::vector<BYTE> &&BinaryBuffer;
     *   - matrix to create model with:
     *       const matr &Transform;
     * RETURNS:
     *   (model_res &) reference to loaded model.
     */
    model_res & LoadGLB( const std::string &FileName, INT GltfSize, std::vector<BYTE> &&BinaryBuffer, const matr &Transform );
  public:

    /* Play model's animation function.
     * ARGUMENTS:
     *   - name of animation to be used:
     *       const std::string &AnimName;
     *   - time to be used in animation:
     *       DBL Time;
     * RETURNS: None.
     */
    VOID DoAnim( const std::string &AnimName, DBL Time );

    /* Class constructor.
     * ARGUMENTS:
     *   - pointer to the render context:
     *       render *RndPtr;
     */
    model_res( render *RndPtr ) : resource(RndPtr)
    {
      Rnd = RndPtr;
    } /* End of 'model_res' function */

    /* Class move constructor.
     * ARGUMENTS:
     *   - rvalue reference to model_res:
     *       model_res &&Old;
     */
    model_res( model_res &&Old );

    /* Class destructor */
    ~model_res( VOID )
    {
      Free();
    } /* End of '~model_res' function */

    /* Create model to shape function.
     * ARGUMENTS:
     *   - filename (.G3DM or .GLTF):
     *       const std::string &FileName;
     *   - transform matrix:
     *       const matr &NewTransform;
     * RETURNS:
     *   (model_res &) reference to loaded model.
     */
    model_res & Create( const std::string &FileName,
                        const matr &NewTransform = matr::Identity() );

    /* Recalculate AABB with new matrix function.
     * ARGUMENTS:
     *   - new transform matrix:
     *       const matr &NewTransform;
     * RETURNS:
     *   (std::pair<vec3, vec3>) MinBB and MaxBB vectors.
     */
    std::pair<vec3, vec3> RecalculateAABB( const matr &NewTransform );

    /* Free model function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );

    /* Unstructured data file reader class */
    class rdr
    {
    private:
      // Memory pointer reference
      BYTE *&Ptr;

    public:
      /* Class constructor.
       * ARGUMENTS:
       *   - reference to memory pointer:
       *       BYTE *&PtrRef;
       */
      rdr( BYTE *&PtrRef ) : Ptr(PtrRef)
      {
      } /* End of 'rdr' function */

      /* Read data function.
       * ARGUMENT:
       *   - read data pointer:
       *       type *Data;
       *   - read data count:
       *       INT Count;
       * RETURNS: None.
       */
      template<typename type>
        VOID operator()( type *Data, const INT Count = 1 )
        {
          if (Count == 1)
            *Data = *(type *)Ptr, Ptr += sizeof(type);
          else
          {
            memcpy(Data, Ptr, sizeof(type) * Count);
            Ptr += sizeof(type) * Count;
          }
        } /* End of 'operator()' function */

      /* Read data function.
       * ARGUMENT:
       *   - read data (skip with store pointer) pointer:
       *       type **Data;
       *   - read data count:
       *       INT Count;
       * RETURNS: None.
       */
      template<typename type>
        VOID operator()( type **Data, const INT Count = 1 )
        {
          *Data = (type *)Ptr;
          Ptr += sizeof(type) * Count;
        } /* End of 'operator()' function */
    }; /* End of 'rdr' class */
  }; /* End of 'model' class */

  /* Model interface type */
  using model = std::shared_ptr<model_res>;
} /* end of 'tam' namespace */

/* Project namespace */
namespace tam
{
  /* 'model_manager' handle class */
  class model_manager : public resource_manager<model_res, std::string>
  {
  public:
    // Global model Id counter
    INT GlobalModelIdLast {1};
    /* Class constructor.
     * ARGUMENTS:
     *   - render instance reference:
     *       render &NewRnd;
     * RETURNS: None.
     */
    model_manager( render &NewRnd ) : resource_manager(NewRnd)
    {
    } /* End of 'model_manager' function */

    /* Class destructor */
    ~model_manager( VOID )
    {
      logger::info << "Models deinitialization";
    } /* End of '~model_manager' function */

    /* Primitive creation function.
     * ARGUMENTS:
     *   - name of file to load model from:
     *       const std::string &FileName;
     *   - matrix to create model with:
     *       const matr &Transform;
     * RETURNS:
     *   (model) created model interface.
     */
    model ModelCreate( const std::string &FileName, const matr &Transform = matr::Identity() )
    {
      model Mdl = Allocate(FileName);

      if (Mdl->Name == "")
      {
        Mdl->Create(FileName, Transform);
        return Mdl;
      }
      else
      {
        std::string Name = FileName + std::format("{}", GlobalModelIdLast++);
        Mdl = Allocate(Name);
        Mdl->Create(FileName, Transform);
        return Mdl;
      }
    } /* End of 'ModelCreate' function */

    /* Delete model function.
     * ARGUMENTS:
     *   - model to free:
     *       model &Mdl;
     * RETURNS: None.
     */
    VOID ModelDelete( model &Mdl )
    {
      Delete(Mdl);
    } /* End of 'ModelDelete' function */
  }; /* End of 'model_manager' class */
} /* end of 'tam' namespace */

#endif /* __tam_res_models_h_ */

/* END OF 'tam_res_models.h' FILE */
