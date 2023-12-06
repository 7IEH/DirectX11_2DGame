#pragma once

#define Single(classtype) private:\
							classtype();\
							~classtype();\
							friend class Singleton<classtype>;

#define DEVICE Device::GetInst()->GetDevice()
#define CONTEXT Device::GetInst()->GetContext()

#define PATH PathMgr::GetInst()->GetPath();

#define DT TimeMgr::GetInst()->GetDeltaTime()

#define KEY_CHECK(Key, State) KeyMgr::GetInst()->GetKeyState(Key) == State
#define KEY_TAP(Key) KEY_CHECK(Key, TAP)
#define KEY_PRESSED(Key) KEY_CHECK(Key, PRESSED)
#define KEY_RELEASED(Key) KEY_CHECK(Key, RELEASED)
#define KEY_NONE(Key) KEY_CHECK(Key, NONE)