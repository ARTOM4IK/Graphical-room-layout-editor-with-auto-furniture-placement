/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_control_select.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX dropdown select control declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 24.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ux_control_select_h_
#define __tam_ux_control_select_h_

#include "tam_ux_control.h"

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* Dropdown select control class.
     * Usage: ux::select_type Mode {Tab1, "Mode", {"A", "B", "C"}, 0};
     * Supports comparison with string and index.
     */
    class select_type : public base_control
    {
    private:
      std::vector<std::string> Items; // Dropdown items
      INT SelectedIndex;              // Currently selected index

    public:
      /* Constructor with items list.
       * ARGUMENTS:
       *   - parent tab reference:
       *       tab &T;
       *   - control display label:
       *       const std::string &L;
       *   - list of selectable items:
       *       const std::vector<std::string> &ItemList;
       *   - default selected index:
       *       INT DefaultIdx;
       */
      select_type( tab &T, const std::string &L,
        const std::vector<std::string> &ItemList, INT DefaultIdx = 0 ) :
        base_control(T, L), Items(ItemList), SelectedIndex(DefaultIdx)
      {
      } /* End of 'select_type' function */

      /* Implicit conversion to selected index operator.
       * ARGUMENTS: None.
       * RETURNS:
       *   (INT) selected index.
       */
      operator INT( VOID ) const
      {
        return SelectedIndex;
      } /* End of 'operator INT' function */

      /* Assignment by index operator.
       * ARGUMENTS:
       *   - index to set:
       *       INT Idx;
       * RETURNS:
       *   (select_type &) self reference.
       */
      select_type & operator=( INT Idx )
      {
        if (Idx >= 0 && Idx < (INT)Items.size())
          SelectedIndex = Idx;
        return *this;
      } /* End of 'operator=' function */

      /* Equality comparison with string operator.
       * ARGUMENTS:
       *   - item name to compare:
       *       const std::string &S;
       * RETURNS:
       *   (BOOL) TRUE if currently selected item name matches.
       */
      BOOL operator==( const std::string &S ) const
      {
        if (SelectedIndex < 0 || SelectedIndex >= (INT)Items.size())
          return FALSE;
        return Items[SelectedIndex] == S;
      } /* End of 'operator==' function */

      /* Inequality comparison with string operator.
       * ARGUMENTS:
       *   - item name to compare:
       *       const std::string &S;
       * RETURNS:
       *   (BOOL) TRUE if currently selected item name does not match.
       */
      BOOL operator!=( const std::string &S ) const
      {
        return !(*this == S);
      } /* End of 'operator!=' function */

      /* Equality comparison with index operator.
       * ARGUMENTS:
       *   - index to compare:
       *       INT Idx;
       * RETURNS:
       *   (BOOL) TRUE if currently selected index matches.
       */
      BOOL operator==( INT Idx ) const
      {
        return SelectedIndex == Idx;
      } /* End of 'operator==' function */

      /* Inequality comparison with index operator.
       * ARGUMENTS:
       *   - index to compare:
       *       INT Idx;
       * RETURNS:
       *   (BOOL) TRUE if currently selected index does not match.
       */
      BOOL operator!=( INT Idx ) const
      {
        return SelectedIndex != Idx;
      } /* End of 'operator!=' function */

      /* Get selected item name function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (const std::string &) selected item name.
       */
      const std::string & GetSelected( VOID ) const
      {
        static const std::string empty = "";
        if (SelectedIndex < 0 || SelectedIndex >= (INT)Items.size())
          return empty;
        return Items[SelectedIndex];
      } /* End of 'GetSelected' function */

      /* Get items count function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (INT) number of items.
       */
      INT GetCount( VOID ) const
      {
        return (INT)Items.size();
      } /* End of 'GetCount' function */

      /* Get items list function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (std::vector<std::string> &) number of items.
       */
      std::vector<std::string> & GetItems( VOID )
      {
        return Items;
      } /* End of 'GetItems' function */

      /* Add item to the end of the list function.
       * ARGUMENTS:
       *   - item name to add:
       *       const std::string &Name;
       * RETURNS:
       *   (select_type &) self reference.
       */
      select_type & Add( const std::string &Name )
      {
        Items.push_back(Name);
        if (Items.size() == 1)
          SelectedIndex = 0;
        return *this;
      } /* End of 'Add' function */

      /* Add item at specified position function.
       * ARGUMENTS:
       *   - item name to add:
       *       const std::string &Name;
       *   - insert position (clamped to valid range):
       *       INT Pos;
       * RETURNS:
       *   (select_type &) self reference.
       */
      select_type & Add( const std::string &Name, INT Pos )
      {
        if (Pos < 0)
          Pos = 0;
        if (Pos > (INT)Items.size())
          Pos = (INT)Items.size();
        Items.insert(Items.begin() + Pos, Name);
        if (Items.size() == 1)
          SelectedIndex = 0;
        else if (SelectedIndex >= Pos)
          SelectedIndex++;
        return *this;
      } /* End of 'Add' function */

      /* Delete item by name function.
       * Removes first occurrence. Adjusts selected index.
       * ARGUMENTS:
       *   - item name to remove:
       *       const std::string &Name;
       * RETURNS:
       *   (BOOL) TRUE if item was found and removed.
       */
      BOOL Delete( const std::string &Name )
      {
        for (INT i = 0; i < (INT)Items.size(); i++)
          if (Items[i] == Name)
            return Delete(i);
        return FALSE;
      } /* End of 'Delete' function */

      /* Delete item at specified position function.
       * Adjusts selected index to stay on the same logical item
       * or clamps it if the selected item was removed.
       * ARGUMENTS:
       *   - position to remove:
       *       INT Pos;
       * RETURNS:
       *   (BOOL) TRUE if position was valid and item removed.
       */
      BOOL Delete( INT Pos )
      {
        if (Pos < 0 || Pos >= (INT)Items.size())
          return FALSE;
        Items.erase(Items.begin() + Pos);
        if (Items.empty())
          SelectedIndex = -1;
        else if (SelectedIndex == Pos)
          SelectedIndex = (Pos < (INT)Items.size()) ? Pos : (INT)Items.size() - 1;
        else if (SelectedIndex > Pos)
          SelectedIndex--;
        return TRUE;
      } /* End of 'Delete' function */

      /* Sort items alphabetically function.
       * Preserves selection on the same item name after sort.
       * ARGUMENTS: None.
       * RETURNS:
       *   (select_type &) self reference.
       */
      select_type & Sort( VOID )
      {
        if (Items.empty())
          return *this;
        std::string cur = "";
        if (SelectedIndex >= 0 && SelectedIndex < (INT)Items.size())
          cur = Items[SelectedIndex];
        std::sort(Items.begin(), Items.end());
        if (!cur.empty())
        {
          auto it = std::find(Items.begin(), Items.end(), cur);
          SelectedIndex = (it != Items.end()) ? (INT)(it - Items.begin()) : 0;
        }
        return *this;
      } /* End of 'Sort' function */

      /* Clear all items function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (select_type &) self reference.
       */
      select_type & Clear( VOID )
      {
        Items.clear();
        SelectedIndex = -1;
        return *this;
      } /* End of 'Clear' function */

      /* Draw dropdown via ImGui function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Draw( VOID ) override;
    }; /* End of 'select_type' class */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_ux_control_select_h_ */

/* END OF 'tam_ux_control_select.h' FILE */
