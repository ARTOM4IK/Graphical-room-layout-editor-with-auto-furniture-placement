/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_test_system.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math matrix test module.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kochugueva Maria (MK3),
 *               Arsentev Artemy (AA4),
 *               Nechaev Vladimir (VN4),
 *               Kupchenkov Ivan (IK4),
 *               Novozhilova Maryana (MN7),
 *               Tretyakov Andrey (AT7),
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 29.01.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Test function */
TEST(VecTest, JustEqualities)
{
  EXPECT_VEC3_EQ(mth::vec3<INT>(1, 1, 1), mth::vec3<INT>(1, 1, 1));
  EXPECT_VEC_NEAR((mth::vec3<DBL>(1, 1, 1) * 2), mth::vec3<DBL>(2, 2, 2), 0.00005);
} /* End of 'AnyTest.Fail' test */

/* Test vec function */
TEST(MatrTest, Transpose)
{
  mth::matr<DBL> M1 = mth::matr<DBL>::Translate(mth::vec3<DBL>(10, 20, 30));
  mth::matr<DBL> M2 = M1.Transpose().Transpose();
  EXPECT_MATR_NEAR(M1, M2, 0.001, DBL);
  mth::matr<INT> M3 = mth::matr<INT>::Translate(mth::vec3<INT>(10, 20, 30));
  mth::matr<INT> M4 = M3.Transpose().Transpose();
  EXPECT_MATR_NEAR(M3, M4, 0.001, INT);
} /* End of 'MatrTest.Transpose' test */

/* Test Normal transform function */
TEST(MatrTest, TransformNormal)
{
  mth::vec3<DBL> N = mth::vec3<DBL>(0, 1, 0);
  mth::matr<DBL> M1 = mth::matr<DBL>::Translate(mth::vec3<DBL>(10, 20, 30));
  N = M1.TransformNormal(N);
  EXPECT_VEC_NEAR(N, mth::vec3<DBL>(0, 1, 0), 0.001);

  mth::vec3<DBL> N1 = mth::vec3<DBL>(1, 0, 0);
  mth::matr<DBL> M2 = mth::matr<DBL>::RotateY(90);
  N1 = M2.TransformNormal(N1);
  EXPECT_VEC_NEAR(N1, mth::vec3<DBL>(0, 0, -1), 0.00001);
} /* End of 'MatrTest.TransformNormal' test */

/* Test Normal inverse transform function */
TEST(MatrTest, InvTranslateNormal)
{
  mth::vec3<DBL> N = mth::vec3<DBL>(0, 1, 0);
  mth::matr<DBL> M1 = mth::matr<DBL>::Translate(mth::vec3<DBL>(10, 20, 30));
  N = M1.TransformNormal(N);
  N = M1.InvTransformNormal(N);
  EXPECT_VEC_NEAR(N, mth::vec3<DBL>(0, 1, 0), 0.00001);

  mth::vec3<DBL> N1 = mth::vec3<DBL>(1, 0, 0);
  mth::matr<DBL> M2 = mth::matr<DBL>::RotateY(90);
  N1 = M2.TransformNormal(N1);
  N1 = M2.InvTransformNormal(N1);
  EXPECT_VEC_NEAR(N1, mth::vec3<DBL>(1, 0, 0), 0.00001);
} /* End of 'MatrTest.InvTranslateNormal' test */

/* Test scale matrix function */
TEST(MatrTest, ScaleTest)
{
  mth::matr<DBL> msmpd = mth::matr<DBL>(2, 0, 0, 0,
                                        0, 2, 0, 0,
                                        0, 0, 2, 0,
                                        0, 0, 0, 1);
  mth::vec3<DBL> vsmpd = mth::vec3<DBL>(2, 2, 2);
  mth::matr<FLT> msmpf = mth::matr<FLT>(-30, 0, 0, 0,
                                        0, -30, 0, 0,
                                        0, 0, -47, 0,
                                        0, 0, 0, 1);
  mth::vec3<FLT> vsmpf = mth::vec3<FLT>(-30, -30, -47);

  EXPECT_MATR_NEAR(msmpd, mth::matr<DBL>::Scale(vsmpd), 1e-12, DBL);
  EXPECT_MATR_NEAR(msmpf, mth::matr<FLT>::Scale(vsmpf), 1e-5, FLT);
} /* End of 'MatrTest.ScaleTest' test */

