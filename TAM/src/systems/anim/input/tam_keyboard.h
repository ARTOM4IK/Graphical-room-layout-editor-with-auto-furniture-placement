/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_keyboard.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Keyboard input declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 21.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_keyboard_h_
#define __tam_keyboard_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* 'keyboard' handle class */
  class keyboard
  {
  public:
    BYTE Keys[256] {}, KeysClick[256] {}, KeysOld[256] {}; // Uses arrays for all keys

    /* Class default constructor */
    keyboard( VOID )
    {
      Reset();
    } /* End of 'keyboard' function */

    /* Class destructor */
    ~keyboard( VOID )
    {
    } /* End of '~keyboard' function */

    /* Reset timer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Reset( VOID )
    {
      memset(Keys, 0, 256);
      memset(KeysClick, 0, 256);
      memset(KeysOld, 0, 256);
    } /* End of 'Reset' function */

    /* Response timer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      GetKeyboardState(Keys);

      for (auto i = 0; i < 256; i++)
      {
        Keys[i] >>= 7;
        KeysClick[i] = Keys[i] && !KeysOld[i];
      }

      CopyMemory(KeysOld, Keys, sizeof(KeysOld));
    } /* End of 'Response' function */
  }; /* End of 'keyboard' class */
} /* end of 'tam' namespace */

#endif /* __tam_keyboard_h_ */

/* END OF 'tam_keyboard.h' FILE */
