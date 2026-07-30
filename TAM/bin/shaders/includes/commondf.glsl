/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : commondf.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Common definitions shader.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 24.04.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#define PI 3.14159265358979323846

// Synchronization buffer type
layout(set = 0, binding = 0) readonly buffer Sync
{
  vec4 SyncGlobalTimeGlobalDeltaTimeTimeDeltaTime; // Global time, global deltatime, time, deltatime
  vec4 IsWireframePauseDebugBrightness;            // Wireframe, pause and debug flags and brightness multiplier values
};

#define GlobalTime SyncGlobalTimeGlobalDeltaTimeTimeDeltaTime.x
#define GlobalDeltaTime SyncGlobalTimeGlobalDeltaTimeTimeDeltaTime.y
#define Time SyncGlobalTimeGlobalDeltaTimeTimeDeltaTime.z
#define DeltaTime SyncGlobalTimeGlobalDeltaTimeTimeDeltaTime.w
#define IsWireframe bool(IsWireframePauseDebugBrightness.x)
#define IsPause bool(IsWireframePauseDebugBrightness.y)
#define IsDebug bool(IsWireframePauseDebugBrightness.z)
#define Brightness (IsWireframePauseDebugBrightness.w)

// Camera buffer type
layout(set = 0, binding = 1) readonly buffer Camera
{
  mat4 MatrV;         // View matrix
  mat4 MatrP;         // Projection matrix
  mat4 MatrVP;        // View-proj matrix
  vec4 CamLocFrameW;  // Camera's location vector and frame width
  vec4 CamDirFrameH;  // Camera's look direction vector and frame height
  vec4 CamRightWp;    // Camera's right direction and project plane width
  vec4 CamUpHp;       // Camera's up direction vector and project plane height
  vec4 CamAtProjDist; // Camera look-at point vector and near project plane distance
  vec4 ProjSizeOffsetXY;    // Inner project plane rectangle size and viewport offset  vec4 MouseXYLR;     // Mouse position and button states
  vec4 MouseXYLR;           // Mouse position and button states
  ivec4 ObjectId4;      // Current object Id
}; /* End of 'Camera' buffer */

#define CamAt CamAtProjDist.xyz
#define CamLoc CamLocFrameW.xyz
#define CamRight CamRightWp.xyz
#define CamUp CamUpHp.xyz
#define CamDir CamDirFrameH.xyz
 
#define FrameW CamLocFrameW.w
#define FrameH CamDirFrameH.w
#define ProjSize ProjSizeOffsetXY.x
#define ProjDist CamAtProjDist.w
#define MouseX int(MouseXYLR.x)
#define MouseY int(MouseXYLR.y)
#define IsMouseLB bool(MouseXYLR.z)
#define IsMouseRB bool(MouseXYLR.w)
#define CurrentObjectId int(ObjectId4.x)
#define FrameOffsetX ProjSizeOffsetXY.y
#define FrameOffsetY ProjSizeOffsetXY.z

/* Primitive first matrix index in matrices pool push constant */
layout(std140, push_constant) uniform PrimMatrInd
{
  ivec2 PushConstantsPack;
  //int FirstMatrixIndex; // Primitive first matrix index
  //int CurrentObjectId;  // Draw primitive/model object Id
}; /* End of 'PrimMatrInd' uniform */

#define FirstMatrixIndex PushConstantsPack.x
#define DrawObjectId  PushConstantsPack.y

/* Buffer with global world matrices */
layout(set = 0, binding = 2) buffer MatrsIn
{
  mat4 MatrsWIn[]; // Input world matrices
}; /* End of 'MatrsIn' buffer */

/* Result matrices data representation type */
struct result_matrices
{
  mat4 BufWVP;  // World-View-Proj matrix
  mat4 BufWInv; // World transposed inverse matrix
                // ! At [3][3] element instead of '1' contains model id
}; /* End of 'result_matrices' structure */ 

/* Recalculated WVP and WInv matrices buffer */ 
layout(set = 0, binding = 3) buffer MatrsOut                     
{                                                       
  result_matrices ResultMatrs[]; // output matrices              
}; /* End of 'MatrsOut' buffer */

/* Buffer with global world matrices */
layout(set = 0, binding = 4) buffer OpaqueIds
{
  int OpaqueId[]; // Per-pixel opaque models indices
}; /* End of 'OpaqueIds' buffer */

//#define OpaqueId(IND) (OpaqueIdBuf[int(IND / 3)][int(IND) % 3])

/* Buffer with global world matrices */
layout(set = 0, binding = 5) buffer TransparentIds
{
  int TransparentId[]; // Per-pixel transparent models indices
}; /* End of 'TransparentIds' buffer */

//#define TransparentId(IND) (TransparentIdBuf[int(IND / 3)][IND % 3])
// #define ModelIndexBufferPos(X, Y) (X + Y * FrameW)

#ifdef VERTEX_SHADER
#define MatrW MatrsWIn[FirstMatrixIndex + gl_InstanceIndex]
//#define MatrWRes MatrsWIn[FirstMatrixIndex + gl_InstanceIndex]
#define MatrWVP ResultMatrs[FirstMatrixIndex + gl_InstanceIndex].BufWVP
#define MatrWInvTrans ResultMatrs[FirstMatrixIndex + gl_InstanceIndex].BufWInv
#endif // VERTEX_SHADER

#ifdef CONTROL_SHADER
#define MatrW MatrsWIn[FirstMatrixIndex + VertInstanceIndex[0]]
//#define MatrWRes MatrsWIn[FirstMatrixIndex + VertInstanceIndex[0]]
#define MatrWVP ResultMatrs[FirstMatrixIndex + VertInstanceIndex[0]].BufWVP
#define MatrWInvTrans ResultMatrs[FirstMatrixIndex + VertInstanceIndex[0]].BufWInv
#endif // CONTROL_SHADER

