#ifndef _TILEMAP
#define _TILEMAP

#include "struct.fx"

// ==============
// TileMap Shader

// Parameter
#define FACE_X gMatrial._int0
#define FACE_Y gMatrial._int1

#define vSliceUV gMatrial._vec0

#define TileAtlas ST0

StructuredBuffer<tTileInfo> g_TileInfo : register(t20);

// ==============
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_TileMap(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), WVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_TileMap(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (gMatrial.spriteCheck0)
    {
        // 면 개수만큼 _in.vUV 를 배율을 늘림
        float2 vUV = _in.vUV * float2(FACE_X, FACE_Y);
        int2 Integer = (int2) floor(vUV);
        vUV = vUV - Integer;
                
        int bufferidx = Integer.y * FACE_X + Integer.x;
        
        if (!g_TileInfo[bufferidx].bRender)
            discard;
        
        vUV = g_TileInfo[bufferidx].vLeftTopUV + (vSliceUV * vUV);
        vColor = TileAtlas.Sample(samplerType2, vUV);
    }
    
    return vColor;
}

#endif