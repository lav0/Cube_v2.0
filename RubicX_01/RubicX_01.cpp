

#include "stdafx.h"
#include "DirectCore.h"
#include "DirectCubeManager.h"

#include <conio.h>
#include <algorithm>

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = nullptr;
HWND                    g_hWnd  = nullptr;
Dimention               g_CubeDimention = 2;
long                    g_wnd_width  = 1024;
long                    g_wnd_height = 800;
bool                    g_mouse_down = false;
short                   g_key_pressed = 0;
bool                    g_shift_key_pressed = false;

std::vector<IKeyboardListener*> m_keyboard_listeners;
std::vector<IMouseListener*>    m_mouse_listeners;

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );

void listerToInput();


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);
  
  HRESULT hr(S_OK);

  hr = InitWindow(hInstance, nCmdShow);
  
  if (FAILED(hr))
    return 0;
    
  DirectCore core(g_hWnd);
  hr = core.InitDevice();

  if (FAILED(hr))
    return 0;

  DirectCubeManager manager;
  hr = manager.Initialize( core.GetImmediateContext(), 
                           g_CubeDimention, 
                           g_wnd_width, 
                           g_wnd_height
                          );

  if (FAILED(hr))
    return 0;

  m_keyboard_listeners.push_back(&manager);
  m_mouse_listeners.push_back(&manager);
    
  // Main message loop
  MSG msg = {0};
  while (WM_QUIT != msg.message)
  {
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }
    else
    {   
      listerToInput();

      core.Clear();
      manager.Render();
      core.SwapChain();
    }
  }

  return static_cast<int>(msg.wParam);
}

bool keydown(short key)
{
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

void listenToKeyboard()
{
  short button = 0;
  short basis  = g_CubeDimention;
  
  auto base = 3 * g_CubeDimention + 1;

  if (keydown(VK_SHIFT) && keydown(VK_RIGHT)) {
    button = 1;
  } else if (keydown(VK_RIGHT)) {
    button = base + 1;
  } else if (keydown(VK_SHIFT) && keydown(VK_LEFT)) {
    button = g_CubeDimention;
  } else if (keydown(VK_LEFT)) {
    button = base + g_CubeDimention;
  } else if (keydown(VK_SHIFT) && keydown(VK_UP)) {
    button = 1 + g_CubeDimention;
  } else if (keydown(VK_UP)) {
    button = base + 1 + g_CubeDimention;
  } else if (keydown(VK_SHIFT) && keydown(VK_DOWN)) {
    button = 2 * g_CubeDimention;
  } else if (keydown(VK_DOWN)) {
    button = base + 2 * g_CubeDimention;
  } else if (keydown(VK_SHIFT) && keydown(VK_HOME)) {
    button = 1 + 2 * g_CubeDimention; 
  } else if (keydown(VK_HOME)) {
    button = base + 1 + 2 * g_CubeDimention; 
  } else if (keydown(VK_SHIFT) && keydown(VK_END)) {
    button = 3 * g_CubeDimention; 
  } else if (keydown(VK_END)) {
    button = base + 3 * g_CubeDimention; 
  } 

  if (g_key_pressed != button) 
  {
    short& key = g_key_pressed;
    std::for_each(m_keyboard_listeners.begin(), m_keyboard_listeners.end(), 
      [&key](IKeyboardListener* listener) 
      {
        listener->ListenToKeyboard(key); 
      }
    );
  }

  g_key_pressed = button;
}

void listenToMouse()
{
  long wdev2 = g_wnd_width  / 2;
  long hdev2 = g_wnd_height / 2;
  
  if (GetAsyncKeyState(VK_LBUTTON))
  {
    POINT mouse_point;
    GetCursorPos(&mouse_point);
    ScreenToClient(g_hWnd, &mouse_point);

    float x = static_cast<float>(mouse_point.x - wdev2) / wdev2;
    float y = static_cast<float>(mouse_point.y - hdev2) / hdev2;
    
    std::for_each(m_mouse_listeners.begin(), m_mouse_listeners.end(),
      [&x, &y](IMouseListener* listener)
      {
        listener->ListenToMouse(true, x, y);  
      }
    );

    g_mouse_down = true;
  } 
  else if (g_mouse_down)
  {
    std::for_each(m_mouse_listeners.begin(), m_mouse_listeners.end(),
      [](IMouseListener* listener)
      {
        listener->ListenToMouse(false, 0, 0);  
      }
    );

    g_mouse_down = false;
  } 
}

void listerToInput()
{ 
  listenToKeyboard();
  listenToMouse();
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )107 );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )107 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, g_wnd_width, g_wnd_height };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( L"TutorialWindowClass", L"Direct3D 11 Tutorial 4: 3D Spaces", WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL );
    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}



