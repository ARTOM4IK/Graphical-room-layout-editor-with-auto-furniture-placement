/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_typewriter.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX dynamic text control impelentation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 10.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux_control_typewriter.h"

/* Draw dynamic text via ImGui function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::typewriter_label::Draw( VOID )
{
  DBL dt = ImGui::GetIO().DeltaTime;

  Timer += dt;

  while (!Finished && Timer >= CharDelay)
  {
    Timer -= CharDelay;

    if (Index < FullText.size())
    {
      if (FullText[Index] == '\n')
      {
        VisibleText += '\n';
        Index++;
        continue;
      }

      if (Index + 1 < FullText.size() &&
          FullText[Index] == '\\' &&
          FullText[Index + 1] == 'n')
      {
        VisibleText += '\n';
        Index += 2;
        continue;
      }

      VisibleText += FullText[Index];
      Index++;
    }
    else
    {
      Finished = TRUE;
      break;
    }
  }
  ImGui::TextUnformatted(VisibleText.c_str());
} /* End of 'tam::ux::typewriter_label::Draw' function */

/* END OF 'tam_ux_control_typewriter.cpp' FILE */
