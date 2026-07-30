/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_unit_layout_science.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Furniture layout ablation study unit.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 29.07.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Project namespace */
namespace tam
{
  /* Anonymous namespace */
  namespace
  {
    /* Objective metrics representation class */
    class obj_metrics
    {
    public:
      INT GeometryValidity = 0;   // Count of geometry intersections
      DBL Accessibility = 0;      // Accessible furniture percent
      DBL FreeSpaceRatio = 0;     // Free room area ratio
      INT OpeningClearance = 0;   // Count of blocked openings
      DBL WallAlignment = 0;      // Average furniture-wall gap
      DBL RuntimeMs = 0;          // Solver runtime in milliseconds
      INT CandidateCount = 0;     // Generated candidate count
    }; /* End of 'obj_metrics' class */

    /* Experiment row representation class */
    class exp_row
    {
    public:
      INT No = 0;              // Experiment number
      std::string Name {};     // Experiment name
      std::string Desc {};     // Short description
      std::string Param {};    // Current parameter text
      DBL ParamValue = 0;      // Numeric parameter for plots
      obj_metrics Metrics {};  // Objective metrics
      BOOL Success = FALSE;    // Solver success flag
      std::string Status {};   // Solver status text
    }; /* End of 'exp_row' class */

    /* Experiment description representation class */
    class exp_desc
    {
    public:
      INT No = 0;              // Experiment number
      const CHAR *Name = "";   // Experiment name
      const CHAR *Desc = "";   // Experiment description
    }; /* End of 'exp_desc' class */

    /* Box with source asset representation class */
    class placed_box
    {
    public:
      std::string Id {};              // Instance id
      const furniture_asset *Asset {}; // Source asset
      obb2 Box {};                    // Solver-space footprint
    }; /* End of 'placed_box' class */

    /* Ablation study experiment table */
    static const exp_desc Experiments[]
    {
      {1, "Full Solver (Baseline)", "All geometric constraints, semantic heuristics, candidate ranking and annealing are enabled."},
      {2, "Geometry Only", "All soft semantic heuristics and candidate ranking are disabled; hard geometry stays active."},
      {3, "No Opening Preference", "Soft preferences around doors and windows are disabled; hard opening clearance stays active."},
      {4, "No Wall Preference", "Wall, corner, center-free and cardinal-alignment preferences are disabled."},
      {5, "No Pair Semantics", "Furniture-pair semantic relations are disabled."},
      {6, "No Candidate Ranking", "Seed candidate ranking and learned rank model contributions are disabled."},
      {7, "Sensitivity Analysis", "OpeningPenalty weight is swept over several values."},
      {8, "Candidate Count", "Maximum stored candidates per furniture item is swept."},
      {9, "Annealing Iterations", "Simulated annealing iteration budget is swept."},
    };

    /* Get experiment description function.
     * ARGUMENTS:
     *   - experiment number:
     *       INT No;
     * RETURNS:
     *   (const exp_desc &) experiment description.
     */
    const exp_desc & GetExpDesc( INT No )
    {
      for (auto &Exp : Experiments)
        if (Exp.No == No)
          return Exp;
      return Experiments[0];
    } /* End of 'GetExpDesc' function */

    /* Find asset by id function.
     * ARGUMENTS:
     *   - source problem:
     *       const problem &Problem;
     *   - asset id:
     *       const std::string &AssetId;
     * RETURNS:
     *   (const furniture_asset *) found asset or nullptr.
     */
    const furniture_asset * FindAsset( const problem &Problem, const std::string &AssetId )
    {
      for (auto &Asset : Problem.Assets)
        if (Asset.AssetId == AssetId)
          return &Asset;
      return nullptr;
    } /* End of 'FindAsset' function */

    /* Build front clearance box function.
     * ARGUMENTS:
     *   - source asset:
     *       const furniture_asset &Asset;
     *   - furniture box:
     *       const obb2 &Box;
     * RETURNS:
     *   (obb2) clearance box.
     */
    obb2 MakeAccessBox( const furniture_asset &Asset, const obb2 &Box )
    {
      vec2 FrontDir = GetForwardDirection(Asset, Box);
      vec2 SideDir = FrontDir.LeftNormal().Normalizing();
      vec2 SideProjection = obb2().ProjectOBB(Box, SideDir);
      vec2 FrontProjection = obb2().ProjectOBB(Box, FrontDir);
      FLT SideHalf = (SideProjection.Y - SideProjection.X) * (FLT)0.425;
      FLT FrontHalf = (FrontProjection.Y - FrontProjection.X) * (FLT)0.5;

      return obb2(Box.Center + FrontDir * (FrontHalf + Asset.FrontClearance / 2.0),
        SideDir, FrontDir, vec2(SideHalf, Asset.FrontClearance / 2.0));
    } /* End of 'MakeAccessBox' function */

