#pragma once

#define Single(classtype) private:\
							classtype();\
							~classtype();\
							friend class Singleton<classtype>;

#define MAIN_HWND Device::GetInst()->GetHwnd()

#define DEVICE Device::GetInst()->GetDevice()
#define CONTEXT Device::GetInst()->GetContext()

#define PATH PathMgr::GetInst()->GetPath()

#define DT TimeMgr::GetInst()->GetDeltaTime()

#define CLONE(TYPE)	virtual TYPE* Clone(){return new TYPE(*this);}
#define CLONE_DISABLE(TYPE) TYPE* Clone(){return nullptr; assert(nullptr);}\
							TYPE(const TYPE& _originBuffer) = delete;

#define KEY_CHECK(Key, State) KeyMgr::GetInst()->GetKeyState(Key) == State
#define KEY_TAP(Key) KEY_CHECK(Key, TAP)
#define KEY_PRESSED(Key) KEY_CHECK(Key, PRESSED)
#define KEY_RELEASED(Key) KEY_CHECK(Key, RELEASED)
#define KEY_NONE(Key) KEY_CHECK(Key, NONE)

#define ASPECT_RATIO Device::GetInst()->GetResolution().x/ Device::GetInst()->GetResolution().y

#define FIND_OBJECT(OBJECTNAME) LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(OBJECTNAME);