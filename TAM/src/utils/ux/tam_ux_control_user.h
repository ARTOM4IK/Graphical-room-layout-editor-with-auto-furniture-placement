/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_user.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX user type control implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 24.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ux_control_user_h_
#define __tam_ux_control_user_h_

#include "tam_ux_control.h"

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* User type control class.
     * Displays a cuastom drawing UX/UI elements.
     */
    class user_type : public base_control
    {
    private:
      // User based UX/UI rendering function
      std::function<VOID ( VOID )> UserRenderFunc;

    public:
      /* Constructor with tab and label.
       * ARGUMENTS:
       *   - parent tab reference:
       *       tab &T;
       *   - user specific UX/UI rendering function:
       *       std::function<VOID ( VOID )> UserRenderFunc
       */
      user_type( tab &T, std::function<VOID ( VOID )> UserRenderFunc = nullptr ) :
        base_control(T, "")
      {
      } /* End of 'user_type' function */

      /* User based UX/UI rendering function.
       * ARGUMENTS:
       *   - new text value:
       *       const std::string &NewText;
       *   - user specific UX/UI rendering function:
       *       std::function<VOID ( VOID )> NewUserRenderFunc;
       * RETURNS:
       *   (user_type &) self reference.
       */
      user_type & SetUserFunc( const std::function<VOID ( VOID )> &NewUserRenderFunc )
      {
        UserRenderFunc = NewUserRenderFunc;
        return *this;
      } /* End of 'SetUserFunc' function */

      /* Draw custom drawing UX/UI elements via ImGui function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Draw( VOID ) override;
    }; /* End of 'text_type' class */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_ux_control_text_h_ */

/* END OF 'tam_ux_control_text.h' FILE */
