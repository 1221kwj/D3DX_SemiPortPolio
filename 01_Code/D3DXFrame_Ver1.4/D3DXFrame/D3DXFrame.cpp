// D3DXFrame.cpp: 응용 프로그램의 진입점을 정의합니다.
#include "stdafx.h"
#include "D3DXFrame.h"
#include "MainGameManager.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HWND			_hWnd;
HINSTANCE		_hInstance;                                // 현재 인스턴스입니다.
WCHAR			szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR			szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
MainGameManager _mainGame;
POINT			_ptMouse;
float			_fDist = 5;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void SetWindowsSize(const int x, const int y, const int width, const int height);

int APIENTRY wWinMain(_In_		HINSTANCE hInstance,
                     _In_opt_	HINSTANCE hPrevInstance,
                     _In_		LPWSTR    lpCmdLine,
                     _In_		int       nCmdShow)
{
	_hInstance = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_D3DXFRAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))  return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_D3DXFRAME));

    MSG msg;

    // 기본 메시지 루프입니다.
   /* while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }*/

	if ( FAILED( _mainGame.init() ) ) return 0;

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			TIMEMANAGER->update(60.0f);
			_mainGame.update();
			_mainGame.render();
		}

	}

	_mainGame.release();

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D3DXFRAME));
    wcex.hInstance      = hInstance;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.lpszClassName	= szWindowClass;
	wcex.lpszMenuName	= nullptr;//MAKEINTRESOURCEW(IDC_D3DXFRAME);
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.style			= CS_HREDRAW | CS_VREDRAW;

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd = CreateWindowW(
	   szWindowClass,
	   szTitle,
	   WS_OVERLAPPEDWINDOW,
	   WINSTARTX,
	   WINSTARTY,
	   WINSIZEX,
	   WINSIZEY,
	   nullptr,
	   nullptr,
	   hInstance,
	   nullptr);

   SetWindowsSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   if (!hWnd) return FALSE;
  
   _hWnd = hWnd;
   

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return _mainGame.MainProc( hWnd, message, wParam, lParam );
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void SetWindowsSize(const int x, const int y, const int width, const int height)
{
	RECT winRect = { 0, };

	winRect.right	= width;
	winRect.bottom	= height;

	AdjustWindowRect(&winRect, WINSTYLE, false);

	SetWindowPos(
		_hWnd,
		nullptr,
		x,
		y,
		(winRect.right - winRect.left),
		(winRect.bottom - winRect.top),
		SWP_NOZORDER | SWP_NOMOVE);
}
