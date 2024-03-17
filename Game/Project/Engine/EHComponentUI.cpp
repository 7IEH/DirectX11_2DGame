#include "pch.h"
#include "EHComponentUI.h"

#include "EHGameObject.h"
#include "EHScriptUI.h"

ComponentUI::ComponentUI(const string& _label, const string& _strID, COMPONENT_TYPE _type)
	:UI(_label, _strID)
	, m_TargetObject(nullptr)
	, m_Type(_type)
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
			{
				if (COLLIDER_TYPE::BOXCOLLIDER2D == _col2d->GetColliderType())
				{
					this->SetLabel("BoxCollider2D");
				}
				else if (COLLIDER_TYPE::CIRCLECOLLDIER2D == _col2d->GetColliderType())
				{
					this->SetLabel("CircleCollider2D");
				}
				Enabled(TRUE);
			}
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
			{
				if (RENDERER_TYPE::MESHRENDERER == _render->GetRenderType())
				{
					this->SetLabel("MeshRenderer");
				}
				else if (RENDERER_TYPE::TILEMAP == _render->GetRenderType())
				{
					this->SetLabel("TileMapRenderer");
				}
				else if(RENDERER_TYPE::PARTICLESYSTEM == _render->GetRenderType())
				{
					this->SetLabel("ParticleSystem");
				}
				else
				{
					this->SetLabel("CavasRenderer");
				}
				Enabled(TRUE);
			}
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
		case COMPONENT_TYPE::TEXT:
		{
			Text* _text = m_TargetObject->GetComponent<Text>(COMPONENT_TYPE::TEXT);

			if (_text == nullptr)
				Enabled(FALSE);
			else
				Enabled(TRUE);
		}
		break;
		case COMPONENT_TYPE::BUTTON:
		{
			Button* _button = m_TargetObject->GetComponent<Button>(COMPONENT_TYPE::BUTTON);

			if (_button == nullptr)
				Enabled(FALSE);
			else
				Enabled(TRUE);
		}
		break;
		case COMPONENT_TYPE::RIGIDBODY:
		{
			Enabled(FALSE);
		}
		break;
		case COMPONENT_TYPE::END:
			break;
		case COMPONENT_TYPE::SCRIPT:
		{
			ScriptUI* _this = dynamic_cast<ScriptUI*>(this);
			string _scriptName = _this->GetName();

			vector<Script*> _scripts = m_TargetObject->GetScripts();

			bool _flag = FALSE;
			for (size_t i = 0;i < _scripts.size();i++)
			{
				if (_scriptName == EH::ConvertString(_scripts[i]->GetName()))
				{
					_flag = TRUE;
					break;
				}
			}

			if (_flag)
			{
				Enabled(TRUE);
			}
			else
			{
				Enabled(FALSE);
			}
		}
		break;
		default:
			break;
		}
	}
}