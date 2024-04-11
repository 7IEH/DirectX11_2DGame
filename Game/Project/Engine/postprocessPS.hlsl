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
    
    // Mozaic
    if (1 == gMatrial._int0)
    {
        // 모자이크 
        if (1 == gMatrial._int2)
        {
            if ((gAccTime2 / 2.f) * 1600 <= 1600.f)
            {
                _fIntensity = 1600.f * (gAccTime2 / 2.f);
            }
            else
            {
                _fIntensity = 1600.f;
            }
        }
    // 모자이크 지우기
        else
        {
            if (1600 * (gAccTime2 / 2.f) <= 1600.f)
            {
                _fIntensity = 1600.f - 1600 * (gAccTime2 / 2.f);
            }
            else
            {
                _fIntensity = 0.f;
            }
        }
        
        float intensity = floor(floor(1.f + floor(_fIntensity / 30.f) / 5.f) * 5.f);
        float2 _uv = floor(_in.vPosition.xy / intensity) * intensity / float2(1600.f, 900.f);
    
        _vColor = g_postprocess.Sample(samplerType2, _uv);
    }
    else
    {
        _vColor = g_postprocess.Sample(samplerType2, _in.vUV);
    }
    
    // 영역 바깥 크기
    float _fBorder = 0.1f;
    
    // 밝기
    float _fBright = 20.f;
   
    // Vignette
    if (1 == gMatrial._int1)
    {
        _in.vUV *= float2(1.f, 1.f) - _in.vUV.yx;
    
        float vig = _in.vUV.x * _in.vUV.y * _fBright;
   
        vig = pow(vig, _fBorder);
       
        _vColor *= vig;
    }
    
    return _vColor;
}

#endif