/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_tab.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX tab class declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 19.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ux_tab_h_
#define __tam_ux_tab_h_

#include "tam_def.h"

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* Forward declaration of control base class */
    class base_control;

    /* Forward declaration of UX core class */
    class core;

    /* Icon resource representation structure */
    struct icon
    {
      std::string Name {}; // Icon resource name

      /* Default constructor */
      icon( VOID )
      {
      } /* End of 'icon' function */

      /* Constructor by resource name.
       * ARGUMENTS:
       *   - icon resource name:
       *       const std::string &IconName;
       */
      icon( const std::string &IconName ) : Name(IconName)
      {
      } /* End of 'icon' function */
    }; /* End of 'icon' structure */

    /* Tab representation class.
     * Groups controls into named tabs within the UX panel.
     */
    class tab
    {
      friend class core;
      friend class base_control;
    private:
      core *Core {};                            // Pointer to owning UX core
      std::string Name {};                      // Tab display name
      icon Icon {};                             // Tab icon (optional)
      std::vector<base_control *> Controls {};  // Registered controls in this tab

    public:
      /* Default constructor */
      tab( VOID ) : Core(nullptr)
      {
      } /* End of 'tab' function */

      /* Constructor with core, name and optional icon.
       * ARGUMENTS:
       *   - pointer to UX core (anim inherits from ux::core):
       *       core *C;
       *   - tab display name:
       *       const std::string &TabName;
       *   - tab icon (optional):
       *       const icon &TabIcon;
       */
      tab( core *C, const std::string &TabName, const icon &TabIcon = {} );

      /* Copying Constructor.
       * ARGUMENTS:
       *   - reference to copyinh tab control:
       *       const tab &CopyTab;
       */
      tab( const tab &CopyTab );

      /* Type destructor */
      ~tab( VOID );

      /* Assignment operator function.
       * ARGUMENTS:
       *   - reference to copyinh tab control:
       *       const tab &CopyTab;
       * RETURNS:
       *   (tab &) selft reference.
       */
      tab & operator=( const tab &CopyTab );

      /* Assignment with move semantic operator function.
       * ARGUMENTS:
       *   - reference to copyinh tab control:
       *       tab &&CopyTab;
       * RETURNS:
       *   (tab &) selft reference.
       */
      tab & operator=( tab &&CopyTab );

      /* Register control in this tab function.
       * ARGUMENTS:
       *   - pointer to control:
       *       base_control *Ctrl;
       * RETURNS: None.
       */
      VOID RegisterControl( base_control *Ctrl );

      /* Unregister control from this tab function.
       * ARGUMENTS:
       *   - pointer to control:
       *       base_control *Ctrl;
       * RETURNS: None.
       */
      VOID UnregisterControl( base_control *Ctrl );

      /* Get tab display name function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (const std::string &) tab name.
       */
      const std::string & GetName( VOID ) const
      {
        return Name;
      } /* End of 'GetName' function */

      /* Get tab icon function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (const icon &) tab icon.
       */
      const icon & GetIcon( VOID ) const
      {
        return Icon;
      } /* End of 'GetIcon' function */

      /* Draw all controls in this tab function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DrawControls( VOID );

      /* Reset one-shot controls (buttons etc.) function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID ResetControls( VOID );
    }; /* End of 'tab' class */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_ux_tab_h_ */

/* END OF 'tam_ux_tab.h' FILE */
