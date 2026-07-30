/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_typewriter.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX dynamic text control declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 01.04.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ux_control_typewriter_h_
#define __tam_ux_control_typewriter_h_

#include "tam_ux_control.h"

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* Typewriter label control class.
     * Displays text with per-character animation.
     */
    class typewriter_label : public base_control
    {
    private:
      std::string FullText {};     // Full text
      std::string VisibleText {};  // Visible text

      DBL Timer = 0;
      DBL CharDelay = 0.03; // type speed

      size_t Index = 0;
      BOOL Finished = FALSE;

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
      typewriter_label( tab &T, const std::string &L, const std::string &InitText = {} ) :
        base_control(T, L)
      {
        SetText(InitText);
      } /* End of 'typewriter_label' function */

      /* Set displayed text value function.
       * ARGUMENTS:
       *   - new text value:
       *       const std::string &NewText;
       * RETURNS:
       *   (typewriter_label &) self reference.
       */
      typewriter_label & SetText( const std::string &NewText )
      {
        FullText = NewText;
        VisibleText.clear();
        Timer = 0;
        Index = 0;
        Finished = FALSE;
        return *this;
      } /* End of 'SetText' function */

      /* Get displayed text value function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (const std::string &) current text value.
       */
      const std::string & GetText( VOID ) const
      {
        return FullText;
      } /* End of 'GetText' function */

      /* Assign text value operator.
       * ARGUMENTS:
       *   - new text value:
       *       const std::string &NewText;
       * RETURNS:
       *   (typewriter_label &) self reference.
       */
      typewriter_label & operator=( const std::string &NewText )
      {
        return SetText(NewText);
      } /* End of 'operator=' function */

      /* Check if is finished function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) is finished.
       */
      BOOL IsFinished( VOID ) const
      {
        return Finished;
      } /* End of 'IsFinished' function */

      /* Set animaton speed function.
       * ARGUMENTS:
       *   - animation speed:
       *       DBL Delay;
       * RETURNS: None.
       */
      VOID SetSpeed( DBL Delay )
      {
        CharDelay = Delay;
      } /* End of 'SetSpeed' function */

      /* Draw dynamic text via ImGui function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Draw( VOID ) override;
    }; /* End of 'typewriter_label' class */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_ux_control_typewriter_h_ */

/* END OF 'tam_ux_control_typewriter.h' FILE */
