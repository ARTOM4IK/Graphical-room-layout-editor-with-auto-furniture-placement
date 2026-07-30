/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_flat_file.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Flat system functions implementation module.
 *               Flat save/load module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kupchenkov Ivan (IK4),
 *               Novozhilova Maryana (MN7),
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 30.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Save flat information into file function.
 * ARGUMENTS:
 *   - name of file to save into:
 *       std::string FileName;
 * RETURNS:
 *   (BOOL) TRUE if flat was successfully saved, FALSE otherwise.
 */
BOOL tam::flat::SaveToFile( std::string FileName )
{
  std::string FlatPath = /* Ani->Path + "bin/flats/" + */ FileName;
  json_saver JsonSaver(FlatPath); /// Add check for success of file opening!
  json_object ResultJSON {};

  ResultJSON["name"] = json(FileName);
  ResultJSON["wall_width"] = json(WallsWidth);
  ResultJSON["wall_height"] = json(WallsHeight);
  json_object Texs;
  for (auto &[Name, Tex] : Wallpapers)
    Texs.insert({Name, json(Tex->Name)});
  if ((UINT)Texs.size() > 0)
    ResultJSON["wallpapers"] = Texs;
  
  json_object scheme;
  scheme["texture"] = json(SchemeTex);
  scheme["rotate"] = json(SchemeRotate);
  scheme["scale"] = json(SchemeScale);
  scheme["translation"] = json_array({SchemeTranslate.X, SchemeTranslate.Y, SchemeTranslate.Z});
  ResultJSON["scheme"] = scheme;

  // Fill points information and get transition between old point indices and new
  std::map<INT, INT> NewPointIndices {};  // Map that contains pairs : old index and new index of POINT in JSON array
  json_array PointsArray {};
  INT PointNo = 0;
  for (auto &[Id, Point] : SegmentPoints)
  {
    PointsArray.push_back(json_array{Point.Pos.X, Point.Pos.Y});
    NewPointIndices[Id] = PointNo++;
  }
  ResultJSON["wall_points"] = PointsArray;

  // Fill segments information and get transiction between old segment indices and new
  std::map<INT, INT> NewSegmentIndices {};  // Map that contains pairs : old index and new index of SEGMENT in JSON array
  json_array SegmentsArray {};
  INT SegmentNo = 0;
  for (auto &[Id, Segment] : Segments)
  {
    NewSegmentIndices[Id] = SegmentNo;
    json_object Segment_JSON {};
    Segment_JSON["points"] = json_array {NewPointIndices[Segment.Point1], NewPointIndices[Segment.Point2]};
    Segment_JSON["type"] =
      Segment.Type == segment_type::eWALL ? "eWALL" :
      Segment.Type == segment_type::eDOOR ? "eDOOR" :
      "eWINDOW";
    if (Segment.Type == segment_type::eWIN)
    {
      Segment_JSON["window left offset"] = Segment.WindowLeftDist;
      Segment_JSON["window right offset"] = Segment.WindowRightDist;
      Segment_JSON["window bottom offset"] = Segment.WindowBottomDist;
      Segment_JSON["window top offset"] = Segment.WindowTopDist;
    }
    else if (Segment.Type == segment_type::eDOOR)
    {
      Segment_JSON["door width"] = Segment.DoorWidth;
      Segment_JSON["door height"] = Segment.DoorHeight;
    }
    SegmentsArray.push_back(Segment_JSON);
  }
  ResultJSON["segments"] = SegmentsArray;

  // Save flat furniture models
  json_array JFurnitureArray {};
  for (auto &[Name, FurMdl] : FlatFurnitureCollection)
  {
    furniture &m = FurMdl.first;

    json_object JModel {};
    JModel["name"] = m.Name;
    JModel["fileName"] = m.FileName;
    JModel["offset"] = json_array({m.Offset.X, m.Offset.Y, m.Offset.Z});
    JModel["rotate"] = json_array({m.Rotate.X, m.Rotate.Y, m.Rotate.Z});
    JModel["scale"] = m.Scale;
    JModel["size"] = json_array({m.Size.X, m.Size.Y, m.Size.Z});
    JModel["minBB"] = json_array({m.MinBB.X, m.MinBB.Y, m.MinBB.Z});
    JModel["maxBB"] = json_array({m.MaxBB.X, m.MaxBB.Y, m.MaxBB.Z});
    JFurnitureArray.push_back(JModel);
  }
  ResultJSON["modelList"] = JFurnitureArray;

  // Save room
  json_array RoomsArray {};
  for (auto &[RoomName, Room] : Rooms)
  {
    json_object ResRoom {};
    ResRoom["name"] = RoomName;
    json_array ResTexs {};
    for (auto &TexName : Rooms[RoomName].TextureNames)
    {
      //if (TexName == "Default wallpapers")
      ResTexs.push_back(TexName);
    }
      //ResTexs.push_back(Wallpapers[TexName]->Name);
    ResRoom["textures"] = ResTexs;
    ResRoom["wallpaper_scale"] = Room.WallpaperScale;
    if (Rooms[RoomName].RoomFloor.FloorTex != "")
      ResRoom["floor_texture"] = Rooms[RoomName].RoomFloor.FloorTex;
    else
      ResRoom["floor_texture"] = "no tex";
    ResRoom["floor_scale"] = Rooms[RoomName].RoomFloor.FloorScale;
    json_array WallsColor {};
    WallsColor.push_back(Room.WallsColor.X);
    WallsColor.push_back(Room.WallsColor.Y);
    WallsColor.push_back(Room.WallsColor.Z);
    ResRoom["walls_color"] = WallsColor;

    json_array PointsArray {};
    for (auto &PointId : Room.RoomPoints)
      PointsArray.push_back(NewPointIndices[PointId]);
    ResRoom["points"] = PointsArray;

    // Save room furniture
    json_array JRoomFurnutureArray {};
    for (auto &[Name, FurPlaces] : Room.Furniture)
    {
      json_object JRoomFurnuture;
      json_array JRoomFurnuturePlacesArray {};
      for (auto &Entry : FurPlaces)
      {
        json_object JPlace {};
        JPlace["position"] = json_array({Entry.Pos.X, Entry.Pos.Y, Entry.Pos.Z});
        JPlace["rotationY"] = Entry.RotationY;
        JRoomFurnuturePlacesArray.push_back(JPlace);
      }
      JRoomFurnuture["modelName"] = Name;
      JRoomFurnuture["modelPlaces"] = JRoomFurnuturePlacesArray;
      JRoomFurnutureArray.push_back(JRoomFurnuture);
    }
    ResRoom["furniture"] = JRoomFurnutureArray;
    RoomsArray.push_back(ResRoom);
  }
  ResultJSON["rooms"] = RoomsArray;
  JsonSaver.Save(ResultJSON);
  return TRUE;
} /* End of 'tam::flat::SaveToFile' function */

