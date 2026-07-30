/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_font.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Font class implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 24.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_res_font.h"

/* Class constructor.
 * ARGUMENTS:
 *   - pointer to render:
 *       render *Rnd;
 */
tam::font_res::font_res( render *Rnd ) : resource(Rnd)
{
} /* End of 'tam::font_res::font_res' function */

/* Class move constructor.
 * ARGUMENTS:
 *   - rvalue reference:
 *       font_res &&Old;
 */
tam::font_res::font_res( font_res &&Old ) :
  resource(Old.Rnd),
  Glyphs(std::move(Old.Glyphs)),
  GlyphsScreen(std::move(Old.GlyphsScreen)),
  LineH(Old.LineH),
  BaseH(Old.BaseH),
  AdvanceX(Old.AdvanceX),
  Name(Old.Name)
{
  Old.Name = "";
  Old.LineH = 0;
} /* End of 'tam::font_res::font_res' function */

/* Class destructor */
tam::font_res::~font_res( VOID )
{
  Free();
} /* End of 'tam::font_res::~font_res' function */

/* Create font function.
 * ARGUMENTS:
 *   - font file name:
 *       const std::string &FontFileName;
 *   - font letters draw color:
 *       const vec4 &TextColor;
 * RETURNS:
 *   (font_res &) self reference to font resource.
 */
tam::font_res & tam::font_res::Create( const std::string &FontFileName, const vec4 &TextColor )
{
  std::ifstream F(Rnd->Path + "bin/fonts/" + FontFileName + ".g3df", std::ios::binary);

  Name = FontFileName;

  if (!F.is_open())
    return *this;

  DWORD Sign;
  F.read(reinterpret_cast<CHAR *>(&Sign), 4);
  if (Sign != "G3DF"_dword)
    return *this;

  // Read font file header
  F.read(reinterpret_cast<CHAR *>(&LineH), 4);
  F.read(reinterpret_cast<CHAR *>(&BaseH), 4);
  F.read(reinterpret_cast<CHAR *>(AdvanceX.data()), 256 * 4);

  /////////////////////////////////
  // Read glyphs vertices
  /////////////////////////////////

  // Font file vertex representation type
  struct font_file_vertex
  {
    vec3 P; // Vertex position
    vec2 T; // Vertex texture coordinates
    vec3 N; // Normal at vertex
    vec4 C; // Vertex color
  };
  std::array<std::vector<font_file_vertex>, 256> Chars;
  for (auto &c : Chars)
  {
    c.resize(4);
    F.read(reinterpret_cast<CHAR *>(c.data()), sizeof(font_file_vertex) * 4);
  }

  // Read font texture
  DWORD W, H;
  F.read(reinterpret_cast<CHAR *>(&W), 4);
  F.read(reinterpret_cast<CHAR *>(&H), 4);
  std::vector<DWORD> TexColors, TexColorsTr;

  TexColors.resize(W * H);
  TexColorsTr.resize(W * H);

  F.read(reinterpret_cast<CHAR *>(TexColors.data()), 4 * W * H);
  F.close();

  for (INT i = 0; i < W * H; i++)
    TexColorsTr[i] = TexColors[i] & 0xFF000000;

  // Create material/patterm
  std::string suffix = std::format(" <{:.5f},{:.5f},{:.5f},{:.5f}>", TextColor.X, TextColor.Y, TextColor.Z, TextColor.W);

  // Create texture
  texture TexBase = Rnd->TextureCreate("Font base texture: " + FontFileName + suffix, W, H, 4, reinterpret_cast<BYTE *>(TexColors.data()), TRUE, FALSE, FALSE);
  texture TexTr = Rnd->TextureCreate("Font transparent texture: " + FontFileName + suffix, W, H, 4, reinterpret_cast<BYTE *>(TexColors.data()), TRUE, FALSE, FALSE);

  material MtlScr =
    Rnd->MaterialCreate("font/screen:" + FontFileName + suffix, Rnd->MtlPatScreen, {TexBase}, {});
  material MtlDir =
    Rnd->MaterialCreate("font/direct:" + FontFileName + suffix, Rnd->MtlPatDirect, {TexBase}, {});

  for (INT i = 0; i < 256; i++)
  {
    for (auto &vrt : Chars[i])
      vrt.C = TextColor;

    Glyphs[i] =
      Rnd->PrimitiveCreate(MtlDir, std::span<font_file_vertex>(Chars[i]), {});
    GlyphsScreen[i] =
      Rnd->PrimitiveCreate(MtlScr, std::span<font_file_vertex>(Chars[i]), {});
  }
  return *this;
} /* End of 'tam::font_res::Create' function */

