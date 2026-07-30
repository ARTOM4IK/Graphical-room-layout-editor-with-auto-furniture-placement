/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_unit_control.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Control unit implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kupchenkov Ivan (IK4).
 * LAST UPDATE : 30.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* 'tam' namespace */
namespace tam
{
  /* Anonymous namespace */
  namespace
  {
    /* Control unit sample representation class */
    class unit_control : public unit
    {
    private:
      BOOL IsWASD = FALSE;  // Flag is WADS using now
      FLT SaveDist = 0.0;   // Last dist before WASD

    public:

      /* Class constructor.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       */
      unit_control( anim *Ani )
      {
        logger::info << "Control unit initialization.";
        Ani->IsCamBufUpdate = TRUE;
        Ani->Cam.SetLocAtUp(mth::vec3<FLT>(4.7), mth::vec3<FLT>(0, 0, 0), mth::vec3<FLT>(0, 1, 0));
      } /* End of 'unit_sample' function */

      /* Get response from unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
        /* Common usage control */
        if (Ani->Keys[VK_LCONTROL] && Ani->KeysClick['P'])
          Ani->IsPause = !Ani->IsPause;

        if (Ani->Keys[VK_LCONTROL] && Ani->KeysClick['F'])
          Ani->Cam.SetLocAtUp(mth::vec3<FLT>(4.7), mth::vec3<FLT>(0, 0, 0), mth::vec3<FLT>(0, 1, 0));

        if (Ani->KeysClick[VK_F11])
          Ani->FlipFullScreen();

        if (Ani->Keys['W'] || Ani->Keys['S'] || Ani->Keys['A'] || Ani->Keys['D'] || Ani->Keys['K'] || Ani->Keys['L'] || Ani->Keys[VK_MENU])
        {
          if (!IsWASD)
            SaveDist = !(Ani->Cam.At - Ani->Cam.Loc), IsWASD = TRUE;
          Ani->Cam.At = Ani->Cam.Loc + (Ani->Cam.At - Ani->Cam.Loc).Normalize() * 0.1;
          mth::vec3<FLT> Dir(0);
          Dir += Ani->Cam.Dir.Normalize() * Ani->Keys['W'] - Ani->Cam.Dir.Normalize() * Ani->Keys['S'];
          Dir += Ani->Cam.Right.Normalize() * Ani->Keys['D'] - Ani->Cam.Right.Normalize() * Ani->Keys['A'];
          Ani->Cam.Up += (Ani->Cam.Right.Normalize() * Ani->Keys['L'] - Ani->Cam.Right.Normalize() * Ani->Keys['K'])* 0.8 * (1 + Ani->Keys[VK_SHIFT] * 4.7) * Ani->GlobalDeltaTime;

          if (Ani->Keys[VK_MENU])
            Dir = vec3(Dir.X, 0, Dir.Z);

          Ani->Cam.Loc = Ani->Cam.Loc + Dir * 1.8 * (1 + Ani->Keys[VK_SHIFT] * 3.0) * Ani->GlobalDeltaTime;
          Ani->Cam.At =  Ani->Cam.At + Dir * 1.8 * (1 + Ani->Keys[VK_SHIFT] * 3.0) * Ani->GlobalDeltaTime;
          Ani->Cam.SetLocAtUp(Ani->Cam.Loc, Ani->Cam.At, Ani->Cam.Up);
        }
        else if (IsWASD && SaveDist >= 0.1)
        {
          Ani->Cam.At = Ani->Cam.Loc + (Ani->Cam.At - Ani->Cam.Loc).Normalize() * SaveDist;
          Ani->Cam.SetLocAtUp(Ani->Cam.Loc, Ani->Cam.At, Ani->Cam.Up);
          IsWASD = FALSE;
        }