/* Load flat information from file function.
 * ARGUMENTS:
 *   - name of file to load from:
 *       std::string FileName;
 * RETURNS:
 *   (BOOL) TRUE if flat was successfully loaded, FALSE otherwise.
 */
BOOL tam::flat::LoadFromFile( std::string FileName )
{
  json FileFlat {};
  json_loader FlatLoader {};
  std::string FlatPath = /*Ani->Path + "bin/flats/" + */ FileName;
  FileFlat = FlatLoader.Load(FlatPath);
  if (FileFlat.GetJsObject().empty())
  {
    logger::warning << "Can't load flat from " >> 0x88AAFF << FlatPath << logger::SetDefStr() << ".";
    return FALSE;
  }
  // Clear flat information only after finding new valid flat
  Clear();

  WallsWidth = FileFlat["wall_width"].GetDouble(0.1);
  WallsHeight = FileFlat["wall_height"].GetDouble(3.0);

  json_object SchemeJson = FileFlat["scheme"].GetJsObject();
  if (SchemeJson.size() > 0 && SchemeJson["texture"].GetString() != "")
  {
    SchemeTex = SchemeJson["texture"].GetString();
    texture tex = Ani->TextureCreate(SchemeTex);
    if (tex != nullptr)
    {
      SchemeMaterial->Update({tex}, SchemeMaterial->Buffers);

      FLT Sc = (FLT)tex->W / tex->H;

      std::vector<vertex_std> Verts
      {
        { vec3(-Sc * 3.0, -0.047, -3.0), {0, 0}, {0, 1, 0}, vec4(1, 1, 1, 1) },
        { vec3(-Sc * 3.0, -0.047, 3.0), {0, 1}, {0, 1, 0}, vec4(1, 1, 1, 1) },
        { vec3(Sc * 3.0, -0.047, 3.0), {1, 1}, {0, 1, 0}, vec4(1, 1, 1, 1) },
        { vec3(Sc * 3.0, -0.047, -3.0), {1, 0}, {0, 1, 0}, vec4(1, 1, 1, 1) }
      };

      std::vector<INT> Inds
      {
        0, 1, 2,
        0, 3, 2
      };

      SchemePrim = Ani->PrimitiveCreate<vertex_std>(SchemeMaterial, std::span(Verts), std::span(Inds));
    }

    SchemeRotate = SchemeJson["rotate"].GetInt();
    SchemeScale = SchemeJson["scale"].GetDouble();
    SchemeTranslate = vec3(SchemeJson["translation"][0].GetDouble(), SchemeJson["translation"][1].GetDouble(), SchemeJson["translation"][2].GetDouble());
  }

  json_object Texs = FileFlat["wallpapers"].GetJsObject();
  for (auto &[Name, TexName] : Texs)
  {
    if (Name != "Default wallpaper")
      Wallpapers.insert({Name, Ani->TextureCreate(TexName.GetString())});
  }
  mth::vec4<BYTE> Data = mth::vec4<BYTE>(255);
  Wallpapers.insert({"Default wallpaper", Ani->TextureCreate("Default wallpaper", 1, 1, 4, Data, TRUE, TRUE, FALSE)});

  // Obtain points positions from loaded JSON
  auto FilePoints = FileFlat["wall_points"].GetArray();
  for (auto &Point : FilePoints)
  {
    if (Point.GetArray().size() == 2)
      SegmentPoints[SegmentPointsCounter++] = segment_point(Point.GetArray()[0].GetDouble(), Point.GetArray()[1].GetDouble());
  }

  // Obtain segments from loaded JSON
  auto FileSegments = FileFlat["segments"].GetArray();
  for (auto &Segment : FileSegments)
  {
    // Get indices of connected points and add segment
    if (Segment["points"].GetArray().size() == 2)
    {
      INT PointInds[2] = {Segment["points"].GetArray()[0].GetInt(), Segment["points"].GetArray()[1].GetInt()};
      INT AddedSegment = AddSegment(PointInds[0], PointInds[1]);
      segment_type SegmentType =
        Segment["type"].GetString() == "eWINDOW" ? segment_type::eWIN :
        Segment["type"].GetString() == "eDOOR" ? segment_type::eDOOR :
        segment_type::eWALL;
      // Handle window segment
      if (SegmentType == segment_type::eWIN)
      {
        Segments[AddedSegment].WindowLeftDist = Segment["window left offset"].GetDouble(0.8);
        Segments[AddedSegment].WindowRightDist = Segment["window right offset"].GetDouble(0.8);
        Segments[AddedSegment].WindowBottomDist = Segment["window bottom offset"].GetDouble(0.8);
        Segments[AddedSegment].WindowTopDist = Segment["window top offset"].GetDouble(0.8);
        ChangeSegmentType(AddedSegment, segment_type::eWIN);
      }
      // Handle door segment
      else if (SegmentType == segment_type::eDOOR)
      {
        Segments[AddedSegment].DoorWidth = Segment["door width"].GetDouble(0.8);
        Segments[AddedSegment].DoorHeight = Segment["door height"].GetDouble(1);
        ChangeSegmentType(AddedSegment, segment_type::eDOOR);
      }
    }
  }
  for (auto &[Key, Sgm] : Segments)
    UpdateSegmentLayout(Key);

  // Load flat furniture models
  json_array JFurnitureArray = FileFlat["modelList"].GetArray();
  //std::vector<furniture> ModelList;
  for (auto &m : JFurnitureArray)
  {
    std::string Name = m["name"].GetString();
    if (Trim(Name) != "")
    {
      std::string FileName = m["fileName"].GetString();
      
      auto rv = m["rotate"].GetArray();
      vec3 Rotate {};
      if (rv.size() > 2)
        Rotate = {(FLT)rv[0].GetDouble(), (FLT)rv[1].GetDouble(), (FLT)rv[2].GetDouble()};
      FLT Scale = m["scale"].GetDouble(1);
      auto sv  = m["size"].GetArray();
      vec3 Size {};
      if (sv.size() > 2)
        Size = {(FLT)sv[0].GetDouble(), (FLT)sv[1].GetDouble(), (FLT)sv[2].GetDouble()};
      auto ov = m["offset"].GetArray();
      vec3 Offset {};
      if (ov.size() > 2)
        Offset = {(FLT)ov[0].GetDouble(), (FLT)ov[1].GetDouble(), (FLT)ov[2].GetDouble()};
      auto mi = m["minBB"].GetArray();
      auto ma = m["maxBB"].GetArray();
      vec3 MinBB {}, MaxBB {};
      if (mi.size() > 2)
        MinBB = {(FLT)mi[0].GetDouble(), (FLT)mi[1].GetDouble(), (FLT)mi[2].GetDouble()};
      if (ma.size() > 2)
        MaxBB = {(FLT)ma[0].GetDouble(), (FLT)ma[1].GetDouble(), (FLT)ma[2].GetDouble()};
      //ModelList.push_back({Name, FileName, Rotate, Scale, Size, Offset, MinBB, MaxBB});
      furniture Fur {Name, FileName, Rotate, Scale, Size, Offset, MinBB, MaxBB};
      matr m = Fur.GetTransform();
      model Mdl = Ani->ModelCreate(Fur.FileName, m);
      if (Mdl != nullptr)
        FlatFurnitureCollection[Name] = {Fur, Mdl};
    }
  }

  // Obtain rooms from loaded JSON
  auto FileRooms = FileFlat["rooms"].GetArray();
  for (auto &FileRoom : FileRooms)
  {
    std::string RoomName = FileRoom["name"].GetString();

    // Fill points info
    auto RoomPoints = FileRoom["points"].GetArray();
    std::vector<INT> ResultRoomPoints {};
    for (auto &RoomPoint : RoomPoints)
      ResultRoomPoints.push_back(RoomPoint.GetInt());

    AddRoom(RoomName, ResultRoomPoints);

    // Load room furniture
    if (Rooms.contains(RoomName))
    {
      auto &Room = Rooms[RoomName];
      json_array JRoomFurnutureArray = FileRoom["furniture"].GetArray();
      for (auto &Fur : JRoomFurnutureArray)
      {
        std::string Name = Fur["modelName"].GetString();
        if (Trim(Name) != "" && FlatFurnitureCollection.contains(Name))
        {
          json_array JRoomFurnuturePlacesArray = Fur["modelPlaces"].GetArray();
          std::vector<room::furniture_entry> FurnutureList;
          for (auto &Place : JRoomFurnuturePlacesArray)
          {
            room::furniture_entry FurEntry {Name};
            auto pv = Place["position"].GetArray();
            vec3 Position {};
            if (pv.size() > 2)
              FurEntry.Pos = {(FLT)pv[0].GetDouble(), (FLT)pv[1].GetDouble(), (FLT)pv[2].GetDouble()};
            FurEntry.RotationY = Place["rotationY"].GetDouble(1);
            FurnutureList.push_back(FurEntry);
          }
          Room.Furniture[Name] = FurnutureList;
        }
      }
      json_array JRoomTextureNames = FileRoom["textures"].GetArray();
      std::vector<std::string> CurTexNames = {};
      if (JRoomTextureNames.size() > 0)
        for (auto &TexName : JRoomTextureNames)
          CurTexNames.push_back(TexName.GetString());

      json_array JWallsColor = FileRoom["walls_color"].GetArray();
      vec3 WallsColor;
      if (JWallsColor.size() > 0)
        WallsColor = vec3(JWallsColor[0].GetDouble(1.0), JWallsColor[1].GetDouble(1.0), JWallsColor[2].GetDouble(1.0));
      else
        WallsColor = vec3(0.6, 0.6, 0.55);

      if ((UINT)CurTexNames.size() == 0)
        CurTexNames = {"Default wallpaper"};
      std::string FloorTexName = FileRoom["floor_texture"].GetString("no tex");
      FLT FloorScale = FileRoom["floor_scale"].GetDouble(1.0);
      BUF_MTL_PBR BufMtlPBR;
      if (FloorTexName != "no tex" && FloorTexName != "")
      {
        BufMtlPBR =
        {
          .BaseColor = vec4(1),
          .MetallicRoughnessScale = vec3(0.18, 0.8, FloorScale),
          .TextureFlags = {1, 0, 0, 0, 0, 0, 0, 0}
        };
        Rooms[RoomName].RoomFloor.FloorTex = FloorTexName;
        Rooms[RoomName].RoomFloor.FloorMtl->Update({Wallpapers[FloorTexName]},
          Rooms[RoomName].RoomFloor.FloorMtl->Buffers);
        Rooms[RoomName].RoomFloor.FloorMtl->Buffers[0]->Update(&BufMtlPBR);
        Rooms[RoomName].RoomFloor.FloorPrim->Mtl = Rooms[RoomName].RoomFloor.FloorMtl;
      }
      UpdateRoomTexs(RoomName, CurTexNames);
      UpdateRoomMtlBuf(RoomName, WallsColor, FileRoom["wallpaper_scale"].GetDouble(1.0));
    }
  }
  return TRUE;
} /* End of 'tam::flat::LoadFromFile' function */

/* END OF 'tam_flat_file.cpp' FILE */
