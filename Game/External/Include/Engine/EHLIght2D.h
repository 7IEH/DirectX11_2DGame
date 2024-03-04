#pragma once
#include "EHComponent.h"

class LIght2D
	:public Component
{
private:
	LightInfo		m_LightInfo;

public:
	LightInfo&			GetLightInfo()							{ return m_LightInfo; }
	void				SetLightInfo(LightInfo& _lightInfo)		{ m_LightInfo = _lightInfo; }

	void				SetColor(Vec4 _color)					{ m_LightInfo._Color = _color; }
	void				SetAmbient(Vec4 _ambient)				{ m_LightInfo._Ambient = _ambient; }
	void				SetSpecular(Vec4 _Specular)				{ m_LightInfo._Specular = _Specular; }
	void				SetDiffuse(Vec4 _diffuse)				{ m_LightInfo._Diffuse = _diffuse; }

	void				SetPostion(Vec3	_position)				{ m_LightInfo._Position = _position; }
	void				SetRadius(float _radius)				{ m_LightInfo._Radius = _radius; }

	void				SetLightDir(Vec3 _lighDir)				{ m_LightInfo._LightDir = _lighDir; }
	void				SetLightType(LIGHT_TYPE _type)			{ m_LightInfo._LightType = (int)_type; }

	// 빛의 세기가 거리에 따라 감소하는 방식을 제어하는 것
	void				SetAtt(Vec3 _att)						{ m_LightInfo._Att = _att; }
	void				SetSpot(float _spot)					{ m_LightInfo._Spot = _spot; }

	void				SetAngle(float _angle)					{ m_LightInfo._Angle = _angle; }

	Vec4				GetColor()								{ return m_LightInfo._Color; }
	Vec4				GetAmbient()							{ return m_LightInfo._Ambient; }
	Vec4				GetSpecular()							{ return m_LightInfo._Specular; }
	Vec4				GetDiffuse()							{ return m_LightInfo._Diffuse; }

	Vec3				GetPostion()							{ return m_LightInfo._Position; }
	float				GetRadius()								{ return m_LightInfo._Radius; }

	Vec3				GetLightDir()							{ return m_LightInfo._LightDir; }
	LIGHT_TYPE			GetLightType()							{ return (LIGHT_TYPE)m_LightInfo._LightType; }

	// 빛의 세기가 거리에 따라 감소하는 방식을 제어하는 것
	Vec3				GetAtt() { return m_LightInfo._Att; }
	float				GetSpot() { return m_LightInfo._Spot; }

	float				GetAngle() { return m_LightInfo._Angle; }

public:
	virtual void LateUpdate() override;
	
	virtual void Save(string _path)override;

	CLONE(LIght2D)
public:
	LIght2D();
	virtual ~LIght2D();
};

