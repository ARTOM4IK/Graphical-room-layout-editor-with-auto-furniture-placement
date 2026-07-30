/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : mth_camera.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math support module.
 *               Camera module implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 06.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_camera_h_
#define __mth_camera_h_

#include "mth_def.h"

/* 'mth' namespace */
namespace mth
{
  /* 'camera' handle class */
  template<typename type>
    class camera
    {
    public:
      vec3<type> 
        Loc,            // camera location
        Dir, Up, Right, // basis camera directions
        At;             // camera pivot point

      type
        ProjDist,       // near project plane distance
        FarClip,        // far project plane distance
        Size,           // inner project plane rectangle size
        Wp, Hp;         // project plane size

      INT
        OffsetX,        // Viewport horizontal offset
        OffsetY,        // Viewport vertical offset
        FrameW,         // frame width
        FrameH;         // frame height

      matr<type>
        View,           // view matrix
        Proj,           // projection matrix
        VP;             // view-proj matrix

      /* Class default constructor */
      camera( VOID ):
        Loc(0, 0, 5), Dir(0, 0, -1), Up(0, 1, 0), Right(1, 0, 0), At(0, 0, 0),
        ProjDist(0.05), FarClip(500), Size(0.05),
        OffsetX(0), OffsetY(0),
        FrameW(500), FrameH(500)
      {
        UpdateProj();
      } /* End of 'camera' function */

      /* Set projection parameters function.
       * ARGUMENTS:
       *   - new size, projsize and farclip:
       *       type NewSize, NewProjDist, NewFarClip;
       * RETURNS:
       *   (camera &) camera reference.
       */
      camera & SetProj( type NewProjDist, type NewSize, type NewFarClip )
      {
        ProjDist = NewProjDist;
        Size = NewSize;
        FarClip = NewFarClip;

        UpdateProj();
        return *this;
      } /* End of 'SetProj' funciton */

      /* Resize camera resolution function.
       * ARGUMENTS:
       *   - new width and height:
       *       INT NewW, NewH;
       * RETURNS:
       *   (camera &) resized camera.
       */
      camera & Resize( INT NewFrameW, INT NewFrameH )
      {
        FrameW = NewFrameW;
        FrameH = NewFrameH;

        UpdateProj();
        return *this;
      } /* End of 'Resize' funciton */

      /* Set camera loc, at and up vectors function.
       * ARGUMENTS:
       *   - loc, at, up vectors:
       *       const vec3<type> &L, &A, &U;
       * RETURNS:
       *   (camera &) camera.
       */
      camera & SetLocAtUp( const vec3<type> &L, const vec3<type> &A, const vec3<type> &U )
      {
        Loc = L;
        At = A;
        Up = U;

        Dir = (At - Loc).Normalizing();
        Right = (Dir % Up).Normalizing();
        Up = Right % Dir;

        UpdateProj();
        return *this;
      } /* End of 'SetLocAtUp' function */

      /* Get ray from camera to point function.
       * ARGUMENTS:
       *   - point coordinates on the screen:
       *       type Xs, Ys;
       * RETURNS:
       *   (ray) result ray.
       */
      ray<type> FrameRay( type Xs, type Ys ) const
      {
        vec3<type> Q =
          Dir * ProjDist +
          Right * ((Xs - OffsetX - FrameW / 2.0) * Wp / FrameW) +
          Up * ((-Ys + OffsetY + FrameH / 2.0) * Hp / FrameH);

        return ray(Loc + Q, Q);
      } /* End of 'FrameRay' function */

    private:
      /* Update camera projection function.
       * ARGUMENTS: none.
       * RETURNS: None.
       */
      VOID UpdateProj( VOID )
      {
        type rx = Size / 2, ry = Size / 2;

        if (FrameW > FrameH)
          rx *= (type)FrameW / FrameH;
        else
          ry *= (type)FrameH / FrameW;

        Wp = rx * 2;
        Hp = ry * 2;

        Proj = matr<type>::Frustum(-rx, rx, -ry, ry, ProjDist, FarClip);
        View = View.View(Loc, At, Up);
        VP = View * Proj;
      } /* End of 'UpdateProj' function */
    }; /* End of 'camera' class */
} /* end of 'mth' namespace */

#endif /* __mth_camera_h_ */

/* END OF 'mth_camera.h' FILE */
