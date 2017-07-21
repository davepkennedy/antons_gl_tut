#pragma once

#include <Windows.h>

template <typename WINDOW>
class Application {
private:
	WINDOW* window;
	void Setup() 
	{
		WINDOW dummy;
		dummy.Register();
		dummy.Create();
		if (dummy.CanUpgrade()) {
			window = new WINDOW(&dummy);
		}
		else {
			window = new WINDOW;
		}
		window->Register();
		window->Create();
		window->ShowWindow(SW_SHOW);
		window->Update();
	}

	void Loop()
	{
		MSG msg;

		ZeroMemory(&msg, sizeof(MSG));
		while (true) {
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {
					break;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (window->MakeCurrent()) {
				//window->Resized();
				window->Tick();
				window->Render();
				window->SwapBuffers();
			}
		}

	}

	void Teardown() 
	{
		delete window;
	}
public:
	int Run() 
	{
		Setup();
		Loop();
		Teardown();
		return 0;
	}
};