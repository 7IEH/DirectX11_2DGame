#pragma once
#include "EHComponent.h"

enum class BUTTON_STATE
{
	HIGHLIGHT,
	PRESSED,
	SELECTED,
	NONE,
	DISABLED,
	END
};

class Button :
	public Component
{
private:
	BUTTON_STATE m_State;

	Ptr<Sprite> m_Images[(UINT)BUTTON_STATE::END];

public:
	void SetImage(BUTTON_STATE _state, Ptr<Sprite>_image) { m_Images[(UINT)_state] = _image; }
	Ptr<Sprite> GetImage(BUTTON_STATE _state) { return m_Images[(UINT)_state]; }

public:
	virtual void Update()override;
	virtual void LateUpdate()override {};

	virtual void Save(string _path)override;

	CLONE(Button)

public:
	Button();
	virtual ~Button();
};

