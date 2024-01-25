#ifndef _SETCOLOR
#define _SETCOLOR

#include "struct.fx"

#define TEX_WIDTH  gMatrial._int0
#define TEX_HEIGHT gMatrial._int1

RWTexture2D<float4> g_TargetTex : register(u0);

[numthreads(32,32,1)]
void CS_SetColor(uint3 id :SV_DispatchThreadID)
{
    if (TEX_WIDTH <= id.x || TEX_HEIGHT <= id.y)
    {
        return;
    }
    
    if (5 == id.x && 1 == id.y)
    {
        g_TargetTex[id.xy] = float4(0.f, 1.f, 1.f, 1.f);
    }
    else
    {
        g_TargetTex[id.xy] = float4(gMatrial._LightMat.Ambient.rgb, 1.f);
    }
}


#endif