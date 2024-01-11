#include "pch.h"
#include "EHGraphicShader.h"

#include "EHDevice.h"
#include "EHPathMgr.h"

#include "EHSprite.h"

GraphicShader::GraphicShader()
	: m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	,m_CullType(CULL_TYPE::BACK)
	,m_DSType(DS_TYPE::LESS)
	,m_BlendType(BLEND_TYPE::DEFAULT)
	,m_SamplerType(SAMPLER_TYPE::POINT)
	,m_Domain(SHADER_DOMAIN::DOMAIN_MASKED)
{
}

GraphicShader::~GraphicShader()
{
}

void GraphicShader::Create(wstring& _shaderPath, string& _vsEntry, string& _psEntry)
{
	// CreateBlob
	CreateBlobFile(SHADER_TYPE::VERTEX, _shaderPath, _vsEntry);
	CreateBlobFile(SHADER_TYPE::PIXEL, _shaderPath, _psEntry);

	// CreateShader
	CreateShader(SHADER_TYPE::VERTEX);
	CreateShader(SHADER_TYPE::PIXEL);

	// CreateLayOut
	CreateLayOut();
}

void GraphicShader::UpdateData()
{
	/************************
	Basic Rendering Pipeline
	************************/
	// Input Assembler Stage
	CONTEXT->IASetPrimitiveTopology(m_Topology);
	CONTEXT->IASetInputLayout(m_LayOut.Get());
	
	// Vertex Shader Stage
	SetShader(SHADER_TYPE::VERTEX);

	// Rasterizer Stage
	CONTEXT->RSSetState(Device::GetInst()->GetRSState(m_CullType).Get());

	// Pixel Shader Stage
	SetShader(SHADER_TYPE::PIXEL);

	// Output-Merger Stage
	CONTEXT->OMSetDepthStencilState(Device::GetInst()->GetDSState(m_DSType).Get(),0);
	CONTEXT->OMSetBlendState(Device::GetInst()->GetBSState(m_BlendType).Get(),nullptr,0xffffffff);
}
 
void GraphicShader::Render()
{
	
}

void GraphicShader::CreateBlobFile(SHADER_TYPE _type, wstring& _path, string& _entry)
{
	wstring _finalPath = PATH + _path;
	switch (_type)
	{
	case SHADER_TYPE::VERTEX:
	{
		D3DCompileFromFile(_finalPath.c_str()
			, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _entry.c_str(), "vs_5_0", D3DCOMPILE_DEBUG, 0
			, m_VBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());
	}
	break;
	case SHADER_TYPE::PIXEL:
	{
		D3DCompileFromFile(_finalPath.c_str()
			, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _entry.c_str(), "ps_5_0", D3DCOMPILE_DEBUG, 0
			, m_PBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());
	}
	break;
	case SHADER_TYPE::HULL:
	{
		//D3DCompileFromFile
	}
	break;
	case SHADER_TYPE::DOMAlN:
	{
		//D3DCompileFromFile
	}
	break;
	case SHADER_TYPE::GEOMETRY:
	{
		//D3DCompileFromFile();
	}
	break;
	case SHADER_TYPE::END:
		break;
	default:
		break;
	}

	if (nullptr != m_ErrBlob)
	{
		char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
	}
}

void GraphicShader::CreateShader(SHADER_TYPE _type)
{
	switch (_type)
	{
	case SHADER_TYPE::VERTEX:
		DEVICE->CreateVertexShader(m_VBlob->GetBufferPointer(),
			m_VBlob->GetBufferSize(), nullptr, m_VS.GetAddressOf());
		break;
	case SHADER_TYPE::PIXEL:
		DEVICE->CreatePixelShader(m_PBlob->GetBufferPointer(),
			m_PBlob->GetBufferSize(), nullptr, m_PS.GetAddressOf());
		break;
	case SHADER_TYPE::HULL:
		break;
	case SHADER_TYPE::DOMAlN:
		break;
	case SHADER_TYPE::GEOMETRY:
		break;
	case SHADER_TYPE::END:
		break;
	default:
		break;
	}
}

void GraphicShader::CreateLayOut()
{
	D3D11_INPUT_ELEMENT_DESC arrElement[3] = {};

	arrElement[0].InputSlot = 0;
	arrElement[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[0].SemanticName = "POSITION";
	arrElement[0].SemanticIndex = 0;
	arrElement[0].InstanceDataStepRate = 0;
	arrElement[0].AlignedByteOffset = 0;
	arrElement[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	arrElement[1].InputSlot = 0;
	arrElement[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[1].SemanticName = "COLOR";
	arrElement[1].SemanticIndex = 0;
	arrElement[1].InstanceDataStepRate = 0;
	arrElement[1].AlignedByteOffset = 12;
	arrElement[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	arrElement[2].InputSlot = 0;
	arrElement[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[2].SemanticName = "TEXCOORD";
	arrElement[2].SemanticIndex = 0;
	arrElement[2].InstanceDataStepRate = 0;
	arrElement[2].AlignedByteOffset = 28;
	arrElement[2].Format = DXGI_FORMAT_R32G32_FLOAT;

	DEVICE->CreateInputLayout(arrElement, 3, m_VBlob->GetBufferPointer(), m_VBlob->GetBufferSize(), m_LayOut.GetAddressOf());
}

void GraphicShader::SetShader(SHADER_TYPE _type)
{
	switch (_type)
	{
	case SHADER_TYPE::VERTEX:
	{
		CONTEXT->VSSetShader(m_VS.Get(), 0, 0);
	}
	break;
	case SHADER_TYPE::PIXEL:
	{
		CONTEXT->PSSetShader(m_PS.Get(), 0, 0);
	}
	break;
	case SHADER_TYPE::HULL:
		break;
	case SHADER_TYPE::DOMAlN:
		break;
	case SHADER_TYPE::GEOMETRY:
		break;
	case SHADER_TYPE::END:
		break;
	default:
		break;
	}
}