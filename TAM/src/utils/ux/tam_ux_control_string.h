/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_string.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX string input control declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 10.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ux_control_string_h_
#define __tam_ux_control_string_h_

#include "tam_ux_control.h"

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* String input control class.
     * Usage: ux::string_type Name {Tab1, "User:", "Default text"};
     *        ux::string_type Name1 {Tab1, "##01"};
     */
    class string_type : public base_control
    {
    private:
      CHAR Buf[256]; // Text buffer
      BOOL IsActiveFlag = FALSE; // Is string field active flag

    public:
      /* Constructor with optional default text.
       * ARGUMENTS:
       *   - parent tab reference:
       *       tab &T;
       *   - control display label:
       *       const std::string &L;
       *   - default text value:
       *       const std::string &Default;
       */
      string_type( tab &T, const std::string &L, const std::string &Default = "" ) :
        base_control(T, L)
      {
        memset(Buf, 0, sizeof(Buf));
        if (!Default.empty())
          strncpy(Buf, Default.c_str(), sizeof(Buf) - 1);
      } /* End of 'string_type' function */

      /* Implicit conversion to C-string operator.
       * ARGUMENTS: None.
       * RETURNS:
       *   (const CHAR *) buffer pointer.
       */
      operator const CHAR *( VOID ) const
      {
        return Buf;
      } /* End of 'operator const CHAR *' function */

      /* Implicit conversion to std::string operator.
       * ARGUMENTS: None.
       * RETURNS:
       *   (std::string) text value.
       */
      operator std::string( VOID ) const
      {
        return std::string(Buf);
      } /* End of 'operator std::string' function */

      /* Assignment from C-string operator.
       * ARGUMENTS:
       *   - string to assign:
       *       const CHAR *S;
       * RETURNS:
       *   (string_type &) self reference.
       */
      string_type & operator=( const CHAR *S )
      {
        memset(Buf, 0, sizeof(Buf));
        if (S != nullptr)
          strncpy(Buf, S, sizeof(Buf) - 1);
        return *this;
      } /* End of 'operator=' function */

      /* Assignment from std::string operator.
       * ARGUMENTS:
       *   - string to assign:
       *       const std::string &S;
       * RETURNS:
       *   (string_type &) self reference.
       */
      string_type & operator=( const std::string &S )
      {
        memset(Buf, 0, sizeof(Buf));
        strncpy(Buf, S.c_str(), sizeof(Buf) - 1);
        return *this;
      } /* End of 'operator=' function */

      /* Check if string field is active function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) Is string field active flag.
       */
      BOOL IsActive( VOID ) const
      {
        return IsActiveFlag;
      } /* End of 'IsActive' fucntion */

      /* Draw text input via ImGui function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Draw( VOID ) override;
    }; /* End of 'string_type' class */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_ux_control_string_h_ */

/* END OF 'tam_ux_control_string.h' FILE */
