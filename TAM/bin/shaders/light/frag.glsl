/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : frag.glsl
 * PURPOSE     : Tough Arrangement Manager project.
 *               Default shader.
 *               Fragment shader implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 31.03.2026
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#version 460

#include "commondf.glsl"

#define latitude (0.333333f * PI)

layout(input_attachment_index = 0, set = 1, binding = 1) uniform subpassInput InBaseColor;
layout(input_attachment_index = 1, set = 1, binding = 2) uniform subpassInput InPos;
layout(input_attachment_index = 2, set = 1, binding = 3) uniform subpassInput InNormal;
layout(input_attachment_index = 3, set = 1, binding = 4) uniform subpassInput InMetallicRoughness;
layout(input_attachment_index = 6, set = 1, binding = 7) uniform subpassInput InDepth;

/* light resource data representation type */
struct light
{
  vec4 PosXYZ_DirX;
  vec4 DirYZ_ColorXY;
  vec4 ColorZ_Int_Type_IsEnabled;
  vec4 Exp_Angle_0_0;
}; /* End of 'light' structure */ 

// Lights buffer type
layout(std430, set = 1, binding = 0) readonly buffer Lig
{
  vec4 Size_timeOfDay_dayOfYear;
  light Lights[];
}; /* End of 'Lig' buffer */

layout(location = 0) out vec4 OutColor;

/* Compute distribution function.
 * ARGUMENTS:
 *   - Normal vector of point:
 *       vec3 N;
 *   - half vector between light vector and incident vector:
 *       vec3 H;
 *   - physic koef of roughness:
 *       float roughness;
 * RETURNS:
 *   (float) distribution.
 */
float DistributionGGX( vec3 N, vec3 H, float roughness )
{
  float a = roughness * roughness;
  float a2 = a * a;
  float NdotH = max(0.0, dot(N, H));
  float NdotH2 = NdotH * NdotH;

  float nom = a2;
  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
  denom = PI * denom * denom;

  return nom / denom;
} /* End of 'DistributionGGX' function */

/* Compute GeometrySchlick function.
 * ARGUMENTS:
 *   - dot product of normal and incident vectors:
 *       float NdotV;
 *   - physic koef of roughness:
 *       float roughness;
 * RETURNS:
 *   (float) GeometrySchlick.
 */
float GeometrySchlickGGX( float NdotV, float roughness )
{
  float r = (roughness + 1.0);
  float k = (r * r) / 8.0;

  float nom = NdotV;
  float denom = NdotV * (1.0 - k) + k;

  return nom / denom;
} /* End of 'GeometrySchlickGGX' function */

/* Compute GeometrySmith function.
 * ARGUMENTS:
 *   - Normal vector of point:
 *       vec3 N;
 *   - Incident vector of point:
 *       vec3 V;
 *   - Light direction vector of point:
 *       vec3 L;
 *   - dot product of normal and incident vectors:
 *       float NdotV;
 *   - physic koef of roughness:
 *       float roughness;
 * RETURNS:
 *   (float) GeometrySmith.
 */
float GeometrySmith( vec3 N, vec3 V, vec3 L, float roughness )
{
  float NdotV = max(0.0, dot(N, V));
  float NdotL = max(0.0, dot(N, L));
  float ggx2 = GeometrySchlickGGX(NdotV, roughness);
  float ggx1 = GeometrySchlickGGX(NdotL, roughness);

  return ggx1 * ggx2;
} /* End of 'GeometrySmith' function */

/* Compute distribution function.
 * ARGUMENTS:
 *   - cosine of angle:
 *       float cosTheta;
 *   - physics koef of metallic:
 *       vec3 F0;
 * RETURNS:
 *   (vec3) fresnelSchlick.
 */
vec3 fresnelSchlick( float cosTheta, vec3 F0 )
{
  return F0 + (1.0 - F0) * pow((1.0 + 0.000001) - cosTheta, 5.0);
} /* End of 'fresnelSchlick' function */

/* Compute distribution function.
 * ARGUMENTS:
 *   - Normal vector of point:
 *       vec3 N;
 *   - Incident vector of point:
 *       vec3 Ve;
 *   - physics koef of metallic:
 *       vec3 F0;
 *   - Light direction vector of point:
 *       vec3 L;
 *   - dot product of normal and incident vectors:
 *       float NdotV;
 *   - physic koef of roughness:
 *       float roughness;
 * RETURNS:
 *   (vec3) pixel color.
 */
