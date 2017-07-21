#pragma once

#include "../GLBase/GLWindow.h"

class VectorsAndMatrices :
	public GLWindow
{
private:
	GLuint vao;
	GLuint shader_programme;
	float matrix[16];
	float speed;
	float lastPosition;
protected:
	void TimeElapsed(ULONGLONG elapsedTime);
public:
	VectorsAndMatrices();
	VectorsAndMatrices(const GLWindow* window);
	~VectorsAndMatrices();
	void Init();
	void Render();
};