    /* Obtain minimal box-to-wall clearance function.
     * ARGUMENTS:
     *   - room polygon:
     *       const std::vector<vec2> &Polygon;
     *   - furniture box:
     *       const obb2 &Box;
     * RETURNS:
     *   (FLT) minimal gap.
     */
    FLT GetWallGap( const std::vector<vec2> &Polygon, const obb2 &Box )
    {
      vec2 Corners[4] {};
      FLT Dist = FLT_MAX;

      Box.GetCorners(Corners);
      for (INT CornerNo = 0; CornerNo < 4; CornerNo++)
        for (INT EdgeNo = 0; EdgeNo < (INT)Polygon.size(); EdgeNo++)
        {
          INT NextEdgeNo = (EdgeNo + 1) % (INT)Polygon.size();

          Dist = mth::Min(Dist, vec2::SegmentDistance(Corners[CornerNo],
            Polygon[EdgeNo], Polygon[NextEdgeNo]));
        }
      return Dist == FLT_MAX ? 0 : Dist;
    } /* End of 'GetWallGap' function */

    /* Build placed boxes from solution function.
     * ARGUMENTS:
     *   - source problem:
     *       const problem &Problem;
     *   - source solution:
     *       const solution &Solution;
     * RETURNS:
     *   (std::vector<placed_box>) placement boxes.
     */
    std::vector<placed_box> BuildBoxes( const problem &Problem, const solution &Solution )
    {
      std::vector<placed_box> Boxes {};

      for (auto &Placement : Solution.Placements)
      {
        const furniture_asset *Asset = FindAsset(Problem, Placement.AssetId);

        if (Asset == nullptr)
          continue;
        Boxes.push_back({Placement.InstanceId, Asset,
          MakeAssetOBB(*Asset, vec2(Placement.Position.X, Placement.Position.Z),
            Placement.RotationY)});
      }
      return Boxes;
    } /* End of 'BuildBoxes' function */

    /* Check if access box is free function.
     * ARGUMENTS:
     *   - source problem:
     *       const problem &Problem;
     *   - placement boxes:
     *       const std::vector<placed_box> &Boxes;
     *   - item index:
     *       INT ItemNo;
     * RETURNS:
     *   (BOOL) accessible flag.
     */
    BOOL IsAccessFree( const problem &Problem, const std::vector<placed_box> &Boxes, INT ItemNo )
    {
      const placed_box &Item = Boxes[ItemNo];
      obb2 AccessBox {};

      if (Item.Asset == nullptr || Item.Asset->FrontClearance <= Threshold)
        return TRUE;
      AccessBox = MakeAccessBox(*Item.Asset, Item.Box);
      if (!obb2().IsOBBInsidePolygon(AccessBox, Problem.Room.Polygon))
        return FALSE;
      for (auto &Forbidden : Problem.ForbiddenPolygons)
        if (obb2().IsOBBIntersectPolygon(AccessBox, Forbidden))
          return FALSE;
      for (auto &Opening : Problem.Room.Openings)
        if (obb2().IsOBBIntersectOBB(AccessBox, MakeOpeningKeepout(Opening, Problem.Room.Polygon)))
          return FALSE;
      for (INT OtherNo = 0; OtherNo < (INT)Boxes.size(); OtherNo++)
        if (OtherNo != ItemNo && obb2().IsOBBIntersectOBB(AccessBox, Boxes[OtherNo].Box))
          return FALSE;
      return TRUE;
    } /* End of 'IsAccessFree' function */

