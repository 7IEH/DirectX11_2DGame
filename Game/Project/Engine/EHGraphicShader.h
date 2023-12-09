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

    ComPtr<ID3D11SamplerState>              m_SamplerState;
    ComPtr<ID3D11ShaderResourceView>        m_ResourceView;

public:
    GraphicShader();
    virtual ~GraphicShader();

public:
    void SetTopology(D3D_PRIMITIVE_TOPOLOGY _Topology) { m_Topology = _Topology; }

public:
    void Create(wstring& _shaderPath, string& _vsEntry, string& _psEntry);
    void CreateResourceView(wstring& _texturePath);

    virtual void UpdateData()override;
    virtual void Render() override;

private:
    void CreateBlobFile(SHADER_TYPE _type, wstring& _path, string& _entry);
    void CreateShader(SHADER_TYPE _type);
    void CreateSamplerState();
    void CreateLayOut();
    void SetShader(SHADER_TYPE _type);
};
