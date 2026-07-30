/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_parameters_editor_unit.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Flat parameters parameter_editor unit.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3),
 *               Arsentev Artemy (AA4),
 *               Nechaev Vladimir (VN4),
 *               Kupchenkov Ivan (IK4),
 *               Novozhilova Maryana (MN7),
 *               Tretyakov Andrey (AT7),
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 21.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Project namespace */
namespace tam
{
  /* Units namespace */
  namespace units
  {
    /* Parameters edtitor unit class */
    class parameter_editor : public editor_unit
    {
    private:
      model DestrModel;
      BOOL ToDestroy = FALSE;
      FLT DTime;

      // Editor tab control
      ux::tab &Tab;
      ux::separator_type FlatSeparator {Tab, "Flat parameters"};                              // Flat separator
      ux::button LoadFlat {Tab, "Load flat"};                                                 // Load flat button
      ux::button SaveFlat {Tab, "Save flat"};                                                 // Save flat button
      ux::button ClearFlatButton {Tab, "Clear flat"};                                         // Clear flat

      ux::separator_type LightSeparator {Tab, "Light parameters"};                            // Light separator
      ux::flt_type ScreenBrightness {Tab, "Screen brightness", 100.0f, 10.0f, 300.0f};        // Brightness multiplier
      ux::radio_type LightingType {Tab, "", {"Scene lighting", "Presentation lighting"}, 0};  // Lighting types selector
                                                                                              // 1 - use every scene lighting,
                                                                                              // 2 - 3 lights near every model
      ux::user_type LightModeType {Tab};                                                      // User type for lighting mode handling

      FLT LightsIntensity = 5.0f;                                                             // Total intensity of present light
      vec3 PresentLightColor = vec3(1);                                                       // Present lights color

      ux::user_type DrawSettings {Tab};                                                       // User type drawing settings
      light SunLight {};                                                                      // Sun light pointer
      ux::bool_type EnableSun {Tab, "Sun", TRUE, "", ""};                                     // Enable Sun (directional light) flag
      ux::text_type TimeText {Tab, "Time"};                                                   // Time text
      ux::sameline_type TimeSameline {Tab};                                                   // For sameline text
      ux::flt_type Time {Tab, "", 12.0f, 0.0f, 24.0f};                                        // Time float type
      ux::select_type SelectType {Tab, "", {"Now", "Day", "Night", "Morning", "Evening"}, 1}; // Selector for select type

    public:
      /* Class constructor.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       *   - edit flat reference:
       *       flat &WorkFlat;
       *   - UX tab control reference:
       *       ux::tab &WorkTab;
       *   - UX function reference for user_type:
       *       const std::function<VOID ( VOID )> &UserUIFunc;
       */
      parameter_editor( anim *Ani, flat &WorkFlat, ux::tab &WorkTab, const std::function<VOID ( VOID )> &UserUIFunc ) :
        editor_unit(Ani, WorkFlat), Tab(WorkTab)
      {
        DrawSettings.SetUserFunc(UserUIFunc);
        SunLight = Ani->LightCreate(tam::light_type::eDIR, "Sun light #1", vec3(0), 4.7, vec3(-1, -1, 1).Normalizing(), vec3(1));
        LightModeType.SetUserFunc([Ani, this](VOID)
        {
          ImGuiWindow *Window = ImGui::GetCurrentWindow();
          ImGui::AlignTextToFramePadding();
          if (LightingType.GetSelected() == "Presentation lighting")
          {
            ImGui::Text("Lighting intensity:");
            ImGui::SameLine();
            ImGui::SliderFloat("##Lighting intensity", &LightsIntensity, 1, 102);
            ImGui::Text("Present lighting color:");
            ImGui::ColorEdit3("##PresentLightColor", &PresentLightColor.X);
          }
        });
        /// DestrModel = Ani->ModelCreate("Q://2025-26/TAM/assets/Furnitures/Special/NuclBB.glb", matr::RotateX(90) * matr::Scale(vec3(1.8)));
        DestrModel = Ani->ModelCreate("Q://2025-26/TAM/assets/Furnitures/Special/dt_75.glb", matr::Scale(vec3(3.0)));
      } /* End of 'parameter_editor' function */