    /* Evaluate objective metrics function.
     * ARGUMENTS:
     *   - source problem:
     *       const problem &Problem;
     *   - source solution:
     *       const solution &Solution;
     *   - measured runtime:
     *       DBL RuntimeMs;
     * RETURNS:
     *   (obj_metrics) metrics.
     */
    obj_metrics EvalMetrics( const problem &Problem, const solution &Solution, DBL RuntimeMs )
    {
      obj_metrics Metrics {};
      std::vector<placed_box> Boxes = BuildBoxes(Problem, Solution);
      DBL RoomArea = fabs(vec2::Zero().SignedPolygonArea(Problem.Room.Polygon));
      DBL Occupied = 0;
      INT Accessible = 0;

      Metrics.RuntimeMs = RuntimeMs;
      Metrics.CandidateCount = Solution.CandidateCount;

      for (INT i = 0; i < (INT)Boxes.size(); i++)
      {
        const obb2 &Box = Boxes[i].Box;

        if (!obb2().IsOBBInsidePolygon(Box, Problem.Room.Polygon))
          Metrics.GeometryValidity++;
        for (auto &Forbidden : Problem.ForbiddenPolygons)
          if (obb2().IsOBBIntersectPolygon(Box, Forbidden))
            Metrics.GeometryValidity++;
        for (INT j = i + 1; j < (INT)Boxes.size(); j++)
          if (obb2().IsOBBIntersectOBB(Box, Boxes[j].Box))
            Metrics.GeometryValidity++;

        Occupied += Box.HalfSize.X * 2.0 * Box.HalfSize.Y * 2.0;
        Metrics.WallAlignment += GetWallGap(Problem.Room.Polygon, Box);
        if (IsAccessFree(Problem, Boxes, i))
          Accessible++;
      }

      for (auto &Opening : Problem.Room.Openings)
      {
        obb2 OpeningBox = MakeOpeningKeepout(Opening, Problem.Room.Polygon);
        BOOL Blocked = FALSE;

        for (auto &Box : Boxes)
          if (obb2().IsOBBIntersectOBB(Box.Box, OpeningBox))
          {
            Blocked = TRUE;
            break;
          }
        if (Blocked)
          Metrics.OpeningClearance++;
      }

      Metrics.Accessibility = Boxes.empty() ? 0 :
        (DBL)Accessible * 100.0 / (DBL)Boxes.size();
      Metrics.FreeSpaceRatio = RoomArea <= Threshold ? 0 :
        mth::Max(0.0, mth::Min(1.0, (RoomArea - Occupied) / RoomArea));
      Metrics.WallAlignment = Boxes.empty() ? 0 :
        Metrics.WallAlignment / (DBL)Boxes.size();
      return Metrics;
    } /* End of 'EvalMetrics' function */

    /* Layout science unit representation class */
    class layout_science : public unit
    {
    private:
      flat WorkFlat {};                  // Loaded research flat
      light CameraLight {};              // Camera-linked point light
      std::vector<exp_row> Rows {};      // Last experiment rows
      std::string AssetsDir =
        "C:/Users/temk/Desktop/DiskQ/2025-26/TAM/assets"; // Source asset root
      std::string SourceFlat {};         // Loaded source flat file
      std::string FixedFlat {};          // Runtime fixed flat file
      std::string RoomName {};           // Active room
      std::string Status = "Not loaded"; // Unit status
      INT CurExp = 1;                    // Current experiment number
      BOOL IsReady = FALSE;              // Scene load flag

      /* Get room furniture count function.
       * ARGUMENTS:
       *   - room name:
       *       const std::string &Name;
       * RETURNS:
       *   (INT) furniture item count.
       */
      INT GetFurnitureCount( const std::string &Name ) const
      {
        INT Count = 0;

        if (!WorkFlat.Rooms.contains(Name))
          return 0;
        for (auto &[FurnitureName, Entries] : WorkFlat.Rooms.at(Name).Furniture)
          if (WorkFlat.FlatFurnitureCollection.contains(FurnitureName))
            Count += (INT)Entries.size();
        return Count;
      } /* End of 'GetFurnitureCount' function */

      /* Find default flat file function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (std::string) source flat file.
       */
      std::string FindFlatFile( VOID ) const
      {
        std::vector<std::string> Names
        {
          "flat03_fixdoors.flat",
          "flat03_wp.flat",
          "flat03.flat",
          "new_test.flat",
          "scheme_first.flat",
          "wallpapers.flat"
        };

        for (auto &Name : Names)
        {
          std::filesystem::path Path = std::filesystem::path(AssetsDir) / Name;

          if (std::filesystem::exists(Path))
            return Path.string();
        }
        return "";
      } /* End of 'FindFlatFile' function */

