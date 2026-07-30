/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX system main include file.
 *               Pulls in core, tab, images, and all control types.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 18.04.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ux_h_
#define __tam_ux_h_

#include "tam_ux_tab.h"
#include "tam_ux_images.h"
#include "utils/imgui/imgui.h"

/* Project namespace */
namespace tam
{
  /* UX system namespace */
  namespace ux
  {
    /* UX core manager class.
     * Base class for animation system, manages panel layout,
     * vertical tabs, main menu, fonts, image cache, and all
     * registered UX controls.
     */
    class core
    {
      friend class tab;
    private:
      // Registered tabs
      std::vector<tab *> Tabs {};  // All registered tabs
      INT ActiveTabIndex {};       // Currently selected tab index

      // Panel layout state
      FLT PanelWidth = 400.0f;               // UX panel current width
      FLT PanelMinWidth = 200.0f;            // UX panel minimal width
      FLT PanelMaxWidthRatio = 2.0f / 3.0f;  // UX panel width ratio (from window width)
      BOOL IsPanelOnRight = FALSE;           // UX panel placement flag
      BOOL IsDraggingSplitter = FALSE;       // UX panel split bar dragging flag
      BOOL IsAnyPopupOpen = FALSE;           // UX popup (dialog) open flag
      FLT TabStripWidth = 38.0f;             // UX panel tab border line width

      // Font management (tam_ux_fonts.cpp)
      std::vector<ImFont *> LoadedFonts {};  // Loaded system fonts
      std::vector<std::string> FontNames {}; // Loaded system font names
      INT SelectedFontIndex = 1;             // Current font number
      FLT FontSize = 18.0f;                  // Current font size
      BOOL IsFontsLoaded = FALSE;            // Font load flag

      // Style state
      INT SelectedStyleIndex = 0;            // UI Style index
      ImGuiStyle SavedDarkStyle {};          // Store dark style for progress color correction
      BOOL IsStyleSaved = FALSE;             // Store style flag

      // Menu state (tam_ux_menu.cpp)        // About dialog show flag
      BOOL ShowAboutModalFlag = FALSE;

      // Image / icon cache (tam_ux_images.cpp)
      image_cache Images {};
      std::map<tab *, VOID *> TabIconTexIDs {}; // Loaded tab icon descriptors
      VOID *LogoTexID {};                       // Logo for menu bar and About dialog
      BOOL IsLogoLoaded = FALSE;

      /***
       * Panel / tab drawing (tam_ux.cpp)
       ***/

      /* Draw vertical tab strip function.
       * ARGUMENTS:
       *   - available height in pixels:
       *       FLT Height;
       * RETURNS: None.
       */
      VOID DrawVerticalTabs( FLT Height );

      /* Draw currently active tab content region function.
       * ARGUMENTS:
       *   - content width (0 = fill remaining):
       *       FLT Width;
       *   - content height:
       *       FLT Height;
       * RETURNS: None.
       */
      VOID DrawTabContent( FLT Width, FLT Height );

      /* Draw rotated text glyph-by-glyph via ImDrawList function.
       * ARGUMENTS:
       *   - ImGui draw list pointer:
       *       ImDrawList *DrawList;
       *   - text to draw:
       *       const CHAR *Text;
       *   - pivot position:
       *       ImVec2 Pos;
       *   - rotation angle in radians:
       *       FLT Angle;
       *   - text color:
       *       ImU32 Color;
       * RETURNS: None.
       */
      static VOID DrawTextRotated( ImDrawList *DrawList, const CHAR *Text,
                                   ImVec2 Pos, FLT Angle, ImU32 Color );

      /***
       * Menu drawing (tam_ux_menu.cpp)
       ***/

      /* Draw main application menu bar function.
       * ARGUMENTS:
       *   - output menu bar height:
       *       FLT &MenuBarHeight;
       * RETURNS: None.
       */
      VOID DrawMainMenu( FLT &MenuBarHeight );

      /* Draw About TAM modal dialog function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DrawAboutModal( VOID );

      /***
       * Font loading (tam_ux_fonts.cpp)
       ***/

      /* Load all system fonts into ImGui atlas function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID LoadFonts( VOID );

      /* Load a single system font by file name function.
       * ARGUMENTS:
       *   - font file name without extension:
       *       const CHAR *FontName;
       *   - registry display name map:
       *       const std::map<std::string, std::string> &DisplayNames;
       * RETURNS: None.
       */
      VOID LoadSystemFont( const CHAR *FontName,
                           const std::map<std::string, std::string> &DisplayNames );

      /* Read font display names from Windows registry function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (std::map<std::string, std::string>) lowercase filename -> display name.
       */
      std::map<std::string, std::string> ReadFontDisplayNames( VOID );

