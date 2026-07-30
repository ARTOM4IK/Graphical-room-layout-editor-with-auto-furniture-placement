/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_mouse.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Mouse input declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 09.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_mouse_h_
#define __tam_mouse_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* 'mouse' handle class */
  class mouse
  {
  protected:
    INT &Wheel;                              // Reference of mouse wheel
    HWND &hWndRef;                           // Reference of hWnd
  public:
    INT Mdx, Mdy, Mdz, Mx, My, Mz;           // Mouse coords
    BYTE MBut[3], OldMBut[3], MButClick[3];  // All uses array of buttons

    /* Class default constructor */
    mouse( HWND &hWndRef, INT &WheelRef ) : hWndRef(hWndRef), Wheel(WheelRef), Mdx(0), Mdy(0), Mdz(0), Mx(0), My(0), Mz(0)
    {
      memset(MBut, 0, 3);
      memset(OldMBut, 0, 3);
      memset(MButClick, 0, 3);
    } /* End of 'mouse' function */

    /* Class destructor */
    ~mouse( VOID )
    {
    } /* End of '~mouse' function */

    /* Reset timer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Reset( VOID )
    {
      Mdx = 0;
      Mdy = 0;
      Mdz = 0;
      Mx = 0;
      My = 0;
      Mz = 0;
      memset(MBut, 0, 3);
      memset(OldMBut, 0, 3);
      memset(MButClick, 0, 3);
    } /* End of 'Reset' function */
    
    /* Response timer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      POINT pt;
 
      GetCursorPos(&pt);
      ScreenToClient(hWndRef, &pt);
      Mdx = pt.x - Mx;
      Mdy = pt.y - My;
      Mx = pt.x;
      My = pt.y;
 
      /* Mouse wheel */
      Mdz = Wheel;
      Mz += Wheel;
      Wheel = 0;
      
      MBut[0] = GetAsyncKeyState(VK_LBUTTON) >> 15;
      MBut[1] = GetAsyncKeyState(VK_RBUTTON) >> 15;
      MBut[2] = GetAsyncKeyState(VK_MBUTTON) >> 15;

      for (INT i = 0; i < 3; i++)
        MButClick[i] = !OldMBut[i] && MBut[i];
      memcpy(OldMBut, MBut, 3);
    } /* End of 'Response' function */
  }; /* End of 'mouse' class */
} /* end of 'tam' namespace */

#endif /* __tam_mouse_h_ */

/* END OF 'tam_mouse.h' FILE */
