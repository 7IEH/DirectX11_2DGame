#ifndef _ST2Dbg
#define _ST2Dbg

SamplerState samplerType;

Texture2D shaderTexture;

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
	float2 vUV : TEXCOORD;
};

VS_OUT VS_Std2D(VS_IN _in)
{
	VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), WVP);
	output.vColor = _in.vColor;
	output.vUV = _in.vUV;
    
	return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
	//float4 color = shaderTexture.Sample(samplerType, _in.vUV);
	return _in.vColor;
}

#endif