/* Free font function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::font_res::Free( VOID )
{
} /* End of 'tam::font_res::Free' function */

/* Draw string in screen space function.
 * ARGUMENTS:
 *   - string to draw:
 *       const std::string &Text;
 *   - draw position:
 *       const vec3 &Pos;
 *   - font size (0 - for original size, otherwise - size in pixels):
 *       FLT Size;
 * RETURNS: None.
 */
VOID tam::font_res::Draw( const std::string &Text, const vec3 &Pos, FLT Size )
{
  INT cpos = 0;
  vec3 p {Pos};
  FLT startx = Pos.X;
  const CHAR *Str = Text.c_str();
  if (Size == 0)
    Size = LineH;

  while (*Str != 0)
  {
    if (*Str == '\n')
      cpos = 0, p.X = startx, p.Y -= Size;
    else if (*Str == '\r')
      cpos = 0, p.X = startx;
    else if (*Str == '\t')
      p.X += (8 - (cpos & 7)) * AdvanceX[' '] * Size, cpos += (8 - (cpos & 7));
    else
      if (AdvanceX[(UCHAR)*Str] != 0)
      {
        Rnd->Draw(GlyphsScreen[(UCHAR)*Str], matr::Scale(vec3(Size, Size, 1)) * matr::Translate(p));
        p.X += AdvanceX[(UCHAR)*Str] * Size;
        cpos++;
      }
    Str++;
  }
} /* End of 'tam::font_res::Draw' function */

/* Draw string in 3D space function.
 * ARGUMENTS:
 *   - string to draw:
 *       const std::string &Text;
 *   - draw position:
 *       const vec3 &Pos;       
 *   - font size:
 *       FLT Size;
 * RETURNS: None.
 */
VOID tam::font_res::Draw3D( const std::string &Text, const vec3 &Pos, FLT Size )
{
  INT cpos = 0;
  vec3 p {Pos};
  FLT startx = Pos.X;
  const CHAR *Str = Text.c_str();
  if (Size == 0)
    Size = 1;

  while (*Str != 0)
  {
    if (*Str == '\n')
      cpos = 0, p.X = startx, p.Y -= Size;
    else if (*Str == '\r')
      cpos = 0, p.X = startx;
    else if (*Str == '\t')
      p.X += (8 - (cpos & 7)) * AdvanceX[' '] * Size, cpos += (8 - (cpos & 7));
    else
    {
      Rnd->Draw(Glyphs[(UCHAR)*Str], matr::Scale(vec3(Size, Size, 1)) * matr::Translate(p));
      p.X += AdvanceX[(UCHAR)*Str] * Size;
      cpos++;
    }
    Str++;
  }
} /* End of 'tam::font_res::Draw3D' function */

/* Draw string in billboard mode function.
 * ARGUMENTS:
 *   - string to draw:
 *       const std::string &Text;
 *   - draw position:
 *       const vec3 &Pos;
 *   - font size in space units:
 *       FLT Size;
 * RETURNS: None.
 */
