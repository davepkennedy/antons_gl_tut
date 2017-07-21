#include "VirtualCamera.h"
#include "../GLBase/Application.h"

int CALLBACK wWinMain(
	HINSTANCE hInst,
	HINSTANCE hPrevInst,
	LPWSTR lpCmdLine,
	int nCmdShow
) {
	Application<VirtualCamera> app;
	return app.Run();
}

VirtualCamera::VirtualCamera()
{
}

VirtualCamera::VirtualCamera(const GLWindow* window)
	: GLWindow(window)
{

}


VirtualCamera::~VirtualCamera()
{
}
