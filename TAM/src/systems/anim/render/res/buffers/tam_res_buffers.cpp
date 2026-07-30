/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_buffers.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               buffer resource implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 17.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
 
/* Buffer class constructor.
 * ARGUMENTS:
 *   - render pointer:
 *       render *Rnd;
 */
tam::buffer_res::buffer_res( render *Rnd ) : resource(Rnd), VlkBuffer(*Rnd)
{
} /* End of 'buffer_res' function */

/* Buffer move constructor.
 * ARGUMENTS:
 *   - old buffer rvalue:
 *       buffer_res &&Old;
 */
tam::buffer_res::buffer_res( buffer_res &&Old ) : resource(Old.Rnd), Size(Old.Size), Id(Old.Id), VlkBuffer(Old.VlkBuffer)
{
  Old.Size = 0;
  Old.Id = 0;
  Old.VlkBuffer.Buffer = VK_NULL_HANDLE;
  Old.VlkBuffer.Size = 0;
} /* End of 'tam::buffer_res::buffer_res' function */

/* Free buffer function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::buffer_res::Free( VOID )
{
  VlkBuffer.Destroy();
  Size = 0;
  Id = 0;
} /* End of 'tam::buffer_res::Free' function */

/* END OF 'tam_res_buffers.cpp' FILE */
