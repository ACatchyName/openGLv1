#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "cgfxopengl.h"

bool exiting, fullscreen = false;
long windowWidth = 600, windowHeight = 400, windowBits = 32;
HDC hDC;

CGfxOpenGL *g_glRender = NULL;

void SetupPixelFormat(HDC hDC)
{
	int pixelFormat;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),	//size
		1,								//version
		PFD_SUPPORT_OPENGL |			//OpenGL window
		PFD_DRAW_TO_WINDOW |			//render to window
		PFD_DOUBLEBUFFER,				//support double-buffer
		PFD_TYPE_RGBA,					//color type
		32,								//prefered color depth
		0, 0, 0, 0, 0, 0,				//color bits - IGNORE
		0,								//no alpha buffer
		0,								//alpha bits - IGNORE
		0,								//no accumulation buffer
		0, 0, 0, 0,						//accum bits - IGNORE
		16,								//depth buffer
		0,								//no stencil buffer
		0,								//no auxiliary buffers
		PFD_MAIN_PLANE,					//main layer
		0,								//reserved
		0, 0, 0,						//no layer, visible, damage masks
	};

	pixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelFormat, &pfd);
}

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HDC hDC;
	static HGLRC hRC;
	int height, width;

	//dispatch message
	switch (uMsg)
	{
		case WM_CREATE:			//window creation
			hDC = GetDC(hWnd);
			SetupPixelFormat(hDC);
			hRC = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hRC);
			break;

		case WM_DESTROY:		//window destroy

		case WM_QUIT:

		case WM_CLOSE:			//window closing

			//deselect rendering context and delete it
			wglMakeCurrent(hDC, NULL);
			wglDeleteContext(hRC);

			PostQuitMessage(0);	//send WM_QUIT
			break;

		case WM_SIZE:
			height = HIWORD(lParam);	//retrieive width, height
			width = LOWORD(lParam);

			g_glRender->SetupProjection(width, height);
			break;

		case WM_ACTIVATEAPP:	//activate app
			break;

		case WM_PAINT:			//paint
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_LBUTTONDOWN:	//left mouse button
			break;

		case WM_RBUTTONDOWN:	//right mouse button
			break;

		case WM_MOUSEMOVE:		//mouse movement
			break;

		case WM_LBUTTONUP:		//left button release
			break;

		case WM_RBUTTONUP:		//right button release
			break;
			
		case WM_KEYUP:
			break;

		case WM_KEYDOWN:
			int fwKeys;
			LPARAM keyData;
			fwKeys = (int)wParam;	//virtual keycode
			keyData = lParam;		//key data
			switch (fwKeys)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
				default:
					break;
			}

		default:
			break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX windowClass;	//window class
	HWND hwnd;				//window handle
	MSG msg;				//message
	DWORD dwExStyle;		//window extended style
	DWORD dwStyle;			//window style
	RECT windowRect;

	g_glRender = new CGfxOpenGL;

	windowRect.left = (long)0;				//set left value to origin
	windowRect.right = (long)windowWidth;	//set right value to requested width
	windowRect.top = (long)0;				//set top value to origin
	windowRect.bottom = (long)windowHeight;	//set bottom to requested height

	//filout window class structure
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = MainWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//default icon
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);		//default arrow
	windowClass.hbrBackground = NULL;						//no background
	windowClass.lpszMenuName = NULL;						//no menu
	windowClass.lpszClassName = "OpenGL";
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);		//small windows logo icon

	if (!RegisterClassEx(&windowClass))	//register the windows class
		return 0;

	if(fullscreen)	//fullscreen?
	{
		DEVMODE dmScreenSettings;	//device mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = windowWidth;		//screen width
		dmScreenSettings.dmPelsHeight = windowHeight;	//screen height
		dmScreenSettings.dmBitsPerPel = windowBits;		//bits per pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PANNINGHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			//setting display mode fail
			MessageBox(NULL, "Display mode failed", NULL, MB_OK);
			fullscreen = false;
		}
	}

	if (fullscreen)	//still fullscreen
	{
		dwExStyle = WS_EX_APPWINDOW;	//window extended style
		dwStyle = WS_POPUP;				//window style
		ShowCursor(FALSE);				//hide mouse
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	//window extended style
		dwStyle = WS_OVERLAPPEDWINDOW;					//window style
	}

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle); //adjust to requested

	hwnd = CreateWindowEx(
		NULL,								//extended style
		"OpenGL",							//class name
		"OpenGL Verion 0.1",				//app name
		dwStyle |
		WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		0, 0,								//x, y coordinates
		windowRect.right - windowRect.left,	//width, height
		windowRect.bottom - windowRect.top,
		NULL,								//handle to parent
		NULL,								//handle to menu
		hInstance,							//app instance
		NULL);								//no extra params

	hDC = GetDC(hwnd);

	if (!hwnd)	//window creation failure?
		return 0;

	ShowWindow(hwnd, SW_SHOW);	//display window
	UpdateWindow(hwnd);			//update window

	g_glRender->Init();

	while (!exiting)
	{
		g_glRender->Prepare(0.0f);
		g_glRender->Render();
		SwapBuffers(hDC);

		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
			{
				exiting = true;
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		delete g_glRender;

		if (fullscreen)
		{
			ChangeDisplaySettings(NULL, 0);	//switch to desktop
			ShowCursor(TRUE);				//show mouse
		}

		return (int)msg.wParam;
	}
}