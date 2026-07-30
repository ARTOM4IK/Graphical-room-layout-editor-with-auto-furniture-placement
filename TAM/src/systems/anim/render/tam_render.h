/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_render.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Render structure declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 21.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_render_h_
#define __tam_render_h_

#include "tam_def.h"
#include "vulkan/tam_vlk_core.h"
#include "res/shaders/tam_res_shaders.h"
#include "res/buffers/tam_res_buffers.h"
#include "res/textures/tam_res_textures.h"
#include "res/material_patterns/tam_res_material_pattern.h"
#include "res/materials/tam_res_material.h"
#include "res/primitives/tam_res_primitives.h"
#include "res/fonts/tam_res_font.h"
#include "res/models/tam_res_models.h"
#include "res/markers/tam_res_markers.h"
#include "res/lights/tam_res_lights.h"
#include "utils/imgui/tam_ui.h"
#include "utils/gfx_utilities/tam_guidelines.h"
#include "tam_render_pipeline.h"

// Walls editor
#include "utils/walls_editor/tam_walls_editor_system.h"

/* 'tam' namespace */
namespace tam
{
  /* 'render' handle class */
  class render :
    public vlk::core,
    public shader_manager, public buffer_manager, public texture_manager,
    public material_pattern_manager, public material_manager,
    public primitive_manager, public light_manager, public font_manager, public model_manager,
    public marker_manager, public ui_manager, public render_pipeline, public guidelines
  {
  public:
    // Synchronization buffer data type
    struct BUF_SYNC
    {
      vec4 GlobalTimeGlobalDeltaTimeTimeDeltaTime; // Global time, global deltatime, time, deltatime
      vec4 IsWireframePauseDebugBrightness;        // Wireframe, pause and debug flags and brightness multiplier value
    }; /* End of 'BUF_SYNC' structure */

    // Camera buffer data type
    struct BUF_CAM
    {
      matr::matr_data MatrView; // View matrix
      matr::matr_data MatrProj; // Projection matrix
      matr::matr_data MatrVP;   // View-proj matrix
      vec4 CamLocFrameW;        // Camera's location vector and frame width
      vec4 CamDirFrameH;        // Camera's look direction vector and frame height
      vec4 CamRightWp;          // Camera's right direction and project plane width
      vec4 CamUpHp;             // Camera's up direction vector and project plane height
      vec4 CamAtProjDist;       // Camera look-at point vector and near project plane distance
      vec4 ProjSizeOffsetXY;    // Inner project plane rectangle size and viewport offset
      vec4 MouseXYLR;           // Mouse position and button states
      mth::vec4<INT> ObjectId;  // Current object Id
    }; /* End of 'BUF_CAM' structure */

    buffer
      CamBuf {},           // Camera buffer
      SyncBuf {},          // Syncronization buffer
      OpaqueIdBuf {},      // Buffer of ids of models
      TransparentIdBuf {}; // Buffer of ids of models

    BOOL IsRenderInit = FALSE; // Flag if render was initialized

    timer &Timer;                   // Global timer reference
    input &Input;                   // Global input reference
    INT FrameW {600}, FrameH {400}; // Frame sizes
    camera Cam {};                  // Global camera
    INT CurrentObjectId {};         // Current object Id for selection
    FLT Brightness = 1;             // Brightness multiplier

    /* Viewport parameter data type */
    struct viewport_rect
    {
      FLT X, Y, W, H;
    }; /* End of 'viewport_rect' structure */

    // User specified viewport
    viewport_rect SceneViewport {};
    
    /* Base render constructor.
     * ARGUMENTS:
     *   - timer reference:
     *       timer &Timer;
     *   - input reference:
     *       input &Input;
     */
    render( timer &Timer, input &Input ) :
      Timer(Timer), Input(Input),
      shader_manager(*this), buffer_manager(*this), texture_manager(*this),
      material_pattern_manager(*this), material_manager(*this),
      primitive_manager(*this), light_manager(*this), font_manager(*this), model_manager(*this),
      marker_manager(*this), ui_manager(*this), render_pipeline(this), guidelines(this)
    {
    } /* End of 'render' function */

    /* Class destructor */
    ~render( VOID )
    {
      Close();
    } /* End of '~render' function */

    /* Initialize render function.
     * ARGUMENTS:
     *   - reference to window handle:
     *       HWND &hWndRef;
     *   - initial frame sizes:
     *       INT Width, Height;
     * RETURNS:
     *   (BOOL) TRUE if render system was successfully initialized, FALSE otherwise.
     */
    BOOL Init( HWND& hWndRef, INT Width = 600, INT Height = 400 );

    /* Close render system function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Close( VOID );

    /* Resize render params function.
     * ARGUMENTS:
     *   - new width and height of window:
     *       INT NewW, NewH;
     * RETURNS: None.
     */
    VOID Resize( INT NewW, INT NewH );

    /* Prepare render for new frame function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID FrameStart( VOID );

    /* End frame function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID FrameEnd( VOID );
  }; /* End of 'render' class */
} /* end of 'tam' namespace */

#endif /* __tam_render_h_ */

/* END OF 'tam_render.h' FILE */
