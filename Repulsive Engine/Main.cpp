#include"CoreEngine.h"
#include"StandardWindow.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CoreEngine engine;

	StandardWindow window("Repulsive Engine" , 800 , 600);
	window.SetIcon("Media/logo.ico");
	auto window_renderer = engine.CreateRenderer(window);

	engine.SetRenderDevice(window_renderer);

	auto sprite = engine.CreateSprite(Image("Media/tree_1.png"));

	sprite.SetPosition(DirectX::XMVectorSet(250, 250, 0, 0));

	while (window.IsOpen())
	{
		engine.ClearFrame(window_renderer);
		sprite.Draw(engine);
		window_renderer.RenderFrame();
		Window::DispatchWindowEvents();
	}

	return 0;
}