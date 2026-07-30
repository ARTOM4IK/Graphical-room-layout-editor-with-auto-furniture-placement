/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res_models.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               Resources module.
 *               Models implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 01.05.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Class move constructor.
 * ARGUMENTS:
 *   - rvalue reference to model_res:
 *       model_res &&Old;
 */
tam::model_res::model_res( model_res &&Old ) : resource(Old.Rnd), Name(Old.Name), Transform(Old.Transform), MinBB(Old.MinBB), MaxBB(Old.MaxBB),
  Prims(Old.Prims), ModelType(Old.ModelType), GLTFData(Old.GLTFData), Geometry(Old.Geometry)
{
  Rnd = Old.Rnd;
  Old.Rnd = nullptr;
  MtlPat = Old.MtlPat;
  OpaqueMtlPat = Old.OpaqueMtlPat;
  TransMtlPat = Old.TransMtlPat;
  Old.ModelType = "";
  Old.GLTFData = {};
  Old.Geometry = {};
} /* End of 'tam::model_res::model_res' function */

/* Create model to shape function.
 * ARGUMENTS:
 *   - filename (.G3DM or .GLTF):
 *       const std::string &FileName;
 *   - transform matrix:
 *       const matr &NewTransform;
 * RETURNS:
 *   (model_res &) reference to loaded model.
 */
tam::model_res & tam::model_res::Create( const std::string &FileName, const matr &NewTransform )
{
  Free();

  // Load file into memory
  std::fstream f(FileName, std::fstream::in | std::fstream::binary);

  if (!f)
  {
    logger::error << std::format("Error while loading {} occured (unable to load file).", FileName);
    return *this;
  }

  f.seekg(0, std::fstream::end);
  std::size_t flen = f.tellg();
  f.seekg(0, std::fstream::beg);
 
  std::vector<BYTE> mem;
  mem.resize(flen);
  f.read((CHAR *)mem.data(), flen);

  BYTE *ptr = mem.data();

  rdr rd(ptr);

  // Load header
  DWORD Sign;
  rd(&Sign);
  if (Sign == *(DWORD *)"G3DM")
  {
    f.close();
    return LoadG3DM(FileName, NewTransform);
  }
  else
  {
    // Check for matching .GLB magic
    if (Sign ==  0x46546C67)
    {
      // Handle .GLB model loading
      INT GlbVersion, GlbSize, JsonSize, BinBufferSize;

      rd(&GlbVersion);
      rd(&GlbSize);
      rd(&JsonSize);
      // Full size of file minus header size, two chuncks header's sizes (8 per one) and json data
      BinBufferSize = GlbSize - 12 - 8 - JsonSize - 8;
      if (BinBufferSize <= 0 || JsonSize <= 8)
      {
        logger::error << std::format("Error loading {}: corrupted file!", FileName);
        return *this;
      }
      std::vector<BYTE> BinBuffer;
      BinBuffer.resize(BinBufferSize, 0);
      // Read binary buffer from it's start point
      f.seekg(12 + 8 + JsonSize + 8, std::fstream::beg);
      f.read(reinterpret_cast<CHAR *>(BinBuffer.data()), BinBufferSize);
      f.close();
      return LoadGLB(FileName, JsonSize, std::move(BinBuffer), NewTransform);
    }
    f.close();
    return LoadGLTF(FileName, NewTransform);
  }
} /* End of 'tam::model_res::Create' function */

/* Load (.G3DM) model function.
 * ARGUMENTS:
 *   - trimesh filename (.G3DM):
 *       const std::string &FileName;
 *   - matrix to create model with:
 *       const matr &Transform;
 * RETURNS:
 *   (model_res &) reference to loaded model.
 */