      /* Class destructor */
      ~parameter_editor( VOID ) override
      {
      } /* End of '~parameter_editor' function */

      /* Render unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Render( anim *Ani ) override
      {
      } /* End of 'Render' function */

      /* Response unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
        Ani->Brightness = ScreenBrightness / 100;
        Ani->LightingMode = LightingType.GetSelected() == "Scene lighting" ? lighting_mode::eNORMAL : lighting_mode::ePRESENT;
        Ani->PresentLightsColor = PresentLightColor;
        Ani->PresentIntensity = LightsIntensity;
        std::time_t t = std::time(nullptr);
        std::tm *now = std::localtime(&t);

        static std::string CurType = "";
        if (CurType == "Now")
          Ani->EnvTime = (DBL)now->tm_hour + now->tm_min / 60.0 + now->tm_sec / 3600.0;
        if (Ani->IsSunEnabled != EnableSun)
          Ani->IsSunEnabled = EnableSun;
        if (Ani->EnvTime != Time)
          Ani->EnvTime = Time;
        if (CurType != SelectType.GetSelected())
        {
          DBL CurTime = (DBL)now->tm_hour + now->tm_min / 60.0 + now->tm_sec / 3600.0;

          CurType = SelectType.GetSelected();
          Time = Ani->EnvTime = CurType == "Now" ? CurTime : CurType == "Day" ? 12.0 : CurType == "Night" ? 0.0 : CurType == "Morning" ? 6.0 : 18.47;
        }
        if (CurType == "Now")
          Ani->EnvTime = (DBL)now->tm_hour + now->tm_min / 60.0 + now->tm_sec / 3600.0;

        if (LoadFlat)
         {
           std::string fn = Ani->OpenFileDialog("Load Flat", "flat\0*.flat\0All\0*.*\0");
           if (!fn.empty())
             Flat.LoadFromFile(fn);
         }
         if (SaveFlat)
         {
           std::string fn = Ani->SaveFileDialog("Save Flat", "flat\0*.flat\0All\0*.*\0");
           if (!fn.empty())
             Flat.SaveToFile(fn);
         }
         if (ClearFlatButton)
         {
           ToDestroy = TRUE;
           DTime = Ani->Time;
         }
         if (ToDestroy)
         {
           /// if (Ani->Time - DTime <= 2.0)
           ///   Ani->Draw(DestrModel, matr::RotateY(Ani->Time * 10) * matr::RotateX(-60 + sin(Ani->Time * 18) * 1.8) * matr::Translate(vec3(0, 36 - (Ani->Time - DTime) * 18 + 1.8, -16 + (Ani->Time - DTime) * 8)));
           /// ScreenBrightness = 100 / (1 + (Ani->Time - DTime));
           /// if (Ani->Time - DTime > 2.0)
           ///   Flat.Clear(), ScreenBrightness = 1000 / (20 * (Ani->Time - DTime - 2.0) + 1.0);
           /// if (Ani->Time - DTime >= 2.5)
           ///   ToDestroy = FALSE, ScreenBrightness = 100;
           Ani->Draw(DestrModel, matr::Translate(vec3(0, 0, -30 * (1.3 - (Ani->Time - DTime)))));
           if (Ani->Time - DTime >= 1.3)
             Flat.Clear();
           if (Ani->Time - DTime > 1.25)
             ScreenBrightness = 1000 / (20 * (Ani->Time - DTime - 1.25) + 1.0);
           if (Ani->Time - DTime >= 1.35)
             ToDestroy = FALSE, ScreenBrightness = 100;
         }
         /// Ani->Draw(DestrModel, matr::Identity());
      } /* End of 'Response' function */
    }; /* End of 'parameter_editor' class */
  } /* end of 'units' namespace */
  static units::editor::unit_register<units::parameter_editor> _("Parameter editor unit");
} /* end of 'tam' namespace */

/* END OF 'tam_parameters_editor_unit.cpp' FILE */
