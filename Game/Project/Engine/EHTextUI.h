#pragma once
#include "EHComponentUI.h"
class TextUI :
	public ComponentUI
{
private:

public:
	virtual void Render_Update()override;

private:
	void TextInput();
	void MainSetting();

public:
	TextUI();
	virtual ~TextUI();
};

