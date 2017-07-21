#pragma once

#include "../GLBase/GLWindow.h"

class VirtualCamera :
	public GLWindow
{
public:
	VirtualCamera();
	VirtualCamera(const GLWindow* window);
	~VirtualCamera();
};

