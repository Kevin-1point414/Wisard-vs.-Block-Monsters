#include "OpenWindow.h"
#include "Scene.h"
#include <map>

Vector2 mousePosition;
std::map<char, bool> keysPressed;
LARGE_INTEGER timeOld;
LARGE_INTEGER frequency;
Vector2 screenSize;
Vector2 screenRatio;
HDC deviceContextHandle;
HWND windowHandle;
bool gameRunning;

bool KeyPressed(char key) {
	return keysPressed[key];
}

static LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {

	case WM_PAINT:
		PAINTSTRUCT paintStructure;

		LARGE_INTEGER time;

		QueryPerformanceCounter(&time);
		Update((float)((double)(time.QuadPart) - timeOld.QuadPart) / frequency.QuadPart);
		timeOld.QuadPart = time.QuadPart;

		glFlush();
		BeginPaint(windowHandle, &paintStructure);
		EndPaint(windowHandle, &paintStructure);
		break;

	case WM_SIZE:
	{
		glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		screenSize = Vector2(LOWORD(lParam), HIWORD(lParam));
		float sum = screenSize.x + screenSize.y;
		screenRatio.x = sum / screenSize.x;
		screenRatio.y = sum / screenSize.y;
	}
		break;

	case WM_KEYDOWN:
		keysPressed[(char)wParam] = true;
		break;

	case WM_KEYUP:
		keysPressed[(char)wParam] = false;
		break;

	case WM_LBUTTONUP:
		keysPressed[LeftMouseButton] = false;
		break;

	case WM_LBUTTONDOWN:
		keysPressed[LeftMouseButton] = true;
		break;

	case WM_RBUTTONUP:
		keysPressed[RightMouseButton] = false;
		break;

	case WM_RBUTTONDOWN:
		keysPressed[RightMouseButton] = true;
		break;

	case WM_MOUSEMOVE:
		mousePosition = Vector2(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_CLOSE:
		gameRunning = false;
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(windowHandle, message, wParam, lParam);
}

static void WindowSetup(HINSTANCE hInstance) {

	gameRunning = true;
	TCHAR windowClassName[] = L"DesktopApp";
	TCHAR title[] = L"Game Window";
	WNDCLASSEX windowClass;
	memset(&windowClass, 0, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_OWNDC;
	windowClass.lpfnWndProc = WndProc;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(windowClass.hInstance, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = LoadIcon(windowClass.hInstance, IDI_APPLICATION);
	RegisterClassEx(&windowClass);

	windowHandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, windowClassName, title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1000, 500, NULL, NULL, hInstance, NULL);

	deviceContextHandle = GetDC(windowHandle);

	const PIXELFORMATDESCRIPTOR pixelFormatDescriptor = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
		PFD_TYPE_RGBA,
		32,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		24,
		8,
		0,
		PFD_MAIN_PLANE,
		0,0,0,0
	};
	SetPixelFormat(deviceContextHandle, ChoosePixelFormat(deviceContextHandle,
		&pixelFormatDescriptor), &pixelFormatDescriptor);

	HGLRC openglRenderingContext = wglCreateContext(deviceContextHandle);
	wglMakeCurrent(deviceContextHandle, openglRenderingContext);

	RECT windowRectangle;
	GetWindowRect(windowHandle, &windowRectangle);
	glViewport(0, 0, windowRectangle.right - windowRectangle.left, windowRectangle.bottom -
		windowRectangle.top);

	screenSize = Vector2((float)windowRectangle.right - windowRectangle.left, (float)windowRectangle.bottom -
		windowRectangle.top);

	// FILE* consleFilePointer;
	//AllocConsole();
	//freopen_s(&consleFilePointer, "CONOUT$", "w", stdout);
	QueryPerformanceCounter(&timeOld);
	QueryPerformanceFrequency(&frequency);

	if (CoInitialize(NULL) != S_OK) {
		std::cout << "Error with CoInitialize at line 95 in OpenWindow.cpp";
	};

	HMODULE libery = LoadLibraryA("opengl32.dll");

	if (libery != 0) {
		BYTE* wglGetProcAddress = (BYTE*)GetProcAddress(libery, "wglGetProcAddress");
	}
	else {
		std::cout << "Error with LoadLibraryA(\"opengl32.dll\") at line 100 in OpenWindow.cpp";
	}
	glAttachShader = (void (*)(GLuint, GLuint))wglGetProcAddress("glAttachShader");
	glBindBuffer = (void (*)(GLenum, GLuint))wglGetProcAddress("glBindBuffer");
	glBufferData = (void (*)(GLenum, GLsizeiptr, const GLvoid*, GLenum))wglGetProcAddress("glBufferData");
	glCompileShader = (void (*)(GLenum))wglGetProcAddress("glCompileShader");
	glCreateProgram = (GLuint(*)(void))wglGetProcAddress("glCreateProgram");
	glCreateShader = (GLuint(*)(GLenum))wglGetProcAddress("glCreateShader");
	glEnableVertexAttribArray = (void (*)(GLenum))wglGetProcAddress("glEnableVertexAttribArray");
	glGenBuffers = (void (*)(GLsizei, GLuint*))wglGetProcAddress("glGenBuffers");
	glGetUniformLocation = (GLint(*)(GLuint, const GLchar*))wglGetProcAddress("glGetUniformLocation");
	glLinkProgram = (void (*)(GLenum))wglGetProcAddress("glLinkProgram");
	glShaderSource = (void (*)(GLuint, GLsizei, const GLchar* const*, const GLint*))wglGetProcAddress("glShaderSource");
	glUniformMatrix4fv = (void (*)(GLint, GLsizei, GLboolean, const GLfloat*))wglGetProcAddress("glUniformMatrix4fv");
	glUseProgram = (void (*)(GLenum))wglGetProcAddress("glUseProgram");
	glDeleteShader = (void (*)(GLenum))wglGetProcAddress("glDeleteShader");
	glBindVertexArray = (void (*)(GLenum))wglGetProcAddress("glBindVertexArray");
	glGenVertexArrays = (void (*)(GLsizei, GLuint*))wglGetProcAddress("glGenVertexArrays");
	glGenerateMipmap = (void (*)(GLenum))wglGetProcAddress("glGenerateMipmap");
	glVertexAttribPointer = (void (*)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*))wglGetProcAddress("glVertexAttribPointer");
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {

	WindowSetup(hInstance);
	Start();
	ShowWindow(windowHandle, nCmdShow);
	UpdateWindow(windowHandle);

	MSG message;
	while (gameRunning)
	{
		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		SendMessage(windowHandle, WM_PAINT, 0, 0);
	}
	return 0;
}

void (*glAttachShader) (GLuint, GLuint);
void (*glBindBuffer) (GLenum, GLuint);
void (*glBufferData) (GLenum, GLsizeiptr, const GLvoid*, GLenum);
void (*glCompileShader) (GLuint);
GLuint(*glCreateProgram) (void);
GLuint(*glCreateShader) (GLenum);
void (*glEnableVertexAttribArray) (GLuint);
void (*glGenBuffers) (GLsizei, GLuint*);
GLint(*glGetUniformLocation) (GLuint, const GLchar*);
void (*glLinkProgram) (GLuint);
void (*glShaderSource) (GLuint, GLsizei, const GLchar* const*, const GLint*);
void (*glUniformMatrix4fv) (GLint, GLsizei, GLboolean, const GLfloat*);
void (*glUseProgram) (GLuint);
void (*glGenerateMipmap) (GLenum);
void (*glDeleteShader) (GLuint);
void (*glBindVertexArray) (GLuint);
void (*glGenVertexArrays) (GLsizei, GLuint*);
void (*glVertexAttribPointer) (GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);
