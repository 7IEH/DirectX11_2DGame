#pragma once

#ifdef _DEBUG
#pragma comment(lib,"Engine_Debug.lib")
#else
#pragma comment(lib,"Engine.lib")
#endif // _DEBUG

#include "Content_enum.h"
#include "content_struct.h"
#include <random>

#define CLONE(TYPE) virtual TYPE* Clone(){return new TYPE(*this);}
#define CLONE_DISABLE(TYPE) TYPE* Clone(){return nullptr; assert(nullptr);}\
							TYPE(const TYPE& _originBuffer) = delete;