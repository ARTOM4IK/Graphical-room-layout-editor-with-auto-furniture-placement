/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_textures.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Textures structure implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 14.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "systems/anim/render/res/textures/tam_res_textures.h"

/* Class constructor */
tam::texture_res::texture_res( render *RndPtr ) : resource(RndPtr), Img(RndPtr), Sampler(RndPtr)
{
} /* End of 'texture' function */

/* Class move constructor */
tam::texture_res::texture_res( texture_res &&Old ) :
  resource(Old.Rnd),
  Name(Old.Name), W(Old.W), H(Old.H), Img(Old.Img), Sampler(Old.Sampler)
{
  Old.Img = VK_NULL_HANDLE;
  
  //Old.Rnd= nullptr;
  Old.Name = "";
  Old.W = 0;
  Old.H = 0;
} /* End of 'texture' function */

/* Create texture function.
 * ARGUMENTS:
 *   - texture name:
 *      const std::string &NewName;
 *   - texture size:
 *      INT NewW, INT NewH;
 *   - pixels:
 *      INT BytesPerPixel, BYTE *Pixels;
 *   - color/depth flag:
 *      BOOL IsColor;
 *   - is mips flag:
 *      BOOL IsMips;
 *   - use srgb filter flag:
 *      BOOL Is Filtered;
 *   - is attachment flag:
 *      BOOL IsAttachment;
 * RETURNS:
 *   (texture_res &) texture.
 */
tam::texture_res & tam::texture_res::Create( const std::string &NewName, INT NewW, INT NewH, INT BytesPerPixel, BYTE *Pixels, BOOL IsColor, BOOL IsMips, BOOL IsFiltered, BOOL IsAttachment )
{
  VkFormat Format;
  W = NewW;
  H = NewH;
  Name = NewName;

  if (IsFiltered)
  {
    Format = VK_FORMAT_B8G8R8A8_SRGB;
    if (BytesPerPixel == 3)
      Format = VK_FORMAT_R8G8B8_SRGB;
    else if (BytesPerPixel == 1)
      Format = VK_FORMAT_R8_SRGB;
  }
  else
  {
    Format = VK_FORMAT_B8G8R8A8_UNORM;
    if (BytesPerPixel == 3)
      Format = VK_FORMAT_R8G8B8_UNORM;
    else if (BytesPerPixel == 1)
      Format = VK_FORMAT_R8_UNORM;
  }

  INT MipCount = 1;
  if (IsMips)
  {
    //MipCount = log(W > H ? W : H) / log(2.0), MipCount = MipCount < 1 ? 1 : MipCount;
    INT MaxDim = W > H ? W : H;
    MipCount = (INT)(log((DBL)MaxDim) / log(2.0)) + 1;
    if (MipCount < 1)
      MipCount = 1;
  }

  if (!IsColor)
    Img.Create(W, H, FALSE, FALSE, VK_FORMAT_D32_SFLOAT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_LAYOUT_UNDEFINED, nullptr, 0, 1);
  else
    if (IsAttachment)
      Img.Create(W, H, TRUE, FALSE, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_IMAGE_LAYOUT_UNDEFINED, Pixels, W * H * BytesPerPixel, MipCount);
    else
      Img.Create(W, H, TRUE, FALSE, Format, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_IMAGE_LAYOUT_UNDEFINED, Pixels, W * H * BytesPerPixel, MipCount);
  Sampler = Rnd->VlkRef->DefSampler;
  return *this;
} /* End of 'tam::texture_res::Create' funtcion */

/* Create texture function.
 * ARGUMENTS:
 *   - texture name:
 *      const std::string &FileName;
 * RETURNS:
 *   (texture_res &) texture.
 */
tam::texture_res & tam::texture_res::Create( const std::string &FileName )
{
  tam::image img(FileName);
 
  return Create(FileName, img.W, img.H, 4, img.RowsB[0][0], TRUE, TRUE);
} /* End of 'tam::texture_res::Create' funtcion */

/* Create cubemap texture function.
 * ARGUMENTS:
 *   - texture name:
 *      const std::string &TextureName;
 * RETURNS:
 *   (texture_res &) cubemap texture.
 */
