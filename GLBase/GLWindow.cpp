#include "GLWindow.h"

#pragma comment (lib, "opengl32")

GLWindow::GLWindow()
	: _lastTick (GetTickCount64())
	, _createdVersion(1)
{
	observe(WM_CREATE, [this](WPARAM, LPARAM) {
		_deviceContext = GetDC(this->Handle());
		this->SetupContext(_deviceContext);
		_glContext = wglCreateContext(_deviceContext);
		MakeCurrent();
		LoadPrototypes();
		Init();
	});
	SetupCommon();
}

int GLWindow::ChoosePixelFormat(const HDC hdc, const int multisamples, const bool stereo3D) {
	FLOAT	fAttributes[] = { 0, 0 };
	int		iAttributes[] = {
		WGL_SAMPLE_BUFFERS_ARB, ((multisamples > 1) ? 1 : 0),
		WGL_SAMPLES_ARB, multisamples,
		WGL_DOUBLE_BUFFER_ARB, TRUE,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_RED_BITS_ARB, 8,
		WGL_BLUE_BITS_ARB, 8,
		WGL_GREEN_BITS_ARB, 8,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_STEREO_ARB, (stereo3D ? TRUE : FALSE),
		0, 0
	};

	int	pixelFormat;
	UINT numFormats;
	if (!wglChoosePixelFormatARB(hdc, iAttributes, fAttributes, 1, &pixelFormat, &numFormats)) {
		return -1;
	}
	return pixelFormat;
}

GLWindow::GLWindow(const GLWindow* window)
	: _lastTick(GetTickCount64())
	, _createdVersion(window->Version())
{

	observe(WM_CREATE, [this, window](WPARAM, LPARAM) {

		LoadPrototypes();

		_deviceContext = GetDC(this->Handle());
		int pixelFormat = this->ChoosePixelFormat(_deviceContext, 0, false);
		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
		DescribePixelFormat(_deviceContext, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
		if (!SetPixelFormat(_deviceContext, pixelFormat, &pfd)) {
			ShowLastError();
		}
		const int attribs[] =
		{
			//WGL_CONTEXT_MAJOR_VERSION_ARB,	3,
			//WGL_CONTEXT_MINOR_VERSION_ARB,	0,
			WGL_CONTEXT_FLAGS_ARB,			WGL_CONTEXT_DEBUG_BIT_ARB,
			WGL_CONTEXT_PROFILE_MASK_ARB,	WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0, 0
		};
		_glContext = wglCreateContextAttribsARB(_deviceContext, nullptr, attribs);

		MakeCurrent();
		Init();
	});
	SetupCommon();
}


void GLWindow::Destroy()
{
	wglDeleteContext(_glContext);
	ReleaseDC(Handle(), _deviceContext);
}

void GLWindow::SetupCommon()
{
	observe(WM_CLOSE, [](WPARAM, LPARAM) {PostQuitMessage(0); });

	observe(WM_SIZE, [this](WPARAM wParam, LPARAM lParam) {
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);
		Resized(width, height);
	});
}

GLWindow::~GLWindow()
{
	wglDeleteContext(_glContext);
}

int GLWindow::Version() const
{
	const char* version = (char*)glGetString(GL_VERSION);
	int version_max, version_min;
	sscanf(version, "%d.%d", &version_max, &version_min);
	return version_max;
}

void GLWindow::SetupContext(HDC hDC) {
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nSize = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;


	int format = ::ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, format, &pfd);
}

BOOL GLWindow::MakeCurrent()
{
	return ::wglMakeCurrent(_deviceContext, _glContext);
}

void GLWindow::Resized(int width, int height)
{
	MakeCurrent();
	glViewport(0, 0, width, height);
}

void GLWindow::SwapBuffers()
{
	::SwapBuffers(_deviceContext);
}

void GLWindow::Init()
{
	glClearColor(0.5, 0.55, 0.5, 1);
}

void GLWindow::Render()
{
	if (MakeCurrent()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void GLWindow::Tick()
{
	ULONGLONG thisTick = GetTickCount64();
	TimeElapsed(thisTick - _lastTick);
	_lastTick = thisTick;
}

void GLWindow::LoadPrototypes() {
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");

	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");

	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");

	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");

	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");

	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
}