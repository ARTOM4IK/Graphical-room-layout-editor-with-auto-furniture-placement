/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Main declaration file.
 *               Base precompile header.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3),
 *               Arsentev Artemy (AA4),
 *               Nechaev Vladimir (VN4),
 *               Kupchenkov Ivan (IK4),
 *               Novozhilova Maryana (MN7),
 *               Tretyakov Andrey (AT7),
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 19.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_h_
#define __tam_h_

// Utilities
#include "utils/profiler/tam_profiler.h"
#include "utils/image/tam_image.h"
#include "utils/log/tam_logger.h"
#include "utils/json/tam_json.h"
#include "utils/json/tam_gltf.h"
#include "utils/imgui/tam_ui.h"
#include "utils/ai/tam_layout.h"
#include "utils/ai/tam_solver.h"
#include "utils/ollama/tam_ai.h"

// Main system modules
#include "systems/win/tam_win.h"
#include "systems/anim/render/vulkan/tam_vlk_core.h"
#include "systems/anim/tam_anim.h"
#include "systems/flat/tam_flat.h"

// Test system
#include "utils/tests/tam_test_system.h"

// Flat editor
#include "units/editor/tam_editor_unit.h"

// Walls editor
#include "utils/walls_editor/tam_walls_editor_system.h"

/* Project namespace */
namespace tam
{
} /* end of 'tam' namespace */

#endif /* __tam_h_ */

/* END OF 'tam.h' FILE */