        /* Camera handle */
        if (Ani->Keys[VK_LCONTROL])
        {
          /* Rotate handle */
          FLT
            Dist = !(Ani->Cam.At - Ani->Cam.Loc),
            cosT = (Ani->Cam.Loc.Y - Ani->Cam.At.Y) / Dist,
            sinT = sqrt(1 - cosT * cosT),
            plen = Dist * sinT,
            cosP = (Ani->Cam.Loc.Z - Ani->Cam.At.Z) / plen,
            sinP = (Ani->Cam.Loc.X - Ani->Cam.At.X) / plen,
            Azimuth = R2D(atan2(sinP, cosP)),
            Elevator = R2D(atan2(sinT, cosT));

          Azimuth += 0.47 * COM_MAX(COM_MIN(Dist, 0.8), 0.6) * ((-0.1 * Ani->Keys[VK_LBUTTON] * Ani->Mdx) + (1 + Ani->Keys[VK_SHIFT] * 3.0) * 
            (-0.1 * Ani->Keys[VK_LBUTTON] * Ani->Mdx +
             Ani->GlobalDeltaTime * 180 * (IsWASD ? 1 : -1.8) * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])));

          Elevator += 0.47 * COM_MAX(COM_MIN(Dist, 0.8), 0.6) * ((-0.1 * Ani->Keys[VK_LBUTTON] * Ani->Mdy) + (1 + Ani->Keys[VK_SHIFT] * 3.0) * 
            (-0.1 * Ani->Keys[VK_LBUTTON] * Ani->Mdy +
             Ani->GlobalDeltaTime * 80 * (IsWASD ? 1.8 : -1) * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])));

          if (Elevator < 0.08)
            Elevator = 0.08;
          else if (Elevator > 178.90)
            Elevator = 178.90;

          Dist += 0.30 * (Dist / 8) * ((1 + Ani->Keys[VK_SHIFT] * 3.0) * (0.01 * -Ani->Mdz + Ani->GlobalDeltaTime * 30 * (Ani->Keys[VK_NEXT] - Ani->Keys[VK_PRIOR])));
          if (Dist < 0.1)
            Dist = 0.1;

          /* Move handle */
          if (Ani->Keys[VK_RBUTTON])
          {
            FLT Wp, Hp, sx, sy;
            mth::vec3<FLT> dv;

            Wp = Ani->Cam.Size;
            Hp = Ani->Cam.Size;

            if (Ani->Cam.FrameW > Ani->Cam.FrameH)
              Wp *= (FLT)Ani->Cam.FrameW / Ani->Cam.FrameH;
            else
              Hp *= (FLT)Ani->Cam.FrameH / Ani->Cam.FrameW;

            sx = COM_MAX(COM_MIN((10 / Dist), 6.0), 0.8) * (1 + Ani->Keys[VK_SHIFT] * 3.0) * (-Ani->Mdx * Wp / Ani->Cam.FrameW * COM_MAX(Dist, 0.8) / Ani->Cam.ProjDist * 0.30);
            sy = COM_MAX(COM_MIN((10 / Dist), 6.0), 0.8) * (1 + Ani->Keys[VK_SHIFT] * 3.0) * (Ani->Mdy * Hp / Ani->Cam.FrameH * COM_MAX(Dist, 0.8) / Ani->Cam.ProjDist * 0.30);

            dv = Ani->Cam.Right * sx + Ani->Cam.Up * sy;
            Ani->Cam.At = Ani->Cam.At + dv;
            Ani->Cam.Loc = Ani->Cam.Loc + dv;
          }

          /* Setup result camera */
          Ani->IsCamBufUpdate = TRUE;
          Ani->Cam.SetLocAtUp((mth::matr<FLT>::RotateX(Elevator) * mth::matr<FLT>::RotateY(Azimuth) * mth::matr<FLT>::Translate(Ani->Cam.At)).TransformPoint(mth::vec3<FLT>(0, Dist, 0)), Ani->Cam.At, mth::vec3<FLT>(0, 1, 0));
        }

      } /* End of 'Response' function */

      /* Render unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Render( anim *Ani ) override
      {
      } /* End of 'Render' function */

      /* Test unit destructor */
      ~unit_control( VOID )
      {
        logger::info << "Control unit deinitialization.";
      } /* End of type destructor */
    };
    static tam::anim::unit_register<unit_control> _("Control");
  } /* end of anonymous namespace */
} /* end of 'tam' namespace */

/* END OF 'tam_unit_control.cpp' FILE */
