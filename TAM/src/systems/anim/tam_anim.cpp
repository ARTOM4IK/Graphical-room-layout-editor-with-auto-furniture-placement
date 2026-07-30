/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_anim.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Animation implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 25.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Render animtaion function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::anim::Render( VOID )
{
  if (IsRenderActive || !IsActive)
    return;
  CurrentFrameNo++;
  CurrentModelId = 1;
  // CurrentObjectId = 0;
  SystemProfiler.SetName("Main Animation Loop");
  SystemProfiler.Reset();
  SystemProfiler.Collect("Total Frame Time");
  IsRenderActive = TRUE;
  timer::Response();
  if (IsActive)
  {
    input::mouse::Wheel = win::MouseWheel;
    input::Response();
  }
  SystemProfiler.Collect("After timer/input");
  render::ResponseUI();
  SystemProfiler.Collect("After UIResponse");

  // Set UX host parameters
  UXHostW = W;
  UXHostH = H;
  UXHosthWnd = hWnd;
  UXHostIsFullScreen = IsFullScreen;
  if (!UXOnFlipFullScreen)
    UXOnFlipFullScreen = [this]( VOID )
    {
      FlipFullScreen();
    };

  // UX: Set up image cache callbacks on first frame
  if (!UXGetImages().LoadFn)
  {
    UXGetImages().LoadFn = [this]( const std::string &Path ) -> std::pair<VOID *, std::any>
    {
      image Img(Path);

      if (Path.find("icon_") != std::string::npos)
      {
        ImGuiStyle &style = ImGui::GetStyle();

        ImVec4 cc = style.Colors[ImGuiCol_Text];
        BYTE
          r = cc.x * 255,
          g = cc.y * 255,
          b = cc.z * 255;

        for (INT y = 0; y < Img.H; y++)
          for (INT x = 0; x < Img.W; x++)
          {
            auto col = Img.RowsD[y][x];

            if ((col & 0xFFFFFF) == 0xFFFFFF)
              col = 0x0;
            else
              col = COM_MAKELONG0123(b, g, r, ~(col & 0xFF));
            Img.RowsD[y][x] = col;
          }
      }
      ///tam::texture tam::texture_manager::TextureCreate( const std::string &NewName, INT NewW, INT NewH, INT BytesPerPixel, BYTE *Pixels, BOOL IsColor, BOOL IsMips, BOOL IsFiltered, BOOL IsAttachment )

      //  auto tex = TextureCreate(Path);
      auto tex = TextureCreate(Path, Img.W, Img.H, 4, Img.RowsB[0][0], TRUE, FALSE, FALSE, FALSE);
      if (tex && tex->Img.View != VK_NULL_HANDLE && tex->Sampler.Sampler != VK_NULL_HANDLE)
      {
        VkDescriptorSet ds =
          ImGui_ImplVulkan_AddTexture(tex->Sampler.Sampler,
            tex->Img.View, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        return {(VOID *)ds, std::any(tex)};
      }
      return {nullptr, {}};
    };
    UXGetImages().UnloadFn = []( VOID *Id )
    {
      if (Id != nullptr)
        ImGui_ImplVulkan_RemoveTexture((VkDescriptorSet)Id);
    };
  }
  UXResponse();
  SceneViewport = {UXSceneViewport.X, UXSceneViewport.Y, UXSceneViewport.W, UXSceneViewport.H};
  SystemProfiler.Collect("After UXResponse");

  CurrentScene.Response(this);
  SystemProfiler.Collect("After Scene Response");
  CurrentScene.Render(this);
  //SystemProfiler.Collect("After Scene Response");
  //SystemProfiler.Collect("Before Walls Draw");
  //for (auto &Wall : Editor.Walls)
  //  Draw(Wall.WallPrim, matr::Identity());
  //SystemProfiler.Collect("Walls Draw");
  SystemProfiler.Collect("After render scene");

  render::FrameStart();
  SystemProfiler.Collect("After frame start");

  render::FrameEnd();
  IsRenderActive = FALSE;
  SystemProfiler.Collect("End animation loop");
} /* End of 'tam::anim::Render' function */

/* WM_CREATE window message handle function.
 * ARGUMENTS:
 *   - structure with creation data:
 *       CREATESTRUCT *CS;
 * RETURNS:
 *   (BOOL) TRUE to continue creation window, FALSE to terminate.
 */
BOOL tam::anim::OnCreate( CREATESTRUCT *CS )
{
  SetTimer(win::hWnd, 30, 0, nullptr);
  if (!render::Init(win::hWnd))
    return FALSE;
  UXInit();
  return TRUE;
} /* End of 'tam::anim::OnCreate' function */

/* WM_DESTROY window message handle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::anim::OnDestroy( VOID )
{
  KillTimer(win::hWnd, 30);
  CurrentScene.Clear();
  CurrentScene.Response(this);
  UXClose();
  render::Close();
  PostQuitMessage(30);
} /* End of 'tam::anim::OnDestroy' function */

/* WM_SIZE window message handle function.
 * ARGUMENTS:
 *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
 *       UINT State;
 *   - new width and height of client area:
 *       INT W, H;
 * RETURNS: None.
 */
VOID tam::anim::OnSize( UINT State, INT W, INT H )
{
  if (W == 0 || H == 0)
  {
    IsActive = FALSE;
    return;
  }
  IsActive = TRUE;
  render::Resize(W, H);
  Render();
} /* End of 'tam::anim::OnSize' function */

/* WM_ERASEBKGND window message handle function.
 * ARGUMENTS:
 *   - device context of client area:
 *       HDC hDC;
 * RETURNS:
 *   (BOOL) TRUE if background is erased, FALSE otherwise.
 */
BOOL tam::anim::OnEraseBkgnd( HDC hDC )
{
  return TRUE;
} /* End of 'tam::anim::OnEraseBkgnd' function */

/* WM_PAINT window message handle function.
 * ARGUMENTS:
 *   - window device context:
 *       HDC hDC;
 *   - paint message structure pointer:
 *       PAINTSTRUCT *PS;
 * RETURNS: None.
 */
VOID tam::anim::OnPaint( HDC hDC, PAINTSTRUCT *PS )
{
  Render();
} /* End of 'tam::anim::OnPaint' function */

/* WM_TIMER window message handle function.
 * ARGUMENTS:
 *   - specified the timer identifier.
 *       INT Id;
 * RETURNS: None.
 */
VOID tam::anim::OnTimer( INT Id )
{
  Render();
} /* End of 'tam::anim::OnTimer' function */

/* WM_EXITSIZEMOVE window message handle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::anim::OnExitSizeMove( VOID )
{
  IsActive = TRUE;
} /* End of 'tam::anim::OnExitSizeMove' function */

/* WM_ENTERSIZEMOVE window message handle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::anim::OnEnterSizeMove( VOID )
{
  IsActive = FALSE;
} /* End of 'tam::anim::OnEnterSizeMove' function */

/* WM_MOUSEWHEEL window message handle function.
 * ARGUMENTS:
 *   - mouse window position:
 *       INT X, Y;
 *   - mouse wheel delta value (120 points per rotate):
 *       INT Z;
 *   - mouse keys bits (see MK_*** bits constants):
 *       UINT Keys;
 * RETURNS: None.
 */
VOID tam::anim::OnMouseWheel( INT X, INT Y, INT Z, UINT Keys )
{
  win::MouseWheel = Z;
} /* End of 'tam::anim::OnMouseWheel' function */

/* WM_*BUTTONUP window message handle function.
 * ARGUMENTS:
 *   - mouse window position:
 *       INT X, Y;
 *   - mouse keys bits (see MK_*** bits constants):
 *       UINT Keys;
 * RETURNS: None.
 */
VOID tam::anim::OnButtonUp( INT X, INT Y, UINT Keys )
{
  ReleaseCapture();
} /* End of 'tam::win::OnButtonUp' function */

/* WM_*BUTTONDOWN window message handle function.
 * ARGUMENTS:
 *   - double click flag:
 *       BOOL IsDoubleClick;
 *   - mouse window position:
 *       INT X, Y;
 *   - mouse keys bits (see MK_*** bits constants):
 *       UINT Keys;
 * RETURNS: None.
 */
VOID tam::anim::OnButtonDown( BOOL IsDoubleClick, INT X, INT Y, UINT Keys )
{
  SetCapture(win::hWnd);
} /* End of 'tam::anim::OnButtonDown' function */

/* Window message universal handle function.
 * Should be returned 'DefWindowProc' call result.
 * ARGUMENTS:
 *   - message type (see WM_***):
 *      UINT Msg;
 *   - message 'word' parameter:
 *      WPARAM wParam;
 *   - message 'long' parameter:
 *      LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message depende return value.
 */
LRESULT tam::anim::OnMessage( UINT Msg, WPARAM wParam, LPARAM lParam )
{
  return DefWindowProc(win::hWnd, Msg, wParam, lParam);
} /* End of 'tam::anim::OnMessage' function */

/* Main window handle loop function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::anim::Run( VOID )
{
  MSG msg;

  while (TRUE)
  {
    /* Check message at window message queue */
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
      if (msg.message == WM_QUIT)
        break;
      else
      {
        /* Displatch message to window */
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    else
      // SendMessage(hWnd, WM_TIMER, 30, 0);
      Render();
  }
} /* End of 'tam::anim::Run' function */

