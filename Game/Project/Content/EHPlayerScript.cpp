#include "pch.h"
#include "EHPlayerScript.h"

#include "EHGameObject.h"
#include "EHTransform.h"

#include "EHKeyMgr.h"

#include "EHTimeMgr.h"

#include "EHImGUIMgr.h"
#include "EHAssetMgr.h"

PlayerScript::PlayerScript()
	:m_Speed(200.f)
{
	SetName(L"PlayerScript");
}

PlayerScript::~PlayerScript()
{
}

void PlayerScript::Update()
{
	Transform* _transform = GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM);
	tTransform* _transformInfo = nullptr;

	if (_transform == nullptr)
	{
		return;
	}
	Vec4 _pos = _transform->GetRelativePosition();
	if (KEY_PRESSED(LEFT))
	{
		_pos.x -= m_Speed * DT;
	}

	if (KEY_PRESSED(RIGHT))
	{
		_pos.x += m_Speed * DT;
	}

	if (KEY_PRESSED(UP))
	{
		_pos.y += m_Speed * DT;
	}

	if (KEY_PRESSED(DOWN))
	{
		_pos.y -= m_Speed * DT;
	}

	if (KEY_PRESSED(SPACE))
	{
		GetOwner()->SetDead(TRUE);
	}

	if (KEY_TAP(V))
	{
		GameObject* _outline = new GameObject;
		Transform* _posttr = _outline->AddComponent<Transform>();
		MeshRenderer* _postMR = _outline->AddComponent<MeshRenderer>();

		_postMR->SetMesh(AssetMgr::GetInst()->FindAsset<Mesh>(L"DefaultRectMesh"));
		_postMR->SetMaterial(AssetMgr::GetInst()->FindAsset<Material>(L"OutLineMat"));

		_posttr->SetRelativePosition(GetOwner()->GetComponent<Transform>(COMPONENT_TYPE::TRANSFORM)->GetRelativePosition());
		_posttr->SetRelativeScale(Vec4(1600.f, 900.f, 0.f, 0.f));

		_outline->AddComponent<TestScript>();

		Object::Instantiate(_outline, (UINT)LAYER_TYPE::BACKGROUND1);
	}

	_transform->SetRelativePosition(_pos);
}