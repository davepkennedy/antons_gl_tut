#pragma once

#include "../GLBase/GLWindow.h"

class VertexBufferObjects :
	public GLWindow
{
private:
	GLuint vao;
	GLuint shader_programme;
public:
	VertexBufferObjects();
	VertexBufferObjects(const GLWindow* window);
	~VertexBufferObjects();
	void Init();
	void Render();
};

