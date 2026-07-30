/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_test_system.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Math quaternion test module.
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

/* Test quaternion constructor function */
TEST(QuatTest, Constructor)
{
  mth::quat<DBL> q1;
  EXPECT_DBL_EQ(q1.X, 0.0);
  EXPECT_DBL_EQ(q1.Y, 0.0);
  EXPECT_DBL_EQ(q1.Z, 0.0);
  EXPECT_DBL_EQ(q1.W, 1.0);

  mth::quat<DBL> q2(1.0, 2.0, 3.0, 4.0);
  EXPECT_DBL_EQ(q2.X, 1.0);
  EXPECT_DBL_EQ(q2.Y, 2.0);
  EXPECT_DBL_EQ(q2.Z, 3.0);
  EXPECT_DBL_EQ(q2.W, 4.0);

  mth::quat<FLT> q3(1.0f, 2.0f, 3.0f, 4.0f);
  EXPECT_FLT_EQ(q3.X, 1.0f);
  EXPECT_FLT_EQ(q3.Y, 2.0f);
  EXPECT_FLT_EQ(q3.Z, 3.0f);
  EXPECT_FLT_EQ(q3.W, 4.0f);
  } /* End of 'QuatTest.Constructor' test */

  /* Test quaternion identity function */
  TEST(QuatTest, Identity)
  {
  mth::quat<DBL> q = mth::quat<DBL>::Identity();
  EXPECT_DBL_EQ(q.X, 0.0);
  EXPECT_DBL_EQ(q.Y, 0.0);
  EXPECT_DBL_EQ(q.Z, 0.0);
  EXPECT_DBL_EQ(q.W, 1.0);
  } /* End of 'QuatTest.Identity' test */

  /* Test quaternion conjugate function */
  TEST(QuatTest, Conjugate)
  {
  mth::quat<DBL> q1(1.0, 2.0, 3.0, 4.0);
  mth::quat<DBL> conj = q1.Conjugate();
  EXPECT_DBL_EQ(conj.X, -1.0);
  EXPECT_DBL_EQ(conj.Y, -2.0);
  EXPECT_DBL_EQ(conj.Z, -3.0);
  EXPECT_DBL_EQ(conj.W, 4.0);
} /* End of 'QuatTest.Conjugate' test */

/* Test quaternion inverse function */
TEST(QuatTest, Inverse)
{
  mth::quat<DBL> q1(1.0, 2.0, 3.0, 4.0);
  mth::quat<DBL> inv = q1.Inverse();
  mth::quat<DBL> prod = q1 * inv;
  EXPECT_NEAR(prod.X, 0.0, 1e-12);
  EXPECT_NEAR(prod.Y, 0.0, 1e-12);
  EXPECT_NEAR(prod.Z, 0.0, 1e-12);
  EXPECT_NEAR(prod.W, 1.0, 1e-12);

  mth::quat<DBL> q2 = mth::quat<DBL>::Identity();
  mth::quat<DBL> inv2 = q2.Inverse();
  EXPECT_DBL_EQ(inv2.X, 0.0);
  EXPECT_DBL_EQ(inv2.Y, 0.0);
  EXPECT_DBL_EQ(inv2.Z, 0.0);
  EXPECT_DBL_EQ(inv2.W, 1.0);
} /* End of 'QuatTest.Inverse' test */

/* Test quaternion normalization function */
TEST(QuatTest, Normalize)
{
  mth::quat<DBL> q1(1.0, 2.0, 3.0, 4.0);
  q1.Normalize();
  DBL len = !q1;
  EXPECT_NEAR(len, 1.0, 1e-12);

  mth::quat<DBL> q2(0.0, 0.0, 0.0, 1.0);
  q2.Normalize();
  EXPECT_DBL_EQ(q2.X, 0.0);
  EXPECT_DBL_EQ(q2.Y, 0.0);
  EXPECT_DBL_EQ(q2.Z, 0.0);
  EXPECT_DBL_EQ(q2.W, 1.0);

  mth::quat<DBL> q3(1.0, 2.0, 3.0, 4.0);
  mth::quat<DBL> norm = q3.Normalizing();
  DBL len2 = !norm;
  EXPECT_NEAR(len2, 1.0, 1e-12);
} /* End of 'QuatTest.Normalize' test */

