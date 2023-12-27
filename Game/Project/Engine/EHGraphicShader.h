#pragma once
#include "EHShader.h"

class GraphicShader
    :public Shader
{
private:
    ComPtr<ID3DBlob>                        m_VBlob;
    ComPtr<ID3DBlob>                        m_PBlob;
    ComPtr<ID3DBlob>                        m_HBlob;
    ComPtr<ID3DBlob>                        m_DBlob;
    ComPtr<ID3DBlob>                        m_GBlob;

    ComPtr<ID3DBlob>                        m_ErrBlob;

    ComPtr<ID3D11VertexShader>              m_VS;
    ComPtr<ID3D11PixelShader>               m_PS;
    ComPtr<ID3D11HullShader>                m_HS;
    ComPtr<ID3D11DomainShader>              m_DS;
    ComPtr<ID3D11GeometryShader>            m_GS;

    ComPtr<ID3D11InputLayout>               m_LayOut;
    D3D_PRIMITIVE_TOPOLOGY                  m_Topology;

    CULL_TYPE                               m_CullType;
    DS_TYPE                                 m_DSType;
    BLEND_TYPE                              m_BlendType;
    SAMPLER_TYPE                            m_SamplerType;
public:
    GraphicShader();
    virtual ~GraphicShader();

public:
    void SetTopology(D3D_PRIMITIVE_TOPOLOGY _Topology) { m_Topology = _Topology; }

    void SetCullType(CULL_TYPE _type) { m_CullType = _type; }
    void SetDSType(DS_TYPE _type) { m_DSType = _type; }
    void SetBlendType(BLEND_TYPE _type) { m_BlendType = _type; }

public:
    void Create(wstring& _shaderPath, string& _vsEntry, string& _psEntry);

    virtual void UpdateData()override;
    virtual void Render() override;

private:
    void CreateBlobFile(SHADER_TYPE _type, wstring& _path, string& _entry);
    void CreateShader(SHADER_TYPE _type);
    void CreateLayOut();
    void SetShader(SHADER_TYPE _type);
};

