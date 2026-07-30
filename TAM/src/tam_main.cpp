/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_main.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Main startup module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3),
 *               Arsentev Artemy (AA4),
 *               Nechaev Vladimir (VN4),
 *               Kupchenkov Ivan (IK4),
 *               Novozhilova Maryana (MN7),
 *               Tretyakov Andrey (AT7),
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 29.01.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Window message handle function (CALLBACK version).
 *   - window handle:
 *       HWND hWnd;
 *   - message identifier (see WM_***):
 *       UINT Msg;
 *   - message first parameter ('word' param):
 *       WPARAM wParam;
 *   - message second parameter ('long' param):
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message return code (depended to Msg type).
 */
static LRESULT CALLBACK TAM_MainWindowFunc( HWND hWnd, UINT Msg,
                                            WPARAM wParam, LPARAM lParam )
{
  switch (Msg)
  {
  case WM_CLOSE:
    // Vulkan destroy: Surface/Instance
    DestroyWindow(hWnd);
    return 0;
  case WM_DESTROY:
    PostQuitMessage(30);
    return 0;
  case WM_PAINT:
    // Vulkan render
    tam::SystemProfiler.SetName("TAM Main stream render pass");
    tam::SystemProfiler.Reset();
    tam::SystemProfiler.Collect("Full time");
    tam::SystemProfiler.Collect("After all");
    ValidateRect(hWnd, nullptr);
    return 0;
  case WM_SIZE:
    // Handle resize
    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      PostMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'TAM_MainWindowFunc' function */

#ifndef NDEBUG
#define tam_assert(Cond) ((!!(Cond)) || (tam_assert_func(#Cond, __FILE__, __LINE__), 0))
#else // NDEBUG
#define tam_assert(Cond) ((VOID)0)
#endif // NDEBUG

/* Main startup program function.
 * ARGUMENTS:
 *   - application instance handle:
 *       HINSTANCE hInstance;
 *   - previouse application instance handle
 *     (not used, alway NULL):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - command line window show parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) system error level value.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT CmdShow )
{
  tam::logger::SetColor(0x00FF00);
  tam::logger::out << "0x00FF00 COLOR\n";
  tam::logger::out << "DEFAULT COLOR\n";
  tam::logger::error << "THIS IS ERROR\n";
  tam::logger::info << "CGSG FOREVER!!!";
  tam::logger::warning << "THIS IS WARNING";
  tam::logger::sys << "CGSG FOREVER!!!";

  // Image handle test
  tam::logger::info << "Image handle tests";
  auto tam_assert_func =
    []( const CHAR *Cond, const CHAR *File, INT Line )
    {
      tam::logger::error << std::format("Error: ASSERT condition: {} in \"{}\", line {}", Cond, File, Line);
      throw std::runtime_error("Assertion failed");
    };
  try
  {
    tam::image Im;
    std::vector<std::string> flist
    {
      "x:/pics/m.g24",
      "x:/pics/gir.g32",
      "x:/pics/m.png",
      "x:/pics/cgsg.bmp",
      "x:/pics/Parrot.jpg",
    };

    for (auto &fn : flist)
    {
      std::cout << "Image: " << fn << " ... ";
      Im.Create(0, 0);
      Im.Load(fn);
      tam_assert(Im.W != 0 && Im.H != 0);
      Im.Create(0, 0);
      std::cout << "done\n";
    }
    for (auto &fn : flist)
    {
      std::cout << "Memory image: " << fn << " ... ";
      std::vector<BYTE> fdata;
      std::ifstream F(fn, std::ios::binary);
      if (F)
      {
        F.seekg(0, std::ios::end);
        fdata.resize(static_cast<SIZE_T>(F.tellg()));
        F.seekg(0, std::ios::beg);
        F.read(reinterpret_cast<CHAR *>(fdata.data()), fdata.size());
        F.close();
        Im.Create(0, 0);
        Im.Set(fdata);
        tam_assert(Im.W != 0 && Im.H != 0);
        Im.Create(0, 0);
        std::cout << "done\n";
      }
      else
        std::cout << "not found\n";
    }
    Im.Load("x:/pics/cgsg.bmp");
    Im.Save("a.jpg");
    Im.Save("a.bmp");
    Im.Save("a.png");
    Im.Save("a.g24");
    Im.Save("a.g32");
    Im.Load("a.jpg");
    tam_assert(Im.W != 0 && Im.H != 0);
    Im.Load("a.bmp");
    tam_assert(Im.W != 0 && Im.H != 0);
    Im.Load("a.png");
    tam_assert(Im.W != 0 && Im.H != 0);
    Im.Load("a.jpg");
    tam_assert(Im.W != 0 && Im.H != 0);
    Im.Load("a.g24");
    tam_assert(Im.W != 0 && Im.H != 0);
    Im.Load("a.g32");
    tam_assert(Im.W != 0 && Im.H != 0);
    std::filesystem::remove("a.bmp");
    std::filesystem::remove("a.png");
    std::filesystem::remove("a.jpg");
    std::filesystem::remove("a.g24");
    std::filesystem::remove("a.g32");
  }
  catch ( std::exception &Ex )
  {
    tam::logger::out << tam::logger::SetColorStr(0xFFAA00) << "Img::Exc:" << Ex.what() << "\n" << tam::logger::SetDefStr();
  }
  catch ( ... )
  {
    tam::logger::out << tam::logger::SetColorStr(0xFFAA00)  << "Img::Exc unknown\n" << tam::logger::SetDefStr();
  }

  // Window class register
  ///WNDCLASS wc {};
  ///CHAR MainWindowClassName[] = "CGSG-TAM Project Window Class";
  ///CHAR MainWindowTitle[] = "CGSG-TAM Project :: CGSG'Sr'2025";
  ///
  ///wc.lpfnWndProc = TAM_MainWindowFunc;
  ///wc.hInstance = hInstance;
  ///wc.lpszClassName = MainWindowClassName;
  ///wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  ///wc.hIcon = LoadIcon(hInstance, (CHAR *)TAM_IDI_LOGO);

  ///RegisterClass(&wc);

  ///// Создание окна
  ///HWND hWnd = CreateWindow(MainWindowClassName,
  ///  MainWindowTitle,
  ///  WS_OVERLAPPEDWINDOW,
  ///  CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
  ///  nullptr, nullptr, hInstance, nullptr);
  ///if (hWnd == nullptr)
  ///  return 0;

  // Vulkan init here: tam::vlk::Create(hWnd);
  //ShowWindow(hWnd, CmdShow);

  tam::logger::info << "Vulkan init start";
  //try
  //{
  //  tam::vlk::core Andrew;
  //  Andrew.Create(hWnd, 102, 47);
  //}
  //catch ( std::exception &Ex )
  //{
  //  tam::logger::out << "Exc:" << Ex.what() << "\n";
  //}
  //catch ( ... )
  //{
  //  tam::logger::out << "Exc unknown\n";
  //}
  tam::logger::info << "Vulkan init end";

  // Main message loop
  //MSG msg {};
  //while (TRUE)
  //{
  //  if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
  //  {
  //    if (msg.message == WM_QUIT)
  //      break;
  //    TranslateMessage(&msg);
  //    DispatchMessage(&msg);
  //  }
  //  // Vulkan render here
  //}
  //// Vulkan deinit here: tam::vlk::Destroy();
  //MessageBox(NULL, "Fin", "TAM", MB_ICONINFORMATION | MB_OK);

  tam::anim MyAnim;
  MyAnim.win::Create();
  //MyAnim << "AT7 test unit" << "Control" << "FontTest" << "MipTest";
  MyAnim << "Control"; // << "Garniture setting test unit" << "VG4 ux test"; // << "AT7 test unit" << "MipTest" << "Teapot" << "FurnitureEditor"; // "MipTest" << "Teapot"; // "Sphereflake"; // "Teapot"; // "Sphereflake" /*<< "Teapot" << "MipTest"
  //MyAnim << "TestMatr";
  //if (GetEnvironmentVariableA("TAM_LAYOUT_SCIENCE", nullptr, 0) > 0)
  //  MyAnim << "Layout science unit";
  //else
  //  MyAnim << "Editor unit";
  MyAnim << "Layout science unit";
  //MyAnim << "VG4 editor dispatcher";
  MyAnim.Run();

  return 30;
} /* End of 'WinMain' function */

/* END OF 'tam_main.cpp' FILE */