VOID tam::font_res::DrawBB( const std::string &Text, const vec3 &Pos, FLT Size )
{
  INT cpos = 0;
  vec3 p {Pos};
  vec3 start = Pos;
  const CHAR *Str = Text.c_str();
  matr m(Rnd->Cam.Right.X, Rnd->Cam.Right.Y, Rnd->Cam.Right.Z, 0,
         Rnd->Cam.Up.X,    Rnd->Cam.Up.Y,    Rnd->Cam.Up.Z, 0,
         Rnd->Cam.Dir.X,   Rnd->Cam.Dir.Y,   Rnd->Cam.Dir.Z, 0,
         0, 0, 0, 1);
  if (Size == 0)
    Size = 1;

  while (*Str != 0)
  {
    if (*Str == '\n')
      cpos = 0, start -= Rnd->Cam.Up * Size, p = start;
    else if (*Str == '\r')
      cpos = 0, p = start;
    else if (*Str == '\t')
      p.X += (8 - (cpos & 7)) * AdvanceX[' '] * Size, cpos += (8 - (cpos & 7));
    else
    {
      Rnd->Draw(Glyphs[(UCHAR)*Str], matr::Scale(vec3(Size, Size, 1)) * m * matr::Translate(p));
      p += Rnd->Cam.Right * (AdvanceX[(UCHAR)*Str] * Size);
      cpos++;
    }
    Str++;
  }
} /* End of 'tam::font_res::DrawBB' function */

/* Draw static string in screen space function.
 * ARGUMENTS:
 *   - string to draw:
 *       const std::string &Text;
 *   - draw position:
 *       const vec3 &Pos;
 *   - font size (0 - for original size, otherwise - size in pixels):
 *       FLT Size;
 * RETURNS: None.
 */
VOID tam::font_res::DrawStatic( const std::string &Text, const vec3 &Pos, FLT Size )
{
  INT cpos = 0;
  vec3 p {Pos};
  FLT startx = Pos.X;
  const CHAR *Str = Text.c_str();
  if (Size == 0)
    Size = LineH;

  while (*Str != 0)
  {
    if (*Str == '\n')
      cpos = 0, p.X = startx, p.Y -= Size;
    else if (*Str == '\r')
      cpos = 0, p.X = startx;
    else if (*Str == '\t')
      p.X += (8 - (cpos & 7)) * AdvanceX[' '] * Size, cpos += (8 - (cpos & 7));
    else
    {
      Rnd->Draw(GlyphsScreen[(UCHAR)*Str], matr::Scale(vec3(Size, Size, 1)) * matr::Translate(p));
      p.X += AdvanceX[(UCHAR)*Str] * Size;
      cpos++;
    }
    Str++;
  }
} /* End of 'tam::font_res::DrawStatic' function */

/* Draw static string in 3D space function.
 * ARGUMENTS:
 *   - string to draw:
 *       const std::string &Text;
 *   - draw position:
 *       const vec3 &Pos;
 *   - font size:
 *       FLT Size;
 * RETURNS: None.
 */
VOID tam::font_res::DrawStatic3D( const std::string &Text, const vec3 &Pos, FLT Size )
{
  INT cpos = 0;
  vec3 p {Pos};
  FLT startx = Pos.X;
  const CHAR *Str = Text.c_str();
  if (Size == 0)
    Size = 1;

  while (*Str != 0)
  {
    if (*Str == '\n')
      cpos = 0, p.X = startx, p.Y -= Size;
    else if (*Str == '\r')
      cpos = 0, p.X = startx;
    else if (*Str == '\t')
      p.X += (8 - (cpos & 7)) * AdvanceX[' '] * Size, cpos += (8 - (cpos & 7));
    else
    {
      Rnd->Draw(Glyphs[(UCHAR)*Str], matr::Scale(vec3(Size, Size, 1)) * matr::Translate(p));
      p.X += AdvanceX[(UCHAR)*Str] * Size;
      cpos++;
    }
    Str++;
  }
} /* End of 'tam::font_res::DrawStatic3D' function */

/* Draw static string in billboard mode function.
 * ARGUMENTS:
 *   - string to draw:
 *       const std::string &Text;
 *   - draw position:
 *       const vec3 &Pos;
 *   - font size in space units:
 *       FLT Size;
 * RETURNS: None.
 */
