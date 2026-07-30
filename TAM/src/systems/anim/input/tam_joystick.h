/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_joystick.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Joystick input declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 31.01.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_joystick_h_
#define __tam_joystick_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* 'joystick' handle class */
  class joystick
  {
  public:
    BYTE JBut[32], OldJBut[32], JButClick[32];  // Uses array for all joystick buttons
    INT JPov;                                   // Joystick powerful number
    DBL Jx, Jy, Jz, Jr;                         // Joystick coords

    /* Class default constructor */
    joystick( VOID ) : JPov(0), Jx(0), Jy(0), Jz(0), Jr(0)
    {
      memset(JBut, 0, 32);
      memset(JButClick, 0, 32);
      memset(OldJBut, 0, 32);
    } /* End of 'joystick' function */

    /* Class destructor */
    ~joystick( VOID )
    {
    } /* End of '~joystick' function */

    /* Response timer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Reset( VOID )
    {
      JPov = 0;
      Jx = 0;
      Jy = 0;
      Jz = 0;
      Jr = 0;
      memset(JBut, 0, 32);
      memset(JButClick, 0, 32);
      memset(OldJBut, 0, 32);
    } /* End of 'Reset' function */

    /* Response timer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      /*
      if (joyGetNumDevs() > 0)
      {
        JOYCAPS js;

        if (joyGetDevCaps(JOYSTICKID1, &js, sizeof(js) == JOYERR_NOERROR))
        {
          JOYINFO ji = {sizeof(JOYINFO), JOY_RETURNALL};

          if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
          {
            for (INT i = 0; i < 32; i++)
            {
              for (INT i = 0; i < 32; i++)
              {
                JBut[i] = (ji.wButtons >> i) & 1;
                JButClick[i] = !OldJBut[i] && JBut[i];
              }
              memcpy(OldJBut, JBut, 32);
              Jx = VIDX_INPUT_GET_JOYSTICK_AXIS(X);
              Jy = VIDX_INPUT_GET_JOYSTICK_AXIS(Y);
              Jz = VIDX_INPUT_GET_JOYSTICK_AXIS(Z);
              Jr = VIDX_INPUT_GET_JOYSTICK_AXIS(R);
              Jpov = ji.dwPOV == 0xFFFF ? 0 : ji.dwPOW / 4500  + 1;
            }
          }
        }
      }
      */
    } /* End of 'Response' function */
  }; /* End of 'joystick' class */
} /* end of 'tam' namespace */

#endif /* __tam_joystick_h_ */

/* END OF 'tam_joystick.h' FILE */
