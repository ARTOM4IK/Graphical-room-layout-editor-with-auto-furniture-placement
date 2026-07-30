/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_text.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX static text control implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 24.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ux_control_text_h_
#define __tam_ux_control_text_h_

#include "tam_ux_control.h"

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* Static text control class.
     * Displays a static text text value.
     * Text value can be updated via SetText or operator=.
     */
    class text_type : public base_control
    {
    private:
      ImVec4 UserColor {ImGui::GetStyleColorVec4(ImGuiCol_Text)};  // Displayed value text
      const ImVec4 &ColorRef; // Displayed value text

    public:
      /* Constructor with tab and text.
       * ARGUMENTS:
       *   - parent tab reference:
       *       tab &T;
       *   - initial text value:
       *       const std::string &InitText;
       */
      text_type( tab &T, const std::string &InitText = {} ) :
        base_control(T, InitText), ColorRef(ImGui::GetStyleColorVec4(ImGuiCol_Text))
      {
      } /* End of 'text_type' function */

      /* Set displayed text value function.
       * ARGUMENTS:
       *   - new text value:
       *       const std::string &NewText;
       * RETURNS:
       *   (text_type &) self reference.
       */
      text_type & SetText( const std::string &NewText )
      {
        Label = NewText;
        return *this;
      } /* End of 'SetText' function */

      /* Get displayed text value function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (const std::string &) current text value.
       */
      const std::string & GetText( VOID ) const
      {
        return Label;
      } /* End of 'GetText' function */

      /* Assign text value operator.
       * ARGUMENTS:
       *   - new text value:
       *       const std::string &NewText;
       * RETURNS:
       *   (text_type &) self reference.
       */
      text_type & operator=( const std::string &NewText )
      {
        Label = NewText;
        return *this;
      } /* End of 'operator=' function */

      /* Implicit conversion to string operator.
       * ARGUMENTS: None.
       * RETURNS:
       *   (const std::string &) current text value.
       */
      operator const std::string &( VOID ) const
      {
        return Label;
      } /* End of 'operator const std::string &' function */

      /* Draw static text via ImGui function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Draw( VOID ) override;
    }; /* End of 'text_type' class */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_ux_control_text_h_ */

/* END OF 'tam_ux_control_text.h' FILE */
