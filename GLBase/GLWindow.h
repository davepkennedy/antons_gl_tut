#pragma once

#include "Window.h"
#include <gl\GL.h>
#include "glext.h"
#include "wglext.h"

class GLWindow :
	public Window
{
private:
	int _createdVersion;
	HDC _deviceContext;
	HGLRC _glContext;
	ULONGLONG _lastTick;
private:
	void SetupCommon();
protected:
	void SetupContext(HDC hDC);
	int ChoosePixelFormat(const HDC hdc, const int multisamples, const bool stereo3D);
	void LoadPrototypes();
	virtual void TimeElapsed(ULONGLONG elapsedTime) {}
public:
	GLWindow();
	GLWindow(const GLWindow* window);
	virtual ~GLWindow();
	void Destroy();
	BOOL MakeCurrent();
	void Resized(int width, int height);
	void Tick();
	virtual void Init();
	virtual void Render();
	void SwapBuffers();
	inline BOOL CanUpgrade() const {
		return (Version() > _createdVersion);
	}

	UINT ClassStyle() { return Window::ClassStyle() | CS_OWNDC; }
	LPCTSTR ClassName() { return TEXT("GLWindow"); }
	int Version() const;

protected:
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

	PFNGLGENBUFFERSPROC glGenBuffers;
	PFNGLBINDBUFFERPROC glBindBuffer;
	PFNGLBUFFERDATAPROC glBufferData;
	PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
	PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

	PFNGLCREATESHADERPROC glCreateShader;
	PFNGLSHADERSOURCEPROC glShaderSource;
	PFNGLCOMPILESHADERPROC glCompileShader;
	PFNGLCREATEPROGRAMPROC glCreateProgram;
	PFNGLATTACHSHADERPROC glAttachShader;
	PFNGLLINKPROGRAMPROC glLinkProgram;

	PFNGLUSEPROGRAMPROC glUseProgram;
	PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;

	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
};

