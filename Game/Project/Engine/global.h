#pragma once

/********************
|	Global Header
********************/

// Text
#ifdef _DEBUG
#pragma comment(lib,"FW1FontWrapper.lib")
#else
#pragma comment(lib,"FW1FontWrapper.lib")
#endif

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

#include <stdlib.h>
#include <Windows.h>
#include <string>
#include <typeinfo>

#include <vector>
#include <queue>
#include <list>
#include <map>

#include <fstream>
#include <iostream>
#include <filesystem>

using std::vector;
using std::list;
using std::map;
using std::queue;
using std::make_pair;

using std::wstring;
using std::string;

#include <wrl.h>
using namespace Microsoft::WRL;

// Directx 11 헤더 및 라이브러리 참조
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <DirectXMath.h>
using namespace DirectX;

#ifdef _DEBUG
#pragma comment(lib,"DirectXTex_debug")
#else
#pragma comment(lib,"DirectXTex")
#endif

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// SimpleMath
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

// 엔진 헤더 참조
#include "singleton.h"
#include "define.h"
#include "enum.h"
#include "struct.h"
#include "func.h"

// ImGUI Header
#include "imgui_stdlib.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <thread>
#include <condition_variable>

// Error Function
void HandleError(HWND _hWnd, wstring _content, int flag);

// Content Library
#ifdef _DEBUG
#pragma comment(lib,"Content_Debug.lib")
#else
#pragma comment(lib,"Content.lib")
#endif // _DEBUG

// Extern
#include "Extern.h"
