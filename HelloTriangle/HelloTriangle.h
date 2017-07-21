#pragma once

#include "../GLBase/GLWindow.h"

class HelloTriangle :
	public GLWindow
{
private:
	GLuint vao;
	GLuint shader_programme;
public:
	HelloTriangle();
	HelloTriangle(const GLWindow* window);
	~HelloTriangle();
	void Init();
	void Render();
};