      /* Store all program user data to register function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID RegStoreAll( VOID );

      /* Restore all program user data from register function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID RegRestoreAll( VOID );

    public:
      // Host frame parameters (set by anim each frame)
      INT UXHostW {}, UXHostH {};                        // Window client area size
      HWND UXHosthWnd {};                                // Window handle
      BOOL UXHostIsFullScreen {};                        // Full screen mode flag
      std::function<VOID( VOID )> UXOnFlipFullScreen {}; // Toggle fullscreen callback

      // Viewport output (read by anim after UXResponse)
      struct
      {
        FLT
          X {}, Y {}, // Viewport origin (lower left)
          W {}, H {}; // Viewport size
      } UXSceneViewport {};

      // Markup guidelines primitive
      prim Guidelines;
      BOOL DrawGuidelines = TRUE;

      /* Default constructor */
      core( VOID )
      {
      } /* End of 'core' function */

      /* Type destructor */
      virtual ~core( VOID )
      {
      } /* End of '~core' function */

      /* Register tab in core function.
       * ARGUMENTS:
       *   - pointer to tab:
       *       tab *Tab;
       * RETURNS: None.
       */
      VOID RegisterTab( tab *Tab );

      /* Unregister tab from core function.
       * ARGUMENTS:
       *   - pointer to tab:
       *       tab *Tab;
       * RETURNS: None.
       */
      VOID UnregisterTab( tab *Tab );

      /* Initialize UX system function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID UXInit( VOID );

      /* Close UX system function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID UXClose( VOID );

      /* UX system response - layout panel, draw menu, tabs and controls function.
       * Call during ImGui frame, before unit Response calls.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID UXResponse( VOID );

      /* UX system render function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID UXRender( VOID );

      /* Get currently active tab index function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (INT) active tab index.
       */
      INT GetActiveTabIndex( VOID ) const
      {
        return ActiveTabIndex;
      } /* End of 'GetActiveTabIndex' function */

      /* Set currently active tab index function.
       * ARGUMENTS: None.
       *   - new active tab iindex:
       *       INT NewActiveIndex;
       * RETURNS: None.
       */
      VOID SetActiveTabIndex( INT NewActiveIndex )
      {
        if (NewActiveIndex >= 0 && NewActiveIndex < Tabs.size())
          ActiveTabIndex = NewActiveIndex;
      } /* End of 'SetActiveTabIndex' function */

      /* Get total number of registered tabs function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (INT) tab count.
       */
      INT GetTabCount( VOID ) const
      {
        return (INT)Tabs.size();
      } /* End of 'GetTabCount' function */

      /* Get image cache reference function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (image_cache &) image cache.
       */
      image_cache & UXGetImages( VOID )
      {
        return Images;
      } /* End of 'UXGetImages' function */

      /***
       * Dialog handling (tam_ux_dialog.cpp)
       ***/

    private:
      /* Confirm dialog parameter data type */
      struct prompt_state
      {
        bool IsActive = FALSE;                    // Dialog active (show) flag
        BOOL IsEdit = FALSE;                      // Edit line flag
        std::string Title, Message;               // Text parameters
        std::string *EditText;                    // Text parameters
        std::vector<std::string> Buttons;         // Button texts
        std::function<VOID ( INT )> CallbackFunc; // Callback function
        CHAR EditLineBuffer[300];                 // Edit line buffer
      }; /* End of 'prompt_state' structure */

      // Current confirm state data
      prompt_state DialogState;