vec3 PBR( vec3 N, vec3 Ve, vec3 F0, vec3 albedo, vec3 L, vec3 H, vec3 light_col, float intensity, float metallic, float roughness )
{
  vec3 radiance = light_col * intensity; //Incoming Radiance

  // cook-torrance brdf
  float NDF = DistributionGGX(N, H, roughness);
  float G = GeometrySmith(N, Ve, L, roughness);
  vec3 F = fresnelSchlick(max(0.0, dot(H, Ve)), F0);

  vec3 kS = F;
  vec3 kD = vec3(1.0) - kS;
  kD *= 1.0 - metallic;

  vec3 nominator = NDF * G * F;
  float denominator = 4 * max(0.0, dot(N, Ve)) * max(0.0, dot(N, L)) + 0.00001;
  vec3 specular = nominator / denominator;

  // add to outgoing radiance Lo
  float NdotL = max(0.0, dot(N, L));
  vec3 diffuse_radiance = kD * (albedo) / PI;

  return (diffuse_radiance + specular) * radiance * NdotL;
}

vec3 ComputeLight( int Index )
{
  if (int(Lights[Index].ColorZ_Int_Type_IsEnabled.w + 0.01) < 1)
    return vec3(0, 0, 0);
  vec3 BaseColor = subpassLoad(InBaseColor).xyz;
  vec3 DrawPos = subpassLoad(InPos).xyz;
  vec3 DrawNormal = (subpassLoad(InNormal).xyz * 2.0) - 1.0;
  vec4 MetallicRoughness = subpassLoad(InMetallicRoughness).xyzw;
  vec3 L_Pos = Lights[Index].PosXYZ_DirX.xyz;
  vec3 L_Dir = L_Pos - DrawPos;
  vec3 V = normalize(CamLoc - DrawPos);
  float intensity = Lights[Index].ColorZ_Int_Type_IsEnabled.y;
  vec3 Color_light = clamp(vec3(Lights[Index].DirYZ_ColorXY.zw, Lights[Index].ColorZ_Int_Type_IsEnabled.x), vec3(0), vec3(1));
  vec3 F0 = vec3(0.04);
  // Get Proper F0 if material is not dielectric
  F0 = mix(F0, BaseColor, MetallicRoughness.r);  
  
  int type = int(Lights[Index].ColorZ_Int_Type_IsEnabled.z + 0.001);

  if (type == 0)
  {
    vec3 D = L_Pos - DrawPos;
    float D_2 = dot(D, D);
    intensity /= D_2;
    L_Dir = normalize(D);
  }
  else if (type == 1)
  {
    L_Dir = -normalize(vec3(Lights[Index].PosXYZ_DirX.w, Lights[Index].DirYZ_ColorXY.xy));
  }
  else if (type == 2)
  {
    vec3 D = L_Pos - DrawPos;
    float D_2 = dot(D, D);
    D = normalize(D);

    L_Dir = -normalize(vec3(Lights[Index].PosXYZ_DirX.w, Lights[Index].DirYZ_ColorXY.xy));
    float theta = dot(D, L_Dir);
    if (theta < Lights[Index].Exp_Angle_0_0.y)
      intensity = 0;
    else
      intensity *= pow(theta, Lights[Index].Exp_Angle_0_0.x);
    intensity /= D_2;
  }
  vec3 H = normalize(V + L_Dir);

  return PBR(normalize(DrawNormal), normalize(V), F0, BaseColor, normalize(L_Dir), normalize(H), Color_light, intensity, MetallicRoughness.r, MetallicRoughness.g);
}

vec3 ComputeSunDir( void )
{
  float delta = radians(23.44) * sin(2.0 * PI * (Size_timeOfDay_dayOfYear.z - 81.0) / 365.0);
  float H = (Size_timeOfDay_dayOfYear.y / 24.0) * 2.0 * PI - PI;

  float x = cos(delta) * sin(H);
  float y = sin(latitude) * sin(delta) + cos(latitude) * cos(delta) * cos(H);
  float z = cos(latitude) * sin(delta) - sin(latitude) * cos(delta) * cos(H);

  return normalize(vec3(x, y, z));
}

