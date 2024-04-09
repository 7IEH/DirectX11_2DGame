#ifndef _PLAYERPS
#define _PLAYERPS

#include "struct.fx"
#include "func.fx"

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float3 vWorld : POSITION;
    float2 vUV : TEXCOORD;
};

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    float4 color = (float4) 0.f;
    if (gAnimUse == 1)
    {
        float2 animUv = _in.vUV * gSliceSize + gLeftTop + gOffsetSize;
        color = atlas_texture.Sample(samplerType2, animUv);
        
        float2 vBackgroundLeftTop = gLeftTop + (gSliceSize / 2.f) - (gBackground / 2.f);
        vBackgroundLeftTop -= gOffsetSize;
        float2 vUV = vBackgroundLeftTop + (gBackground * _in.vUV);
    
        if (vUV.x < gLeftTop.x || (gLeftTop.x + gSliceSize.x) < vUV.x
            || vUV.y < gLeftTop.y || (gLeftTop.y + gSliceSize.y) < vUV.y)
        {
            //vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
        else
        {
            if (1 == gMatrial._int2)
            {
                if (1 == gMatrial._int1)
                {
                    if(color.a>=0.2f)
                        color = gMatrial._LightMat.Specular;
                }
            }
        }
    }
    else
    {
        float4 _lightcolor = (float4) 0.f;
        if (gMatrial.spriteCheck0 == 1)
        {
            color = ST0.Sample(samplerType2, _in.vUV);
            
            if (gMatrial._int3 == 1 && color.a != 0)
            {
                float4 pixelUp = ST0.Sample(samplerType2, _in.vUV + float2(0, 0.02f));
                float4 pixelDown = ST0.Sample(samplerType2, _in.vUV - float2(0, 0.02f));
                float4 pixelRight = ST0.Sample(samplerType2, _in.vUV + float2(0.02f, 0));
                float4 pixelLeft = ST0.Sample(samplerType2, _in.vUV - float2(0.02f, 0));
                
                if (pixelUp.a <= 0.1f || pixelDown.a <= 0.1f || pixelRight.a <= 0.1f || pixelLeft.a <= 0.1f)
                {
                    color = float4(1.f, 0.f, 0.f, 1.f);
                }
            }
        }
    }
    
    LightInfo lightColor = (LightInfo) 0.f;
    
    for (int i = 0; i < gLight2DSize; i++)
    {
        ComputeLight2D(_in.vWorld, i, lightColor);
    }

    color.rgb *= (lightColor.Ambient.rgb + lightColor.Color.rgb);
    color *= gMatrial._LightMat.Color;
    
    if (0.f == color.a)
        discard;
    
    return color;
}
#endif