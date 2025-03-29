// LabProject05.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "LabProject05-1.h"

#include "GameFramework.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

CGameFramework g_GameFramework;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif _DEBUG

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LABPROJECT051, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LABPROJECT051));
    
    MSG msg;

    // 기본 메시지 루프입니다:
    while (1)
    {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            if (!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
        }
        else
        {
            g_GameFramework.FrameAdvance();
        }
    }
    g_GameFramework.OnDestroy();


#ifdef _DEBUG
    _CrtDumpMemoryLeaks();
#endif

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_LABPROJECT051));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LABPROJECT051);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_BORDER;
   RECT rc{ 0,0,FRAME_BUFFER_WIDTH,FRAME_BUFFER_HEIGHT };
   ::AdjustWindowRect(&rc, dwStyle, FALSE);

   HWND hWnd = ::CreateWindow(szWindowClass, szTitle, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

#ifdef _DEBUG
   g_GameFramework.OnCreate(hInstance, hWnd, true, true);
#elif
   g_GameFramework.OnCreate(hInstance, hWnd, false, false);
#endif

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

#ifdef _WITH_SWAPCHAIN_FULLSCREEN_STATE
   g_GameFramework.ChangeSwapChainState();
#else

#endif

   return TRUE;
}


void HandleMenu(WORD param)
{
    switch (param)
    {
    case ID_DEMO_0:
        g_GameFramework.SetDemoShader(0);
        break;
    case ID_DEMO_1:
        g_GameFramework.SetDemoShader(1);
        break;
    case ID_DEMO_2:
        g_GameFramework.SetDemoShader(2);
        break;
    case ID_DEMO_3:
        g_GameFramework.SetDemoShader(3);
        break;
    case ID_DEMO_4:
        g_GameFramework.SetDemoShader(4);
        break;
    case ID_DEMO_5:
        g_GameFramework.SetDemoShader(5);
        break;
    case ID_DEMO_6:
        g_GameFramework.SetDemoShader(6);
        break;
    case ID_DEMO_7:
        g_GameFramework.SetDemoShader(7);
        break;
    case ID_DEMO_8:
        g_GameFramework.SetDemoShader(8);
        break;
    case ID_DEMO_9:
        g_GameFramework.SetDemoShader(9);
        break;
    case ID_DEMO_10:
        g_GameFramework.SetDemoShader(10);
        break;
    case ID_DEMO_11:
        g_GameFramework.SetDemoShader(11);
        break;
    case ID_DEMO_12:
        g_GameFramework.SetDemoShader(12);
        break;
    case ID_DEMO_13:
        g_GameFramework.SetDemoShader(13);
        break;
    case ID_DEMO_14:
        g_GameFramework.SetDemoShader(14);
        break;
    }
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MOUSEMOVE:
    case WM_KEYDOWN:
    case WM_KEYUP:
        g_GameFramework.OnProcessingWindowMessage(hWnd, message, wParam, lParam);
        break;

    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;

    case WM_COMMAND:
        HandleMenu(LOWORD(wParam));
    default:
        return ::DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
