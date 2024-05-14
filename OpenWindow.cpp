#include "OpenWindow.h"
#include "Scene.h"
#include <map>

Vector2 MousePosition;
std::map<char, bool> KeysPressed;

bool KeyPressed(char key) {
	return KeysPressed[key];
}

static LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {

	case WM_PAINT:
		PAINTSTRUCT paintStructure;
		Update();
		glFlush();
		BeginPaint(windowHandle, &paintStructure);
		EndPaint(windowHandle, &paintStructure);
		break;

	case WM_DESTROY:
		CoUninitialize();
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_KEYDOWN:
		KeysPressed[(char)wParam] = true;
		break;

	case WM_KEYUP:
		KeysPressed[(char)wParam] = false;
		break;

	case WM_LBUTTONUP:
		KeysPressed[LeftMouseButton] = false;
		break;

	case WM_LBUTTONDOWN:
		KeysPressed[LeftMouseButton] = true;
		break;

	case WM_RBUTTONUP:
		KeysPressed[RightMouseButton] = false;
		break;

	case WM_RBUTTONDOWN:
		KeysPressed[RightMouseButton] = true;
		break;

	case WM_MOUSEMOVE:
		MousePosition = Vector2(LOWORD(lParam), HIWORD(lParam));
		break;
	}
	return DefWindowProc(windowHandle, message, wParam, lParam);
}

static HWND WindowSetup(HINSTANCE hInstance) {

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

	HWND windowHandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, windowClassName, title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1000, 500, NULL, NULL, hInstance, NULL);

	HDC deviceContextHandle = GetDC(windowHandle);

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

	FILE* consleFilePointer;
	AllocConsole();
	freopen_s(&consleFilePointer, "CONOUT$", "w", stdout);

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

	glGetProgramiv = (void (*)(GLuint, GLenum, GLint*))wglGetProcAddress("glGetProgramiv");
	glActiveTexture = (void (*)(GLenum))wglGetProcAddress("glActiveTexture");
	glAttachShader = (void (*)(GLuint, GLuint))wglGetProcAddress("glAttachShader");
	glBindBuffer = (void (*)(GLenum, GLuint))wglGetProcAddress("glBindBuffer");
	glBindFramebuffer = (void (*)(GLenum, GLuint))wglGetProcAddress("glBindFramebuffer");
	glBufferData = (void (*)(GLenum, GLsizeiptr, const GLvoid*, GLenum))wglGetProcAddress("glBufferData");
	glBufferSubData = (void (*)(GLenum, GLintptr, GLsizeiptr, const GLvoid*))wglGetProcAddress("glBufferSubData");
	glCheckFramebufferStatus = (GLenum(*)(GLenum))wglGetProcAddress("glCheckFramebufferStatus");
	glClearBufferfv = (void (*)(GLenum, GLint, const GLfloat*))wglGetProcAddress("glClearBufferfv");
	glCompileShader = (void (*)(GLenum))wglGetProcAddress("glCompileShader");
	glCreateProgram = (GLuint(*)(void))wglGetProcAddress("glCreateProgram");
	glCreateShader = (GLuint(*)(GLenum))wglGetProcAddress("glCreateShader");
	glDeleteBuffers = (void (*)(GLsizei, const GLuint*))wglGetProcAddress("glDeleteBuffers");
	glDeleteFramebuffers = (void (*)(GLsizei, const GLuint*))wglGetProcAddress("glDeleteFramebuffers");
	glEnableVertexAttribArray = (void (*)(GLenum))wglGetProcAddress("glEnableVertexAttribArray");
	glDrawBuffers = (void (*)(GLsizei, const GLenum*))wglGetProcAddress("glDrawBuffers");
	glFramebufferTexture2D = (void (*)(GLenum, GLenum, GLenum, GLuint, GLint))wglGetProcAddress("glFramebufferTexture2D");
	glGenBuffers = (void (*)(GLsizei, GLuint*))wglGetProcAddress("glGenBuffers");
	glGenFramebuffers = (void (*)(GLsizei, GLuint*))wglGetProcAddress("glGenFramebuffers");
	glGetAttribLocation = (GLint(*)(GLuint, const GLchar*))wglGetProcAddress("glGetAttribLocation");
	glGetShaderInfoLog = (void (*)(GLuint, GLsizei, GLsizei*, GLchar*))wglGetProcAddress("glGetShaderInfoLog");
	glGetShaderiv = (void (*)(GLuint, GLenum, GLint*))wglGetProcAddress("glGetShaderiv");
	glGetUniformLocation = (GLint(*)(GLuint, const GLchar*))wglGetProcAddress("glGetUniformLocation");
	glLinkProgram = (void (*)(GLenum))wglGetProcAddress("glLinkProgram");
	glShaderSource = (void (*)(GLuint, GLsizei, const GLchar* const*, const GLint*))wglGetProcAddress("glShaderSource");
	glUniform1i = (void (*)(GLint, GLint))wglGetProcAddress("glUniform1i");
	glUniform1f = (void (*)(GLint, GLfloat))wglGetProcAddress("glUniform1f");
	glUniform2f = (void (*)(GLint, GLfloat, GLfloat))wglGetProcAddress("glUniform2f");
	glUniform4f = (void (*)(GLint, GLfloat, GLfloat, GLfloat, GLfloat))wglGetProcAddress("glUniform4f");
	glUniformMatrix4fv = (void (*)(GLint, GLsizei, GLboolean, const GLfloat*))wglGetProcAddress("glUniformMatrix4fv");
	glUseProgram = (void (*)(GLenum))wglGetProcAddress("glUseProgram");
	glDeleteShader = (void (*)(GLenum))wglGetProcAddress("glDeleteShader");
	glBindVertexArray = (void (*)(GLenum))wglGetProcAddress("glBindVertexArray");
	glGenVertexArrays = (void (*)(GLsizei, GLuint*))wglGetProcAddress("glGenVertexArrays");
	glGenerateMipmap = (void (*)(GLenum))wglGetProcAddress("glGenerateMipmap");
	glVertexAttribPointer = (void (*)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*))wglGetProcAddress("glVertexAttribPointer");
	glUniformMatrix3fv = (void (*)(GLint, GLsizei, GLboolean, const GLfloat*))wglGetProcAddress("glUniformMatrix3fv");
	return windowHandle;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {

	HWND windowHandle = WindowSetup(hInstance);
	Start();
	ShowWindow(windowHandle, nCmdShow);
	UpdateWindow(windowHandle);
	MSG message;
	while (true)
	{
		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		SendMessage(windowHandle, WM_PAINT, 0, 0);
	}
}

