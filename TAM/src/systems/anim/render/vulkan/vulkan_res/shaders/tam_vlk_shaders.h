/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_shaders.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Vulkan shader structure declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 10.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_vlk_shaders_h_
#define __tam_vlk_shaders_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* 'vlk' namespace */
  namespace vlk
  {
    /* Vulkan core class forward declaration */
    class core;

    /* 'shader' handle class */
    class shader
    {
      core *VlkPtr; // Vulkan class instance pointer

      /* Load file to memory function.
       * ARGUMENTS:
       *   - file name:
       *       const std::string &FileName;
       * RETURNS:
       *   (std::string) loaded text.
       */
      std::string LoadTextFile( const std::string &FileName );

      /* Save text into log file function.
       * ARGUMENTS:
       *   - name of shader:
       *       const std::string &Name;
       *   - shader stage:
       *       const std::string &Stage;
       *   - text to log:
       *       const std::string &Text;
       * RETURNS: None.
       */
      VOID Log( const std::string &Name, const std::string &Stage, const std::string &Text );

      /* Binary SPIR-V file load to memory function.
       * ARGUMENTS:
       *   - SPIR-V file name:
       *       const std::string &FileName;
       * RETURNS:
       *   (VkShaderModule) loaded SPIR-V shader module.
       */
      VkShaderModule LoadSPIRVFile( const std::string &FileName );
    public:

      std::string ShaderDirName {}; // loaded shader directory name

      // shader modules (if absent - VK_NULL_HANDLE)
      VkShaderModule 
        ShaderModuleVert = VK_NULL_HANDLE,
        ShaderModuleCtrl = VK_NULL_HANDLE,
        ShaderModuleEval = VK_NULL_HANDLE,
        ShaderModuleGeom = VK_NULL_HANDLE,
        ShaderModuleFrag = VK_NULL_HANDLE,
        ShaderModuleComp = VK_NULL_HANDLE;

      /* Vulkan shader handle type constructor.
       * ARGUMENTS:
       *   - global Vulkan handle pointer:
       *      core &Vlk;
       */
      shader( core *Vlk ) : VlkPtr(Vlk)
      {
      } /* End of 'shader' function */

      /* Load shader function.
       * ARGUMENTS:
       *   - shader directory name:
       *       const std::string &ShaderName;
       * RETURNS:
       *   (BOOL) TRUE if shader was loaded successfully, FALSE otherwise.
       */
      BOOL Create( const std::string &ShaderName );

      /* Destroy Vulkan shader function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Destroy( VOID );

      /* Reload shader function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) TRUE if shader was reloaded successfully, FALSE otherwise.
       */
      BOOL Update( VOID );
    }; /* End of 'shader' class */
  } /* end of 'vlk' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_vlk_shaders_h_ */

/* END OF 'tam_vlk_shaders.h' FILE */