      /* Prepare flat file with local paths function.
       * ARGUMENTS:
       *   - source file:
       *       const std::string &FileName;
       * RETURNS:
       *   (std::string) fixed file name.
       */
      std::string PrepareFlatFile( const std::string &FileName )
      {
        std::ifstream In(FileName, std::ios::binary);
        std::string Text {};
        std::filesystem::path OutPath = "bin/flats/layout_science_tmp.flat";

        if (!In)
          return "";
        Text.assign(std::istreambuf_iterator<CHAR>(In),
          std::istreambuf_iterator<CHAR>());
        std::replace(Text.begin(), Text.end(), '\\', '/');
        Text = ReplaceAll(Text, "Q:/2025-26/TAM/assets", AssetsDir);
        Text = ReplaceAll(Text, "Q://2025-26/TAM/assets", AssetsDir);

        std::filesystem::create_directories(OutPath.parent_path());
        std::ofstream Out(OutPath.string(), std::ios::binary);

        if (!Out)
          return "";
        Out << Text;
        return OutPath.string();
      } /* End of 'PrepareFlatFile' function */

      /* Select active room function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (BOOL) selection result.
       */
      BOOL SelectRoom( VOID )
      {
        if (GetFurnitureCount("Bathroom") > 0)
        {
          RoomName = "Bathroom";
          return TRUE;
        }
        for (auto &[Name, Room] : WorkFlat.Rooms)
          if (GetFurnitureCount(Name) > 0)
          {
            RoomName = Name;
            return TRUE;
          }
        return FALSE;
      } /* End of 'SelectRoom' function */

      /* Setup camera function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID SetupCamera( anim *Ani )
      {
        vec2 MinP(FLT_MAX), MaxP(-FLT_MAX);

        for (auto &[Id, Point] : WorkFlat.SegmentPoints)
        {
          MinP.X = mth::Min(MinP.X, Point.Pos.X);
          MinP.Y = mth::Min(MinP.Y, Point.Pos.Y);
          MaxP.X = mth::Max(MaxP.X, Point.Pos.X);
          MaxP.Y = mth::Max(MaxP.Y, Point.Pos.Y);
        }
        if (MinP.X == FLT_MAX)
          return;

        vec3 Center((MinP.X + MaxP.X) * (FLT)0.5, 0, (MinP.Y + MaxP.Y) * (FLT)0.5);
        FLT Span = mth::Max(MaxP.X - MinP.X, MaxP.Y - MinP.Y);
        FLT Dist = mth::Max((FLT)8.0, Span * (FLT)1.15);

        Ani->Cam.SetLocAtUp(Center + vec3(Dist, Dist * (FLT)0.9, Dist),
          Center, vec3(0, 1, 0));
        Ani->IsCamBufUpdate = TRUE;
      } /* End of 'SetupCamera' function */

      /* Load research scene function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS:
       *   (BOOL) load result.
       */
      BOOL LoadScene( anim *Ani )
      {
        SourceFlat = FindFlatFile();
        if (SourceFlat == "")
        {
          Status = "No .flat file found in assets.";
          logger::warning << "Layout ablation study: no source .flat file found.";
          return FALSE;
        }

        FixedFlat = PrepareFlatFile(SourceFlat);
        if (FixedFlat == "" || !WorkFlat.LoadFromFile(FixedFlat))
        {
          Status = std::format("Failed to load '{}'.", SourceFlat);
          logger::warning << "Layout ablation study: flat load failed.";
          return FALSE;
        }
        if (!SelectRoom())
        {
          Status = "No room with loaded furniture was found.";
          logger::warning << "Layout ablation study: no room with furniture.";
          return FALSE;
        }

        WorkFlat.DrawCeiling = FALSE;
        WorkFlat.DrawFloor = TRUE;
        WorkFlat.DrawFurniture = TRUE;
        WorkFlat.DrawWalls = TRUE;
        SetupCamera(Ani);
        Status = std::format("Loaded '{}' / room '{}'.", SourceFlat, RoomName);
        logger::out << "Layout ablation study loaded room '" << RoomName << "' from '" << SourceFlat << "'.";
        return TRUE;
      } /* End of 'LoadScene' function */

