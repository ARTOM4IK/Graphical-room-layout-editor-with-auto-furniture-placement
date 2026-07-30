/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_material_res.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Material functions declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3),
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 14.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_res_material_res_h_
#define __tam_res_material_res_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* Forward declaraiton of 'prim_res' class for 'prim' type usage */
  class prim_res;
  /* Define primitive interface type */
  using prim = std::shared_ptr<prim_res>;

  /* Material buffer (for phong lighting system) structure */
  struct BUF_MTL_PHONG
  {
    vec4 Ka;              // Ambient
    vec4 KdTrans;         // Diffuse + transparency
    vec4 KsPh;            // Specular + Phong
    BOOL TextureFlags[8]; // Texture usage flags
  }; /* End of 'BUF_MTL_PHONG' structure */

  /* Material buffer (for PBR) structure */
  struct BUF_MTL_PBR
  {
    vec4 BaseColor;              // Base color (if txture is not in use)
    vec3 MetallicRoughnessScale; // Metallic and roughness coefficients, texture coordinates scale
    BOOL TextureFlags[8];        // Texture usage flags
  }; /* End of 'BUF_MTL_PBR' structure */

  /* 'material_res' handle class */
  class material_res : public resource
  {
  public:
    std::string Name {};              // Material name
    material_pattern MtlPat {};       // Material pattern
    std::vector<texture> Textures {}; // Textures array
    std::vector<buffer> Buffers {};   // Material data buffers vector

    tam::vlk::descriptor_set MtlDescriptorSet; // Material descriptor set (for buffers and textures)

    UINT64 StoreFrameNo {}; // Number of frame when material was drawn
    material NextDraw {};   // Pointer to next-to-draw material
    prim FirstDraw {};      // Pointer to first-to-draw primitive

    /* Class constructor.
     * ARGUMENTS:
     *   - pointer to the render context:
     *       render *RndPtr;
     */
    material_res( render *RndPtr );

    /* Class move constructor.
     * ARGUMENTS:
     *   - rvalue reference to material_res:
     *       material_res &&Old;
     */
    material_res( material_res &&Old );

    /* Class destructor */
    ~material_res( VOID )
    {
      Free();
    } /* End of '~material_res' function */

    /* Material creation function.
     * ARGUMENTS:
     *   - material name:
     *       const std::string &NewName;
     *   - material pattern:
     *       material_pattern NewMtlPat;
     *   - textures vector:
     *       std::vector<texture> Texs;
     *   - buffers vector:
     *       std::vector<buffer> Bufs;
     * RETURNS:
     *   (material_res &) self reference.
     */
    material_res & Create( const std::string &NewName, material_pattern &NewMtlPat,
      const std::vector<texture> &Texs = {}, const std::vector<buffer> &Bufs = {}, const std::vector<texture> &Attachments = {} );

    /* Material destroy function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );

    /* Apply material_res function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (shader *) shader interface;
     */
    VOID Apply( VOID );

    /* Update material buffers and textures function.
     * ARGUMENTS:
     *   - textures vector:
     *        const std::vector<texture> &Texs;
     *   - buffers vector:
     *        const std::vector<buffer> &Bufs;
     *   - attachments vector:
     *        std::vector<texture> &Attachments;
     * RETURNS: None.
     */
    VOID Update( const std::vector<texture> &Texs, const std::vector<buffer> &Bufs = {}, const std::vector<texture> &Attachments = {});
  }; /* End of 'material_res' class */

  /* 'material' class */
  using material = std::shared_ptr<material_res>;

  /* 'material_manager' handle class */
  class material_manager : public resource_manager<material_res, std::string>
  {
  public:
    /* Class default constructor */
    material_manager( render &Rnd ) : resource_manager(Rnd)
    {
    } /* End of 'material_manager' function */

    /* Class destructor */
    ~material_manager( VOID )
    {
      logger::info << "Materials deinitialization";
    } /* End of '~material_manager' function */

    /* Material creation (manager handling) function.
     * ARGUMENTS:
     *   - material name:
     *       const std::string &NewName;
     *   - material pattern:
     *       material_pattern NewMtlPat;
     *   - illumination coefficients (ambient. diffuse, specular):
     *       const vec3 &NewKa, &NewKd, &NewKs;
     *   - Phong shininess coefficient:
     *       FLT NewPh;
     *   - transparency coefficient:
     *       FLT NewTrans;
     *   - textures list:
     *       std::vector<texture> Texs;
     *   - buffers list:
     *       std::vector<buffer> Bufs;
     * RETURNS:
     *   (material) created material interface.
     */
    material MaterialCreate( const std::string &NewName,
                             material_pattern NewMtlPat,
                             std::vector<texture> Texs = {}, std::vector<buffer> Bufs = {}, const std::vector<texture> &Attachments = {} )
    {
      material Mtl = Allocate(NewName);
      if (Mtl->Name == "")
        Mtl->Create(NewName, NewMtlPat, Texs, Bufs, Attachments);
      return Mtl;
    } /* End of 'MaterialCreate' function */

    /* Delete material function.
     * ARGUMENTS:
     *   - material pointer:
     *       material &Mtl;
     * RETURNS: None.
     */
    VOID MaterialDelete( material &Mtl )
    {
      resource_manager::Delete(Mtl);
    } /* End of 'MtlFree' function */

    /* Find material in stock function.
     * ARGUMENTS:
     *   - material name:
     *       const std::string &Str;
     * RETURNS: 
     *   (material) self.
     */
    material MtlFind( const std::string &Str )
    {
      return Find(Str);
    } /* End of 'MtlFind' function */
  }; /* End of 'material' class */
} /* end of 'tam' namespace */

#endif /* __tam_res_material_res_h_ */

/* END OF 'tam_res_material_res.h' FILE */
