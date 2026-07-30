/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_units.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Unit module structures declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 13.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_units_h_
#define __tam_units_h_

#include "tam_def.h"

/* Project namespace */
namespace tam
{
  /* Forward declaration of 'anim' class */
  class anim;

  /* Unit representation class */
  class unit
  {
  public:
    /* Type destructor */
    virtual ~unit( VOID )
    {
    } /* End of '~unit' function */

    /* Get response from unit function.
     * ARGUMENTS:
     *   - animation context pointer:
     *       anim *Ani;
     * RETURNS: None.
     */
    virtual VOID Response( anim *Ani )
    {
    } /* End of 'Response' function */

    /* Render unit function.
     * ARGUMENTS:
     *   - animation context pointer:
     *       anim *Ani;
     * RETURNS: None.
     */
    virtual VOID Render( anim *Ani )
    {
    } /* End of 'Render' function */
  }; /* End of unit class */

  /* Scene representation class */
  class scene
  {
  private:
    std::vector<unit *> Units;      // Vector of all scene units
    std::vector<unit *> UnitsToAdd; // Vector of units to be add
    std::vector<unit *> UnitsToDel; // Vector of units to be delete
  public:
    /* default scene destructor */
    ~scene( VOID );

    /* Get response from all units function.
     * ARGUMENTS:
     *   - animation context pointer:
     *       anim *Ani;
     * RETURNS: None.
     */
    VOID Response( anim *Ani );

    /* Render scene function.
     * ARGUMENTS:
     *   - animation context pointer:
     *       anim *Ani;
     * RETURNS: None.
     */
    VOID Render( anim *Ani );

    /* Add unit into scene's stock function.
     * ARGUMENTS:
     *   - unit to add:
     *       unit *Uni;
     * RETURNS:
     *   (&scene) pointer to itself.
     */
    scene & operator<<( unit *Uni )
    {
      UnitsToAdd.push_back(Uni);
      return *this;
    } /* End of 'operator<<' function */

    /* Get number of units in scene function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (INT) number of units.
     */
    INT GetUnitCount( VOID ) const
    {
      return (INT)Units.size();
    } /* End of 'GetUnitCount' function */

    /* Clear scene function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Clear( VOID )
    {
      for (auto Uni : Units)
        UnitsToDel.push_back(Uni);
    } /* End of 'Clear' function */

    /* Clear all instead of reference unit function.
     * ARGUMENTS:
     *   - left unit pointer:
     *       unit *UniLeft;
     * RETURNS: None.
     */
    VOID ClearInsteadOf( unit *UniLeft )
    {
      for (auto Uni : Units)
        if (Uni != UniLeft)
          UnitsToDel.push_back(Uni);
    } /* End of 'ClearInsteadOf' function */
  }; /* End of scene class */
} /* end of 'tam' namespace */

#endif /* __tam_units_h_ */

/* END OF 'tam_units.h' FILE */
