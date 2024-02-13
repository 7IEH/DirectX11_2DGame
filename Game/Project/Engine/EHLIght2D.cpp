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
	std::ofstream _file(_path.data(), std::fstream::out | std::fstream::app);

	int _lightType = m_LightInfo._LightType;
	Vec4 _lightColor = m_LightInfo._Color;
	Vec4 _lightAmbient = m_LightInfo._Ambient;
	float _angle = m_LightInfo._Angle;

	string _temp = "";

	_temp = "LIGHT2D\n";

	_file.write(_temp.c_str(), _temp.size());
	_file << std::to_string(_lightType) + '\n';
	_temp.clear();
	EH::WriteVector4(_lightColor, _temp);
	_file << _temp + '\n';
	_temp.clear();
	EH::WriteVector4(_lightAmbient, _temp);
	_file << _temp + '\n';
	_temp.clear();
	_file << std::to_string(_angle) + '\n';

	_file.close();
}
