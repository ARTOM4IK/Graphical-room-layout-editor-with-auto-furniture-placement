/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_registry.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX core Windows registry styles handle file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 24.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux.h"

#define CGSG_SOFTWARE_NAME "TAM"

/* Set registry string function.
  * ARGUMENTS:
  *   - string name:
  *       const std::string &DataName;
  *   - string data:
  *       const std::string &Data;
  * RETURNS:
  *   (BOOL) TRUE if success, FALSE otherwise.
  */
static BOOL StoreRegData( const std::string &DataName, const std::string &Data )
{
  // Registry DB path handle
  HKEY hProgram {0}; // Key handle in current user software catalog for saving program data
  LSTATUS Res {};    // Function call result temporary variable
  Res = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\CGSG\\" CGSG_SOFTWARE_NAME, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, &hProgram);

  // If system cannot find key, create new
  if (Res == ERROR_FILE_NOT_FOUND || Res == ERROR_PATH_NOT_FOUND)
  {
    SECURITY_ATTRIBUTES Sec {};
    Sec.bInheritHandle = TRUE;
    CHAR Buf[0x100] {""};

    Res = RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\CGSG\\" CGSG_SOFTWARE_NAME, 0, Buf, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, &Sec, &hProgram, 0);
    constexpr const CHAR *ProgrammInfo {"CGSG Tough Arrangement Manager"};
    RegSetValueExA(hProgram, "", 0, REG_SZ, (BYTE *)ProgrammInfo, (DWORD)strlen(ProgrammInfo) + 1u);
  }
  //Res = RegSetValueA(hProgram, DataName.c_str(), REG_SZ, Data.c_str(), (DWORD)(Data.length() + 1));
  Res = RegSetValueExA(hProgram, DataName.c_str(), 0, REG_SZ, (BYTE *)Data.c_str(), (DWORD)(Data.length() + 1));
  RegCloseKey(hProgram);

  if (Res == ERROR_SUCCESS)
    return TRUE;
  if (Res == ERROR_MORE_DATA)
  {
    INT k = 0;
  }
  if (Res == ERROR_FILE_NOT_FOUND)
  {
    INT i = 0;
  }
  return FALSE;
} /* End of 'SetRegData' function */

/* Get registry string function.
 * ARGUMENTS:
 *   - string name:
 *       const std::string &DataName;
 *   - ontain string data:
 *       std::string &Data;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
static BOOL GetRegData( const std::string &DataName, std::string &Data )
{
  // Registry DB path handle
  HKEY hProgram {0}; // Key handle in current user software catalog for saving program data
  LSTATUS Res {};     // Function call result temporary variable
  Res = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\CGSG\\" CGSG_SOFTWARE_NAME, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, &hProgram);

  // If system cannot find key, create new
  if (Res == ERROR_FILE_NOT_FOUND || Res == ERROR_PATH_NOT_FOUND)
    return FALSE;
  CHAR Memory[300];
  DWORD ReqSize {}, ResSize {sizeof(Memory)}, ReqType {RRF_RT_REG_SZ}, ResType {RRF_RT_REG_SZ};
  Res = RegGetValueA(hProgram, "", DataName.c_str(), ReqType, &ResType, Memory, &ResSize);
  RegCloseKey(hProgram);

  if (Res == ERROR_SUCCESS)
  {
    Data = Memory;
    return TRUE;
  }
  if (Res == ERROR_MORE_DATA)
  {
    INT k = 0;
  }
  if (Res == ERROR_FILE_NOT_FOUND)
  {
    INT i = 0;
  }
  return FALSE;
} /* End of 'GetRegData' function */

/* Store all program user data to register function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::core::RegStoreAll( VOID )
{
  if (SelectedFontIndex >= 0 && SelectedFontIndex < FontNames.size())
    StoreRegData("FontName", FontNames[SelectedFontIndex]);
  StoreRegData("FontSize", std::format("{}", FontSize));
  StoreRegData("SplitSize", std::format("{}", PanelWidth));
  StoreRegData("ThemeName", std::get<2>(styles::ColorStyles[styles::CurrentStyle]));
} /* End of 'tam::ux::core::RegStoreAll' function */

/* Restore all program user data from register function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::core::RegRestoreAll( VOID )
{
  std::string res;

  if (GetRegData("FontName", res))
  {
    /// auto &IO = ImGui::GetIO();
    /// 
    for (INT i = 0; i < (INT)FontNames.size(); i++)
      if (FontNames[i] == res)
        SelectedFontIndex = i;
    /// for (ImFont *font : IO.Fonts->Fonts)
    ///   if (font->GetDebugName() == res)
    ///   {
    ///     IO.FontDefault = font;
    ///     break;
    ///   }
  }
  if (GetRegData("FontSize", res))
  {
    DBL size = std::strtod(res.c_str(), nullptr);

    if (size < 8 || size > 47)
      size = 18;
    //ImGui::GetStyle()._NextFrameFontSizeBase = ImGui::GetStyle().FontSizeBase = size;
    FontSize = size;
  } // std::format("{}", ImGui::GetStyle().FontSizeBase));
  if (GetRegData("SplitSize", res))
  {
    DBL size = std::strtod(res.c_str(), nullptr);

    PanelWidth = size;
  }
  if (GetRegData("ThemeName", res))
  {
    styles::SetStyle(res);
  } // std::get<2>(ui::styles::ColorStyles[ui::styles::CurrentStyle]));
} /* End of 'tam::ux::core::RegRestoreAll' function */

/* END OF 'tam_ux_registry.cpp' FILE */
