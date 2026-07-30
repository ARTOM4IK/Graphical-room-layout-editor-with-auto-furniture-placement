/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_def.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Default common definition module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3),
 *               Arsentev Artemy (AA4),
 *               Nechaev Vladimir (VN4),
 *               Kupchenkov Ivan (IK4),
 *               Novozhilova Maryana (MN7),
 *               Tretyakov Andrey (AT7),
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 09.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_def_h_
#define __tam_def_h_

#ifndef WIN32
#define WIN32
#include <commondf.h>
#undef WIN32
#else
#include <commondf.h>
#endif /* WIN32 */


/* Debug memory allocation support */
#ifndef NDEBUG 
# define _CRTDBG_MAP_ALLOC
# include <crtdbg.h> 
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
static class __Dummy
{
public:
  /* Class constructor */
  __Dummy(VOID)
  {
    //SetDbgMemHooks();
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
  } /* End of '__Dummy' constructor */
} __oops;

#endif /* _DEBUG */ 

#ifdef _DEBUG 
# ifdef _CRTDBG_MAP_ALLOC 
#   define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
# endif /* _CRTDBG_MAP_ALLOC */ 
#endif /* _DEBUG */

#include <string>
#include <vector>
#include <map>
#include <array>
#include <span>
#include <stack>
#include <variant>
#include <set>

#include <functional>
#include <algorithm>
#include <thread>
#include <exception>

#include <format>
#include <fstream>
#include <istream>
#include <ostream>
#include <iostream>

#include <filesystem>
#include <chrono>
#include <random>

#include <cassert>

#include <windows.h>
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

/* Math support */
#include "mth/mth.h"

/* Base large integer types */
typedef unsigned long long UINT64;
typedef long long INT64;

/* Base float-point types */
typedef FLOAT FLT;
typedef DOUBLE DBL;

/* Project namespace */
namespace tam
{
  /* Math types definitions */
  typedef mth::vec2<FLT> vec2;
  typedef mth::vec3<FLT> vec3;
  typedef mth::vec4<FLT> vec4;
  typedef mth::matr<FLT> matr;
  typedef mth::ray<FLT> ray;
  typedef mth::camera<FLT> camera;
  typedef mth::cg::mesh<FLT> mesh;
} /* end of 'tam' namespace */

/* Convert 4-symbol string to double word integer function.
 * ARGUMENTS:
 *   - string constant:
 *       const CHAR *Str;
 *   - string length:
 *       std::size_t Len;
 * RETURNS:
 *   (UINT) result converted double word integer value.
 */
inline constexpr UINT operator""_dword( const CHAR *Str, std::size_t Len )
{
  UINT res {};
  if (Len > 0)
    res |= Str[0];
  if (Len > 1)
    res |= Str[1] << 8;
  if (Len > 2)
    res |= Str[2] << 16;
  if (Len > 3)
    res |= Str[3] << 24;
  return res;
} /* End of 'operator""_dword' function */

/* ANSI to wide character string converter.
 * ARGUMENTS:
 *   - source string:
 *       const std::string &Str;
 *   - result code page (CP_ACP, CP_UTF8):
 *       UINT CodePage;
 * RETURNS:
 *   (std::wstring) result wide string.
 */
inline std::wstring StringToWString( const std::string &Str, UINT CodePage = CP_ACP )
{
  if (Str == "")
    return L"";
    
  INT BufferSize = MultiByteToWideChar(CodePage, 0, Str.c_str(), -1, nullptr, 0);
  if (BufferSize == 0)
    return L"";
    
  std::wstring WideStr(BufferSize - 1, L'\0');
  MultiByteToWideChar(CodePage, 0, Str.c_str(), -1, &WideStr[0], BufferSize);
    
  return WideStr;
} /* End of 'StringToWString' function */

/* Wide character to ANSI string converter.
 * ARGUMENTS:
 *   - source string:
 *       const std::wstring &Str;
 *   - result code page (CP_ACP, CP_UTF8):
 *       UINT CodePage;
 * RETURNS:
 *   (std::string) result wide string.
 */
