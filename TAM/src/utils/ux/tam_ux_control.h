/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX control base classes declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 02.04.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ux_control_h_
#define __tam_ux_control_h_

#include "tam_ux_tab.h"

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* Forward declaration */
    class group_begin;
    class group_end;

    /* Group manager - handles nested groups via static stack */
    class group_manager
    {
    private:
      /* Group stack */
      inline static std::stack<group_begin *> GroupStack;

    public:
      /* Push specified group to stack function.
       * ARGUMENTS:
       *   - pushed group pointer:
       *       group_begin *G;
       * RETURNS: None.
       */
      static void PushGroup( group_begin *G )
      {
        GroupStack.push(G);
      } /* End of '' function */

      /* Pop group from stack function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      static void PopGroup( VOID )
      {
        if (!GroupStack.empty())
          GroupStack.pop();
      } /* End of 'PopGroup' function */

      /* Obtain current group from stack function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (group_begin *) group pointer.
       */
      static group_begin * CurrentGroup( VOID )
      {
        return GroupStack.empty() ? nullptr : GroupStack.top();
      } /* End of 'CurrentGroup' function */
    }; /* End of 'group_manager' class */

    /* Base control abstract class.
     * All UX controls inherit from this class.
     * Registers/unregisters itself with the parent tab automatically.
     */
    class base_control
    {
      friend class tab;
      static inline INT NextControlUID = 0; // Global unique ID counter
    protected:
      tab &Tab;            // Reference to parent tab
      std::string Label;   // Display label for ImGui
      INT ControlUID;      // Unique control identifier

      /* Get ImGui-unique label (Label##uxN) function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (std::string) label with unique ImGui ID suffix.
       */
      std::string GetUID( VOID ) const
      {
        return Label + "##ux" + std::to_string(ControlUID);
      } /* End of 'GetUID' function */

    public:
      /* Constructor with tab and label.
       * ARGUMENTS:
       *   - parent tab reference:
       *       tab &T;
       *   - control display label:
       *       const std::string &L;
       */
      base_control( tab &T, const std::string &L = {} );

      /* Type destructor */
      virtual ~base_control( VOID )
      {
        Tab.UnregisterControl(this);
      } /* End of '~base_control' function */

      /* Draw control via ImGui function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      virtual VOID Draw( VOID ) = 0;

      /* Reset one-shot state (for buttons etc.) function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      virtual VOID ResetFrame( VOID )
      {
      } /* End of 'ResetFrame' function */

      /* Set control label function.
       * ARGUMENTS:
       *   - new label:
       *       const std::string &NewLabel;
       * RETURNS: None.
       */
      VOID SetLabel( const std::string &NewLabel )
      {
        Label = NewLabel;
        ResetFrame();
      } /* End of 'SetLabel' function */
    }; /* End of 'base_control' class */

    /* Typed control base template class.
     * Provides value storage and implicit conversion to value type.
     */
    template<typename type>
      class base_type : public base_control
      {
      protected:
        type Value {}; // Stored value

      public:
        /* Constructor with tab, label and default value.
         * ARGUMENTS:
         *   - parent tab reference:
         *       tab &T;
         *   - control display label:
         *       const std::string &L;
         *   - default value:
         *       const type &Default;
         */
        base_type( tab &T, const std::string &L = {}, const type &Default = {} ) :
          base_control(T, L), Value(Default)
        {
        } /* End of 'base_type' function */

        /* Implicit conversion to stored value type operator.
         * ARGUMENTS: None.
         * RETURNS:
         *   (type) stored value.
         */
        operator type( VOID ) const
        {
          return Value;
        } /* End of 'operator type' function */
      }; /* End of 'base_type' class template */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_ux_control_h_ */

/* END OF 'tam_ux_control.h' FILE */
