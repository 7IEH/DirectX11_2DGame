#pragma once
#include "EHShader.h"

#include "EHStructuredBuffer.h"
#include "EHSprite.h"

class ComputeShader :
	public Shader
{
public:
	ComPtr<ID3DBlob>            m_CSBlob;
	ComPtr<ID3D11ComputeShader> m_CS;

	UINT                        m_GroupX;
	UINT                        m_GroupY;
	UINT                        m_GroupZ;

protected:
	const UINT                  m_ThreadX;
	const UINT                  m_ThreadY;
	const UINT                  m_ThreadZ;

	material                    m_Const;

public:
	HRESULT Create(const wstring& _strRelativePath, const string& _strFuncName);

public:
	void Execute();

protected:
	UINT GetGroupX() { return m_GroupX; }
	UINT GetGroupY() { return m_GroupY; }
	UINT GetGroupZ() { return m_GroupZ; }

	void SetGroupX(UINT _group) { m_GroupX = _group; }
	void SetGroupY(UINT _group) { m_GroupY = _group; }
	void SetGroupZ(UINT _group) { m_GroupZ = _group; }

private:
	virtual HRESULT UpdateData() = 0;
	virtual void UpdateGroupCount() = 0;
	virtual void Clear() = 0;

public:
	ComputeShader(UINT _threadX, UINT _threadY, UINT _threadZ);
	virtual ~ComputeShader();
};