inline std::string WStringToString( const std::wstring &WStr, UINT CodePage = CP_ACP )
{
  if (WStr.empty())
    return "";
    
  INT BufferSize = WideCharToMultiByte(CodePage, 0, WStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
  if (BufferSize == 0)
    return "";
    
  std::string Str(BufferSize - 1, '\0');
  WideCharToMultiByte(CodePage, 0, WStr.c_str(), -1, &Str[0], BufferSize, nullptr, nullptr);
    
  return Str;
} /* End of 'WStringToString' function */

// Converter from wchar_t (UTF-16) to UTF-8
inline std::string ToUTF8( const wchar_t* wstr) 
{
  if (!wstr) return "";
  int size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
  std::string result(size, 0);
  WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &result[0], size, NULL, NULL);
  return result;
}
inline std::string UTF8ToSystem(const char* utf8_str) 
{
  if (!utf8_str) return "";
    
  // Convert UTF-8 into UTF-16
  int utf16_size = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, NULL, 0);
  std::wstring utf16_str(utf16_size, 0);
  MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, &utf16_str[0], utf16_size);
    
  // Convert UTF-16 into system encoding
  int system_size = WideCharToMultiByte(CP_ACP, 0, utf16_str.c_str(), -1, NULL, 0, NULL, NULL);
  std::string result(system_size, 0);
  WideCharToMultiByte(CP_ACP, 0, utf16_str.c_str(), -1, &result[0], system_size, NULL, NULL);
    
  return result;
}
/* Helpful string replace function.
 * ARGUMENTS:
 *   - source string:
 *       const std::string &Str;
 *   - string to find:
 *       const std::string& From;
 *   - string for replace to:
 *       const std::string& To;
 * RETURNS:
 *   (std::string) result string.
 */
inline std::string ReplaceAll( const std::string &Str, const std::string& From, const std::string& To )
{
  std::string res(Str);
  size_t start_pos = 0;

  while ((start_pos = res.find(From, start_pos)) != std::string::npos)
  {
    res.replace(start_pos, From.length(), To);
    start_pos += To.length(); // Handles case where 'to' is a substring of 'from'
  }
  return res;
} /* End of 'ReplaceAll' function */

/* Convert string russian letter to lower case function.
 * ARGUMENTS:
 *   - source string:
 *       const std::string &Str;
 * RETURNS:
 *   (std::string) result string.
 */
inline std::string ToLower( const std::string &Str )
{
  std::string res = "";
  for (auto ch : Str)
  {
    if ((UCHAR)ch >= (UCHAR)'À' && (UCHAR)ch <= (UCHAR)'ß')
      ch = (UCHAR)ch - (UCHAR)'À' + (UCHAR)'à';
    else if ((UCHAR)ch == (UCHAR)'¨')
      ch = (UCHAR)'¸';
    else
      ch = std::tolower(ch);
    res += ch;
  }
  return res;
} /* End of 'ToLower' function */

/* Convert string russian letter to upper case function.
 * ARGUMENTS:
 *   - source string:
 *       const std::string &Str;
 * RETURNS:
 *   (std::string) result string.
 */
inline std::string ToUpper( const std::string &Str )
{
  std::string res = "";
  for (auto ch : Str)
  {
    if ((UCHAR)ch >= (UCHAR)'à' && (UCHAR)ch <= (UCHAR)'ÿ')
      ch = (UCHAR)ch - (UCHAR)'à' + (UCHAR)'À';
    else if ((UCHAR)ch == (UCHAR)'¸')
      ch = (UCHAR)'¨';
    else
      ch = std::toupper(ch);
    res += ch;
  }
  return res;
} /* End of 'ToUpper' function */

/* Helpful string trimming function.
 * ARGUMENTS:
 *   - source string:
 *       const std::string &Str;
 * RETURNS:
 *   (std::string) result string.
 */
inline std::string Trim( const std::string &Str )
{
  INT start_pos = 0, end_pos = (INT)Str.length() - 1;

  // strip start of line
  while (start_pos <= end_pos && isspace((INT)(UCHAR)Str[start_pos]))
    start_pos++;
  // strip end of line
  while (start_pos <= end_pos && isspace((INT)(UCHAR)Str[end_pos]))
    end_pos--;

  if (start_pos > end_pos)
    return "";
  return Str.substr(start_pos, end_pos - start_pos + 1);
} /* End of 'Trim' function */

#endif /* __tam_def_h_ */

/* END OF 'tam_def.h' FILE */