tam::texture_res & tam::texture_res::CreateCubeMap( const std::string &TextureName )
{
  std::string CurrentType = "-";
  const std::vector<std::string> Names = 
  {
    "xpos", "xneg", "ypos", "yneg", "zpos", "zneg"
  };

  const std::vector<std::string> TypeImg = 
  {
    "bmp", "png", "g24", "g32", "tga", "dds", "jpg"
  };

  for (const auto &type : TypeImg)
  {
    std::string path = TextureName + "/xpos." + type;
    image img(path);
    if (!(img.W == 0 || img.H == 0 || img.RowsB.empty() || img.RowsB[0] == nullptr))
    {
      CurrentType = type;
      break;
    }
  }

  if (CurrentType == "-")
  {
    tam::logger::error << "Invalid type of cubemap";
    return *this;
  }

  //BYTE* ImageData = nullptr;
  std::vector<BYTE> Cubemap;

  for (INT i = 0; i < 6; i++) 
  {
    image img(TextureName + "/" + Names[i] + "." + CurrentType);
    W = img.W;
    H = img.H;
    if (!Cubemap.size())
      Cubemap.resize(W * H * 24);
    CopyMemory(Cubemap.data() + W * H * 4 * i, img.RowsB[0][0], W * H * 4);
    //memcpy(ImageData + (sizeof(img.RowsB[0][0]) * i), img.RowsB[0][0], sizeof(img.RowsB[0][0]));
  }
  Img.Create(W, H, TRUE, TRUE, VK_FORMAT_B8G8R8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_IMAGE_LAYOUT_UNDEFINED, /*ImageData*/Cubemap.data(), W * H * 24, 1);

  return *this;
} /* End of 'tam::texture_res::CreateCubeMap' function */

/* Create 1x1 texture function.
 * ARGUMENTS:
 *   - color vector:
 *      vec4 &Color;
 * RETURNS:
 *   (texture &) texture.
 */
tam::texture_res & tam::texture_res::Create( vec4 Color )
{
  W = 1;
  H = 1;
  Name = std::format(" <{:.5f},{:.5f},{:.5f},{:.5f}>", Color.X, Color.Y, Color.Z, Color.W);  
  std::vector<BYTE> Pix = {(BYTE)Color.X, (BYTE)Color.Y, (BYTE)Color.Z, (BYTE)Color.W};
  Img.Create(W, H, TRUE, FALSE, VK_FORMAT_B8G8R8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |  VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_IMAGE_LAYOUT_UNDEFINED, Pix.data(), W * H * 4, 1);

  return *this;
} /* End of 'tam::texture_res::Create' function */

/* Create texture function.
 * ARGUMENTS:
 *   - texture name:
 *      const std::string &NewName;
 *   - vulkan image reference:
 *      image Image;
 * RETURNS:
 *   (texture &) texture.
 */
tam::texture_res & tam::texture_res::Create( const std::string &NewName, vlk::image &Image )
{
  Name = NewName;
  W = Image.W;
  H = Image.H;
  Img = Image;
  Sampler = Rnd->VlkRef->DefSampler;

  return *this;
} /* End of 'tam::texture_res::TextureCreate' function */

/* Free texture function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::texture_res::Free( VOID )
{
  Name = "";
  W = 0;
  H = 0;
  Img.Destroy();
} /* End of 'tam::texture_res::Free' funtcion */

/* Rewrite image in texture without deleting function.
 *   - New vulkan image:
 *      const vlk::image &NewImage;
 * RETURNS: None.
 */
VOID tam::texture_res::RewriteImage( const vlk::image &NewImage )
{
  Img = NewImage;
} /* End of 'tam::texture_res::RewriteImage' function */

/* Get vec4 color from texture function.
 * ARGUMENTS:
 *   - Point position:
 *      const vec2 &Pos;
 * RETURNS:
 *   (vec4) color.
 */
