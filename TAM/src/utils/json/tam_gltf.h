/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_gltf.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               GLTF and GLB handle declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 22.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_gltf_h_
#define __tam_gltf_h_

#include "tam_def.h"
//#include "tam_json.h"

/* 'tam' namespace */
namespace tam
{
  /* 'gltf' handle class */
  class gltf
  {
  public:
    /* Buffer representation type */
    struct gltf_buffer
    {
      std::vector<BYTE> Data; // Buffer data
    }; /* End of 'gltf_buffer' structure */

    /* Buffer view representation type */
    struct gltf_buffer_view
    {
      INT Buffer;        // Buffer number at buffers array
      SIZE_T Offset;     // Offset in buffer in bytes
      SIZE_T Length;     // Length of data in bytes
      INT Stride;        // Stride between stored data or 0  for comact packing
    }; /* End of 'gltf_buffer_view' structure */

    /* Component types */
    enum class gltf_component_type
    {
      // Type of components: BYTE, WORD, DWORD, FLT, etc.
      eI8 = 0x1400, eU8 = 0x1401, eI16 = 0x1402, eU16 = 0x1403, eI32 = 0x1404, eU32 = 0x1405, eF32 = 0x1406,
    }; /* End of 'gltf_component_type' enumeration */

    /* Accessor types */
    enum class gltf_accessor_type
    {
      // Type of accessors: "SCALAR", "VEC2", "VEC3", "VEC4", "MAT4", etc.
      eSCALAR, eVEC2, eVEC3, eVEC4, eMAT2, eMAT3, eMAT4,
    }; /* End of 'gltf_accessor_type' enumeration */

    enum class gltf_alpha_mode
    {
      // Alpha mode: "OPAQUE", "MASK", "BLEND"
      eOPAQUE, eMASK, eBLEND,
    };

    // Component types sizes
    std::map<gltf_component_type, INT> ComponentTypes =
    {
      {gltf_component_type::eI8, 1},
      {gltf_component_type::eU8, 1},
      {gltf_component_type::eI16, 2},
      {gltf_component_type::eU16, 2},
      {gltf_component_type::eI32, 4},
      {gltf_component_type::eU32, 4},
      {gltf_component_type::eF32, 4}
    };

    // Accessor types number of components
    std::map<gltf_accessor_type, INT> AccessorTypesCount =
    {
      {gltf_accessor_type::eSCALAR, 1},
      {gltf_accessor_type::eVEC2, 2},
      {gltf_accessor_type::eVEC3, 3},
      {gltf_accessor_type::eVEC4, 4},
      {gltf_accessor_type::eMAT2, 4},
      {gltf_accessor_type::eMAT3, 9},
      {gltf_accessor_type::eMAT4, 16}
    };
 
    /* Draw mode types */
    enum class gltf_draw_mode_type
    {
      // Type of primitive topology: point, triangle, etc.
      ePOINTS = 0, eLINES = 1, eLINE_LOOP = 2, eLINE_STRIP = 3, eTRIANGLES = 4, eTRIANGLE_STRIP = 5, eTRIANGLE_FAN = 6,
    }; /* End of 'gltf_accessor_type' enumeration */

    /* Accessor representation type */
    struct gltf_accessor
    {
      INT BufferView;                    // Buffer view index
      SIZE_T ByteOffset;                 // Offset from buffer view offset
      gltf_component_type ComponentType; // Stored component type
      gltf_accessor_type Type;           // Accessor type
      SIZE_T Count;                      // Data unit count
      BOOL IsNormalized;                 // Normalized flag
    }; /* End of 'gltf_accessor' structure */
 
    /* Primitive representation type */
    struct gltf_primitive
    {
      INT PositionAccessor;  // Position accessor number or -1 if absent.
      INT NormalAccessor;    // Normal accessor number or -1 if absent.
      INT TexCoord0Accessor; // Texture coordinates accessor number or -1 if absent.
      INT TexCoord1Accessor; // Texture coordinates accessor number or -1 if absent.
      INT Color0Accessor;    // Color accessor number or -1 if absent.
      INT TangentAccessor;   // Tangent accessor number or -1 if absent.
      INT IndexAccessor;     // Index buffer accessor number or -1 if absent.
      INT JointsAccessor;    // Joints accessor number or -1 if absent.
      INT WeightsAccessor;   // Weights accessor number or -1 if absent.
      INT Mode;              // OpenGL primitive mode: 4 - GL_TRIANGLES etc.
      INT MaterialNo = 0;    // Material index.
    }; /* End of 'gltf_primitive' structure */

