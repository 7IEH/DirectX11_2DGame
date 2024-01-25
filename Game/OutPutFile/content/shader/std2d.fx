#ifndef _STD2D
#define _STD2D

#include "struct.fx"
#include "func.fx"

struct VS_IN
{
    float4 vColor : COLOR;
    float3 vPos : POSITION; // Sementic
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float3 vWorld : POSITION;
    float2 vUV : TEXCOORD;
};

RasterizerState WireframeRS
{
    FillMode = Wireframe;
    CullMode = BACK;
    FrontCounterClockwise = false;
};

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), WVP);
    float4 _world = mul(float4(_in.vPos.x, _in.vPos.y, _in.vPos.z, 1.f), World);
    output.vWorld = _world.xyz;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;

    return output;
}

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
            //color = atlas_texture.Sample(samplerType2, vUV);
           
            //if (gMatrial._int3 == 1 && color.a != 0)
            //{
            //    float4 pixelUp = ST0.Sample(samplerType2, _in.vUV + float2(0, 0.02f));
            //    float4 pixelDown = ST0.Sample(samplerType2, _in.vUV - float2(0, 0.02f));
            //    float4 pixelRight = ST0.Sample(samplerType2, _in.vUV + float2(0.02f, 0));
            //    float4 pixelLeft = ST0.Sample(samplerType2, _in.vUV - float2(0.02f, 0));
                
            //    if (pixelUp.a <= 0.1f || pixelDown.a <= 0.1f || pixelRight.a <= 0.1f || pixelLeft.a <= 0.1f)
            //    {
            //        color = float4(1.f, 0.f, 0.f, 1.f);
            //    }
            //}
        }
        
    }
    else
    {
        float4 _lightcolor = (float4) 0.f;
        if (gMatrial.spriteCheck0 == 1)
        {
            color = ST0.Sample(samplerType2, _in.vUV);
            
            if(gMatrial._int3 == 1 && color.a != 0)
            {
                float4 pixelUp = ST0.Sample(samplerType2, _in.vUV + float2(0, 0.02f));
                float4 pixelDown = ST0.Sample(samplerType2, _in.vUV - float2(0, 0.02f));
                float4 pixelRight = ST0.Sample(samplerType2, _in.vUV + float2(0.02f, 0));
                float4 pixelLeft = ST0.Sample(samplerType2, _in.vUV - float2(0.02f, 0));
                
                if (pixelUp.a <= 0.1f || pixelDown.a<=0.1f || pixelRight.a <=0.1f || pixelLeft.a<=0.1f)
                {
                    color = float4(1.f, 0.f, 0.f, 1.f);
                }
            }
        }
    }
    
    LightInfo lightColor = (LightInfo) 0.f;
    
    for (int i = 0; i < gLight2DSize;i++)
    {
        ComputeLight2D(_in.vWorld, i, lightColor);
    }

    color.rgb *= (lightColor.Ambient.rgb + lightColor.Color.rgb);
    
    if(0.f == color.a)
        discard;
    
    return color;
}

float4 PS_Std2D_Effect(VS_OUT _in) : SV_Target
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
            color = atlas_texture.Sample(samplerType2, vUV);
        }
        
    }
    else
    {
        float4 _lightcolor = (float4) 0.f;
        if (gMatrial.spriteCheck0 == 1)
        {
            color = ST0.Sample(samplerType2, _in.vUV);
        }
    }
    
    LightInfo lightColor = (LightInfo) 0.f;
    
    for (int i = 0; i < gLight2DSize; i++)
    {
        ComputeLight2D(_in.vWorld, i, lightColor);
    }

    color.rgb *= (lightColor.Ambient.rgb + lightColor.Color.rgb);
     
    return color;
}


#endif