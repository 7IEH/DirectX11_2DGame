#include "pch.h"
#include "EHComponentUI.h"

#include "EHGameObject.h"

ComponentUI::ComponentUI(const string& _label, const string& _strID, COMPONENT_TYPE _type)
	:UI(_label,_strID)
	,m_TargetObject(nullptr)
	,m_Type(_type)
{
}

ComponentUI::~ComponentUI()
{
}

void ComponentUI::Render_Update()
{
}

void ComponentUI::SetTargetObject(GameObject* _target)
{
	m_TargetObject = _target;

	if (nullptr == m_TargetObject)
	{
		Enabled(FALSE);
	}
	else
	{
		switch (m_Type)
		{
		case COMPONENT_TYPE::TRANSFORM:
		{
			Transform* _tr = m_TargetObject->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);

			if (_tr == nullptr)
				Enabled(FALSE);
			else
				Enabled(TRUE);
		}
			break;
		case COMPONENT_TYPE::LIGHT2D:
		{
			LIght2D* _light2d = m_TargetObject->GetComponent<LIght2D>(COMPONENT_TYPE::LIGHT2D);

			if (_light2d == nullptr)
				Enabled(FALSE);
			else
				Enabled(TRUE);
		}
			break;
		case COMPONENT_TYPE::COLLIDER2D:
		{
			Collider2D* _col2d = m_TargetObject->GetComponent<Collider2D>(COMPONENT_TYPE::COLLIDER2D);

			if (_col2d == nullptr)
				Enabled(FALSE);
			else
				Enabled(TRUE);
		}
			break;
		case COMPONENT_TYPE::ANIMATOR2D:
		{
			Animator2D* _anim2d = m_TargetObject->GetComponent<Animator2D>(COMPONENT_TYPE::ANIMATOR2D);

			if (_anim2d == nullptr)
				Enabled(FALSE);
			else
				Enabled(TRUE);
		}
			break;
		case COMPONENT_TYPE::RENDERER:
		{
			Renderer* _render = m_TargetObject->GetComponent<Renderer>(COMPONENT_TYPE::RENDERER);

			if (_render == nullptr)
				Enabled(FALSE);
			else
				Enabled(TRUE);
		}
			break;
		case COMPONENT_TYPE::CAMERA:
		{
			Camera* _cam = m_TargetObject->GetComponent<Camera>(COMPONENT_TYPE::CAMERA);

			if (_cam == nullptr)
				Enabled(FALSE);
			else
				Enabled(TRUE);
		}
			break;
		case COMPONENT_TYPE::RIGIDBODY:
		{
			Enabled(FALSE);
		}
		case COMPONENT_TYPE::END:
			break;
		case COMPONENT_TYPE::SCRIPT:
		{

		}
			break;
		default:
			break;
		}
	}
}