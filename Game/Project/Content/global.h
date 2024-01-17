#pragma once

#ifdef _DEBUG
#pragma comment(lib,"Engine_Debug.lib")
#else
#pragma comment(lib,"Engine.lib")
#endif // _DEBUG

#include "Content_enum.h"
#include "content_struct.h"
#include <random>