      /* Render active dialog function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID ShowConfirmDialog( VOID )
      {
        if (!DialogState.IsActive)
          return;

        auto dlg_name = std::format("{}##DialogUX", DialogState.Title);
        ImGui::OpenPopup(dlg_name.c_str());

        // Center window
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal(dlg_name.c_str(), &DialogState.IsActive,
              ImGuiWindowFlags_AlwaysAutoResize))
        {
          ImGui::Text("%s", DialogState.Message.c_str());
          ImGui::Separator();
          if (DialogState.IsEdit)
          {
            ImGui::InputText("##EditLine", DialogState.EditLineBuffer, sizeof(DialogState.EditLineBuffer),
              ImGuiInputTextFlags_None /* | ImGuiInputTextFlags_EnterReturnsTrue */);
            ImGui::Separator();
          }
          // Buttons handle
          INT no = 0;
          for (auto b : DialogState.Buttons)
          {
            if (no != 0)
              ImGui::SameLine();
            if (ImGui::Button(b.c_str(), ImVec2(120, 0)))
            {
              if (DialogState.IsEdit && DialogState.EditText != nullptr)
                *DialogState.EditText = DialogState.EditLineBuffer;
              if (DialogState.CallbackFunc)
                DialogState.CallbackFunc(no);
              DialogState.IsActive = FALSE;
              ImGui::CloseCurrentPopup();
            }
            no++;
          }
          ImGui::EndPopup();
        }
      } /* End of 'ShowConfirmDialog' function */

    public:

      /* Confirm dialog handle function.
       * Usage:
       *   UXConfirm("Exit", "Sure?", []( BOOL IsConfirmResult )
       *   {
       *     if (IsConfirmResult)
       *        DoForYes();
       *     else
       *        DoForNo();
       *   });
       * ARGUMENTS:
       *   - confirm title:
       *       const std::string &Title;
       *   - confirm message:
       *       const std::string &Message;
       *   - confirm callback function (with BOOL argument):
       *       const func_type &Func;
       *   - confirm button text "yes":
       *       const std::string &YesText;
       *   - confirm button text "No":
       *       const std::string &YesText;
       * RETURNS: None.
       */
      template<typename func_type>
        VOID UXConfirm( const std::string &Title, const std::string &Message,
                        const func_type &Func,
                        const std::string &YesText = "Yes", const std::string &NoText = "No" )
        {
          DialogState =
          {
            .IsActive = TRUE,
            .IsEdit = FALSE,
            .Title = Title,
            .Message = Message,
            .Buttons = {YesText, NoText},
            .CallbackFunc = Func,
          };
        } /* End of 'UXConfirm' function */

      /* Confirm dialog handle function.
       * Usage:
       *   UXEdit("Exit", "Sure?", []( BOOL IsConfirmResult )
       *   {
       *     if (IsConfirmResult)
       *        DoForYes();
       *     else
       *        DoForNo();
       *   });
       * ARGUMENTS:
       *   - confirm title:
       *       const std::string &Title;
       *   - confirm message:
       *       const std::string &Message;
       *   - string to be edit:
       *       std::string &EditString;
       *   - confirm callback function (with BOOL argument):
       *       const func_type &Func;
       *   - confirm button text "yes":
       *       const std::string &YesText;
       *   - confirm button text "No":
       *       const std::string &YesText;
       * RETURNS: None.
       */
      template<typename func_type>
        VOID UXEdit( const std::string &Title, const std::string &Message,
                     std::string &EditString,
                     const func_type &Func,
                     const std::string &YesText = "Yes", const std::string &NoText = "No" )
        {
          DialogState =
          {
            .IsActive = TRUE,
            .IsEdit = TRUE,
            .Title = Title,
            .Message = Message,
            .EditText = &EditString,
            .Buttons = {YesText, NoText},
            .CallbackFunc = Func,
          };
          DialogState.EditLineBuffer[0] = 0;
          if (&EditString != nullptr)
            strncpy(DialogState.EditLineBuffer, EditString.c_str(), sizeof(DialogState.EditLineBuffer));
        } /* End of 'UXEdit' function */
    }; /* End of 'core' class */

    /* Styles collection namespace */
    namespace styles
    {
      // Current selected style
      extern INT CurrentStyle;

      // Styles array (Russian name, setup function, English name)
      extern std::vector<std::tuple<std::string, std::function<VOID(VOID)>, std::string>> ColorStyles;


      /* Set ImGui style by number function.
       * ARGUMENTS:
       *   - style number from style table:
       *       INT StyleNo;
       * RETURNS:
       *   (INT) real set style number.
       */
      INT SetStyle( INT StyleNo );

      /* Set ImGui style by name function.
       * ARGUMENTS:
       *   - style number from style table:
       *       const std::string &StyleName;
       * RETURNS:
       *   (INT) real set style number.
       */
      INT SetStyle( const std::string &StyleName );
    }; /* end of 'styles' namespace */
  } /* end of 'ux' namespace */
} /* end of 'tam' namespace */

/* All control type declarations */
#include "tam_ux_control_float.h"
#include "tam_ux_control_int.h"
#include "tam_ux_control_bool.h"
#include "tam_ux_control_separator.h"
#include "tam_ux_control_select.h"
#include "tam_ux_control_color.h"
#include "tam_ux_control_button.h"
#include "tam_ux_control_string.h"
#include "tam_ux_control_label.h"
#include "tam_ux_control_sameline.h"
#include "tam_ux_control_text.h"
#include "tam_ux_control_user.h"
#include "tam_ux_control_typewriter.h"
#include "tam_ux_control_group.h"
#include "tam_ux_control_radio.h"

#endif /* __tam_ux_h_ *//* END OF 'tam_ux.h' FILE */