/* Test quaternion multiplication function */
TEST(QuatTest, Multiplication)
{
  mth::quat<DBL> q1(1.0, 2.0, 3.0, 4.0);
  mth::quat<DBL> q2(5.0, 6.0, 7.0, 8.0);
  mth::quat<DBL> prod = q1 * q2;

  EXPECT_NEAR(prod.X, 24.0, 1e-12);
  EXPECT_NEAR(prod.Y, 48.0, 1e-12);
  EXPECT_NEAR(prod.Z, 48.0, 1e-12);
  EXPECT_NEAR(prod.W, -6.0, 1e-12);

  mth::quat<DBL> q3(0.5, 0.5, 0.5, 0.5);
  mth::quat<DBL> left = (q1 * q2) * q3;
  mth::quat<DBL> right = q1 * (q2 * q3);
  EXPECT_NEAR(left.X, right.X, 1e-12);
  EXPECT_NEAR(left.Y, right.Y, 1e-12);
  EXPECT_NEAR(left.Z, right.Z, 1e-12);
  EXPECT_NEAR(left.W, right.W, 1e-12);
} /* End of 'QuatTest.Multiplication' test */

/* Test quaternion division function */
TEST(QuatTest, Division)
{
  mth::quat<DBL> q1(1.0, 2.0, 3.0, 4.0);
  mth::quat<DBL> q2(5.0, 6.0, 7.0, 8.0);
  mth::quat<DBL> div = q1 / q2;
  mth::quat<DBL> check = div * q2;

  EXPECT_NEAR(check.X, q1.X, 1e-12);
  EXPECT_NEAR(check.Y, q1.Y, 1e-12);
  EXPECT_NEAR(check.Z, q1.Z, 1e-12);
  EXPECT_NEAR(check.W, q1.W, 1e-12);
} /* End of 'QuatTest.Division' test */

/* Test quaternion rotation from axis and angle function */
TEST(QuatTest, RotateAxisAngle)
{
  mth::vec3<DBL> axis(0.0, 1.0, 0.0);
  DBL angle = D2R(90.0);

  mth::quat<DBL> q = mth::quat<DBL>::Rotate(axis, angle);

  DBL len = !q;
  EXPECT_NEAR(len, 1.0, 1e-12);

   EXPECT_NEAR(q.X, 0.0, 1e-12);
  EXPECT_NEAR(q.Y, sin(angle / 2.0), 1e-12);
  EXPECT_NEAR(q.Z, 0.0, 1e-12);
  EXPECT_NEAR(q.W, cos(angle / 2.0), 1e-12);
} /* End of 'QuatTest.RotateAxisAngle' test */

/* Test quaternion arithmetic operators function */
TEST(QuatTest, ArithmeticOperators)
{
  mth::quat<DBL> q1(1.0, 2.0, 3.0, 4.0);
  mth::quat<DBL> q2(5.0, 6.0, 7.0, 8.0);

  // Test addition
  mth::quat<DBL> sum = q1 + q2;
  EXPECT_DBL_EQ(sum.X, 6.0);
  EXPECT_DBL_EQ(sum.Y, 8.0);
  EXPECT_DBL_EQ(sum.Z, 10.0);
  EXPECT_DBL_EQ(sum.W, 12.0);

  // Test subtraction
  mth::quat<DBL> diff = q1 - q2;
  EXPECT_DBL_EQ(diff.X, -4.0);
  EXPECT_DBL_EQ(diff.Y, -4.0);
  EXPECT_DBL_EQ(diff.Z, -4.0);
  EXPECT_DBL_EQ(diff.W, -4.0);

  // Test negation
  mth::quat<DBL> neg = -q1;
  EXPECT_DBL_EQ(neg.X, -1.0);
  EXPECT_DBL_EQ(neg.Y, -2.0);
  EXPECT_DBL_EQ(neg.Z, -3.0);
  EXPECT_DBL_EQ(neg.W, -4.0);

  // Test scalar multiplication
  mth::quat<DBL> scaled = q1 * 2.0;
  EXPECT_DBL_EQ(scaled.X, 2.0);
  EXPECT_DBL_EQ(scaled.Y, 4.0);
  EXPECT_DBL_EQ(scaled.Z, 6.0);
  EXPECT_DBL_EQ(scaled.W, 8.0);

  // Test scalar division
  mth::quat<DBL> divided = q1 / 2.0;
  EXPECT_DBL_EQ(divided.X, 0.5);
  EXPECT_DBL_EQ(divided.Y, 1.0);
  EXPECT_DBL_EQ(divided.Z, 1.5);
  EXPECT_DBL_EQ(divided.W, 2.0);
} /* End of 'QuatTest.ArithmeticOperators' test */

