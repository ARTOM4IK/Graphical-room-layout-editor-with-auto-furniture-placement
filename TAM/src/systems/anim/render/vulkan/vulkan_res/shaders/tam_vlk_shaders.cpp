/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_vlk_shaders.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan system module.
 *               Vulkan shader main functions impementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 28.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Load file to string function.
 * ARGUMENTS:
 *   - file name:
 *       const std::string &FileName;
 * RETURNS:
 *   (std::string) loaded text.
 */
std::string tam::vlk::shader::LoadTextFile( const std::string &FileName )
{
  std::ifstream f(FileName);
  return std::string((std::istreambuf_iterator<char>(f)),
                     std::istreambuf_iterator<char>());
} /* End of 'tam::vlk::shader::LoadTextFile' function */

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
VOID tam::vlk::shader::Log( const std::string &Name, const std::string &Stage, const std::string &Text )
{
  std::ofstream(VlkPtr->Path + "bin/shaders/~TAM_SHD.LOG", std::ios_base::app) <<
    Name << "\n" << Stage << ":\n" << Text << "\n";
} /* End of 'tam::vlk::shader::Log' function */

/* Binary SPIR-V file load to memory function.
 * ARGUMENTS:
 *   - SPIR-V file name:
 *       const std::string &FileName;
 * RETURNS:
 *   (VkShaderModule) load shader module.
 */
VkShaderModule tam::vlk::shader::LoadSPIRVFile( const std::string &FileName )
{
  std::ifstream f(FileName, std::ios::binary | std::ios::in);
  if (!f.is_open())
    return VK_NULL_HANDLE;
  f.seekg(0, std::ios::end);
  UINT flen = static_cast<UINT>(f.tellg());
  f.seekg(0, std::ios::beg);
  std::vector<UINT32> code;
  code.resize(flen / 4);
  f.read((CHAR *)code.data(), flen);
  f.close();
  static const UINT32 SPIRV_MAGIC = 0x07230203;
  if (code.size() == 0 || code[0] != SPIRV_MAGIC || flen % 4 != 0)
    return VK_NULL_HANDLE;
  VkShaderModuleCreateInfo ShaderModuleCreateInfo
  {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .codeSize = flen,
    .pCode = code.data(),
  };
  VkShaderModule ShaderModule;
  if (vkCreateShaderModule(VlkPtr->Device, &ShaderModuleCreateInfo, nullptr, &ShaderModule) != VK_SUCCESS)
    return VK_NULL_HANDLE;
  return ShaderModule;
} /* End of 'tam::vlk::shader::LoadSPIRVFile' function */

/* Load shader function.
 * ARGUMENTS:
 *   - shader directory name:
 *       const std::string &ShaderName;
 * RETURNS:
 *   (BOOL) TRUE if shader was loaded successfully, FALSE otherwise.
 */
