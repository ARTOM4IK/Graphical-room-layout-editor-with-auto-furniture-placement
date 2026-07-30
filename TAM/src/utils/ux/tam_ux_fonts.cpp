/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_fonts.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX core font loading implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 20.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux.h"

/* Read font display names from Windows registry function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (std::map<std::string, std::string>) lowercase filename -> display name.
 */
std::map<std::string, std::string> tam::ux::core::ReadFontDisplayNames( VOID )
{
  std::map<std::string, std::string> result;
  HKEY hKey;

  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
      "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts",
      0, KEY_READ, &hKey) != ERROR_SUCCESS)
    return result;

  DWORD index = 0;
  CHAR value_name[256];
  BYTE value_data[MAX_PATH];
  DWORD name_len, data_len, type;

  while (TRUE)
  {
    name_len = sizeof(value_name);
    data_len = sizeof(value_data);
    if (RegEnumValueA(hKey, index++, value_name, &name_len,
        nullptr, &type, value_data, &data_len) != ERROR_SUCCESS)
      break;
    if (type != REG_SZ)
      continue;

    std::string FileName = (const CHAR *)value_data;
    auto dot = FileName.rfind('.');
    if (dot != std::string::npos)
      FileName = FileName.substr(0, dot);
    for (auto &c : FileName)
      c = (CHAR)tolower(c);

    std::string display = value_name;
    auto pos = display.find(" (TrueType)");
    if (pos != std::string::npos)
      display = display.substr(0, pos);

    result[FileName] = display;
  }
  RegCloseKey(hKey);
  return result;
} /* End of 'tam::ux::core::ReadFontDisplayNames' function */

/* Load a single system font by file name function.
 * ARGUMENTS:
 *   - font file name without extension (e.g. "calibri"):
 *       const CHAR *FontName;
 *   - registry display name map (filename_lc -> display name):
 *       const std::map<std::string, std::string> &DisplayNames;
 * RETURNS: None.
 */
VOID tam::ux::core::LoadSystemFont( const CHAR *FontName,
  const std::map<std::string, std::string> &DisplayNames )
{
  ImGuiIO &io = ImGui::GetIO();
  std::string path = std::string("C:\\Windows\\Fonts\\") + FontName + ".ttf";

  if (ImFont *f = io.Fonts->AddFontFromFileTTF(path.c_str(), 16.0f); f != nullptr)
  {
    LoadedFonts.push_back(f);

    std::string key = FontName;
    for (auto &c : key)
      c = (CHAR)tolower(c);
    auto it = DisplayNames.find(key);
    FontNames.push_back(it != DisplayNames.end() ? it->second : std::string(FontName));
  }
} /* End of 'tam::ux::core::LoadSystemFont' function */

/* Load all system fonts into ImGui atlas function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::core::LoadFonts( VOID )
{
  ImGuiIO &io = ImGui::GetIO();

  LoadedFonts.push_back(io.Fonts->AddFontDefault());
  FontNames.push_back("Default");

  auto DisplayNames = ReadFontDisplayNames();

  const CHAR *font_names[] =
  {
    "calibri", "calibrib", // "calibril", "calibrili",
    "consola", "consolab", // "consolai", "consolaz",
    "verdana", "verdanab", // "verdanai", "verdanaz",
    "arial", "arialbd",    // "ariali", "arialbi",
    "cour", "courbd",      // "couri", "courbi",
    "comic", "comicbd",
    "BOOKOS", "BOOKOSB",
    "tahoma", "tahomabd",
    "times", "timesbd",    // "timesi", "timesbi",
    "segoeui", "segoeuib", // "segoeuii", "segoeuiz",
    "trebuc", "trebucbd",  // "trebucbi", "trebucit",
    "georgia", "georgiab", // "georgiai", "georgiaz",
  };

  for (auto &name : font_names)
    LoadSystemFont(name, DisplayNames);

  io.Fonts->Build();
  RegRestoreAll();
} /* End of 'tam::ux::core::LoadFonts' function */

/* END OF 'tam_ux_fonts.cpp' FILE */