#ifdef EVALUATION_SHADER
//#define MatrWRes MatrsWIn[FirstMatrixIndex + TessCtrlInstanceIndex[0]]
#define MatrW MatrsWIn[FirstMatrixIndex + TessCtrlInstanceIndex[0]]
#define MatrWVP ResultMatrs[FirstMatrixIndex + TessCtrlInstanceIndex[0]].BufWVP
#define MatrWInvTrans ResultMatrs[FirstMatrixIndex + TessCtrlInstanceIndex[0]].BufWInv
#endif // EVALUATION_SHADER

#ifdef GEOMETRY_SHADER
#define MatrW MatrsWIn[FirstMatrixIndex + VertInstanceIndex]
//#define MatrWRes MatrsWIn[FirstMatrixIndex + VertInstanceIndex]
#define MatrWVP ResultMatrs[FirstMatrixIndex + VertInstanceIndex].BufWVP
#define MatrWInvTrans ResultMatrs[FirstMatrixIndex + VertInstanceIndex].BufWInv
#endif // EVALUATION_SHADER

#ifdef FRAGMENT_SHADER
#define MatrW MatrsWIn[FirstMatrixIndex + VertInstanceIndex]
//#define MatrWRes MatrsWIn[FirstMatrixIndex + VertInstanceIndex]
#define MatrWVP ResultMatrs[FirstMatrixIndex + VertInstanceIndex].BufWVP
#define MatrWInvTrans ResultMatrs[FirstMatrixIndex + VertInstanceIndex].BufWInv
#endif // EVALUATION_SHADER

#ifdef FRAGMENT_SHADER

/* Power 3D vector function.
 * ARGUMENTS:
 *   - initial vector:
 *       vec3 V;
 *   - degree:
 *       float Deg;
 * RETURNS:
 *   (vec3) result vector.
 */
vec3 PowVec3( vec3 V, float Deg )
{
  return vec3(pow(V.x, Deg), pow(V.y, Deg), pow(V.z, Deg));
} /* End of 'PowVec3' function 

/* Interpolate 3D vector function.
 * ARGUMENTS:
 *   - minimum vector:
 *       vec3 Min;
 *   - maximum vector:
 *       vec3 Max;
 *   - value:
 *       float Val;
 * RETURNS:
 *   (vec3) result vector.
 */
vec3 LerpVec3( vec3 Min, vec3 Max, float Val )
{
  return vec3(mix(Min.x, Max.x, Val), mix(Min.y, Max.y, Val), mix(Min.z, Max.z, Val));
} /* End of 'LerpVec3' function 

/* Shade point with Phong's model function.
 * ARGUMENTS:
 *   - camera position:
 *       vec3 CamPos;
 *   - position of shading point:
 *       vec3 P;
 *   - shading normal:
 *       vec3 N;
 *   - diffuse coefficient:
 *       vec3 Kd;
 *   - specular coefficient:
 *       vec3 Ks;
 *   - Phong's coefficient:
 *       float Ph;
 *   - direction to light:
 *       vec3 LDir;
 *   - light color:
 *       vec3 LColor;
 */
vec3 ShadePhong( vec3 CamPos, vec3 P, vec3 N, vec3 Kd, vec3 Ks, float Ph, vec3 LDir, vec3 LColor )
{
  vec3 color = vec3(0);
  vec3 V = normalize(P - CamPos);
  //N = faceforward(N, V, N);
  
  // Diffuse
  color += max(0, dot(N, LDir)) * Kd * LColor;
  // Specular
  vec3 R = reflect(V, N);
  color += pow(max(0, dot(R, LDir)), Ph) * Ks * LColor;
  return color;
} /* End of 'ShadePhong' function */
 
#endif // FRAGMENT_SHADER

// STD4 and STD7

#ifdef VERTEX_STD4
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

layout(location = 0) out vec4 DrawColor;
layout(location = 1) out vec3 DrawNormal;
layout(location = 2) out vec4 DrawPosId;
layout(location = 3) out vec2 DrawTexCoord;
layout(location = 4) out flat int ModelId;
#endif // VERTEX_STD4

#ifdef FRAGMENT_STD4
layout(location = 0) in vec4 DrawColor;
layout(location = 1) in vec3 DrawNormal;
layout(location = 2) in vec4 DrawPosId;
layout(location = 3) in vec2 DrawTexCoord;
layout(location = 4) in flat int ModelId;
#endif // FRAGMENT_STD4

#ifdef VERTEX_STD7
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;
layout(location = 4) in vec4 InTangent;
layout(location = 5) in ivec4 InJoints;
layout(location = 6) in vec4 InWeights;

layout(location = 0) out vec4 DrawColor;
layout(location = 1) out vec3 DrawNormal;
layout(location = 2) out vec4 DrawPosId;
layout(location = 3) out vec2 DrawTexCoord;
layout(location = 4) out vec4 DrawTangent;
layout(location = 5) out ivec4 DrawJoints;
layout(location = 6) out vec4 DrawWeights;
layout(location = 7) out flat int ModelId;
#endif // VERTEX_STD7

#ifdef FRAGMENT_STD7
layout(location = 0) in vec4 DrawColor;
layout(location = 1) in vec3 DrawNormal;
layout(location = 2) in vec4 DrawPosId;
layout(location = 3) in vec2 DrawTexCoord;
layout(location = 4) in vec4 DrawTangent;
layout(location = 5) in flat ivec4 DrawJoints;
layout(location = 6) in vec4 DrawWeights;
layout(location = 7) in flat int ModelId;
#endif // FRAGMENT_STD7
                  
/* END OF 'commondf.glsl' FILE */
