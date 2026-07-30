/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_furniture.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Furniture collection declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3),
 *               Arsentev Artemy (AA4),
 *               Nechaev Vladimir (VN4),
 *               Kupchenkov Ivan (IK4),
 *               Novozhilova Maryana (MN7),
 *               Tretyakov Andrey (AT7),
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 16.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_furniture_h_
#define __tam_furniture_h_

#include "tam_def.h"

/* Project namespace */
namespace tam
{
  /* Furniture element description type */
  class furniture
  {
  private:

  public:
    std::string Name;     // Furniture name
    std::string FileName; // Furniture model file name
    vec3 Rotate;          // Rotate angles in degrees
    FLT Scale;            // Scale factor
    vec3 Size;            // Bound size
    vec3 Offset;          // Canonical placing offset
    vec3 MinBB, MaxBB;    // Bound size

    /* Obtain furniture transform matrix function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (matr) transformation matrix.
     */
    matr GetTransform( VOID ) const
    {
      return
        matr::Scale(vec3(Scale)) *
        matr::RotateX(Rotate.X) *
        matr::RotateY(Rotate.Y) *
        matr::RotateZ(Rotate.Z) *
        matr::Translate(Offset);
    } /* End of 'GetTransform' function */

    /* Recalculate AABB by model geometry function.
     * ARGUMENTS:
     *   - furniture model geometry:
     *       model Mdl;
     *   - result matrix pointer (nullptr for no store):
     *       matr *Transform;
     * RETURNS: None.
     */
    VOID RecalcBB( model Mdl, matr *Transform = nullptr )
    {
      if (Mdl == nullptr)
        return;
      matr m = GetTransform();

      /* Old issue
      matr m =
        matr::Scale(vec3(Scale)) *
        matr::RotateX(Rotate.X) *
        matr::RotateY(Rotate.Y) *
        matr::RotateZ(Rotate.Z) * matr::Translate(Offset);

      // Recalculate bound box
      vec3 p[8] =
      {
        vec3(-1, -1, -1),
        vec3(-1, -1,  1),
        vec3( 1, -1,  1),
        vec3( 1, -1, -1),

        vec3(-1, 1, -1),
        vec3(-1, 1,  1),
        vec3( 1, 1,  1),
        vec3( 1, 1, -1),
      };
      for (INT i = 0; i < 8; i++)
        p[i] = m.TransformPoint(Mdl->MinBB + (Mdl->MaxBB - Mdl->MinBB) * (p[i] + vec3(1)) / 2);
      vec3 mi, ma;
      mi = ma = p[0];
      for (INT i = 0; i < 8; i++)
        mi = mi.Min(p[i]), ma = ma.Max(p[i]);
      MinBB = mi;
      MaxBB = ma;
      */
      //Mdl->Transform = matr::Identity();
      std::tie(MinBB, MaxBB) = Mdl->RecalculateAABB(m);
      //Mdl->Transform = m;
      //std::tie(Mdl->MinBB, Mdl->MaxBB) = Mdl->RecalculateAABB(matr::Identity());
      Size = MaxBB - MinBB;
      if (Transform != nullptr)
        *Transform = m;
    } /* End of 'RecalcBB' function */
  }; /* End of 'furniture' class */

  /* Furniture type description type */
  class furniture_type
  {
  private:

  public:
    std::string TypeName; // Furniture type name
    std::vector<furniture> ModelList; // Furniture models list
  }; /* End of 'furniture_type' class */

  /* Furniture collection description type */
  class furniture_collection
  {
  public:
    std::string Name = "Furniture"; // Furniture collection name
    std::string FileName;           // Furniture collection file name

    std::vector<furniture_type> Types; // Furniture types list

    /* Load collection function.
     * ARGUMENTS:
     *   - collection .json file name:
     *       const std::string &LoadFileName;
     * RETURNS:
     *   (BOOL) load result.
     */
    BOOL Load( const std::string &LoadFileName = "" );

    /* Save collection function.
     * ARGUMENTS:
     *   - collection .json file name:
     *       const std::string &SaveFileName;
     * RETURNS:
     *   (BOOL) save result.
     */
    BOOL Save( const std::string &SaveFileName = "" );
  }; /* End of 'furniture_collection' class */
} /* end of 'tam' namespace */

#endif /* __tam_furniture_h_ */

/* END OF 'tam_furniture.h' FILE */