tam::vec4 tam::texture_res::GetColor( const vec2 &Pos )
{
  UINT size = W * H * 4;
  Data.resize(size);

  Img.GetData(Data.data(), size);

  if (Pos.X < 0 || Pos.Y < 0 || Pos.X >= W || Pos.Y >= H)
    return vec4(0);

  UINT idx = (Pos.Y * W + Pos.X) * 4;

  UINT8 b = Data[idx + 0];
  UINT8 g = Data[idx + 1];
  UINT8 r = Data[idx + 2];
  UINT8 a = Data[idx + 3];

  return vec4(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
} /* End of 'tam::texture_res::GetColor' function */

/* Create texture function.
 * ARGUMENTS:
 *   - texture name:
 *      const std::string &NewName;
 * RETURNS:
 *   (texture *) pointer to texture.
 */
tam::texture tam::texture_manager::TextureCreate( const std::string &FileName, BOOL IsFiltered )
{
  texture Tex = Allocate(FileName);
  if (Tex->Name == "")
  {
    image img(FileName);
    
    if (img.W != 0 && img.H != 0 && !img.RowsB.empty() && img.RowsB[0] != nullptr)
    {    
      Tex->Create(FileName, img.W, img.H, 4, reinterpret_cast<BYTE*>(img.RowsB[0]), TRUE, TRUE, IsFiltered);
      return Tex;
    }
    return nullptr;
  }
  return Tex;
} /* End of 'tam::texture::TextureCreate' funtcion */

/* Create 1x1 texture function.
 * ARGUMENTS:
 *   - color vector:
 *      vec4 &Color;
 * RETURNS:
 *   (texture *) pointer to texture.
 */
tam::texture tam::texture_manager::TextureCreate( vec4 Color )
{
  texture Tex = Allocate(std::format(" <{:.5f},{:.5f},{:.5f},{:.5f}>", Color.X, Color.Y, Color.Z, Color.W));

  if (Tex->Name == "")
    Tex->Create(Color);
  return Tex;
} /* End of 'tam::texture::TextureCreate' funtcion */

/* Create texture cubemap function.
 * ARGUMENTS:
 *   - texture name:
 *      const std::string &NewName;
 * RETURNS:
 *   (texture *) pointer to texture.
 */
tam::texture tam::texture_manager::TextureCreateCubeMap( const std::string &FileName )
{
  texture Tex = Allocate(FileName);

  if (Tex->Name == "")
    Tex->CreateCubeMap(FileName);
  return Tex;
} /* End of 'tam::texture::TextureCreateCubeMap' funtcion */

/* Create texture function.
 * ARGUMENTS:
 *   - texture name:
 *      const std::string &NewName;
 *   - texture size:
 *      INT NewW, INT NewH;
 *   - pixels:
 *      INT BytesPerPixel, BYTE *Pixels;
 *   - color/depth flag:
 *      BOOL IsColor;
 *   - is mips flag:
 *      BOOL IsMips;
 *   - use srgb filter flag:
 *      BOOL Is Filtered;
 *   - is attachment flag:
 *      BOOL IsAttachment;
 * RETURNS:
 *   (texture *) pointer to texture.
 */
tam::texture tam::texture_manager::TextureCreate( const std::string &NewName, INT NewW, INT NewH, INT BytesPerPixel, BYTE *Pixels, BOOL IsColor, BOOL IsMips, BOOL IsFiltered, BOOL IsAttachment )
{
  texture Tex = Allocate(NewName);

  if (Tex->Name == "")
    Tex->Create(NewName, NewW, NewH, BytesPerPixel, Pixels, IsColor, IsMips, IsFiltered, IsAttachment);
  return Tex;
} /* End of 'tam::texture::TextureCreate' funtcion */

/* Create texture function.
 * ARGUMENTS:
 *   - texture name:
 *      const std::string &NewName;
 *   - vulkan image reference:
 *      image Image;
 * RETURNS:
 *   (texture *) pointer to texture.
 */
tam::texture tam::texture_manager::TextureCreate( const std::string &NewName, vlk::image &Image )
{
  texture Tex = Allocate(NewName);

  if (Tex->Name == "")
    Tex->Create(NewName, Image);
  return Tex;
} /* End of 'tam::texture_manager::TextureCreate' function */

/* END OF 'tam_res_textures.cpp' FILE */