/* Test rotateX matrix function */
TEST(MatrTest, RotateXTest)
{
  DBL angle1 = 30.0, angle2 = 45.0;
  mth::matr<DBL> msmpd = mth::matr<DBL>(1, 0, 0, 0,
                                        0, sqrt(3) / 2, 0.5, 0,
                                        0, -0.5, sqrt(3) / 2, 0,
                                        0, 0, 0, 1);

  mth::matr<FLT> msmpf = mth::matr<FLT>(1, 0, 0, 0,
                                        0, sqrt(2) / 2, sqrt(2) / 2, 0,
                                        0, -sqrt(2) / 2, sqrt(2) / 2, 0,
                                        0, 0, 0, 1);

  EXPECT_MATR_NEAR(msmpd, mth::matr<DBL>::RotateX(angle1), 1e-7, DBL);
  EXPECT_MATR_NEAR(msmpf, mth::matr<FLT>::RotateX(angle2), 1e-5, FLT);
} /* End of 'MatrTest.RotateXTest' test */

/* Test rotateY matrix function */
TEST(MatrTest, RotateYTest)
{
  DBL angle1 = 30.0, angle2 = 45.0;
  mth::matr<DBL> msmpd = mth::matr<DBL>(sqrt(3) / 2, 0, -0.5, 0,
                                        0, 1, 0, 0,
                                        0.5, 0, sqrt(3) / 2, 0,
                                        0, 0, 0, 1);

  mth::matr<FLT> msmpf = mth::matr<FLT>(sqrt(2) / 2, 0, -sqrt(2) / 2, 0,
                                        0, 1, 0, 0,
                                        sqrt(2) / 2, 0, sqrt(2) / 2, 0,
                                        0, 0, 0, 1);

  EXPECT_MATR_NEAR(msmpd, mth::matr<DBL>::RotateY(angle1), 1e-7, DBL);
  EXPECT_MATR_NEAR(msmpf, mth::matr<FLT>::RotateY(angle2), 1e-5, FLT);
} /* End of 'MatrTest.RotateYTest' test */

/* Test rotateZ matrix function */
TEST(MatrTest, RotateZTest)
{
  DBL angle1 = 30.0, angle2 = 45.0;
  mth::matr<DBL> msmpd = mth::matr<DBL>(sqrt(3) / 2, 0.5, 0, 0,
                                        -0.5, sqrt(3) / 2, 0, 0,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1);

  mth::matr<FLT> msmpf = mth::matr<FLT>(sqrt(2) / 2, sqrt(2) / 2, 0, 0,
                                        -sqrt(2) / 2, sqrt(2) / 2, 0, 0,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1);

  EXPECT_MATR_NEAR(msmpd, mth::matr<DBL>::RotateZ(angle1), 1e-7, DBL);
  EXPECT_MATR_NEAR(msmpf, mth::matr<FLT>::RotateZ(angle2), 1e-5, FLT);
} /* End of 'MatrTest.RotateZTest' test */

/* Test rotate around arbitrary axis matrix function */
TEST(MatrTest, RotateTest)
{
  DBL angle1 = 90.0;
  mth::vec3<DBL> vsmpd = mth::vec3<DBL>(0, 0, 1);
  mth::vec3<FLT> vsmpf = mth::vec3<FLT>(0, 0, 1);
  mth::matr<DBL> msmpd = mth::matr<DBL>(0, 1, 0, 0,
                                        -1, 0, 0, 0,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1);

  mth::matr<FLT> msmpf = mth::matr<FLT>(0, 1, 0, 0,
                                        -1, 0, 0, 0,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1);
  EXPECT_MATR_NEAR(msmpd, mth::matr<DBL>::Rotate(angle1, vsmpd), 1e-5, DBL);
  EXPECT_MATR_NEAR(msmpf, mth::matr<FLT>::Rotate(angle1, vsmpf), 1e-5, FLT);
} /* End of 'MatrTest.RotateTest' test */