    /* Mesh representation type */
    struct gltf_mesh
    {
      std::string Name {};                  // Mesh name
      std::vector<gltf_primitive> Prims {}; // Mesh primitives
    }; /* End of 'gltf_mesh' structure */

    // Material representation type
    struct gltf_material
    {
      std::string Name {};               // Material name
      gltf_alpha_mode AlphaMode;         // Material alpha mode
      INT AlphaCutOff;                    // Alpha cut off when AlphaMode is MASK
      INT TextureNo = -1;                // Texture index
      INT MetRoughTexNo;                 // Metallic-Roughness texture index
      INT NormalMapTexNo = -1;           // Normal map texture index
      INT EmissiveTexNo = -1;            // Emissive texture index
      mth::vec4<FLT> BaseColorFactor {}; // Base color factor
      mth::vec3<FLT> Factors {};         // Metallic, roughness, occlusion
    }; /* End of 'gltf_material' structure */

    /* Node structure */
    struct gltf_node
    {
      std::string Name {};          // Node name
      INT Mesh = -1;                // Mesh index
      std::vector<INT> Subnodes {}; // Node's children
      mesh Geometry {};             // Node's geometry (if exists), fills in model loading at primitives creation
      INT Skin = -1;                // Node's skin
      INT Index = 0;                // Node's index (in nodes' array)
      BOOL IsMatrUsed;              // Flag if matrix is used
      matr
        Matrix {},                  // Current transformation matrix
        BaseMatrix {};              // Initial transformation matrix
      vec4                          
        Rotation {},                // Current rotation quaternion
        BaseRotation {};            // Initial rotation quaternion
      vec3                          
        Scale {},                   // Current scale coeffitients
        BaseScale{},                // Initial scale coeffitients
        Translation {},             // Current translation coords
        BaseTranslation {};         // Initial translation coords
    }; /* End of 'gltf_node' structure */

    /* Scene representation type */
    struct gltf_scene
    {
      std::string Name {};           // Scene name
      std::vector<INT> RootNodes {}; // Root nodes index
    }; /* End of 'gltf_scene' structure */

    /* Skin representation type */
    struct gltf_skin
    {
      INT InvBindMatrAccessor;       // Inverse bind matrices accessor
      std::vector<INT> Joints {};    // Indices of nodes that define the skeleton hierarchy
      std::vector<matr::matr_data> Matrices {}; // Matrices vector read from buffer using accessor
    }; /* End of 'gltf_skin' structure */

    /* Image representation type */
    struct gltf_image
    {
      std::string Name {};            //Image name
      std::vector<BYTE> ImageData {}; // Image data bytes
    }; /* End of 'gltf_image' structure */

    /* Sampler filter types */
    enum class gltf_sampler_filter_type
    {
      // Type of filters: "NEAREST", "LINEAR", etc.
      eNEAREST = 0x2600, eLINEAR = 0x2601, eNEAREST_MIPMAP_NEAREST = 0x2700,
      eLINEAR_MIPMAP_NEAREST = 0x2701, eNEAREST_MIPMAP_LINEAR = 0x2702,
      eLINEAR_MIPMAP_LINEAR = 0x2703
    }; /* End of 'gltf_sampler_filter_type' enumeration */

    /* Sampler addressing types */
    enum class gltf_sampler_address_type
    {
      // Type of addressing: "REPEAT", "CLAMP", etc.
      eCLAMP = 0x2900, eREPEAT = 0x2901, eCLAMP_TO_EDGE = 0x812F
    }; /* End of 'gltf_sampler_address_type' enumeration */

    /* Sampler representation type */
    struct gltf_sampler
    {
      gltf_sampler_filter_type MagFilter, MinFilter; // Magnify and minimize filters
      /* Sampler filter types */
      gltf_sampler_address_type AddressS, AddressT; // Sampler addressing types (by S and T)
    }; /* End of 'gltf_sampler' structure */

    /* Texture representation type */
    struct gltf_texture
    {
      std::string Name {}; // Texture name
      gltf_image Image;    // Image binded to texture
      INT SamplerNo;       // Sampler index
    }; /* End of 'gltf_texture' structure */