vec3 ComputeSun( void )
{
  if (int(Size_timeOfDay_dayOfYear.w + 0.001) < 1)
    return vec3(0, 0, 0);
  vec3 BaseColor = subpassLoad(InBaseColor).xyz;
  vec3 DrawPos = subpassLoad(InPos).xyz;
  vec3 DrawNormal = (subpassLoad(InNormal).xyz * 2.0) - 1.0;
  vec4 MetallicRoughness = subpassLoad(InMetallicRoughness).xyzw;

  float Wp = ProjSize, Hp = ProjSize;
  /* Correct aspect ratio */
  if (FrameW > FrameH)
    Wp *= FrameW / FrameH;
  else
    Hp *= FrameH / FrameW;
 
  float xp = Wp * (gl_FragCoord.x - FrameOffsetX) / FrameW - Wp / 2;
  float yp = Hp * (gl_FragCoord.y - FrameOffsetY) / FrameH - Hp / 2;
  vec3 Dir = CamDir * ProjDist + CamRight * xp - CamUp * yp;
  Dir = normalize(Dir);
  vec3 sunDir = ComputeSunDir();

  vec3 V = normalize(CamLoc - DrawPos);
  float intensity = 20.0;

  float sunVisible = smoothstep(-0.1, 0.01, sunDir.y);
  vec3 sunColor = vec3(1.0, 1.0, 0.5) * sunVisible;
  vec3 F0 = vec3(0.04);
  // Get Proper F0 if material is not dielectric
  F0 = mix(F0, BaseColor, MetallicRoughness.r);  
  
  vec3 H = normalize(V + sunDir);
  
//  return sunDir;
  return PBR(normalize(DrawNormal), normalize(V), F0, BaseColor, normalize(sunDir), normalize(H), sunColor, intensity, MetallicRoughness.r, MetallicRoughness.g);
}

vec3 Skybox( void )
{
  float Wp = ProjSize, Hp = ProjSize;
  /* Correct aspect ratio */
  if (FrameW > FrameH)
    Wp *= FrameW / FrameH;
  else
    Hp *= FrameH / FrameW;
 
  float xp = Wp * (gl_FragCoord.x - FrameOffsetX) / FrameW - Wp / 2;
  float yp = Hp * (gl_FragCoord.y - FrameOffsetY) / FrameH - Hp / 2;
  vec3 Dir = CamDir * ProjDist + CamRight * xp - CamUp * yp;
  Dir = normalize(Dir);
  vec3 sunDir = ComputeSunDir();

  float horizon = clamp(Dir.y * 0.5 + 0.5, 0.0, 1.0);

  vec3 skyTop = vec3(0.15, 0.35, 0.85);
  vec3 skyBottom = vec3(0.85, 0.92, 1.0);

  vec3 sky = mix(skyBottom, skyTop, horizon);

  float sunDot = max(dot(Dir, sunDir), 0.0);
  float sunDisk = pow(sunDot, 1300.0);
  float sunVisible = smoothstep(-0.1, 0.01, sunDir.y);
  sunDisk *= sunVisible;
  vec3 sunColor = vec3(1.0, 1.0, 0.5) * sunDisk * int(Size_timeOfDay_dayOfYear.w + 0.001);

  float sunset = clamp(1.0 - sunDir.y * 3.0, 0.0, 1.0);
  vec3 sunsetColor = vec3(1.0, 0.2, 0.1);

  sky = mix(sky, sunsetColor, sunset * (1.0 - horizon));
  sky = mix(vec3(0, 0, 0), sky, smoothstep(-0.1, -0.05, sunDir.y));
  vec3 color = sky + sunColor;

  return color;
}

/* The main fragment shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void main( void )
{
  vec3 color = vec3(0, 0, 0);
  
  if (!bool(subpassLoad(InNormal).w))
  {
    if (subpassLoad(InDepth).x < 1)
      OutColor = vec4(subpassLoad(InBaseColor).xyz, 0);
    else
      OutColor = vec4(Skybox(), 0);
    return;
  }
  
  color = ComputeSun();

  int i = 0;
  while (i < int(Size_timeOfDay_dayOfYear.x + 0.1))
  {
    color += ComputeLight(i);
    i++;
  }

  // HDR 
  color = color / (color + vec3(1.0));

  OutColor = vec4(color, 1);
} /* End of 'main' function */

/* END OF 'frag.glsl' FILE */
