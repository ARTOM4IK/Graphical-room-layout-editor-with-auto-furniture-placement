/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_test_system.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Vulkan test module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3),
 *               Arsentev Artemy (AA4),
 *               Nechaev Vladimir (VN4),
 *               Kupchenkov Ivan (IK4),
 *               Novozhilova Maryana (MN7),
 *               Tretyakov Andrey (AT7),
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 12.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
 
#ifndef __test_system_h_
#define __test_system_h_
 
#include "tam_def.h"
 
/* Generate passed result macro */
#define TEST_PASSED(Str)                                                                                      \
  tam::logger::out << tam::logger::SetBackColorStr(0x008800) << "PASSED:" << tam::logger::SetDefStr() << Str
/* Generate failed result macro */
#define TEST_FAILED(Str)                                                                                         \
  do                                                                                                             \
  {                                                                                                              \
    tam::logger::out << tam::logger::SetBackColorStr(0xFF0000) << "FAILED:" << tam::logger::SetDefStr() << Str;   \
    throw std::logic_error("");                                                                                  \
  } while (FALSE)
 
/* Check equivalence macro function */
#define EXPECT_EQ(a, b)                                                  \
  do                                                                     \
  {                                                                      \
    if ((a) != (b))                                                      \
      TEST_FAILED(std::format("{} != {} ({} vs {})", #a, #b, (a), (b))); \
    else                                                                 \
      TEST_PASSED(std::format("{} == {}", #a, #b));                      \
  } while (FALSE)
 
/* Check mostly equivalence macro function */
#define EXPECT_NEAR(a, b, epsilon)                                                          \
  do                                                                                        \
  {                                                                                         \
    if (std::abs((a) - (b)) > epsilon)                                                      \
      TEST_FAILED(std::format("{} != {} within {} ({} vs {})", #a, #b, epsilon, (a), (b))); \
    else                                                                                    \
      TEST_PASSED(std::format("{} ~= {} (epsilon {})", #a, #b, epsilon));                   \
  } while (FALSE)
 
/* Check equivalence for float type macro function */
#define EXPECT_FLT_EQ(a, b)      \
    EXPECT_NEAR((a), (b), 1e-5F)
 
/* Check equivalence for double type macro function */
#define EXPECT_DBL_EQ(a, b)       \
    EXPECT_NEAR((a), (b), 1e-12F)
 
/* Check for conditional TRUE value macro function */
#define EXPECT_TRUE(condition)             \
  do                                       \
  {                                        \
    if (!(condition))                      \
      TEST_FAILED(#condition " is false"); \
    else                                   \
      TEST_PASSED(#condition " is true");  \
  } while (FALSE)
 
/* Check mostly equivalence macro function */
#define EXPECT_VEC_NEAR(v1, v2, epsilon)                                                                                                        \
  do                                                                                                                                            \
  {                                                                                                                                             \
    if (std::abs((v1).X - (v2).X) > (epsilon) || std::abs((v1).Y - (v2).Y) > epsilon || std::abs((v1).Z - (v2).Z) > (epsilon))                  \
      TEST_FAILED(std::format("vec3({}, {}, {}) != vec3({}, {}, {}) within {}", (v1).X, (v1).Y, (v1).Z, (v2).X, (v2).Y, (v2).Z, (epsilon)));    \
    else                                                                                                                                        \
      TEST_PASSED(std::format("vec3({}, {}, {}) ~= vec3({}, {}, {}) (epsilon {})", (v1).X, (v1).Y, (v1).Z, (v2).X, (v2).Y, (v2).Z, (epsilon))); \
  } while (FALSE)
 
/* Check near equivalence of 4x4 matrices macro function */
#define EXPECT_MATR_NEAR(m1, m2, eps, t)                   \
  do                                                       \
  {                                                        \
    BOOL ok = TRUE;                                        \
                                                           \
    for (INT i = 0; i < 4; i++)                            \
      for (INT j = 0; j < 4; j++)                          \
        if (std::abs((m1).M[i][j] - (m2).M[i][j]) > (eps)) \
          ok = FALSE;                                      \
    if (ok)                                                \
      TEST_PASSED(std::format("{}: matr1 == matr2", #t));  \
    else                                                   \
      TEST_FAILED(std::format("{}: matr1 != matr2", #t));  \
                                                           \
  } while (FALSE)

/* Test declaration macro code */
#define TEST(TestSuiteName, TestName)                                          \
  VOID TestSuiteName ## TestName( VOID );                                      \
  struct TestSuiteName ## TestName ## _Register                                \
  {                                                                            \
    TestSuiteName ## TestName ## _Register( VOID )                             \
    {                                                                          \
      tst::RegisterTest(#TestSuiteName, #TestName, TestSuiteName ## TestName); \
    }                                                                          \
  } TestSuiteName ## TestName ## _Instance;                                    \
  VOID TestSuiteName ## TestName( VOID )
 
/* Check vec3 equivalence macro function */
#define EXPECT_VEC3_EQ(v1, v2)                                                                                                       \
  do                                                                                                                                 \
  {                                                                                                                                  \
    if ((v1).X != (v2).X || (v1).Y != (v2).Y || (v1).Z != (v2).Z)                                                                    \
      TEST_FAILED(std::format("{} != {} (({}, {}, {}) vs ({}, {}, {}))", #v1, #v2, (v1).X, (v1).Y, (v1).Z, (v2).X, (v2).Y, (v2).Z)); \
    else                                                                                                                             \
      TEST_PASSED(std::format("{} == {}", #v1, #v2));                                                                                \
  } while (FALSE)
 
/* Check vec3 equivalence float macro function */
#define EXPECT_FLT_VEC3_EQ(v1, v2)                                                                                                                \
  do                                                                                                                                              \
  {                                                                                                                                               \
    if (std::abs((v1).X - (v2).X) > 1e-5F || std::abs((v1).Y - (v2).Y) > 1e-5F || std::abs((v1).Z - (v2).Z) > 1e-5F)                              \
      TEST_FAILED(std::format("{} != {} within {} (({} {} {}) vs ({} {} {}))", #v1, #v2, 1e-5F, (v1).X, (v1).Y, (v1).Z, (v2).X, (v2).Y, (v2).Z)); \
    else                                                                                                                                          \
      TEST_PASSED(std::format("{} ~= {} (epsilon {})", #v1, #v2, 1e-5F));                                                                         \
  } while (FALSE)
 
/* Check vec3 equivalence double macro function */
#define EXPECT_DBL_VEC3_EQ(v1, v2)                                                                                                                 \
  do                                                                                                                                               \
  {                                                                                                                                                \
    if (std::abs((v1).X - (v2).X) > 1e-12F || std::abs((v1).Y - (v2).Y) > 1e-12F || std::abs((v1).Z - (v2).Z) > 1e-12F)                            \
      TEST_FAILED(std::format("{} != {} within {} (({} {} {}) vs ({} {} {}))", #v1, #v2, 1e-12F, (v1).X, (v1).Y, (v1).Z, (v2).X, (v2).Y, (v2).Z)); \
    else                                                                                                                                           \
      TEST_PASSED(std::format("{} ~= {} (epsilon {})", #v1, #v2, 1e-12F));                                                                         \
  } while (FALSE)
 
/* Test support namespace */
namespace tst
{
  /* Change output color function.
   * ARGUMENTS:
   *   - foreground and background colors (RGB in DWORD pack):
   *       DWORD ForeColor, BackColor;
   * RETURNS: None.
   */
  inline std::string SetColor( DWORD ForeColor, DWORD BackColor = 0 )
  {
    return std::format("\x1b[38;2;{};{};{}m\x1b[48;2;{};{};{}m",
      COM_LONGBYTE2(ForeColor), COM_LONGBYTE1(ForeColor), COM_LONGBYTE0(ForeColor),
      COM_LONGBYTE2(BackColor), COM_LONGBYTE1(BackColor), COM_LONGBYTE0(BackColor));
  } /* End of 'SetColor' function */
 
  // Function type
  using test_func_type = VOID(*)( VOID );
  /* Store test function type */
  struct test_case
  {
    std::string Suite;   // Suite name
    std::string Name;    // Subtask name
    test_func_type Func; // Test function
  }; /* End of 'test_case' structure */
 
  /* Get all test single ton vector function.
   * ARGUMENTS: None.
   * RETURNS:
   *   (std::vector<test_case> &) result test subtasks vector.
   */
  inline std::vector<test_case> & GetTests( VOID )
  {
    static std::vector<test_case> Tests;
    return Tests;
  } /* End of 'GetTests' function */
 
  /* Register test function.
   * ARGUMENTS:
   *   - test suite name:
   *       const std::string &Suite;
   *   - test name:
   *       const std::string &Name;
   *   - test function:
   *       test_func_type Func
   * RETURNS: None.
   */
  inline VOID RegisterTest( const std::string &Suite, const std::string &Name, test_func_type Func )
  {
    GetTests().push_back({Suite, Name, Func});
  } /* End of 'RegisterTest' function */
 
  /* Run all registered tests function.
   * ARGUMENTS: None.
   * RETURNS:
   *   (BOOL) TRUE if all tests are passed.
   */
  inline BOOL RunAllTests( VOID )
  {
    INT passed = 0, failed = 0;
 
    tam::logger::test << "Math tests started";
    for (const auto &test : GetTests())
    {
      tam::logger::out << tam::logger::SetBackColorStr(0x0000FF) << "[RUN]" << tam::logger::SetDefStr() << test.Suite << "." << test.Name;
      try
      {
        test.Func();
        tam::logger::out  << tam::logger::SetBackColorStr(0x87AA00) << "[OK]" << tam::logger::SetDefStr() << test.Suite << "." << test.Name;
        passed++;
      }
      catch (...)
      {
        tam::logger::out << tam::logger::SetBackColorStr(0xEE00BB) << "[FAILED]" << tam::logger::SetDefStr() << test.Suite << "." << test.Name;
        failed++;
      }
    }
    tam::logger::out << "TESTS PASSED: " << passed << ", FAILED: " << failed;
    tam::logger::test << "Math tests ended";

    return failed == 0;
  } /* End of 'RunAllTests' function */

  /* Run all vulkan tests function.
   * ARGUMENTS: None.
   * RETURNS:
   *   (BOOL) TRUE if all tests are passed.
   */
  inline VOID RunVlkTests( HWND hWnd )
  {
    tam::logger::test << "Vuklan test started";
    tam::vlk::core Core;
    tam::vlk::sampler smpl(&Core);
    Core.Create(hWnd, 0, 0);
    tam::logger::test << "Core created";
    tam::image im;
    smpl.Create();
    tam::logger::test << "Sampler created";
    im.Create(0, 0);
    im.Load("bin/M.G24");
    tam::logger::test << "Loaded: bin/M.G24";
    tam::vlk::image img(&Core);
    img.Create(im.W, im.H, TRUE, FALSE, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VK_IMAGE_LAYOUT_UNDEFINED, im.RowsB[0][0], im.W * im.H * 4, TRUE);
    tam::logger::test << "Image created";
    
    std::vector<VkDescriptorSetLayoutBinding> DescriptorSetLayoutBindings
    {
      {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr,
      },
    };

    //VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutCreateInfo
    //{
    //  .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,   // Structure type.
    //  .flags = 0,                                                     // Flags.
    //  .bindingCount = (UINT32)std::size(DescriptorSetLayoutBindings), // Number of bindings.
    //  .pBindings = DescriptorSetLayoutBindings.data(),                       // Pointer to first binding.
    //};
  
    
    //vkCreateDescriptorSetLayout(Core.Device, &DescriptorSetLayoutCreateInfo, nullptr, &SuperTestLayout);
    tam::logger::test << "Descriptor set layout created";

    tam::vlk::pipeline pip(&Core);
    tam::vlk::shader shd(&Core);
    shd.Create("test");
    tam::logger::test << "Shader default created";
    struct vertex
    {
      mth::vec3<FLT> P;
    };
    // Vertices data format.
    VkVertexInputBindingDescription BindingDescr
    {
      .binding = 0,
      .stride = (UINT32)sizeof(vertex),
      .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,  // Basic format of vertices (needed for instancing)
    };
    std::vector<VkVertexInputAttributeDescription> AttribDescrs {};
    AttribDescrs.resize(1);
    // Position attribute data.
    AttribDescrs[0].binding = 0;
    AttribDescrs[0].location = 0;
    AttribDescrs[0].format = VK_FORMAT_R32G32B32_SFLOAT;     // vec3
    AttribDescrs[0].offset = 0;

    //std::vector<VkDescriptorSetLayout> Testvector;
    //Testvector.push_back(SuperTestLayout.DescriptorSetLayout);
    tam::vlk::descriptor_set_layout SuperTestLayout(&Core);
    SuperTestLayout.Create(DescriptorSetLayoutBindings);
    std::vector<tam::vlk::descriptor_set_layout> Layouts {SuperTestLayout};

    pip.Create(shd, Layouts, {tam::vlk::vertex_attribute::eVEC3}, tam::vlk::topology::TRIMESH);
    tam::logger::test << "Pipeline created";

    std::array<VkDescriptorPoolSize, 1> PoolSizes {};

    PoolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    PoolSizes[0].descriptorCount = 1;                      // Number of descriptor sizes.

    Core.CreateDescriptorPool();
    //VkDescriptorPoolCreateInfo PoolCreateInfo
    //{
    //  .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
    //  .maxSets = 100,                            // Max number ofdescriptor sets, that you can create.
    //  .poolSizeCount = (UINT)PoolSizes.size(), // Number of descriptor sizes.
    //  .pPoolSizes = PoolSizes.data(),          // Descriptor sizes.
    //};

    //VkDescriptorPool DescriptorPool;
    //vkCreateDescriptorPool(Core.Device, &PoolCreateInfo, nullptr, &DescriptorPool);
    tam::logger::test << "Descriptor pool created";

    VkDescriptorSetAllocateInfo AllocInfo
    {
      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
      .descriptorPool = Core.DescriptorPool,        // Descriptor pool whrere you alloc memory.
      .descriptorSetCount = 1,                 // Number of descriptor set layouts.
      .pSetLayouts = &SuperTestLayout.DescriptorSetLayout,    
    };

    VkDescriptorImageInfo imageInfo = 
    {
        .sampler = smpl.Sampler,
        .imageView = img.View,
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    };
    tam::vlk::descriptor_set TestDescriptorSet(&Core);
    //VkDescriptorSet DescriptorSet;
    vkAllocateDescriptorSets(Core.Device, &AllocInfo, &TestDescriptorSet.DescriptorSet);
    std::array<VkWriteDescriptorSet, 1> DescriptorWrites;
    DescriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    DescriptorWrites[0].dstSet = TestDescriptorSet.DescriptorSet;                             
    DescriptorWrites[0].dstBinding = 0;                                     
    DescriptorWrites[0].dstArrayElement = 0;                                
    DescriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorWrites[0].descriptorCount = 1;                                
    DescriptorWrites[0].pImageInfo = &imageInfo;                          
    DescriptorWrites[0].pNext = nullptr;                          
    vkUpdateDescriptorSets(Core.Device, (UINT)DescriptorWrites.size(), DescriptorWrites.data(), 0, nullptr);
    tam::logger::test << "Descriptor set updated";

    pip.Update(shd);
    tam::logger::test << "Shader default updated";

    tam::vlk::buffer VertexBuf(Core);
    std::vector<mth::vec3<FLT>> vert {mth::vec3<FLT>(-1, 3, 0), mth::vec3<FLT>(-1, -1, 0), mth::vec3<FLT>(3, -1, 0)};
    //vert.push_back();
    //vert.push_back();
    //vert.push_back();
    VertexBuf.Create(vert.size() * sizeof(mth::vec3<FLT>), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_SHARING_MODE_EXCLUSIVE);
    tam::logger::test << "Vertex buffer created";

    VertexBuf.Write(vert.data(), vert.size() * sizeof(mth::vec3<FLT>), 0);

    MSG msg {};

    while (TRUE)
    {
      if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
      {
        if (msg.message == WM_QUIT)
          break;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      // Vulkan render here
      Core.FrameStart();

      vkCmdBindPipeline(Core.CommandBuffers[Core.CurrentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pip.Pipeline);
      vkCmdBindDescriptorSets(Core.CommandBuffers[Core.CurrentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pip.Layout, 0, 1, &TestDescriptorSet.DescriptorSet, 0, nullptr);

      VkBuffer VertexBuffers[] = {VertexBuf.Buffer}; 
      VkDeviceSize Offsets[] = {0};               

      vkCmdBindVertexBuffers(Core.CommandBuffers[Core.CurrentFrame], 0, 1, VertexBuffers, Offsets);
      vkCmdDraw(Core.CommandBuffers[Core.CurrentFrame], 3, 1, 0, 0);
      Core.FrameEnd();
    }
    
    VertexBuf.Destroy();
    tam::logger::test << "Vertex buffer destroyed";
    Core.DestroyDescriptorPool();
    //vkDestroyDescriptorPool(Core.Device, DescriptorPool, nullptr);
    tam::logger::test << "Descriptor pool destroyed";
    //vkDestroyDescriptorSetLayout(Core.Device, SuperTestLayout, nullptr);
    SuperTestLayout.Destroy();
    tam::logger::test << "Descriptor set layout destroyed";
    smpl.Destroy();
    tam::logger::test << "Sampler destroyed";
    img.Destroy();
    tam::logger::test << "Image destroyed";
    shd.Destroy();
    tam::logger::test << "Shader default destroyed";
    pip.Destroy();
    tam::logger::test << "Pipeline destroyed";
    Core.Destroy();
    tam::logger::test << "Vuklan test ended";
  } /* End of 'RunVlkTests' function */
} /* end of 'tam' namespace */
 
#endif /* __test_system_h_ */
 
/* END OF 'tam_test_system.h' FILE */
