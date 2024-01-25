#include "pch.h"
#include "EHComputeShader.h"

#include "EHDevice.h"
#include "EHConstantBuffer.h"

#include "EHPathMgr.h"

ComputeShader::ComputeShader(UINT _threadX,UINT _threadY,UINT _threadZ)
	:Shader(ASSET_TYPE::COMPUTE_SHADER)
	, m_GroupX(1)
	, m_GroupY(1)
	, m_GroupZ(1)
	, m_ThreadX(_threadX)
	, m_ThreadY(_threadY)
	, m_ThreadZ(_threadZ)
{
}

ComputeShader::~ComputeShader()
{
}

HRESULT ComputeShader::Create(const wstring& _strRelativePath, const string& _strFuncName)
{
	wstring _absolutePath = PATH;
	wstring _strFilePath = _absolutePath + _strRelativePath;

	// Pixel Shader Create
	// Pixel Shader Compile
	if (FAILED(D3DCompileFromFile(_strFilePath.c_str()
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "cs_5_0", D3DCOMPILE_DEBUG, 0
		, m_CSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
		}
		else
		{
			HandleError(MAIN_HWND, L"Shader File Not Exist", 0);
		}

		return E_FAIL;
	}

	DEVICE->CreateComputeShader(m_CSBlob->GetBufferPointer()
		, m_CSBlob->GetBufferSize(), nullptr
		, m_CS.GetAddressOf());

	return S_OK;
}

void ComputeShader::Execute()
{
	if (FAILED(UpdateData()))
		return;
	
	static ConstantBuffer* _CB = Device::GetInst()->GetConstantBuffer(CONSTANT_TYPE::MATERIAL);
	_CB->SetData(&m_Const,sizeof(m_Const),1);
	_CB->UpdateData_CS();

	CONTEXT->CSSetShader(m_CS.Get(), 0, 0);
	CONTEXT->Dispatch(m_GroupX, m_GroupY, m_GroupZ);

	Clear();
}