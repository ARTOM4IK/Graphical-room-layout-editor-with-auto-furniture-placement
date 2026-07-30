/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_label.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX read-only label control declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 10.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ux_control_label_h_
#define __tam_ux_control_label_h_

#include "tam_ux_control.h"

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* Read-only label control class.
     * Displays a label and a dynamic text value.
     * Value can be updated via SetText or operator=.
     */
    class label_type : public base_control
    {
    private:
      std::string Text {}; // Displayed value text

    public:
      /* Constructor with tab and label.
       * ARGUMENTS:
       *   - parent tab reference:
       *       tab &T;
       *   - control display label:
       *       const std::string &L;
       *   - initial text value:
       *       const std::string &InitText;
       */
      label_type( tab &T, const std::string &L, const std::string &InitText = {} ) :
        base_control(T, L), Text(InitText)
      {
      } /* End of 'label_type' function */

      /* Set displayed text value function.
       * ARGUMENTS:
       *   - new text value:
       *       const std::string &NewText;
       * RETURNS:
       *   (label_type &) self reference.
       */
      label_type & SetText( const std::string &NewText )
      {
        Text = NewText;
        return *this;
      } /* End of 'SetText' function */

      /* Get displayed text value function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (const std::string &) current text value.
       */
      const std::string & GetText( VOID ) const
      {
        return Text;
      } /* End of 'GetText' function */

      /* Assign text value operator.
       * ARGUMENTS:
       *   - new text value:
       *       const std::string &NewText;
       * RETURNS:
       *   (label_type &) self reference.
       */
      label_type & operator=( const std::string &NewText )
      {
        Text = NewText;
        return *this;
      } /* End of 'operator=' function */

      /* Implicit conversion to string operator.
       * ARGUMENTS: None.
       * RETURNS:
       *   (const std::string &) current text value.
       */
      operator const std::string &( VOID ) const
      {
        return Text;
      } /* End of 'operator const std::string &' function */

      /* Draw label via ImGui function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Draw( VOID ) override;
    }; /* End of 'label_type' class */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_ux_control_label_h_ */

/* END OF 'tam_ux_control_label.h' FILE */
