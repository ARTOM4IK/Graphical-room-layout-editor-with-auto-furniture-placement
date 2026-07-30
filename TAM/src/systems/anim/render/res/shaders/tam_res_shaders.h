/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_shaders.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Shader declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kupchenkov Ivan (IK4).
 * LAST UPDATE : 05.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_res_shaders_h_
#define __tam_res_shaders_h_

#include "..\tam_res.h"

/* 'tam' namespace */
namespace tam
{
  /* Directory watcher class */
  class dir_watcher
  {
  private:
    // Overlapped structure for changing hook
    OVERLAPPED Ovr {};
    // Traching directory handle
    HANDLE hDir {};
    // Read directory changes data buffer
    BYTE Buffer[30 * 47] {};
    // Store path
    std::string Path;

  public:
    /* Start directory watching function.
     * ARGUMENTS:
     *   - directory path:
     *       const std::string &DirPath;
     * RETURNS: None.
     */
    virtual VOID StartWatch( const std::string &DirPath );

    /* Stop directory watching function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID StopWatch( VOID );

    /* Check if directory is changed function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (BOOL) TRUE if directory contents is changed (and subfolders).
     */
    virtual BOOL IsChanged( VOID );
  }; /* End of 'dir_watcher' class */

  /* 'shader_res' handle class */
  class shader_res : public resource
  {
  private:
    dir_watcher Watcher;     // Dir watcher

  public:
    vlk::shader VlkShd;      // Vulkan shader
    std::string Name {};     // Shader dir name
    BOOL IsChanged = FALSE;  // Is shader changed

    /* Class default constructor.
     * ARGUMENTS:
     *   - pointer to render:
     *       render *RndPtr;
     */
    shader_res( render *RndPtr );

    /* Class move constructor.
     * ARGUMENTS:
     *   - old shaders:
     *       shader_res &&Old;
     */
    shader_res( shader_res &&Old );

    /* Class destructor */
    ~shader_res( VOID )
    {
      Free();
    } /* End of '~shader_res' function */

    /* Shader creation function.
     * ARGUMENTS:
     *   - shader dir name:
     *       std::string DirName;
     * RETURNS:
     *   (shader_res &) shader.
     */
    shader_res & Create( std::string DirName );

    /* Free shader function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );

    /* Update shader function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (BOOL) is shader updated.
     */
    BOOL Update( VOID );

  }; /* End of 'shader_res' class */

  /* 'shader' class */
  using shader = std::shared_ptr<shader_res>;

  /* 'shader_manager' handle class */
  class shader_manager : public resource_manager<shader_res, std::string>
  {
  public:
    // Global shaders dir watcher
    dir_watcher GlobalWatcher;

    // Flag is any shader changed
    BOOL IsShaderChange = TRUE;

    /* Class default constructor */
    shader_manager( render &Rnd ) : resource_manager(Rnd)
    {
      GlobalWatcher.StartWatch("bin/shaders/");
    } /* End of 'shader_manager' function */

    /* Class destructor */
    ~shader_manager( VOID )
    {
      logger::info << "Shaders deinitialization";
    } /* End of '~shader_manager' function */

    /* Shader creating function.
     * ARGUMENTS:
     *   - name of shader(with directories):
     *       std::string FileName;
     * RETURNS:
     *   (shader) shader.
     */
    shader ShaderCreate( std::string FileName )
    {
      shader Sh = Allocate(FileName);

      if (Sh->Name == "")
        Sh->Create(FileName);

      return Sh;
    } /* End of 'ShaderCreate' function */

    /* Free shader function.
     * ARGUMENTS:
     *   - shader to free reference:
     *       shader &Shd;
     * RETURNS: None.
     */
    VOID ShaderDelete( shader &Shd )
    {
      Delete(Shd);
    } /* End of 'ShaderFree' function */

    /* Shaders update function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID ShadersUpdate( VOID );

    /* Change all shaders IsChanged flags to FALSE function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID ShadersChange( VOID );

  }; /* End of 'shader_manager' class */
} /* end of 'tam' namespace */

#endif /* __tam_res_shaders_h_ */

/* END OF 'tam_res_shaders.h' FILE */
