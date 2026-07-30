/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ui.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               user interface handle declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 02.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Response input keys for user interface function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ui_manager::ResponseUI( VOID )
{
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
} /* End of 'ResponseUI' function */

/* Create user interface dependencies function.
 * ARGUMENTS: 
 *   - window context:
 *       HWND &hWnd.
 * RETURNS: None.
 */
VOID tam::ui_manager::CreateUI( HWND &hWnd )
{
  // Initialization ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui::StyleColorsDark();
  ImGuiStyle *style = &ImGui::GetStyle();
  ImVec4 *colors = style->Colors;

  style->Alpha = 0.9;
  style->WindowRounding = 2.0;
  style->FrameRounding = 1.0;
  style->PopupRounding = 2.0;
  style->ScrollbarRounding = 3.0;
  style->GrabRounding = 4.0;
  style->FrameBorderSize = 1.0;
  style->WindowBorderSize = 0.18;

  colors[ImGuiCol_Text]                  = ImVec4(0.90, 0.97, 1.00, 1.00);
  colors[ImGuiCol_TextDisabled]          = ImVec4(0.66, 0.66, 0.66, 1.00);
  colors[ImGuiCol_WindowBg]              = ImVec4(0.09, 0.16, 0.24, 1.00);
  colors[ImGuiCol_ChildBg]               = ImVec4(0.00, 0.00, 0.00, 0.08);
  colors[ImGuiCol_PopupBg]               = ImVec4(0.07, 0.13, 0.23, 1.00);
  colors[ImGuiCol_Border]                = ImVec4(0.20, 0.32, 0.51, 1.00);
  colors[ImGuiCol_BorderShadow]          = ImVec4( 0.0,  0.0,  0.0, 0.08);
  colors[ImGuiCol_FrameBg]               = ImVec4(0.04, 0.20, 0.44, 1.00);
  colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.15, 0.30, 0.54, 1.00);
  colors[ImGuiCol_FrameBgActive]         = ImVec4(0.20, 0.35, 0.60, 1.00);
  colors[ImGuiCol_TitleBg]               = ImVec4(0.03, 0.22, 0.40, 1.00);
  colors[ImGuiCol_TitleBgActive]         = ImVec4(0.11, 0.30, 0.54, 1.00);
  colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.09, 0.25, 0.46, 1.00);
  colors[ImGuiCol_MenuBarBg]             = ImVec4(0.05, 0.17, 0.34, 1.00);
  colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.05, 0.15, 0.29, 1.00);
  colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.20, 0.38, 0.63, 1.00);
  colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.22, 0.42, 0.69, 1.00);
  colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.25, 0.46, 0.74, 1.00);
  colors[ImGuiCol_CheckMark]             = ImVec4(0.12, 0.34, 0.66, 1.00);
  colors[ImGuiCol_SliderGrab]            = ImVec4(0.40, 0.65, 0.96, 1.00);
  colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.55, 0.75, 0.99, 1.00);
  colors[ImGuiCol_Button]                = ImVec4(0.10, 0.42, 0.70, 1.00);
  colors[ImGuiCol_ButtonHovered]         = ImVec4(0.15, 0.58, 0.93, 1.00);
  colors[ImGuiCol_ButtonActive]          = ImVec4(0.20, 0.49, 0.80, 1.00);
  colors[ImGuiCol_Header]                = ImVec4(0.15, 0.48, 0.80, 1.00);
  colors[ImGuiCol_HeaderHovered]         = ImVec4(0.20, 0.56, 0.90, 1.00);
  colors[ImGuiCol_HeaderActive]          = ImVec4(0.27, 0.64, 0.95, 1.00);
  colors[ImGuiCol_Separator]             = ImVec4(0.15, 0.38, 0.63, 1.00);
  colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.20, 0.56, 0.90, 1.00);
  colors[ImGuiCol_SeparatorActive]       = ImVec4(0.18, 0.50, 0.80, 1.00);
  colors[ImGuiCol_ResizeGrip]            = ImVec4(0.35, 0.68, 0.99, 1.00);
  colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.47, 0.74, 1.00, 1.00);
  colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.55, 0.80, 1.00, 1.00);
  colors[ImGuiCol_PlotLines]             = ImVec4(0.20, 0.45, 0.85, 1.00);
  colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.25, 0.52, 0.90, 1.00);
  colors[ImGuiCol_PlotHistogram]         = ImVec4(0.21, 0.50, 0.86, 1.00);
  colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.25, 0.58, 0.91, 1.00);
  colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.20, 0.42, 0.80, 0.18);
  colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00, 1.00, 0.00, 0.90);
  colors[ImGuiCol_NavHighlight]          = ImVec4(0.10, 0.10, 0.10, 0.08);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00, 1.00, 1.00, 0.74);
  colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80, 0.80, 0.80, 0.18);
  colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.20, 0.20, 0.20, 0.35);

  colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.15, 0.30, 0.55, 0.85);
  colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.20, 0.42, 0.75, 0.90);
  colors[ImGuiCol_TableBorderLight]      = ImVec4(0.15, 0.29, 0.54, 0.70);
  colors[ImGuiCol_TableRowBg]            = ImVec4(0.15, 0.33, 0.85, 0.18);
  colors[ImGuiCol_TableRowBgAlt]         = ImVec4(0.10, 0.25, 0.67, 0.18);

  // Initialization backends ImGui
  ImGui_ImplWin32_Init(hWnd);

  ImGui_ImplVulkan_InitInfo init_info = {};
  init_info.Instance = Rnd.Instance;
  init_info.PhysicalDevice = Rnd.PhysDevice;
  init_info.Device = Rnd.Device;
  init_info.QueueFamily = Rnd.GraphicsQueueFamilyIndex;
  init_info.Queue = Rnd.GraphicsQueue;
  init_info.PipelineCache = VK_NULL_HANDLE;
  init_info.DescriptorPool = Rnd.DescriptorPool;
  init_info.MinImageCount = 3;
  init_info.ImageCount = Rnd.SwapchainImagesCount;
  init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
  init_info.Allocator = nullptr;
  init_info.CheckVkResultFn = nullptr;
  init_info.RenderPass = Rnd.VlkRef->PostProcRenderPass.RenderPass;

  ImGui_ImplVulkan_Init(&init_info);
} /* End of 'CreateUI' function */

/* Render user interface in target function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ui_manager::RenderUI( VOID )
{
  // Render
  ImGui::Render();
} /* End of 'RenderUI' function */

/* Draw user interface on GPU function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ui_manager::DrawUI( VOID )
{
  // Draw ImGui data
  ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), Rnd.CommandBuffers[Rnd.CurrentFrame]);
} /* End of 'DrawUI' function */

/* Close user interface handle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ui_manager::CloseUI( VOID )
{
  vkDeviceWaitIdle(Rnd.Device);
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
} /* End of 'CloseUI' function */

/* END OF 'tam_ui.cpp' FILE */
