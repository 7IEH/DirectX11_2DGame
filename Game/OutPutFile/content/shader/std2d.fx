#ifndef _STD2D
#define _STD2D

#include "struct.fx"
#include "func.fx"

struct VS_IN
{
    float4 vColor : COLOR;
    float3 vPos : POSITION; // Sementic
    //float3 vNomral : NORMAL;
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
    output.vWorld = mul(float4(_in.vPos, 1.f), World).xyz;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
         
    return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    float3 toEye = float3(0.f, 0.f, -0.1f);
     
    float4 ambient = float4(0.f, 0.f, 0.f, 0.f);
    float4 diffuse = float4(0.f, 0.f, 0.f, 0.f);
    float4 spec = float4(0.f, 0.f, 0.f, 0.f);
    
    float4 A, D, S;
    
    //ComputeDirectionalLight(gMatrial, gLight._DL, vNormal, toEye, A, D, S);
    //ambient += A;
    //diffuse += D;
    //spec += S;
    
    //ComputePointLight(gMatrial, gLight._PL, _in.vWorld, vNormal, toEye, A, D, S);
    //ambient += A;
    //diffuse += D;
    //spec += S;
      
    //ComputeSpotLight(gMatrial._LightMat, gLight._SL, _in.vWorld, vNormal, toEye, A, D, S);
    //ambient += A;
    //diffuse += D;
    //spec += S;
    
    float4 lightColor = ambient + diffuse + spec;
    float4 color = (float4) 0.f;
    
    if (gAnimUse == 1)
    {
        float2 animUv = _in.vUV * gSliceSize + gLeftTop + gOffsetSize;
        color = atlas_texture.Sample(samplerType2, animUv) + lightColor;
        
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
            for (int i = 0; i < 10;i++)
            {
                if(g_Light[i].LightType)
                {
                    
                }
                color = ST0.Sample(samplerType2, _in.vUV) * g_Light[0].Ambient;
            }
        }
    }
       
    return color;
}
#endif