/** Units functions implementation module **/

/* Scene destructor */
tam::scene::~scene( VOID )
{
  for (auto &Uni: Units)
    delete Uni;
  Units.clear();
  for (auto &Uni: UnitsToAdd)
    delete Uni;
  UnitsToAdd.clear();
  UnitsToDel.clear();
} /* End of 'tam::scene::~scene' function */

/* Get response from all units function.
 * ARGUMENTS:
 *   - animation context pointer:
 *       anim *Ani;
 * RETURNS: None.
 */
VOID tam::scene::Response( anim *Ani )
{
  // Add a new units
  for (auto Uni: UnitsToAdd)
    Units.push_back(Uni);
  UnitsToAdd.clear();
  // Del marked units
  for (const auto Uni : UnitsToDel)
    if (auto it = std::find(Units.begin(), Units.end(), Uni); it != Units.end())
    {
      delete *it;
      Units.erase(it);
    }
  UnitsToDel.clear();

  for (auto Uni: Units)
    Uni->Response(Ani);
} /* End of 'tam::scene::Response' function */

/* Render scene function.
 * ARGUMENTS:
 *   - animation context pointer:
 *       anim *Ani;
 * RETURNS: None.
 */
VOID tam::scene::Render( anim *Ani )
{
  for (auto &Uni: Units)
    Uni->Render(Ani);
} /* End of 'tam::scene::Render' function */

/* Add unit into current scene function.
 * ARGUMENTS:
 *   - pointer to a new unit:
 *       unit *Uni;
 * RETURNS:
 *   (anim &) reference to itself.
 */
tam::anim & tam::anim::operator<<( unit *Uni )
{
  CurrentScene << Uni;
  return *this;
} /* End of 'tam::anim::operator<<' function */

/* Add unit into animation by name function.
 * ARGUMENTS:
 *   - unit to add name:
 *       const std::string &UnitName;
 * RETURNS:
 *   (anim &) reference to itself.
 */
tam::anim & tam::anim::operator<<( const std::string &UnitName )
{
  logger::info << std::format("Try to register unit: {}", UnitName);
  if (UnitNames.find(UnitName) != UnitNames.end())
    *this << UnitNames[UnitName](this);
  return *this;
} /* End of 'tam::anim::operator<<' function */

/* END OF 'tam_anim.cpp' FILE */