      /* Apply experiment switches function.
       * ARGUMENTS:
       *   - experiment number:
       *       INT No;
       *   - target problem:
       *       problem &Problem;
       * RETURNS: None.
       */
      VOID ApplyExpOptions( INT No, problem &Problem )
      {
        Problem.Options.RandomSeed = 20260421;

        if (No == 2)
        {
          Problem.Options.UseOpeningPref = FALSE;
          Problem.Options.UseWallPref = FALSE;
          Problem.Options.UseRoomSem = FALSE;
          Problem.Options.UsePairSem = FALSE;
          Problem.Options.UseCandRank = FALSE;
        }
        else if (No == 3)
          Problem.Options.UseOpeningPref = FALSE;
        else if (No == 4)
          Problem.Options.UseWallPref = FALSE;
        else if (No == 5)
          Problem.Options.UsePairSem = FALSE;
        else if (No == 6)
          Problem.Options.UseCandRank = FALSE;
      } /* End of 'ApplyExpOptions' function */

      /* Run one solver configuration function.
       * ARGUMENTS:
       *   - experiment number:
       *       INT No;
       *   - parameter text:
       *       const std::string &Param;
       *   - parameter value:
       *       DBL ParamValue;
       *   - extra option callback:
       *       const std::function<VOID (problem &)> &Setup;
       * RETURNS: None.
       */
      VOID RunOne( INT No, const std::string &Param, DBL ParamValue,
        const std::function<VOID ( problem & )> &Setup )
      {
        const exp_desc &Info = GetExpDesc(No);
        problem Problem {};
        solver Solver {};
        solution Result {};
        exp_row Row {};
        auto Start = std::chrono::high_resolution_clock::now();
        DBL RuntimeMs = 0;

        Row.No = No;
        Row.Name = Info.Name;
        Row.Desc = Info.Desc;
        Row.Param = Param;
        Row.ParamValue = ParamValue;

        if (!BuildProblem(WorkFlat, RoomName, Problem))
        {
          Row.Status = "Problem build failed";
          Rows.push_back(Row);
          return;
        }
        ApplyExpOptions(No, Problem);
        Setup(Problem);

        Start = std::chrono::high_resolution_clock::now();
        Result = Solver.Solve(Problem);
        RuntimeMs = std::chrono::duration<DBL, std::milli>(
          std::chrono::high_resolution_clock::now() - Start).count();

        Row.Success = Result.Success;
        Row.Status = Result.StatusMessage;
        Row.Metrics = EvalMetrics(Problem, Result, RuntimeMs);
        Rows.push_back(Row);
        if (Result.Success)
          ApplySolution(WorkFlat, RoomName, Result);
      } /* End of 'RunOne' function */

      /* Print experiment report function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID PrintReport( VOID ) const
      {
        const exp_desc &Info = GetExpDesc(CurExp);
        std::string Text;

        Text += "\n";
        Text += std::format("Ablation Study Experiment {}: {}\n", CurExp, Info.Name);
        Text += std::format("Room: {}\n", RoomName);
        Text += std::format("Description: {}\n", Info.Desc);
        Text += "| Parameter | Geometry Validity | Accessibility (%) | Free Space Ratio | Opening Clearance | Wall Alignment (m) | Runtime (ms) | Candidate Count | Status |\n";
        Text += "|---|---:|---:|---:|---:|---:|---:|---:|---|\n";
        for (auto &Row : Rows)
          Text += std::format("| {} | {} | {:.2f} | {:.4f} | {} | {:.4f} | {:.3f} | {} | {} |\n",
            Row.Param, Row.Metrics.GeometryValidity, Row.Metrics.Accessibility,
            Row.Metrics.FreeSpaceRatio, Row.Metrics.OpeningClearance,
            Row.Metrics.WallAlignment, Row.Metrics.RuntimeMs,
            Row.Metrics.CandidateCount, Row.Status);

        Text += "CSV\n";
        Text += "experiment,name,param,geometry_validity,accessibility_percent,free_space_ratio,opening_clearance,wall_alignment_m,runtime_ms,candidate_count,status\n";
        for (auto &Row : Rows)
          Text += std::format("{},\"{}\",\"{}\",{},{:.2f},{:.4f},{},{:.4f},{:.3f},{},\"{}\"\n",
            Row.No, Row.Name, Row.Param, Row.Metrics.GeometryValidity,
            Row.Metrics.Accessibility, Row.Metrics.FreeSpaceRatio,
            Row.Metrics.OpeningClearance, Row.Metrics.WallAlignment,
            Row.Metrics.RuntimeMs, Row.Metrics.CandidateCount, Row.Status);
        logger::out << Text;

        try
        {
          std::filesystem::create_directories("bin/reports");
          std::ofstream Out("bin/reports/layout_ablation_report.md", std::ios::app);

          Out << Text << "\n";
        }
        catch (...)
        {
          logger::warning << "Layout ablation report file was not written.";
        }
      } /* End of 'PrintReport' function */

