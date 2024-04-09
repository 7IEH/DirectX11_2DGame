#ifndef _POSTPROCESSPS
#define _POSTPROCESSPS

#include "struct.fx"
#include "func.fx"

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

float4 PS_MOZAIC(VS_OUT _in) : SV_Target
{
    float4 _vColor = (float4) 0.f;
    float _fIntensity = 0.f;
    
    // 모자이크 
    if (0 == gMatrial._int3)
    {
        if ((gAccTime2 / 2.f) * 800 <= 800.f)
        {
            _fIntensity = 800 * (gAccTime2 / 2.f);
        }
        else
        {
            _fIntensity = 800.f;
        }
    }
    // 모자이크 지우기
    else
    {
        if (800 * (gAccTime2 / 2.f) <= 800.f)
        {
            _fIntensity = 800.f - 800 * (gAccTime2 / 2.f);
        }
        else
        {
            _fIntensity = 0.f;
        }
    }
   
    float intensity = floor(floor(1.f + floor(_fIntensity / 30.f) / 5.f) * 5.f);
    float2 _uv = floor(_in.vPosition.xy / intensity) * intensity / float2(1600.f, 900.f);
    
    _vColor = g_postprocess.Sample(samplerType2, _uv);
    return _vColor;
}

#endif