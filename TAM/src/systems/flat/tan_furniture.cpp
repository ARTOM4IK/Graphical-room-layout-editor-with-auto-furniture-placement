/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_furniture.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Furniture collection implementation file.
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

/* Load collection function.
 * ARGUMENTS:
 *   - collection .json file name:
 *       const std::string &LoadFileName;
 * RETURNS:
 *   (BOOL) load result.
 */
BOOL tam::furniture_collection::Load( const std::string &LoadFileName )
{
  json JFile {};
  json_loader JLoader {};
  std::string DefPath = anim::render::core::Path + "assets/MainFurniture.json";
  DefPath = "Q:/2025-26/TAM/assets/MainFurniture.json";
  std::string FilePath = LoadFileName == "" ? DefPath : LoadFileName;

  FileName = FilePath;
  Types.clear();
  JFile = JLoader.Load(FilePath);

  if (JFile.GetJsObject().empty())
  {
    logger::warning << "Can't load flat from " >> 0x88AAFF << FilePath << logger::SetDefStr() << ".";
    return FALSE;
  }

  Name = JFile["name"].GetString();

  json_array JTypesArray = JFile["types"].GetArray();
  for (auto &t : JTypesArray)
  {
    std::string type_name = t["name"].GetString();
    if (Trim(type_name) != "")
    {
      json_array JModelsArray = t["modelList"].GetArray();
      std::vector<furniture> TypeModelList;
      for (auto &m : JModelsArray)
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
          TypeModelList.push_back({Name, FileName, Rotate, Scale, Size, Offset, MinBB, MaxBB});
        }
      }
      Types.push_back({type_name, TypeModelList});
    }
  }
  return TRUE;
} /* End of 'tam::furniture_collection::Load' function */

/* Save collection function.
 * ARGUMENTS:
 *   - collection .json file name:
 *       const std::string &SaveFileName;
 * RETURNS:
 *   (BOOL) save result.
 */
BOOL tam::furniture_collection::Save( const std::string &SaveFileName )
{
  std::string FilePath = SaveFileName;
  FilePath = SaveFileName == "" ? anim::render::core::Path + "assets/MainFurniture.json" : SaveFileName;
  json_saver JsonSaver(FilePath); /// Add check for success of file opening!
  json_object ResultJSON {};

  ResultJSON["name"] = json(Name);
  json_array JTypesArray {};
  for (auto &t : Types)
  {
    json_array JModelsArray {};
    for (auto &m : t.ModelList)
    {
      json_object JModel {};
      JModel["name"] = m.Name;
      JModel["fileName"] = m.FileName;
      JModel["offset"] = json_array({m.Offset.X, m.Offset.Y, m.Offset.Z});
      JModel["rotate"] = json_array({m.Rotate.X, m.Rotate.Y, m.Rotate.Z});
      JModel["scale"] = m.Scale;
      JModel["size"] = json_array({m.Size.X, m.Size.Y, m.Size.Z});
      JModel["minBB"] = json_array({m.MinBB.X, m.MinBB.Y, m.MinBB.Z});
      JModel["maxBB"] = json_array({m.MaxBB.X, m.MaxBB.Y, m.MaxBB.Z});
      JModelsArray.push_back(JModel);
    }
    json_object JType {};
    JType["name"] = t.TypeName;
    JType["modelList"] = JModelsArray;
    JTypesArray.push_back(JType);
  }
  ResultJSON["types"] = JTypesArray;
  JsonSaver.Save(ResultJSON);
  return TRUE;
} /* End of 'tam::furniture_collection::Save' function */

/* END OF 'tam_furniture.cpp' FILE */