      /* Reset report file function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID ResetReportFile( VOID ) const
      {
        try
        {
          std::filesystem::create_directories("bin/reports");
          std::ofstream Out("bin/reports/layout_ablation_report.md", std::ios::trunc);

          Out << "# Layout Ablation Study Report\n";
        }
        catch (...)
        {
          logger::warning << "Layout ablation report file was not reset.";
        }
      } /* End of 'ResetReportFile' function */

      /* Run selected experiment function.
       * ARGUMENTS:
       *   - experiment number:
       *       INT No;
       * RETURNS: None.
       */
      VOID RunExp( INT No )
      {
        CurExp = No;
        Rows.clear();

        if (!IsReady)
          return;

        if (No >= 1 && No <= 6)
          RunOne(No, "default", 0,
            []( problem &Problem )
            {
              (VOID)Problem;
            });
        else if (No == 7)
        {
          DBL Values[] = {0, 0.25, 0.5, 1, 2, 4};

          for (auto Value : Values)
            RunOne(No, std::format("OpeningPenalty={:.2f}", Value), Value,
              [Value]( problem &Problem )
              {
                Problem.Options.OpeningWeight = (FLT)Value;
              });
        }
        else if (No == 8)
        {
          INT Values[] = {24, 48, 96, 192, 384};

          for (auto Value : Values)
            RunOne(No, std::format("MaxCandidatesPerItem={}", Value), Value,
              [Value]( problem &Problem )
              {
                Problem.Options.MaxCandidatesPerItem = Value;
              });
        }
        else if (No == 9)
        {
          INT Values[] = {100, 250, 500, 1000, 2500, 5000};

          for (auto Value : Values)
            RunOne(No, std::format("AnnealingIterations={}", Value), Value,
              [Value]( problem &Problem )
              {
                Problem.Options.AnnealingIterations = Value;
              });
        }

        Status = Rows.empty() ? "Experiment failed before producing rows." :
          std::format("Experiment {} finished: {} row(s).", No, Rows.size());
        PrintReport();
      } /* End of 'RunExp' function */

      /* Draw metric row function.
       * ARGUMENTS:
       *   - label:
       *       const CHAR *Name;
       *   - value:
       *       const std::string &Value;
       * RETURNS: None.
       */
      VOID MetricText( const CHAR *Name, const std::string &Value )
      {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted(Name);
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted(Value.c_str());
      } /* End of 'MetricText' function */

