#include "pch.h"
#include "EHTestComputeShader.h"

TestComputeShader::TestComputeShader()
	:ComputeShader(32, 32, 1)
{
	Create(L"\\shader\\setcolor.fx", "CS_SetColor");
}

TestComputeShader::~TestComputeShader()
{
}

HRESULT TestComputeShader::UpdateData()
{
	if (nullptr == m_TargetTex)
		return E_FAIL;

	// ��� ����
	m_Const.iArr[0] = m_TargetTex->GetSpriteWidth();
	m_Const.iArr[1] = m_TargetTex->GetSpriteHeight();

	m_Const.vLightMat.Ambient = m_Color;

	// �ؽ��� U0�� ���ε�
	if (FAILED(m_TargetTex->UpdateData_CS_UAV(0)))
	{
		return E_FAIL;
	}

	// ������ �׷� �� üũ

	UpdateGroupCount();

	return S_OK;
}

void TestComputeShader::UpdateGroupCount()
{
	UINT _width = m_TargetTex->GetSpriteWidth();
	UINT _height = m_TargetTex->GetSpriteHeight();

	SetGroupX(1 + _width / m_ThreadX);
	SetGroupY(1 + _height / m_ThreadY);
	SetGroupZ(1);
}

void TestComputeShader::Clear()
{
	m_TargetTex->Clear_CS_UAV();
}