#include "pch.h"
#include "EHLIght2D.h"

#include "EHDevice.h"
#include "EHConstantBuffer.h"

#include "EHRenderMgr.h"

#include "EHGameObject.h"

LIght2D::LIght2D()
	:Component(COMPONENT_TYPE::LIGHT2D)
{
}

LIght2D::~LIght2D()
{
}

void LIght2D::LateUpdate()
{
	GameObject* _light = GetOwner();
	Transform* _lightTr = _light->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	m_LightInfo._Position = Vec3(_lightTr->GetRelativePosition());
	m_LightInfo._LightDir = _lightTr->GetWorldDir(DIRECTION_TYPE::RIGHT);

	RenderMgr::GetInst()->RegisterLight2D(GetOwner());
}

void LIght2D::Save(string _path)
{
	// 1. LightType 2. LightColor 3. LightAmbient 4. LightAngle 5. LightRadius
	std::ofstream _file(_path.data(), std::fstream::out | std::fstream::app);

	int _lightType = m_LightInfo._LightType;
	Vec4 _lightColor = m_LightInfo._Color;
	Vec4 _lightAmbient = m_LightInfo._Ambient;
	float _angle = m_LightInfo._Angle;
	float _radius = m_LightInfo._Radius;

	_file << "LIGHT2D\n";
	_file << std::to_string(_lightType) + '\n';
	_file << EH::WriteVector4(_lightColor) + '\n';
	_file << EH::WriteVector4(_lightAmbient) + '\n';
	_file << std::to_string(_angle) + '\n';
	_file << std::to_string(_radius) + '\n';

	_file.close();
}

void LIght2D::Load(std::wifstream* _file)
{
	wstring _line = L"";

	for (int i = 0;i < 5;i++)
	{
		std::getline(*_file, _line);
		if (i == 0)
		{
			m_LightInfo._LightType = stoi(_line);
		}
		else if (i == 1)
		{
			EH::InputVector4(_line, m_LightInfo._Color);
		}
		else if (i == 2)
		{
			EH::InputVector4(_line, m_LightInfo._Ambient);
		}
		else if (i == 3)
		{
			m_LightInfo._Angle = stof(_line);
		}
		else
		{
			m_LightInfo._Radius = stof(_line);
		}
	}
}
