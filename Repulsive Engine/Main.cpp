#include"StandardWindow.h"

#include"CoreEngine.h"
#include"Sprite.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	StandardWindow window;
	CoreEngine engine(window);
	
	Sprite sprite(engine.GetGraphicsDevice());

	while (window.IsOpen())
	{
		engine.ClearFrame();
		sprite.Draw(engine.GetDeviceContext());
		engine.RenderFrame();
		Window::DispatchWindowEventsNonBlocking();
	}
	return 0;
}