#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "struct.fx"
#include "func.fx"

float mask[9] =
{
    -1, -1, -1,
    -1, 8, -1,
    -1, -1, -1
};

int coord[3] = { -1, 0, 1 };

float divider = 1;

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

VS_OUT VS_GrayFilter(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_GrayFilter(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    vColor = float4(0.3f, 0.3f, 0.3f, 1.f);
    
    return vColor;
}

VS_OUT VS_OutLineFilter(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), WVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_OutLineFilter(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    float2 ScreenUV = _in.vPosition.xy / gRenderResolution;
      
    float2 cp = -1.0 + 2.0 * _in.vUV;
    float cl = length(cp);
    float2 uv = ScreenUV;
    if (cl > gMatrial._float0 * 0.2f && cl < gMatrial._float0 * 0.4f)
    {
        uv = ScreenUV + (cp / cl) * cos(cl * 10.0 - gMatrial._float0 * 13.0) * 0.02f;
    }
        
    vColor = g_postprocess.Sample(samplerType2, uv);
    
    return vColor;
}

VS_OUT VS_DistortionFilter(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), WVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_DistortionFilter(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    float2 ScreenUV = _in.vPosition.xy / gRenderResolution;

    if (gMatrial.spriteCheck0)
    {
        float4 vDiscard = ST0.Sample(samplerType2, _in.vUV);
        if (vDiscard.a <= 0.1f)
        {
            discard;
        }
    }
    
    if (gMatrial.spriteCheck1)
    {
        float2 vUv = _in.vUV;
        vUv += gAccTime * 0.1f;
        
        float4 temp = ST1.Sample(samplerType2, vUv);
        float2 color = temp.xy;
        color = (color.xy - 0.5f) * 0.1f;
        
        ScreenUV += color;
    }
    
    // 반전 효과
    ScreenUV.y *= -1.f;
   
    vColor = g_postprocess.Sample(samplerType2, ScreenUV) * float4(0.8f, 0.8f, 1.f, 0.8f);
    
    return vColor;
}


#endif