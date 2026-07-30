/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : comp.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Matrix recalculation shader.
 *               Compute shader implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 24.04.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

// Camera buffer type
layout(set = 0, binding = 0) readonly buffer Camera
{
  mat4 MatrV;         // View matrix
  mat4 MatrP;         // Projection matrix
  mat4 MatrVP;        // View-proj matrix
  vec4 CamLocFrameW;  // Camera's location vector and frame width
  vec4 CamDirFrameH;  // Camera's look direction vector and frame height
  vec4 CamRightWp;    // Camera's right direction and project plane width
  vec4 CamUpHp;       // Camera's up direction vector and project plane height
  vec4 CamAtProjDist; // Camera look-at point vector and near project plane distance
  vec4 ProjSize;      // inner project plane rectangle size
}; /* End of 'Camera' buffer */

/* Buffer with global world matrices */
layout(set = 0, binding = 1) buffer MatrsIn
{
  mat4 MatrsWIn[]; // input world matrices
}; /* End of 'MatrsIn' buffer */

/* Result matrices data representation type */
struct result_matrices
{
  mat4 WVP;  // World-View-Proj matrix
  mat4 WInv; // World transposed inverse matrix
}; /* End of 'result_matrices' structure */

/* Recalculated WVP and WInv matrices buffer */ 
layout(set = 0, binding = 2) buffer MatrsOut                     
{                                                       
  result_matrices Matrs[]; // output matrices              
}; /* End of 'MatrsOut' buffer */
                                
/* Compute shader main entry point function.
 * ARGUMNETS: None.
 * RETURNS: None.
 */
void main( void )
{
  int n = int(gl_GlobalInvocationID.x + 1024 * gl_GlobalInvocationID.y + 1024 * 1024 * gl_GlobalInvocationID.z); /*gl_LocalInvocationIndex*/
  float FltModelId = MatrsWIn[n][3][3];
  int ModelId = int(FltModelId);
  if (FltModelId - ModelId > 0.5)
    ModelId++;
  MatrsWIn[n][3][3] = 1;
  Matrs[n].WVP = MatrVP * MatrsWIn[n]; 
  Matrs[n].WInv = inverse(transpose(MatrsWIn[n]));
  Matrs[n].WInv[3][3] = ModelId;
} /* End of 'main' function */ 

/* END OF 'comp.glsl' FILE */