tam::model_res & tam::model_res::LoadG3DM( const std::string &FileName, const matr &Transform )
{
  Free();
  this->Transform = Transform;
  Name = FileName;

  // Load file into memory
  std::fstream f(FileName, std::fstream::in | std::fstream::binary);

  if (!f)
  {
    logger::error << std::format("Error while loading {} occured (unable to load file).", FileName);
    return *this;
  }
  ModelType = "G3DM";

  f.seekg(0, std::fstream::end);
  std::size_t flen = f.tellg();
  f.seekg(0, std::fstream::beg);
 
  std::vector<BYTE> mem;
  mem.resize(flen);
  f.read((CHAR *)mem.data(), flen);

  BYTE *ptr = mem.data();

  rdr rd(ptr);

  // Load header
  DWORD sign;
  rd(&sign);

  INT NoofP = 0, NoofM = 0, NoofT = 0;
  rd(&NoofP);
  rd(&NoofM);
  rd(&NoofT);
  logger::info << std::format("\nG3DM: prims {}, mtls {}, texs {}\n",
    NoofP, NoofM, NoofT);

  std::vector<INT> prims_mtl;
  prims_mtl.resize(NoofP);
  Prims.resize(NoofP);

  shader MdlShd = Rnd->ShaderCreate("model");
  if ((MtlPat = Rnd->material_pattern_manager::Find("Model material pattern")) == nullptr)
    MtlPat = Rnd->MaterialPatternCreate(Rnd->shader_manager::Find("model"), "Model material pattern", 5, 1, {}, {},
      {vlk::vertex_attribute::eVEC3, vlk::vertex_attribute::eVEC2, vlk::vertex_attribute::eVEC3, vlk::vertex_attribute::eVEC4},
      vlk::topology::TRIMESH);
  material ModelMtl;
  if ((ModelMtl = Rnd->material_manager::Find("Model material")) == nullptr)
    ModelMtl = Rnd->MaterialCreate(std::format("Model material::{}", Rnd->GlobalModelIdLast), MtlPat, {}, {});

  logger::info << "Load primitives in .G3DM model";
  // Load primitives
  INT StartVertexIndex = 0;
  for (INT i = 0; i < NoofP; i++)
  {
    INT nv = 0, ni = 0;

    vertex_std *V;
    INT *Ind;

    rd(&nv);
    rd(&ni);
    rd(&prims_mtl[i]);
    rd(&V, nv);
    rd(&Ind, ni);
    // Apply transform matrix and get AABB
    for (INT i = 0; i < ni - 2; i += 3)
      Geometry.Triangles.push_back( mth::cg::triangle {Ind[i] + StartVertexIndex, Ind[i + 1] + StartVertexIndex, Ind[i + 2] + StartVertexIndex});
    StartVertexIndex += ni;
    if (i == 0 && nv > 0)
    {
      V[0].P = Transform.TransformPoint(V[0].P);
      MinBB = MaxBB = V[0].P;
      Geometry.Vertices.push_back(V[0].P);
    }
    for (INT j = 1; j < nv; j++)
    {
      V[0].P = Transform.TransformPoint(V[j].P);
      Geometry.Vertices.push_back(V[0].P);
      // Get AABB of model
      MinBB = vec3(COM_MIN(MinBB.X, V[0].P.X), COM_MIN(MinBB.Y, V[0].P.Y), COM_MIN(MinBB.Z, V[0].P.Z));
      MaxBB = vec3(COM_MAX(MaxBB.X, V[0].P.X), COM_MAX(MaxBB.Y, V[0].P.Y), COM_MAX(MaxBB.Z, V[0].P.Z));
    }
    Prims[i] = Rnd->PrimitiveCreate(ModelMtl, std::span(V, nv), std::span(Ind, ni));
  }

  // Load materials
  /* Stored material structure */
  struct STORE_MATERIAL
  {
    CHAR Name[300]; // Material name

    /* Illumination coefficients */
    mth::vec3<FLT> Ka, Kd, Ks; // Ambient, diffuse, specular coefficients
    FLT Ph;                    // Phong power coefficient
    FLT Trans;                 // Transparency factor
    INT Tex[8];                // Texture references

    CHAR ShaderStr[300];       // Shader string
    INT Shader;                // Shader index (not used)
  }; /* End of 'STORE_MATERIAL' struct */

  logger::info << "Load materials in .G3DM model";
  std::vector<material> mtls;
  STORE_MATERIAL *store_mtls;
  rd(&store_mtls, NoofM);
  mtls.resize(NoofM);

  // Load textures
  logger::info << "Load textures in .G3DM model";
  std::vector<texture> Textures;
  Textures.resize(NoofT);
  for (INT i = 0; i < NoofT; i++)
  {
    CHAR Name[300];
    INT W, H, C;

    rd(Name, 300);
    rd(&W);
    rd(&H);
    rd(&C);

    SIZE_T last_byte = (ptr - mem.data()) + W * H * C;
    if (last_byte <= mem.size())
      Textures[i] = Rnd->TextureCreate(FileName + "_" + Name, W, H, C, ptr);
    else
      break;
    ptr += W * H * C;
  }

  mth::vec4<BYTE> Data = mth::vec4<BYTE>(255);
  texture DefaultTex = Rnd->TextureCreate("Default texture", 1, 1, 4, Data);
  if (NoofM > 0)
  {
    for (INT i = 0; i < NoofM; i++)
    {
      BUF_MTL_PHONG BufMtlPhong
      {
        .Ka = store_mtls[i].Ka,
        .KdTrans = vec4(store_mtls[i].Kd, store_mtls[i].Trans),
        .KsPh = vec4(store_mtls[i].Ks, store_mtls[i].Ph),
      };
      std::vector<texture> Texs {};
      for (INT t = 0; t < 8; t++)
      {
        if (store_mtls[i].Tex[t] >= 0 && store_mtls[i].Tex[t] < Textures.size() &&
            Textures[store_mtls[i].Tex[t]] != nullptr)
          Texs.push_back(Textures[store_mtls[i].Tex[t]]), BufMtlPhong.TextureFlags[t] = 1;
      }
      buffer BufMtl = Rnd->BufferCreate<BUF_MTL_PHONG>(&BufMtlPhong, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
      mtls[i] = Rnd->MaterialCreate(std::format("{}::{}{}", FileName, store_mtls[i].Name, Rnd->GlobalModelIdLast), MtlPat, Texs, {BufMtl});
      //mtls[i]->Shd = Rnd->shader_manager::Find("");
    }
  }
  else
  {
    BUF_MTL_PHONG BufMtlPhong
    {
      .Ka = vec4(0.1),
      .KdTrans = vec4(0.5, 0.5, 0.5, 1),
      .KsPh = vec4(0.4, 0.4, 0.4, 30),
      .TextureFlags = {0, 0, 0, 0, 0, 0, 0, 0},
    };
    buffer BufMtl = Rnd->BufferCreate<BUF_MTL_PHONG>(&BufMtlPhong, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    material_pattern DefMtlPat = Rnd->MaterialPatternCreate(Rnd->shader_manager::Find("model"), "Model material pattern", 5, 1, {}, {},
      {vlk::vertex_attribute::eVEC3, vlk::vertex_attribute::eVEC2, vlk::vertex_attribute::eVEC3, vlk::vertex_attribute::eVEC4},
      vlk::topology::TRIMESH);
    mtls.push_back(Rnd->MaterialCreate("Default material", DefMtlPat, {}, {BufMtl}));
  }

  // Correct primitives materials references
  for (INT i = 0; i < NoofP; i++)
  {
    if (mtls.size() == 1) // if only one material - set it to every primtive of model
      Prims[i]->Mtl = mtls[0];
    else if (prims_mtl[i] >= 0 && prims_mtl[i] < mtls.size())
      Prims[i]->Mtl = mtls[prims_mtl[i]];
    else
      Prims[i]->Mtl = Rnd->material_manager::Find("Default material");
  }

  return *this;
} /* End of 'tam::model_res::LoadG3DM' function */

/* Create (.GLTF) model function.
 * ARGUMENTS:
 *   - filename:
 *       const std::string &FileName;
 * RETURNS: None.
 */
VOID tam::model_res::CreateGLTFModel( const std::string &FileName )
{
  std::vector<texture> Texs {};
  // Create default texture for invalid textures handling
  mth::vec4<BYTE> Data = mth::vec4<BYTE>(255);
  texture DefaultTex = Rnd->TextureCreate("Default texture", 1, 1, 4, Data);
  for (auto &tex : GLTFData.Textures)
  {
    /// Check if texture is invalid
    if (tex.Image.Name == "No data found file")
      Texs.push_back(DefaultTex);
    else
    {
      image ImgTex = image(tex.Image.ImageData);
      Texs.push_back(Rnd->TextureCreate(FileName + "::" + tex.Name + "_" + tex.Image.Name, ImgTex.W, ImgTex.H, 4,
        ImgTex.RowsB[0][0], TRUE, TRUE));
    }
  }

  std::vector<buffer> Bufs {};
  if (GLTFData.JointMatrs.size() != 0)
  {
    JointMatrBuf = Rnd->BufferCreate((UINT)GLTFData.JointMatrs.size() * 16 * 4, (VOID *)GLTFData.JointMatrs.data(),
      VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    Bufs.push_back(JointMatrBuf);
  }
  if (GLTFData.InvJointMatrs.size() != 0)
  {
    InvJointMatrBuf = Rnd->BufferCreate((UINT)GLTFData.InvJointMatrs.size() * 16 * 4, (VOID *)GLTFData.InvJointMatrs.data(),
      VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    Bufs.push_back(InvJointMatrBuf);
  }

  shader PBRShd = Rnd->ShaderCreate("pbr");
  OpaqueMtlPat = Rnd->MaterialPatternCreate(PBRShd, "PBR opaque material pattern::" + FileName + "::" +
    std::format("{}", Rnd->GlobalModelIdLast), 5, 0, Bufs, {},
    {vlk::vertex_attribute::eVEC3, vlk::vertex_attribute::eVEC2, vlk::vertex_attribute::eVEC3, vlk::vertex_attribute::eVEC4,
    vlk::vertex_attribute::eVEC4, vlk::vertex_attribute::eiVEC4, vlk::vertex_attribute::eVEC4},
    vlk::topology::TRIMESH, vlk::blend_mode::eOPAQUE);

  if (GLTFData.HasTrans)
    TransMtlPat = Rnd->MaterialPatternCreate(PBRShd, "PBR transparent material pattern::" + FileName + "::" +
      std::format("{}", Rnd->GlobalModelIdLast), 5, 0, Bufs, {},
      {vlk::vertex_attribute::eVEC3, vlk::vertex_attribute::eVEC2, vlk::vertex_attribute::eVEC3, vlk::vertex_attribute::eVEC4,
      vlk::vertex_attribute::eVEC4, vlk::vertex_attribute::eiVEC4, vlk::vertex_attribute::eVEC4},
      vlk::topology::TRIMESH, vlk::blend_mode::eTRANSPARENT);

  // Read materials
  std::vector<material> Materials {};
  for (auto &mtl : GLTFData.Materials)
  {
    texture Tex, NMTex, EmiTex, MRTex, FactorsTex;
    std::vector<texture> MtlTexs {};
    if (mtl.TextureNo != -1)
    {
      if (GLTFData.Textures[mtl.TextureNo].Image.Name != "No data found file")
      {
        image ImgTex = image(GLTFData.Textures[mtl.TextureNo].Image.ImageData);
        Tex = Rnd->TextureCreate(FileName + "::" + mtl.Name + + "::" + GLTFData.Textures[mtl.TextureNo].Name +
          "::" + GLTFData.Textures[mtl.TextureNo].Image.Name + "(BaseColorTex)",
          ImgTex.W, ImgTex.H, 4, ImgTex.RowsB[0][0], TRUE, TRUE, FALSE);
        MtlTexs.push_back(Tex);
      }
      else
        MtlTexs.push_back(Texs[mtl.TextureNo]);
    }
    else
    {
      mth::vec4<BYTE> Data = mth::vec4<BYTE>(255 * mtl.BaseColorFactor.X,
                                             255 * mtl.BaseColorFactor.Y,
                                             255 * mtl.BaseColorFactor.Z,
                                             255 * mtl.BaseColorFactor.W);
      Tex = Rnd->TextureCreate(FileName + "::" + mtl.Name + "::BaseColorTex", 1, 1, 4, Data, TRUE, TRUE, FALSE);
      MtlTexs.push_back(Tex);
    }
    if (mtl.MetRoughTexNo != -1)
    {
      image ImgTex = image(GLTFData.Textures[mtl.MetRoughTexNo].Image.ImageData);
      MRTex = Rnd->TextureCreate(FileName + "::" + mtl.Name + + "::" + GLTFData.Textures[mtl.MetRoughTexNo].Name +
        "::" + GLTFData.Textures[mtl.MetRoughTexNo].Image.Name + "(MetallicRoughnessTex)",
        ImgTex.W, ImgTex.H, 4, ImgTex.RowsB[0][0], TRUE, TRUE, FALSE);
      MtlTexs.push_back(MRTex);
    }
    else
    {
      mth::vec4<BYTE> Data = mth::vec4<BYTE>(255 * mtl.Factors.X,
                                             255 * mtl.Factors.Y,
                                             255 * mtl.Factors.Z,
                                             0);
      MRTex = Rnd->TextureCreate(FileName + "::" + mtl.Name + "::MetallicRoughnessTex", 1, 1, 4, Data, TRUE, TRUE, FALSE);
      MtlTexs.push_back(MRTex);
    }
    if (mtl.NormalMapTexNo != -1)
    {
      image ImgTex = image(GLTFData.Textures[mtl.NormalMapTexNo].Image.ImageData);
      NMTex = Rnd->TextureCreate(FileName + "::" + mtl.Name + + "::" + GLTFData.Textures[mtl.NormalMapTexNo].Name +
        "::" + GLTFData.Textures[mtl.NormalMapTexNo].Image.Name + "(NormalMapTex)",
        ImgTex.W, ImgTex.H, 4, ImgTex.RowsB[0][0], TRUE, TRUE, FALSE);
      MtlTexs.push_back(NMTex);
    }
    else
    {
      mth::vec4<BYTE> Data = mth::vec4<BYTE>(0, 255, 0, 255);
      NMTex = Rnd->TextureCreate(FileName + "::" + mtl.Name + "::NormalMapTex", 1, 1, 4, Data, TRUE, TRUE, FALSE);
      MtlTexs.push_back(NMTex);
    }
    if (mtl.EmissiveTexNo != -1)
    {
      image ImgTex = image(GLTFData.Textures[mtl.EmissiveTexNo].Image.ImageData);
      EmiTex = Rnd->TextureCreate(FileName + "::" + mtl.Name + + "::" + GLTFData.Textures[mtl.EmissiveTexNo].Name +
        "::" + GLTFData.Textures[mtl.EmissiveTexNo].Image.Name + "(EmissiveTex)",
        ImgTex.W, ImgTex.H, 4, ImgTex.RowsB[0][0]);
      MtlTexs.push_back(EmiTex);
    }
    else
    {
      mth::vec4<BYTE> Data = mth::vec4<BYTE>(255);
      EmiTex = Rnd->TextureCreate(FileName + "::" + mtl.Name + "::EmissiveRoughnessTex", 1, 1, 4, Data);
      MtlTexs.push_back(EmiTex);
    }
    mth::vec4<BYTE> Data = mth::vec4<BYTE>(mtl.Factors.X * 255, mtl.Factors.Y * 255, mtl.Factors.Z * 255, 255 * BYTE(mtl.NormalMapTexNo != -1));
    FactorsTex = Rnd->TextureCreate(FileName + "::" + mtl.Name + "::SpareTex", 1, 1, 4, &Data.X, TRUE, TRUE, FALSE);
    MtlTexs.push_back(FactorsTex);
    Materials.push_back(Rnd->MaterialCreate(std::format("{}::{}{}", FileName, mtl.Name, Rnd->GlobalModelIdLast),
                                            (mtl.AlphaMode == gltf::gltf_alpha_mode::eOPAQUE) ? OpaqueMtlPat : TransMtlPat, MtlTexs, {}));
  }

  BOOL IsFirstAABBPoint = TRUE;
  INT StartVertexIndex = 0;
  // Load primitives
  std::function<VOID( INT )> CreatePrimitives =
    [this, Materials, &IsFirstAABBPoint, &CreatePrimitives, &StartVertexIndex]( INT NodeIndex )
    {
      gltf::gltf_node &nd = GLTFData.Nodes[NodeIndex];

      matr Tr = nd.Matrix;
      if (nd.Mesh != -1)
        for (auto &pr : GLTFData.Meshes[nd.Mesh].Prims)
        {
          if (pr.MaterialNo == -1)
            continue;
          std::vector<gltf::pbr_vertex> V {};
          std::vector<INT> Ind {};

          // Indices
          if (std::vector<UINT16> ind16 = GLTFData.GetAccessorData<UINT16>(pr.IndexAccessor); ind16.size() != 0)
            Ind.reserve(ind16.size()), std::transform(ind16.begin(), ind16.end(), std::back_inserter(Ind), []( UINT16 X ){ return static_cast<INT>(X); });
          else if (auto ind32 = GLTFData.GetAccessorData<INT>(pr.IndexAccessor); ind32.size() != 0)
            Ind = std::move(ind32);

          // Vertices
          auto pos = GLTFData.GetAccessorData<vec3>(pr.PositionAccessor);
          // Apply transform matrix and get AABB
          for (INT i = 0; i < (INT)Ind.size() - 2; i += 3)
            nd.Geometry.Triangles.push_back(mth::cg::triangle {Ind[i] + StartVertexIndex, Ind[i + 1] + StartVertexIndex, Ind[i + 2] + StartVertexIndex});
          //Geometry.Triangles.insert(Geometry.Triangles.end(), nd.Geometry.Triangles.begin(), nd.Geometry.Triangles.end());
          INT IndStart = (INT)Geometry.Vertices.size();
          std::transform(nd.Geometry.Triangles.begin(), 
                         nd.Geometry.Triangles.end(),
                         std::back_inserter(Geometry.Triangles),
                         [IndStart]( mth::cg::triangle Trg ) -> mth::cg::triangle
                         {
                           return {Trg.V0 + IndStart, Trg.V1 + IndStart, Trg.V2 + IndStart};
                         });
          // StartVertexIndex += static_cast<INT>(Ind.size());
          auto norm = GLTFData.GetAccessorData<vec3>(pr.NormalAccessor);
          auto tex = GLTFData.GetAccessorData<vec2>(pr.TexCoord0Accessor);
          auto tangent = GLTFData.GetAccessorData<vec4>(pr.TangentAccessor);
          auto color = GLTFData.GetAccessorData<vec4>(pr.Color0Accessor);
          std::vector<mth::vec4<INT>> joints {};

          if (auto jointsb = GLTFData.GetAccessorData<mth::vec4<BYTE>>(pr.JointsAccessor); jointsb.size() != 0)
            joints.reserve(jointsb.size()), std::transform(jointsb.begin(), jointsb.end(),
              std::back_inserter(joints), []( mth::vec4<BYTE> X ){ return mth::vec4<INT>(X.X, X.Y, X.Z, X.W); });
          else if (auto jointssh = GLTFData.GetAccessorData<mth::vec4<SHORT>>(pr.JointsAccessor); jointssh.size() != 0)
            joints.reserve(jointssh.size()), std::transform(jointssh.begin(), jointssh.end(),
              std::back_inserter(joints), []( mth::vec4<SHORT> X ){ return mth::vec4<INT>(X.X, X.Y, X.Z, X.W); });

          auto weights = GLTFData.GetAccessorData<mth::vec4<FLT>>(pr.WeightsAccessor);

          V.resize(pos.size(), {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1},
            {1.0, 0.0, 0.0, 1.0}, {nd.Index, 0, 0, 0}, {1.0, 0.0, 0.0, 0.0}});
          INT vno = 0, is_tng_used = 0;
          is_tng_used = (tangent.size() != 0);
          if (IsFirstAABBPoint && pos.size() > 0)
          {
            MinBB = MaxBB = Tr.TransformPoint(pos[0]);
            // MinBB = MaxBB = pos[0];
            IsFirstAABBPoint = FALSE;
          }
          // nd.Geometry.Vertices.reserve(V.size());
          for (auto &vrt : V)
          {
            vrt.P = pos[vno];
            // Obtain AABB of model
            vec3 P = Tr.TransformPoint(vrt.P);
            // vec3 P = vrt.P;
            /// nd.Geometry.Vertices.push_back(vrt.P);
            nd.Geometry.Vertices.push_back(P);
            MinBB = vec3(COM_MIN(MinBB.X, P.X), COM_MIN(MinBB.Y, P.Y), COM_MIN(MinBB.Z, P.Z));
            MaxBB = vec3(COM_MAX(MaxBB.X, P.X), COM_MAX(MaxBB.Y, P.Y), COM_MAX(MaxBB.Z, P.Z));

            if (vno < tex.size())
              vrt.T = tex[vno];
            if (vno < norm.size())
              vrt.N = norm[vno];
            if (vno < color.size())
              vrt.C = color[vno];
            if (vno < tangent.size())
              vrt.Tng = tangent[vno];
            if (vno < joints.size())
            {
              if (nd.Skin != -1)
                vrt.J = mth::vec4<INT>(
                  GLTFData.Skins[nd.Skin].Joints[joints[vno].X],
                  GLTFData.Skins[nd.Skin].Joints[joints[vno].Y],
                  GLTFData.Skins[nd.Skin].Joints[joints[vno].Z],
                  GLTFData.Skins[nd.Skin].Joints[joints[vno].W]);
            }
            if (vno < weights.size())
              vrt.W = weights[vno];
            vno++;
          }
          Geometry.Vertices.insert(Geometry.Vertices.end(), nd.Geometry.Vertices.begin(), nd.Geometry.Vertices.end());
          prim p;
          if (pr.MaterialNo == -1)
          {
            material PBRMtl = Rnd->MaterialCreate(std::format("PBR material::{}", Rnd->GlobalModelIdLast), OpaqueMtlPat, {}, {});
            p = Rnd->PrimitiveCreate(Rnd->material_manager::Find("PBR material"), std::span(V),
              std::span(Ind));
          }
          else
            p = Rnd->PrimitiveCreate(Materials[pr.MaterialNo], std::span(V), std::span(Ind));
          Prims.push_back(p);
        }
      for (INT subnode : nd.Subnodes)
        CreatePrimitives(subnode);
    };
  for (auto &rootnd : GLTFData.Scenes[GLTFData.SceneNo].RootNodes)
    CreatePrimitives(rootnd);
} /* End of 'tam::model_res::CreateGLTFModel' function */

/* Load (.GLB) model function.
 * ARGUMENTS:
 *   - filename (.GLB):
 *       const std::string &FileName;
 *   - size of gltf part in file:
 *       INT GltfSize;
 *   - binary buffer rvalue:
 *       std::vector<BYTE> &&BinaryBuffer;
 *   - matrix to create model with:
 *       const matr &Transform;
 * RETURNS:
 *   (model_res &) reference to loaded model.
 */
tam::model_res & tam::model_res::LoadGLB( const std::string &FileName, INT GltfSize, std::vector<BYTE> &&BinaryBuffer, const matr &Transform )
{
  Free();
  this->Transform = Transform;
  Name = FileName;
  ModelType = "GLB";

  // Save glb buffer data into structure
  GLTFData.GlbBinaryBufferData = BinaryBuffer;
  BinaryBuffer = {};

  // Parse json from start of chunk data to it's end
  GLTFData.LoadGLTF(FileName, Transform, GltfSize, 12 + 8);

  // Use gltf struct to create model of our resources
  CreateGLTFModel(FileName);
  logger::info << std::format("Loaded: '{}' model\n", FileName);
  return *this;
} /* End of 'tam::model_res::LoadGLB' function */

/* Load (.GLTF) model function.
 * ARGUMENTS:
 *   - filename (.GLTF):
 *       const std::string &FileName;
 *   - matrix to create model with:
 *       const matr &Transform;
 * RETURNS:
 *   (model_res &) reference to loaded model.
 */
tam::model_res & tam::model_res::LoadGLTF( const std::string &FileName, const matr &Transform )
{
  Free();
  this->Transform = Transform;
  Name = FileName;
  ModelType = "GLTF";
  GLTFData.LoadGLTF(FileName, Transform);

  // Use gltf struct to create model of our resources
  CreateGLTFModel(FileName);
  logger::info << std::format("Loaded: '{}' model\n", FileName);
  return *this;
} /* End of 'tam::model_res::LoadGLTF' function */


/* Recalculate AABB with new matrix function.
 * ARGUMENTS:
 *   - new transform matrix:
 *       const matr &NewTransform;
 * RETURNS:
 *   (std::pair<vec3, vec3>) MinBB and MaxBB vectors.
 */
std::pair<tam::vec3, tam::vec3> tam::model_res::RecalculateAABB( const matr &NewTransform )
{
  vec3 NewMinBB {}, NewMaxBB {};
  if (ModelType == "G3DM")
  {
    if (Geometry.Vertices.size() > 0)
      NewMinBB = NewMaxBB = NewTransform.TransformPoint(Geometry.Vertices[0]);
    // Get transformed AABB
    for (INT i = 0; i < Geometry.Vertices.size(); i++)
    {
      vec3 P = NewTransform.TransformPoint(Geometry.Vertices[i]);
      NewMinBB = vec3(COM_MIN(NewMinBB.X, P.X), COM_MIN(NewMinBB.Y, P.Y), COM_MIN(NewMinBB.Z, P.Z));
      NewMaxBB = vec3(COM_MAX(NewMaxBB.X, P.X), COM_MAX(NewMaxBB.Y, P.Y), COM_MAX(NewMaxBB.Z, P.Z));
    }
  }
  // Calculate AABB for every node in gltf model and get summary AABB
  else
  {
    BOOL IsFirstPoint = TRUE;
    for (auto &Node : GLTFData.Nodes)
    {
      if (Node.Geometry.Vertices.size() > 0)
      {
        matr ResTransform = /* Node.Matrix * */ NewTransform;
        if (IsFirstPoint)
        {
          NewMinBB = NewMaxBB = ResTransform.TransformPoint(Node.Geometry.Vertices[0]);
          IsFirstPoint = FALSE;
        }
        for (auto &V : Node.Geometry.Vertices)
        {
          vec3 P = ResTransform.TransformPoint(V);
          NewMinBB = vec3(COM_MIN(NewMinBB.X, P.X), COM_MIN(NewMinBB.Y, P.Y), COM_MIN(NewMinBB.Z, P.Z));
          NewMaxBB = vec3(COM_MAX(NewMaxBB.X, P.X), COM_MAX(NewMaxBB.Y, P.Y), COM_MAX(NewMaxBB.Z, P.Z));
        }
      }
    }
    if (IsFirstPoint)
      NewMinBB = NewMaxBB = vec3(0);
  }
  return std::pair<vec3, vec3>(NewMinBB, NewMaxBB);
} /* End of 'tam::model_res::RecalculateAABB' function */


/* Play model's animation function.
 * ARGUMENTS:
 *   - name of animation to be used:
 *       const std::string &AnimName;
 *   - time to be used in animation:
 *       DBL Time;
 * RETURNS: None.
 */
VOID tam::model_res::DoAnim( const std::string &AnimName, DBL Time )
{
  if (!GLTFData.IsAnim)
    return;
  gltf::gltf_animation anim {};
  for (auto &a : GLTFData.Animations)
    if (a.Name == AnimName)
    {
      anim = a;
      break;
    }
  if (anim.Name == "")
    return;

  for (auto &nd : GLTFData.Nodes)
  {
    nd.Scale = nd.BaseScale;
    nd.Rotation = nd.BaseRotation;
    nd.Translation = nd.BaseTranslation;
    nd.Matrix = nd.BaseMatrix;
  }

  DBL CurTime = Time - (INT)(Time / anim.MaxLength) * anim.MaxLength;
  for (auto &Channel : anim.Channels)
  {
    INT TransformNo = 0;
    if (CurTime >= Channel.Timestamps.back())
    {
      if (Channel.TransformType == gltf::gltf_anim_transform_type::eTRANSLATE)
        GLTFData.Nodes[Channel.NodeIndex].Translation = {Channel.Transform.back().GetVec3().X,
          Channel.Transform.back().GetVec3().Y, Channel.Transform.back().GetVec3().Z};
      else if (Channel.TransformType == gltf::gltf_anim_transform_type::eROTATE)
        GLTFData.Nodes[Channel.NodeIndex].Rotation = {Channel.Transform.back().GetVec4().X,
          Channel.Transform.back().GetVec4().Y, Channel.Transform.back().GetVec4().Z,
            Channel.Transform.back().GetVec4().W};
      else if (Channel.TransformType == gltf::gltf_anim_transform_type::eSCALE)
        GLTFData.Nodes[Channel.NodeIndex].Scale = {Channel.Transform.back().GetVec3().X,
          Channel.Transform.back().GetVec3().Y, Channel.Transform.back().GetVec3().Z};
    }
    else
    {
      while (Channel.Timestamps[TransformNo] < CurTime)
        TransformNo++;
      if (TransformNo != 0)
      {
        DBL t = (CurTime - Channel.Timestamps[TransformNo - 1]) /
          (Channel.Timestamps[TransformNo] - Channel.Timestamps[TransformNo - 1]);
        if (Channel.Interpolation == "LINEAR")
        {
          if (Channel.TransformType == gltf::gltf_anim_transform_type::eTRANSLATE)
          {
            vec3 trans = vec3::Lerp(
              Channel.Transform[TransformNo - 1].GetVec3(),
                Channel.Transform[TransformNo].GetVec3(), t);
            GLTFData.Nodes[Channel.NodeIndex].Translation = {trans[0], trans[1], trans[2]};
          }
          else if (Channel.TransformType == gltf::gltf_anim_transform_type::eROTATE)
          {
            vec4 rot = vec4::SlerpQuat(
              Channel.Transform[TransformNo - 1].GetVec4(),
                Channel.Transform[TransformNo].GetVec4(), t);
            GLTFData.Nodes[Channel.NodeIndex].Rotation = {rot[0], rot[1], rot[2], rot[3]};
          }
          else if (Channel.TransformType == gltf::gltf_anim_transform_type::eSCALE)
          {
            vec3 scale = vec3::Lerp(
              Channel.Transform[TransformNo - 1].GetVec3(),
                Channel.Transform[TransformNo].GetVec3(), t);
            GLTFData.Nodes[Channel.NodeIndex].Scale = {scale[0], scale[1], scale[2]};
          }
        }
      }
      else
      {
        DBL t = CurTime / Channel.Timestamps[0];
        if (Channel.Interpolation == "LINEAR")
        {
          if (Channel.TransformType == gltf::gltf_anim_transform_type::eTRANSLATE)
          {
            vec3 trans = vec3::Lerp(
              vec3(GLTFData.Nodes[Channel.NodeIndex].BaseTranslation[0], GLTFData.Nodes[Channel.NodeIndex].BaseTranslation[1],
                   GLTFData.Nodes[Channel.NodeIndex].BaseTranslation[2]),
              Channel.Transform[0].GetVec3(), t);
            GLTFData.Nodes[Channel.NodeIndex].Translation = {trans[0], trans[1], trans[2]};
          }
          else if (Channel.TransformType == gltf::gltf_anim_transform_type::eROTATE)
          {
            vec4 rot = vec4::SlerpQuat(
              vec4(GLTFData.Nodes[Channel.NodeIndex].BaseRotation[0], GLTFData.Nodes[Channel.NodeIndex].BaseRotation[1],
                  GLTFData.Nodes[Channel.NodeIndex].BaseRotation[2], GLTFData.Nodes[Channel.NodeIndex].BaseRotation[3]),
              Channel.Transform[0].GetVec4(), t);
            GLTFData.Nodes[Channel.NodeIndex].Rotation = {rot[0], rot[1], rot[2], rot[3]};
          }
          if (Channel.TransformType == gltf::gltf_anim_transform_type::eSCALE)
          {
            vec3 scale = vec3::Lerp(
              vec3(GLTFData.Nodes[Channel.NodeIndex].BaseScale[0], GLTFData.Nodes[Channel.NodeIndex].BaseScale[1],
                  GLTFData.Nodes[Channel.NodeIndex].BaseScale[2]),
              Channel.Transform[0].GetVec3(), t);
            GLTFData.Nodes[Channel.NodeIndex].Scale = {scale[0], scale[1], scale[2]};
          }
        }
      }
    }
  }
  // Recalculate nodes matrices
  for (auto &nd : GLTFData.Nodes)
    if (!nd.IsMatrUsed)
      nd.Matrix =
        matr::Scale(nd.Scale) *
        matr::RotateQuat(nd.Rotation) *
        matr::Translate(nd.Translation);

  // Compute matrices
  std::function<VOID( INT, const matr & )> ComputeWorldMatrix =
    [this, &ComputeWorldMatrix]( INT NodeIndex, const matr &ParentWorld )
    {
      const gltf::gltf_node &nd = GLTFData.Nodes[NodeIndex];
      matr world = nd.Matrix * ParentWorld;
      GLTFData.Nodes[NodeIndex].Matrix = world;
      for (INT subnode : nd.Subnodes)
        ComputeWorldMatrix(subnode, world);
    };
  for (auto &sc : GLTFData.Scenes)
    for (auto &rootnd : sc.RootNodes)
      ComputeWorldMatrix(rootnd, matr::Identity());

  // Create new matrix pool
  std::vector<matr::matr_data> JointMatrs {};
  for (auto &nd : GLTFData.Nodes)
    JointMatrs.push_back(nd.Matrix);

  if (JointMatrs.size() != 0)
    JointMatrBuf->Update(JointMatrs.data(), 0, (UINT)JointMatrs.size() * 16 * 4);
} /* End of 'tam::model_res::DoAnim' function */

/* Free model function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID tam::model_res::Free( VOID )
{
  Name = "";
  Prims.clear();
  Transform = matr::Identity();
  Geometry.Triangles.clear();
  Geometry.Vertices.clear();
  GLTFData = {};
} /* End of 'tam::model_res::Free' function */

/* END OF 'tam_res_models.cpp' FILE */
