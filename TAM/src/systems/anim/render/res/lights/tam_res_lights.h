/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_lights.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Light resource declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 19.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_res_lights_h_
#define __tam_res_lights_h_

#include "..\tam_res.h"

/* 'tam' namespace */

namespace tam
{
  /* Light type representation enum class */
  enum class light_type
  {
    ePOINT = 0, // Point light value
    eDIR = 1,   // Directional light value
    eSPOT = 2   // Spot light value
  }; /* End of 'light_type' enum class */

   /* 'light_res' handle class */
  class light_res : public resource
  {
  public:
    vec3
      Pos,            // Point and spot light source position
      Dir,            // Directional and spot light source direction
      Color;          // Light source color
    FLT Exp;          // Attenuation of light
    FLT CutOfAngle;   // cut spot light in angle
    FLT Intensity;    // intensity of light
    BOOL IsEnabled;   // Is enabled light flag
    std::string Name; // Light source name
    light_type Type;  // Type of light
    /* Class default constructor */
    light_res( render *RndPtr );

    /* light move constructor.
     * ARGUMENTS:
     *   - rvalue reference to light_res:
     *       light_res &&Old;
     */
    light_res( light_res &&Old );

    /* Class destructor */
    ~light_res( VOID )
    {
      Free();
    } /* End of '~light_res' function */

    /* Create light resource function.
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
    light_res & Create( light_type LightType, const std::string &NewName, const vec3 &Position, const FLT Intensity, const vec3 &Direction, const vec3 &Colour, FLT Att, FLT CutOfAngle );

    /* Update light function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Update( VOID );

    /* Free light function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );
  }; /* End of 'light_res' class */

  /* texture resource user type definition */
  using light = std::shared_ptr<light_res>;
} /* end of 'tam' namespace */

/* 'tam' namespace */
namespace tam
{
  /* Lighting mode flag representation class */
  enum class lighting_mode
  {
    eNORMAL = 0,  // Normal scene lighting
    ePRESENT = 1, // 'Fake' lighting of models from 3 sides
  }; /* End of 'lighting_mode' enum class */

  /* 'light_manager' handle class */
  class light_manager : public resource_manager<light_res, std::string>
  {
  public:
    lighting_mode LightingMode = lighting_mode::eNORMAL; // Lighting mode flag
    vec3 PresentLightsColor = vec3(1);                   // Color of present light sources
    FLT PresentIntensity = 5.0f;                         // Intensity of present lights

    /* light manager class constructor.
     * ARGUMENTS:
     *   - reference to render:
     *       render &Rnd;
     */
    light_manager( render &Rnd ) : resource_manager(Rnd)
    {
    } /* End of 'light_manager' function */

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
    light LightCreate( light_type LightType, const std::string &NewName, const vec3 &Position = {}, const FLT Intensity = 2, const vec3 &Direction = {}, const vec3 &Colour = {1, 1, 1}, FLT Att = 2.0, FLT CutOfAngle = 45 );

    /* Delete user light resouce function.
     * ARGUMENTS:
     *   - light entry reference:
     *        light &Entry;
     * RETURNS: None.
     */
    VOID LightDelete( light &Entry );

    /* Get all lights function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (const std::unordered_map<std::string, tam::light> &) lights stock.
     */
    const std::unordered_map<std::string, light> & GetLights( VOID );
  }; /* End of 'light_manager' class */
} /* end of 'tam' namespace */

#endif /* __tam_res_lights_h_ */

/* END OF 'tam_res_lights.h' FILE */
