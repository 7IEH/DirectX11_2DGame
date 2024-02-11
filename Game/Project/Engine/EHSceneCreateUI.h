#pragma once
#include "EHUI.h"
class SceneCreateUI :
    public UI
{
private:
    string  m_SceneName;
public:
    virtual void Render_Update();
    
private:
    void File_Create(string _filepath);

public:
    SceneCreateUI();
    virtual ~SceneCreateUI();
};