    /* JSON structure */
    struct gltf_anim_transform_struct :
      public std::variant<vec3, vec4>
    {
      using variant::variant;

      /* Get value as 3D vector function.
       * ARGUMENTS:
       *   - default value to be set:
       *       vec3 DefaultValue;
       * RETURNS:
       *   (vec3) reference to 3D vector.
       */
      vec3 GetVec3( vec3 DefaultValue = vec3(0) ) const
      {
        if (std::holds_alternative<vec3>(*this))
          return std::get<vec3>(*this);
        return vec3(0);
      } /* End of 'GetVec3' function */

      /* Get value as 4D vector function.
       * ARGUMENTS:
       *   - default value to be set:
       *       vec4 DefaultValue;
       * RETURNS:
       *   (vec4) reference to 4D vector.
       */
      vec4 GetVec4( vec4 DefaultValue = vec4(0) ) const
      {
        if (std::holds_alternative<vec4>(*this))
          return std::get<vec4>(*this);
        return vec4(0);
      } /* End of 'GetVec4' function */
    }; /* End of 'gltf_anim_transform_struct' structure */

    /* Animation transformation type (for vectors, matrices) */
    enum class gltf_anim_transform_type
    {
      eTRANSLATE, eROTATE, eSCALE, eWEIGHTS
    }; /* End of 'gltf_transform_type' struct */

    /* Animation sampler representation type */
    struct gltf_anim_sampler
    {
      std::vector<FLT> Timestamps {}; // Timestamps vector (input accessor)
      std::string Interpolation {}; // Interpolation type
      std::vector<gltf_anim_transform_struct> Transform {}; // Output transformation
      INT OutputAccessor; // Output accessor index
    }; /* End of 'gltf_anim_sampler' structure */

    /* Animation channel structure */
    struct gltf_anim_channel
    {
      INT Sampler {}; // Sampler index
      INT NodeIndex {}; // Node index
      gltf_anim_transform_type TransformType; // Morph path (translation, rotation, scale)
      std::vector<FLT> Timestamps {}; // Timestamps vector (input accessor)
      std::string Interpolation {}; // Interpolation type
      std::vector<gltf_anim_transform_struct> Transform {}; // Output transformation
      INT OutputAccessor; // Output accessor index
    }; /* End of 'gltf_channel' structure */

    /* Animation representation type */
    struct gltf_animation
    {
      std::string Name {};                        // Animation name
      FLT MaxLength = 0;                          // The longest animation time
      std::vector<gltf_anim_channel> Channels {}; // Animation channels vector
    }; /* End of 'gltf_animation' structure */

    /* PBR vertex structure */
    struct pbr_vertex
    {
      vec3 P;           // Vertex position
      vec2 T;           // Vertex texture coordinates
      vec3 N;           // Vertex normal
      vec4 C;           // Vertex color
      vec4 Tng;         // Vertex tangent
      mth::vec4<INT> J; // Vertex joints
      vec4 W;           // Vertex weights (used for joints matrices)
    }; /* End of 'pbr_vertex' structure */

  public:
    std::vector<gltf_scene> Scenes {};            // Scenes vector
    std::vector<gltf_skin> Skins {};              // Skins vector
    std::vector<gltf_node> Nodes {};              // Nodes vector
    std::vector<gltf_mesh> Meshes {};             // Primitives vector
    std::vector<gltf_accessor> Accessors {};      // Accessors vector
    std::vector<gltf_buffer_view> BufferViews {}; // Buffer views vector
    std::vector<gltf_buffer> Buffers {};          // Buffers vector
    std::vector<gltf_material> Materials {};      // GLTF materials vector
    std::vector<gltf_texture> Textures {};        // GLTF textures vector
    std::vector<gltf_image> Images {};            // Images vector
    std::vector<gltf_sampler> Samplers {};        // Samplers vector
    std::vector<gltf_animation> Animations {};    // Animations vector

    std::vector<BYTE> GlbBinaryBufferData {};     // Data of binary buffer of .GLB models (empty if model isn't .GLB)

    std::vector<matr::matr_data>
      JointMatrs {},    // Joint matrices buffer
      InvJointMatrs {}; // Joint inverse matrices buffer

    INT SceneNo; // Scene index
    INT SkinNo; // Skin index

    BOOL IsAnim = FALSE;   // Flag if model should be animated or not
    BOOL HasTrans = FALSE; // Flag if model has materials with transparency

