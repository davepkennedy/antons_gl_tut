#include "Window.h"

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* window = nullptr;
	if (msg == WM_CREATE) {
		LPCREATESTRUCT lpCreateStruct = (LPCREATESTRUCT)lParam;
		window = (Window*)lpCreateStruct->lpCreateParams;
		window->_window = hWnd;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)window);
	}
	else {
		window = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	if (window) {
		window->invoke(msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::Window()
	: _window(nullptr)
{
}


Window::~Window()
{
	Close();
	Destroy();
}

ATOM Window::Register()
{
	WNDCLASSEX wcx;

	::ZeroMemory(&wcx, sizeof(WNDCLASSEX));

	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcx.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcx.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcx.hIconSm = wcx.hIcon;
	wcx.hInstance = GetModuleHandle(nullptr);
	wcx.lpfnWndProc = WndProc;
	wcx.lpszClassName = ClassName();
	wcx.lpszMenuName = nullptr;
	wcx.style = ClassStyle();
	
	return RegisterClassEx(&wcx);
}

void Window::Create()
{
	CreateWindow(ClassName(), WindowName(), WindowStyle(),
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, 
		nullptr, nullptr, GetModuleHandle(nullptr), this);
}

void Window::Update()
{
	UpdateWindow(_window);
}

void Window::ShowWindow(int nCmdShow)
{
	::ShowWindow(_window, nCmdShow);
}

void Window::Close()
{
	if (_window) {
		CloseWindow(_window);
	}
}

void Window::Destroy()
{
	if (_window) {
		DestroyWindow(_window);
		_window = nullptr;
	}
}

SIZE Window::Size() const
{
	RECT rect;
	SIZE size = { 0, 0 };

	if (GetWindowRect(_window, &rect)) {
		size.cx = rect.right - rect.left;
		size.cy = rect.bottom - rect.top;
	}
	return size;
}


void Window::ShowLastError() const
{
	DWORD err = GetLastError();

	// Translate ErrorCode to String.
	LPTSTR error = nullptr;
	if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		err,
		0,(LPTSTR)&error,
		0,
		nullptr) == 0)
	{
		// Failed in translating.
	}

	// Display message.
	MessageBox(nullptr, error, TEXT("Error"), MB_OK | MB_ICONWARNING);

	// Free the buffer.
	if (error)
	{
		::LocalFree(error);
	}
}