/* Determ3x3 testing function */
TEST(MatrTest, Determ3x3)
{
EXPECT_EQ(mth::matr<INT>::Determ3x3(0, 0, 1,
                                    0, 1, 0,
                                    1, 0, 0), -1);

EXPECT_EQ(mth::matr<INT>::Determ3x3(1, 2, 3,
                                    3, 7, 1,
                                    4, 5, 6), -30);
} /* End of 'MatrTest.Determ3x3' test */

/* Determinant evaluation testing function */
TEST(MatrTest, Determ)
{
  mth::matr<DBL> m = mth::matr<DBL>(2.4, 23.4, 45.412, 43.23,
                                    4.4, 5.5, 3.3, 7.45,
                                    12.11, 3.3, 54.8, 856.45,
                                    32.3, 34.2, 8.9, 74.423);
  EXPECT_NEAR(m.Determ(), -365074.555034, 0.0005);
} /* End of 'MatrTest.Determ' test */

/* Inverse matrix testing function */
TEST(MatrTest, Inverse)
{
  mth::matr<DBL> m = mth::matr<DBL>::RotateY(30) * mth::matr<DBL>::Translate(10, 20, 30);
  mth::matr<DBL> InvM = m.Inverse();
  mth::matr<DBL> ResM = m * InvM;
  EXPECT_MATR_NEAR(ResM, mth::matr<DBL>::Identity(), 1e-10, DBL);
} /* End of 'MatrTest.Inverse' test */

/* Transform point testing function */
TEST(MatrTest, TransformPoint)
{
  mth::vec3<INT> v = mth::vec3<INT>(0, 0, 1);

  mth::matr<INT> m1 = mth::matr<INT>::Translate(10, 10, 10);
  mth::vec3<INT> v1 = m1.TransformPoint(v);
  EXPECT_VEC3_EQ(v1, mth::vec3<INT>(10, 10, 11));

  mth::vec3<DBL> V = mth::vec3<DBL>(0, 0, 1);
  mth::matr<DBL> m2 = mth::matr<DBL>::RotateX(90);
  mth::vec3<DBL> v2 = m2.TransformPoint(V);
  EXPECT_VEC_NEAR(v2, mth::vec3<DBL>(0.0, -1.0, 0.0), 0.0005);
} /* End of 'MatrTest.TransformPoint' test */

/* Inverse transform point testing function */
TEST(MatrTest, InvTransformPoint)
{
  mth::vec3<INT> v = mth::vec3<INT>(0, 0, 1);

  mth::matr<INT> m1 = mth::matr<INT>::Translate(10, 10, 10);
  mth::vec3<INT> v1 = m1.TransformPoint(v);
  mth::vec3<INT> v2 = m1.InvTransformPoint(v1);
  EXPECT_VEC3_EQ(v2, mth::vec3<INT>(0, 0, 1));

  mth::vec3<DBL> V = mth::vec3<DBL>(10.0, 12.0, 6.0);
  mth::matr<DBL> m2 = mth::matr<DBL>::Rotate(90, mth::vec3<DBL>(30.0, 18.0, 47.0));
  mth::vec3<DBL> v3 = m2.TransformPoint(V);
  mth::vec3<DBL> v4 = m2.InvTransformPoint(v3);
  EXPECT_VEC_NEAR(v4, mth::vec3<DBL>(10.0, 12.0, 6.0), 0.0005);
} /* End of 'MatrTest.InvTransformPoint' test */