    /* Get accessor data span function.
     * ARGUMENTS:
     *   - accessor index:
     *       INT AccessorIndex;
     * RETURNS:
     *   (std::vector<type>) result span value (slice from buffer).
     */
    template<typename type>
      std::vector<type> GetAccessorData( INT AccessorIndex )
      {
        std::vector<type> &&res {};
        if (AccessorIndex == -1)
          return res;
        gltf_accessor &ac = Accessors[AccessorIndex];
        if (AccessorTypesCount[ac.Type] * ComponentTypes[ac.ComponentType] != sizeof(type))
          return res;
        gltf_buffer_view &bv = BufferViews[ac.BufferView];
        INT stride = bv.Stride;
        if (stride == 0)
          stride = sizeof(type);
        std::span<BYTE> bv_data(&Buffers[bv.Buffer].Data[bv.Offset], bv.Length);
        std::span<BYTE> ac_data(&bv_data[ac.ByteOffset], ac.Count * stride);
        res.resize(ac.Count);
        if (bv.Stride == 0)
          memcpy(reinterpret_cast<VOID *>(&res[0]), reinterpret_cast<VOID *>(&ac_data[0]), ac.Count * sizeof(type));
        else
          for (INT i = 0; i < ac.Count; i++)
            memcpy(reinterpret_cast<VOID *>(&res[i]), reinterpret_cast<VOID *>(&ac_data[stride * i]), sizeof(type));
        return res;
      } /* End of 'GetAccessorData' function */

    /* Buffer decoding function.
     * ARGUMENTS:
     *   - buffer to be decoded:
     *       const std::string &Data;
     *   - data size:
     *       UINT Size;
     * RETURNS:
     *   (std::vector<BYTE>) decoded buffer.
     */
    static std::vector<BYTE> DecodeBuf( const std::string &Data, UINT Size )
    {
      std::vector<BYTE> &&res = {};
      static const std::string bs("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");

      if (Size == -1)
        Size = static_cast<INT>(Data.length());
      res.resize(Size);
      auto inpos = Data.find(";base64,");
      if (inpos == std::string::npos)
        return {};
      inpos += 8; // skip base64 head text
      SIZE_T outpos = 0;
 
      BYTE N[4] {};
      DWORD BR;
      INT k = 0;
      auto i = inpos;
      for (; i < Data.length(); i++)
      {
        if (auto pos = bs.find(Data[i]); pos != std::string::npos)
        {
          N[k++] = static_cast<BYTE>(pos);
          if (k == 4)
          {
            k = 0;
            BR = (N[0] << 18) | (N[1] << 12) | (N[2] << 6) | N[3];
            N[0] = N[1] = N[2] = N[3] = 0;
            for (INT p = 0; p < 3; p++)
              if (outpos < Size)
                res[outpos++] = reinterpret_cast<BYTE *>(&BR)[2 - p];
          }
        }
      }
      if (k != 4)
      {
        BR = (N[0] << 18) | (N[1] << 12) | (N[2] << 6) | N[3];
        for (INT p = 0; p < 3; p++)
          if (outpos < Size)
            res[outpos++] = reinterpret_cast<BYTE *>(&BR)[2 - p];
      }
      res.resize(outpos);
      return res;
    } /* End of 'DecodeBuf' function */