/* Test quaternion dot product function */
TEST(QuatTest, DotProduct)
{
  mth::quat<DBL> q1(1.0, 2.0, 3.0, 4.0);
  mth::quat<DBL> q2(5.0, 6.0, 7.0, 8.0);

  DBL dot = q1 & q2;
  EXPECT_DBL_EQ(dot, 70.0); // 1*5 + 2*6 + 3*7 + 4*8 = 5 + 12 + 21 + 32 = 70
} /* End of 'QuatTest.DotProduct' test */

/* Test quaternion length function */
TEST(QuatTest, Length)
{
  mth::quat<DBL> q1(0.0, 0.0, 0.0, 1.0);
  DBL len1 = !q1;
  EXPECT_DBL_EQ(len1, 1.0);

  mth::quat<DBL> q2(1.0, 2.0, 3.0, 4.0);
  DBL len2 = !q2;
  EXPECT_NEAR(len2, sqrt(30.0), 1e-12); // sqrt(1^2 + 2^2 + 3^2 + 4^2) = sqrt(30)
} /* End of 'QuatTest.Length' test */

/* Test quaternion assignment operators function */
TEST(QuatTest, AssignmentOperators)
{
  mth::quat<DBL> q1(1.0, 2.0, 3.0, 4.0);
  mth::quat<DBL> q2(5.0, 6.0, 7.0, 8.0);

  // Test +=
  mth::quat<DBL> q3 = q1;
  q3 += q2;
  EXPECT_DBL_EQ(q3.X, 6.0);
  EXPECT_DBL_EQ(q3.Y, 8.0);
  EXPECT_DBL_EQ(q3.Z, 10.0);
  EXPECT_DBL_EQ(q3.W, 12.0);

  // Test -=
  mth::quat<DBL> q4 = q1;
  q4 -= q2;
  EXPECT_DBL_EQ(q4.X, -4.0);
  EXPECT_DBL_EQ(q4.Y, -4.0);
  EXPECT_DBL_EQ(q4.Z, -4.0);
  EXPECT_DBL_EQ(q4.W, -4.0);

  // Test *=
  mth::quat<DBL> q5 = q1;
  q5 *= 2.0;
  EXPECT_DBL_EQ(q5.X, 2.0);
  EXPECT_DBL_EQ(q5.Y, 4.0);
  EXPECT_DBL_EQ(q5.Z, 6.0);
  EXPECT_DBL_EQ(q5.W, 8.0);

  // Test /=
  mth::quat<DBL> q6 = q1;
  q6 /= 2.0;
  EXPECT_DBL_EQ(q6.X, 0.5);
  EXPECT_DBL_EQ(q6.Y, 1.0);
  EXPECT_DBL_EQ(q6.Z, 1.5);
  EXPECT_DBL_EQ(q6.W, 2.0);

  // Test *= with quaternion
  mth::quat<DBL> q7 = mth::quat<DBL>::Identity();
  q7 *= q1;
  EXPECT_DBL_EQ(q7.X, q1.X);
  EXPECT_DBL_EQ(q7.Y, q1.Y);
  EXPECT_DBL_EQ(q7.Z, q1.Z);
  EXPECT_DBL_EQ(q7.W, q1.W);

  // Test /= with quaternion
  mth::quat<DBL> q8 = q1;
  q8 /= q1;
  EXPECT_NEAR(q8.X, 0.0, 1e-12);
  EXPECT_NEAR(q8.Y, 0.0, 1e-12);
  EXPECT_NEAR(q8.Z, 0.0, 1e-12);
  EXPECT_NEAR(q8.W, 1.0, 1e-12);
} /* End of 'QuatTest.AssignmentOperators' test */

/* END OF 'tam_test_quat.cpp' FILE */