/* Test matrices multiplication function */
TEST(MatrTest, MatrMulMatr)
{
  mth::matr<DBL> M1 = mth::matr<DBL>::Translate(mth::vec3<DBL>(10, 20, 30));
  mth::matr<DBL> M2 = mth::matr<DBL>::Identity();
  EXPECT_MATR_NEAR((M1 * M2), M1, 0.001, DBL);
  EXPECT_MATR_NEAR((M2 * M1), M1, 0.001, DBL);

  mth::matr<DBL> M3 = mth::matr<DBL>::Scale(mth::vec3<DBL>(18.47, 30.18, 102.30));
  mth::matr<DBL> M4 = M3.Inverse().Transpose();
  EXPECT_MATR_NEAR((M3 * M4), M2, 0.001, DBL);
  EXPECT_MATR_NEAR((M4 * M3), M2, 0.001, DBL);

  mth::vec3<DBL> P = mth::vec3<DBL>(1, 0, 0);
  mth::matr<DBL> M5 = mth::matr<DBL>::Translate(9, 0, 0);
  mth::matr<DBL> M6 = mth::matr<DBL>::RotateY(180);
  EXPECT_VEC_NEAR(((M5 * M6).TransformPoint(P)), mth::vec3<DBL>(-10, 0, 0), 0.00001);
} /* End of 'MatrTest.MatrMulMatr' test */

/* Ortho matrix testing function */
TEST(MatrTest, Ortho)
{
  mth::matr<DBL> ortho = mth::matr<DBL>::Ortho(0.5, 2.5, 0.8, 1.2, 4.4, 1.5);
  EXPECT_MATR_NEAR(ortho, mth::matr<DBL>(1.0, 0, 0, 0,
                                         0, 5.0, 0, 0,
                                         0, 0, 0.68965517, 0,
                                         -1.5, -5.0, 2.034482758, 1), 0.0005, DBL);
} /* End of 'MatrTest.Ortho' test */

/* View matrix testing function */
TEST(MatrTest, View)
{
  mth::vec3<DBL> Loc = mth::vec3<DBL>(0, 0, 0);
  mth::vec3<DBL> At = mth::vec3<DBL>(0, 0, -1);
  mth::vec3<DBL> Up = mth::vec3<DBL>(0, 1, 0);

  mth::matr<DBL> view = mth::matr<DBL>::View(Loc, At, Up);
  EXPECT_MATR_NEAR(view, mth::matr<DBL>::Identity(), 1e-10, DBL);
} /* End of 'MatrTest.View' test */

/* Frustum matrix testing function */
TEST(MatrTest, Frustum)
{
  mth::matr<DBL> frustum = mth::matr<DBL>::Frustum(-1, 1, -1, 1, 1, 100);
  EXPECT_MATR_NEAR(frustum, mth::matr<DBL>(1, 0, 0, 0,
                                           0, 1, 0, 0,
                                           0, 0, -1.02020202, -1,
                                           0, 0, -2.02020202, 0), 0.0005, DBL);
} /* End of 'MatrTest.Frustum' test */

/* Transform vector testing function */
TEST(MatrTest, TransformVector)
{
  mth::vec3<DBL> v = mth::vec3<DBL>(1, 0, 0);
  mth::matr<DBL> m = mth::matr<DBL>::Translate(10, 20, 30);

  mth::vec3<DBL> v1 = m.TransformVector(v);
  EXPECT_VEC_NEAR(v1, mth::vec3<DBL>(1, 0, 0), 0.00001);

  mth::matr<DBL> rot = mth::matr<DBL>::RotateY(90);
  mth::vec3<DBL> v2 = rot.TransformVector(v);
  EXPECT_VEC_NEAR(v2, mth::vec3<DBL>(0, 0, -1), 0.00001);
} /* End of 'MatrTest.TransformVector' test */

/* Quaternion rotation matrix testing function */
TEST(MatrTest, RotateQuat)
{
  mth::vec4<DBL> q = mth::vec4<DBL>(0, 0, 0, 1); // Identity quaternion
  mth::matr<DBL> m = mth::matr<DBL>::RotateQuat(q);
  EXPECT_MATR_NEAR(m, mth::matr<DBL>::Identity(), 1e-10, DBL);

  // 90 degree rotation around Y axis
  mth::vec4<DBL> q2 = mth::vec4<DBL>(0, sin(D2R(45)), 0, cos(D2R(45)));
  mth::matr<DBL> m2 = mth::matr<DBL>::RotateQuat(q2);
  mth::matr<DBL> expected = mth::matr<DBL>::RotateY(90);
  EXPECT_MATR_NEAR(m2, expected, 1e-10, DBL);
} /* End of 'MatrTest.RotateQuat' test */

/* END OF 'tam_test_quat.cpp' FILE */