    /* Parse (.GLTF) file function.
     * ARGUMENTS:
     *   - name of file to load model from:
     *       const std::string &FileName;
     *   - transform matrix:
     *       const matr &Transform;
     *   - size of gltf data from start of reading:
     *       INT Size = -1;
     *   - offset from start of file:
     *       INT Offset = 0;
     *  RETURNS: None.
     */
    VOID LoadGLTF( const std::string &FileName, const matr &Transform = matr::Identity(), INT Size = -1, INT Offset = 0 )
    {
      json js {};
      json_loader loader {};
      std::string buf {};
      js = loader.Load(FileName, Size, Offset);
      std::string path = std::filesystem::path(FileName).parent_path().string() + "\\";

      // Read buffers
      auto buffers = js["buffers"].GetArray();
      for (INT i = 0; i < buffers.size(); i++)
      {
        auto b = buffers[i];

        const std::string uri = b["uri"].GetString();
        INT buffer_length = b["byteLength"].GetInt();
        gltf_buffer buf;

        if (std::filesystem::exists(path + uri) && uri != "")
        {
          // Read buffer from file
          std::ifstream f(path + uri, std::ios::binary | std::ios::ate);
          auto flen = f.tellg();
          std::vector<BYTE> filebuf(flen);
          f.seekg(0, std::ios::beg);
          f.read(reinterpret_cast<char*>(filebuf.data()), flen);
          buf.Data = std::move(filebuf);
        }
        else if (uri.find(";base64,") != std::string::npos)
          // Decode buffer from string
          buf.Data = DecodeBuf(uri, buffer_length);
        // Handle glb binary buffer
        else if (Offset != 0)
          buf.Data = GlbBinaryBufferData;
        else
          throw std::runtime_error("File (buffer) " + uri + " is not found");
        Buffers.emplace_back(buf);
      }

      // Read buffer views
      auto buffer_views = js["bufferViews"].GetArray();
      BufferViews.resize(buffer_views.size());
      for (INT i = 0; i < buffer_views.size(); i++)
      {
        auto bv = buffer_views[i];
        BufferViews[i].Buffer = bv["buffer"].GetInt();
        BufferViews[i].Length = bv["byteLength"].GetInt();
        BufferViews[i].Offset = bv["byteOffset"].GetInt();
        BufferViews[i].Stride = bv["byteStride"].GetInt();
      }

      // Read accessors
      auto accessors = js["accessors"].GetArray();
      Accessors.resize(accessors.size());
      for (INT i = 0; i < accessors.size(); i++)
      {
        auto a = accessors[i];
        Accessors[i].BufferView = a["bufferView"].GetInt();
        Accessors[i].ByteOffset = a["byteOffset"].GetInt();
        INT cType = a["componentType"].GetInt();
        Accessors[i].ComponentType = cType == 5126 ? gltf_component_type::eF32 : cType == 5123 ? gltf_component_type::eU16 : gltf_component_type::eU32;
        Accessors[i].Count = a["count"].GetInt();
        std::string aType = a["type"].GetString();
        Accessors[i].Type = aType == "VEC2" ? gltf_accessor_type::eVEC2 : aType == "VEC3" ? gltf_accessor_type::eVEC3 : aType == "VEC4" ?
          gltf_accessor_type::eVEC4 : aType == "MAT2" ? gltf_accessor_type::eMAT2 : aType == "MAT3" ? gltf_accessor_type::eMAT3 :
          aType == "MAT4" ? gltf_accessor_type::eMAT4 : gltf_accessor_type::eSCALAR;
        Accessors[i].IsNormalized = FALSE;
      }

      // Read meshes with primitives
      for (auto &ms : js["meshes"].GetArray())
      {
        std::vector<gltf_primitive> Prims;
        for (auto &pr : ms["primitives"].GetArray())
        {
          INT p =  pr["mode"].GetInt(static_cast<INT>(gltf_draw_mode_type::eTRIANGLES));
          Prims.push_back(gltf_primitive
          {
            .PositionAccessor = pr["attributes"]["POSITION"].GetInt(-1),
            .NormalAccessor = pr["attributes"]["NORMAL"].GetInt(-1),
            .TexCoord0Accessor = pr["attributes"]["TEXCOORD_0"].GetInt(-1),
            .TexCoord1Accessor = pr["attributes"]["TEXCOORD_1"].GetInt(-1),
            .Color0Accessor = pr["attributes"]["COLOR_0"].GetInt(-1),
            .TangentAccessor = pr["attributes"]["TANGENT"].GetInt(-1),
            .IndexAccessor = pr["indices"].GetInt(-1),
            .JointsAccessor = pr["attributes"]["JOINTS_0"].GetInt(-1),
            .WeightsAccessor = pr["attributes"]["WEIGHTS_0"].GetInt(-1),
            .Mode = p,
            .MaterialNo = pr["material"].GetInt(-1)
          });
        }
        Meshes.push_back(gltf_mesh
        {
          .Name = ms["name"].GetString(),
          .Prims = std::move(Prims),
        });
      }

      INT Ind = 0;
      /* Load nodes from JSON */
      for (auto &Node : js["nodes"].GetArray())
      {
        std::vector<INT> Children {};
        for (auto &Child : Node["children"].GetArray())
          Children.push_back((INT)Child.GetInt());
        std::vector<INT> Weights {};
        for (auto &Weight : Node["weights"].GetArray())
          Weights.push_back(Weight.GetInt());
        vec4 Rotations = vec4(0);
        vec3 Scales = vec3(1);
        vec3 Translates = vec3(0);
        matr Matr = matr::Identity();
        BOOL IsMatrUse = FALSE;
        json_array MatrArr = Node["matrix"].GetArray();
        if (MatrArr.size() > 0)
        {
          IsMatrUse = TRUE;
          for (INT i = 0; i < 16; i++)
            Matr[i] = MatrArr[i].GetDouble();
        }
        else
        {
          auto ndrotate = Node["rotation"].GetArray();
          if (ndrotate.size() > 0)
            Rotations = vec4(
              (FLT)ndrotate[0].GetDouble(),
              (FLT)ndrotate[1].GetDouble(),
              (FLT)ndrotate[2].GetDouble(),
              (FLT)ndrotate[3].GetDouble(1)
            );
          auto ndscale = Node["scale"].GetArray();
          if (ndscale.size() > 0)
            Scales = vec3(ndscale[0].GetDouble(1), ndscale[1].GetDouble(1), ndscale[2].GetDouble(1));
          auto ndtrans = Node["translation"].GetArray();
          if (ndtrans.size() > 0)
            Translates = vec3(ndtrans[0].GetDouble(1), ndtrans[1].GetDouble(1), ndtrans[2].GetDouble(1));
          matr
            Scale = matr::Scale(Scales),
            Rotate = matr::RotateQuat(Rotations),
            Translate = matr::Translate(Translates);
          Matr =  Scale * Rotate * Translate;
        }
        Nodes.push_back(gltf_node
          {
            .Name = Node["name"].GetString(),
            .Mesh = Node["mesh"].GetInt(-1),
            .Subnodes = std::move(Children),
            .Skin = Node["skin"].GetInt(-1),
            .Index = Ind++,
            .IsMatrUsed = IsMatrUse,
            .Matrix = Matr,
            .BaseMatrix = Matr,
            .Rotation = Rotations,
            .BaseRotation = Rotations,
            .Scale = Scales,
            .BaseScale = Scales,
            .Translation = Translates,
            .BaseTranslation = Translates,
          });
      }

      // Read scenes
      auto scenes = js["scenes"].GetArray();
      Scenes.resize(scenes.size());
      for (INT i = 0; i < scenes.size(); i++)
      {
        auto sc = scenes[i];
        Scenes[i].Name = sc["name"].GetString();
        auto nds = sc["nodes"].GetArray();
        for (INT j = 0; j < nds.size(); j++)
          Scenes[i].RootNodes.push_back(nds[j].GetInt());
      }

      SceneNo = js["scene"].GetInt(-1);
      if (SceneNo == -1 && Scenes.size() > 0)
        SceneNo = 0;

      // Read skins
      auto skins = js["skins"].GetArray();
      Skins.resize(skins.size());
      for (INT i = 0; i < skins.size(); i++)
      {
        auto sk = skins[i];
        auto joints = sk["joints"].GetArray();
        for (INT j = 0; j < joints.size(); j++)
          Skins[i].Joints.push_back(joints[j].GetInt());
        Skins[i].InvBindMatrAccessor = sk["inverseBindMatrices"].GetInt();
        // Matrices
        if (auto matrdbl = GetAccessorData<mth::matr<DBL>::matr_data>(Skins[i].InvBindMatrAccessor); (UINT)matrdbl.size() != 0)
          Skins[i].Matrices.reserve(matrdbl.size()), std::transform(matrdbl.begin(), matrdbl.end(), std::back_inserter(Skins[i].Matrices),
          []( mth::matr<DBL>::matr_data X )
            {
              return matr::matr_data(matr(X.M[0][0], X.M[0][1], X.M[0][2], X.M[0][3],
                X.M[1][0], X.M[1][1], X.M[1][2], X.M[1][3],
                X.M[2][0], X.M[2][1], X.M[2][2], X.M[2][3],
                X.M[3][0], X.M[3][1], X.M[3][2], X.M[3][3]));
            });
        else if (auto matrflt = GetAccessorData<matr::matr_data>(Skins[i].InvBindMatrAccessor); (UINT)matrflt.size() != 0)
          Skins[i].Matrices = std::move(matrflt);
      }

      SkinNo = js["skin"].GetInt(-1);
      if (Skins.size() > 0 && SkinNo == -1)
        SkinNo = 0;

      // Compute matrices
      std::function<VOID( INT, const matr & )> ComputeWorldMatrix =
        [this, &ComputeWorldMatrix]( INT NodeIndex, const matr &ParentWorld )
        {
          const gltf_node &nd = Nodes[NodeIndex];
          matr world = nd.Matrix * ParentWorld;
          Nodes[NodeIndex].Matrix = world;
          for (INT subnode : nd.Subnodes)
            ComputeWorldMatrix(subnode, world);
        };
      for (auto &sc : Scenes)
        for (auto &rootnd : sc.RootNodes)
          ComputeWorldMatrix(rootnd, Transform);

      for (auto &nd : Nodes)
      {
        JointMatrs.push_back(nd.Matrix);
        InvJointMatrs.push_back(matr::Identity().M);
      }

      for (auto &sk : Skins)
        for (INT i = 0; i < sk.Joints.size(); i++)
          InvJointMatrs[sk.Joints[i]] = sk.Matrices[i].M;

      // Read animations (if exist)
      auto animations = js["animations"].GetArray();
      Animations.resize(animations.size());
      if (animations.size() == 0)
        IsAnim = FALSE;
      else
      {
        IsAnim = TRUE;
        INT i = 0;
        std::vector<gltf_anim_sampler> Samplers {};
        for (auto &anim : animations)
        {
          Animations[i].Name = anim["name"].GetString();
          auto smpls = anim["samplers"].GetArray();
          for (auto smpl : smpls)
          {
            gltf_anim_sampler s {};
            s.Timestamps = GetAccessorData<FLT>(smpl["input"].GetInt());
            s.Interpolation = smpl["interpolation"].GetString("LINEAR");
            s.OutputAccessor = smpl["output"].GetInt();
            Samplers.push_back(s);
          }
          auto chnls = anim["channels"].GetArray();
          for (auto &chnl : chnls)
          {
            gltf_anim_channel ch {};
            ch.Sampler = chnl["sampler"].GetInt();
            ch.NodeIndex = chnl["target"]["node"].GetInt();
            ch.TransformType =
              chnl["target"]["path"].GetString() == "translation" ? gltf_anim_transform_type::eTRANSLATE :
              chnl["target"]["path"].GetString() == "rotation" ? gltf_anim_transform_type::eROTATE :
              chnl["target"]["path"].GetString() == "scale" ? gltf_anim_transform_type::eSCALE :
              gltf_anim_transform_type::eWEIGHTS;
            ch.Interpolation = Samplers[ch.Sampler].Interpolation;
            ch.OutputAccessor = Samplers[ch.Sampler].OutputAccessor;
            ch.Timestamps = Samplers[ch.Sampler].Timestamps;
            Animations[i].MaxLength = ch.Timestamps.back() >= Animations[i].MaxLength ?
              ch.Timestamps.back() : Animations[i].MaxLength;
            ch.Transform = Samplers[ch.Sampler].Transform;
            if (Accessors[Samplers[ch.Sampler].OutputAccessor].Type == gltf_accessor_type::eVEC3)
            {
              auto v = GetAccessorData<vec3>(ch.OutputAccessor);
              ch.Transform.reserve(v.size()), std::transform(v.begin(), v.end(),
                std::back_inserter(ch.Transform),
                []( vec3 X ){ return gltf_anim_transform_struct(X); });
            }
            else
            {
              auto v = GetAccessorData<vec4>(ch.OutputAccessor);
              ch.Transform.reserve(v.size()), std::transform(v.begin(), v.end(),
                std::back_inserter(ch.Transform),
                []( vec4 X ){ return gltf_anim_transform_struct(X); });
            }
            Animations[i].Channels.push_back(ch);
          }
          i++;
        }
      }

      // Read images
      for (auto &img : js["images"].GetArray())
      {
        std::string Name {};
        const std::string uri = img["uri"].GetString();
        const std::string mime = img["mimeType"].GetString();
        INT bv_no = img["bufferView"].GetInt(-1);
        const std::string name = img["name"].GetString();
        std::vector<BYTE> data;
        if (uri != "" && std::filesystem::exists(path + uri))
        {
          // Read image from file
          std::ifstream f(path + uri, std::ios::binary | std::ios::ate);
          auto flen = f.tellg();
          std::vector<BYTE> filebuf(flen);
          f.seekg(0, std::ios::beg);
          f.read(reinterpret_cast<char*>(filebuf.data()), flen);
          data = std::move(filebuf);
          Name = "File: " + uri;
        }
        else if (uri.find(";base64,") != std::string::npos)
        {
          // Decode image from string
          Name = "From string encoding uri Base64";
          data = DecodeBuf(uri, -1);
        }
        else if (bv_no != -1)
        {
          gltf_buffer_view &bv = BufferViews[bv_no];
          std::span<BYTE> bv_data(&Buffers[bv.Buffer].Data[bv.Offset], bv.Length);
          data.resize(bv.Length);
          memcpy(reinterpret_cast<VOID *>(&data[0]), reinterpret_cast<VOID *>(&bv_data[0]), bv.Length);
          Name = "From buffer:" + Name;
        }
        else
        {
          /// throw std::runtime_error("Image data is not found/load");
          Name = "No data found file";
          data = {0, 0, 0, 0};
        }
        Images.emplace_back(gltf_image
        {
          .Name = Name,
          .ImageData = std::move(data),
        });
      }

      // Read samplers
      for (auto &smpl : js["samplers"].GetArray())
      {
        Samplers.push_back(gltf_sampler
        {
          .MagFilter = static_cast<gltf_sampler_filter_type>(smpl["magFilter"].GetInt(static_cast<INT>(gltf_sampler_filter_type::eNEAREST))),
          .MinFilter = static_cast<gltf_sampler_filter_type>(smpl["minFilter"].GetInt(static_cast<INT>(gltf_sampler_filter_type::eNEAREST))),
          .AddressS = static_cast<gltf_sampler_address_type>(smpl["wrapS"].GetInt(static_cast<INT>(gltf_sampler_address_type::eREPEAT))),
          .AddressT = static_cast<gltf_sampler_address_type>(smpl["wrapT"].GetInt(static_cast<INT>(gltf_sampler_address_type::eREPEAT))),
        });
      }

      // Read textures
      for (auto &tex : js["textures"].GetArray())
      {
        Textures.push_back(gltf_texture
        {
          .Name = tex["name"].GetString(),
          .Image = Images[tex["source"].GetInt(-1)],
          .SamplerNo = tex["sampler"].GetInt(-1),
        });
      }

      // Read materials
      auto mtls = js["materials"].GetArray();
      for (auto &mtl : mtls)
      {
        auto AlphaMode = mtl["alphaMode"].GetString();
        if (!HasTrans && AlphaMode == "MASK" || AlphaMode == "BLEND")
          HasTrans = TRUE;
        auto AlphaCutOff = mtl["alphaCutOff"].GetInt(-1);
        auto bcf = mtl["pbrMetallicRoughness"]["baseColorFactor"].GetArray();
        if (bcf.size() == 0)
          bcf = mtl["extensions"]["KHR_materials_pbrSpecularGlossiness"]["diffuseFactor"].GetArray();
        auto tex = mtl["pbrMetallicRoughness"]["baseColorTexture"]["index"].GetInt(-1);
        auto mr = mtl["pbrMetallicRoughness"]["metallicRoughnessTexture"]["index"].GetInt(-1);
        auto nm = mtl["normalTexture"]["index"].GetInt(-1);
        auto emi = mtl["pbrMetallicRoughness"]["emissiveTexture"]["index"].GetInt(-1);
        if (tex == -1)
          tex = mtl["extensions"]["KHR_materials_pbrSpecularGlossiness"]["diffuseTexture"]["index"].GetInt(-1);
        Materials.push_back(gltf_material
        {
          .Name = mtl["name"].GetString(),
          .AlphaMode = (AlphaMode == "MASK") ? gltf_alpha_mode::eMASK : (AlphaMode == "BLEND") ? gltf_alpha_mode::eBLEND : gltf_alpha_mode::eOPAQUE,
          .AlphaCutOff = AlphaCutOff,
          .TextureNo = tex,
          .MetRoughTexNo = mr,
          .NormalMapTexNo = nm,
          .EmissiveTexNo = emi,
          .BaseColorFactor = bcf.size() == 0 ? mth::vec4<FLT>(1.0, 1.0, 1.0, 1.0) :
            mth::vec4<FLT>{static_cast<FLT>(bcf[0].GetDouble()), static_cast<FLT>(bcf[1].GetDouble()),
            static_cast<FLT>(bcf[2].GetDouble()), static_cast<FLT>(bcf[3].GetDouble(1))},
          .Factors = mth::vec3<FLT>{static_cast<FLT>(mtl["metallicFactor"].GetDouble(1.0)),
            static_cast<FLT>(mtl["roughnessFactor"].GetDouble(1.0)),
            static_cast<FLT>(mtl["occlusionFactor"].GetDouble(1.0))},
        });
      };
    } /* End of 'LoadGLTF' function */
  }; /* End of 'gltf' class */
} /* end of 'tam' namespace */

#endif /* __tam_gltf_h_ */

/* END OF 'tam_gltf.h' FILE */
