#ifndef _STRUCT
#define _STRUCT

Texture2D ST0 : register(t0);
Texture2D ST1 : register(t1);
Texture2D ST2 : register(t2);
Texture2D ST3 : register(t3);
Texture2D ST4 : register(t4);
Texture2D ST5 : register(t5);

Texture2D CUBE6 : register(t6);
Texture2D CUBE7 : register(t7);

Texture2D STARR8 : register(t8);
Texture2D STARR9 : register(t9);

SamplerState samplerType : register(s0);
SamplerState samplerType2 : register(s1);

// Animation Texture
Texture2D atlas_texture : register(t10);

// Structed Register
StructuredBuffer<float> g_Light : register(t14);

// Luna Light Example
struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
    float pad;
};

struct PointLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Position;
    float Range;
    float3 Att;
    float pad;
};

struct SpotLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Position;
    float Range;
    float3 Direction;
    float Spot;
    float3 Att;
    float pad;
};

struct tLight
{
    DirectionalLight _DL;
    PointLight _PL;
    SpotLight _SL;
};

struct LightMaterial
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Reflect;
};

struct Material
{
    int _int0;
    int _int1;
    int _int2;
    int _int3;
    
    float _float0;
    float _float1;
    float _float2;
    float _float3;
    
    float2 _vec0;
    float2 _vec1;
    float2 _vec2;
    float2 _vec3;
   
    LightMaterial _LightMat;
    
    matrix _matrix0;
    matrix _matrix1;
    matrix _matrix2;
    matrix _matrix3;
    
    int spriteCheck0;
    int spriteCheck1;
    int spriteCheck2;
    int spriteCheck3;
    int spriteCheck4;
    int spriteCheck5;

    int TEXCUBE_0;
    int TEXCUBE_1;

    int TEXARR_0;
    int TEXARR_1;
    
    int2 padding;
};

cbuffer Worldspcae : register(b0)
{
    matrix World;
    matrix matWorldInv;
    
    matrix View;
    matrix matViewInv;
    
    matrix Projection;
    matrix matProjInv;
    
    matrix WV;
    matrix WVP;
};

cbuffer Material : register(b1)
{
    Material gMatrial;
}

cbuffer Light : register(b2)
{
    tLight gLight;
};

cbuffer Normal : register(b3)
{
    float3 vNormal;
    float pad1;
}

cbuffer tAnimationInfo : register(b4)
{
    float2  gOffsetSize;
    float2  gLeftTop;
    float2  gSliceSize;
    float2  gBackground;
    int     gAnimUse;
    float3  gPadding;
};

struct VS_TEST
{
    float4 t_ambient;
    float4 t_diffuse;
    float4 t_spec;
};

#endif