/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ux_tab.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               UX tab class implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 19.03.2026
 * NOTE        : Namespace 'tam::ux'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"
#include "tam_ux.h"

/* Constructor with core, name and optional icon.
 * ARGUMENTS:
 *   - pointer to UX core:
 *       core *C;
 *   - tab display name:
 *       const std::string &TabName;
 *   - tab icon:
 *       const icon &TabIcon;
 */
tam::ux::tab::tab( tam::ux::core *C, const std::string &TabName, const tam::ux::icon &TabIcon ) :
  Core(C), Name(TabName), Icon(TabIcon)
{
  if (Core != nullptr)
    Core->RegisterTab(this);
} /* End of 'tam::ux::tab::tab' function */

/* Copying Constructor.
 * ARGUMENTS:
 *   - reference to copyinh tab control:
 *       const tab &CopyTab;
 */
tam::ux::tab::tab( const tab &CopyTab ) :
  Core(CopyTab.Core), Name(CopyTab.Name), Icon(CopyTab.Icon), Controls(CopyTab.Controls)
{
  if (Core != nullptr)
    Core->RegisterTab(this);
} /* End of 'tam::ux::tab::tab' function */

/* Assignment operator function.
 * ARGUMENTS:
 *   - reference to copyinh tab control:
 *       const tab &CopyTab;
 * RETURNS:
 *   (tab &) selft reference.
 */
tam::ux::tab & tam::ux::tab::operator=( const tab &CopyTab )
{
  if (this == &CopyTab)
    return *this;

  Core = CopyTab.Core;
  Name = CopyTab.Name;
  Icon = CopyTab.Icon;
  Controls = CopyTab.Controls;

  if (Core != nullptr)
    Core->RegisterTab(this);
  return *this;
} /* End of 'tam::ux::tab::operator=' function */

/* Assignment with move semantic operator function.
 * ARGUMENTS:
 *   - reference to copyinh tab control:
 *       const tab &CopyTab;
 * RETURNS:
 *   (tab &) selft reference.
 */
tam::ux::tab & tam::ux::tab::operator=( tab &&CopyTab )
{
  if (this == &CopyTab)
    return *this;

  if (CopyTab.Core != nullptr)
    CopyTab.Core->UnregisterTab(&CopyTab);

  std::swap(Core, CopyTab.Core);
  std::swap(Name, CopyTab.Name);
  std::swap(Icon, CopyTab.Icon);
  std::swap(Controls, CopyTab.Controls);

  if (Core != nullptr)
    Core->RegisterTab(this);
  return *this;
} /* End of 'tam::ux::tab::operator=' function */

/* Tab destructor */
tam::ux::tab::~tab( VOID )
{
  if (Core != nullptr)
    Core->UnregisterTab(this);
} /* End of 'tam::ux::tab::~tab' function */

/* Register control in this tab function.
 * ARGUMENTS:
 *   - pointer to control:
 *       base_control *Ctrl;
 * RETURNS: None.
 */
VOID tam::ux::tab::RegisterControl( tam::ux::base_control *Ctrl )
{
  if (Ctrl != nullptr)
    Controls.push_back(Ctrl);
} /* End of 'tam::ux::tab::RegisterControl' function */

/* Unregister control from this tab function.
 * ARGUMENTS:
 *   - pointer to control:
 *       base_control *Ctrl;
 * RETURNS: None.
 */
VOID tam::ux::tab::UnregisterControl( tam::ux::base_control *Ctrl )
{
  auto it = std::find(Controls.begin(), Controls.end(), Ctrl);
  if (it != Controls.end())
    Controls.erase(it);
} /* End of 'tam::ux::tab::UnregisterControl' function */

/* Draw all controls in this tab function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::tab::DrawControls( VOID )
{
  for (auto *C : Controls)
    C->Draw();
} /* End of 'tam::ux::tab::DrawControls' function */

/* Reset one-shot controls function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::ux::tab::ResetControls( VOID )
{
  for (auto *C : Controls)
    C->ResetFrame();
} /* End of 'tam::ux::tab::ResetControls' function */

/* END OF 'tam_ux_tab.cpp' FILE */
