/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : vert.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               PBR shader.
 *               Vertex shader implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 10.04.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

#define VERTEX_STD7 1
#include "commondf.glsl"

/* Joint matrices buffer */
layout(set = 1, binding = 0) buffer BUF_MATR
{
  mat4 JointMatrBuf[];
};

/* Inverse joint matrices buffer */
layout(set = 1, binding = 1) buffer BUF_MATR_INV
{
  mat4 InvJointMatrBuf[];
};

/* Main vertex shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{                   
   mat4 MatrSkin =
     InWeights.x * JointMatrBuf[InJoints.x] * InvJointMatrBuf[InJoints.x] +
     InWeights.y * JointMatrBuf[InJoints.y] * InvJointMatrBuf[InJoints.y] +
     InWeights.z * JointMatrBuf[InJoints.z] * InvJointMatrBuf[InJoints.z] +
     InWeights.w * JointMatrBuf[InJoints.w] * InvJointMatrBuf[InJoints.w];
  gl_Position = MatrWVP * MatrSkin * (vec4(InPosition, 1));
  
  DrawColor = InColor;
  DrawTexCoord = InTexCoord;
  DrawNormal = mat3(inverse(transpose(MatrW * MatrSkin))) * InNormal;
  DrawNormal = normalize(DrawNormal);
  DrawPosId = vec4((MatrW * MatrSkin * (vec4(InPosition, 1))).xyz, MatrWInvTrans[3][3]);
  ModelId = int(MatrWInvTrans[3][3]);
  DrawTangent = vec4(normalize(mat3(inverse(transpose(MatrW * MatrSkin))) * InTangent.xyz), InTangent.w);
  DrawJoints = InJoints;
  DrawWeights = InWeights;
} /* End of 'main' function */

/* END OF 'vert.glsl' FILE */
