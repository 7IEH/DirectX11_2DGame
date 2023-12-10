#include "pch.h"
#include "EHGraphicShader.h"

#include "EHDevice.h"
#include "EHPathMgr.h"

GraphicShader::GraphicShader()
	: m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
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

	// Texture Create
	CreateSamplerState();

	// CreateLayOut
	CreateLayOut();

	// Create BlendState
	CreateBlendState();
}

void GraphicShader::CreateResourceView(wstring& _texturePath)
{
	wstring _path = PATH + _texturePath;
	ScratchImage _Image;
	LoadFromWICFile(_path.c_str(), WIC_FLAGS_NONE, NULL, _Image);
	ComPtr<ID3D11Texture2D> _texture;
	CreateTexture(DEVICE, _Image.GetImages(), _Image.GetImageCount(), _Image.GetMetadata(), (ID3D11Resource**)_texture.GetAddressOf());

	// 溅捞歹 府家胶 轰 积己.
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	::ZeroMemory(&srvDesc, sizeof(srvDesc));

	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	D3D11_TEXTURE2D_DESC textureDesc;
	_texture->GetDesc(&textureDesc);

	srvDesc.Format = textureDesc.Format;

	ID3D11ShaderResourceView* shaderResourceView;
	DEVICE->CreateShaderResourceView(
		_texture.Get(),
		&srvDesc,
		m_ResourceView.GetAddressOf()
	);

}

void GraphicShader::UpdateData()
{
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CONTEXT->IASetInputLayout(m_LayOut.Get());
	SetShader(SHADER_TYPE::VERTEX);
	SetShader(SHADER_TYPE::PIXEL);
	CONTEXT->PSSetShaderResources(0, 1, m_ResourceView.GetAddressOf());
	CONTEXT->PSSetSamplers(0, 1, m_SamplerState.GetAddressOf());
	CONTEXT->OMSetBlendState(m_BlendState.Get(), NULL, 0x3FF);
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

void GraphicShader::CreateSamplerState()
{
	D3D11_SAMPLER_DESC tDesc = {};
	tDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.MipLODBias = 0.0f;
	tDesc.MaxAnisotropy = 1;
	tDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	tDesc.BorderColor[0] = 0;
	tDesc.BorderColor[1] = 0;
	tDesc.BorderColor[2] = 0;
	tDesc.BorderColor[3] = 0;
	tDesc.MinLOD = 0;
	tDesc.MaxLOD = D3D11_FLOAT32_MAX;

	DEVICE->CreateSamplerState(&tDesc, m_SamplerState.GetAddressOf());
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

void GraphicShader::CreateBlendState()
{
	D3D11_BLEND_DESC tDesc = {};
	tDesc.RenderTarget[0].BlendEnable = true;
	tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	DEVICE->CreateBlendState(&tDesc, m_BlendState.GetAddressOf());
}