#pragma once

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

using std::vector;
using std::list;
using std::map;
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

#include <thread>
#include <condition_variable>

// Error Function
void HandleError(HWND _hWnd, wstring _content, int flag);