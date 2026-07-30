/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_lights.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Light resource implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 31.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Class default constructor */
tam::light_res::light_res( render *RndPtr ) : resource(RndPtr)
{
} /* End of 'light_res' function */

/* light move constructor.
 * ARGUMENTS:
 *   - rvalue reference to light_res:
 *       light_res &&Old;
 */
tam::light_res::light_res( light_res &&Old ) : resource(Old.Rnd), 
                Pos(Old.Pos), Dir(Old.Dir), Color(Old.Color), Exp(Old.Exp), Intensity(Old.Intensity)
{
  if (this == &Old)
    return;
} /* End of 'light_res' function */

/* Create light function.
 * ARGUMENTS:
 *   - light name:
 *      const std::string &NewName;
 *   - Position of light:
 *      const vec3 &Position;
 *   - Direction of light (if directional or spot):
 *      const vec3 &Direction;
 *   - Colour of light:
 *      const vec3 &Direction;
 *   - type of light:
 *      light_type Type;
 * RETURNS:
 *   (light_res &) reference to light resource.
 */
tam::light_res & tam::light_res::Create( light_type LightType, const std::string &NewName, const vec3 &Position, const FLT Intensity, const vec3 &Direction, const vec3 &Colour, FLT Att, FLT CutOfAngle )
{
  Name = NewName;
  Pos = Position;
  Dir = Direction;
  Color = Colour;
  this->Type = LightType;
  IsEnabled = TRUE;
  this->Intensity = Intensity;
  Exp = Att;
  this->CutOfAngle = CutOfAngle;

  return *this;
} /* End of 'tam::light_res::Create' function */

/* Update light function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::light_res::Update( VOID )
{
  Rnd->IsLightsUpdated = FALSE;
} /* End of 'tam::light_res::Update' function */

/* Free light function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::light_res::Free( VOID )
{
} /* End of 'tam::light_res::Free' function */

/* Create light function.
 * ARGUMENTS:
 *   - light name:
 *      const std::string &NewName;
 *   - Position of light:
 *      const vec3 &Position;
 *   - Direction of light (if directional or spot):
 *      const vec3 &Direction;
 *   - Colour of light:
 *      const vec3 &Direction;
 *   - type of light:
 *      light_type LightType;
 * RETURNS:
 *   (light) light resource.
 */
tam::light tam::light_manager::LightCreate( light_type LightType, const std::string &NewName, const vec3 &Position, const FLT Intensity, const vec3 &Direction, const vec3 &Colour, FLT Att, FLT CutOfAngle )
{
  light L = Allocate(NewName);

  if (L->Name == "")
    L->Create(LightType, NewName, Position, Intensity, Direction, Colour, Att, CutOfAngle);
  return L;
} /* End of 'tam::light_manager::LightCreate' function */

/* Delete user light resouce function.
 * ARGUMENTS:
 *   - light entry reference:
 *        light &Entry;
 * RETURNS: None.
 */
VOID tam::light_manager::LightDelete( light &Entry )
{
  Delete(Entry);
} /* End of 'tam::light_manager::LightDelete' function */

/* Get all lights function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (const std::unordered_map<std::string, tam::light> &) lights stock.
 */
const std::unordered_map<std::string, tam::light> & tam::light_manager::GetLights( VOID )
{
  return Stock;
} /* End of 'tam::light_manager::GetLights' function */

/* END OF 'tam_res_lights.cpp' FILE */
