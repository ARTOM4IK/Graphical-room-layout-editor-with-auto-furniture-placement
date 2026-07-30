/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_def.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Vulkan module common declarations file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 03.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_vlk_def_h_
#define __tam_vlk_def_h_

#include <shlwapi.h>
#pragma comment(lib, "shlwapi")

/* 'tam' namespace */
namespace tam
{
  /* 'vlk' namespace */
  namespace vlk
  {
    /* Forward declaration of 'core' structure */
    class core;

    /* Reference to core representation class */
    class core_ref
    {
    protected:
      core *Ptr = nullptr; // Pointer to core
    public:
      core_ref( VOID ) = default;

      /* Initialize core reference function.
       * ARGUMENTS:
       *   - reference to core:
       *       core &Core;
       * RETURNS: None.
       */
      VOID Init( core &C ) noexcept
      {
        Ptr = std::addressof(C);
      } /* End of 'Init' function */

      /* Get pointer to core function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (core *) pointer to core.
       */
      core * operator->( VOID )
      {
        assert(Ptr != nullptr && "core_ref not initialized!");
        return Ptr;
      } /* End of 'operator->' function */

      /* Get core function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (const core) result.
       */
      operator const core &( VOID ) const
      {
        assert(Ptr != nullptr && "core_ref not initialized!");
        return *Ptr;
      } /* End of '&' operator */
 
      core_ref( const core_ref & ) = delete;
      core_ref &operator=( const core_ref & ) = delete;
      core_ref( core_ref && ) noexcept = default;
      core_ref &operator=( core_ref && ) noexcept = default;
    }; /* End of 'core_ref' class */

    /* Vulkan core reference representation type */
    struct vlk_ref
    {
    public:
      core_ref VlkRef; // reference to Vulkan core instance

      /* Default type constructor.
       * ARGUMENTS: none.
       */
      vlk_ref( VOID )
      {
      } /* End of 'vlk_ref' function */

      /* Initialize Vulkan reference function.
       * ARGUMENTS:
       *   - core refenrence:
       *       core &C;
       * RETURNS: None.
       */
      VOID Init( core &C )
      {
        VlkRef.Init(C);
      } /* End of 'Init' function */
    }; /* End of 'vlk_ref' class */

    /* Animation system kernel class declaration */
    class project_kernel
    {
    private:
      // Core access mutex
      inline static std::recursive_mutex core_mutex;
    public:
      // Program start path
      inline static std::string Path;
      /* Class constructor */
      project_kernel( VOID )
      {
        // Lock access
        const std::lock_guard<std::recursive_mutex> lock(core_mutex);

        CHAR ProgramPath[MAX_PATH];
        GetModuleFileName(nullptr, ProgramPath, MAX_PATH);
        PathRemoveFileSpec(ProgramPath);

        Path = ProgramPath;
        Path += "\\";

        if (std::size_t pos = Path.find("out\\x64\\Debug\\"); pos != std::string::npos)
          Path = Path.substr(0, pos);
        else if (std::size_t pos = Path.find("out\\x64\\Release\\"); pos != std::string::npos)
          Path = Path.substr(0, pos);
        else if (std::size_t pos = Path.find("x64\\Release\\"); pos != std::string::npos)
          Path = Path.substr(0, pos);
        else if (std::size_t pos = Path.find("x64\\Debug\\"); pos != std::string::npos)
          Path = Path.substr(0, pos);
        else if (std::size_t pos = Path.find("Release\\"); pos != std::string::npos)
          Path = Path.substr(0, pos);
        else if (std::size_t pos = Path.find("Debug\\"); pos != std::string::npos)
          Path = Path.substr(0, pos);
      } /* End of 'project_kernel' function */
    }; /* End of 'project_kernel' class */
  } /* end of 'vlk' namespace */
} /* end of 'tam' namespace */

#endif /* __tam_vlk_def_h_ */

/* END OF 'tam_vlk_def.h' FILE */
