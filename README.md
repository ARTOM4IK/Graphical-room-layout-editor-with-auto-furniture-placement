# Graphical Room Layout Editor with Auto Furniture Placement

[![C++](https://img.shields.io/badge/C++-20-blue.svg)](https://isocpp.org/)
[![Vulkan](https://img.shields.io/badge/Vulkan-1.4-red.svg)](https://www.vulkan.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/License-Proprietary-yellow.svg)](LICENSE)
[![Status](https://img.shields.io/badge/Status-Experimental-orange.svg)]()

Interactive 2D/3D interior design tool with a layout solver that automatically arranges furniture in rooms, respecting geometric constraints, functional requirements, and human-centric aesthetics. Powered by Vulkan and a custom combinatorial optimisation engine.

**Developer:** Arsentev Artemy (AA4)

---

## Important Notes

This repository is a **mirror** of a project originally developed in a private Perforce repository. As such:

- **Commit history** – All commits remain in the original Perforce repository. The author will attempt to export the full history at a later date.
- **Missing files** – External library binaries and the `bin/` folder containing shaders, textures, and models are **not included** in this repository. The project will not build or run without these files.
- **Experimental status** – This is a research/demonstration project, not a production-ready engine. It is provided for educational and reference purposes only.
- **No copying** – The code and assets are proprietary. Please refer to the license section below.
- **To overview project** - watch the video: https://youtu.be/tCcxkZN85MM
---

## Key Features

- **Interactive floor plan editor** – point & segment wall editing, room creation from polygons, procedural geometry with configurable thickness and height, doors and windows, undo/redo.
- **Vulkan-based 3D rendering** – deferred PBR pipeline, multiple passes (opaque, transparent, shadow, merge, post‑processing), dynamic lights (point, directional, spot), live shader hot‑reload.
- **Automatic furniture placement solver** – candidate generation (wall‑aligned, corner, grid), hard constraints (room boundary, opening keepout, overlap, front clearance), soft preferences (wall proximity, facing interior, cardinal rotations), pairwise rules (chair‑table distance/facing, bathroom grouping), simulated annealing, and a trained linear logistic ranking model (20 features) to score candidate quality.
- **Supported room types** – bedroom, living room, office, bathroom, kitchen, dining room, laundry room, generic.
- **Furniture roles** – anchor, storage, table, seating, center.
- **Editor subsystems** – parameters (load/save/clear, lighting), walls, rooms (textures, colours), furniture collection management, interactive placement, AI bot for one‑click auto‑arrangement.

---

## Architecture Overview

The application is structured in layers:

- **System layer** – Win32 windowing, high‑resolution timer, input (keyboard, mouse, joystick), Vulkan core (instance, device, swapchain, memory allocator).
- **Mathematics library** – vectors, matrices, quaternions, computational geometry (OBB, AABB, BVH, ray, polygon triangulation).
- **Resource managers** – shaders, textures, materials, models, buffers, fonts, lights.
- **Render pipeline** – deferred shading with multiple passes, compute shader for world matrix transformations, instanced rendering.
- **Flat data model** – points, segments, rooms, furniture, procedural wall/floor/ceiling generation, JSON serialisation.
- **Layout solver** – problem builder, candidate generation, state evaluation (hard/soft/pairwise), simulated annealing, learn‑to‑rank model.
- **Editor subsystems** – specialised tabs (walls, rooms, furniture, placement, AI bot) built on a UX framework over Dear ImGui.

---

## Project Structure (Simplified)

```
src/
├── systems/
│   ├── anim/                      # Animation system
│   │   ├── input/                 # Keyboard, mouse, joystick, timer
│   │   └── render/                # Vulkan rendering pipeline
│   │       ├── vulkan/            # Vulkan core (instance, device, swapchain)
│   │       │   ├── vulkan_res/    # Resources (buffers, images, shaders)
│   │       │   └── vma/           # Vulkan Memory Allocator
│   │       └── res/               # High-level resources (materials, models, fonts)
│   ├── flat/                      # Flat data model and procedural geometry
│   └── win/                       # Windows subsystem
│
├── units/                         # Application units
│   ├── editor/                    # Editor sub-units (main, walls, rooms, furniture)
│   ├── samples/                   # Sample/demo units
│   └── tests/                     # Test units
│
├── utils/                         # Utilities
│   ├── ai/                        # Layout solver (tam_layout, tam_solver)
│   ├── ux/                        # UX framework (ImGui controls)
│   ├── imgui/                     # Dear ImGui integration
│   ├── image/                     # Image loading (WIC + native)
│   ├── json/                      # Custom JSON parser/saver
│   ├── log/                       # Colored console logging
│   ├── profiler/                  # Performance profiler
│   └── walls_editor/              # Wall editing system
│
├── mth/                           # Mathematics library (header-only)
│   ├── cg/                        # Computational geometry (OBB, BVH, triangulation)
│   └── mth_*.h                    # Vectors, matrices, quaternions, noise
│
└── tam_def.h                      # Common definitions and types
```

---

## Editor Controls (Quick Reference)

| Action | Keyboard / Mouse |
|--------|------------------|
| Add point | `LMB` on empty space |
| Select point | `LMB` on existing point |
| Add segment | Select two points (auto‑connects) |
| Move point/segment | `Shift + LMB + Drag` |
| Delete selected | `Y` |
| Clear selection | `E` or `Esc` |
| Undo | `Ctrl + Z` |
| Switch selection mode | `Q` (Points ↔ Segments) |
| Area selection | `Space + Drag` |
| Set window/door | `2` (Window) / `3` (Door) on selected segment |
| Toggle wall display | `G` |
| Camera orbit | `RMB + Drag` |
| Camera pan | `MMB + Drag` |
| Camera zoom | `Mouse Wheel` |
| Auto‑arrange room | `LMB` on a room in the AI Bot tab |

---

## Dependencies

- Vulkan 1.4
- Vulkan Memory Allocator
- Dear ImGui
- stb_image (fallback)

---

## Documentation

- Detailed architecture description – [Architecture_specification.md](docs/Architecture_specification.md)
- Project overview presentation – [TAM_Overview.pptx](docs/TAM_Overview.pptx)

---

## License

Proprietary. No part of this file may be changed without agreement of the Computer Graphics Support Group of 30 Phys‑Math Lyceum.

---

## Author

**Arsentev Artemy (AA4)** – Computer Graphics Support Group, 30th Phys‑Math Lyceum