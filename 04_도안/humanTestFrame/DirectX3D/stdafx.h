// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <assert.h>
#include <d3d9.h>
#include <d3dx9math.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")



extern HWND g_hWnd;

#include "Utility.h"

#include "Object.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "AutoReleasePool.h"
#include "TimeManager.h"

#include "VideoDevice.h"
#include "TextureManager.h"
#include "FontManager.h"

#include "Asciitok.h"
#include "ASECharacter.h"
#include "ASEFrame.h"
#include "ASELoader.h"

#include "AllocateHierarchy.h"
#include "SkinnedMesh.h"
#include "SkinnedMeshManager.h"
#include "FrustumCulling.h"

#include "UIObject.h"
#include "UIImageView.h"
#include "UITextView.h"
#include "UIButton.h"
#include "UIManager.h"

#include "CubeCtrl.h"
#include "SkyBox.h"
#include "CameraCtrl.h"
#include "GridCtrl.h"
#include "RayCtrl.h"
#include "CharacterCtrl.h"
#include "LinearInterpolation.h"

#include "MainGameManager.h"





