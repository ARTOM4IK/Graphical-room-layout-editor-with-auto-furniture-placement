/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_bot_unit.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Bot unit implementation module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 24.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* 'tam' namespace */
namespace tam
{
  /* Units namespace */
  namespace units
  {
    /* Bot unit representation class */
    class bot : public editor_unit
    {
    private:
      ux::tab &Tab; // Bot UI tab
      ux::user_type DrawSettings {Tab}; // User UI callback holder
      ux::separator_type SolverSeparator {Tab, "Layout solver"};
      ux::text_type SolverInfo {Tab,
        "Layout solver mode:\n"
        "LMB on room => arrange furniture in clicked room\n"
        "Arrange all rooms => solve every room that already has furniture\n"
        "Room type override is optional and may be empty."};
      ux::string_type RoomTypeOverride {Tab, "Room type override", ""};
      ux::button ArrangeAllRoomsButton {Tab, "Arrange all rooms"};
      ux::typewriter_label Prompt {Tab, "Last room", "No room selected"};
      ux::typewriter_label Resp {Tab, "Solver status", "Left click on a room to arrange its furniture."};

      vec2 MousePoint {};        // Last mouse point on room plane
      std::string ActiveRoom {}; // Last arranged room name

      /* Get trimmed room type override function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (std::string) trimmed room type override.
       */
      std::string GetRoomTypeOverride( VOID ) const
      {
        return Trim(RoomTypeOverride.operator std::string());
      } /* End of 'GetRoomTypeOverride' function */

      /* Count room furniture items available in collection function.
       * ARGUMENTS:
       *   - room name:
       *       const std::string &RoomName;
       * RETURNS:
       *   (INT) furniture count.
       */
      INT GetRoomFurnitureCount( const std::string &RoomName ) const
      {
        INT Count = 0;

        if (!Flat.Rooms.contains(RoomName))
          return 0;

        // Only furniture that has an asset in the collection may be passed to the solver.
        for (auto &[FurnitureName, Entries] : Flat.Rooms.at(RoomName).Furniture)
          if (Flat.FlatFurnitureCollection.contains(FurnitureName))
            Count += (INT)Entries.size();
        return Count;
      } /* End of 'GetRoomFurnitureCount' function */

      /* Set bot status text function.
       * ARGUMENTS:
       *   - room text:
       *       const std::string &RoomText;
       *   - status text:
       *       const std::string &StatusText;
       * RETURNS: None.
       */
      VOID SetStatus( const std::string &RoomText, const std::string &StatusText )
      {
        Prompt.SetText(RoomText);
        Resp.SetText(StatusText);
      } /* End of 'SetStatus' function */

      /* Arrange one room function.
       * ARGUMENTS:
       *   - room name:
       *       const std::string &RoomName;
       * RETURNS:
       *   (BOOL) arrange result.
       */
      BOOL ArrangeRoom( const std::string &RoomName )
      {
        std::string RoomType = GetRoomTypeOverride();
        INT FurnitureCount = GetRoomFurnitureCount(RoomName);

        if (!Flat.Rooms.contains(RoomName))
        {
          SetStatus("Room: not found", "Clicked point does not belong to a saved room.");
          return FALSE;
        }

        ActiveRoom = RoomName;
        Prompt.SetText(std::format("Room: {}", RoomName));

        if (FurnitureCount == 0)
        {
          Resp.SetText(std::format("Room '{}' has no furniture instances linked to collection assets.", RoomName));
          return FALSE;
        }

        // The solver writes directly back into the existing flat furniture entries.
        if (!Flat.AutoArrangeRoom(RoomName, RoomType))
        {
          Resp.SetText(std::format("Solver failed to build a valid layout for '{}' ({} item(s)).",
            RoomName, FurnitureCount));
          logger::warning << "Layout solver failed for room '" << RoomName << "'.";
          return FALSE;
        }

        if (RoomType == "")
          Resp.SetText(std::format("Placed {} furniture item(s) in '{}'.", FurnitureCount, RoomName));
        else
          Resp.SetText(std::format("Placed {} furniture item(s) in '{}' with room type '{}'.",
            FurnitureCount, RoomName, RoomType));
        logger::out << "Layout solver arranged room '" << RoomName << "'.";
        return TRUE;
      } /* End of 'ArrangeRoom' function */

      /* Arrange all rooms that already have furniture function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID ArrangeAllRooms( VOID )
      {
        std::string RoomType = GetRoomTypeOverride();
        INT Success = 0, Failed = 0, Skipped = 0;

        for (auto &[RoomName, Room] : Flat.Rooms)
        {
          INT FurnitureCount = GetRoomFurnitureCount(RoomName);

          if (FurnitureCount == 0)
          {
            Skipped++;
            continue;
          }
          if (Flat.AutoArrangeRoom(RoomName, RoomType))
          {
            Success++;
            ActiveRoom = RoomName;
          }
          else
            Failed++;
        }

        Prompt.SetText("Room: all");
        Resp.SetText(std::format("Arranged {} room(s), failed {} room(s), skipped {} empty room(s).",
          Success, Failed, Skipped));
      } /* End of 'ArrangeAllRooms' function */

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
      bot( anim *Ani, flat &WorkFlat, ux::tab &WorkTab, const std::function<VOID ( VOID )> &UserUIFunc ) :
        editor_unit(Ani, WorkFlat), Tab(WorkTab)
      {
        DrawSettings.SetUserFunc(UserUIFunc);
      } /* End of 'bot' function */

      /* Get response from unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
        if (ArrangeAllRoomsButton)
          ArrangeAllRooms();

        if (Ani->KeysClick[VK_LBUTTON] && !ImGui::GetIO().WantCaptureMouse)
        {
          mth::ray<FLT> R = Ani->Cam.FrameRay(Ani->Mx, Ani->My);
          DBL t = R.IntersectPlane();
          std::string RoomName {};

          // One click on a room directly runs the layout solver for that room.
          MousePoint = vec2(R(t).X, R(t).Z);
          RoomName = Flat.GetRoomByPoint(MousePoint);
          if (RoomName != "")
            ArrangeRoom(RoomName);
        }
      } /* End of 'Response' function */

      /* Render unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Render( anim *Ani ) override
      {
      } /* End of 'Render' function */

      /* Class destructor */
      ~bot( VOID )
      {
      } /* End of '~bot' function */
    };
    static units::editor::unit_register<units::bot> _("Bot unit");
  } /* end of 'units' namespace */
} /* end of 'tam' namespace */

/* END OF 'tam_bot_unit.cpp' FILE */