      /* Draw current metrics function.
       * ARGUMENTS:
       *   - row:
       *       const exp_row &Row;
       * RETURNS: None.
       */
      VOID DrawMetrics( const exp_row &Row )
      {
        const obj_metrics &M = Row.Metrics;

        if (ImGui::BeginTable("##cur_metrics", 2,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        {
          ImGui::TableSetupColumn("Metric", ImGuiTableColumnFlags_WidthStretch);
          ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, 150);
          ImGui::TableHeadersRow();
          MetricText("Geometry Validity", std::format("{} intersection(s)", M.GeometryValidity));
          MetricText("Accessibility", std::format("{:.2f} %", M.Accessibility));
          MetricText("Free Space Ratio", std::format("{:.4f}", M.FreeSpaceRatio));
          MetricText("Opening Clearance", std::format("{} blocked opening(s)", M.OpeningClearance));
          MetricText("Wall Alignment", std::format("{:.4f} m", M.WallAlignment));
          MetricText("Runtime", std::format("{:.3f} ms", M.RuntimeMs));
          MetricText("Candidate Count", std::format("{}", M.CandidateCount));
          ImGui::EndTable();
        }

        ImGui::Text("Accessibility");
        ImGui::ProgressBar((FLOAT)(M.Accessibility / 100.0), ImVec2(-1, 0),
          std::format("{:.2f} %", M.Accessibility).c_str());
        ImGui::Text("Free Space Ratio");
        ImGui::ProgressBar((FLOAT)M.FreeSpaceRatio, ImVec2(-1, 0),
          std::format("{:.4f}", M.FreeSpaceRatio).c_str());
      } /* End of 'DrawMetrics' function */

      /* Draw result table function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DrawRowsTable( VOID )
      {
        if (Rows.size() <= 1)
          return;
        if (!ImGui::BeginTable("##rows", 8,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
            ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY,
            ImVec2(0, 220)))
          return;

        ImGui::TableSetupColumn("Param");
        ImGui::TableSetupColumn("Geom");
        ImGui::TableSetupColumn("Access");
        ImGui::TableSetupColumn("Free");
        ImGui::TableSetupColumn("Open");
        ImGui::TableSetupColumn("Wall");
        ImGui::TableSetupColumn("Time");
        ImGui::TableSetupColumn("Cand");
        ImGui::TableHeadersRow();
        for (auto &Row : Rows)
        {
          ImGui::TableNextRow();
          ImGui::TableSetColumnIndex(0);
          ImGui::TextUnformatted(Row.Param.c_str());
          ImGui::TableSetColumnIndex(1);
          ImGui::Text("%d", Row.Metrics.GeometryValidity);
          ImGui::TableSetColumnIndex(2);
          ImGui::Text("%.2f", Row.Metrics.Accessibility);
          ImGui::TableSetColumnIndex(3);
          ImGui::Text("%.4f", Row.Metrics.FreeSpaceRatio);
          ImGui::TableSetColumnIndex(4);
          ImGui::Text("%d", Row.Metrics.OpeningClearance);
          ImGui::TableSetColumnIndex(5);
          ImGui::Text("%.4f", Row.Metrics.WallAlignment);
          ImGui::TableSetColumnIndex(6);
          ImGui::Text("%.3f", Row.Metrics.RuntimeMs);
          ImGui::TableSetColumnIndex(7);
          ImGui::Text("%d", Row.Metrics.CandidateCount);
        }
        ImGui::EndTable();
      } /* End of 'DrawRowsTable' function */

      /* Draw plot helper function.
       * ARGUMENTS:
       *   - label:
       *       const CHAR *Label;
       *   - values:
       *       const std::vector<FLOAT> &Values;
       * RETURNS: None.
       */
      VOID DrawPlot( const CHAR *Label, const std::vector<FLOAT> &Values )
      {
        if (Values.size() < 2)
          return;
        ImGui::PlotLines(Label, Values.data(), (INT)Values.size(),
          0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0, 70));
      } /* End of 'DrawPlot' function */

      /* Draw plots function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DrawPlots( VOID )
      {
        std::vector<FLOAT> Runtime {}, Geometry {}, Access {}, FreeSpace {};
        std::vector<FLOAT> Opening {}, Wall {}, Candidates {};

        if (Rows.size() < 2)
          return;
        for (auto &Row : Rows)
        {
          Runtime.push_back((FLOAT)Row.Metrics.RuntimeMs);
          Geometry.push_back((FLOAT)Row.Metrics.GeometryValidity);
          Access.push_back((FLOAT)Row.Metrics.Accessibility);
          FreeSpace.push_back((FLOAT)Row.Metrics.FreeSpaceRatio);
          Opening.push_back((FLOAT)Row.Metrics.OpeningClearance);
          Wall.push_back((FLOAT)Row.Metrics.WallAlignment);
          Candidates.push_back((FLOAT)Row.Metrics.CandidateCount);
        }

        ImGui::SeparatorText("Plots");
        DrawPlot("Runtime", Runtime);
        DrawPlot("Geometry Validity", Geometry);
        DrawPlot("Accessibility", Access);
        DrawPlot("Free Space Ratio", FreeSpace);
        DrawPlot("Opening Clearance", Opening);
        DrawPlot("Wall Alignment", Wall);
        DrawPlot("Candidate Count", Candidates);
      } /* End of 'DrawPlots' function */

