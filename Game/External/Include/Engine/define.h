#pragma once

#define Single(classtype) private:\
							classtype();\
							~classtype();\
							friend class Singleton<classtype>;

#define DEVICE Device::GetInst()->GetDevice()
#define CONTEXT Device::GetInst()->GetContext()

#define PATH PathMgr::GetInst()->GetPath();

#define DT TimeMgr::GetInst()->GetDeltaTime();