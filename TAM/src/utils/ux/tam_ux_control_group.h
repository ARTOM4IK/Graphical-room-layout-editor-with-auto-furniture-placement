/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_group.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX read-only label control declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 02.04.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ux_control_group_h_
#define __tam_ux_control_group_h_

#include "tam_ux_control.h"

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* Group begin control class.
     * Displays a collapsed group with dynamic text value.
     * Value can be updated via SetText or operator=.
     */
    class group_begin : public base_control
    {
      friend class group_manager;
      friend class group_end;
    private:
      std::string Header;           // Group header text
      BOOL IsOpen = TRUE;           // Current open/closed state
      BOOL IsActive = FALSE;        // Whether we are currently in draw mode
      // Controls inside this group
      std::vector<base_control *> ChildControls; 

    public:
      /* Constructor with tab and label.
       * ARGUMENTS:
       *   - parent tab reference:
       *       tab &T;
       *   - group header text:
       *       const std::string &HeaderText;
       */
      group_begin(tab &T, const std::string &HeaderText = {}) :
        base_control(T, HeaderText), Header(HeaderText)
      {
        // Register with group manager
        group_manager::PushGroup(this);
      } /* End of 'group_begin' function */

      /* Destructor (ensures group is closed) function */
      ~group_begin( VOID )
      {
        if (group_manager::CurrentGroup() == this)
          group_manager::PopGroup();
      } /* End of '~group_begin' function */

      /* Add a child control to this group.
       * ARGUMENTS:
       *   - control to add:
       *       base_control *Ctrl;
       * RETURNS: None.
       */
      void AddChild( base_control *Ctrl )
      {
        if (Ctrl != nullptr && std::find(ChildControls.begin(), ChildControls.end(), Ctrl) == ChildControls.end())
          ChildControls.push_back(Ctrl);
      } /* End of 'AddChild' function */

      /* Get header text function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (const std::string &) header text.
       */
      const std::string & GetHeader( VOID ) const
      {
        return Header;
      } /* End of 'GetHeader' function */

      /* Set header text function.
       * ARGUMENTS:
       *   - new header text:
       *       const std::string &NewHeader;
       * RETURNS:
       *   (group_begin &) self reference.
       */
      group_begin & SetHeader( const std::string &NewHeader )
      {
        Header = NewHeader;
        return *this;
      } /* End of 'SetHeader' function */

      /* Get open state function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) true if group is open.
       */
      BOOL IsOpenState( VOID ) const
      {
        return IsOpen;
      } /* End of 'IsOpenState' function */

      /* Set open state function.
       * ARGUMENTS:
       *   - new open state:
       *       BOOL NewOpen;
       * RETURNS:
       *   (group_begin &) self reference.
       */
      group_begin & SetOpen( BOOL NewOpen )
      {
        IsOpen = NewOpen;
        return *this;
      } /* End of 'SetOpen' function */

      /* Reset one-shot state for child controls.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID ResetFrame(VOID) override
      {
        for (auto* Ctrl : ChildControls)
          Ctrl->ResetFrame();
      } /* End of '' function */

      /* Implicit conversion to open state.
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) TRUE if group is open.
       */
      operator BOOL( VOID ) const
      {
        return IsOpen;
      } /* End of 'operator BOOL' function */

      /* Draw label via ImGui function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Draw( VOID ) override;
    }; /* End of 'group_begin' class */

    /* Group end control class (based on group begin).
     * Displays a collapsed group finish label.
     */
    class group_end : public base_control
    {
    private:
      // Reference to associated group begin control
      group_begin &GroupBeginItem;

    public:
      /* Constructor with tab.
       * ARGUMENTS:
       *   - parent group begin item:
       *       group_begin &ParentGroupBeginItem;
       */
      group_end( group_begin &ParentGroupBeginItem ) :
        GroupBeginItem(ParentGroupBeginItem),
        base_control(ParentGroupBeginItem.Tab)
      {
        // Pop from stack when group_end is created
        group_manager::PopGroup();
      } /* End of 'group_end' function */

      /* Draw label via ImGui function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Draw( VOID ) override;
    }; /* End of 'group_end' class */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_ux_control_group_h_ */

/* END OF 'tam_ux_control_group.h' FILE */