      /* Draw experiment buttons function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DrawControl( anim *Ani )
      {
        ImGui::SetNextWindowPos(ImVec2(16, 16), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(330, 245), ImGuiCond_Once);
        ImGui::SetNextWindowCollapsed(FALSE, ImGuiCond_Always);
        ImGui::Begin("Ablation Control", nullptr, ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Room: %s", RoomName.c_str());
        ImGui::Text("Status: %s", Status.c_str());
        ImGui::Separator();

        for (INT i = 0; i < 9; i++)
        {
          const exp_desc &Info = Experiments[i];
          std::string Label = std::format("Exp {}##layout_exp_{}", Info.No, Info.No);

          if (ImGui::Button(Label.c_str(), ImVec2(82, 0)))
            RunExp(Info.No);
          if (ImGui::BeginItemTooltip())
          {
            ImGui::TextUnformatted(Info.Name);
            ImGui::TextWrapped("%s", Info.Desc);
            ImGui::EndTooltip();
          }
          if (i % 3 != 2)
            ImGui::SameLine();
        }

        if (ImGui::Button("Reload assets"))
        {
          IsReady = LoadScene(Ani);
          if (IsReady)
            RunExp(CurExp);
        }
        ImGui::End();
      } /* End of 'DrawControl' function */

      /* Draw research window function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DrawResearchWindow( VOID )
      {
        const exp_desc &Info = GetExpDesc(CurExp);

        ImGui::SetNextWindowPos(ImVec2(16, 270), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(620, 570), ImGuiCond_Once);
        ImGui::SetNextWindowCollapsed(FALSE, ImGuiCond_Always);
        ImGui::Begin("Ablation Study", nullptr, ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Experiment %d", CurExp);
        ImGui::TextUnformatted(Info.Name);
        ImGui::TextWrapped("%s", Info.Desc);
        ImGui::Separator();
        ImGui::Text("Source flat: %s", SourceFlat.c_str());
        ImGui::Text("Fixed flat: %s", FixedFlat.c_str());
        ImGui::Text("Room: %s", RoomName.c_str());

        if (!Rows.empty())
        {
          const exp_row &Row = Rows.back();

          ImGui::Text("Current parameters: %s", Row.Param.c_str());
          ImGui::Text("Solver status: %s", Row.Status.c_str());
          DrawMetrics(Row);
          DrawRowsTable();
          DrawPlots();
        }
        else
          ImGui::TextUnformatted("No results yet.");

        ImGui::End();
      } /* End of 'DrawResearchWindow' function */

    public:
      /* Class constructor.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       */
      layout_science( anim *Ani )
      {
        logger::info << "Layout ablation study unit initialization.";
        WorkFlat.Create(Ani);
        CameraLight = Ani->LightCreate(light_type::ePOINT, "Layout science camera light",
          Ani->Cam.Loc, 6.0f, -Ani->Cam.Dir, vec3(1, 1, 0.98), 2, 47.0f);
        IsReady = LoadScene(Ani);
        ResetReportFile();
        if (IsReady)
        {
          if (GetEnvironmentVariableA("TAM_LAYOUT_SCIENCE_RUN_ALL", nullptr, 0) > 0)
            for (INT ExpNo = 1; ExpNo <= 9; ExpNo++)
              RunExp(ExpNo);
          else
            RunExp(1);
        }
        if (GetEnvironmentVariableA("TAM_LAYOUT_SCIENCE_EXIT", nullptr, 0) > 0)
          PostQuitMessage(0);
      } /* End of 'layout_science' function */

      /* Render unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Render( anim *Ani ) override
      {
        (VOID)Ani;
        WorkFlat.DrawFlat();
      } /* End of 'Render' function */

      /* Response unit function.
       * ARGUMENTS:
       *   - animation context pointer:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
        if (CameraLight != nullptr)
        {
          CameraLight->Pos = Ani->Cam.Loc;
          CameraLight->Dir = Ani->Cam.Dir;
          CameraLight->Update();
        }
        DrawControl(Ani);
        DrawResearchWindow();
      } /* End of 'Response' function */

      /* Class destructor */
      ~layout_science( VOID ) override
      {
        logger::info << "Layout ablation study unit deinitialization.";
      } /* End of '~layout_science' function */
    }; /* End of 'layout_science' class */

    static anim::unit_register<layout_science> _("Layout science unit");
  } /* end of anonymous namespace */
} /* end of 'tam' namespace */

/* END OF 'tam_unit_layout_science.cpp' FILE */
