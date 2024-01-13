#pragma once

#ifdef _DEBUG
#pragma comment(lib,"Engine_Debug.lib")
#else
#pragma comment(lib,"Engine.lib")
#endif // _DEBUG

#include "Content_enum.h"
#include "struct.h"
#include <random>