VOID tam::font_res::DrawStaticBB( const std::string &Text, const vec3 &Pos, FLT Size )
{
  INT cpos = 0;
  vec3 p {Pos};
  vec3 start = Pos;
  const CHAR *Str = Text.c_str();
  matr m(Rnd->Cam.Right.X, Rnd->Cam.Right.Y, Rnd->Cam.Right.Z, 0,
         Rnd->Cam.Up.X, Rnd->Cam.Up.Y, Rnd->Cam.Up.Z, 0,
         Rnd->Cam.Dir.X, Rnd->Cam.Dir.Y, Rnd->Cam.Dir.Z, 0,
         0, 0, 0, 1);
  if (Size == 0)
    Size = 1;

  while (*Str != 0)
  {
    if (*Str == '\n')
      cpos = 0, start -= Rnd->Cam.Up * Size, p = start;
    else if (*Str == '\r')
      cpos = 0, p = start;
    else if (*Str == '\t')
      p.X += (8 - (cpos & 7)) * AdvanceX[' '] * Size, cpos += (8 - (cpos & 7));
    else
    {
      Rnd->Draw(Glyphs[(UCHAR)*Str], matr::Scale(vec3(Size, Size, 1)) * m * matr::Translate(p));
      p += Rnd->Cam.Right * (AdvanceX[(UCHAR)*Str] * Size);
      cpos++;
    }
    Str++;
  }
} /* End of 'tam::font_res::DrawStaticBB' function */

/* Free all fonts function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::font_manager::FreeFonts( VOID )
{
  //Free();
} /* End of 'tam::font_manager::FreeFonts' function */

/* Create font function.
 * ARGUMENTS:
 *   - font file name:
 *       const std::string &FontFileName;
 *   - font letters draw color:
 *       const vec4 &TextColor;
 * RETURNS:
 *   (font) self reference.
 */
tam::font tam::font_manager::FontCreate( const std::string &FontFileName, const vec4 &TextColor )
{
  if (IsFirstCreation)
  {
    IsFirstCreation = FALSE;
    shader ShdScr = RndRef.ShaderCreate("font/screen");
    shader ShdDir = RndRef.ShaderCreate("font/direct");

    MtlPatScreen =
      RndRef.MaterialPatternCreate(ShdScr, "font/screen", 1, 0, {}, {},
        {
          vlk::vertex_attribute::eVEC3,
          vlk::vertex_attribute::eVEC2,
          vlk::vertex_attribute::eVEC3,
          vlk::vertex_attribute::eVEC4,
        }, vlk::topology::STRIP, vlk::blend_mode::eTRANSPARENT);
    MtlPatDirect =
      RndRef.MaterialPatternCreate(ShdDir, "font/direct", 1, 0, {}, {},
        {
          vlk::vertex_attribute::eVEC3,
          vlk::vertex_attribute::eVEC2,
          vlk::vertex_attribute::eVEC3,
          vlk::vertex_attribute::eVEC4,
        }, vlk::topology::STRIP, vlk::blend_mode::eTRANSPARENT);
  }

  std::string res_name = FontFileName + std::format(" <{:.5f},{:.5f},{:.5f},{:.5f}>", TextColor.X, TextColor.Y, TextColor.Z, TextColor.W);
  font FntR = Allocate(res_name);
  if (FntR->Name == "")
    FntR->Create(FontFileName, TextColor);
  FntR->Name = res_name;
  return FntR;//font_res(&RndRef));//.Create(FontFileName, TextColor));
} /* End of 'tam::font_manager::FontCreate' function */

/* Class constructor.
 * ARGUMENTS:
 *   - render reference:
 *       render &RndRef;
 */
tam::font_manager::font_manager( render &RndRef ) : resource_manager(RndRef)
{
} /* End of 'tam::font_manager::font_manager' function */

/* END OF 'tam_res_font.cpp' FILE */