BOOL tam::vlk::shader::Create( const std::string &ShaderName )
{
  ShaderDirName = ShaderName;
  struct
  {
    std::string Suff;          // Shader file name
    VkShaderModule &Module;    // Shader module
    std::string CompilerParam; // Shader compilation parameters
    std::string Define;        // Shader define
  } shdr[] =
  {
    {"comp", ShaderModuleComp, "comp", "COMPUTE_SHADER"},
    {"vert", ShaderModuleVert, "vert", "VERTEX_SHADER"},
    {"ctrl", ShaderModuleCtrl, "tesc", "CONTROL_SHADER"},
    {"eval", ShaderModuleEval, "tese", "EVALUATION_SHADER"},
    {"geom", ShaderModuleGeom, "geom", "GEOMETRY_SHADER"},
    {"frag", ShaderModuleFrag, "frag", "FRAGMENT_SHADER"},
  };
  BOOL isok = TRUE, IsComputeShd = FALSE;

  /* Load shaders */
  for (auto &s : shdr)
  {
    std::string fn = VlkPtr->Path + "bin\\shaders\\" + ShaderName + "\\" + s.Suff;
    std::fstream f(fn + ".glsl", std::fstream::in);
    if (f.is_open() && s.Suff == "comp")
      IsComputeShd = TRUE;
    /* Check for text file presence */
    if (!f.is_open())
    {
      if (s.Suff != "vert" && s.Suff != "frag")
        continue;
      else if (!IsComputeShd)
      {
        Log(ShaderName, s.Suff, "No file");
        logger::error << "Shader: " >> 0x88AAFF << ShaderName + "\\" + s.Suff << ".glsl" >> logger::DefFore << " wasn't loaded: no such file.\nPath: " + fn + ".glsl";
        isok = FALSE;
        break;
      }
    }
    f.close();

    // Compile shader
    std::string cmd = "glslc -D" + s.Define + "=1" + " -Ibin/shaders/includes -g -fshader-stage=" + s.CompilerParam + " -o " + fn + ".spv " + fn + ".glsl 2> compile.log";
    std::system(cmd.c_str());

    logger::debug_info << "Compiling shader: " >> 0x88AAFF << ShaderName << "\\" << s.Suff << ".glsl" >> logger::DefFore << "...";

    // Check for errors
    std::string err = LoadTextFile("compile.log");
    std::remove("compile.log");
    if (err != "")
    {
      Log(ShaderName, s.Suff, "Compile errors:\n" + err);
      isok = FALSE;
      logger::error << "Shader: " >> 0x88AAFF << ShaderName << "\\" << s.Suff << ".glsl" >> tam::logger::DefFore << " compilation error:\n" + err;
    }
    // Load shader copiled SPIR-V byte code
    if ((s.Module = LoadSPIRVFile(fn + ".spv")) == VK_NULL_HANDLE)
      if (s.Suff != "vert" && s.Suff != "frag")
        continue;
      else
      {
        Log(ShaderName, s.Suff, "SPIR-V load error");
        logger::error << "Shader: "  >> 0x88AAFF << ShaderName << "\\" << s.Suff << ".glsl" >> tam::logger::DefFore << " SPIR-V load error.";
        isok = FALSE;
        break;
      }
    if (IsComputeShd)
      break;
  }
  if (!isok)
  {
    for (auto s : shdr)
      if (s.Module != VK_NULL_HANDLE)
        vkDestroyShaderModule(VlkPtr->Device, s.Module, nullptr), s.Module = VK_NULL_HANDLE;
    return FALSE;
  }
  logger::info >> 0x88AAFF << ShaderName << logger::SetDefStr() << " shader was " >> 0x33FA88 << "loaded" >> logger::DefFore << " successfully.";
  return TRUE;
} /* End of 'tam::vlk::shader::Create' function */

/* Destroy Vulkan shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::vlk::shader::Destroy( VOID )
{
  if (VlkPtr->Device == VK_NULL_HANDLE)
    return;
  if (ShaderModuleVert != VK_NULL_HANDLE)
  {
    vkDestroyShaderModule(VlkPtr->Device, ShaderModuleVert, nullptr);
    ShaderModuleVert = VK_NULL_HANDLE;
  }
  if (ShaderModuleCtrl != VK_NULL_HANDLE)
  {
    vkDestroyShaderModule(VlkPtr->Device, ShaderModuleCtrl, nullptr);
    ShaderModuleCtrl = VK_NULL_HANDLE;
  }
  if (ShaderModuleEval != VK_NULL_HANDLE)
  {
    vkDestroyShaderModule(VlkPtr->Device, ShaderModuleEval, nullptr);
    ShaderModuleEval = VK_NULL_HANDLE;
  }
  if (ShaderModuleGeom != VK_NULL_HANDLE)
  {
    vkDestroyShaderModule(VlkPtr->Device, ShaderModuleGeom, nullptr);
    ShaderModuleGeom = VK_NULL_HANDLE;
  }
  if (ShaderModuleFrag != VK_NULL_HANDLE)
  {
    vkDestroyShaderModule(VlkPtr->Device, ShaderModuleFrag, nullptr);
    ShaderModuleFrag = VK_NULL_HANDLE;
  }
  if (ShaderModuleComp != VK_NULL_HANDLE)
  {
    vkDestroyShaderModule(VlkPtr->Device, ShaderModuleComp, nullptr);
    ShaderModuleComp = VK_NULL_HANDLE;
  }
} /* End of 'tam::vlk::shader::Destroy' function */

/* Reload Vulkan shader function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) TRUE if shader was reloaded successfully, FALSE otherwise.
 */
BOOL tam::vlk::shader::Update( VOID )
{
  logger::debug_info << "Try to " >> 0x33FA88 << "reload " >> 0x88AAFF << ShaderDirName >> logger::DefFore << " shader.";
  Destroy();
  return Create(ShaderDirName);
} /* End of 'tam::vlk::shader::Update' function */

/* END OF 'tam_vlk_shaders.cpp' FILE */
