//
//
#ifndef MESH_H
#define MESH_H

struct VertMeshStaticInstanced
{
    float3 Position     : POSITION0;
    float3 Normal       : NORMAL;
    float2 TexCoord0    : TEXCOORD0;
    float2 TexCoord1    : TEXCOORD1;
    float3 Tangent      : TANGENT;
    float3 BiTangent    : BINORMAL;
    uint   InstIndex    : TEXCOORD2;
};


struct VertMeshDynamicInstanced
{
    float3 Position     : POSITION0;
    float3 Normal       : NORMAL;
    float2 TexCoord0    : TEXCOORD0;
    float2 TexCoord1    : TEXCOORD1;
    float3 Tangent      : TANGENT;
    float3 BiTangent    : BINORMAL;
    uint4  BoneIndices  : BLENDINDICES0;
    float4 BoneWeight   : BLENDWEIGHT0;
    uint   InstIndex    : TEXCOORD2;
};


struct VertStaticPositionInstanced
{
    float3 Position     : POSITION0;
    uint   InstIndex    : TEXCOORD2;
    float2 TexCoord0    : TEXCOORD0;
    float2 TexCoord1    : TEXCOORD1;
};


struct MeshData
{
    // World To Clip space matrix.
    float4x4    WorldToClip;
    float4x4    N;
};


struct PerInstancedMeshData
{
    float3 Position;
    float3 Scale;
    uint   MaterialId;
    float  Pad1;
    float4 Rotation;
};


StructuredBuffer<MeshData> PerInstanceMeshData;

#endif // MESH_H