/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_shaders.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Shaders implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kupchenkov Ivan (IK4).
 * LAST UPDATE : 05.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Start directory watching function.
 * ARGUMENTS:
 *   - directory path:
 *       const std::string &DirPath;
 * RETURNS: None.
 */
VOID tam::dir_watcher::StartWatch( const std::string &DirPath )
{
  StopWatch();
  ZeroMemory(&Ovr, sizeof(OVERLAPPED));
  Path = DirPath;

  // Open directory
  hDir =
    CreateFile(DirPath.c_str(),
      FILE_LIST_DIRECTORY,
      FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
      nullptr, OPEN_EXISTING,
      FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
      nullptr);

  // Start monitoring
  Ovr.hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

  ReadDirectoryChangesW(hDir, Buffer, sizeof(Buffer), TRUE,
    FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_FILE_NAME,
    nullptr, &Ovr, nullptr);

} /* End of 'tam::dir_watcher::StartWatch' function */

/* Stop directory watching function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::dir_watcher::StopWatch( VOID )
{
  if (Ovr.hEvent != nullptr)
    CloseHandle(Ovr.hEvent);
  if (hDir != nullptr)
    CloseHandle(hDir);

  ZeroMemory(&Ovr, sizeof(OVERLAPPED));
  hDir = nullptr;
  Path = "";
} /* End of 'tam::dir_watcher::StopWatch' function */

/* Check if directory is changed function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) TRUE if directory contents is changed (and subfolders).
 */
BOOL tam::dir_watcher::IsChanged( VOID )
{
  if (Ovr.hEvent == nullptr || hDir == nullptr)
    return FALSE;

  INT id = WaitForSingleObject(Ovr.hEvent, 0);
  if (id == WAIT_OBJECT_0)
  {
    // Restart monitoring
    ResetEvent(Ovr.hEvent);
    ReadDirectoryChangesW(hDir, Buffer, sizeof(Buffer), TRUE,
      FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_FILE_NAME,
      nullptr, &Ovr, nullptr);
    return TRUE;
  }
  return FALSE;
} /* End of 'tam::dir_watcher::IsChanged' function */

/* Class default constructor.
 * ARGUMENTS:
 *   - pointer to render:
 *       render *RndPtr;
 */
tam::shader_res::shader_res( render *RndPtr ) : resource(RndPtr), VlkShd(RndPtr)
{
} /* End of 'tam::shader_res::shader_res' function */

/* Class move constructor.
 * ARGUMENTS:
 *   - old shaders:
 *       shader_res &&Old;
 */
tam::shader_res::shader_res( shader_res && Old ) : resource(Old.Rnd), Watcher(Old.Watcher), VlkShd(Old.VlkShd), Name(Old.Name), IsChanged(Old.IsChanged)
{
  Old.Name = "";
  // Old.VlkShd.ShaderModuleVert = VK_NULL_HANDLE;
  // Old.VlkShd.ShaderModuleCtrl = VK_NULL_HANDLE;
  // Old.VlkShd.ShaderModuleEval = VK_NULL_HANDLE;
  // Old.VlkShd.ShaderModuleGeom = VK_NULL_HANDLE;
  // Old.VlkShd.ShaderModuleFrag = VK_NULL_HANDLE;
  // Old.VlkShd.ShaderModuleComp = VK_NULL_HANDLE;
  Old.VlkShd.Destroy();
  Old.IsChanged = FALSE;
} /* End of 'shader_res' function */

/* Shader creation function.
 * ARGUMENTS:
 *   - shader dir name:
 *       std::string DirName;
 * RETURNS:
 *   (shader_res &) shader.
 */
tam::shader_res & tam::shader_res::Create( std::string DirName )
{
  Free();
  Name = DirName;
  IsChanged = TRUE;

  logger::debug_info << "Try to " >> 0x33FA88 << "load " >> 0x88AAFF << Name >> logger::DefFore << " shader.";
  if (!VlkShd.Create(Name))
    tam::logger::error << "Error in creating " >> 0x88AAFF << Name >> logger::DefFore << " shader.";

  Watcher.StartWatch("bin/shaders/" + Name);
  return *this;
} /* End of 'tam::shader_res::Create' function */

/* Free shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::shader_res::Free( VOID )
{
  VlkShd.Destroy();
  Watcher.StopWatch();
  if (Name != "")
    tam::logger::debug_info << "Shader " >> 0x88AAFF << Name >> 0x33FA88 << " deleted" >> tam::logger::DefFore << ".";  // 0xFA3130
  Name = "";
} /* End of 'tam::shader_res::Free' function */

/* Update shader function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) is shader updated.
 */
BOOL tam::shader_res::Update( VOID )
{
  if (!Watcher.IsChanged())
  {
    IsChanged = FALSE;
    return FALSE;
  }
  Sleep(30);
  if (!VlkShd.Update())
  {
    tam::logger::error << "Error in creating " >> 0x88AAFF << Name >> tam::logger::DefFore << " shader.";
    return FALSE;
  }
  Sleep(3);
  tam::logger::info << "Shader " >> 0x88AAFF << Name >> 0x33FA88 << " updated" >> tam::logger::DefFore << ".";
  Watcher.StartWatch("bin/shaders/" + Name);
  IsChanged = TRUE;
  return TRUE;
} /* End of 'tam::shader_res::Update' function */

/* Shaders update function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::shader_manager::ShadersUpdate( VOID )
{
  IsShaderChange = FALSE;
  if (!GlobalWatcher.IsChanged())
    return;
  for (const auto &Shd : Stock)
  {
    if (Shd.second.get()->Update())
      IsShaderChange = TRUE;
  }
} /* End of 'tam::shader_manager::ShadersUpdate' function */

/* Change all shaders IsChanged flags to FALSE function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::shader_manager::ShadersChange( VOID )
{
  for (auto Shd : Stock)
    Shd.second.get()->IsChanged = FALSE;
  IsShaderChange = FALSE;
} /* End of 'tam::shader_manager::ShadersUpdate' function */

/* END OF 'tam_res_shaders.cpp' FILE */
