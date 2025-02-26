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
#include <commdlg.h>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <d3d9.h>
#include <d3dx9math.h>
#include <assert.h>
#include <iostream>


#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

using namespace std;

#include "commonMacroFunction.h"
#include "Object.h"
#include "deviceManager.h"
#include "Camera.h"
#include "SkinnedMesh.h"
#include "NoBoneMesh.h"
#include "AllocateHierarchy.h"
#include "UIImageView.h"
#include "UIObject.h"
#include "randomFunction.h"
#include "gridCtrl.h"
#include "timeManager.h"
#include "keyManager.h"
#include "sceneManager.h"
#include "textureManager.h"
#include "fontManager.h"
#include "ObjectManager.h"
#include "AutoReleasePool.h"
#include "Group.h"
#include "enemy.h"
#include "OBJLoaderManager.h"
#include "SkinnedMeshManager.h"
#include "Loading.h"

//=========================================================
//                       ## DEFINE ##
//=========================================================
//WinAPI 관련 디파인 문
#define WINSTARTX	0
#define WINSTARTY	0
#define WINSIZEX	1280
#define WINSIZEY	720
#define WINSTYLE	WS_CAPTION | WS_SYSMENU

//싱글톤 디파인
#define RANDOMFUNCTION		 randomFunction		::getSingleton()
#define TIMEMANAGER			 timeManager		::getSingleton()
#define KEYMANAGER			 keyManager			::getSingleton()
#define SCENEMANAGER		 sceneManager		::getSingleton()
#define DEVICEMANAGER		 deviceManager		::getSingleton()
#define DEVICE				 deviceManager		::getSingleton()->GetDevice()
#define TEXTUREMANAGER		 textureManager		::getSingleton()
#define FONTMANAGER			 fontManager		::getSingleton()
#define CAMERAMANAGER		 Camera				::getSingleton()
#define GRIDMANAGER			 gridCtrl			::getSingleton()
#define OBJECTMANAGER		 ObjectManager		::getSingleton()
#define AUTORELEASEPOOL		 AutoReleasePool	::getSingleton()
#define OBJLOADERMANAGER	 OBJLoaderManager	::getSingleton()
#define SKINNEDMESHMANAGER	 SkinnedMeshManager	::getSingleton()

//=========================================================
//                 ## GLOBAL VARIABLE ##
//=========================================================
extern HWND			_hWnd;
extern HINSTANCE	_hInstance;
extern POINT		_ptMouse;
extern float		_fDist;

#ifdef _DEBUG
#pragma comment(linker , "/entry:wWinMainCRTStartup /subsystem:console")
#endif