void (*glGetProgramiv)(GLuint, GLenum, GLint*);
void (*glActiveTexture) (GLenum);
void (*glAttachShader) (GLuint, GLuint);
void (*glBindBuffer) (GLenum, GLuint);
void (*glBindFramebuffer) (GLenum, GLuint);
void (*glBufferData) (GLenum, GLsizeiptr, const GLvoid*, GLenum);
void (*glBufferSubData) (GLenum, GLintptr, GLsizeiptr, const GLvoid*);
GLenum(*glCheckFramebufferStatus) (GLenum);
void (*glClearBufferfv) (GLenum, GLint, const GLfloat*);
void (*glCompileShader) (GLuint);
GLuint(*glCreateProgram) (void);
GLuint(*glCreateShader) (GLenum);
void (*glDeleteFramebuffers) (GLsizei, const GLuint*);
void (*glEnableVertexAttribArray) (GLuint);
void (*glDrawBuffers) (GLsizei, const GLenum*);
void (*glFramebufferTexture2D) (GLenum, GLenum, GLenum, GLuint, GLint);
void (*glGenBuffers) (GLsizei, GLuint*);
void (*glGenFramebuffers) (GLsizei, GLuint*);
GLint(*glGetAttribLocation) (GLuint, const GLchar*);
void (*glGetShaderInfoLog) (GLuint, GLsizei, GLsizei*, GLchar*);
void (*glGetShaderiv) (GLuint, GLenum, GLint*);
GLint(*glGetUniformLocation) (GLuint, const GLchar*);
void (*glLinkProgram) (GLuint);
void (*glShaderSource) (GLuint, GLsizei, const GLchar* const*, const GLint*);
void (*glUniform1i) (GLint, GLint);
void (*glUniform1f) (GLint, GLfloat);
void (*glUniform2f) (GLint, GLfloat, GLfloat);
void (*glUniform4f) (GLint, GLfloat, GLfloat, GLfloat, GLfloat);
void (*glUniformMatrix4fv) (GLint, GLsizei, GLboolean, const GLfloat*);
void (*glUseProgram) (GLuint);
void (*glGenerateMipmap) (GLenum);
void (*glDeleteShader) (GLuint);
void (*glBindVertexArray) (GLuint);
void (*glGenVertexArrays) (GLsizei, GLuint*);
void (*glVertexAttribPointer) (GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);
void (*glUniformMatrix3fv)(GLint, GLsizei, GLboolean, const GLfloat*);
void (*glDeleteBuffers) (GLsizei, const GLuint*);