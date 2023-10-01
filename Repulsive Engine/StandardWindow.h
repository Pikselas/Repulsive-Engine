#pragma once
#include"CustomWindow.h"

class StandardWindow : public CustomWindow
{
private:
	bool Closed = false;
	static int WindowCount;
public:
	static void MainLoop(const StandardWindow* const window = nullptr, EventDispatcher e = DispatchWindowEvents);
public:
	StandardWindow(const std::string& Title = "Window", unsigned int Width = 800, unsigned int Height = 600 , StandardWindow* Parent = nullptr);
	~StandardWindow();
public:
	bool IsOpen() const;
	unsigned int GetWindowCount() const;
protected:
	LRESULT CALLBACK EventHandler(HWND handle, UINT msgcode, WPARAM wparam, LPARAM lparam) override;
};